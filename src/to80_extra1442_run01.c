#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern fx64c FX_GetDivResultFx64c(void);
extern fx32 FX_GetDivResult(void);
extern void FX_InvAsync(fx32);

static inline void inv(fx32 x) {
    volatile u64 *r = (volatile u64 *)0x04000290;
    r[0] = (u64)4096 << 32;
    r[1] = (u32)x;
}

static inline void divfixed(fx32 a, fx32 b) {
    u32 *r = (u32 *)0x04000290;
    r[0] = 0;
    r[1] = a;
    r[2] = b;
    r[3] = 0;
}

static inline fx32 mul32(fx32 a, fx32 b) {
    return (fx32)(((s64)a * b + 2048) >> 12);
}

static inline fx64c applyScale(fx64c a, fx32 b) {
    fx64c product = a * b;
    return product / 4096;
}

void MTX_OrthoW(fx32 top, fx32 bottom, fx32 left, fx32 right, fx32 near, fx32 far, fx32 scale, MtxFx44 *m) {
    fx64c x, y, z;
    FX_InvAsync(right - left);
    m->_01 = 0;
    m->_02 = 0;
    m->_03 = 0;
    m->_10 = 0;
    m->_12 = 0;
    m->_13 = 0;
    m->_20 = 0;
    m->_21 = 0;
    m->_23 = 0;
    m->_33 = scale;
    x = FX_GetDivResultFx64c();
    inv(top - bottom);
    if (scale != 4096) {
        x = x * scale / 4096;
    }
    m->_00 = (fx32)(((x << 13) + 0x80000000LL) >> 32);
    y = FX_GetDivResultFx64c();
    inv(near - far);
    if (scale != 4096) {
        y = y * scale / 4096;
    }
    m->_11 = (fx32)(((y << 13) + 0x80000000LL) >> 32);
    z = FX_GetDivResultFx64c();
    if (scale != 4096) {
        z = z * scale / 4096;
    }
    m->_22 = (fx32)(((z << 13) + 0x80000000LL) >> 32);
    m->_30 = (fx32)((x * -(right + left) + 0x80000000LL) >> 32);
    m->_31 = (fx32)((y * -(top + bottom) + 0x80000000LL) >> 32);
    m->_32 = (fx32)((z * (far + near) + 0x80000000LL) >> 32);
}
