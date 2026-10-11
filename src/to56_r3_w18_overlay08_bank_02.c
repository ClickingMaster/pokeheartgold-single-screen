#include "to56_r3_w18_overlay08_bank_02_private.h"
#pragma force_active on
int BattlePartyMenu_ShowLearnMovePage(Ov08WorkR3W18 *work) {
    if (work->mode == 0) {
        BattlePartyMenu_ChangePage(work, 6);
    } else {
        BattlePartyMenu_ChangePage(work, 8);
    }
    return 19;
}
int BattlePartyMenu_ShowForgetMovePage(Ov08WorkR3W18 *work) {
    BattlePartyMenu_ShowMoveIcons(work);
    if (work->mode == 0) {
        BattlePartyMenu_ChangePage(work, 7);
    } else {
        BattlePartyMenu_ChangePage(work, 9);
    }
    return 20;
}
int BattlePartyMenu_ShowRestorePPPage(Ov08OwnerR3W18 **work) {
    BattlePartyMenu_ChangePage(work, 5);
    if (GetItemAttr((*work)->item, 0x25, (*work)->heapId) != 0) {
        return 24;
    }
    return 21;
}
int BattlePartyMenu_RefreshMonPage(Ov08WorkR3W18 *work) {
    BattlePartyMenu_ShowPageSprites(work, work->selection);
    BattlePartyMenu_PrintPageText(work, work->selection);
    BattlePartyMenu_DrawPageButtons(work, work->selection);
    BattlePartyMenu_DrawExpBar(work, work->selection);
    if (work->selection == 2) {
        return 3;
    }
    return 4;
}
