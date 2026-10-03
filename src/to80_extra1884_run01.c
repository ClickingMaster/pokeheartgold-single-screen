#pragma force_active on
#include "global.h"
extern void ov68_021E66A0(void*,int,u8,int,int,int,int);

void ov68_021E7A18(void*w,u32 selected);

#pragma force_active on
#pragma opt_lifetimes off
#pragma opt_common_subs on
void ov68_021E7A18(void*w,u32 selected){u32 i;u32 x;i=0;x=4;do{if(i==selected)ov68_021E66A0(w,0,(u8)x,16,4,0,8);else if(i>=*((u8*)w+0x1b8))ov68_021E66A0(w,0,(u8)x,16,4,0,4);else ov68_021E66A0(w,0,(u8)x,16,4,0,0);x+=4;i++;}while(i<4);}
