#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct { u8 pad[0x29C]; ManagedSprite *unk29C; u8 pad2[0x2D0 - 0x2A0]; ManagedSprite *sprites[16]; } Work;
extern int ov15_02200998[];
extern int ov15_022009A0[];
extern int ov15_02200A14[][4];
extern int ov15_02200A58[][6];
extern int ov15_02200A88[][6];
int ov15_022002EC(int);
void ov15_02200300(Work *w, int type, int value);
// @fn ov15_02200300
void ov15_02200300(Work *w, int type, int value) {
int i; int r; if (type == 2 && value > 99) value = 99; for (i = 0; i < ov15_02200998[type - 2]; i++) { ManagedSprite_SetDrawFlag(w->sprites[ov15_02200A58[type - 2][i]], 1); ManagedSprite_SetAnim(w->sprites[ov15_02200A58[type - 2][i]], ov15_02200A88[type - 2][i]); } r = ov15_022002EC(value); if (r != 0) { if (type - 2 == 0 && r == 2) { ManagedSprite_SetDrawFlag(w->sprites[0], 0); ManagedSprite_SetDrawFlag(w->sprites[3], 0); } else if (type - 2 == 1) { for (i = 0; i < ov15_022009A0[r - 1]; i++) ManagedSprite_SetDrawFlag(w->sprites[ov15_02200A14[r - 1][i]], 0); } } ManagedSprite_SetDrawFlag(w->sprites[6], 1); ManagedSprite_SetAnimationFrame(w->sprites[6], 0); ManagedSprite_SetAnim(w->sprites[6], 0x25); ManagedSprite_SetAnimationFrame(w->unk29C, 0); ManagedSprite_SetAnim(w->unk29C, 0x27);
}
