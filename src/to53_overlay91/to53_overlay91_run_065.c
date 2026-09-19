#include "private_w20.h"

void ov91_02260A10(void) {
    NNS_G3dGlbPolygonAttr(0, GX_POLYGONMODE_MODULATE, GX_CULL_ALL, 0, 0x1f, 0);
}

void ov91_02260A2C(u8 *p) {
    *(u32 *)(p + 0xf8) = 0;
    *(u32 *)(p + 0xf4) = 0;
    Sprite_SetDrawFlag(*(void **)p, FALSE);
    sub_020182A0(p + 0x7c, 0);
}
