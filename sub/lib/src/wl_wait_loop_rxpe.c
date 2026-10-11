#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Waits (at most 4000 polls) for bit 7 of register 0x19C (the receiver is up).
void WaitLoop_Rxpe(void) {
    u32 n;
    for (n = 4000; n; n--) {
        if (WL_REG(0x19C) & 128) {
            return;
        }
    }
}
