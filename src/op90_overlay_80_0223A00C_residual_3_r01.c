#pragma force_active on
#include "global.h"
#include "sprite.h"

typedef struct Ov80Motion {
    fx32 scale;
    u8 data[0x10];
} Ov80Motion;

typedef struct Ov80ScaleSprite {
    Sprite *sprite;
} Ov80ScaleSprite;

typedef struct Ov80ScaleAnim {
    s16 timer;
    s16 count;
    Ov80ScaleSprite *sprites[4];
    Ov80Motion motions[4];
} Ov80ScaleAnim;

int ov80_0223A8D4(Ov80Motion *motion);
void ov80_0223A91C(VecFx32 *vec, fx32 x, fx32 y, fx32 z);
BOOL ov80_0223A834(Ov80ScaleAnim *anim);

// @fn ov80_0223A834
BOOL ov80_0223A834(Ov80ScaleAnim *anim) {
    int i;
    BOOL done = TRUE;
    int result;
    VecFx32 scale;

    if (anim->count < 4) {
        done = FALSE;
        anim->timer--;
        if (anim->timer <= 0) {
            anim->timer = 3;
            anim->count++;
        }
    }
    for (i = 0; i < anim->count; i++) {
        result = ov80_0223A8D4(&anim->motions[i]);
        ov80_0223A91C(&scale, anim->motions[i].scale, anim->motions[i].scale, anim->motions[i].scale);
        Sprite_SetAffineScale(anim->sprites[i]->sprite, &scale);
        Sprite_SetDrawFlag(anim->sprites[i]->sprite, TRUE);
        if (result == 0) {
            done = FALSE;
        }
    }
    return done;
}
