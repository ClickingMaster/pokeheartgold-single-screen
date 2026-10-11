#include "../include/to57_overlay08_bank14_private.h"
#pragma force_active on

void BattlePartyMenu_PrintPartyMonLevel(void *work, int battler) {
    if (((u32) * (u8 *)((u8 *)work + battler * 0x50 + 0x1B) << 24) >> 31 == 0) {
        BattlePartyMenu_PrintMonLevel(work, battler, battler, 0, 0x20);
    }
}
