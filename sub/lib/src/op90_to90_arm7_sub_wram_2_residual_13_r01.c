#pragma force_active on
// CTRDG ARM7 work: CTRDG_Init .. CTRDG_CheckPullOut_Polling as one translation unit.
// The original TU addresses all its .bss through one base (lock_id); MWCC (-ipa file) pools a
// TU's .bss and orders it by an (unstable) heap sort on object size, so the declaration order
// below reproduces the native layout lock_id, current_vib, isInitialized$3644, isCartridgePullOut$3768,
// skipCheck$3770, isInitialized$3633, ctw_sp, pulse_edge_alarm (0x03809450..0x038094AC).
// CTRDGi_InitModuleInfo's flag must be a function-local static: that is what lets MWCC schedule the
// POWCNT ldrh before the flag store like the native code.
#include <nitro/types.h>
#include <nitro/os/interrupt.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma dollar_identifiers on
typedef struct {
    u32 address;
    u32 x04;
    u32 x08;
    u32 x0c;
    volatile u32 ready;
    u32 x14;
} CtrdgWork;
typedef struct {
    u8 pad[5];
    u8 isAgb : 1;
    u8 rest : 7;
} CtrdgModuleInfo;
typedef struct {
    u32 entry;
    u16 logo[0x4e];
} CartHeader;
typedef struct { u8 data[0x2c]; } CtrdgAlarm;
typedef struct{u32 index,count,finalDelay,on[6],off[6],active,limit,repeats;} Pulse;
u16 lock_id;
static BOOL isCartridgePullOut$3768;
static u32 current_vib;
static BOOL skipCheck$3770;
CtrdgAlarm pulse_edge_alarm;
static CtrdgWork ctw_sp;
static BOOL isInitialized$3633;
extern u32 isFirstCheck$3769[];
extern void VENEER_SVC_WaitByLoop(s32 count);
extern s32 OS_GetLockID(void);
extern void OS_LockCartridge(u16 id);
extern void OS_UnLockCartridge(u16 id);
extern void OS_ReleaseLockID(u16 id);
extern void CTRDGi_SendtoPxi(u32 data);
extern void OS_InitTick(void),OS_InitAlarm(void),OS_CreateAlarm(void*),CTRDGi_InitCommon(void),PXI_Init(void);
typedef void(*CB)(u32,u32,int);extern void PXI_SetFifoRecvCallback(u32,CB);
extern void CTRDGi_CallbackForSetPhi(u32,u32,int);
extern void OS_Terminate(void);
extern void SND_BeginSleep(void),WVR_Shutdown(void);
extern u32 OS_ReadOwnerOfLockWord(void*);extern int OS_TryLockCartridge(u32);extern void OS_UnlockCartridge(u32),OS_SpinWait(u32),OS_CancelAlarm(void*);extern void OS_SetAlarm(void*,u64,void(*)(void*),void*);
extern BOOL CTRDG_IsPulledOut(void);
extern BOOL CTRDG_IsExisting(void);
extern void OS_Sleep(u32);
extern int PXI_SendWordByFifo(int, u32, BOOL);
void CTRDG_Init(void);
void CTRDGi_InitModuleInfo(void);
void CTRDGi_CallbackForInitModuleInfo(u32 tag,u32 data,int error);
void CTRDGi_CallbackForPulledOut(u32 tag,u32 data,int error);
void CTRDGi_CallbackForCTREx(u32 tag,u32 data,int error);
void CTRDG_VibPulseEdgeUpdate(Pulse*p);
void CTRDG_CheckPullOut_Polling(void);
// @fn CTRDG_Init
void CTRDG_Init(void){int id;OS_InitTick();OS_InitAlarm();OS_CreateAlarm(&pulse_edge_alarm);if(isInitialized$3633)return;isInitialized$3633=1;CTRDGi_InitCommon();id=OS_GetLockID();if(id==-3)return;lock_id=id;PXI_Init();PXI_SetFifoRecvCallback(13,CTRDGi_CallbackForInitModuleInfo);CTRDGi_InitModuleInfo();PXI_SetFifoRecvCallback(13,CTRDGi_CallbackForPulledOut);PXI_SetFifoRecvCallback(16,CTRDGi_CallbackForCTREx);PXI_SetFifoRecvCallback(17,CTRDGi_CallbackForSetPhi);}

// @fn CTRDGi_InitModuleInfo
void CTRDGi_InitModuleInfo(void)
{
    OSIrqMask mask;
    BOOL ime;
    CtrdgModuleInfo *info;
    CartHeader *hdr;
    CartHeader *data;
    BOOL ok;
    u16 id;
    u16 m;
    const u16 *rom;
    int i;
    u16 post;
    static BOOL isInitialized;
    if (isInitialized)
        return;
    isInitialized = TRUE;
    post = *(vu16 *)0x04000300;
    if (!(post & 1))
        return;
    mask = OS_SetIrqMask(0x40000);
    ime = OS_EnableIrq();
    while (ctw_sp.ready != 1)
        VENEER_SVC_WaitByLoop(0x100);
    data = (CartHeader *)((((ctw_sp.address & 0x01ffffc0) >> 6) << 5) + 0x02000000);
    hdr = (CartHeader *)0x08000000;
    rom = hdr->logo;
    ok = TRUE;
    id = (u16)OS_GetLockID();
    OS_LockCartridge(id);
    for (i = 0; i < 0x4e; i++) {
        m = 0xffff;
        if (i == 0x4c)
            m ^= 0x84;
        else if (i == 0x4d)
            m ^= 3;
        if ((m & data->logo[i]) != rom[i]) {
            ok = FALSE;
            break;
        }
    }
    OS_UnLockCartridge(id);
    OS_ReleaseLockID(id);
    info = (CtrdgModuleInfo *)0x027ffc30;
    info->isAgb = ok;
    CTRDGi_SendtoPxi(1);
    (void)OS_RestoreIrq(ime);
    OS_SetIrqMask(mask);
}

// @fn CTRDGi_CallbackForInitModuleInfo
void CTRDGi_CallbackForInitModuleInfo(u32 tag,u32 data,int error){if((data&63)==1){ctw_sp.address=data;ctw_sp.ready=1;}else OS_Terminate();}

// @fn CTRDGi_CallbackForPulledOut
void CTRDGi_CallbackForPulledOut(u32 tag,u32 data,int error){if((data&63)==2){CTRDG_VibPulseEdgeUpdate(0);SND_BeginSleep();WVR_Shutdown();OS_Terminate();}else OS_Terminate();}

// @fn CTRDGi_CallbackForCTREx
void CTRDGi_CallbackForCTREx(u32 tag,u32 data,int error){CTRDG_VibPulseEdgeUpdate((void*)data);}

// @fn CTRDG_VibPulseEdgeUpdate
void CTRDG_VibPulseEdgeUpdate(Pulse*p){u32 own;if(p&&p->index==0){p->repeats++;if(p->limit&&p->repeats>p->limit)p=0;}if(!p||!p->active){OSIntrMode irq=OS_DisableInterrupts();if(current_vib==2){int done=0;while(!done){own=OS_ReadOwnerOfLockWord((void*)0x27fffe8)&128;if(own||OS_TryLockCartridge(lock_id)==0){current_vib=0;done=1;*(volatile u16*)0x8001000=0;if(!own)OS_UnlockCartridge(lock_id);}else OS_SpinWait(0x80e8);}}OS_CancelAlarm(&pulse_edge_alarm);OS_RestoreInterrupts(irq);}else if(p){own=OS_ReadOwnerOfLockWord((void*)0x27fffe8)&128;if(own||OS_TryLockCartridge(lock_id)==0){if(p->index==p->count){current_vib=0;*(volatile u16*)0x8001000=0;OS_SetAlarm(&pulse_edge_alarm,p->finalDelay,(void(*)(void*))CTRDG_VibPulseEdgeUpdate,p);p->index=0;}else if(p->index&1){current_vib=0;*(volatile u16*)0x8001000=0;OS_SetAlarm(&pulse_edge_alarm,p->off[p->index>>1],(void(*)(void*))CTRDG_VibPulseEdgeUpdate,p);p->index++;}else{current_vib=2;*(volatile u16*)0x8001000=2;OS_SetAlarm(&pulse_edge_alarm,p->on[p->index>>1],(void(*)(void*))CTRDG_VibPulseEdgeUpdate,p);p->index++;}if(!own)OS_UnlockCartridge(lock_id);}else OS_SetAlarm(&pulse_edge_alarm,523,(void(*)(void*))CTRDG_VibPulseEdgeUpdate,p);}}

// @fn CTRDG_CheckPullOut_Polling
void CTRDG_CheckPullOut_Polling(void)
{
    u32 next = isFirstCheck$3769[1];
    if (next == 0xffffffff) {
        isFirstCheck$3769[1] = *(vu32 *)0x027ffc3c + 10;
        return;
    }
    if (skipCheck$3770 || isCartridgePullOut$3768)
        return;
    if (*(vu32 *)0x027ffc3c < next)
        return;
    isFirstCheck$3769[1] = *(vu32 *)0x027ffc3c + 10;
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
        while (PXI_SendWordByFifo(13, 17, FALSE))
            OS_Sleep(100);
    }
}

