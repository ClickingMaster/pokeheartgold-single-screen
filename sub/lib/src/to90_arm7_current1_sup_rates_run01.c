#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define BASE (*(u8 **)0x0380fff4)
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define P(p,o) (*(u8 **)((u8*)(p)+(o)))
#define HW(a) (*(volatile u16 *)(a))
void WL_WriteByte(void*,u32);
extern u16 RateBit2Element[16];
u32 SetSupRateSet(void *destination);
// @fn SetSupRateSet
u32 SetSupRateSet(void *destination){u8 *dest=destination;u8*s=BASE+0x344;u32 count=0,i;WL_WriteByte(dest,1);count+=2;for(i=0;i<16;i++){if(H(s,0x62)&(1u<<i)){if(H(s,0x60)&(1u<<i))WL_WriteByte(dest+count,(u8)(RateBit2Element[i]|0x80));else WL_WriteByte(dest+count,(u8)RateBit2Element[i]);count++;}}WL_WriteByte(dest+1,(u8)(count-2));return count;}
