#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include "renderer_joint_family_private.h"
extern void GX_BeginLoadTex(void),GX_EndLoadTex(void),GX_BeginLoadTexPltt(void),GX_EndLoadTexPltt(void);
extern void GX_LoadTex(const void *,u32,u32),GX_LoadTexPltt(const void *,u32,u32);
extern int MTX_Inverse43(const MtxFx43 *,MtxFx43 *);
extern void MTX_ScaleApply43(const MtxFx43 *,MtxFx43 *,fx32,fx32,fx32);
extern void MTX_Copy43To44_(const MtxFx43 *,MtxFx44 *);
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define B(p,o) (*(u8 *)((u8 *)(p)+(o)))
extern void updateHintVec_(u32 *,NNSG3dAnmObj *);
extern void addLink_(NNSG3dAnmObj **,NNSG3dAnmObj *);
extern void NNS_G3dGeBufferOP_N(u32,const u32 *,u32);
extern int mtx_inverse44(const MtxFx44 *,MtxFx44 *);
extern void (*_02110B9C[])(void *,int);
void NNS_G3dRenderObjAddAnmObj(NNSG3dRenderObj *p,NNSG3dAnmObj *a) ;
int removeLink_(NNSG3dAnmObj **head,NNSG3dAnmObj *item) ;
void NNS_G3dRenderObjRemoveAnmObj(NNSG3dRenderObj *p,NNSG3dAnmObj *a) ;
u32 NNS_G3dTexGetRequiredSize(const NNSG3dResTex *p) ;
u32 NNS_G3dTex4x4GetRequiredSize(const NNSG3dResTex *p) ;
u32 NNS_G3dPlttGetRequiredSize(const NNSG3dResTex *p) ;
void NNS_G3dTexSetTexKey(NNSG3dResTex *p,u32 a,u32 b) ;
void NNS_G3dTexLoad(NNSG3dResTex *p,BOOL begin) ;
void NNS_G3dTexReleaseTexKey(NNSG3dResTex *p,u32 *a,u32 *b) ;
void NNS_G3dPlttSetPlttKey(NNSG3dResTex *p,u32 key) ;
void NNS_G3dPlttLoad(NNSG3dResTex *p,BOOL begin) ;
u32 NNS_G3dPlttReleasePlttKey(NNSG3dResTex *p) ;
void NNS_G3dGlbFlushP(void) ;
void NNS_G3dGlbSetBaseTrans(const VecFx32 *v) ;
void NNS_G3dGlbSetBaseScale(const VecFx32 *v) ;
void NNS_G3dGlbLightVector(GXLightId id,fx16 x,fx16 y,fx16 z) ;
void NNS_G3dGlbLightColor(GXLightId id,GXRgb rgb) ;
void NNS_G3dGlbMaterialColorDiffAmb(GXRgb a,GXRgb b,BOOL flag) ;
void NNS_G3dGlbMaterialColorSpecEmi(GXRgb a,GXRgb b,BOOL flag) ;
void NNS_G3dGlbPolygonAttr(int a,GXPolygonMode b,GXCull c,int id,int alpha,int misc) ;
MtxFx43 *NNS_G3dGlbGetInvV(void) ;
MtxFx44 *NNS_G3dGlbGetInvP(void) ;
void calcSrtCameraMtx_(void) ;
MtxFx43 *NNS_G3dGlbGetWV(void) ;
MtxFx43 *NNS_G3dGlbGetInvWV(void) ;
MtxFx44 *NNS_G3dGlbGetInvVP(void) ;
void NNS_G3dGlbGetViewPort(u32 *a,u32 *b,u32 *c,u32 *d) ;
void blendScaleVec_(s32 *dst,const s32 *src,s32 ratio,int one) ;
void updateHintVec___g3d(u32 *p,NNSG3dAnmObj *a) ;
void NNSi_G3dFuncSbc_NOP(u8 *p) ;
void NNSi_G3dFuncSbc_RET(u8 *p) ;

extern void MIi_CpuClearFast(u32,u32 *,u32);
int NNSi_G3dAnmBlendMat(void *p,const NNSG3dAnmObj *a,u32 i);
int NNSi_G3dAnmBlendVis(u32 *p,const NNSG3dAnmObj *a,u32 i);
void NNS_G3dDraw(NNSG3dRenderObj *r);
void NNS_G3dGlbInit(void);

MtxFx44 *NNS_G3dGlbGetInvP(void) { if(!(NNS_G3dGlb.flag&16)){mtx_inverse44(&NNS_G3dGlb.projMtx,&NNS_G3dGlb.invProjMtx);NNS_G3dGlb.flag|=16;}return &NNS_G3dGlb.invProjMtx; }

void calcSrtCameraMtx_(void) { MTX_Concat43((MtxFx43 *)&NNS_G3dGlb.prmBaseRot,&NNS_G3dGlb.cameraMtx,&NNS_G3dGlb.srtCameraMtx);MTX_ScaleApply43(&NNS_G3dGlb.srtCameraMtx,&NNS_G3dGlb.srtCameraMtx,NNS_G3dGlb.prmBaseScale.x,NNS_G3dGlb.prmBaseScale.y,NNS_G3dGlb.prmBaseScale.z);MTX_Inverse43(&NNS_G3dGlb.srtCameraMtx,&NNS_G3dGlb.invSrtCameraMtx); }

MtxFx43 *NNS_G3dGlbGetWV(void) { if(!(NNS_G3dGlb.flag&128)){calcSrtCameraMtx_();NNS_G3dGlb.flag|=128;}return &NNS_G3dGlb.srtCameraMtx; }

MtxFx43 *NNS_G3dGlbGetInvWV(void) { if(!(NNS_G3dGlb.flag&128)){calcSrtCameraMtx_();NNS_G3dGlb.flag|=128;}return &NNS_G3dGlb.invSrtCameraMtx; }

MtxFx44 *NNS_G3dGlbGetInvVP(void) { MtxFx44 v;const MtxFx43 *a;const MtxFx44 *b;if(!(NNS_G3dGlb.flag&64)){a=NNS_G3dGlbGetInvV();b=NNS_G3dGlbGetInvP();MTX_Copy43To44_(a,&v);MTX_Concat44(b,&v,&NNS_G3dGlb.invCameraProjMtx);NNS_G3dGlb.flag|=64;}return &NNS_G3dGlb.invCameraProjMtx; }

void NNS_G3dGlbGetViewPort(u32 *a,u32 *b,u32 *c,u32 *d) { if(a)*a=NNS_G3dGlb.prmViewPort&255;if(b)*b=(NNS_G3dGlb.prmViewPort>>8)&255;if(c)*c=(NNS_G3dGlb.prmViewPort>>16)&255;if(d)*d=(NNS_G3dGlb.prmViewPort>>24)&255; }

int NNSi_G3dAnmBlendMat(void *p,const NNSG3dAnmObj *a,u32 i) { int result=0;if(!a)return result;do{if(i<a->numMapData){u16 v=a->mapData[i];if((v&0x300)==0x100 && a->funcAnm){((void (*)(void *,const NNSG3dAnmObj *,u32))a->funcAnm)(p,a,v&255);result=1;}}a=a->next;}while(a);return result; }

void blendScaleVec_(s32 *dst,const s32 *src,s32 ratio,int one) { if(one){dst[0]+=ratio;dst[1]+=ratio;dst[2]+=ratio;return;}dst[0]+=(ratio*src[0])>>12;dst[1]+=(ratio*src[1])>>12;dst[2]+=(ratio*src[2])>>12; }
