#include "to52_r6_overlay41_touch_setup_r32_private.h"
BOOL ov41_022481D8(void *unused, u32 x, u32 y) {
    TouchscreenHitbox hitbox;
    hitbox.rect.top = 0x12;
    hitbox.rect.bottom = 0x8f;
    hitbox.rect.left = 0x8a;
    hitbox.rect.right = 0xf6;
    return TouchscreenHitbox_PointIsIn(&hitbox, x, y);
}
