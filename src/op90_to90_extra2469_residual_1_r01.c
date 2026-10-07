#pragma force_active on
#include <nitro/types.h>
#include <nitro/fx/fx.h>
#include <nitro/fx/fx_const.h>
#include <nitro/hw/mmap_shared.h>
#include <nitro/hw/ARM9/io_reg.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
void rotTypeXYZ(s32 s, s32 c, s32 *m);
void drawXZPlane(fx32 s, fx32 t, fx32 x, fx32 z);
void drawXYPlane(fx32 s, fx32 t, fx32 x, fx32 y);
static inline void TexCoord(fx32 s, fx32 t) {
    reg_G3_TEXCOORD = (u16)(s >> 8) | ((u16)(t >> 8) << 16);
}
static inline void Vtx10(fx16 x, fx16 y, fx16 z) {
    reg_G3_VTX_10 = ((x >> 6) & 0x3ff) | (((y >> 6) & 0x3ff) << 10) | (((z >> 6) & 0x3ff) << 20);
}
// @fn rotTypeXYZ
void rotTypeXYZ(s32 s, s32 c, s32 *m) {
    s32 a = FX_MUL(FX32_ONE - c, 0x555);
    s32 b = FX_MUL(s, 0x93D);
    s32 diag, pos, neg;
    pos = a + b;
    diag = a + c;
    neg = a - b;
    m[0] = diag;
    m[3] = pos;
    m[6] = neg;
    m[9] = 0;
    m[1] = neg;
    m[4] = diag;
    m[7] = pos;
    m[10] = 0;
    m[2] = pos;
    m[5] = neg;
    m[8] = diag;
    m[11] = 0;
}
