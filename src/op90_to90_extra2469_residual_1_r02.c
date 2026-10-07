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
// @fn drawXZPlane
void drawXZPlane(fx32 s, fx32 t, fx32 x, fx32 z) {
    reg_G3_BEGIN_VTXS = 1;
    TexCoord(0, 0);
    Vtx10((fx16)(x - FX32_ONE), 0, (fx16)(z + FX32_ONE));
    TexCoord(s, 0);
    Vtx10((fx16)(x + FX32_ONE), 0, (fx16)(z + FX32_ONE));
    TexCoord(s, t);
    Vtx10((fx16)(x + FX32_ONE), 0, (fx16)(z - FX32_ONE));
    TexCoord(0, t);
    Vtx10((fx16)(x - FX32_ONE), 0, (fx16)(z - FX32_ONE));
    reg_G3_END_VTXS = 0;
}
