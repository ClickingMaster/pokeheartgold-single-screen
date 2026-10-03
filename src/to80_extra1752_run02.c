#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Work Work;
struct Work{u8*stream;void*model;u32 flags;u8 pad0[0x18];void(*node)(Work*);void(*bb)(Work*);void(*bby)(Work*);u8 pad1[0x62];u8 phase[3];};
typedef struct{u8 pad[0x4c];MtxFx43 view;u8 pad2[0x80];u32 flags;}Glob;
extern u32 G3X_GetClipMtx(MtxFx44*);extern void MIi_CpuSend32(const void*,void*,u32);
extern void NNS_G3dGeBufferOP_N(u32,const u32*,u32),NNS_G3dGeFlushBuffer(void),MTX_Copy43To44_(const MtxFx43*,MtxFx44*);
extern const MtxFx43 *NNS_G3dGlbGetWV(void),*NNS_G3dGlbGetInvWV(void),*NNS_G3dGlbGetInvV(void);
static inline void fifo(u32 value){*(vu32*)0x04000400=value;}
extern VecFx32 _02110B84,_02110B90;extern u32 _02110B58[],_02110B60[],_02110B54[];
void NNSi_G3dFuncSbc_BBY(Work*,u32);
void NNSi_G3dFuncSbc_BBY(Work*w,u32 option){struct{u32 pop,restore;MtxFx44 view,vw,clip;}l;u32 length=2;int timing;int handled;VecFx32 *translation=&_02110B84,*scale=&_02110B90,*matrix=(VecFx32*)_02110B60;
 if(w->flags&0x200){if(option==0x40||option==0x60)length++;if(option==0x20||option==0x60)length++;w->stream+=length;return;}
 if(option==0x40||option==0x60){length++;if(!(w->flags&0x100)){if(option==0x40)l.restore=w->stream[2];else l.restore=w->stream[3];NNS_G3dGeBufferOP_N(0x14,&l.restore,1);}}
 timing=w->bby?w->phase[2]:0;
 if(timing==1){w->flags&=~0x40;w->bby(w);timing=w->bby?w->phase[2]:0;handled=w->flags&0x40;}else handled=0;
 if(!(w->flags&0x100)&&!handled){NNS_G3dGeFlushBuffer();fifo(0x151110);fifo(0);fifo(0);while(G3X_GetClipMtx(&l.clip)!=0){}
 if(NNS_G3dGlb.flag&1){MTX_Copy43To44_(NNS_G3dGlbGetWV(),&l.vw);MTX_Concat44(&l.clip,&l.vw,&l.clip);}else if(NNS_G3dGlb.flag&2){MTX_Copy43To44_(&NNS_G3dGlb.cameraMtx,&l.view);MTX_Concat44(&l.clip,&l.view,&l.clip);}
 translation->x=((s32*)&l.clip)[12];translation->y=((s32*)&l.clip)[13];translation->z=((s32*)&l.clip)[14];scale->x=VEC_Mag((VecFx32*)&l.clip);scale->y=VEC_Mag((VecFx32*)&l.clip._10);scale->z=VEC_Mag((VecFx32*)&l.clip._20);
{if(((s32*)&l.clip)[5]||((s32*)&l.clip)[6]){VEC_Normalize((VecFx32*)&l.clip._10,&matrix[1]);matrix[2].y=-matrix[1].z;matrix[2].z=matrix[1].y;}else{VEC_Normalize((VecFx32*)&l.clip._20,&matrix[2]);matrix[1].z=-matrix[2].y;matrix[1].y=matrix[2].z;}}
if(NNS_G3dGlb.flag&1){fifo(0x171012);MIi_CpuSend32(_02110B58,(void*)0x04000400,8);MIi_CpuSend32(NNS_G3dGlbGetInvWV(),(void*)0x04000400,48);fifo(0x1b19);MIi_CpuSend32(_02110B60,(void*)0x04000400,60);}else if(NNS_G3dGlb.flag&2){fifo(0x171012);MIi_CpuSend32(_02110B58,(void*)0x04000400,8);MIi_CpuSend32(NNS_G3dGlbGetInvV(),(void*)0x04000400,48);fifo(0x1b19);MIi_CpuSend32(_02110B60,(void*)0x04000400,60);}else MIi_CpuSend32(_02110B54,(void*)0x04000400,72);}
 if(timing==3){w->flags&=~0x40;w->bby(w);handled=w->flags&0x40;}else handled=0;
 if(option==0x20||option==0x60){length++;if(!handled&&!(w->flags&0x100)){l.pop=w->stream[2];NNS_G3dGeBufferOP_N(0x13,&l.pop,1);}}w->stream+=length;
}
