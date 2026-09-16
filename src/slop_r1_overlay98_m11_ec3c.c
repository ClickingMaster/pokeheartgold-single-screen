#include "slop_r1_overlay98_module11_internal.h"

void ov98_0221EC3C(Ov98TextContext *textSys, u32 windowIdx, u32 msgId, BOOL center, u8 x, u8 y, u32 fontId, u8 fill) {
    u8 xOffset = 0;
    String *string;

    GF_ASSERT(windowIdx < textSys->windowCount);
    string = ReadMsgData_ExpandPlaceholders(textSys->msgFormat, textSys->msgData, msgId, textSys->heapId);
    if (center) {
        xOffset = GetWindowWidth(&textSys->windows[windowIdx]) * 8;
        xOffset -= FontID_String_GetWidthMultiline(fontId, string, 0);
        xOffset /= 2;
    }
    FillWindowPixelBuffer(&textSys->windows[windowIdx], fill);
    AddTextPrinterParameterizedWithColor(&textSys->windows[windowIdx], fontId, string, xOffset + x, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    ScheduleWindowCopyToVram(&textSys->windows[windowIdx]);
    String_Delete(string);
}
