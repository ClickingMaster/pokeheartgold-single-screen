#include "overlay74_thumb_w06_private.h"

u16 ov74_02228C84(u16 value) {
    u32 i;
    for (i = 0; i < 0x123; i++) {
        if (ov74_0223BD68[i][1] == value) {
            return ov74_0223BD68[i][0];
        }
    }
    return 0x3F;
}
