#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))

u8 SPI_DummyWaitReceive(void);

u8 SPI_DummyWaitReceive(void) {
    H(0x040001c2) = 0;
    while (H(0x040001c0) & 128) {}
    return (u8)H(0x040001c2);
}
