#include "private_w20.h"

void ov91_02260D00(u8 *p) {
    Sprite_SetDrawFlag(*(void **)(p + 4), FALSE);
    *(u32 *)p = 0;
}
