#include "../include/to57_overlay08_bank14_private.h"
#pragma force_active on

void BattlePartyMenu_PrintPageText(void *work, int mode) {
    switch (mode) {
    case 0:
        BattlePartyMenu_PrintPartyListPage(work);
        break;
    case 1:
        BattlePartyMenu_PrintMonMenuPage(work);
        break;
    case 2:
        BattlePartyMenu_PrintSummaryPage(work);
        break;
    case 3:
        BattlePartyMenu_PrintCheckMovesPage(work);
        break;
    case 4:
        BattlePartyMenu_PrintMoveDetailPage(work);
        break;
    case 5:
        BattlePartyMenu_PrintRestorePPPage(work);
        break;
    case 6:
    case 8:
        BattlePartyMenu_PrintLearnMovePage(work);
        break;
    case 7:
        BattlePartyMenu_PrintForgetMovePage(work);
        break;
    case 9:
        BattlePartyMenu_PrintContestForgetMovePage(work);
        break;
    }
}
