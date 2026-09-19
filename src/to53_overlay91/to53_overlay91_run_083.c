#include "private_w20.h"

u32 ov91_022619E8(u8 *p) {
    *(u16 *)p = 0;
    p[2] = 1;
    p[3] = 0;
    return PlaySE(0x586);
}
