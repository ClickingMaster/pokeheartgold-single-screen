#pragma force_active on
#include "global.h"
#pragma force_active on
extern void GfGfx_EngineATogglePlanes(int,int);
extern void *GF_3DVramMan_Create(int,int,int,int,int,void(*)(void));
extern void ov91_0225FD2C(void*,int,int,int);

void ov91_0225F434(void);
void ov91_0225F4C0(u8*w,int a,int b,int heap);

void ov91_0225F434(void){volatile u16*p=(volatile u16*)0x04000008;GfGfx_EngineATogglePlanes(1,1);*p=(*p&~3)|2;p=(volatile u16*)0x04000060;*p&=~0x3002;*p=(*p&~0x3000)|16;*p&=0xcffb;*p=(*p&~0x3000)|8;*p=(*p&~0x3000)|32;G3X_SetFog(0,(GXFogBlend)0,(GXFogSlope)0,0);G3X_SetClearColor(0x6b5a,0,0x7fff,63,0);*(volatile u32*)0x04000580=0xbfff0000;}
void ov91_0225F4C0(u8*w,int a,int b,int heap){*(void**)(w+24)=GF_3DVramMan_Create(heap,0,1,0,4,ov91_0225F434);ov91_0225FD2C(w,a,b,heap);}
