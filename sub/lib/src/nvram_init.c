#pragma force_active on
#include <nitro/types.h>

#include "spi_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

void NVRAM_Init(void);

void NVRAM_Init(void) {
    int i;
    for (i = 0; i < 16; i++) {
        nvramw.command[i] = 0;
    }
}
