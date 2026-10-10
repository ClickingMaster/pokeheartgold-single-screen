#pragma force_active on
#pragma dollar_identifiers on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define BASE    (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))
#define HW(a)   (*(volatile u16 *)(a))
void WDisableTmpttPowerSave(void);

void WDisableTmpttPowerSave(void) {
    H(BASE, 0x3ea) = 1;
    if (H(BASE, 0x468) == 0) {
        HW(0x04808038) &= ~2;
        HW(0x04808048) = 0;
    }
}
