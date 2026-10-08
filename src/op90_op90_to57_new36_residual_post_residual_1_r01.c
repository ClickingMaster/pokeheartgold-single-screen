#pragma force_active on
#include "global.h"
#include "sprite_system.h"
BOOL ov07_022222F0(void *state, ManagedSprite *sprite);
BOOL ov07_0222FF04(ManagedSprite *sprite, void *state);
// @fn ov07_0222FF04
BOOL ov07_0222FF04(ManagedSprite *sprite, void *state) {
    s16 x;
    s16 y;
    s16 angle;
    fx32 sin;
    f32 scale;
    if (ov07_022222F0(state, sprite)) {
        ManagedSprite_GetPositionXY(sprite, &x, &y);
        angle = x;
        sin = FX_SinIdx((u16)(angle * 364));
        ManagedSprite_SetPositionXY(sprite, angle, y + (s16)(FX_Mul(sin, 14 * FX32_ONE) >> FX32_SHIFT));
        scale = 1.0f + (f32)(-FX_Mul(sin, 696)) / 4096.0f;
        ManagedSprite_SetAffineScale(sprite, scale, scale);
        return FALSE;
    }
    return TRUE;
}
