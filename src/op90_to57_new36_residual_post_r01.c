#pragma force_active on
#include "global.h"
#include "sprite_system.h"
void ov07_02222268(void *motion, s16 x0, s16 x1, s16 y0, s16 y1, int frames);
void ov07_0222FEB0(ManagedSprite *sprite, void *motion, int dx, int dy);

// @fn ov07_0222FEB0
void ov07_0222FEB0(ManagedSprite *sprite, void *motion, int dx, int dy) {
    s16 x, y;
    int ix, iy;
    ManagedSprite_GetPositionXY(sprite, &x, &y);
    iy = y; ix = x;
    ov07_02222268(motion, ix, ix + (dx << 8), iy, iy + dy * -128, 59);
    ManagedSprite_SetDrawFlag(sprite, 1);
}
