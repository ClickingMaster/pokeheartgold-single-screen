#pragma force_active on
#include "global.h"

typedef struct Pair {
    u32 v[2];
} Pair;

typedef struct Work {
    u8 unk0[0x150];
    void *handles[2];
    void *extra[2];
} Work;

static const Pair sIds3 = {{0x66, 0x67}};
static const Pair sIds2 = {{4, 5}};
const Pair ov96_0221C728 = {{0x66, 0x67}};
static const Pair sIds4 = {{0x66, 0x67}};
void PokeathlonCourseSpritePool_CreateSprite(void *ctx, int a, int b, u8 c, u8 d);
void *PokeathlonCourseSpritePool_Take(void *ctx, u8 a, u8 b);
void *PokeathlonCourseSpritePoolSprite_GetSprite(void *handle);
void PokeathlonCourseSpritePoolSprite_Show(void *handle, int a, int b);
void PokeathlonCourseSpritePoolSprite_SetAnim(void *handle, int a);
void PokeathlonCourseSpritePoolSprite_SetPosition(void *handle, VecFx32 *pos);
void Sprite_SetMatrix(void *sprite, VecFx32 *pos);
void ov96_02200D7C(void *ctx, Work *w);
void ov96_02200C8C(void *ctx, Work *w);

// @fn ov96_02200C8C
void ov96_02200C8C(void *ctx, Work *w) {
    Pair a = ov96_0221C728;
    Pair b = sIds2;
    VecFx32 pos;
    int i;
    void *sprite;

    for (i = 0; i < 2; i++) {
        PokeathlonCourseSpritePool_CreateSprite(ctx, 3, 2, a.v[i], b.v[i]);
    }
    for (i = 0; i < 2; i++) {
        w->handles[i] = PokeathlonCourseSpritePool_Take(ctx, a.v[i], b.v[i]);
        sprite = PokeathlonCourseSpritePoolSprite_GetSprite(w->handles[i]);
        pos.z = 0;
        pos.x = 0x28000;
        pos.y = ((88 * (1 - i) + 48) << 12) + 0x200000;
        Sprite_SetMatrix(sprite, &pos);
        PokeathlonCourseSpritePoolSprite_Show(w->handles[i], 1, 1);
        w->extra[i] = PokeathlonCourseSpritePool_Take(ctx, 0x65, 0x10);
        PokeathlonCourseSpritePoolSprite_SetAnim(w->extra[i], 8);
        PokeathlonCourseSpritePoolSprite_SetPosition(w->extra[i], &pos);
        PokeathlonCourseSpritePoolSprite_Show(w->extra[i], 1, 0);
    }
    ov96_02200D7C(ctx, w);
}
