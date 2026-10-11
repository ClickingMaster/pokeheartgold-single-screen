#include "overlay08_exact_bank_private.h"

#pragma force_active on

void BattlePartyMenu_ShowCapsuleIcon(BOOL enabled, void *sprite, int x, int y) {
    if (enabled != FALSE) {
        ManagedSprite_SetAnim(sprite, 2);
        BattlePartyMenu_ShowSpriteAt(sprite, x, y);
    }
}
