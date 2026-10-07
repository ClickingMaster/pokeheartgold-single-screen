#pragma force_active on
#include <nitro/types.h>
#include <nitro/os/thread.h>
#include <nitro/os/system.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u8 wmspW[],wmspIndicateThread[],wmspRequestThread[];
extern OSIntrMode __VENEER_OS_DisableInterrupts(void),__VENEER_OS_RestoreInterrupts(OSIntrMode);
extern u32 __VENEER_OS_DisableScheduler(void),__VENEER_OS_EnableScheduler(void);
extern BOOL __VENEER_OS_SetThreadPriority(OSThread *,u32);
extern void *WL_GetThreadStruct(void);
void WMSP_SetThreadPriorityHigh(void);
// @fn WMSP_SetThreadPriorityHigh
void WMSP_SetThreadPriorityHigh(void){OSIntrMode irq=__VENEER_OS_DisableInterrupts();__VENEER_OS_DisableScheduler();__VENEER_OS_SetThreadPriority((OSThread*)wmspIndicateThread,*(u32*)(wmspW+0x1578));__VENEER_OS_SetThreadPriority((OSThread*)WL_GetThreadStruct(),*(u32*)(wmspW+0x157c));__VENEER_OS_SetThreadPriority((OSThread*)wmspRequestThread,*(u32*)(wmspW+0x1580));__VENEER_OS_EnableScheduler();__VENEER_OS_RestoreInterrupts(irq);}
