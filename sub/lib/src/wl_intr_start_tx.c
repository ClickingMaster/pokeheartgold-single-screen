#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// TX start: (unk_690 bit 5) restarts the TX when its pointer lies between the queue 2 and queue 4 frames,
// then waits for register 0x2B8 to move; it sets fatal error 0x40 when it does not.
void WlIntrStartTx(void) {
    WlTxCtrl *txCtrl = &wlWorkPlain->txCtrl;
    WL_REG(0x010) = 0x80;
    if (wlWorkPlain->unk_690 & 0x20) {
        u32 state = WL_REG(0x214) & 0xFF;
        u16 pointer = WL_REG(0x268);
        if (state >= 3 && state <= 5 && pointer >= (((u32)txCtrl->txq[4].frame >> 1) & 0xFFF) && pointer <= (((u32)txCtrl->txq[2].frame >> 1) & 0xFFF)) {
            WL_REG(0x244) |= 0x80;
            WL_REG(0x244) &= ~0x80;
        }
    }
    if (WL_REG(0x000) != 0x1440 && (WL_REG(0x19C) & 0x42) == 0x42) {
        u16 value = WL_REG(0x2B8);
        if (value != 0) {
            u32 i = 0;
            while (value == WL_REG(0x2B8)) {
                if (i++ > 1000) {
                    SetFatalErr(0x40);
                    return;
                }
            }
        }
    }
}
