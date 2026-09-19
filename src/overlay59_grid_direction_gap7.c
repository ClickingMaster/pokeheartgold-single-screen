#include "overlay59_grid_direction_gap7_private.h"
#define U8(w, o) (*(u8 *)((u8 *)(w) + (o)))
void ov59_0223A7FC(Ov59Gap7Work *work, int direction) {
    u8 value;
    if (direction == 0) {
        U8(work, 0x4B) = U8(work, 0x4A);
        U8(work, 0x49) = 1;
        value = U8(work, 0x4A);
        switch (value) {
        case 1:
        case 2:
            if (value != U8(work, 0x4C) && value - 1 != U8(work, 0x4C)) {
                U8(work, 0x4A) = 1;
            } else {
                U8(work, 0x4A) = U8(work, 0x4C);
            }
            break;
        case 3:
            U8(work, 0x4A) = 2;
            break;
        case 0:
        default:
            U8(work, 0x4A) = 0;
            break;
        }
    } else {
        U8(work, 0x4C) = U8(work, 0x4A);
        U8(work, 0x49) = 0;
        switch (U8(work, 0x4A)) {
        case 0:
            value = U8(work, 0x4B);
            if (value > 1) {
                U8(work, 0x4A) = 1;
            } else {
                U8(work, 0x4A) = value;
            }
            break;
        case 1:
            value = U8(work, 0x4B);
            if (value == 0) {
                U8(work, 0x4A) = 1;
            } else if (value == 3) {
                U8(work, 0x4A) = 2;
            } else {
                U8(work, 0x4A) = value;
            }
            break;
        case 2:
            value = U8(work, 0x4B);
            if (value < 2) {
                U8(work, 0x4A) = 2;
            } else {
                U8(work, 0x4A) = value;
            }
            break;
        default:
            U8(work, 0x4A) = 0;
            break;
        }
    }
}
