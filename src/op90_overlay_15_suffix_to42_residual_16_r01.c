#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct { u8 pad[0x29C]; ManagedSprite *unk29C; u8 pad2[0x2D0 - 0x2A0]; ManagedSprite *sprites[16]; } Work;
extern int sQuantityButtonCounts[];
extern int sHiddenQuantityButtonCounts[];
extern int sHiddenQuantityButtons[][4];
extern int sQuantityButtonSprites[][6];
extern int sQuantityButtonAnims[][6];
int BagApp_GetUnusedDigitCount(int);
void BagApp_ShowQuantityButtons(Work *w, int type, int value);
// @fn BagApp_ShowQuantityButtons
void BagApp_ShowQuantityButtons(Work *w, int type, int value) {
int i; int r; if (type == 2 && value > 99) value = 99; for (i = 0; i < sQuantityButtonCounts[type - 2]; i++) { ManagedSprite_SetDrawFlag(w->sprites[sQuantityButtonSprites[type - 2][i]], 1); ManagedSprite_SetAnim(w->sprites[sQuantityButtonSprites[type - 2][i]], sQuantityButtonAnims[type - 2][i]); } r = BagApp_GetUnusedDigitCount(value); if (r != 0) { if (type - 2 == 0 && r == 2) { ManagedSprite_SetDrawFlag(w->sprites[0], 0); ManagedSprite_SetDrawFlag(w->sprites[3], 0); } else if (type - 2 == 1) { for (i = 0; i < sHiddenQuantityButtonCounts[r - 1]; i++) ManagedSprite_SetDrawFlag(w->sprites[sHiddenQuantityButtons[r - 1][i]], 0); } } ManagedSprite_SetDrawFlag(w->sprites[6], 1); ManagedSprite_SetAnimationFrame(w->sprites[6], 0); ManagedSprite_SetAnim(w->sprites[6], 0x25); ManagedSprite_SetAnimationFrame(w->unk29C, 0); ManagedSprite_SetAnim(w->unk29C, 0x27);
}
