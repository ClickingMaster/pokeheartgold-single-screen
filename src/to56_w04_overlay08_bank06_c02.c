#include "to56_w04_overlay08_bank06_private.h"

#pragma force_active on

void BattlePartyMenu_InitText(Ov08W04State *work) {
    work->msgData = NewMsgDataFromNarc(0, 0x1B, 6, (*(Ov08W04Root **)work)->heapId);
    work->printer = MessagePrinter_New(0xF, 0xE, 0, (*(Ov08W04Root **)work)->heapId);
    work->msgFormat = MessageFormat_New((*(Ov08W04Root **)work)->heapId);
    work->string = String_New(0x200, (*(Ov08W04Root **)work)->heapId);
}
void BattlePartyMenu_FreeText(Ov08W04State *work) {
    DestroyMsgData(work->msgData);
    MessagePrinter_Delete(work->printer);
    MessageFormat_Delete(work->msgFormat);
    String_Delete(work->string);
}
