#include "overlay08_exact_bank_private.h"

#pragma force_active on

void ov08_022213C8(Ov08WorkMode *work) {
    if (work->mode == 0) {
        ov08_02221294(work);
    } else {
        ov08_02221328(work);
    }
}

void ov08_022213E4(void *sprite, u32 anim) {
    if (anim != ManagedSprite_GetActiveAnim(sprite)) {
        ManagedSprite_SetAnimationFrame(sprite, 0);
        ManagedSprite_SetAnim(sprite, anim);
    }
}
