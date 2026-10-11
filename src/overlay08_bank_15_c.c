#include "overlay08_exact_bank_private.h"

#pragma force_active on

void BattlePartyMenu_ShowSpriteAt(void *sprite, int x, int y) {
    ManagedSprite_SetDrawFlag(sprite, TRUE);
    ManagedSprite_SetPositionXY(sprite, (s16)x, (s16)y);
}
