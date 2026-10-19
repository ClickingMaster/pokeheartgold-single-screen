#ifndef POKEHEARTGOLD_MODEL_BOX_TEST_H
#define POKEHEARTGOLD_MODEL_BOX_TEST_H

#include <nnsys.h>

typedef struct BoxTestBounds {
    s16 width;
    s16 height;
    s16 depth;
    s16 padding_06;
    fx32 scaleX; // integer scale factors
    fx32 scaleY;
    fx32 scaleZ;
} BoxTestBounds;

BOOL GF3d_BoundsBoxTest(const VecFx32 *, const BoxTestBounds *);
BOOL GF3d_ModelBoxTest(NNSG3dResMdl *mdl, const VecFx32 *vec, const MtxFx33 *mtx, VecFx32 *scale);

#endif // POKEHEARTGOLD_MODEL_BOX_TEST_H
