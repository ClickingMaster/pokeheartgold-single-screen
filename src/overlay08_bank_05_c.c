#include "overlay08_exact_bank_private.h"

#pragma force_active on

int BattlePartyMenu_WaitButtonAnim(const u8 *work) {
    if ((((u32)work[0x1FA3] << 24) >> 31) == 0) {
        return work[0x2079];
    }
    return 0x16;
}
