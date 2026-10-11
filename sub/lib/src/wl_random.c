#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

void RND_Init(u32 mul, u32 add) {
    WlRandom *rnd = &wlWorkPlain->rnd;
    rnd->mul = (mul & 0xFFF8) + 5;
    rnd->add = add | 1;
}

void RND_seed(u32 seed) {
    wlWorkPlain->rnd.seed = seed;
}

u16 RND_rand(void) {
    WlWork *w = wlWorkPlain;
    WlRandom *rnd = &w->rnd;
    rnd->seed = (u32)rnd->seed * w->rnd.mul + rnd->add;
    return rnd->seed;
}

// CRC-16, one byte at a time (4 bits per table lookup).
u16 calc_NextCRC(int data, int crc) {
    u16 next = crc16_table[crc & 15] ^ (u16)((crc >> 4) & 0xFFF) ^ crc16_table[data & 15];
    next = crc16_table[next & 15] ^ (u16)((next >> 4) & 0xFFF) ^ crc16_table[(data >> 4) & 15];
    return next;
}
