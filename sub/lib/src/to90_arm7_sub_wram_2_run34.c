#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
void NVRAM_DeepPowerDown(void);

void NVRAM_DeepPowerDown(void) {
    volatile u16 *control = (volatile u16 *)0x040001C0;
    volatile u16 *data;
    while (*control & 0x80) {}
    data = (volatile u16 *)0x040001C2;
    *control = 0x8100;
    *data = 185;
    control = data - 1;
    while (*control & 0x80) {}
}
