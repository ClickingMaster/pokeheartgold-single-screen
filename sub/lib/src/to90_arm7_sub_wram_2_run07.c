#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))

void SPI_SendWait_03801D48(u16 value);

void SPI_SendWait_03801D48(u16 value) {
    H(0x040001c2) = (u8)value;
    while (H(0x040001c0) & 128) {}
}
