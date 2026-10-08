#pragma force_active on
#include "global.h"

void ov41_02249978(u8 *rect, int x, int y, int rx, int ry);

// @fn ov41_02249978
void ov41_02249978(u8 *rect, int x, int y, int rx, int ry) {
    if (y - ry >= 0) {
        rect[0] = y - ry;
    } else {
        rect[0] = 0;
    }
    if (y + ry <= 191) {
        rect[1] = y + ry;
    } else {
        rect[1] = 191;
    }
    if (x - rx >= 0) {
        rect[2] = x - rx;
    } else {
        rect[2] = 0;
    }
    if (x + rx <= 255) {
        rect[3] = x + rx;
    } else {
        rect[3] = 255;
    }
}
