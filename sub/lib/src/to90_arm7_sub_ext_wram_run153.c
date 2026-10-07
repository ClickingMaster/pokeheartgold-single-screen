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
void WSetMacAdrs2(u16*d,const u16*a,const u16*b);
// @fn WSetMacAdrs2
void WSetMacAdrs2(u16*d,const u16*a,const u16*b){d[0]=a[0];d[1]=a[1];d[2]=a[2];d[3]=b[0];d[4]=b[1];d[5]=b[2];}
