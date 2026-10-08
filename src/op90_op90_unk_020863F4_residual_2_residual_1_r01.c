#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct { ManagedSprite *sprite; u8 pad[0x18]; } Obj;
typedef struct { u8 pad[0x220]; Obj objs[7]; u8 pad2[0x304 - 0x220 - 7 * 0x1C]; u8 rects[32][4]; } Owner;
void sub_02086B6C(Owner *w, int i, int j);
// @fn sub_02086B6C
void sub_02086B6C(Owner *w, int i, int j) {
    ManagedSprite_SetPositionXY(w->objs[j].sprite, (w->rects[i + 16][2] + w->rects[i + 16][3]) / 2, (w->rects[i + 16][0] + w->rects[i + 16][1]) / 2);
}
