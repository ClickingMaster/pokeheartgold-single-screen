#include "to55_overlay91_run062_private.h"

void ov91_0226042C(const VecFx32 *position, u16 *out) {
    out[0] = 0xff - (position->x + 0x4b000) / 0x960;
    out[1] = (-0xe1000 - position->z) / 0x960;
}

void ov91_0226045C(const VecFx32 *src, VecFx32 *dst, int row, int column) {
    MtxFx33 matrix;
    u16 angle;

    MTX_Identity33_(&matrix);
    angle = ov91_02261D3C[column][row];
    MTX_RotY33_(&matrix, FX_SinIdx(angle), FX_CosIdx(angle));
    MTX_MultVec33(src, &matrix, dst);
}

void ov91_022604A4(const VecFx32 *src, VecFx32 *dst, int row, int column) {
    MtxFx33 matrix;
    u16 angle;

    MTX_Identity33_(&matrix);
    angle = ov91_02261D3C[column][row];
    MTX_RotY33_(&matrix, FX_SinIdx(angle), FX_CosIdx(angle));
    MTX_Inverse33(&matrix, &matrix);
    MTX_MultVec33(src, &matrix, dst);
}
