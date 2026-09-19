#include "private_w09.h"

int ov13_022216BC(const u8 *data, int length) {
    int i;
    for (i = 0; i < length; i++) {
        u8 value = *data++;
        if (value < 0x20 || value > 0x7F) {
            return -1;
        }
    }
    return 0;
}
