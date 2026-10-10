#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern u32 ov00_0221B1F0[2];
extern u8 ov00_0221B1D8[24];
void *ov00_0220AD00(u32);

void *ov00_0220AD00(u32 x) {
    ov00_0221B1F0[0] = x;
    return ov00_0221B1D8 + 24;
}
