#pragma force_active on
#include "global.h"
void ov96_021FB5C8(void *,int,u32);
#pragma force_active on
#pragma opt_lifetimes off
void ov96_021FB5C8(void *w,int index,u32 value){u8 shift,slot,bits;u8 *p;if((u32)index>=12)GF_AssertFail();if(value==0||value>12)GF_AssertFail();shift=(u8)(index%2);slot=(u8)(index>>1);bits=(u8)(value<<(shift*4));p=(u8*)w+28;p[slot]|=bits;}
