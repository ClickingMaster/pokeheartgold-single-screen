#include "global.h"
#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#pragma force_active on
void ov40_02237030(void *work, int messageId);
// @fn ov40_02237030
void ov40_02237030(void *work, int messageId) {
    void *panel = *(void **)((u8 *)work + 0x860);
    Window *window = (Window *)((u8 *)panel + 0x1D4);
    MsgData *messages = *(void **)((u8 *)work + 0x48);
    String *message = NewString_ReadMsgData(messages, messageId);
    FillWindowPixelBuffer(window, 0xCC);
    AddTextPrinterParameterizedWithColor(window, 0, message, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(message);
}
