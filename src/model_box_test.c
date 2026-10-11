#include "model_box_test.h"

#include <nnsys.h>

#include "global.h"

// G3OP_BOX_TEST parameters
typedef struct BoxTestParams {
    s16 x;
    s16 y;
    s16 z;
    s16 width;
    s16 height;
    s16 depth;
} BoxTestParams;

typedef struct BoxTestBounds {
    s16 width;
    s16 height;
    s16 depth;
    s16 pad06;
    fx32 scaleX; // integer scale factors
    fx32 scaleY;
    fx32 scaleZ;
} BoxTestBounds;

// not declared in lib/include
int G3X_GetBoxTestResult(u32 *result);

BOOL GF3d_BoundsBoxTest(const VecFx32 *translation, const BoxTestBounds *bounds);
void GF3d_SetBoxTestParams(s16 x, s16 y, s16 z, s16 width, s16 height, s16 depth, BoxTestParams *box);
BOOL GF3d_RunBoxTest(const BoxTestParams *box);

BOOL GF3d_ModelBoxTest(NNSG3dResMdl *model, const VecFx32 *translation, const MtxFx33 *rotation, VecFx32 *baseScale) {
    BoxTestParams box;
    VecFx32 trans = *translation;
    NNSG3dResMdlInfo *info = model != NULL ? &model->info : NULL;
    u32 scale[3];
    u32 scaleValue;
    u32 one;
    BOOL result;

    GF3d_SetBoxTestParams(info->boxX, info->boxY, info->boxZ, info->boxW, info->boxH, info->boxD, &box);
    NNS_G3dGlbSetBaseTrans(&trans);
    MI_Copy36B(rotation, &NNS_G3dGlb.prmBaseRot);
    NNS_G3dGlb.flag &= ~0xA4;
    NNS_G3dGlbSetBaseScale(baseScale);
    NNS_G3dGlbFlushP();
    NNS_G3dGeBufferOP_N(G3OP_MTX_PUSH, NULL, 0);
    scaleValue = info->boxPosScale;
    scale[0] = scaleValue;
    scale[1] = scaleValue;
    scale[2] = scaleValue;
    NNS_G3dGeBufferOP_N(G3OP_MTX_SCALE, scale, 3);
    result = GF3d_RunBoxTest(&box);
    one = 1;
    NNS_G3dGeBufferOP_N(G3OP_MTX_POP, &one, 1);
    return result;
}

BOOL GF3d_BoundsBoxTest(const VecFx32 *translation, const BoxTestBounds *bounds) {
    BoxTestParams box;
    u32 one;
    BOOL result;

    GF3d_SetBoxTestParams(0, 0, 0, bounds->width, bounds->height, bounds->depth, &box);
    NNS_G3dGlbSetBaseTrans(translation);
    NNS_G3dGlbFlushP();
    NNS_G3dGeBufferOP_N(G3OP_MTX_PUSH, NULL, 0);
    {
        fx32 scaleZ = bounds->scaleZ << 12;
        fx32 scaleY = bounds->scaleY << 12;
        fx32 scaleX = bounds->scaleX << 12;
        VecFx32 scale = { scaleX, scaleY, scaleZ };
        NNS_G3dGeBufferOP_N(G3OP_MTX_SCALE, (const u32 *)&scale, 3);
    }
    result = GF3d_RunBoxTest(&box);
    one = 1;
    NNS_G3dGeBufferOP_N(G3OP_MTX_POP, &one, 1);
    return result;
}

void GF3d_SetBoxTestParams(s16 x, s16 y, s16 z, s16 width, s16 height, s16 depth, BoxTestParams *box) {
    box->x = x;
    box->y = y;
    box->z = z;
    box->width = width;
    box->height = height;
    box->depth = depth;
}

BOOL GF3d_RunBoxTest(const BoxTestParams *box) {
    u32 result = 1;
    u32 polygonAttr = 0x30C1;
    u32 zero;

    NNS_G3dGeBufferOP_N(G3OP_POLYGON_ATTR, &polygonAttr, 1);
    zero = 0;
    NNS_G3dGeBufferOP_N(G3OP_BEGIN, &zero, 1);
    NNS_G3dGeBufferOP_N(G3OP_END, NULL, 0);
    NNS_G3dGeBufferOP_N(G3OP_BOX_TEST, (const u32 *)box, 3);
    NNS_G3dGeFlushBuffer();
    while (G3X_GetBoxTestResult(&result) != 0) {
    }
    return result;
}
