#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define HW16(a)   (*(volatile u16 *)(a))
#define HW32(a)   (*(volatile u32 *)(a))
#define HW8(a)    (*(volatile u8 *)(a))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U8(p, o)  (*((u8 *)(p) + (o)))

typedef struct W35241Param {
    u32 count;
    const u8 *source;
    u8 *destination;
    u32 verified;
} W35241Param;

typedef struct W35241Status {
    u32 checked;
    W35241Param param;
} W35241Status;

#define status_checked (*(W35241Status *)((u8 *)&cardi_param - 4))
extern W35241Param cardi_param;
extern u8 cardi_common[0x200];
extern u8 arm7_arg3983[];
extern u8 arm7_buf3759[];
extern u8 arm7_arg3806[];
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
void CARDi_CommReadCore(W35241Param *);
void CARDi_CommWriteCore(W35241Param *);
void CARDi_CommVerifyCore(W35241Param *);
void CARDi_CommArray(const void *, void *, u32, void (*)(W35241Param *));
void CARDi_SetRomOp(u32, u32);
u32 CARDi_GetRomFlag(u32);

void *CARD_GetRomHeader(void);

void *CARD_GetRomHeader(void) {
    return (void *)0x027ffa80;
}
