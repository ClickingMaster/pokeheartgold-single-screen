#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
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
void G3dDrawInternal_Loop_(u8 *p) ;
void updateHintVec___g3d(u32 *p,NNSG3dAnmObj *a) ;
void NNSi_G3dFuncSbc_NOP(u8 *p) ;
void NNSi_G3dFuncSbc_RET(u8 *p) ;

extern u8 *_021DA700;
extern u32 _02110A80[],_02110A74[],_02110A8C[];
extern void MIi_CpuClearFast(u32,u32 *,u32);
int NNSi_G3dAnmBlendMat(void *p,const NNSG3dAnmObj *a,u32 i);
int NNSi_G3dAnmBlendVis(u32 *p,const NNSG3dAnmObj *a,u32 i);
void G3dDrawInternal_(u8 *p,NNSG3dRenderObj *r);
void NNS_G3dDraw(NNSG3dRenderObj *r);
void NNS_G3dGlbInit(void);

void NNS_G3dRenderObjRemoveAnmObj(NNSG3dRenderObj *p,NNSG3dAnmObj *a) { if(removeLink_(&p->anmMat,a)||removeLink_(&p->anmJnt,a)||removeLink_(&p->anmVis,a))p->flag|=16; }

u32 NNS_G3dTexGetRequiredSize(const NNSG3dResTex *p) { return p?H(p,12)*8:0; }

u32 NNS_G3dTex4x4GetRequiredSize(const NNSG3dResTex *p) { return p?H(p,28)*8:0; }

void NNS_G3dTexSetTexKey(NNSG3dResTex *p,u32 a,u32 b) { if(a)W(p,8)=a;if(b)W(p,24)=b; }
