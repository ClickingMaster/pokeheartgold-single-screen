#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u8 __VENEER_MATH_CountPopulation(u32);
#define REG_VCOUNT (*(volatile u16 *)0x04000006)
u16 WmspGetTmptt(u32 hdr, u32 size, u32 mask, u32 line);

u16 WmspGetTmptt(u32 hdr, u32 size, u32 mask, u32 line) {
    s32 diff;
    u32 total;
    u16 vc;
    if (size & 0x8000) {
        size &= 0x7fff;
    } else {
        size = (size + 0x1c) * 4 + 0x66;
    }
    total = (hdr + 0x22) * 4 + 0x60 + (size * __VENEER_MATH_CountPopulation(mask) + 0x388);
    vc = REG_VCOUNT;
    diff = (s32)(line - 2) - vc;
    while (diff < 0) {
        diff += 263;
    }
    diff *= 127;
    diff /= 20;
    if (diff * 10 < total) {
        diff = 0;
    }
    return diff;
}
