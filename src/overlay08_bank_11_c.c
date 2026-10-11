#include "overlay08_exact_bank_private.h"

#pragma force_active on

void BattlePartyMenu_ClearContestMoveInfo(u8 *work) {
    ov08_0221D81C(work);
    ScheduleBgTilemapBufferTransfer(*(void **)(work + 0x1E4), 7);
}
