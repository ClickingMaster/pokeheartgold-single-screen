#pragma force_active on
#include "global.h"
#include <string.h>
typedef struct { u32 v[2]; } Pair;
typedef struct Work Work;
static const Pair sIds10 = {{4, 5}};
static const Pair sIds08 = {{0x66, 0x67}};
const Pair ov96_0221CBCC = {{0x66, 0x67}};
static const Pair sIds18 = {{0x66, 0x67}};
extern void ov96_021EB408(void *, int, int, u8, u8);
extern void ov96_021EB52C(void *, int, int);
extern void ov96_022089D0(void *, Work *);
extern void Sprite_SetMatrix(void *, VecFx32 *);
extern void *ov96_021EB4F4(void *, u8, u8);
extern void *ov96_021EB5B8(void *);
struct Work { u8 prefix[0x130]; void *handles[2]; };
void ov96_02208914(void *ctx, Work *w);
// @fn ov96_02208914
void ov96_02208914(void *ctx, Work *w) {
    Pair a = ov96_0221CBCC;
    Pair b = sIds10;
    VecFx32 pos;
    void *sprite;
    int i;
    for (i = 0; i < 2; i++) {
        ov96_021EB408(ctx, 3, 2, a.v[i], b.v[i]);
    }
    for (i = 0; i < 2; i++) {
        w->handles[i] = ov96_021EB4F4(ctx, a.v[i], b.v[i]);
        sprite = ov96_021EB5B8(w->handles[i]);
        pos.z = 0;
        pos.x = 0x28000;
        pos.y = ((88 * (1 - i) + 48) << 12) + 0x2BC000;
        Sprite_SetMatrix(sprite, &pos);
        ov96_021EB52C(w->handles[i], 1, 1);
    }
    ov96_022089D0(ctx, w);
}
