#pragma force_active on
#include "global.h"
extern u16 LCRandom(void);
extern u32 ov80_02229F6C(void *,u16,u32,int,u32,u8,int,int,int);
u32 ov80_0222A4EC(void *,u16,int,u32,int,int,int);
u32 ov80_0222A4EC(void *p,u16 val,int index,u32 data,int a,int b,int c){u32 r=LCRandom();r|=(u32)LCRandom()<<16;return ov80_02229F6C(p,val,r,a,data,(u8)index,0,b,c);}
