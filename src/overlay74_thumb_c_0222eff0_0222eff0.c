#include "overlay74_thumb_w07_private.h"

void ov74_0222EFF0(void *data, int mask, int target, int *state) {
    BeginNormalPaletteFade(0, mask, mask, 0, 6, 1, 0x55);
    if (state != NULL) {
        *state = 0x1c;
    }
    U32AT(data, 0x2bf8) = target;
}
