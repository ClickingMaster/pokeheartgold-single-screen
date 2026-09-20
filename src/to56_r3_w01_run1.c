#include "to56_r3_w01_private.h"
void ov08_022216CC(u8 *raw) {
    Ov08Bank20Work *work = (Ov08Bank20Work *)raw;
    if (work->upper == 0) {
        ov08_02224BF8(work->cursor, 0x5F);
    } else {
        ov08_02224BF8(work->cursor, 0x7F);
    }
}
void ov08_022216F4(u8 *raw) {
    Ov08Bank20Work *work = (Ov08Bank20Work *)raw;
    if (work->upper == 0) {
        ov08_02224BF8(work->cursor, 5);
    } else {
        ov08_02224BF8(work->cursor, 7);
    }
}
