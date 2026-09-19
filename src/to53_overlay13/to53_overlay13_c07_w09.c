#include "private_w09.h"

int ov13_022220A4(int value) {
    int result = 0;
    if (value & 0x10) {
        result = 1;
    }
    return result;
}
int ov13_022220B4(int which, void *a, void *b, void *c) {
    switch (which) {
    case 0:
        ov13_022217D0(2);
        return ov13_02222108(a, b, c);
    case 1:
        ov13_022217D0(3);
        return ov13_0222224C(a, b, c);
    case 2:
        ov13_022217D0(5);
        return ov13_02222328(a, b, c);
    default:
        return -1;
    }
}
