#include "global.h"
#include "sprite.h"
extern void GF_AssertFail(void);
void ov112_021F1D70(Sprite *, const void *, u32, u32);
void ov112_021F1D70(Sprite *sprite, const void *data, u32 size, u32 offset) {
    NNS_G2D_VRAM_TYPE type = Sprite_GetVramType(sprite);
    u32 base = NNS_G2dGetImageLocation(Sprite_GetImageProxy(sprite), type);
    DC_FlushRange(data, size);
    switch (type) {
    case 1:
        GX_LoadOBJ(data, base + offset, size);
        return;
    case 2:
        GXS_LoadOBJ(data, base + offset, size);
        return;
    default:
        GF_AssertFail();
        return;
    }
}
