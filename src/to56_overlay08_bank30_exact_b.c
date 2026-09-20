#include "to56_overlay08_bank30_private.h"

#pragma force_active on

void ov08_0222377C(W19Work *w) {
    u8 *work = (u8 *)w;
    FillBgTilemapRect(*(void **)(work + 4), 5, 0, 0, 0, 0x20, 0x13, 0x11);
    u16 i = 0;
    do {
        ov08_0222370C(w, i);
        i++;
    } while (i < 6);
    work[0x31] ^= 1;
}
