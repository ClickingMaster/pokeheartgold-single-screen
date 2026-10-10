#pragma force_active on
#include <nitro/os/system.h>
#include <nitro/os/thread.h>
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u8 wmspW[];
extern u8 wmspIndicateThread[];
extern u8 wmspRequestThread[];
extern OSIntrMode __VENEER_OS_DisableInterrupts(void);
extern OSIntrMode __VENEER_OS_RestoreInterrupts(OSIntrMode);
extern u32 __VENEER_OS_DisableScheduler(void);
extern u32 __VENEER_OS_EnableScheduler(void);
extern BOOL __VENEER_OS_SetThreadPriority(OSThread *, u32);
extern void *WL_GetThreadStruct(void);
void WMSP_SetThreadPriorityHigh(void);

void WMSP_SetThreadPriorityHigh(void) {
    OSIntrMode irq = __VENEER_OS_DisableInterrupts();
    __VENEER_OS_DisableScheduler();
    __VENEER_OS_SetThreadPriority((OSThread *)wmspIndicateThread, *(u32 *)(wmspW + 0x1578));
    __VENEER_OS_SetThreadPriority((OSThread *)WL_GetThreadStruct(), *(u32 *)(wmspW + 0x157c));
    __VENEER_OS_SetThreadPriority((OSThread *)wmspRequestThread, *(u32 *)(wmspW + 0x1580));
    __VENEER_OS_EnableScheduler();
    __VENEER_OS_RestoreInterrupts(irq);
}
