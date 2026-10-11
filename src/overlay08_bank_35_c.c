#include "overlay08_exact_bank_private.h"

#pragma force_active on

void BattleBagMenu_DeactivateCursor(u8 *work) {
    BattleDpadCursor *state = *(BattleDpadCursor **)(work + 0x34);
    BattleDpadCursor_SetActive(state, 0);
    BattleDpadCursor_ResetPosition(*(BattleDpadCursor **)(work + 0x34));
    BattleCursor_Disable(BattleDpadCursor_GetBattleCursor(*(BattleDpadCursor **)(work + 0x34)));
}
