#include "../include/to56_overlay08_bank18_private.h"
#pragma force_active on

void BattlePartyMenu_ShowPageSprites(Ov08Work *work, u32 mode) {
    u32 i;
    for (i = 0; i < 38; i++) {
        ManagedSprite_SetDrawFlag(work->sprites[i], FALSE);
    }
    switch (mode) {
    case 0:
        BattlePartyMenu_ShowPartyListSprites(work);
        break;
    case 1:
        BattlePartyMenu_ShowMonMenuSprites(work);
        break;
    case 2:
        BattlePartyMenu_ShowSummarySprites(work);
        break;
    case 3:
        BattlePartyMenu_ShowCheckMovesSprites(work);
        break;
    case 4:
        BattlePartyMenu_ShowMoveDetailSprites(work);
        break;
    case 5:
        BattlePartyMenu_ShowRestorePPSprites(work);
        break;
    case 6:
        BattlePartyMenu_ShowLearnMoveSprites(work);
        break;
    case 7:
        BattlePartyMenu_ShowForgetMoveSprites(work);
        break;
    case 8:
        BattlePartyMenu_ShowContestLearnMoveSprites(work);
        break;
    case 9:
        BattlePartyMenu_ShowContestForgetMoveSprites(work);
        break;
    }
}
