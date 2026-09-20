#include "overlay08_exact_bank_private.h"

#pragma force_active on

int ov08_02223368(void *unused, const void *hitboxes) {
    return TouchscreenHitbox_FindRectAtTouchNew(hitboxes);
}
