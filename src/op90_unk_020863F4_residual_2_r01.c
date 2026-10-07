#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct { u8 pad[0x23C]; ManagedSprite *sprite; u8 pad2[0x304 - 0x240]; u8 rects[32][4]; } Owner;
void sub_02086B2C(Owner *w, int i);
// @fn sub_02086B2C
void sub_02086B2C(Owner *w, int i) {
    ManagedSprite_SetPositionXY(w->sprite, (w->rects[i + 16][2] + w->rects[i + 16][3]) / 2, (w->rects[i + 16][0] + w->rects[i + 16][1]) / 2);
}
