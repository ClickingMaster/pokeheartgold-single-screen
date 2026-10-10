#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
void SPI_DummyWait_03803F70(void);

void SPI_DummyWait_03803F70(void) {
    volatile u16 *data = (volatile u16 *)0x040001C2;
    volatile u16 *control;
    *data = 0;
    control = data - 1;
    while (*control & 0x80) {}
}
