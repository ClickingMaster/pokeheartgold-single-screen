#include "overlay08_exact_bank_private.h"

#pragma force_active on

int ov08_0221D5D0(void *unused, const void *hitboxes) {
    return TouchscreenHitbox_FindRectAtTouchNew(hitboxes);
}
