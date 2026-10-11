#include "../include/to57_overlay08_bank14_private.h"
#pragma force_active on

void BattlePartyMenu_UpdateMovePP(void *work, int value, int slot) {
    u8 battler = *(u8 *)((u8 *)*(void **)work + 0x11);
    BattlePartyMenu_UpdateMoveListPP(work, (u8 *)work + 0x34 + battler * 0x50 + slot * 8, value);
}
