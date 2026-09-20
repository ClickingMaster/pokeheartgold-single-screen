#include "to55_overlay91_run_040_private.h"

BOOL ov91_0225ED6C(Ov91R40Work *work) {
    s32 x;
    s32 y;

    if (work->frames < 2) {
        return FALSE;
    }
    x = 128 - gSystem.touchX;
    y = -96 - gSystem.touchY;
    if ((FX_Sqrt((x * x + y * y) << FX32_SHIFT) >> FX32_SHIFT) <= 286) {
        return TRUE;
    }
    return FALSE;
}
