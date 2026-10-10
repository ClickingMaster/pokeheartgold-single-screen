#pragma force_active on
#pragma dollar_identifiers on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define HW16(a)   (*(volatile u16 *)(a))
#define HW32(a)   (*(volatile u32 *)(a))
#define HW8(a)    (*(volatile u8 *)(a))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U8(p, o)  (*((u8 *)(p) + (o)))

typedef struct W35257Param {
    u32 count;
    const u8 *source;
    u8 *destination;
    u32 verified;
} W35257Param;

typedef struct W35257Status {
    u32 checked;
    W35257Param param;
} W35257Status;

extern W35257Status status_checked$3825;
extern W35257Param cardi_param;
extern u8 cardi_common[0x200];
extern u8 arg$3983[];
extern u8 buf$3759[];
extern u8 arg$3806[];
void OS_Sleep(u32);
u64 OS_GetTick(void);
u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32);
void OS_WakeupThread(void *);
void OS_WakeupThreadDirect(void *);
u8 CARDi_CommandReadStatus(void);
void CARDi_CommandEnd(s32, s32);
int CARDi_WaitPrevCommand(void);
void CARDi_WriteEnable(void);
void CARDi_SendSpiAddressingCommand(u32, u32);
void CARDi_SetWriteProtectCore(u8);
void CARDi_CommReadCore(W35257Param *);
void CARDi_CommWriteCore(W35257Param *);
void CARDi_CommVerifyCore(W35257Param *);
void CARDi_CommArray(const void *, void *, u32, void (*)(W35257Param *));
void CARDi_SetRomOp(u32, u32);
u32 CARDi_GetRomFlag(u32);

void CARDi_EraseBackupSubSectorCore(u32 address, u32 size);

void CARDi_EraseBackupSubSectorCore(u32 address, u32 size) {
    void *c = (void *)U32(cardi_common, 0);
    u32 sector = U32(c, 32);
    if ((sector - 1) & (address | size)) {
        U32(c, 0) = 2;
        return;
    }
    if (CARDi_WaitPrevCommand()) {
        while (size) {
            CARDi_WriteEnable();
            status_checked$3825.param.count = U32(c, 0x28) + 1;
            CARDi_SendSpiAddressingCommand(address, 32);
            CARDi_CommandEnd((s32)U32(c, 72), (s32)U32(c, 76));
            if (U32(c, 0)) {
                break;
            }
            address += sector;
            size -= sector;
        }
    }
}
