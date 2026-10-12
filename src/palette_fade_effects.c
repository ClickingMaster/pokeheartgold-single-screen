#include "palette_fade_effects_internal.h"

BOOL FadeFunc_00(FadeWork *work) {
    if (work->state == 0) {
        work->topScreen = 1;
        work->bottomScreen = 1;
        BrightnessTransition_Start(work);
        return 0;
    }
    return BrightnessTransition_Run(work);
}
