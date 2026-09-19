#include "private_w20.h"

u32 ov91_02260B48(u8 *p, u32 base) {
    u32 n;
    if (*(u32 *)(p + 0x28) == 1) {
        n = 4;
    } else {
        n = *(u16 *)(p + 4);
    }
    return base + n * 16;
}
