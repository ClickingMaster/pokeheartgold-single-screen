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

typedef struct W35242Param {
    u32 count;
    const u8 *source;
    u8 *destination;
    u32 verified;
} W35242Param;

typedef struct W35242Status {
    u32 checked;
    W35242Param param;
} W35242Status;

extern W35242Status status_checked$3825;
extern W35242Param cardi_param;
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
void CARDi_CommReadCore(W35242Param *);
void CARDi_CommWriteCore(W35242Param *);
void CARDi_CommVerifyCore(W35242Param *);
void CARDi_CommArray(const void *, void *, u32, void (*)(W35242Param *));
void CARDi_SetRomOp(u32, u32);
u32 CARDi_GetRomFlag(u32);

void CARDi_CommandEnd(s32 initial, s32 limit);

static inline int W35242Ready(void) {
    return !(CARDi_CommandReadStatus() & 1);
}

void CARDi_CommandEnd(s32 initial, s32 limit) {
    if ((u32)initial + (u32)limit) {
        if (initial) {
            OS_Sleep(initial);
        }
        if (limit) {
            s32 remain = (s32)((u32)limit - (u32)initial);
            while (!W35242Ready() && remain > 0) {
                {
                    s32 delay = remain < 5 ? remain : 5;
                    OS_Sleep(delay);
                    remain -= delay;
                }
            }
        }
        {
            int ready = !(CARDi_CommandReadStatus() & 1);
            if (!ready) {
                U32((void *)U32(cardi_common, 0), 0) = 4;
            }
        }
    }
}
