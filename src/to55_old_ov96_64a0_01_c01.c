#include "to55_old_ov96_64a0_private.h"

void ov96_021F64A0(s32 *src, int flags, s32 *dst) {
    int i = 0;
    int f1 = 1, f2 = 2, f4 = 4;
    f1 &= flags;
    f2 &= flags;
    f4 &= flags;
    for (; i < 128; i++, dst++, src++) {
        *dst = 0;
        if (f1) {
            if (*dst > src[0x80]) {
                *dst = src[0x80];
            }
            if (*dst > src[0x200]) {
                *dst = src[0x200];
            }
        }
        if (f2) {
            if (*dst > src[0x100]) {
                *dst = src[0x100];
            }
            if (*dst > src[0x280]) {
                *dst = src[0x280];
            }
        }
        if (f4) {
            if (*dst > src[0x180]) {
                *dst = src[0x180];
            }
            if (*dst > src[0x300]) {
                *dst = src[0x300];
            }
        }
    }
}
