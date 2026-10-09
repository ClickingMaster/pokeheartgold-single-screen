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
void ov96_021EB408(void *ctx, int a, int b, u8 c, u8 d);
void *ov96_021EB4F4(void *ctx, u8 a, u8 b);
void *ov96_021EB5B8(void *handle);
void ov96_021EB52C(void *handle, int a, int b);
void ov96_021EB564(void *handle, int a);
void ov96_021EB588(void *handle, VecFx32 *pos);
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
        ov96_021EB408(ctx, 3, 2, a.v[i], b.v[i]);
    }
    for (i = 0; i < 2; i++) {
        w->handles[i] = ov96_021EB4F4(ctx, a.v[i], b.v[i]);
        sprite = ov96_021EB5B8(w->handles[i]);
        pos.z = 0;
        pos.x = 0x28000;
        pos.y = ((88 * (1 - i) + 48) << 12) + 0x200000;
        Sprite_SetMatrix(sprite, &pos);
        ov96_021EB52C(w->handles[i], 1, 1);
        w->extra[i] = ov96_021EB4F4(ctx, 0x65, 0x10);
        ov96_021EB564(w->extra[i], 8);
        ov96_021EB588(w->extra[i], &pos);
        ov96_021EB52C(w->extra[i], 1, 0);
    }
    ov96_02200D7C(ctx, w);
}
