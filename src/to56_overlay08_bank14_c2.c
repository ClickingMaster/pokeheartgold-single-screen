#include "../include/to57_overlay08_bank14_private.h"
#pragma force_active on

void ov08_0221E340(void *arg, int msgNo) {
    Ov08R2W11Work *work = arg;
    void *string;
    DrawFrameAndWindow2(&work->windows[0], TRUE, TRUE, 0xE);
    FillWindowPixelBuffer(&work->windows[0], 0xF);
    string = NewString_ReadMsgData(work->msgData, msgNo);
    AddTextPrinterParameterizedWithColor(&work->windows[0], 1, string, 0, 0, 0xFF, 0x10200, 0);
    String_Delete(string);
    ScheduleWindowCopyToVram(&work->windows[0]);
}
