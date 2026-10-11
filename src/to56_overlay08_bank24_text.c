#include "to56_overlay08_bank24_private.h"
#pragma force_active on

void BattleBagMenu_InitText(u8 *work) {
    PTR(work, 0x10) = NewMsgDataFromNarc(0, 0x1B, 5, U32(PTR(work, 0), 0xC));
    PTR(work, 0xC) = MessagePrinter_New(0xF, 0xE, 0, U32(PTR(work, 0), 0xC));
    PTR(work, 0x14) = MessageFormat_New(U32(PTR(work, 0), 0xC));
    PTR(work, 0x18) = String_New(0x200, U32(PTR(work, 0), 0xC));
}
