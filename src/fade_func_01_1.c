#include "fade_func_01_internal.h"

BOOL FadeFunc_01(FadeWork *work) {
    if (work->state == 0) {
        work->topScreen = 0;
        work->bottomScreen = 1;
        BrightnessTransition_Start(work, 0, 1);
        return FALSE;
    }
    return BrightnessTransition_Run(work);
}
