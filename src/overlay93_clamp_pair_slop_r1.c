#include "overlay93_arm_slop_r1_internal.h"

// global.h selects Thumb by default. This include must remain after the project
// header so this translation unit is compiled as the original ARM code.
#include <nitro/code32.h>

s32 ov93_0225F8E4(Ov93SlopR1Work *work, s32 a1, s32 a2, s32 a3, s32 *p4, s32 *p5) {
    BOOL ret = FALSE;
    s32 sum;

    *p4 = a2;
    sum = a2 + a1;
    if (sum > 0x64000) {
        a1 -= sum - 0x64000;
        ret = TRUE;
    }

    *p5 = a3;
    *p4 = a2 + a1;

    if (a3 > a2) {
        *p5 = a3;
    } else {
        *p5 = a3 - (a1 * 25) / 100;
    }

    return ret;
}
