#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(a) (*(volatile u16 *)(a))
#define WL (*(u8 **)0x0380fff4)
#define R16(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define R32(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define P(p,o) (*(u8 **)((u8 *)(p)+(o)))
extern void WUpdateCounter(void);
void WlIntrCntOvf(void);
// @fn WlIntrCntOvf
void WlIntrCntOvf(void){WUpdateCounter();H(0x048081ac)=0xffff;H(0x04808010)=0x30;}
