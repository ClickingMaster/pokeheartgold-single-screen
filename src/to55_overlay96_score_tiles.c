#include "to55_r3_w17_private.h"

void ov96_022005B4(void *w, int value, u32 score) {
    u8 x[2];
    u8 y[2];
    u8 digit;
    u8 tens;
    u8 ones;
    u8 i;

    digit = (u8)((float)score / 102.4);
    tens = (u8)(value / 10);
    x[0] = (u8)((tens % 5) * 4);
    y[0] = (u8)((tens / 5) * 8);
    ones = (u8)(value % 10);
    x[1] = (u8)((ones % 5) * 4);
    y[1] = (u8)((ones / 5) * 8);

    for (i = 0; i < 2; i++) {
        CopyRectToBgTilemapRect((void *)WORD(w, 8), 5, (u8)(i * 4 + 14), 8, 4, 8, U8P((void *)WORD(w, 0x38), 0xc), x[i], y[i], 20, 16);
    }
    CopyRectToBgTilemapRect((void *)WORD(w, 8), 5, 24, 8, 4, 8, U8P((void *)WORD(w, 0x38), 0xc), (u8)((digit % 5) * 4), (u8)((digit / 5) * 8), 20, 16);
    ScheduleBgTilemapBufferTransfer((void *)WORD(w, 8), 5);
}
