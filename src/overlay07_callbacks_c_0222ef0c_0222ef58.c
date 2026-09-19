#include "overlay07_callbacks_w07_private.h"

#define WORD(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define HALF(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void ov07_0222EF0C(void *sprite, u32 *state, fx32 speed) {
    *state = 0;
    ManagedSprite_SetDrawPriority(sprite, 0x64);
    ManagedSprite_SetPriority(sprite, 1);
    ManagedSprite_SetDrawFlag(sprite, TRUE);
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    ManagedSprite_SetAnimSpeed(sprite, speed);
    ManagedSprite_SetAnim(sprite, 0);
    ManagedSprite_SetAnimationFrame(sprite, 0);
    ManagedSprite_TickNFrames(sprite, 0x37000);
}
BOOL ov07_0222EF58(void *sprite, u32 *state) {
    BOOL ret = FALSE;
    switch (*state) {
    case 0:
        if (!ManagedSprite_IsAnimated(sprite)) {
            ret = TRUE;
            (*state)++;
        }
        break;
    case 1:
        ret = TRUE;
        break;
    default:
        ret = TRUE;
        break;
    }
    return ret;
}
