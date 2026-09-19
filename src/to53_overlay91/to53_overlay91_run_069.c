#include "private_w20.h"

u32 ov91_02260CA8(u8 *p) {
    return Sprite_Delete(*(void **)(p + 4));
}

void ov91_02260CB4(u8 *p) {
    Sprite_SetDrawFlag(*(void **)(p + 4), TRUE);
    *(u32 *)p = 1;
}
