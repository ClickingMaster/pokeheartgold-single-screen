#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "text.h"

typedef struct Ov80Work {
    u8 pad0[0x34];
    enum HeapID heapID;
    u8 pad38[0x44 - 0x38];
    MessageFormat *msgFmt;
} Ov80Work;

void ov80_02233594(Ov80Work *work, Window *window, PlayerProfile *profile, u16 value);

// @fn ov80_02233594
void ov80_02233594(Ov80Work *work, Window *window, PlayerProfile *profile, u16 value) {
    String *tmp = String_New(20, work->heapID);
    String *buf = String_New(20, work->heapID);
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 31, HEAP_ID_FIELD2);

    BufferIntegerAsString(work->msgFmt, 0, value, 4, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    ReadMsgDataIntoString(msgData, 2, tmp);
    StringExpandPlaceholders(work->msgFmt, buf, tmp);
    AddTextPrinterParameterized(window, 0, buf, 16, 16, TEXT_SPEED_NOTRANSFER, NULL);
    BufferPlayersName(work->msgFmt, 0, profile);
    ReadMsgDataIntoString(msgData, 0, tmp);
    StringExpandPlaceholders(work->msgFmt, buf, tmp);
    AddTextPrinterParameterized(window, 0, buf, 0, 0, TEXT_SPEED_INSTANT, NULL);
    String_Delete(tmp);
    String_Delete(buf);
    DestroyMsgData(msgData);
    CopyWindowToVram(window);
}
