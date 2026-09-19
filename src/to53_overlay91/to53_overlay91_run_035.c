#include "private_w19.h"

void ov91_0225E3F4(void *unused, u8 *p, u16 value, u32 word) {
    p[1] = 1;
    *(u16 *)(p + 6) = value;
    *(u32 *)(p + 0x24) = word;
}


void ov91_0225E400(void *p) {
    memset(p, 0, 0x44);
}
