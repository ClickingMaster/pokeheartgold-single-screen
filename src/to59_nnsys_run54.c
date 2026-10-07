#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
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

void *NNSi_G3dGetTexPatAnmDataByIdx(u8 *animation, u32 index) {
    u8 *dictionary = animation + 0xc;
    if (dictionary != 0) {
        if (index < animation[0xd]) {
            u16 offset = *(u16 *)(animation + 0x12);
            u16 stride = *(u16 *)(dictionary + offset);
            return dictionary + offset + 4 + stride * index;
        }
    }
    return 0;
}
