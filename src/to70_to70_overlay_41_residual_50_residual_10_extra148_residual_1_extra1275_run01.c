#pragma force_active on
#include "global.h"
extern void ov41_0224ACA4(void *,void *),ov41_0224AD0C(void *,void *,int,int,int,int,int,int),ov41_0224AED8(void *,void *,void *,int,void *),ov41_0224B118(void *,void *,void *,int,void *,int,int),ov41_0224AE24(void *,int,void *,int,int,int,int,int),ov41_0224ACDC(void *,void *);
extern void FillWindowPixelBuffer(void *,int),DrawFrameAndWindow2(void *,int,int,int);
extern int Options_GetFrame(void *),Options_GetTextFrameDelay(void *);
void DressUpHud_Init(u32 *,u32 *,u32);
void DressUpHud_Init(u32 *w,u32 *p,u32 flags){if(flags&1)memset(w,0,0x148);if(flags&2)ov41_0224ACA4(w,(void *)p[0]);if(flags&4)ov41_0224AD0C((u8 *)w+0x2c,(void *)p[0],2,19,27,4,31,1);if(flags&8)ov41_0224AED8((u8 *)w+0x38,(void *)p[1],(void *)p[2],p[4],(void *)p[9]);if(flags&16)ov41_0224B118((u8 *)w+0xa0,(void *)p[1],(void *)p[2],p[7],(void *)p[0],p[8],p[9]);if(flags&32){ov41_0224AD0C((u8 *)w+0x134,(void *)p[0],2,1,27,2,139,1);FillWindowPixelBuffer((void *)w[0x4d],15);ov41_0224AE24((void *)w[0x4d],27,(void *)p[5],6,0,0,0x1020f,255);ov41_0224AE24((void *)w[0x4d],27,(void *)p[5],p[6],72,0,0x1020f,255);DrawFrameAndWindow2((void *)w[0x4d],0,1,1);}w[12]=p[1];w[13]=p[2];ov41_0224ACDC((void *)p[0],(void *)p[3]);w[0x4f]=Options_GetFrame((void *)p[3]);w[0x50]=Options_GetTextFrameDelay((void *)p[3]);w[0x4e]|=flags;}
