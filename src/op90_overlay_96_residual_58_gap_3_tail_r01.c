#pragma force_active on
#include "global.h"
#include "sprite.h"
Sprite *ov96_021EB5B8(void *x);
void ov96_021F4DAC(u8 t, VecFx32 *pts, void *obj);
// @fn ov96_021F4DAC
void ov96_021F4DAC(u8 t, VecFx32 *pts, void *obj) {
    fx32 sx;
    fx32 sy;
    fx32 x;
    fx32 y;
    VecFx32 pos;
    Sprite *sprite;
    if (t <= 5) {
        sx = pts[0].x;
        sy = pts[0].y;
        x = pts[1].x - sx;
        x = sx + x * t / 5;
        y = pts[1].y - sy;
        y = sy + y * t / 5;
    } else if (t > 5 && t <= 45) {
        t -= 5;
        sx = pts[1].x;
        sy = pts[1].y;
        x = pts[2].x - sx;
        x = sx + x * t / 40;
        y = pts[2].y - sy;
        y = sy + y * t / 40;
    } else {
        t -= 45;
        sx = pts[2].x;
        sy = pts[2].y;
        x = pts[3].x - sx;
        x = sx + x * t / 5;
        y = pts[3].y - sy;
        y = sy + y * t / 5;
    }
    sprite = ov96_021EB5B8(obj);
    pos.x = x;
    pos.y = y + FX32_CONST(512);
    pos.z = 0;
    Sprite_SetMatrix(sprite, &pos);
}
