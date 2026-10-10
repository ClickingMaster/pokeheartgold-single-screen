#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))
extern u8 micw[];
int MicSetTimerValue(u32 value);

int MicSetTimerValue(u32 value) {
    if (value < 0x10000) {
        *(u16 *)(micw + 0x36) = 0;
        *(u16 *)(micw + 0x34) = (u16)(0x10000 - value);
        return 1;
    }
    if (value < 0x400000) {
        *(u16 *)(micw + 0x36) = 1;
        *(u16 *)(micw + 0x34) = (u16)(0x10000 - (value >> 6));
        return 1;
    }
    if (value < 0x1000000) {
        *(u16 *)(micw + 0x36) = 2;
        *(u16 *)(micw + 0x34) = (u16)(0x10000 - (value >> 8));
        return 1;
    }
    if (value < 0x4000000) {
        *(u16 *)(micw + 0x36) = 3;
        *(u16 *)(micw + 0x34) = (u16)(0x10000 - (value >> 10));
        return 1;
    }
    return 0;
}
