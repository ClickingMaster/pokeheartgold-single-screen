#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

void SPI_SendWait_03803D98(u32 data);

void SPI_SendWait_03803D98(u32 data) {
    *(volatile u16 *)0x40001c2 = data & 255;
    while (*(volatile u16 *)0x40001c0 & 128) {}
}
