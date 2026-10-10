#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    fx32 x, y;
    u8 pad[12];
    NNS_G2D_VRAM_TYPE type;
} Surface;

typedef struct {
    Surface *surface;
    u32 pad;
    NNSG2dImageProxy *image;
    NNSG2dImagePaletteProxy *palette;
    u32 offset, imageLocation, paletteLocation;
} Core;

extern BOOL NNS_G2dIsImageReadyToUse(const NNSG2dImageProxy *, NNS_G2D_VRAM_TYPE);
extern BOOL NNS_G2dIsImagePaletteReadyToUse(const NNSG2dImagePaletteProxy *, NNS_G2D_VRAM_TYPE);
extern Core *_021DA498;
void NNS_G2dRndCoreBeginRendering(Core *);

void NNS_G2dRndCoreBeginRendering(Core *c) {
    NNS_G2D_VRAM_TYPE type;
    _021DA498 = c;
    type = c->surface->type;
    if (type == 0) {
        *(volatile u32 *)0x04000440 = 0;
        *(volatile u32 *)0x04000444 = 0;
        Surface *s = c->surface;
        u32 x = s->x, y = s->y;
        *(volatile u32 *)0x04000470 = 0u - x;
        *(volatile u32 *)0x04000470 = 0u - y;
        *(volatile u32 *)0x04000470 = 0;
        *(volatile u32 *)0x04000440 = 1;
        {
            NNSG2dImageProxy *p = _021DA498->image;
            NNSG2dImagePaletteProxy *q;
            c->imageLocation = NNS_G2dIsImageReadyToUse(p, NNS_G2D_VRAM_TYPE_3DMAIN) ? NNS_G2dGetImageLocation(p, NNS_G2D_VRAM_TYPE_3DMAIN) : 0;
            q = _021DA498->palette;
            c->paletteLocation = NNS_G2dIsImagePaletteReadyToUse(q, NNS_G2D_VRAM_TYPE_3DMAIN) ? NNS_G2dGetImagePaletteLocation(q, NNS_G2D_VRAM_TYPE_3DMAIN) : 0;
        }
    } else {
        NNSG2dImageProxy *p = c->image;
        u32 value = NNS_G2dIsImageReadyToUse(p, type) ? NNS_G2dGetImageLocation(p, type) >> (((*(s32 *)((u8 *)p + 32) & 0x700000) >> 20) + 5) : 0;
        _021DA498->offset = value;
    }
}
