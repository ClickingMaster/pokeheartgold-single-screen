#pragma force_active on
#pragma dollar_identifiers on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define BASE (*(u8 **)0x0380fff4)
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define P(p,o) (*(u8 **)((u8*)(p)+(o)))
#define HW(a) (*(volatile u16 *)(a))
void WaitLoop_ClrAid(void);void DeleteTxFrames(u32);void CAM_SetStaState(u32,u32);void WClearAids(void);
// @fn WClearAids
void WClearAids(void){u8*s=BASE+0x344;H(s,0x6a)=0;WaitLoop_ClrAid();HW(0x0480802a)=0;if(H(s,0x88)){DeleteTxFrames(H(s,0x88));CAM_SetStaState(H(s,0x88),32);H(s,0x88)=0;}}
