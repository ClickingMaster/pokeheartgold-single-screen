#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define HW16(a)   (*(volatile u16 *)(a))
#define HW32(a)   (*(volatile u32 *)(a))
#define HW8(a)    (*(volatile u8 *)(a))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U8(p, o)  (*((u8 *)(p) + (o)))

typedef struct W35264Param {
    u32 count;
    const u8 *source;
    u8 *destination;
    u32 verified;
} W35264Param;

typedef struct W35264Status {
    u32 checked;
    W35264Param param;
} W35264Status;

#define status_checked (*(W35264Status *)((u8 *)&cardi_param - 4))
extern W35264Param cardi_param;
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
void CARDi_CommReadCore(W35264Param *);
void CARDi_CommWriteCore(W35264Param *);
void CARDi_CommVerifyCore(W35264Param *);
void CARDi_CommArray(const void *, void *, u32, void (*)(W35264Param *));
void CARDi_SetRomOp(u32, u32);
u32 CARDi_GetRomFlag(u32);

u32 CARDi_ReadRomIDCore(void);

u32 CARDi_ReadRomIDCore(void) {
    CARDi_SetRomOp(0xb8000000, 0);
    HW32(0x040001a4) = CARDi_GetRomFlag(0x07000000) & ~0x1fff;
    while (!(HW32(0x040001a4) & 0x00800000)) {}
    return HW32(0x04100010);
}
