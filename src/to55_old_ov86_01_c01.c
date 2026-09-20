#include "to55_old_ov86_private.h"

void ov86_021E78B8(void *work, int index) {
    unsigned int value;
    U8_AT(work, 0x24C) = (U8_AT(work, 0x24C) & ~1) | 1;
    U8_AT(work, 0x24C) = (U8_AT(work, 0x24C) & ~0xFE) | 4;
    U8_AT(work, 0x24D) = (U8_AT(work, 0x24D) & ~0xF) | 2;
    U8_AT(work, 0x24D) = U8_AT(work, 0x24D) & ~0xF0;
    value = ov86_021E81DC[index];
    U8_AT(work, 0x250) = (value & 1) * 13 + 0x26;
    U8_AT(work, 0x251) = (value / 2) * 4 + 0x25;
    U8_AT(work, 0x252) = 9;
    U8_AT(work, 0x253) = 2;
    U8_AT(work, 0x24E) = 0;
    U8_AT(work, 0x24F) = 0;
}
void ov86_021E792C(void *work, u8 x, u8 y) {
    U8_AT(work, 0x24C) = (U8_AT(work, 0x24C) & ~1) | 1;
    U8_AT(work, 0x24C) = (U8_AT(work, 0x24C) & ~0xFE) | 4;
    U8_AT(work, 0x24D) = (U8_AT(work, 0x24D) & ~0xF) | 2;
    U8_AT(work, 0x24D) = U8_AT(work, 0x24D) & ~0xF0;
    U8_AT(work, 0x250) = x;
    U8_AT(work, 0x251) = y;
    U8_AT(work, 0x252) = 4;
    U8_AT(work, 0x253) = 4;
    U8_AT(work, 0x24E) = 0;
    U8_AT(work, 0x24F) = 0;
}
