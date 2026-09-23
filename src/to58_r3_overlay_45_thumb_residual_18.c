#include "global.h"

#pragma require_prototypes off

void ov45_0222BD30(volatile u8 *p) {
    u32 count = 0x14;
    do {
        *p++ = 0;
        count--;
    } while (count != 0);
}
