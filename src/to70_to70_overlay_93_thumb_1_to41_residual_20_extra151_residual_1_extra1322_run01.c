#pragma force_active on
#include "global.h"
extern void NNS_G3dGeBufferOP_N(u32,const u32 *,u32),NNS_G3dGeFlushBuffer(void);
extern void ov93_0225EE4C(int,int),ov93_0225EE98(void),ov93_0225EFAC(void *);
void ov93_0225EB70(u8 *);
void ov93_0225EB70(u8 *w) {
 u32 a,b; u32 translate[3],scale[3];u32 c,d,e,f,g,h;
 s32 sy,sx,tmp;u32 *obj;
 a=0x2d8b6127;NNS_G3dGeBufferOP_N(0x32,&a,1);b=0x7fff;NNS_G3dGeBufferOP_N(0x33,&b,1);NNS_G3dGeBufferOP_N(0x11,0,0);
 translate[0]=0;translate[1]=0x13000;translate[2]=-0x5000;NNS_G3dGeBufferOP_N(0x1c,translate,3);
 tmp=*(s32*)(w+0x264);sy=tmp+tmp*(*(s16*)(w+0x26a))/100;
 tmp=*(s32*)(w+0x260);sx=tmp+tmp*(*(s16*)(w+0x268)+*(s16*)(w+0x26c))/100;
 scale[0]=sx;scale[2]=4096;scale[1]=sy;NNS_G3dGeBufferOP_N(0x1b,scale,3);
 c=3;NNS_G3dGeBufferOP_N(0x10,&c,1);NNS_G3dGeBufferOP_N(0x15,0,0);
 d=2;NNS_G3dGeBufferOP_N(0x10,&d,1);ov93_0225EE4C(1,1);ov93_0225EE98();
 obj=*(u32**)(w+4);tmp=(u16)obj[11];e=0x72400000|((u16)obj[2]&0x1fffffff);NNS_G3dGeBufferOP_N(0x2a,&e,1);
 f=(u32)tmp>>1;NNS_G3dGeBufferOP_N(0x2b,&f,1);g=0x1f00c1;NNS_G3dGeBufferOP_N(0x29,&g,1);
 ov93_0225EFAC(w);h=1;NNS_G3dGeBufferOP_N(0x12,&h,1);NNS_G3dGeFlushBuffer();
}
