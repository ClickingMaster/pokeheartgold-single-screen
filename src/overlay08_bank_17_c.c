#include "overlay08_exact_bank_private.h"

#pragma force_active on

void ov08_02220C3C(BOOL enabled, void *sprite, int x, int y) {
    if (enabled != FALSE) {
        ManagedSprite_SetAnim(sprite, 2);
        ov08_02220A8C(sprite, x, y);
    }
}
