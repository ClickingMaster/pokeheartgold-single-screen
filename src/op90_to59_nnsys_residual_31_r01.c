#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
#include <nnsys/g2d/g2d_Image.h>
typedef struct SpriteParams { fx32 u0, v0, u1, v1, w, h; } SpriteParams;
typedef void (*UVCallback)(fx32 *u0, fx32 *v0, fx32 *u1, fx32 *v1, BOOL flipH, BOOL flipV);
typedef struct OamSoftEmuState { UVCallback callback; fx32 z; BOOL autoZ; } OamSoftEmuState;
extern OamSoftEmuState _021DA47C;
extern const u16 _02109310[3][4];
extern const u16 _021092F8[3][4];
extern const int _02109344[];
extern const int _02109364[3][4];
extern const int _02109394[3][4];
extern const GXTexFmt _0210933C[];
void CalcSpriteParams_(const GXOamAttr *oam, const NNSG2dImageAttr *tex, u32 texBase, u32 plttBase, SpriteParams *out);

static inline void TexImageParam(GXTexFmt fmt, GXTexGen gen, GXTexSizeS s, GXTexSizeT t, GXTexRepeat rep, GXTexFlip flip, GXTexPlttColor0 pltt0, u32 addr)
{
    reg_G3_TEXIMAGE_PARAM = GX_PACK_TEXIMAGE_PARAM(fmt, gen, s, t, rep, flip, pltt0, addr);
}

static inline void TexPlttBase(u32 addr, GXTexFmt fmt)
{
    reg_G3_TEXPLTT_BASE = GX_PACK_TEXPLTTBASE_PARAM(addr, fmt);
}

// @fn CalcSpriteParams_
void CalcSpriteParams_(const GXOamAttr *oam, const NNSG2dImageAttr *tex, u32 texBase, u32 plttBase, SpriteParams *out)
{
    u32 shapeAttr = oam->attr01 & 0xC000C000;
    u32 size = (shapeAttr & 0xC0000000) >> 30;
    int shape = (int)(shapeAttr & 0xC000) >> 14;
    u16 charNo = oam->charNo;
    BOOL flipH, flipV;

    out->w = _02109310[shape][size];
    out->h = _021092F8[shape][size];

    if (tex->mappingType == 0) {
        TexImageParam(tex->fmt, GX_TEXGEN_TEXCOORD, tex->sizeS, tex->sizeT, GX_TEXREPEAT_NONE, GX_TEXFLIP_NONE, tex->plttUse, texBase);
        if (tex->fmt == GX_TEXFMT_PLTT256) {
            charNo >>= 1;
        }
        out->u0 = (charNo & (_02109344[tex->sizeS] - 1)) << FX32_SHIFT + 3;
        out->v0 = (charNo >> tex->sizeS) << FX32_SHIFT + 3;
    } else {
        u16 shape16 = shape;
        u16 size16 = size;
        u32 addr = texBase + (charNo << (((tex->mappingType & 0x700000) >> 20) + 5));
        reg_G3_TEXIMAGE_PARAM = (tex->fmt << 26) | (addr >> 3) | (GX_TEXGEN_TEXCOORD << 30) | (_02109364[shape16][size16] << 20) | (_02109394[shape16][size16] << 23) | (tex->plttUse << 29);
        out->u0 = 0;
        out->v0 = 0;
    }
    out->u1 = out->u0 + (out->w << FX32_SHIFT);
    out->v1 = out->v0 + (out->h << FX32_SHIFT);

    flipV = oam->flipV;
    flipH = oam->flipH;
    if (flipH) {
        fx32 t = out->u0;
        out->u0 = out->u1;
        out->u1 = t;
    }
    if (flipV) {
        fx32 t = out->v0;
        out->v0 = out->v1;
        out->v1 = t;
    }
    if (_021DA47C.callback) {
        _021DA47C.callback(&out->u0, &out->v0, &out->u1, &out->v1, flipH, flipV);
    }
    {
        GXTexFmt fmt = _0210933C[oam->colorMode];
        u16 pal = oam->cParam;
        u32 offset;
        if (tex->bExtendedPlt) {
            offset = pal << 9;
        } else if (fmt == GX_TEXFMT_PLTT256) {
            offset = 0;
        } else {
            offset = pal << 5;
        }
        TexPlttBase(plttBase + offset, fmt);
    }
}
