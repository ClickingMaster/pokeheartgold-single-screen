#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include "to91_tex_callback_41566_private.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
typedef struct Render Render;
struct Render{u8*cmd;u32 unk4,flags;u8 gapC[0x30];void(*env)(Render*);void(*prj)(Render*);u8 gap44[0x54];u8 envStage,prjStage;u8 gap9a[0x16];u8*mat;u8 gapb4[0x24];u8*resources;};
extern u32 _02110A9C[],_02110AA8[],_02110AA0[],_02110ACC[];
extern void NNS_G3dGetCurrentMtx(MtxFx43*,MtxFx33*),NNS_G3dGeFlushBuffer(void),NNS_G3dGeSendDL(const void*,u32);
extern MtxFx43*NNS_G3dGlbGetInvV(void);
static inline u8*lookup(u8*base,u32 index,int mat){u8*dict,*entry,*result;if(!base)goto null;dict=base+(mat?4:0);if(dict&&index<B(base,mat?5:1)){u16 off=H(base,mat?10:6),stride=H(dict,off);entry=dict+off+4+stride*index;}else entry=0;if(entry){result=base+W(entry,0);goto done;}null:result=0;done:return result;}
static inline u8*matrixData(u8*mat){u8*p=mat+0x2c;u16 flags=H(mat,0x1e);if(!(flags&2))p+=8;if(!(flags&4))p+=4;if(!(flags&8))p+=8;return p;}
typedef struct{u32 flags,pad[5],scaleS,scaleT;s16 sin,cos;u32 transS,transT;u16 width,height;u32 originalS,originalT;} TexResult;


void NNS_G3dDraw1Mat1Shp(const NNSG3dResMdl*resource,u32 matIndex,u32 shapeIndex,int useMat){u32 geometry[7];TexResult tex;u32 scale[3],inverse[3];u8*model=(u8*)resource,*mat,*shape,*p;u32 val;u16 flags;To91TexSend41566 fn;val=W(model,0x1c);if(val!=4096){scale[0]=val;scale[1]=val;scale[2]=val;NNS_G3dGeBufferOP_N(27,(const u32*)(scale),3);}if(useMat&&matIndex<B(model,0x18)){p=model&&W(model,8)?model+W(model,8):0;mat=lookup(p,matIndex,1);if(!(W(mat,12)&0x1f0000))return;geometry[0]=0x293130;geometry[1]=W(mat,4);geometry[2]=W(mat,8);geometry[3]=W(mat,12);if(H(mat,0x1e)&0x20)geometry[3]&=~0x1f0000;geometry[4]=0x2b2a;geometry[5]=W(mat,0x14);geometry[6]=H(mat,0x1c);NNS_G3dGeBufferOP_N(geometry[0],(const u32*)(geometry+1),6);if(H(mat,0x1e)&1){tex.flags=8;tex.width=H(mat,0x20);tex.height=H(mat,0x22);tex.originalS=W(mat,0x24);tex.originalT=W(mat,0x28);p=mat+0x2c;flags=H(mat,0x1e);if(flags&2)tex.flags|=1;else{tex.scaleS=W(p,0);tex.scaleT=W(p,4);p+=8;}flags=H(mat,0x1e);if(!(flags&4)){tex.sin=*(s16*)p;tex.cos=*(s16*)(p+2);p+=4;}else tex.flags|=2;flags=H(mat,0x1e);if(!(flags&8)){tex.transS=W(p,0);tex.transT=W(p,4);}else tex.flags|=4;fn=_02110A8C[B(model,0x16)];if(fn)fn(&tex);}}if(shapeIndex<B(model,0x19)){p=model&&W(model,12)?model+W(model,12):0;shape=lookup(p,shapeIndex,0);NNS_G3dGeSendDL(shape+W(shape,8),W(shape,12));}val=W(model,0x20);if(val!=4096){inverse[0]=val;inverse[1]=val;inverse[2]=val;NNS_G3dGeBufferOP_N(27,(const u32*)(inverse),3);}}
