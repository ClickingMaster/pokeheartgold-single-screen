#include "r40_overlay_14_residual_28_private.h"

void ov14_021ED62C(Ov14R28Data *data) {
    PCBox_DrawBoxMenu(data);
    PCBox_SlideInBoxMenu(*(void **)((u8 *)data->unk34 + 0x2f0));
    PCBox_WaitForUpdateFunc(data, PCBox_UpdateBoxMenuSlide, 0x38);
}

u32 ov14_021ED650(Ov14R28Data *data) {
    PCBox_CreateGridInput(data, 9, data->unk2c);
    if (data->unk2c <= 5) {
        PCBox_SetSpriteAnim(data->unk34, 9, 0xe);
    } else {
        PCBox_SetSpriteAnim(data->unk34, 9, 8);
    }
    PCBox_PrintMenuPrompt(data, 0, 0x27);
    return 0x3d;
}

void ov14_021ED684(Ov14R28Data *data) {
    PCBox_SetSpriteVisible(data->unk34, 9, 0);
    PCBox_CloseMessageWindow(data->unk34, 0x27);
    ov14_021F10DC(data, 0x3a);
}

void ov14_021ED6A4(Ov14R28Data *data) {
    PCBox_SlideOutBoxMenu(*(void **)((u8 *)data->unk34 + 0x2f0));
    PCBox_WaitForUpdateFunc(data, PCBox_UpdateBoxMenuSlide, 0x3b);
}
