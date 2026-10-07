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
int WSetPowerState(u32 state);
// @fn WSetPowerState
int WSetPowerState(u32 state){H(BASE,0x354)=state>>1;HW(0x0480803c)=state;return 0;}
