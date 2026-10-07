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
void WSetMacAdrs1(u16*d,const u16*a);
// @fn WSetMacAdrs1
void WSetMacAdrs1(u16*d,const u16*a){d[0]=a[0];d[1]=a[1];d[2]=a[2];}
