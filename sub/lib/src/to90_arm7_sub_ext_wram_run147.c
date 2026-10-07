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
void __VENEER_MIi_CpuCopy16(const void*,void*,u32);int WInitGameInfo(u32 length,const void*input);
// @fn WInitGameInfo
int WInitGameInfo(u32 length,const void*input){u8*s=BASE+0x344;if(length>128)return 4;__VENEER_MIi_CpuCopy16(input,P(s,0x9c),length+1);H(s,0xa0)=length;return 0;}
