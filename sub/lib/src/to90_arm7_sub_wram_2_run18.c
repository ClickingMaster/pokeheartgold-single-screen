#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

void MI_SetCartridgeRomCycle2nd(u32 cycle);

void MI_SetCartridgeRomCycle2nd(u32 cycle) {
    *(volatile u16 *)0x4000204 = (*(volatile u16 *)0x4000204 & ~16) | (cycle << 4);
}
