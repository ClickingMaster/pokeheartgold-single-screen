#include "private_w20.h"

void ov91_02260830(u32 unused, u8 *p) {
    Sprite_Delete(*(void **)p);
    memset(p, 0, 0x108);
}
