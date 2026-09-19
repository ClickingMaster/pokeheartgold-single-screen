#include "global.h"

typedef struct ManagedSprite ManagedSprite;
extern const u16 ov93_02262C7A[];
void ManagedSprite_GetPositionXYWithSubscreenOffset(ManagedSprite *, s16 *, s16 *, fx32);
void ManagedSprite_SetPositionXYWithSubscreenOffset(ManagedSprite *, s16, s16, fx32);
void ManagedSprite_SetDrawFlag(ManagedSprite *, BOOL);
void ManagedSprite_SetAnim(ManagedSprite *, u32);
void ManagedSprite_TickNFrames(ManagedSprite *, fx32);
void PlaySE(u32);

typedef struct Ov93Anim {
    ManagedSprite *sprite;
    ManagedSprite *effect;
    fx32 x;
    fx32 y;
    s16 targetY;
    u8 kind;
    u8 state;
    u8 timer;
    u8 active;
    s16 delay;
} Ov93Anim;

BOOL ov93_02261D3C(void *, Ov93Anim *);
BOOL ov93_02261D3C(void *unused, Ov93Anim *anim) {
    s16 x, y;
    switch (anim->state) {
    case 0:
        ManagedSprite_GetPositionXYWithSubscreenOffset(anim->sprite, &x, &y, 0x160000);
        anim->x = x << FX32_SHIFT;
        anim->y = y << FX32_SHIFT;
        anim->state++;
    case 1:
        if (anim->delay > 0) {
            anim->delay--;
            break;
        }
        ManagedSprite_SetDrawFlag(anim->sprite, TRUE);
        anim->y += 0x8000;
        if (anim->y / FX32_ONE >= anim->targetY) {
            anim->y = anim->targetY << FX32_SHIFT;
            anim->state++;
        }
        break;
    case 2:
        ManagedSprite_GetPositionXYWithSubscreenOffset(anim->sprite, &x, &y, 0x160000);
        ManagedSprite_SetPositionXYWithSubscreenOffset(anim->effect, x, y - 32, 0x160000);
        ManagedSprite_SetAnim(anim->effect, 0x21);
        ManagedSprite_SetDrawFlag(anim->effect, TRUE);
        PlaySE(0x593);
        ManagedSprite_SetAnim(anim->sprite, ov93_02262C7A[anim->kind] + 2);
        anim->timer = 8;
        anim->state++;
        break;
    case 3:
        anim->timer--;
        if (anim->timer == 3) {
            ManagedSprite_TickNFrames(anim->effect, 0x4000);
        }
        if (anim->timer == 0) {
            ManagedSprite_SetDrawFlag(anim->effect, FALSE);
            ManagedSprite_SetAnim(anim->sprite, ov93_02262C7A[anim->kind]);
            anim->state++;
        }
        break;
    case 4:
        anim->y -= 0x8000;
        if (anim->y / FX32_ONE <= anim->targetY - 0x60) {
            ManagedSprite_SetDrawFlag(anim->sprite, FALSE);
            anim->state = 0;
            anim->active = 0;
            return FALSE;
        }
        break;
    }
    ManagedSprite_SetPositionXYWithSubscreenOffset(anim->sprite, anim->x / FX32_ONE, anim->y / FX32_ONE, 0x160000);
    return TRUE;
}
