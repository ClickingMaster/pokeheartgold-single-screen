#include "overlay08_exact_bank_private.h"

#pragma force_active on

int BattleBagMenu_WaitButtonAnim(const u8 *work) {
    if (work[0x113E] == 2) {
        return work[0x114B];
    }
    return 11;
}
