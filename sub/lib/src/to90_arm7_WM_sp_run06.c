#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#pragma opt_common_subs off
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
void WMSP_AddRssiToList(u32 r);
// @fn WMSP_AddRssiToList
void WMSP_AddRssiToList(u32 r){u32 x;wmspW[0x1554+U(W,0x574)]=r;U(W,0x574)=(U(W,0x574)+1)&31;x=r^((u32)*(volatile u16 *)0x027fff98<<1);*(volatile u16 *)0x027fff98=x^(x>>16);}
