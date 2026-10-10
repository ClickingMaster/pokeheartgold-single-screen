#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "renderer_joint_family_private.h"
extern void GX_BeginLoadTex(void);
extern void GX_EndLoadTex(void);
extern void GX_BeginLoadTexPltt(void);
extern void GX_EndLoadTexPltt(void);
extern void GX_LoadTex(const void *, u32, u32);
extern void GX_LoadTexPltt(const void *, u32, u32);
extern int MTX_Inverse43(const MtxFx43 *, MtxFx43 *);
extern void MTX_ScaleApply43(const MtxFx43 *, MtxFx43 *, fx32, fx32, fx32);
extern void MTX_Copy43To44_(const MtxFx43 *, MtxFx44 *);
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define B(p, o) (*(u8 *)((u8 *)(p) + (o)))
extern void updateHintVec_(u32 *, NNSG3dAnmObj *);
extern void addLink_(NNSG3dAnmObj **, NNSG3dAnmObj *);
extern void NNS_G3dGeBufferOP_N(u32, const u32 *, u32);
extern int mtx_inverse44(const MtxFx44 *, MtxFx44 *);
extern void (*_02110B9C[])(void *, int);
void NNS_G3dRenderObjAddAnmObj(NNSG3dRenderObj *p, NNSG3dAnmObj *a);
int removeLink_(NNSG3dAnmObj **head, NNSG3dAnmObj *item);
void NNS_G3dRenderObjRemoveAnmObj(NNSG3dRenderObj *p, NNSG3dAnmObj *a);
u32 NNS_G3dTexGetRequiredSize(const NNSG3dResTex *p);
u32 NNS_G3dTex4x4GetRequiredSize(const NNSG3dResTex *p);
u32 NNS_G3dPlttGetRequiredSize(const NNSG3dResTex *p);
void NNS_G3dTexSetTexKey(NNSG3dResTex *p, u32 a, u32 b);
void NNS_G3dTexLoad(NNSG3dResTex *p, BOOL begin);
void NNS_G3dTexReleaseTexKey(NNSG3dResTex *p, u32 *a, u32 *b);
void NNS_G3dPlttSetPlttKey(NNSG3dResTex *p, u32 key);
void NNS_G3dPlttLoad(NNSG3dResTex *p, BOOL begin);
u32 NNS_G3dPlttReleasePlttKey(NNSG3dResTex *p);
void NNS_G3dGlbFlushP(void);
void NNS_G3dGlbSetBaseTrans(const VecFx32 *v);
void NNS_G3dGlbSetBaseScale(const VecFx32 *v);
void NNS_G3dGlbLightVector(GXLightId id, fx16 x, fx16 y, fx16 z);
void NNS_G3dGlbLightColor(GXLightId id, GXRgb rgb);
void NNS_G3dGlbMaterialColorDiffAmb(GXRgb a, GXRgb b, BOOL flag);
void NNS_G3dGlbMaterialColorSpecEmi(GXRgb a, GXRgb b, BOOL flag);
void NNS_G3dGlbPolygonAttr(int a, GXPolygonMode b, GXCull c, int id, int alpha, int misc);
MtxFx43 *NNS_G3dGlbGetInvV(void);
MtxFx44 *NNS_G3dGlbGetInvP(void);
void calcSrtCameraMtx_(void);
MtxFx43 *NNS_G3dGlbGetWV(void);
MtxFx43 *NNS_G3dGlbGetInvWV(void);
MtxFx44 *NNS_G3dGlbGetInvVP(void);
void NNS_G3dGlbGetViewPort(u32 *a, u32 *b, u32 *c, u32 *d);
void blendScaleVec_(s32 *dst, const s32 *src, s32 ratio, int one);
void updateHintVec___g3d(u32 *p, NNSG3dAnmObj *a);
void NNSi_G3dFuncSbc_NOP(u8 *p);
void NNSi_G3dFuncSbc_RET(u8 *p);

extern void MIi_CpuClearFast(u32, u32 *, u32);
int NNSi_G3dAnmBlendMat(void *p, const NNSG3dAnmObj *a, u32 i);
int NNSi_G3dAnmBlendVis(u32 *p, const NNSG3dAnmObj *a, u32 i);
void NNS_G3dDraw(NNSG3dRenderObj *r);
void NNS_G3dGlbInit(void);

void NNS_G3dRenderObjAddAnmObj(NNSG3dRenderObj *p, NNSG3dAnmObj *a) {
    if (!a || !a->resAnm) {
        return;
    }
    switch (*(u8 *)a->resAnm) {
    case 0x4d:
        updateHintVec_(p->hintMatAnmExist, a);
        addLink_(&p->anmMat, a);
        break;
    case 0x4a:
        updateHintVec_(p->hintJntAnmExist, a);
        addLink_(&p->anmJnt, a);
        break;
    case 0x56:
        updateHintVec_(p->hintVisAnmExist, a);
        addLink_(&p->anmVis, a);
        break;
    }
}
