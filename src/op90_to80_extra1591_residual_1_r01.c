#pragma force_active on
#include "global.h"
#include "sprite.h"
typedef struct {
    u32 v[2];
} Pair;
typedef struct {
    u8 y[4];
    u8 x[4];
} Coords;
static const Pair s18 = {{5, 6}};
static const Pair s10 = {{0x66, 0x67}};
static const Pair s08 = {{0x66, 0x67}};
const Coords ov96_0221BFF4 = {{0x88, 0x48, 0x68, 0x88}, {0x20, 0x98, 0x98, 0x98}};
static const Pair s20 = {{0x66, 0x67}};
typedef struct {
    u8 pad[0xf8];
    void *xf8[2];
} Work;
void ov96_021EB408(void *a0, u8 a, u8 b, u8 c, u8 d);
void *ov96_021EB4F4(void *a0, u8 a, u8 b);
Sprite *ov96_021EB5B8(void *x);
void ov96_021EB52C(void *x, int a, int b);
void ov96_021F440C(void *a0, Work *work);
void ov96_021F3EC0(void *a0, Work *work);
// @fn ov96_021F3EC0
void ov96_021F3EC0(void *a0, Work *work) {
    Pair a = s10;
    Pair b = s18;
    int i;
    Sprite *sprite;
    VecFx32 pos;
    for (i = 0; i < 2; i++) {
        ov96_021EB408(a0, 3, 2, a.v[i], b.v[i]);
    }
    for (i = 0; i < 2; i++) {
        work->xf8[i] = ov96_021EB4F4(a0, a.v[i], b.v[i]);
        sprite = ov96_021EB5B8(work->xf8[i]);
        Sprite_SetDrawPriority(sprite, 2);
        pos.z = 0;
        pos.x = (i * 0x80 + 0x40) << FX32_SHIFT;
        pos.y = 0x26 << 16;
        Sprite_SetMatrix(sprite, &pos);
        ov96_021EB52C(work->xf8[i], 1, 0);
    }
    ov96_021F440C(a0, work);
}
