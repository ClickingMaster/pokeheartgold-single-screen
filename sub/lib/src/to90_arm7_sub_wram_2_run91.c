#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u16 nvramw[];
void NVRAM_Init(void);

void NVRAM_Init(void) {
    int i;
    for (i = 0; i < 16; i++) {
        nvramw[i] = 0;
    }
}
