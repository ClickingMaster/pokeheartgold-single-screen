#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
u32 RtcBCD2HEX(u32 value);

u32 RtcBCD2HEX(u32 value) {
    u32 result = 0, multiplier;
    int i, j;
    for (i = 0; i < 8; i++) {
        if (((value >> (i * 4)) & 15) >= 10) {
            return 0;
        }
    }
    j = 0;
    multiplier = 1;
    for (; j < 8; j++) {
        result += multiplier * ((value >> (j * 4)) & 15);
        multiplier *= 10;
    }
    return result;
}
