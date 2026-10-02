#pragma force_active on
#include "global.h"
void ov59_0223C3F0(void);
void ov59_0223C3F0(void){volatile u16 *reg=(volatile u16*)0x4000060;NNS_G3dInit();G3X_Init();G3X_InitMtxStack();*reg=*reg&0xffffcffd;*reg=*reg&0xcffb;*reg=(*reg&0xffffcfff)|8;*reg=(*reg&0xffffcfff)|16;*reg=*reg&0xcfdf;G3X_SetFog(0,(GXFogBlend)0,(GXFogSlope)0,0);G3X_SetClearColor(0,0,0x7fff,63,0);*(volatile u32*)0x4000540=2;*(volatile u32*)0x4000580=0xbfff0000;}
