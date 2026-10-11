#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#define WL_MAC_MEMORY      ((vu16 *)0x04804000)
#define WL_MAC_MEMORY_SIZE 0x2000

// Self test of the 8 KB MAC memory with a falling count and the alternating patterns 0x5A5A and 0xA5A5.
// More than 32 mismatches end the test; any mismatch sets bit 1 of WlParam.errors.
void DiagMacMemory(void) {
    vu16 *p = WL_MAC_MEMORY;
    u32 errors = 0, i;
    u16 pattern = 0xFFFF;
    for (i = 0; i < WL_MAC_MEMORY_SIZE; i += 2) {
        *p++ = pattern--;
    }
    p = WL_MAC_MEMORY;
    pattern = 0xFFFF;
    for (i = 0; i < WL_MAC_MEMORY_SIZE; i += 2, p++, pattern--) {
        if (*p != pattern && errors++ > 32) {
            goto done;
        }
    }
    p = WL_MAC_MEMORY;
    pattern = 0x5A5A;
    for (i = 0; i < WL_MAC_MEMORY_SIZE; i += 2) {
        *p++ = pattern;
        pattern = ~pattern;
    }
    p = WL_MAC_MEMORY;
    pattern = 0x5A5A;
    for (i = 0; i < WL_MAC_MEMORY_SIZE; i += 2, p++, pattern = (u16)~pattern) {
        if (*p != pattern && errors++ > 32) {
            goto done;
        }
    }
    p = WL_MAC_MEMORY;
    pattern = 0xA5A5;
    for (i = 0; i < WL_MAC_MEMORY_SIZE; i += 2) {
        *p++ = pattern;
        pattern = ~pattern;
    }
    p = WL_MAC_MEMORY;
    pattern = 0xA5A5;
    for (i = 0; i < WL_MAC_MEMORY_SIZE; i += 2, p++, pattern = (u16)~pattern) {
        if (*p != pattern && errors++ > 32) {
            goto done;
        }
    }
done:
    if (errors) {
        wlWorkPlain->param.errors |= 2;
    }
}
