#include "overlay08_exact_bank_private.h"

#pragma force_active on

int BattlePartyMenu_ShowPartyListPage(void *work) {
    BattlePartyMenu_ChangePage(work, 0);
    return 1;
}

int BattlePartyMenu_ShowMonMenuPage(void *work) {
    BattlePartyMenu_ChangePage(work, 1);
    return 2;
}

int BattlePartyMenu_ShowSummaryPage(void *work) {
    BattlePartyMenu_ChangePage(work, 2);
    return 3;
}

int BattlePartyMenu_ShowCheckMovesPage(void *work) {
    BattlePartyMenu_ChangePage(work, 3);
    return 4;
}

int BattlePartyMenu_ShowMoveDetailPage(void *work) {
    BattlePartyMenu_ChangePage(work, 4);
    return 5;
}
