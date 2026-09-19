#include "to50_overlay80_menu_template_eb54_private.h"

void ov80_0222EB54(void *work) {
    PTR(work, 0x194) = (u8 *)work + 0x1BC;
    PTR(work, 0x198) = ov80_0222EC84;
    PTR(work, 0x19C) = ov80_0222EC60;
    PTR(work, 0x1A0) = (u8 *)work + 8;
    U16(work, 0x1A4) = U8(work, 0x9B);
    U16(work, 0x1A6) = 8;
    U8(work, 0x1A8) = 1;
    U8(work, 0x1A9) = 12;
    U8(work, 0x1AA) = 2;
    U8(work, 0x1AB) = (U8(work, 0x1AB) & ~0x0F) | 1;
    U8(work, 0x1AB) = (U8(work, 0x1AB) & ~0xF0) | 0x10;
    U8(work, 0x1AC) = (U8(work, 0x1AC) & ~0x0F) | 0x0F;
    U8(work, 0x1AC) = (U8(work, 0x1AC) & ~0xF0) | 0x20;
    U16(work, 0x1AE) &= ~0x0007;
    U16(work, 0x1AE) &= ~0x0078;
    U16(work, 0x1AE) &= ~0x0180;
    U16(work, 0x1AE) &= ~0x7E00;
    U16(work, 0x1AE) &= ~0x8000;
    PTR(work, 0x1B0) = work;
}
