#pragma force_active on
#include "global.h"
extern void GfGfx_EngineATogglePlanes(int,int);

void ov48_02259798(void);

void ov48_02259798(void){volatile u16*bg=(volatile u16*)0x4000008;volatile u16*dis=(volatile u16*)0x4000060;GfGfx_EngineATogglePlanes(1,1);*bg=(*bg&~3)|1;*dis=*dis&~0x3002;*dis=(*dis&~0x3000)|16;*dis=*dis&0xcffb;*dis=(*dis&~0x3000)|8;*dis=(*dis&~0x3000)|32;G3X_SetFog(0,GX_FOGBLEND_COLOR_ALPHA,GX_FOGSLOPE_0x8000,0);G3X_SetClearColor(0x6b5a,0,0x7fff,63,0);*(volatile u32*)0x4000580=0xbfff0000;}
