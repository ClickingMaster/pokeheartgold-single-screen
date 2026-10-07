#pragma force_active on
#include "global.h"

int ov80_02237F9C(int arg);
void ov80_02237F3C(u16 *buf, int arg);

// @fn ov80_02237F3C
void ov80_02237F3C(u16 *buf, int arg) {
    u8 tiles[10];
    u32 i, x, y;
    int pal = ov80_02237F9C(arg);

    for (i = 0; i < 5; i++) {
        tiles[i] = i;
        tiles[5 + i] = 4 - i;
    }
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 10; x++) {
            buf[y * 10 + x] = tiles[x] + (pal * 0x60 + 0x10 + y * 0x20);
            if (x >= 5) {
                buf[y * 10 + x] |= 0x400;
            }
        }
    }
}
