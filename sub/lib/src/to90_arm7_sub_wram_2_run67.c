#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on

u32 MATH_CountPopulation(u32 x);
// @fn MATH_CountPopulation
u32 MATH_CountPopulation(u32 x){x=x-((x>>1)&0x55555555);x=(x&0x33333333)+((x>>2)&0x33333333);x=(x+(x>>4))&0x0f0f0f0f;x=x+(x>>8);x=x+(x>>16);return x&255;}
