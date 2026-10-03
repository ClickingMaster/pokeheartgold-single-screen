#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define PTR(p,o) ((void*)W(p,o))
void G2x_SetBlendAlpha_(u32,int,int,int,int);
void ov07_0221F3F4(void*p);
void ov07_0221F3F4(void*p){u32 x,y;W(p,24)+=4;x=*(u32*)W(p,24);W(p,24)+=4;y=*(u32*)W(p,24);W(p,24)+=4;G2x_SetBlendAlpha_(0x04001050,1,2,(u16)x,(u16)y);}
