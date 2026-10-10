#pragma force_active on
#pragma dollar_identifiers on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define BASE    (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))
#define HW(a)   (*(volatile u16 *)(a))
int WSetDTIMPeriod(u16 x);

int WSetDTIMPeriod(u16 x) {
    if (x < 1 || x > 255) {
        return 5;
    }
    H(BASE, 0x3b8) = x;
    HW(0x0480808e) = x;
    HW(0x04808088) = 0;
    return 0;
}
