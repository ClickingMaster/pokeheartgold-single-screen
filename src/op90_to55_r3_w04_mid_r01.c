#pragma force_active on
#include "global.h"
void ov112_021E9934(u8 *data, int x, int y, int flags);

// @fn ov112_021E9934
void ov112_021E9934(u8 *data, int x, int y, int flags) {
    int row = y / 8;
    int bit = y % 8;
    int high = flags & 2;
    int mask;
    int offset;
    if (flags & 1) {
        mask = 1 << bit;
        offset = x * 2 + row * 128;
        data[offset] |= mask;
    } else {
        mask = 1 << bit;
        offset = x * 2 + row * 128;
        data[offset] &= 0xFF ^ mask;
    }
    if (high) {
        data[offset + 1] |= mask;
    } else {
        data[offset + 1] &= 0xFF ^ mask;
    }
}
