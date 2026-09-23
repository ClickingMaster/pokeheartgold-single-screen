#include "to57_overlay08_bank14_private.h"
#pragma force_active on
void ov08_0221E3A4(void *arg, int window, int msgNo) {
    Ov08R2W11Work *work = arg;
    Window *windows = work->windowArray;
    Window *target = &windows[window];
    void *string = NewString_ReadMsgData(work->msgData, msgNo);
    u32 width = FontID_String_GetWidth(4, string, 0);
    u32 windowWidth = GetWindowWidth(target);
    AddTextPrinterParameterizedWithColor(target, 4, string, (windowWidth * 8 - width) / 2, 5, 0xFF, 0x70809, 0);
    String_Delete(string);
    ScheduleWindowCopyToVram(target);
}
