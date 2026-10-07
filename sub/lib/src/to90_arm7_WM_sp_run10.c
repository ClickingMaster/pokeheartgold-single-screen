#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern u8 wmspW[],wmspIndicateThread[],wmspRequestThread[];
#define W ((u8 *)(wmspW+0x1000))
#define P (*(u8 **)(W+0x550))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define U(p,o) (*(u32 *)((u8 *)(p)+(o)))
extern BOOL __VENEER_OS_SendMessage(void *,void *,int),__VENEER_OS_ReceiveMessage(void *,void *,int);
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *),__VENEER_SND_BeginSleep(void),__VENEER_OS_Terminate(void);
extern u32 __VENEER_OS_DisableInterrupts(void);
extern void __VENEER_OS_RestoreInterrupts(u32);
extern BOOL __VENEER_OS_DisableScheduler(void),__VENEER_OS_EnableScheduler(void),__VENEER_OS_SetThreadPriority(void *,u32);
extern void *WL_GetThreadStruct(void);
extern u32 WMSP_GetLinkLevel(u32);
#pragma opt_common_subs off
void WMSP_SetThreadPriorityLow(void);
// @fn WMSP_SetThreadPriorityLow
void WMSP_SetThreadPriorityLow(void){u32 irq=__VENEER_OS_DisableInterrupts();__VENEER_OS_DisableScheduler();__VENEER_OS_SetThreadPriority(wmspRequestThread,U(W,0x58c));__VENEER_OS_SetThreadPriority(WL_GetThreadStruct(),U(W,0x588));__VENEER_OS_SetThreadPriority(wmspIndicateThread,U(W,0x584));__VENEER_OS_EnableScheduler();__VENEER_OS_RestoreInterrupts(irq);}
