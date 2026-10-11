#include "overlay08_exact_bank_private.h"

#pragma force_active on

int BattlePartyMenu_CheckTouchHitbox(void *unused, const void *hitboxes) {
    return TouchscreenHitbox_FindRectAtTouchNew(hitboxes);
}
