#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
fx32 FX_Div(fx32 numerator, fx32 denominator);
s64 FX_GetDivResultFx64c(void);
fx32 FX_GetDivResult(void);
void MTX_PerspectiveW(fx32 fovySin, fx32 fovyCos, fx32 aspect, fx32 n, fx32 f, fx32 scaleW, MtxFx44 *mtx);
static inline void DivAsyncShift(fx32 numer, fx32 denom) {
    *(s64 *)REG_DIV_NUMER_ADDR = (s64)numer << 32;
    *(u64 *)REG_DIV_DENOM_ADDR = (u32)denom;
}
// @fn MTX_PerspectiveW
void MTX_PerspectiveW(fx32 fovySin, fx32 fovyCos, fx32 aspect, fx32 n, fx32 f, fx32 scaleW, MtxFx44 *mtx) {
    fx32 fovyCot;
    s64 t;
    fovyCot = FX_Div(fovyCos, fovySin);
    DivAsyncShift(FX32_ONE, n - f);
    if (scaleW != FX32_ONE) {
        fovyCot = (fovyCot * scaleW) / FX32_ONE;
    }
    mtx->_01 = 0;
    mtx->_02 = 0;
    mtx->_03 = 0;
    mtx->_10 = 0;
    mtx->_11 = fovyCot;
    mtx->_12 = 0;
    mtx->_13 = 0;
    mtx->_20 = 0;
    mtx->_21 = 0;
    mtx->_23 = -scaleW;
    mtx->_30 = 0;
    mtx->_31 = 0;
    mtx->_33 = 0;
    t = FX_GetDivResultFx64c();
    DivAsyncShift(fovyCot, aspect);
    if (scaleW != FX32_ONE) {
        t = (t * scaleW) / FX32_ONE;
    }
    mtx->_22 = (fx32)((t * (f + n) + 0x80000000LL) >> 32);
    mtx->_32 = (fx32)((t * (fx32)((((s64)(n << 1) * f) + 0x800) >> 12) + 0x80000000LL) >> 32);
    mtx->_00 = FX_GetDivResult();
}
