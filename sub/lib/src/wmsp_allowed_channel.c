#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <nitro/wram_begin.h>

u16 WMSP_GetAllowedChannel(u16 bitField);

#pragma force_active on

// Picks channels spread over the allowed ones (bits 0-12 of bitField): only the lowest when they span fewer
// than 5 channels, otherwise the lowest, the highest and one near the middle (dropped when it is closer than 5
// to either end).
u16 WMSP_GetAllowedChannel(u16 bitField) {
    u16 ch = (u16)(bitField & 0x1fff);
    int hi, lo, mid, i, range;
    if (ch == 0) {
        return 0;
    }
    for (lo = 0; lo < 16; lo++) {
        if (ch & (1 << lo)) {
            break;
        }
    }
    for (hi = 15; hi != 0; hi--) {
        if (ch & (1 << hi)) {
            break;
        }
    }
    range = hi - lo;
    if (range < 5) {
        return (u16)(1 << lo);
    }
    mid = (hi + lo) / 2;
    for (i = 0; i < range; i++) {
        mid += i * ((i % 2) * 2 - 1);
        if (ch & (1 << mid)) {
            break;
        }
    }
    if (hi - mid < 5 || mid - lo < 5) {
        return (u16)((1 << lo) | (1 << hi));
    }
    return (u16)((1 << hi) | (1 << mid) | (1 << lo));
}

#include <nitro/wram_end.h>
