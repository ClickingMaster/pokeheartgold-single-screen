#pragma force_active on
#include "global.h"
extern void ov68_021E71C4(void*,int),ov68_021E7224(void*,void*,int),GfGfx_EngineATogglePlanes(int,int);
void ov68_021E7288(void*);
#pragma force_active on
void ov68_021E7288(void*w){volatile u16*r=(volatile u16*)0x4000060;NNS_G3dInit();G3X_Init();G3X_InitMtxStack();*r&=~0x3002u;*r&=0xcffb;*r=(*r&~0x3000u)|8;*r=(*r&~0x3000u)|16;*r&=0xcfdf;G3X_SetFog(0,(GXFogBlend)0,(GXFogSlope)0,0);G3X_SetClearColor(0,0,32767,63,0);*(volatile u32*)0x4000540=2;*(volatile u32*)0x4000580=0xbfff0000;MI_CpuFill8((u8*)w+0x154,0,28);ov68_021E71C4((u8*)w+0x154,66);ov68_021E7224((u8*)w+0x154,**(void***)w,66);GfGfx_EngineATogglePlanes(1,1);*(volatile u16*)0x4000008=(*(volatile u16*)0x4000008&~3)|1;}
