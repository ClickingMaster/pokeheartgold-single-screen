#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "renderer_joint_family_private.h"

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
void NNS_G3dMdlSetMdlDepthTestCondAll(u8 *model, u32 value);
void NNS_G3dMdlSetMdl1DotAll(u8 *model, u32 value);
void NNS_G3dMdlSetMdlFarClipAll(u8 *model, u32 value);
void NNS_G3dMdlSetMdlXLDepthUpdateAll(u8 *model, u32 value);
void *NNSi_G3dGetTexPatAnmTexNameByIdx(u8 *animation, u32 index);
void *NNSi_G3dGetTexPatAnmPlttNameByIdx(u8 *animation, u32 index);
void *NNSi_G3dGetTexPatAnmDataByIdx(u8 *animation, u32 index);

NNSG3dResMdlSet *NNS_G3dGetMdlSet(const NNSG3dResFileHeader *header) {
    const u8 *file = (const u8 *)header;
    u16 offset = *(const u16 *)(file + 0xc);
    return (NNSG3dResMdlSet *)(file + *(const u32 *)(file + offset));
}

NNSG3dResTex *NNS_G3dGetTex(const NNSG3dResFileHeader *header) {
    const u8 *file = (const u8 *)header;
    u16 offset = *(const u16 *)(file + 0xc);
    u16 count = *(const u16 *)(file + 0xe);
    const u8 *record = file + offset;
    if (count == 1) {
        if (*(const u32 *)file == 0x30585442) {
            return (NNSG3dResTex *)(file + *(const u32 *)record);
        }
        return 0;
    }
    return (NNSG3dResTex *)(file + *(const u32 *)(record + 4));
}
