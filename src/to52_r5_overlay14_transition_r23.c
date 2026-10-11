#include "to52_r5_overlay14_transition_r23_private.h"

void PCBox_State_OpenMarkingMenu(void *work) {
    *(void **)((u8 *)*(void **)((u8 *)work + 0x34) + 0x40c0) = PCBox_GetSlotMonData(work, *((u8 *)work + 0x21), 0xb, 0);
    PCBox_DrawMarkingMenu(work);
    PCBox_DrawConfirmCancel(*(void **)((u8 *)work + 0x34));
    PCBox_SlideInMarkingMenu(*(void **)((u8 *)*(void **)((u8 *)work + 0x34) + 0x2f0));
    PCBox_WaitForUpdateFunc(work, PCBox_UpdateMarkingMenuSlide, 0x15);
}

u32 ov14_021EBE2C(void *work) {
    PCBox_CreateGridInput(work, 8, 0);
    PCBox_SetIconsTranslucent(work, 1, 1);
    PCBox_SetIconsTranslucent(work, 2, 1);
    PCBox_SetSlotIconTranslucent(*(void **)((u8 *)work + 0x34), *((u8 *)work + 0x21), 0);
    PCBox_PrintTouchTheMark(work, 0x26);
    return 0x16;
}

u32 PCBox_State_MarkingMenuInput(void *work) {
    u32 input = GridInputHandler_HandleInput_AllowHold(*(void **)((u8 *)*(void **)((u8 *)work + 0x34) + 0x2c));
    switch (input) {
    case 0:
        PlaySE(0x5dd);
        PCBox_ToggleMarking(work, 0);
        break;
    case 1:
        PlaySE(0x5dd);
        PCBox_ToggleMarking(work, 1);
        break;
    case 2:
        PlaySE(0x5dd);
        PCBox_ToggleMarking(work, 2);
        break;
    case 3:
        PlaySE(0x5dd);
        PCBox_ToggleMarking(work, 3);
        break;
    case 4:
        PlaySE(0x5dd);
        PCBox_ToggleMarking(work, 4);
        break;
    case 5:
        PlaySE(0x5dd);
        PCBox_ToggleMarking(work, 5);
        break;
    case 6: {
        u8 value = *(u32 *)((u8 *)*(void **)((u8 *)work + 0x34) + 0x40c0);
        PCBox_SetSlotMonData(work, *((u8 *)work + 0x21), 0xb, &value);
        PCBox_DrawMarkings(work, value);
        PlaySE(0x5dd);
        return ov14_021F23F0(work, 0, 0x9a);
    }
    case 7:
    case (u32)-2:
        PlaySE(0x5dc);
        return ov14_021F23F0(work, 1, 0x9a);
    case (u32)-3:
        PlaySE(0x5dc);
        return PCBox_WaitForPendingUpdateFunc(work, 0x18);
    case (u32)-4:
        PlaySE(0x5dc);
        break;
    }
    return 0x16;
}
