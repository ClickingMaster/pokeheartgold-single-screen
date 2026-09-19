#include "to52_r7_overlay59_r5_internal.h"

extern void FillWindowPixelBuffer(void *window, int fillValue);
extern void ov59_02239D08(Ov59Work *work, int msgNo);
extern u8 ov59_02239EA8(Ov59Work *work);
extern u8 AddTextPrinterParameterized(void *window, int font, void *string, int x, int y, int speed, void *callback);
extern void ScheduleWindowCopyToVram(void *window);
extern void DrawFrameAndWindow2(void *window, int copyToVram, int baseTile, int palette);
extern void TextFlags_SetCanTouchSpeedUpPrint(BOOL enable);

void ov59_02238C40(Ov59Work *work, int msgNo) {
    void *window = (u8 *)work + 0x138;

    FillWindowPixelBuffer(window, 0);
    ov59_02239D08(work, msgNo);
    AddTextPrinterParameterized(window, 0, PTRAT(work, 0x68), 0, 0, 0xFF, NULL);
    ScheduleWindowCopyToVram(window);
}

void ov59_02238C7C(Ov59Work *work) {
    void *window = (u8 *)work + 0x168;
    u32 selection;

    FillWindowPixelBuffer(window, 0xF);
    selection = ov59_02239EA8(work);
    if (selection == 8) {
        ScheduleWindowCopyToVram(window);
        return;
    }
    if (selection <= 6) {
        if (U8AT(work, 0x1A + selection) == 0) {
            ScheduleWindowCopyToVram(window);
            return;
        }
        ov59_02239D08(work, selection + 0xD);
    } else if (U8AT(work, 0x14) != 0) {
        ov59_02239D08(work, 0x15);
    } else {
        ov59_02239D08(work, 0x14);
    }
    AddTextPrinterParameterized(window, 0, PTRAT(work, 0x68), 0, 0, 0xFF, NULL);
    ScheduleWindowCopyToVram(window);
}

void ov59_02238CFC(Ov59Work *work, int msgNo, int speed) {
    void *window = (u8 *)work + 0x128;

    FillWindowPixelBuffer(window, 0xF);
    DrawFrameAndWindow2(window, 1, 1, 0xD);
    ov59_02239D08(work, msgNo);
    if (speed != 0xFF && speed != 0) {
        TextFlags_SetCanTouchSpeedUpPrint(TRUE);
        U8AT(work, 0x47) = AddTextPrinterParameterized(window, 1, PTRAT(work, 0x68), 0, 0, speed, NULL);
    } else {
        AddTextPrinterParameterized(window, 1, PTRAT(work, 0x68), 0, 0, speed, NULL);
        U8AT(work, 0x47) = 0xFF;
    }
    ScheduleWindowCopyToVram(window);
}
