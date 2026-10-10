#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <nnsys/g2d/g2d_RendererCore.h>
typedef GXOamAttr Oam;
extern u16 _02109310[3][4];
extern u16 _021092F8[3][4];
void NNS_G2dDrawOneOam3DDirectWithPosFast(int, int, int, Oam *, void *, int, int);
typedef NNSG2dRndCoreInstance Render;
extern void G3_LoadMtx43(const MtxFx43 *);
extern Render *_021DA498;
extern int NNS_G2dIsRndCoreFlipH(Render *);
extern int NNS_G2dIsRndCoreFlipV(Render *);

static inline int X(Oam *o) {
    int x = o->x;
    if (x > 255) {
        x = (s16)(x | 0xff00);
    }
    return x;
}

static inline int Y(Oam *o) {
    int y = o->y;
    if (y > 127) {
        y = (s16)(y | 0xff00);
    }
    return y;
}

void DrawOamToSurface3D_(Oam *);

void DrawOamToSurface3D_(Oam *o) {
    G3_LoadMtx43(&_021DA498->mtxFor3DGE);
    if (_021DA498->flipFlag) {
        u32 packed = o->attr01 & 0xc000c000;
        int x;
        u32 y, rawX;
        u32 horizontal, vertical;
        if (NNS_G2dIsRndCoreFlipH(_021DA498)) {
            u16 width = _02109310[(int)(packed & 0xc000) >> 14][(packed & 0xc0000000) >> 30];
            int px = X(o);
            px += width;
            px = -px;
            rawX = (u16)px;
        } else {
            rawX = (u16)X(o);
        }
        x = (s16)rawX;
        if (NNS_G2dIsRndCoreFlipV(_021DA498)) {
            u16 height = _021092F8[(int)(packed & 0xc000) >> 14][(packed & 0xc0000000) >> 30];
            int py = Y(o);
            py += height;
            py = -py;
            y = (u16)py;
        } else {
            y = (u16)Y(o);
        }
        horizontal = o->flipH;
        horizontal ^= NNS_G2dIsRndCoreFlipH(_021DA498);
        vertical = o->flipV;
        vertical ^= NNS_G2dIsRndCoreFlipV(_021DA498);
        o->attr01 &= ~0x10000000;
        o->attr01 |= horizontal << 28;
        o->attr01 &= ~0x20000000;
        o->attr01 |= vertical << 29;
        NNS_G2dDrawOneOam3DDirectWithPosFast(x, (s16)y, -1, o, (u8 *)_021DA498->pImgProxy + 12, _021DA498->baseTexAddr3D, _021DA498->basePltAddr3D);
    } else {
        NNS_G2dDrawOneOam3DDirectWithPosFast(X(o), Y(o), -1, o, (u8 *)_021DA498->pImgProxy + 12, _021DA498->baseTexAddr3D, _021DA498->basePltAddr3D);
    }
}
