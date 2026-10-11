#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#define REG_IME (*(vu16 *)0x04000208)

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Clears the key station ID (register 0x028) once the MAC is not receiving or sending an MP frame; the
// check and the store run with interrupts disabled.
void WaitLoop_ClrAid(void) {
    u16 old;
    u32 wait = 1;
    while (wait) {
        old = REG_IME;
        u32 x;
        REG_IME = 0;
        x = WL_REG(0x19C) & 3;
        if (x != 3) {
            x = WL_REG(0x214);
            if (x != 5 && x != 7 && x != 8) {
                WL_REG(0x028) = 0;
                wait = 0;
            }
        }
        (void)REG_IME;
        REG_IME = old;
    }
}

// Returns 0 when the BBP is ready, 1 after 0x2800 polls.
int WaitLoop_BbpAccess(void) {
    u32 i;
    for (i = 0; i < 0x2800; i++) {
        if (!(WL_REG(0x15E) & 1)) {
            return 0;
        }
    }
    return 1;
}

// Returns 0 when the RF chip is ready, 1 after 0x2800 polls.
int WaitLoop_RfAccess(void) {
    u32 i;
    for (i = 0; i < 0x2800; i++) {
        if (!(WL_REG(0x180) & 1)) {
            return 0;
        }
    }
    return 1;
}

#pragma dont_inline reset
