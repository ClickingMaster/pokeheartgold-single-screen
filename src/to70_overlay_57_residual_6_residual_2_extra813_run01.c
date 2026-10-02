#pragma force_active on
#include "global.h"
extern void GfGfx_EngineATogglePlanes(int,int);
void ov57_02238B48(void);
void ov57_02238B48(void){volatile u16 *bg=(volatile u16*)0x4000008;volatile u16 *reg=(volatile u16*)0x4000060;GfGfx_EngineATogglePlanes(1,1);*bg=(*bg&~3)|1;*reg=*reg&0xffffcffd;*reg=(*reg&0xffffcfff)|16;*reg=*reg&0xcffb;*reg=(*reg&0xffffcfff)|8;*reg=*reg&0xcfdf;G3X_SetFog(0,(GXFogBlend)0,(GXFogSlope)0,0);G3X_SetClearColor(0,0,0x7fff,63,0);*(volatile u32*)0x4000580=0xbfff0000;}
