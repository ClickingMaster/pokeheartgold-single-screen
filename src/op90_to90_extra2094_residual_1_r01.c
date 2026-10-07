#pragma force_active on
#include "global.h"
u32 ov74_0223127C(u32 seed, u32 size, u32 *data);

// @fn ov74_0223127C
u32 ov74_0223127C(u32 seed, u32 size, u32 *data) {
    u32 i;
    u16 hi, lo;
    for (i = 0; i < size / 4; i++) {
        seed = seed * 0x5D588B65 + 0x269EC3;
        hi = seed >> 16;
        seed = seed * 0x5D588B65 + 0x269EC3;
        lo = seed >> 16;
        data[i] ^= (hi << 16) | lo;
    }
    return seed;
}
