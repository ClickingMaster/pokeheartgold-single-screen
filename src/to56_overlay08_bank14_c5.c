#include "../include/to57_overlay08_bank14_private.h"
#pragma force_active on

void BattlePartyMenu_ShowMessage(void *arg) {
    Ov08R2W11Work *work = arg;
    DrawFrameAndWindow2(&work->windows[1], TRUE, TRUE, 0xE);
    FillWindowPixelBuffer(&work->windows[1], 0xF);
    BattlePartyMenu_PrintMessage(work);
}

void BattlePartyMenu_PrintMessage(void *work) {
    int delay;
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    delay = BattleSystem_GetTextFrameDelay(*(void **)((u8 *)*(void **)work + 8));
    *(u8 *)((u8 *)work + 0x207B) = AddTextPrinterParameterized((u8 *)work + 0x2060, 1, *(void **)((u8 *)work + 0x1FB0), 0, 0, delay, 0);
}
