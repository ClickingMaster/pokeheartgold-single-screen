#include "to56_overlay08_bank30_private.h"

#pragma force_active on

void BattleBagMenu_PrintPocketName(W19Work *w) {
    u8 *work = (u8 *)w;
    FillWindowPixelBuffer((u8 *)*(void **)(work + 0x2C) + 0x180, 0);
    switch (work[0x114D]) {
    case 0:
        BattleBagMenu_PrintCenteredText(w, 0x18, 0x16, 0, 4, 0x10200);
        BattleBagMenu_PrintCenteredText(w, 0x18, 0x17, 0, 0x14, 0x10200);
        break;
    case 1:
        BattleBagMenu_PrintCenteredText(w, 0x18, 0x18, 0, 4, 0x10200);
        BattleBagMenu_PrintCenteredText(w, 0x18, 0x19, 0, 0x14, 0x10200);
        break;
    case 2:
        BattleBagMenu_PrintCenteredText(w, 0x18, 0x1A, 0, 0xC, 0x10200);
        break;
    case 3:
        BattleBagMenu_PrintCenteredText(w, 0x18, 0x1B, 0, 0xC, 0x10200);
        break;
    }
}
