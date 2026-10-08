#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct SpriteParams { fx32 u0, v0, u1, v1, w, h; } SpriteParams;
typedef struct OamSoftEmuState { void *callback; fx32 z; BOOL autoZ; } OamSoftEmuState;
extern OamSoftEmuState _021DA47C;
extern fx32 _021109FC;
extern const u16 _02109310[3][4];
extern const u16 _021092F8[3][4];
void CalcSpriteParams_(const GXOamAttr *oam, void *a, void *b, void *c, SpriteParams *params);
void NNS_G2dDrawOneOam3DDirectWithPosFast(int x, int y, int z, const GXOamAttr *oam, void *a, void *b, void *c);

static inline fx32 GetZ(int z)
{
    if (_021DA47C.autoZ) {
        return _021DA47C.z + (z << FX32_SHIFT);
    }
    return z << FX32_SHIFT;
}

static inline void Scale(fx32 x, fx32 y, fx32 z)
{
    reg_G3_MTX_SCALE = (u32)x;
    reg_G3_MTX_SCALE = (u32)y;
    reg_G3_MTX_SCALE = (u32)z;
}

static inline void TexCoord(fx32 s, fx32 t)
{
    s16 tt = (s16)(t >> 8);
    s16 ss = (s16)(s >> 8);
    reg_G3_TEXCOORD = (u16)ss | ((u32)(u16)tt << 16);
}

// @fn NNS_G2dDrawOneOam3DDirectWithPosFast
void NNS_G2dDrawOneOam3DDirectWithPosFast(int x, int y, int z, const GXOamAttr *oam, void *a, void *b, void *c)
{
    SpriteParams p;
    u32 mode;
    CalcSpriteParams_(oam, a, b, c, &p);
    mode = oam->attr01 & 0x300;
    if (mode != 0x100 && mode != 0x300) {
        mode |= oam->attr01 & 0x30000000;
    }
    if (mode == 0x300) {
        u32 attr = oam->attr01 & 0xC000C000;
        u32 size;
        int shape;
        int hh;
        int hw;
        shape = (int)(attr & 0xC000) >> 14;
        size = (attr & 0xC0000000) >> 30;
        hw = _02109310[shape][size] >> 1;
        hh = _021092F8[shape][size] >> 1;
        G3_Translate((x + hw) << FX32_SHIFT, (y + hh) << FX32_SHIFT, GetZ(z));
    } else {
        G3_Translate(x << FX32_SHIFT, y << FX32_SHIFT, GetZ(z));
    }
    Scale(p.w << FX32_SHIFT, p.h << FX32_SHIFT, FX32_ONE);
    reg_G3_BEGIN_VTXS = 1;
    TexCoord(p.u0, p.v1);
    reg_G3_VTX_10 = 0x10000;
    TexCoord(p.u1, p.v1);
    reg_G3_VTX_10 = 0x10040;
    TexCoord(p.u1, p.v0);
    reg_G3_VTX_10 = 0x40;
    TexCoord(p.u0, p.v0);
    reg_G3_VTX_10 = 0;
    reg_G3_END_VTXS = 0;
    if (_021DA47C.autoZ) {
        _021DA47C.z += _021109FC;
    }
}
