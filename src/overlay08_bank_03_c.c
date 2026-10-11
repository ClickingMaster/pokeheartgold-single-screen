#include "overlay08_exact_bank_private.h"

#pragma force_active on

int BattlePartyMenu_ShowPendingMessage(u8 *work) {
    BattlePartyMenu_ShowMessage(work);
    work[0x2079] = 0x10;
    return 0x11;
}

int BattlePartyMenu_CloseMessage(u8 *work) {
    ClearFrameAndWindow2(work + 0x2060, FALSE);
    return 2;
}

int BattlePartyMenu_WaitMessage(const u8 *work) {
    if (TextPrinterCheckActive(work[0x207B]) == 0) {
        return 0x12;
    }
    return 0x11;
}
