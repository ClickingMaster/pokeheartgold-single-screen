#include "overlay74_thumb_w07_private.h"

int ov74_0222E85C(s32 *values) {
    s32 *minimum = &values[0];
    s32 minimumValue = values[0];
    int result = 0;
    if (values[1] < minimumValue) {
        minimumValue = values[1];
        result = 1;
        minimum = &values[1];
    }
    if (values[2] < minimumValue) {
        minimum = values;
        minimumValue = values[2];
        result = 2;
        minimum += 2;
    }
    if (values[3] < minimumValue) {
        minimum = values;
        result = 3;
        minimum += 3;
    }
    *minimum = 0x3fff0001;
    return result + 1;
}
int ov74_0222E898(void) {
    int i;
    int count = 0;
    for (i = 1; i < 5; i++) {
        if (sub_02034818(i) != NULL) {
            count++;
        }
    }
    return count;
}
