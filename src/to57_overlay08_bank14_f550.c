#include "../include/to57_overlay08_bank14_private.h"
#pragma force_active on

void BattlePartyMenu_PrintMonHPAndBar(void *arg, int battler) {
    Ov08R2W11Work *work = arg;
    FillWindowPixelRect(&((Window *)work->windowArray)[battler], 0, 0x38, 0x20, 0x18, 8);
    FillWindowPixelRect(&((Window *)work->windowArray)[battler], 0, 0x40, 0x18, 0x40, 8);
    BattlePartyMenu_PrintMonHP(work, battler, battler, 0x38, 0x20);
    BattlePartyMenu_DrawHPBar(work, battler, battler, 0x40, 0x18);
}
