#pragma force_active on
#include "global.h"
#include "sprite.h"
typedef struct {
    u16 x;
    u16 y;
} Pos16;
typedef struct {
    u8 pad[0x284];
    void *x284;
    u8 pad288[0x71c - 0x288];
    Sprite *sprites[3];
    u8 count;
    u8 x729;
} Work;
Sprite *ov96_021EA6E4(int a, int b, int c, int d, void *e);
void ov96_021E62AC(Work *work, int yOffset, int a2, int a3, int a4, u8 count, Pos16 *pos);
// @fn ov96_021E62AC
void ov96_021E62AC(Work *work, int yOffset, int a2, int a3, int a4, u8 count, Pos16 *pos) {
    u8 i;
    VecFx32 v;
    work->count = count;
    for (i = 0; i < count; i++) {
        work->sprites[i] = ov96_021EA6E4(a2, a3, a4, 0, work->x284);
        v.z = 0;
        v.x = pos[i].x << FX32_SHIFT;
        v.y = (yOffset + pos[i].y) << FX32_SHIFT;
        Sprite_SetMatrix(work->sprites[i], &v);
    }
    for (; i < 3; i++) {
        work->sprites[i] = NULL;
    }
    work->x729 = 0;
}
