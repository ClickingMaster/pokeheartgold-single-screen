#include "private_w20.h"

void ov91_02260400(int x, int y, s32 *out) {
    out[0] = (255 - x) * 0x960 - (0x960 << 7);
    out[2] = -0x12c000 - (y * 0x960 - (0x960 << 7));
}
