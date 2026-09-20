#include "to56_w13_overlay08_bank32_private.h"

#pragma force_active on

void ov08_02223B20(W13Work *w) {
    DrawFrameAndWindow2((u8 *)w + 0x1C, TRUE, 0x3E2, 14);
    FillWindowPixelBuffer((u8 *)w + 0x1C, 15);
    ov08_02223B48(w);
}

void ov08_02223B48(W13Work *w) {
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    *((u8 *)w + 0x32) = AddTextPrinterParameterized((u8 *)w + 0x1C, 1, w->string, 0, 0, BattleSystem_GetTextFrameDelay(w->root->battleSystem), NULL);
}

BOOL ov08_02223B78(W13Work *w) {
    if (w->root->item == 0) {
        return FALSE;
    }
    if (!Bag_HasItem(w->root->bag, w->root->item, 1, w->root->heapId)) {
        w->root->item = 0;
        w->root->unk1F = 0;
        return FALSE;
    }
    return TRUE;
}
