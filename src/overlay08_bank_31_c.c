#include "overlay08_exact_bank_private.h"

#pragma force_active on

void BattleBagMenu_PrintItemListPage(void *work) {
    BattleBagMenu_PrintItemList(work);
    BattleBagMenu_PrintPocketName(work);
    BattleBagMenu_PrintPageNumber(work);
}
