#include "palette_fade_effects_internal.h"

void sub_02011068(void *work, int a, int b, int useHardware) {
    if (useHardware == 0) {
        ScreenWindow_SetVisible(a, b);
    } else {
        ScreenWindow_QueueSetVisible(work, a);
    }
}
