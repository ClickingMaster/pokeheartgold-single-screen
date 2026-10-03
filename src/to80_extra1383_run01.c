#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#include <string.h>
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define B(p,o) (*(u8 *)((u8 *)(p)+(o)))
extern u8 *ov13_0224F5E0;
extern u8 *ov13_022402E0(u32);
extern u32 ov13_02241EBC(int,int,int,u32*);
extern int NNSi_G2dCalcRequiredOBJ(int,int);
extern void NNS_G2dCharCanvasInitForOBJ1D(void*,void*,int,int,int);
u8 *ov13_02240798(int,int,int,int,u32*,int);
u8 *ov13_02240798(int engine,int width,int height,int flags,u32*required,int index){u32 offset;u8*c=ov13_022402E0(W(ov13_0224F5E0,0x670));B(c,46)=width;B(c,47)=height;W(c,40)=ov13_02241EBC(engine,width*height,flags,&offset);H(c,44)=offset;*required=NNSi_G2dCalcRequiredOBJ(width,height);{u32 tile=offset;u32 base=engine==1?0x6600000:0x6400000;NNS_G2dCharCanvasInitForOBJ1D(c,(void*)(base+(tile<<7)),width,height,4);}((void(*)(void*,int))W((void*)W(c,20),4))(c,0);{u8 *owner=ov13_0224F5E0+index*8;W(c,24)=(u32)c;W(c,28)=(u32)owner;}W(c,32)=1;W(c,36)=1;return c;}
