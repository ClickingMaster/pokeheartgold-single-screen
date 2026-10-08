#pragma force_active on
#include "global.h"

typedef struct Ov87Work {
    u8 pad0[0x38C];
    u8 *pixels;
} Ov87Work;

void ov87_021E7AB0(Ov87Work *work, u32 x, u32 y);

// @fn ov87_021E7AB0
void ov87_021E7AB0(Ov87Work *work, u32 x, u32 y) {
    u8 mask;
    u32 xTile, yTile, xPix;
    mask = 0xFF;
    if ((x & 1) == 0) {
        mask ^= 0x0F;
    } else {
        mask ^= 0xF0;
    }
    xTile = x / 8 * 0x20;
    yTile = y / 8 * 0x400;
    xPix = (x % 8) / 2;
    if (y < 8) {
        y = y * 4;
    } else {
        y = (y % 8) * 4;
    }
    work->pixels[(u32)(y + (xPix + (yTile + xTile)))] &= mask;
}
