#include "private_w09.h"

int ov13_02221BE8(int value, const void *data) {
    int i;
    int present;
    int result;
    result = 0;
    present = result;
    for (i = 0; i < 6; i++) {
        if (ov13_0224DCF8[i] != 0) {
            present = 1;
            break;
        }
    }
    if (present) {
        if (ov13_02222948(ov13_0224DCF8, data, 6) != 0) {
            result = 1;
        }
    } else if (value != 0x1000) {
        result = 2;
    }
    return result;
}
int ov13_02221C2C(const u8 *a, u8 *b) {
    int result = 0;
    ov13_022227A0(b, 8, ov13_02245A20, ov13_02222A9C(ov13_02245A20));
    if (ov13_02222948(a, b, 6) != 0) {
        result = -1;
    } else {
        int av = ov13_02222A84(*(u16 *)(a + 6));
        int bv = ov13_02222A84(*(u16 *)(b + 6));
        if (av + 1 != bv) {
            result = -2;
        }
    }
    return result;
}
