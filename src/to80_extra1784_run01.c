#pragma force_active on
#include "global.h"
#pragma force_active on

void ov49_0225A06C(u8*w,u32 i,u8 value);
void ov49_0225A06C(u8*w,u32 i,u8 value){GF_ASSERT(i<20);w[i+32]=value;}
