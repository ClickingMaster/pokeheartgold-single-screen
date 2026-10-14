#pragma force_active on
#include <nitro/types.h>
// The SDK headers below expect <nitro/types.h> to be included first.
#include <nitro/ctrdg/ARM7/vibrate.h>
#include <nitro/hw/mmap_shared.h>
#include <nitro/mi/dma.h>
#include <nitro/os/spinLock.h>
#include <nitro/os/system.h>
#include <nitro/os/terminate_proc.h>
#include <nitro/os/thread.h>
#include <nitro/os/tick.h>
#include <nitro/pxi.h>
#include <nitro/pxi/fifo.h>
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#pragma dollar_identifiers on

#define REG_CARD_SPI_CNT  (*(vu16 *)0x040001a0)
#define REG_CARD_SPI_DATA (*(vu16 *)0x040001a2)
#define REG_CARD_ROM_CNT  (*(vu32 *)0x040001a4)
#define REG_CARD_ROM_DATA (*(vu32 *)0x04100010)
#define REG_IF            (*(vu32 *)0x04000214)

// Values from nitro/card/common.h and nitro/card/rom.h. Those headers cannot be included here: their
// inline functions are written for the ARM9 CARDiCommon, which is one word shorter than the ARM7 one.
#define CARD_THREAD_PRIORITY_DEFAULT 4

#define CARD_PXI_COMMAND_MASK       0x3f
#define CARD_PXI_COMMAND_PULLED_OUT 0x0011
#define CARD_PXI_COMMAND_TERMINATE  0x0001

#define CARD_RESULT_SUCCESS       0
#define CARD_RESULT_FAILURE       1
#define CARD_RESULT_INVALID_PARAM 2
#define CARD_RESULT_UNSUPPORTED   3
#define CARD_RESULT_TIMEOUT       4
#define CARD_RESULT_NO_RESPONSE   6

#define CARD_REQ_INIT                0
#define CARD_REQ_ACK                 1
#define CARD_REQ_IDENTIFY            2
#define CARD_REQ_READ_ID             3
#define CARD_REQ_READ_ROM            4
#define CARD_REQ_WRITE_ROM           5
#define CARD_REQ_READ_BACKUP         6
#define CARD_REQ_WRITE_BACKUP        7
#define CARD_REQ_PROGRAM_BACKUP      8
#define CARD_REQ_VERIFY_BACKUP       9
#define CARD_REQ_ERASE_PAGE_BACKUP   10
#define CARD_REQ_ERASE_SECTOR_BACKUP 11
#define CARD_REQ_ERASE_CHIP_BACKUP   12

#define CARD_STAT_INIT   (1 << 0)
#define CARD_STAT_BUSY   (1 << 2)
#define CARD_STAT_TASK   (1 << 3)
#define CARD_STAT_RECV   (1 << 4)
#define CARD_STAT_CANCEL (1 << 6)

#define MROMOP_G_READ_ID   0xB8000000
#define CARD_LATENCY1_MASK 0x00001FFF

// Backup device description and request filled in by the ARM9. Same layout as CARDiCommandArg in
// nitro/card/common.h, plus a sub-sector size and its erase timings.
typedef struct CardCommandArg {
    u32 result;
    u32 type;
    u32 id;
    u32 src;
    u32 dest;
    u32 len;
    u32 total_size;
    u32 sect_size;
    u32 subsect_size;
    u32 page_size;
    u32 addr_width;
    u32 program_page;
    u32 write_page;
    u32 write_page_total;
    u32 erase_chip;
    u32 erase_chip_total;
    u32 erase_sector;
    u32 erase_sector_total;
    u32 erase_subsector;
    u32 erase_subsector_total;
    u32 erase_page;
    u8 initial_status;
    u8 padding1[3];
    u32 caps;
} CardCommandArg;

// ARM7 layout of cardi_common: CARDiCommon in nitro/card/common.h with one more word (recv_count).
typedef struct CardCommon {
    CardCommandArg *cmd;
    u32 command;
    u32 recv_count;
    volatile s32 lock_owner;
    s32 lock_ref;
    OSThreadQueue lock_queue;
    u32 lock_target;
    u32 src;
    u32 dst;
    u32 len;
    u32 dma;
    u32 req_type;
    s32 req_retry;
    u32 req_mode;
    void (*callback)(void *);
    void *callback_arg;
    void (*task_func)(struct CardCommon *);
    OSThread thread;
    OSThread *cur_th;
    u32 priority;
    OSThreadQueue busy_q;
    volatile u32 flag;
} CardCommon;

// State of the SPI transfer run by CARDi_CommArray.
typedef struct CardSpiParam {
    u32 count;
    const u8 *src;
    u8 *dst;
    u32 verified;
} CardSpiParam;

typedef struct CardSpiStatus {
    u32 checked;
    CardSpiParam param;
} CardSpiStatus;

typedef struct CardSpiDeselect {
    u8 delay;
    u8 padding[3];
    u32 pending;
} CardSpiDeselect;

typedef struct CardRomStat {
    void (*read_func)(void *);
    u32 ctrl;
    u8 *cache_page;
    u32 dummy[5];
    u8 cache_buf[512];
} CardRomStat;

extern CardCommon cardi_common;
extern CardSpiParam cardi_param;
// The same storage as cardi_param, preceded by the "status register checked" flag; the task
// thread's stack ends right below it.
extern CardSpiStatus status_checked$3825;
extern CardSpiDeselect _03806AF4;
extern u8 arg$3983[];
extern u8 buf$3759[];
extern u8 arg$3806[];
extern const u8 *cardi_rom_header_addr;
extern CardRomStat rom_stat;
extern u32 cardi_rom_base;
extern BOOL CARDi_EnableFlag;
extern u32 skipCheck$3668[];
extern u32 nextCount$3668[];

void SND_BeginSleep(void);
void WVR_Shutdown(void);
void VENEER_SVC_WaitByLoop(s32 count);

void FS_Init(void);
void CARDi_InitCommon(void);
int CARD_SetThreadPriority(int priority);
void *CARD_GetRomHeader(void);
void CARDi_CommandEnd(s32 initial, s32 limit);
u8 CARDi_CommandReadStatus(void);
BOOL CARDi_WaitPrevCommand(void);
void CARDi_CommArray(const void *src, void *dst, u32 size, void (*core)(CardSpiParam *));
void CARDi_CommReadCore(CardSpiParam *param);
void CARDi_CommWriteCore(CardSpiParam *param);
void CARDi_CommVerifyCore(CardSpiParam *param);
void CARDi_WriteEnable(void);
void CARDi_SendSpiAddressingCommand(u32 address, u32 command);
void CARDi_InitStatusRegister(void);
void CARDi_ReadBackupCore(u32 address, void *dst, u32 size);
void CARDi_ProgramBackupCore(u32 address, const void *src, u32 size);
void CARDi_WriteBackupCore(u32 address, const void *src, u32 size);
void CARDi_VerifyBackupCore(u32 address, const void *src, u32 size);
void CARDi_EraseBackupSectorCore(u32 address, u32 size);
void CARDi_EraseBackupSubSectorCore(u32 address, u32 size);
void CARDi_EraseChipCore(void);
void CARDi_SetWriteProtectCore(u8 protection);
void CARDi_SetRomOp(u32 first, u32 second);
void CARDi_ReadEnd(void);
u32 CARDi_GetRomFlag(u32 mask);
void CARDi_ReadCard(void *argument);
u32 CARDi_ReadRomIDCore(void);
u32 CARDi_ReadRomID(void);
void CARDi_WaitTask(CardCommon *common, void (*callback)(void *), void *argument);
void CARD_Init(void);
void CARDi_OnFifoRecv(PXIFifoTag tag, u32 data, BOOL err);
void CARDi_TaskThread(void *argument);
void CARD_InitPulledOutCallback(void);
void CARDi_CallbackForPulledOut(PXIFifoTag tag, u32 data, BOOL err);
BOOL CARD_IsPulledOut(void);
BOOL CARD_CompareCardID(void);
BOOL CARD_IsCardIreqLo(void);
void CARD_CheckPullOut_Polling(void);

static inline BOOL IsWmBoot(void) {
    return *(vu16 *)HW_WM_BOOT_BUF == 2;
}

// Checks the backup chip's busy bit (status register bit 0).
static inline BOOL IsBackupWriteDone(void) {
    return !(CARDi_CommandReadStatus() & 1);
}

void FS_Init(void) {
    CARD_Init();
}

#pragma opt_propagation off

void CARDi_InitCommon(void) {
    CardCommon *p = &cardi_common;
    p->lock_owner = OS_LOCK_ID_ERROR;
    *(vu32 *)&p->priority = CARD_THREAD_PRIORITY_DEFAULT; // a volatile store keeps it next to lock_owner
    p->lock_ref = 0;
    p->lock_target = 0;
    p->cmd = NULL;
    p->recv_count = 0;
    p->lock_queue.tail = NULL;
    p->lock_queue.head = NULL;
    p->busy_q.tail = NULL;
    p->busy_q.head = NULL;
    OS_CreateThread(&p->thread, CARDi_TaskThread, NULL, &status_checked$3825, 0x400, p->priority);
    OS_WakeupThreadDirect(&p->thread);
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_FS, CARDi_OnFifoRecv);
    if (!IsWmBoot()) {
        CARDi_EnableFlag = TRUE;
    }
}

// -ipa file would otherwise inline the small functions below into their callers in this file.
#pragma dont_inline on

int CARD_SetThreadPriority(int priority) {
    CardCommon *p = &cardi_common;
    OSIntrMode irq = OS_DisableInterrupts();
    int old = p->priority;
    p->priority = priority;
    OS_SetThreadPriority(&p->thread, priority);
    OS_RestoreInterrupts(irq);
    return old;
}

#pragma opt_propagation reset

void *CARD_GetRomHeader(void) {
    return (void *)0x027ffa80;
}

#pragma dont_inline reset

void CARDi_CommandEnd(s32 initial, s32 limit) {
    if ((u32)initial + (u32)limit) {
        if (initial) {
            OS_Sleep(initial);
        }
        if (limit) {
            s32 remain = (s32)((u32)limit - (u32)initial);
            while (!IsBackupWriteDone() && remain > 0) {
                s32 delay = remain < 5 ? remain : 5;
                OS_Sleep(delay);
                remain -= delay;
            }
        }
        {
            BOOL ready = !(CARDi_CommandReadStatus() & 1);
            if (!ready) {
                cardi_common.cmd->result = CARD_RESULT_TIMEOUT;
            }
        }
    }
}

#pragma dont_inline on

u8 CARDi_CommandReadStatus(void) {
    u8 result;
    status_checked$3825.param.count = 2;
    CARDi_CommArray(arg$3983, NULL, 1, CARDi_CommWriteCore);
    CARDi_CommArray(NULL, &result, 1, CARDi_CommReadCore);
    return result;
}

BOOL CARDi_WaitPrevCommand(void) {
    CardCommandArg *cmd;
    CARDi_CommandEnd(0, 50);
    cmd = cardi_common.cmd;
    if (cmd->result == CARD_RESULT_TIMEOUT) {
        cmd->result = CARD_RESULT_NO_RESPONSE;
        return FALSE;
    }
    return TRUE;
}

#pragma opt_propagation off

void CARDi_CommArray(const void *src, void *dst, u32 size, void (*core)(CardSpiParam *)) {
    CardSpiParam *p = &cardi_param;
    p->src = src;
    p->dst = dst;
    REG_CARD_SPI_CNT = REG_MI_MCCNT0_E_MASK | REG_MI_MCCNT0_SEL_MASK | REG_MI_MCCNT0_MODE_MASK;
    while (size) {
        if (_03806AF4.pending && (((s32)cardi_common.cmd->type >> 16) & 0xff) == 0xff) {
            u64 start = OS_GetTick();
            while (((OS_GetTick() - start) * 64000) / 33514 < (s32)_03806AF4.delay) {}
            REG_CARD_SPI_CNT = REG_MI_MCCNT0_E_MASK | REG_MI_MCCNT0_SEL_MASK | REG_MI_MCCNT0_MODE_MASK | (2 << REG_MI_MCCNT0_BAUDRATE_SHIFT);
            while (REG_CARD_SPI_CNT & REG_MI_MCCNT0_BUSY_MASK) {}
            REG_CARD_SPI_DATA = 0;
            while (REG_CARD_SPI_CNT & REG_MI_MCCNT0_BUSY_MASK) {}
            {
                volatile u16 discard = REG_CARD_SPI_DATA;
            }
            _03806AF4.pending = 0;
            start = OS_GetTick();
            while (((OS_GetTick() - start) * 64000) / 33514 < (s32)_03806AF4.delay) {}
        }
        if (--p->count == 0) {
            REG_CARD_SPI_CNT = REG_MI_MCCNT0_E_MASK | REG_MI_MCCNT0_SEL_MASK;
            _03806AF4.pending = 1;
        } else {
            REG_CARD_SPI_CNT = REG_MI_MCCNT0_E_MASK | REG_MI_MCCNT0_SEL_MASK | REG_MI_MCCNT0_MODE_MASK;
        }
        while (REG_CARD_SPI_CNT & REG_MI_MCCNT0_BUSY_MASK) {}
        core(p);
        if (!p->count) {
            break;
        }
        size--;
    }
    if (!p->count) {
        REG_CARD_SPI_CNT = 0;
    }
}

#pragma opt_propagation reset

void CARDi_CommReadCore(CardSpiParam *param) {
    REG_CARD_SPI_DATA = 0;
    while (REG_CARD_SPI_CNT & REG_MI_MCCNT0_BUSY_MASK) {}
    *param->dst = (u8)REG_CARD_SPI_DATA;
    param->dst++;
}

void CARDi_CommWriteCore(CardSpiParam *param) {
    volatile u16 discard;
    REG_CARD_SPI_DATA = *param->src;
    param->src++;
    while (REG_CARD_SPI_CNT & REG_MI_MCCNT0_BUSY_MASK) {}
    discard = REG_CARD_SPI_DATA;
}

void CARDi_CommVerifyCore(CardSpiParam *param) {
    REG_CARD_SPI_DATA = 0;
    while (REG_CARD_SPI_CNT & REG_MI_MCCNT0_BUSY_MASK) {}
    if ((u8)REG_CARD_SPI_DATA != *param->src) {
        param->verified = FALSE;
        if (param->count > 1) {
            param->count = 1;
        }
    }
    param->src++;
}

void CARDi_WriteEnable(void) {
    status_checked$3825.param.count = 1;
    CARDi_CommArray(buf$3759, NULL, 1, CARDi_CommWriteCore);
}

void CARDi_SendSpiAddressingCommand(u32 address, u32 command) {
    u32 packed;
    u32 width = cardi_common.cmd->addr_width;
    switch (width) {
    case 1:
        packed = command | ((address >> 5) & 8) | ((address & 0xff) << 8);
        break;
    case 2:
        packed = command | (address & 0xff00) | ((address & 0xff) << 16);
        break;
    case 3:
        packed = command | ((address >> 8) & 0xff00) | ((address & 0xff00) << 8) | (address << 24);
        break;
    }
    CARDi_CommArray(&packed, NULL, width + 1, CARDi_CommWriteCore);
}

void CARDi_InitStatusRegister(void) {
    u8 protection = cardi_common.cmd->initial_status;
    if (protection != 0xff && !status_checked$3825.checked) {
        if (protection != CARDi_CommandReadStatus()) {
            CARDi_SetWriteProtectCore(protection);
        }
        status_checked$3825.checked = TRUE;
    }
}

void CARDi_ReadBackupCore(u32 address, void *dst, u32 size) {
    if (CARDi_WaitPrevCommand()) {
        status_checked$3825.param.count = cardi_common.cmd->addr_width + 1 + size;
        CARDi_SendSpiAddressingCommand(address, 3);
        CARDi_CommArray(NULL, dst, size, CARDi_CommReadCore);
    }
}

void CARDi_ProgramBackupCore(u32 address, const void *src, u32 size) {
    if (CARDi_WaitPrevCommand()) {
        u32 mask;
        CardCommandArg *cmd = cardi_common.cmd;
        u32 page = cmd->page_size;
        mask = page - 1;
        while (size) {
            u32 chunk = page - (address & mask);
            if (chunk > size) {
                chunk = size;
            }
            CARDi_WriteEnable();
            status_checked$3825.param.count = cmd->addr_width + 1 + chunk;
            CARDi_SendSpiAddressingCommand(address, 2);
            CARDi_CommArray(src, NULL, chunk, CARDi_CommWriteCore);
            CARDi_CommandEnd((s32)cmd->program_page, 0);
            if (cmd->result) {
                break;
            }
            src = (const u8 *)src + chunk;
            address += chunk;
            size -= chunk;
        }
    }
}

void CARDi_WriteBackupCore(u32 address, const void *src, u32 size) {
    if (CARDi_WaitPrevCommand()) {
        u32 mask;
        CardCommandArg *cmd = cardi_common.cmd;
        u32 page = cmd->page_size;
        mask = page - 1;
        while (size) {
            u32 chunk = page - (address & mask);
            if (chunk > size) {
                chunk = size;
            }
            CARDi_WriteEnable();
            status_checked$3825.param.count = cmd->addr_width + 1 + chunk;
            CARDi_SendSpiAddressingCommand(address, 10);
            CARDi_CommArray(src, NULL, chunk, CARDi_CommWriteCore);
            CARDi_CommandEnd((s32)cmd->write_page, (s32)cmd->write_page_total);
            if (cmd->result) {
                break;
            }
            src = (const u8 *)src + chunk;
            address += chunk;
            size -= chunk;
        }
    }
}

void CARDi_VerifyBackupCore(u32 address, const void *src, u32 size) {
    if (CARDi_WaitPrevCommand()) {
        CardCommandArg *cmd = cardi_common.cmd;
        status_checked$3825.param.verified = TRUE;
        status_checked$3825.param.count = cmd->addr_width + 1 + size;
        CARDi_SendSpiAddressingCommand(address, 3);
        CARDi_CommArray(src, NULL, size, CARDi_CommVerifyCore);
        if (!cmd->result && !status_checked$3825.param.verified) {
            cmd->result = CARD_RESULT_FAILURE;
        }
    }
}

void CARDi_EraseBackupSectorCore(u32 address, u32 size) {
    CardCommandArg *cmd = cardi_common.cmd;
    u32 sector = cmd->sect_size;
    if ((sector - 1) & (address | size)) {
        cmd->result = CARD_RESULT_INVALID_PARAM;
        return;
    }
    if (CARDi_WaitPrevCommand()) {
        while (size) {
            CARDi_WriteEnable();
            status_checked$3825.param.count = cmd->addr_width + 1;
            CARDi_SendSpiAddressingCommand(address, 0xd8);
            CARDi_CommandEnd((s32)cmd->erase_sector, (s32)cmd->erase_sector_total);
            if (cmd->result) {
                break;
            }
            address += sector;
            size -= sector;
        }
    }
}

void CARDi_EraseBackupSubSectorCore(u32 address, u32 size) {
    CardCommandArg *cmd = cardi_common.cmd;
    u32 sector = cmd->subsect_size;
    if ((sector - 1) & (address | size)) {
        cmd->result = CARD_RESULT_INVALID_PARAM;
        return;
    }
    if (CARDi_WaitPrevCommand()) {
        while (size) {
            CARDi_WriteEnable();
            status_checked$3825.param.count = cmd->addr_width + 1;
            CARDi_SendSpiAddressingCommand(address, 0x20);
            CARDi_CommandEnd((s32)cmd->erase_subsector, (s32)cmd->erase_subsector_total);
            if (cmd->result) {
                break;
            }
            address += sector;
            size -= sector;
        }
    }
}

void CARDi_EraseChipCore(void) {
    if (CARDi_WaitPrevCommand()) {
        CardCommandArg *cmd = cardi_common.cmd;
        CARDi_WriteEnable();
        status_checked$3825.param.count = 1;
        CARDi_CommArray(arg$3806, NULL, 1, CARDi_CommWriteCore);
        CARDi_CommandEnd((s32)cmd->erase_chip, (s32)cmd->erase_chip_total);
    }
}

void CARDi_SetWriteProtectCore(u8 protection) {
    if (CARDi_WaitPrevCommand()) {
        u8 command[2];
        CardCommandArg *cmd = cardi_common.cmd;
        int retries = 10;
        command[1] = protection;
        command[0] = 1;
        do {
            CARDi_WriteEnable();
            status_checked$3825.param.count = 2;
            CARDi_CommArray(command, NULL, 2, CARDi_CommWriteCore);
            CARDi_CommandEnd(5, 0);
            if (cmd->result != CARD_RESULT_TIMEOUT) {
                break;
            }
        } while (--retries > 0);
    }
}

void CARDi_SetRomOp(u32 first, u32 second) {
    vu8 *p = (vu8 *)0x040001a1;
    while (REG_CARD_ROM_CNT & REG_MI_MCCNT1_START_MASK) {}
    p[0] = 0xc0;
    p[7] = (u8)(first >> 24);
    p[8] = (u8)(first >> 16);
    p[9] = (u8)(first >> 8);
    p[10] = (u8)first;
    p[11] = (u8)(second >> 24);
    p[12] = (u8)(second >> 16);
    p[13] = (u8)(second >> 8);
    p[14] = (u8)second;
}

#pragma opt_propagation off

void CARDi_ReadEnd(void) {
    CardCommon *p = &cardi_common;
    OSIntrMode irq;
    void *argument;
    void (*callback)(void *);
    p->cmd->result = CARD_RESULT_SUCCESS;
    callback = p->callback;
    argument = p->callback_arg;
    irq = OS_DisableInterrupts();
    p->flag &= ~(CARD_STAT_BUSY | CARD_STAT_TASK | CARD_STAT_CANCEL);
    OS_WakeupThread(&p->busy_q);
    if (p->flag & CARD_STAT_RECV) {
        OS_WakeupThreadDirect(&p->thread);
    }
    OS_RestoreInterrupts(irq);
    if (callback) {
        callback(argument);
    }
}

#pragma opt_propagation reset

u32 CARDi_GetRomFlag(u32 mask) {
    return (*(u32 *)(cardi_rom_header_addr + 0x60) & ~REG_MI_MCCNT1_PC_MASK) | mask | 0xa0000000;
}

void CARDi_ReadCard(void *argument) {
}

u32 CARDi_ReadRomIDCore(void) {
    CARDi_SetRomOp(MROMOP_G_READ_ID, 0);
    REG_CARD_ROM_CNT = CARDi_GetRomFlag(0x07000000) & ~CARD_LATENCY1_MASK;
    while (!(REG_CARD_ROM_CNT & REG_MI_MCCNT1_RDY_MASK)) {}
    return REG_CARD_ROM_DATA;
}

u32 CARDi_ReadRomID(void) {
    u32 id;
    CARDi_WaitTask(&cardi_common, NULL, NULL);
    id = CARDi_ReadRomIDCore();
    CARDi_ReadEnd();
    return id;
}

void CARDi_WaitTask(CardCommon *common, void (*callback)(void *), void *argument) {
    OSIntrMode irq = OS_DisableInterrupts();
    while (common->flag & CARD_STAT_BUSY) {
        OS_SleepThread(&common->busy_q);
    }
    common->flag |= CARD_STAT_BUSY;
    common->callback = callback;
    common->callback_arg = argument;
    OS_RestoreInterrupts(irq);
}

void CARD_Init(void) {
    CardCommon *p = &cardi_common;
    if (p->flag == 0) {
        p->flag = CARD_STAT_INIT;
        p->len = 0;
        p->dst = 0;
        p->src = 0;
        p->dma = (u32)-1;
        p->callback = NULL;
        p->callback_arg = NULL;
        cardi_rom_base = 0;
        CARDi_InitCommon();
        rom_stat.read_func = CARDi_ReadCard;
        CARD_InitPulledOutCallback();
    }
}

void CARDi_OnFifoRecv(PXIFifoTag tag, u32 data, BOOL err) {
    CardCommon *p;
    if (tag == PXI_FIFO_TAG_FS && err) {
        p = &cardi_common;
        if (p->recv_count == 0) {
            p->command = data;
        }
        switch (p->command) {
        case CARD_REQ_INIT:
            if (p->recv_count == 0) {
                break;
            }
            if (p->recv_count == 1) {
                p->cmd = (CardCommandArg *)data;
                p->flag |= CARD_STAT_RECV;
            }
            break;
        case CARD_REQ_ACK:
            break;
        case CARD_REQ_IDENTIFY:
        case CARD_REQ_READ_ID:
        case CARD_REQ_READ_ROM:
        case CARD_REQ_WRITE_ROM:
        case CARD_REQ_READ_BACKUP:
        case CARD_REQ_WRITE_BACKUP:
        case CARD_REQ_PROGRAM_BACKUP:
        case CARD_REQ_VERIFY_BACKUP:
        case CARD_REQ_ERASE_PAGE_BACKUP:
        case CARD_REQ_ERASE_SECTOR_BACKUP:
        case CARD_REQ_ERASE_CHIP_BACKUP:
        case 13:
        case 14:
        case 15:
            p->flag |= CARD_STAT_RECV;
            break;
        }
        if (!(p->flag & CARD_STAT_RECV)) {
            p->recv_count++;
        } else {
            p->recv_count = 0;
            OS_WakeupThreadDirect((p->flag & CARD_STAT_BUSY) ? p->cur_th : &p->thread);
        }
    }
}

// Requests 13 (read status register), 14 (write status register) and 15 (erase sub-sector) are
// ARM7 extensions of the CARDRequest list in nitro/card/common.h.
void CARDi_TaskThread(void *argument) {
    CardCommon *p = &cardi_common;
    (void)argument;
    for (;;) {
        BOOL remote = FALSE;
        OSIntrMode irq = OS_DisableInterrupts();
        for (;;) {
            if (!(p->flag & CARD_STAT_BUSY)) {
                if (p->flag & CARD_STAT_RECV) {
                    remote = TRUE;
                    p->flag |= CARD_STAT_BUSY;
                    p->flag &= ~CARD_STAT_RECV;
                    break;
                }
            } else if (p->flag & CARD_STAT_TASK) {
                break;
            }
            p->cur_th = &p->thread;
            OS_SleepThread(NULL);
        }
        OS_RestoreInterrupts(irq);
        if (remote) {
            p->cmd->result = CARD_RESULT_SUCCESS;
            {
                CardCommandArg *cmd = p->cmd;
                u32 command = p->command;
                if (!(cmd->caps & (1u << command))) {
                    cmd->result = CARD_RESULT_UNSUPPORTED;
                } else {
                    switch (command) {
                    case CARD_REQ_INIT:
                    case CARD_REQ_ACK:
                        break;
                    case CARD_REQ_IDENTIFY:
                        CARDi_InitStatusRegister();
                        break;
                    case CARD_REQ_READ_ID:
                        p->cmd->id = CARDi_ReadRomIDCore();
                        break;
                    case CARD_REQ_READ_ROM:
                        cmd->result = CARD_RESULT_UNSUPPORTED;
                        break;
                    case CARD_REQ_READ_BACKUP:
                        CARDi_ReadBackupCore(cmd->src, (void *)cmd->dest, cmd->len);
                        break;
                    case CARD_REQ_WRITE_BACKUP:
                        CARDi_WriteBackupCore(cmd->dest, (const void *)cmd->src, cmd->len);
                        break;
                    case CARD_REQ_PROGRAM_BACKUP:
                        CARDi_ProgramBackupCore(cmd->dest, (const void *)cmd->src, cmd->len);
                        break;
                    case CARD_REQ_VERIFY_BACKUP:
                        CARDi_VerifyBackupCore(cmd->dest, (const void *)cmd->src, cmd->len);
                        break;
                    case CARD_REQ_ERASE_SECTOR_BACKUP:
                        CARDi_EraseBackupSectorCore(cmd->dest, cmd->len);
                        break;
                    case 15:
                        CARDi_EraseBackupSubSectorCore(cmd->dest, cmd->len);
                        break;
                    case CARD_REQ_ERASE_CHIP_BACKUP:
                        CARDi_EraseChipCore();
                        break;
                    case 13:
                        *(u8 *)p->cmd->dest = CARDi_CommandReadStatus();
                        break;
                    case 14:
                        CARDi_SetWriteProtectCore(*(u8 *)cmd->src);
                        break;
                    default:
                        cmd->result = CARD_RESULT_UNSUPPORTED;
                        break;
                    }
                }
            }
            while (PXI_SendWordByFifo(PXI_FIFO_TAG_FS, CARD_PXI_COMMAND_TERMINATE, TRUE) < 0) {}
            irq = OS_DisableInterrupts();
            p->flag &= ~(CARD_STAT_BUSY | CARD_STAT_TASK | CARD_STAT_CANCEL);
            OS_WakeupThread(&p->busy_q);
            if (p->flag & CARD_STAT_RECV) {
                OS_WakeupThreadDirect(&p->thread);
            }
            OS_RestoreInterrupts(irq);
        } else {
            p->task_func(p);
        }
    }
}

void CARD_InitPulledOutCallback(void) {
    if (!skipCheck$3668[2]) {
        skipCheck$3668[2] = TRUE;
        PXI_Init();
        while (!PXI_IsCallbackReady(PXI_FIFO_TAG_CARD, PXI_PROC_ARM9)) {}
        PXI_SetFifoRecvCallback(PXI_FIFO_TAG_CARD, CARDi_CallbackForPulledOut);
    }
}

void CARDi_CallbackForPulledOut(PXIFifoTag tag, u32 data, BOOL err) {
    (void)tag;
    (void)err;
    if ((data & CARD_PXI_COMMAND_MASK) == CARD_PXI_COMMAND_TERMINATE) {
        OSIntrMode irq;
        MI_StopDma(0);
        MI_StopDma(1);
        MI_StopDma(2);
        MI_StopDma(3);
        CTRDG_VibPulseEdgeUpdate(0);
        irq = OS_DisableInterrupts();
        SND_BeginSleep();
        WVR_Shutdown();
        OS_RestoreInterrupts(irq);
        OS_Terminate();
    } else {
        OS_Terminate();
    }
}

BOOL CARD_IsPulledOut(void) {
    if (!skipCheck$3668[3]) {
        if (*(vu8 *)(HW_ROM_HEADER_BUF + 0x1f) & 0x80) {
            CARD_CompareCardID();
        } else {
            CARD_IsCardIreqLo();
        }
    }
    return skipCheck$3668[3];
}

BOOL CARD_CompareCardID(void) {
    BOOL equal = TRUE;
    s32 lock = OS_GetLockID();
    if (lock != OS_LOCK_ID_ERROR) {
        if (OS_TryLockCard((u16)lock) == 0) {
            vu32 *address;
            vu32 id;
            u32 actual;
            if (*(vu16 *)0x027ffc10 == 0) {
                address = (vu32 *)0x027ff800;
            } else {
                address = (vu32 *)0x027ffc00;
            }
            id = *address;
            actual = CARDi_ReadRomID();
            equal = (actual == id);
            OS_UnlockCard((u16)lock);
        }
        OS_ReleaseLockID((u16)lock);
    }
    skipCheck$3668[3] = !equal;
    return equal;
}

BOOL CARD_IsCardIreqLo(void) {
    BOOL low = TRUE;
    if (REG_IF & 0x100000) {
        low = FALSE;
        skipCheck$3668[3] = TRUE;
    }
    return low;
}

void CARD_CheckPullOut_Polling(void) {
    u32 next;
    if (skipCheck$3668[1]) {
        return;
    }
    if (*(vu16 *)HW_WM_BOOT_BUF == 2) {
        return;
    }
    next = nextCount$3668[0];
    if (next == 0xffffffff) {
        nextCount$3668[0] = *(vu32 *)HW_VBLANK_COUNT_BUF + 10;
        return;
    }
    if (*(vu32 *)HW_VBLANK_COUNT_BUF < next) {
        return;
    }
    nextCount$3668[0] = *(vu32 *)HW_VBLANK_COUNT_BUF + 10;
    if (CARD_IsPulledOut()) {
        skipCheck$3668[1] = TRUE;
        if (*(u32 *)((u8 *)CARD_GetRomHeader() + 0xc) == 0 && nextCount$3668[1] != 0) {
            return;
        }
    }
    nextCount$3668[1] = 0;
    if (skipCheck$3668[1]) {
        while (PXI_SendWordByFifo(PXI_FIFO_TAG_CARD, CARD_PXI_COMMAND_PULLED_OUT, FALSE)) {
            VENEER_SVC_WaitByLoop(100);
        }
    }
}

#pragma dont_inline reset
