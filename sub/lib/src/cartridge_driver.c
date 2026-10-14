#include <nitro/os/interrupt.h>
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// ARM7 side of the Game Boy Advance slot (CTRDG): module detection, pull-out polling, access-cycle and
// lock helpers, the rumble pulse generator, and the PXI handshakes with the ARM9.

#pragma dollar_identifiers on

#define REG_EXMEMCNT     (*(volatile u16 *)0x04000204)
#define REG_POSTFLG      (*(vu16 *)0x04000300)
#define CTRDG_LOCK_WORD  ((void *)0x027FFFE8)
#define CTRDG_RUMBLE_REG (*(volatile u16 *)0x08001000)
#define VBLANK_COUNT     (*(vu32 *)0x027FFC3C)

// The cartridge module info the ARM9 shares at 0x027FFC30.
typedef struct CtrdgModuleInfo {
    u16 cartId; // 0xFFFF: no cartridge
    u8 unk_02;
    u8 unk_03;
    u8 unk_04;
    u8 isAgb : 1;
    u8 isPulledOut : 1;
    u8 unk_05_2 : 6;
    u8 unk_06;
    u8 unk_07;
    u32 gameCode;
} CtrdgModuleInfo;

#define CTRDG_MODULE_INFO ((CtrdgModuleInfo *)0x027FFC30)

// Game Boy Advance cartridge ROM header (0x08000000).
typedef struct CtrdgHeader {
    u32 entry;
    u16 logo[0x4E];
    u8 title[12];
    u32 gameCode;  // 0xAC
    u16 makerCode; // 0xB0
    u8 fixed;      // 0xB2, 0x96
    u8 unk_b3[0xBE - 0xB3];
    u16 cartId;
} CtrdgHeader;

#define CTRDG_HEADER ((CtrdgHeader *)0x08000000)

typedef struct CtrdgLock {
    u32 owned; // the ARM9 holds the cartridge lock
    OSIntrMode irq;
} CtrdgLock;

typedef struct CtrdgAccessCycle {
    u32 first;
    u32 second;
} CtrdgAccessCycle;

typedef struct CtrdgWork {
    u32 address;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0c;
    volatile u32 ready;
    u32 unk_14;
} CtrdgWork;

typedef struct CtrdgAlarm {
    u8 data[0x2C];
} CtrdgAlarm;

// Rumble pulse pattern: alternating on/off times, then a final delay.
typedef struct CtrdgVibPulse {
    u32 index;
    u32 count;
    u32 finalDelay;
    u32 on[6];
    u32 off[6];
    u32 active;
    u32 limit;
    u32 repeats;
} CtrdgVibPulse;

typedef void (*CtrdgPxiCallback)(u32 tag, u32 data, int error);

// The original TU addresses all its .bss through one base (lock_id); MWCC (-ipa file) pools a TU's .bss
// and orders it by an (unstable) heap sort on object size, so this declaration order reproduces the
// native layout lock_id, current_vib, isInitialized$3644, isCartridgePullOut$3768, skipCheck$3770,
// isInitialized$3633, ctw_sp, pulse_edge_alarm.
u16 lock_id;
static BOOL isCartridgePullOut$3768;
static u32 current_vib;
static BOOL skipCheck$3770;
CtrdgAlarm pulse_edge_alarm;
static CtrdgWork ctw_sp;
static BOOL isInitialized$3633;

extern u32 isFirstCheck$3769[];
extern u16 CTRDGi_EnableFlag[]; // [3]: lock id of the processor lock
extern u32 CTRDGi_Work;

s32 OS_GetLockID(void);
void OS_ReleaseLockID(u16 id);
void OS_LockCartridge(u16 id);
void OS_UnLockCartridge(u32 id);
void OS_UnlockCartridge(u32 id);
int OS_TryLockCartridge(u32 id);
u32 OS_ReadOwnerOfLockWord(void *lock);
void OS_SpinWait(u32 cycles);
void OS_Sleep(u32 msec);
void OS_Terminate(void);
void OS_InitTick(void);
void OS_InitAlarm(void);
void OS_CreateAlarm(void *alarm);
void OS_CancelAlarm(void *alarm);
void OS_SetAlarm(void *alarm, u64 tick, void (*handler)(void *), void *arg);
void PXI_Init(void);
void PXI_SetFifoRecvCallback(u32 tag, CtrdgPxiCallback callback);
int PXI_SendWordByFifo(u32 tag, u32 data, int error);
void VENEER_SVC_WaitByLoop(s32 count);
void SVC_CpuSet(const void *src, void *dest, u32 control);
void SND_BeginSleep(void);
void WVR_Shutdown(void);

void CTRDGi_InitCommon(void);
void __VENEER_SVC_CpuSet(const void *src, void *dest, u32 control);
BOOL CTRDG_IsPulledOut(void);
BOOL CTRDG_IsExisting(void);
void CTRDGi_ChangeLatestAccessCycle(CtrdgAccessCycle *cycle);
void MI_SetCartridgeRomCycle1st(u32 cycle);
void MI_SetCartridgeRomCycle2nd(u32 cycle);
int CTRDGi_LockByProcessor(u32 id, CtrdgLock *lock);
void CTRDGi_UnlockByProcessor(u32 id, CtrdgLock *lock);
void CTRDGi_SendtoPxi(u32 data);
void CTRDG_Init(void);
void CTRDGi_InitModuleInfo(void);
void CTRDGi_CallbackForInitModuleInfo(u32 tag, u32 data, int error);
void CTRDGi_CallbackForPulledOut(u32 tag, u32 data, int error);
void CTRDGi_CallbackForCTREx(u32 tag, u32 data, int error);
void CTRDG_VibPulseEdgeUpdate(CtrdgVibPulse *pulse);
void CTRDG_CheckPullOut_Polling(void);
void CTRDGi_CallbackForSetPhi(u32 tag, u32 data, int error);

#pragma force_active on

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

void CTRDGi_InitCommon(void) {
    u32 zero = 0;
    __VENEER_SVC_CpuSet(&zero, &CTRDGi_Work, 0x05000001);
    CTRDGi_EnableFlag[3] = OS_GetLockID();
}

void __VENEER_SVC_CpuSet(const void *src, void *dest, u32 control) {
    SVC_CpuSet(src, dest, control);
}

BOOL CTRDG_IsPulledOut(void) {
    volatile CtrdgModuleInfo *info = CTRDG_MODULE_INFO;
    if (info->cartId == 0xFFFF) {
        return FALSE;
    }
    if (!info->isPulledOut) {
        CTRDG_IsExisting();
    }
    return info->isPulledOut;
}

BOOL CTRDG_IsExisting(void) {
    BOOL ret = TRUE;
    CtrdgLock lock;
    CtrdgAccessCycle cycle;
    CtrdgModuleInfo *info = CTRDG_MODULE_INFO;
    u8 fixed;
    CtrdgHeader *header = CTRDG_HEADER;
    if (info->cartId == 0xFFFF) {
        return FALSE;
    }
    if (info->isPulledOut == 1) {
        return FALSE;
    }
    if (CTRDGi_LockByProcessor(CTRDGi_EnableFlag[3], &lock) == 0) {
        OS_RestoreInterrupts(lock.irq);
        return ret;
    }
    CTRDGi_ChangeLatestAccessCycle(&cycle);
    fixed = header->fixed;
    if ((fixed == 0x96 && info->cartId != header->cartId) || (fixed != 0x96 && info->cartId != *(u16 *)0x0801FFFE)
        || (info->gameCode != header->gameCode && info->isAgb)) {
        ret = FALSE;
        info->isPulledOut = 1;
    }
    MI_SetCartridgeRomCycle1st(cycle.first);
    MI_SetCartridgeRomCycle2nd(cycle.second);
    CTRDGi_UnlockByProcessor(CTRDGi_EnableFlag[3], &lock);
    return ret;
}

void CTRDGi_ChangeLatestAccessCycle(CtrdgAccessCycle *cycle) {
    cycle->first = (REG_EXMEMCNT & 12) >> 2;
    cycle->second = (REG_EXMEMCNT & 16) >> 4;
    MI_SetCartridgeRomCycle1st(3);
    MI_SetCartridgeRomCycle2nd(0);
}

void MI_SetCartridgeRomCycle1st(u32 cycle) {
    REG_EXMEMCNT = (REG_EXMEMCNT & ~12) | (cycle << 2);
}

void MI_SetCartridgeRomCycle2nd(u32 cycle) {
    REG_EXMEMCNT = (REG_EXMEMCNT & ~16) | (cycle << 4);
}

int CTRDGi_LockByProcessor(u32 id, CtrdgLock *lock) {
    lock->irq = OS_DisableInterrupts();
    lock->owned = OS_ReadOwnerOfLockWord(CTRDG_LOCK_WORD) & 128;
    if (lock->owned || !OS_TryLockCartridge(id)) {
        return 1;
    }
    return 0;
}

void CTRDGi_UnlockByProcessor(u32 id, CtrdgLock *lock) {
    if (!lock->owned) {
        OS_UnLockCartridge(id);
    }
    OS_RestoreInterrupts(lock->irq);
}

void CTRDGi_SendtoPxi(u32 data) {
    while (PXI_SendWordByFifo(13, data, FALSE)) {
        VENEER_SVC_WaitByLoop(1);
    }
}

#pragma dont_inline reset

void CTRDG_Init(void) {
    int id;
    OS_InitTick();
    OS_InitAlarm();
    OS_CreateAlarm(&pulse_edge_alarm);
    if (isInitialized$3633) {
        return;
    }
    isInitialized$3633 = 1;
    CTRDGi_InitCommon();
    id = OS_GetLockID();
    if (id == -3) {
        return;
    }
    lock_id = id;
    PXI_Init();
    PXI_SetFifoRecvCallback(13, CTRDGi_CallbackForInitModuleInfo);
    CTRDGi_InitModuleInfo();
    PXI_SetFifoRecvCallback(13, CTRDGi_CallbackForPulledOut);
    PXI_SetFifoRecvCallback(16, CTRDGi_CallbackForCTREx);
    PXI_SetFifoRecvCallback(17, CTRDGi_CallbackForSetPhi);
}

void CTRDGi_InitModuleInfo(void) {
    OSIrqMask mask;
    BOOL ime;
    CtrdgModuleInfo *info;
    CtrdgHeader *hdr;
    CtrdgHeader *data;
    BOOL ok;
    u16 id;
    u16 m;
    const u16 *rom;
    int i;
    u16 post;
    static BOOL isInitialized;
    if (isInitialized) {
        return;
    }
    isInitialized = TRUE;
    post = REG_POSTFLG;
    if (!(post & 1)) {
        return;
    }
    mask = OS_SetIrqMask(OS_IE_FIFO_RECV);
    ime = OS_EnableIrq();
    while (ctw_sp.ready != 1) {
        VENEER_SVC_WaitByLoop(0x100);
    }
    data = (CtrdgHeader *)((((ctw_sp.address & 0x01ffffc0) >> 6) << 5) + 0x02000000);
    hdr = CTRDG_HEADER;
    rom = hdr->logo;
    ok = TRUE;
    id = (u16)OS_GetLockID();
    OS_LockCartridge(id);
    for (i = 0; i < 0x4e; i++) {
        m = 0xffff;
        if (i == 0x4c) {
            m ^= 0x84;
        } else if (i == 0x4d) {
            m ^= 3;
        }
        if ((m & data->logo[i]) != rom[i]) {
            ok = FALSE;
            break;
        }
    }
    OS_UnLockCartridge(id);
    OS_ReleaseLockID(id);
    info = CTRDG_MODULE_INFO;
    info->isAgb = ok;
    CTRDGi_SendtoPxi(1);
    (void)OS_RestoreIrq(ime);
    OS_SetIrqMask(mask);
}

void CTRDGi_CallbackForInitModuleInfo(u32 tag, u32 data, int error) {
    if ((data & 63) == 1) {
        ctw_sp.address = data;
        ctw_sp.ready = 1;
    } else {
        OS_Terminate();
    }
}

void CTRDGi_CallbackForPulledOut(u32 tag, u32 data, int error) {
    if ((data & 63) == 2) {
        CTRDG_VibPulseEdgeUpdate(0);
        SND_BeginSleep();
        WVR_Shutdown();
        OS_Terminate();
    } else {
        OS_Terminate();
    }
}

void CTRDGi_CallbackForCTREx(u32 tag, u32 data, int error) {
    CTRDG_VibPulseEdgeUpdate((void *)data);
}

void CTRDG_VibPulseEdgeUpdate(CtrdgVibPulse *p) {
    u32 own;
    if (p && p->index == 0) {
        p->repeats++;
        if (p->limit && p->repeats > p->limit) {
            p = 0;
        }
    }
    if (!p || !p->active) {
        OSIntrMode irq = OS_DisableInterrupts();
        if (current_vib == 2) {
            int done = 0;
            while (!done) {
                own = OS_ReadOwnerOfLockWord(CTRDG_LOCK_WORD) & 128;
                if (own || OS_TryLockCartridge(lock_id) == 0) {
                    current_vib = 0;
                    done = 1;
                    CTRDG_RUMBLE_REG = 0;
                    if (!own) {
                        OS_UnlockCartridge(lock_id);
                    }
                } else {
                    OS_SpinWait(0x80e8);
                }
            }
        }
        OS_CancelAlarm(&pulse_edge_alarm);
        OS_RestoreInterrupts(irq);
    } else if (p) {
        own = OS_ReadOwnerOfLockWord(CTRDG_LOCK_WORD) & 128;
        if (own || OS_TryLockCartridge(lock_id) == 0) {
            if (p->index == p->count) {
                current_vib = 0;
                CTRDG_RUMBLE_REG = 0;
                OS_SetAlarm(&pulse_edge_alarm, p->finalDelay, (void (*)(void *))CTRDG_VibPulseEdgeUpdate, p);
                p->index = 0;
            } else if (p->index & 1) {
                current_vib = 0;
                CTRDG_RUMBLE_REG = 0;
                OS_SetAlarm(&pulse_edge_alarm, p->off[p->index >> 1], (void (*)(void *))CTRDG_VibPulseEdgeUpdate, p);
                p->index++;
            } else {
                current_vib = 2;
                CTRDG_RUMBLE_REG = 2;
                OS_SetAlarm(&pulse_edge_alarm, p->on[p->index >> 1], (void (*)(void *))CTRDG_VibPulseEdgeUpdate, p);
                p->index++;
            }
            if (!own) {
                OS_UnlockCartridge(lock_id);
            }
        } else {
            OS_SetAlarm(&pulse_edge_alarm, 523, (void (*)(void *))CTRDG_VibPulseEdgeUpdate, p);
        }
    }
}

void CTRDG_CheckPullOut_Polling(void) {
    u32 next = isFirstCheck$3769[1];
    if (next == 0xffffffff) {
        isFirstCheck$3769[1] = VBLANK_COUNT + 10;
        return;
    }
    if (skipCheck$3770 || isCartridgePullOut$3768) {
        return;
    }
    if (VBLANK_COUNT < next) {
        return;
    }
    isFirstCheck$3769[1] = VBLANK_COUNT + 10;
    isCartridgePullOut$3768 = CTRDG_IsPulledOut();
    if (!CTRDG_IsExisting()) {
        if (isFirstCheck$3769[0]) {
            skipCheck$3770 = TRUE;
            return;
        }
        isCartridgePullOut$3768 = TRUE;
    }
    isFirstCheck$3769[0] = 0;
    if (isCartridgePullOut$3768) {
        while (PXI_SendWordByFifo(13, 17, FALSE)) {
            OS_Sleep(100);
        }
    }
}

#pragma dont_inline on

void CTRDGi_CallbackForSetPhi(u32 tag, u32 data, int error) {
    if ((data & 63) == 3) {
        {
            u32 reg = REG_EXMEMCNT;
            data = (data & 0x1FFFFC0) >> 6;
            reg &= ~96;
            REG_EXMEMCNT = reg | (data << 5);
        }
        while (PXI_SendWordByFifo(17, 18, FALSE)) {
            VENEER_SVC_WaitByLoop(1);
        }
    } else {
        OS_Terminate();
    }
}

#pragma dont_inline reset
