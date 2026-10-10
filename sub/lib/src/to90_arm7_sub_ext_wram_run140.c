#pragma force_active on
#pragma dollar_identifiers on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define BASE    (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))
#define HW(a)   (*(volatile u16 *)(a))
int WSetActiveZoneTime(u32, u32);
int WSetPowerMgtMode(u32 mode);

int WSetPowerMgtMode(u32 mode) {
    u8 *w = BASE;
    u8 *s = w + 0x31c;
    H(w, 0x352) = mode;
    if (mode && H(s, 0x12) != 1) {
        HW(0x04808006) |= 0x40;
    } else {
        HW(0x04808006) &= ~0x40;
        WSetActiveZoneTime(H(s, 0x20), 0);
    }
    return 0;
}
