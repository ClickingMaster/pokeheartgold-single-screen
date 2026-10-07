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
int WSetListenInterval(u32 x);
// @fn WSetListenInterval
int WSetListenInterval(u32 x){if(x<1||x>255)return 5;H(BASE,0x3b4)=x;return 0;}
