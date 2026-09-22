#include "../include/to57_overlay08_bank14_private.h"
#pragma force_active on

void ov08_022201C0(void *arg) {
    Ov08R2W11Work *work = arg;
    DrawFrameAndWindow2(&work->windows[1], TRUE, TRUE, 0xE);
    FillWindowPixelBuffer(&work->windows[1], 0xF);
    ov08_022201E8(work);
}

void ov08_022201E8(void *work) {
    int delay;
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    delay = BattleSystem_GetTextFrameDelay(*(void **)((u8 *)*(void **)work + 8));
    *(u8 *)((u8 *)work + 0x207B) = AddTextPrinterParameterized((u8 *)work + 0x2060, 1, *(void **)((u8 *)work + 0x1FB0), 0, 0, delay, 0);
}
