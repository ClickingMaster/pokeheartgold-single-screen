#pragma force_active on
#include "unk020124b0_sol_partial_internal.h"
// @fn sub_020124B0
void sub_020124B0(FadePalette *work) {
    u32 phase = (u16)(work->amount % 0x3FFF);
    s16 *a = sub_02010EE0(work, 0);
    s16 *b = sub_02010EE0(work, 1);
    s16 *pb;
    int i, x, m;
    u32 inv;


    for (i = 0, pb = b; i < 96; i++) {
        if (work->amount < 0x3FFF) {
            x = sub_020109D8(phase, 96 - i);
            if (x > 127) {
                x = 127;
            }
            m = 191 - i;
            a[m + 0x180] = 128 - x;
            a[m + 0x240] = 128;
            pb[0x180] = 128;
            pb[0x240] = x + 128;
        } else {
            m = 191 - i;
            a[m + 0x180] = 0;
            a[m + 0x240] = 128;
            pb[0x180] = 128;
            pb[0x240] = 255;
        }
        pb++;
    }
    inv = 0x3FFF - phase;

    for (i = 96, b += 96; i < 192; i++) {
        if (work->amount < 0x3FFF) {
            m = 191 - i;
            a[m + 0x180] = 128;
            a[m + 0x240] = 128;
            b[0x180] = 128;
            b[0x240] = 128;
        } else {
            x = sub_020109D8(inv, i - 96);
            if (x > 127) {
                x = 127;
            }
            m = 191 - i;
            a[m + 0x180] = 0;
            a[m + 0x240] = 128 - x;
            b[0x180] = x + 128;
            b[0x240] = 255;
        }
        b++;
    }
}
