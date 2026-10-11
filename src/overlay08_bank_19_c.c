#include "overlay08_exact_bank_private.h"

#pragma force_active on

void BattlePartyMenu_ShowMoveIcons(Ov08WorkMode *work) {
    if (work->mode == 0) {
        BattlePartyMenu_ShowMoveTypeIcons(work);
    } else {
        BattlePartyMenu_ShowMoveContestTypeIcons(work);
    }
}

void BattlePartyMenu_SetSpriteAnim(void *sprite, u32 anim) {
    if (anim != ManagedSprite_GetActiveAnim(sprite)) {
        ManagedSprite_SetAnimationFrame(sprite, 0);
        ManagedSprite_SetAnim(sprite, anim);
    }
}
