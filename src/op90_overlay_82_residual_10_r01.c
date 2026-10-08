#pragma force_active on
#include "global.h"
#include "bg_window.h"
void ov82_0223F5E0(BgConfig *bg, int idx, int mode);
// @fn ov82_0223F5E0
void ov82_0223F5E0(BgConfig *bg, int idx, int mode) {
    u8 pal;
    int x;
    u8 xb;
    int col;
    u8 h;
    u8 y;
    if (mode == 0) {
        pal = 0;
    } else if (mode == 1) {
        pal = 5;
    } else if (mode == 2) {
        pal = 4;
    } else {
        pal = 3;
    }
    x = (idx % 4) * 8;
    xb = x;
    col = idx % 8;
    if (col < 4) {
        h = 5;
    } else {
        h = 4;
    }
    if ((u32)idx < 4) {
        y = 0;
    } else if ((u32)idx < 8) {
        y = 5;
    } else if ((u32)idx < 12) {
        y = 9;
    } else if ((u32)idx < 16) {
        y = 14;
    } else {
        y = 18;
    }
    BgTilemapRectChangePalette(bg, 3, xb, y, 8, h, pal);
    if (mode == 0) {
        xb = x;
        if (col < 4) {
            h = 2;
        } else {
            h = 3;
        }
        if ((u32)idx < 4) {
            y = 2;
        } else if ((u32)idx < 8) {
            y = 6;
        } else if ((u32)idx < 12) {
            y = 11;
        } else if ((u32)idx < 16) {
            y = 15;
        } else {
            y = 20;
        }
        if ((u32)idx < 9) {
            BgTilemapRectChangePalette(bg, 3, xb, y, 1, h, 1);
        } else {
            BgTilemapRectChangePalette(bg, 3, xb, y, 1, h, 2);
        }
    }
}
