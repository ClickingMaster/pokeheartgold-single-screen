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
void WUpdateCounter(void);void __VENEER_MIi_CpuClear32(u32,void*,u32);void WInitCounter(void);
// @fn WInitCounter
void WInitCounter(void){WUpdateCounter();__VENEER_MIi_CpuClear32(0,BASE+0x53c,180);}
