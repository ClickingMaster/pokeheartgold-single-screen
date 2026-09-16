#include "global.h"

void ov80_0223857C(u16 *buf, u8 toggle);

void ov80_0223857C(u16 *buf, u8 toggle) {
    int base;
    int i;
    int j;
    int rowBase;
    if (toggle == 0) {
        base = 0xC;
    } else {
        base = 0xE;
    }
    rowBase = 0x60;
    for (i = 0; i < 2u; i++) {
        for (j = 0; j < 2u; j++) {
            buf[i * 2 + j] = base + j + rowBase;
        }
        rowBase += 0x10;
    }
}
