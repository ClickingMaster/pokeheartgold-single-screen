// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
typedef struct Render Render;
struct Render{u8*cmd;u32 unk4,flags;u8 gapC[0x30];void(*env)(Render*);void(*prj)(Render*);u8 gap44[0x54];u8 envStage,prjStage;u8 gap9a[0x16];u8*mat;u8 gapb4[0x24];u8*resources;};
extern u32 _02110A9C[],_02110AA8[],_02110AA0[],_02110ACC[];
extern void NNS_G3dGetCurrentMtx(MtxFx43*,MtxFx33*),NNS_G3dGeFlushBuffer(void),NNS_G3dGeSendDL(const void*,u32);
extern MtxFx43*NNS_G3dGlbGetInvV(void);
static inline u8*lookup(u8*base,u32 index,int mat){u8*dict,*entry,*result;if(!base)goto null;dict=base+(mat?4:0);if(dict&&index<B(base,mat?5:1)){u16 off=H(base,mat?10:6);entry=dict+off+4;u16 stride=H(dict,off);entry+=stride*index;}else entry=0;if(entry){result=base+W(entry,0);goto done;}null:result=0;done:return result;}
static inline u8*matrixData(u8*mat){u8*p=mat+0x2c;u16 flags=H(mat,0x1e);if(!(flags&2))p+=8;if(!(flags&4))p+=4;if(!(flags&8))p+=8;return p;}
extern int G3X_GetClipMtx(MtxFx44*);
void NNSi_G3dFuncSbc_PRJMAP(Render*,u32);
static inline void ClearRegs(volatile u32 *p){p[0]=0;p[1]=0;p[5]=0;}
static inline void MtxStore(u32 n){u32 data=n;NNS_G3dGeBufferOP_N(19,&data,1);}
static inline void MtxMode(u32 m){u32 data=m;NNS_G3dGeBufferOP_N(16,&data,1);}
static inline void MtxRestore(u32 n){u32 data=n;NNS_G3dGeBufferOP_N(20,&data,1);}
static inline void TexCoord(fx32 s,fx32 t){u32 data=(u16)(s16)(s>>8)|((u16)(s16)(t>>8)<<16);NNS_G3dGeBufferOP_N(34,&data,1);}
#pragma opt_propagation off
// @fn NNSi_G3dFuncSbc_PRJMAP
void NNSi_G3dFuncSbc_PRJMAP(Render*s,u32 opt){MtxFx43 current;MtxFx44 clip;u32 flags,step,skip;u8*mat;u32 w,h;volatile u32*regs;flags=s->flags;if(flags&0x200)goto end;if(!(flags&1))goto end;NNS_G3dGetCurrentMtx(&current,0);MtxStore(30);if((W(s->mat,16)&0xc0000000)!=0xc0000000){W(s->mat,16)&=~0xc0000000;W(s->mat,16)|=0xc0000000;_02110A9C[1]=W(s->mat,16);NNS_G3dGeBufferOP_N(_02110A9C[0],(const u32*)(_02110AA0),1);}step=s->prj?s->prjStage:0;if(step==1){s->flags&=~0x40;s->prj(s);step=s->prj?s->prjStage:0;skip=s->flags&0x40;}else skip=0;if(!skip){h=H(s->mat,0x2e);w=H(s->mat,0x2c);_02110A9C[0x30/4]=w<<15;_02110A9C[0x44/4]=(u32)(-h)<<15;_02110A9C[0x60/4]=w<<15;_02110A9C[0x64/4]=h<<15;NNS_G3dGeBufferOP_N(22,(const u32*)(_02110ACC),16);}if(step==2){s->flags&=~0x40;s->prj(s);step=s->prj?s->prjStage:0;skip=s->flags&0x40;}else skip=0;if(!skip){mat=lookup(s->resources,B(s->cmd,1),1);if(H(mat,0x1e)&0x2000)NNS_G3dGeBufferOP_N(24,(const u32*)(matrixData(mat)),16);}if(step==3){s->flags&=~0x40;s->prj(s);skip=s->flags&0x40;}else skip=0;if(!skip){flags=NNS_G3dGlb.flag;if(flags&1){NNS_G3dGeBufferOP_N(28,(const u32*)(&NNS_G3dGlb.prmBaseTrans),3);NNS_G3dGeBufferOP_N(26,(const u32*)(&NNS_G3dGlb.prmBaseRot),9);NNS_G3dGeBufferOP_N(25,(const u32*)(&current),12);}else if(flags&2)NNS_G3dGeBufferOP_N(25,(const u32*)(&current),12);else{NNS_G3dGeBufferOP_N(25,(const u32*)(NNS_G3dGlbGetInvV()),12);NNS_G3dGeBufferOP_N(25,(const u32*)(&current),12);}NNS_G3dGeFlushBuffer();ClearRegs((volatile u32*)0x04000440);do{}while(G3X_GetClipMtx(&clip));regs=(volatile u32*)0x04000448;regs[0]=1;regs[-2]=3;NNS_G3dGeBufferOP_N(22,(const u32*)(&clip),16);TexCoord(clip._30>>4,clip._31>>4);}MtxMode(2);MtxRestore(30);end:s->cmd+=3;}
