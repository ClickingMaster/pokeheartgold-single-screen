#include "to56_w04_overlay08_bank06_private.h"

#pragma force_active on

void ov08_0221D0F4(Ov08W04State *work) {
    work->msgData = NewMsgDataFromNarc(0, 0x1B, 6, (*(Ov08W04Root **)work)->heapId);
    work->printer = MessagePrinter_New(0xF, 0xE, 0, (*(Ov08W04Root **)work)->heapId);
    work->msgFormat = MessageFormat_New((*(Ov08W04Root **)work)->heapId);
    work->string = String_New(0x200, (*(Ov08W04Root **)work)->heapId);
}
void ov08_0221D14C(Ov08W04State *work) {
    DestroyMsgData(work->msgData);
    MessagePrinter_Delete(work->printer);
    MessageFormat_Delete(work->msgFormat);
    String_Delete(work->string);
}
