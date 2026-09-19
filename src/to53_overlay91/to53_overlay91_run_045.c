#include "private_w19.h"

void ov91_0225F23C(u8 *p) {
    p[0x38] = 1;
    p[0x3a] = 0;
    p[0x39] = 0;
    Sprite_SetDrawFlag(*(void **)(p + 0x34), TRUE);
}
