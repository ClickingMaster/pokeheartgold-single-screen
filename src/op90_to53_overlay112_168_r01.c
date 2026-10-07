#pragma force_active on
#include "global.h"
#include "assert.h"
#include "msgdata.h"
#include "bg_window.h"
typedef struct Ov112Work {
    u32 unk0;
    enum HeapID heapId;
    u8 pad8[0x10];
    Window windows[4];
    MessageFormat *msgFormat;
    MsgData *msgData;
} Ov112Work;
u8 AddTextPrinterParameterizedWithColor(Window *window, int fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, void *callback);
void ov112_021F135C(Ov112Work *work, u32 idx, u32 msgId, u16 x, int font);

// @fn ov112_021F135C
void ov112_021F135C(Ov112Work *work, u32 idx, u32 msgId, u16 x, int font) {
    String *str;
    GF_ASSERT(idx < 4);
    FillWindowPixelBuffer(&work->windows[idx], 0);
    str = ReadMsgData_ExpandPlaceholders(work->msgFormat, work->msgData, msgId, work->heapId);
    AddTextPrinterParameterizedWithColor(&work->windows[idx], font, str, x, 0, 0xff, 0x10200, NULL);
    String_Delete(str);
    CopyWindowToVram(&work->windows[idx]);

}
