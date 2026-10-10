#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
u8 WMSP_GetRssi8_06001B5C(int x);

u8 WMSP_GetRssi8_06001B5C(int x) {
    if (x & 2) {
        return x >> 2;
    } else {
        return (x >> 2) + 25;
    }
}
