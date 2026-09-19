#include "private_w20.h"

void ov91_02260D80(u32 unused, u8 *p) {
    int i;
    for (i = 0; i < 8; i++, p += 0x1c) {
        Sprite_Delete(*(void **)(p + 4));
    }
}
