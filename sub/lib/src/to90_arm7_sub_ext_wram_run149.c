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
void WSetAids(u16 aid);
// @fn WSetAids
void WSetAids(u16 aid){H(BASE,0x3ae)=aid;HW(0x0480802a)=aid;if(((struct {u16 lower:7,mirror:1,upper:8;}*)(BASE+0x33a))->mirror)HW(0x04808028)=aid;}
