#include "overlay14_box_selection_menu_private.h"

u32 ov14_021EE338(void *p) {
    ov14_021F4BC0(p);
    PCBox_UpdateCurrentBoxMarker(p);
    PCBox_UpdateTrayCursor(p);
    PCBox_DrawTrayBoxNameAndCount(p);
    return ov14_021F10B4(p, 0x5f);
}

u32 ov14_021EE35C(void *p) {
    PCBox_DrawDepositTitle(p);
    ov14_021E8560(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    return PCBox_WaitForUpdateFunc(p, ov14_021E95C8, 0x60);
}

u32 ov14_021EE380(void *p) {
    U32_AT(PTR_AT(p, 0x34), 0x43c) = U8_AT(p, 0x25) % 6;
    PCBox_CreateGridInput(p, 1, U32_AT(PTR_AT(p, 0x34), 0x43c));
    PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 9, 0xe);
    PCBox_CloseMessageWindow(PTR_AT(p, 0x34), 0x25);
    PCBox_PrintMonActionMessage(p, 0, 3, 0x27);
    return 0x61;
}

u32 ov14_021EE3C8(void *p) {
    u32 input = GridInputHandler_HandleInput_AllowHold(PTR_AT(PTR_AT(p, 0x34), 0x2c));

    switch (input) {
    case 0:
        return ov14_021F1448(p, 0);
    case 1:
        return ov14_021F1448(p, 1);
    case 2:
        return ov14_021F1448(p, 2);
    case 3:
        return ov14_021F1448(p, 3);
    case 4:
        return ov14_021F1448(p, 4);
    case 5:
        return ov14_021F1448(p, 5);
    case 6:
        PlaySE(0x5dc);
        return ov14_021F1504(p, -1);
    case 7:
        PlaySE(0x5dc);
        return ov14_021F1504(p, 1);
    case 8:
        return ov14_021F1540(p);
    case -3:
        PlaySE(0x5dc);
        if (U32_AT(PTR_AT(p, 0x34), 8) != 0) {
            return PCBox_WaitForPendingUpdateFunc(p, 0x71);
        }
        break;
    case 9:
        PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 9, 8);
    case -2:
        PlaySE(0x5dc);
        return ov14_021F1534(p, 0);
    default:
        break;
    }
    return 0x61;
}

u32 ov14_021EE4AC(void *p) {
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 0);
    ov14_021E8588(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    return PCBox_WaitForUpdateFunc(p, ov14_021E9604, 0x63);
}
