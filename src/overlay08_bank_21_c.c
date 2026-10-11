#include "overlay08_exact_bank_private.h"

#pragma force_active on

int BattleBagMenu_ShowPocketMenuPage(void *work) {
    BattleBagMenu_ChangePage(work, 0);
    return 1;
}

int BattleBagMenu_ShowItemListPage(void *work) {
    BattleBagMenu_ChangePage(work, 1);
    return 2;
}

int BattleBagMenu_ShowUseItemPage(void *work) {
    BattleBagMenu_ChangePage(work, 2);
    return 3;
}

int BattleBagMenu_CloseMessage(u8 *work) {
    ClearFrameAndWindow2(work + 0x1C, FALSE);
    return 3;
}

int BattleBagMenu_WaitMessage(const u8 *work) {
    if (TextPrinterCheckActive(work[0x32]) == 0) {
        return 10;
    }
    return 9;
}
