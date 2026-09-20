#include "to55_r3_w04_private.h"

void ov112_021E9A30(void *dst, const void *src, int value) {
    int y;
    int x;
    ov112_021E9998(dst, src, value, -8, -14);
    ov112_021E9998((u8 *)dst + 0x300, (const u8 *)src + 0x300, value, -22, -14);
    for (y = 0; y < 48; y++) {
        for (x = 0; x < 64; x++) {
            ov112_021E98F8(dst, x, y);
        }
    }
}
