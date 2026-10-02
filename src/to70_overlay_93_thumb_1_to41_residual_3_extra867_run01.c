#pragma force_active on
#include "global.h"
#pragma force_active on
extern void GfGfx_EngineATogglePlanes(int,int);
void ov93_0225CF34(void);
void ov93_0225CF34(void){volatile u16 *bg=(volatile u16 *)0x04000008;volatile u16 *disp=(volatile u16 *)0x04000060;GfGfx_EngineATogglePlanes(1,1);*bg=(*bg&~3)|1;*disp&=~0x3002;*disp=(*disp&~0x3000)|16;*disp&=0xcffb;*disp=(*disp&~0x3000)|8;*disp&=0xcfdf;G3X_SetFog(0,(GXFogBlend)0,(GXFogSlope)0,0);G3X_SetClearColor(0,0,32767,63,0);*(volatile u32 *)0x04000580=0xbfff0000;}
