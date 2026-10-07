#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "math_util.h"

typedef struct Effect {
    ManagedSprite *sprite;
    int active;
    fx32 vx;
    fx32 vy;
    s16 x;
    s16 y;
} Effect;

void ov96_0220B1D8(Effect *effects, u8 num, s16 x, s16 y);

// @fn ov96_0220B1D8
void ov96_0220B1D8(Effect *effects, u8 num, s16 x, s16 y) {
    int i;
    int count = 0;
    for (i = 0; i < 16; i++, effects++) {
        if (effects->active == 0) {
            effects->active = 1;
            effects->x = x;
            effects->y = y;
            effects->vx = FX32_CONST(MTRandom() % 8 + 4) * (i % 2 == 0 ? 1 : -1);
            effects->vy = FX32_CONST(MTRandom() % 8 + 8);
            ManagedSprite_SetPositionXY(effects->sprite, x, y);
            ManagedSprite_ResetSpriteAnimCtrlState(effects->sprite);
            ManagedSprite_SetDrawFlag(effects->sprite, 1);
            count++;
        }
        if (count >= num) {
            break;
        }
    }
}
