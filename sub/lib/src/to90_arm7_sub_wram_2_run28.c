#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
void NVRAM_ReadStatusRegister(u8 *out);

void NVRAM_ReadStatusRegister(u8 *out) {
    volatile u16 *c = (volatile u16 *)0x040001C0;
    volatile u16 *d;
    while (*c & 128) {}
    d = (volatile u16 *)0x040001C2;
    *c = 0x8900;
    *d = 5;
    c = d - 1;
    while (*c & 128) {}
    d = (volatile u16 *)0x040001C2;
    *c = 0x8100;
    *d = 0;
    c = d - 1;
    while (*c & 128) {}
    *out = (u8) * (volatile u16 *)0x040001C2;
}
