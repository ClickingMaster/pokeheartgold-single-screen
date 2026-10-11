#include "overlay08_exact_bank_private.h"

#pragma force_active on

void BattleBagMenu_FreeText(u8 *work) {
    DestroyMsgData(*(void **)(work + 0x10));
    MessagePrinter_Delete(*(void **)(work + 0xC));
    MessageFormat_Delete(*(void **)(work + 0x14));
    String_Delete(*(void **)(work + 0x18));
}
