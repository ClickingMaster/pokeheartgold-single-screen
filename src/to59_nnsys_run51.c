typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

void NNS_G3dMdlSetMdlDiff(void *model, u32 index, u32 value);
void NNS_G3dMdlSetMdlAmb(void *model, u32 index, u32 value);
void NNS_G3dMdlSetMdlSpec(void *model, u32 index, u32 value);
void NNS_G3dMdlSetMdlEmi(void *model, u32 index, u32 value);
void NNS_G3dMdlSetMdlLightEnableFlag(void *model, u32 index, u32 value);
void NNS_G3dMdlSetMdlPolygonMode(void *model, u32 index, u32 value);
void NNS_G3dMdlSetMdlCullMode(void *model, u32 index, u32 value);
void NNS_G3dMdlSetMdlPolygonID(void *model, u32 index, u32 value);
void NNS_G3dMdlSetMdlAlpha(void *model, u32 index, u32 value);
void NNS_G3dMdlSetMdlFogEnableFlag(void *model, u32 index, u32 value);
void NNS_G3dMdlSetMdlDepthTestCond(void *model, u32 index, u32 value);
void NNS_G3dMdlSetMdl1Dot(void *model, u32 index, u32 value);
void NNS_G3dMdlSetMdlFarClip(void *model, u32 index, u32 value);
void NNS_G3dMdlSetMdlXLDepthUpdate(void *model, u32 index, u32 value);

void NNS_G3dMdlSetMdlDiffAll(u8 *model, u32 value);
void NNS_G3dMdlSetMdlAmbAll(u8 *model, u32 value);
void NNS_G3dMdlSetMdlSpecAll(u8 *model, u32 value);
void NNS_G3dMdlSetMdlEmiAll(u8 *model, u32 value);
void NNS_G3dMdlSetMdlLightEnableFlagAll(u8 *model, u32 value);
void NNS_G3dMdlSetMdlPolygonModeAll(u8 *model, u32 value);
void NNS_G3dMdlSetMdlCullModeAll(u8 *model, u32 value);
void NNS_G3dMdlSetMdlPolygonIDAll(u8 *model, u32 value);
void NNS_G3dMdlSetMdlAlphaAll(u8 *model, u32 value);
void NNS_G3dMdlSetMdlFogEnableFlagAll(u8 *model, u32 value);
void NNS_G3dMdlSetMdlDepthTestCondAll(u8 *model, u32 value);
void NNS_G3dMdlSetMdl1DotAll(u8 *model, u32 value);
void NNS_G3dMdlSetMdlFarClipAll(u8 *model, u32 value);
void NNS_G3dMdlSetMdlXLDepthUpdateAll(u8 *model, u32 value);
void *NNS_G3dGetMdlSet(u8 *file);
void *NNS_G3dGetTex(u8 *file);
void *NNSi_G3dGetTexPatAnmTexNameByIdx(u8 *animation, u32 index);
void *NNSi_G3dGetTexPatAnmPlttNameByIdx(u8 *animation, u32 index);
void *NNSi_G3dGetTexPatAnmDataByIdx(u8 *animation, u32 index);
void getJntSRTAnmResult_(void *result, u8 *animation, int frame, u32 index);
void NNSi_G3dAnmCalcNsBca(void *result, u8 *animationObject, u32 index);

#define MAKE_ALL(name)                                       \
    void NNS_G3dMdlSetMdl##name##All(u8 *model, u32 value) { \
        u32 i;                                               \
        for (i = 0; i < model[0x18]; i++) {                  \
            NNS_G3dMdlSetMdl##name(model, i, value);         \
        }                                                    \
    }

MAKE_ALL(Diff)
MAKE_ALL(Amb)
MAKE_ALL(Spec)
MAKE_ALL(Emi)
MAKE_ALL(LightEnableFlag)
MAKE_ALL(PolygonMode)
MAKE_ALL(CullMode)
MAKE_ALL(PolygonID)
MAKE_ALL(Alpha)
MAKE_ALL(FogEnableFlag)
MAKE_ALL(DepthTestCond)
MAKE_ALL(1Dot)
MAKE_ALL(FarClip)
MAKE_ALL(XLDepthUpdate)
