#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern int MATH_CountPopulation(u32);void*WmGetSharedDataAddress(void*,u32,void*,int);
void*WmGetSharedDataAddress(void*p,u32 mask,void*data,int aid){int count=MATH_CountPopulation(mask&((1u<<aid)-1));return(u8*)data+*(u16*)((u8*)p+0x810)*count;}
