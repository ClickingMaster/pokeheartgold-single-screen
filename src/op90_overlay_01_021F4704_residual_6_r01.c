#pragma force_active on
#include "global.h"
int ov01_021F5A80(int dir, int width, int div, int val);
// @fn ov01_021F5A80
int ov01_021F5A80(int dir, int width, int div, int val) {
    int r;
    switch (dir) {
    case 1:
        r = val % div + 1;
        if (r >= div) {
            r = val;
        } else {
            r = val + 1;
        }
        break;
    case 2:
        r = val + div;
        if (r >= width * 32 * div) {
            r = val;
        }
        break;
    case 3:
        r = val % div - 1;
        if (r < 0) {
            r = val;
        } else {
            r = val - 1;
        }
        break;
    case 4:
        r = val - div;
        if (r < 0) {
            r = val;
        }
        break;
    default:
        GF_AssertFail();
        r = 0;
        break;
    }
    return r;
}
