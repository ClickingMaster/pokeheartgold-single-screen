#include "overlay08_exact_bank_private.h"

#pragma force_active on

void BattlePartyMenu_RemovePageWindows(u8 *work) {
    WindowArray_Delete(*(void **)(work + 0x2070), work[0x2074]);
}
