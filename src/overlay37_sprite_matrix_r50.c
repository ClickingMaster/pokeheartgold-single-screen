#include "global.h"

#include "sprite.h"

void ov37_021E657C(Sprite *sprite, s32 x, s32 y);

void ov37_021E657C(Sprite *sprite, s32 x, s32 y) {
    VecFx32 vec;
    float value;

    if (x > 0) {
        value = (float)(x << FX32_SHIFT) + 0.5f;
    } else {
        value = (float)(x << FX32_SHIFT) - 0.5f;
    }
    vec.x = (s32)value;
    y -= 8;
    if (y > 0) {
        value = (float)(y << FX32_SHIFT) + 0.5f;
    } else {
        value = (float)(y << FX32_SHIFT) - 0.5f;
    }
    vec.y = (s32)value;
    vec.z = 0;
    Sprite_SetMatrix(sprite, &vec);
}
