#include "to55_r3_w04_private.h"

u8 ov112_021E98F8(const u8 *data, int x, int y) {
    int bit = y % 8;
    int offset = x * 2 + (y / 8) * 128;
    return (((data[offset + 1] >> bit) & 1) << 1) | ((data[offset] >> bit) & 1);
}
