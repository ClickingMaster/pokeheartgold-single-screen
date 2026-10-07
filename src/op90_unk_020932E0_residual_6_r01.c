#pragma force_active on
#include "global.h"
#include "sprite.h"
typedef struct Elem { Sprite *sprite; u8 data[0x30]; } Elem;
typedef struct W {
    u8 pad0[0x198];
    Elem a[31];
    u8 *rects;
} W;
extern void sub_02094150(SpriteTemplate *tmpl, W *w, int a2);
extern void sub_0209428C(u8 *rect, int x, int y, int w, int h);
void sub_020942B0(W *w);
// @fn sub_020942B0
void sub_020942B0(W *w){SpriteTemplate tmpl; int col, row, n; sub_02094150(&tmpl, w, 1); n = 0; for (row = 0; row < 5; row++) { for (col = 0; col < 6; col++) { sub_0209428C(w->rects + n * 4, 20 + col * 24, 40 + row * 24, 24, 24); tmpl.position.x = (20 + col * 24) * FX32_ONE; tmpl.position.y = (40 + row * 24) * FX32_ONE; if (w->a[n].sprite == NULL) { w->a[n].sprite = Sprite_CreateAffine(&tmpl); } Sprite_SetAnimActiveFlag(w->a[n].sprite, TRUE); Sprite_SetAnimCtrlSeq(w->a[n].sprite, n + 7); Sprite_SetPriority(w->a[n].sprite, 1); Sprite_SetDrawFlag(w->a[n].sprite, FALSE); Sprite_SetDrawPriority(w->a[n].sprite, 6); n++; } } tmpl.position.x = 20 * FX32_ONE; tmpl.position.y = 40 * FX32_ONE; if (w->a[30].sprite == NULL) { w->a[30].sprite = Sprite_CreateAffine(&tmpl); } Sprite_SetAnimActiveFlag(w->a[30].sprite, TRUE); Sprite_SetAnimCtrlSeq(w->a[30].sprite, 48); Sprite_SetPriority(w->a[30].sprite, 1); Sprite_SetDrawFlag(w->a[30].sprite, FALSE); Sprite_SetDrawPriority(w->a[30].sprite, 8); Sprite_SetOamMode(w->a[30].sprite, GX_OAM_MODE_XLU);}
