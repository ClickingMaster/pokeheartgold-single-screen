#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

void NVRAM_WriteEnable(void);

void NVRAM_WriteEnable(void) {
    while (*(volatile u16 *)0x40001c0 & 128) {}
    *(volatile u16 *)0x40001c0 = 0x8100;
    *(volatile u16 *)0x40001c2 = 6;
    while (*(volatile u16 *)0x40001c0 & 128) {}
}
