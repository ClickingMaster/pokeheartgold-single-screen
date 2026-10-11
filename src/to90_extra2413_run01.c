#pragma force_active on
#include "global.h"
#pragma force_active on
void ModelAnimation_AdvanceLooped(void*,s32),ModelAnimation_SetFrame(void*,s32),ModelInstance_Draw(void*);void ov91_022610A8(u8*);
#define I(w,o) (*(s32*)((w)+(o)))
#define B(w,o) (*((w)+(o)))
#define H(w,o) (*(s16*)((w)+(o)))
void ov91_022610A8(u8*w){int i;ModelAnimation_AdvanceLooped(w+0x198,I(w,0x1d4));if(B(w,0x1d8)){switch(B(w,0x1d9)){case 0:{s32 step=I(w,0x1e0);if(I(w,0x1dc)+step<0x33000)I(w,0x1dc)+=step;else I(w,0x1dc)=0;if(I(w,0x1e0)+410<=0x24cd)I(w,0x1e0)+=410;else{I(w,0x1e0)=0x24cd;H(w,0x1da)=24;B(w,0x1d9)++;}break;}case 1:{s32 step=I(w,0x1e0);if(I(w,0x1dc)+step<0x33000)I(w,0x1dc)+=step;else I(w,0x1dc)=0;H(w,0x1da)--;if(H(w,0x1da)<=0){B(w,0x1d9)++;I(w,0x1dc)=0x33000;}break;}case 2:if(I(w,0x1dc)+4096<0x64000)I(w,0x1dc)+=4096;else I(w,0x1dc)=0x33000;break;default:GF_AssertFail();break;}ModelAnimation_SetFrame(w+0x1ac,I(w,0x1dc));ModelAnimation_SetFrame(w+0x1c0,I(w,0x1dc));}for(i=0;i<3;i++,w+=0x78)ModelInstance_Draw(w);}
