#include "to52_r6_overlay41_touch_setup_r32_private.h"
void ov41_0224825C(u8 *state, int index, void *last) {
    Config c;
    c.resource = *(void **)(state + 0x40);
    c.f04 = 0x1a;
    c.f08 = index * 4 + 0x86;
    c.f0c = index * 4 + 0x87;
    c.f10 = index * 4 + 0x88;
    c.f14 = 0x88;
    c.f18 = 0x10;
    c.f1c = 2;
    c.f20 = 1;
    c.f24 = 0xd;
    c.f28 = 0;
    c.f2c = (u32)last;
    ov41_02249C7C(state + 0x48, &c);
    *(u32 *)(state + 0x74) = index;
}
