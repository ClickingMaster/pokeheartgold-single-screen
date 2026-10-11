#include "overlay14_box_move_actions_r38_private.h"

u32 ov14_021F1808(void *p, u32 value) {
    U8_AT(p, 0x21) = value;
    U16_AT(PTR_AT(p, 0x34), 0x88c8) = PCBox_GetSlotMonData(p, U8_AT(p, 0x21), 6, 0);
    PCBox_DrawBagGiveButton(p, U16_AT(PTR_AT(p, 0x34), 0x88c8));
    PCBox_SlideInActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    PCBox_SlideOutPartyButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E8294(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E8314(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    if (U16_AT(PTR_AT(p, 0x34), 0x88c8) != 0) {
        PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 11, 0);
        PCBox_MoveHeldItemIconToSlot(PTR_AT(p, 0x34), U8_AT(p, 0x21), 0);
        PCBox_LoadHeldItemIcon(PTR_AT(p, 0x34), U16_AT(PTR_AT(p, 0x34), 0x88c8));
        PCBox_CreateHeldItemIconOutline(PTR_AT(p, 0x34));
        ov14_021E88BC(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    }
    return PCBox_WaitForUpdateFunc(p, ov14_021EA408, 0x76);
}

u32 ov14_021F18B0(void *p, u32 value) {
    s16 xy[2];
    u16 oldItem = U16_AT(PTR_AT(p, 0x34), 0x88c8);
    U8_AT(p, 0x21) = value;
    U16_AT(PTR_AT(p, 0x34), 0x88c8) = PCBox_GetSlotMonData(p, U8_AT(p, 0x21), 6, 0);
    if (PCBox_IsActionMenuOpen(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
        if (oldItem != 0 || U16_AT(PTR_AT(p, 0x34), 0x88c8) != 0) {
            ov14_021E83F4(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        } else if (U16_AT(PTR_AT(p, 0x34), 0x88c8) != 0) {
            PCBox_SlideOutActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        }
    } else {
        PCBox_SlideOutPartyButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        ov14_021E8294(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        ov14_021E8314(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    }
    PCBox_DrawBagGiveButton(p, U16_AT(PTR_AT(p, 0x34), 0x88c8));
    if (U16_AT(PTR_AT(p, 0x34), 0x88c8) != 0) {
        PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 11, 0);
        PCBox_MoveHeldItemIconToSlot(PTR_AT(p, 0x34), U8_AT(p, 0x21), 0);
        PCBox_LoadHeldItemIcon(PTR_AT(p, 0x34), U16_AT(PTR_AT(p, 0x34), 0x88c8));
        PCBox_CreateHeldItemIconOutline(PTR_AT(p, 0x34));
        PCBox_SetSlotIconTranslucent(PTR_AT(p, 0x34), U8_AT(p, 0x21), 1);
        ov14_021E88BC(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    } else if (PCBox_IsSpriteVisible(PTR_AT(p, 0x34), 11) == 1) {
        PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 11, 0);
        PCBox_HideSelectedIconOutline(p);
    }
    PCBox_GetSlotPosition(U8_AT(p, 0x21), &xy[1], &xy[0], 0);
    U32_AT(PTR_AT(p, 0x34), 0x40b8) = xy[1] + 8;
    U32_AT(PTR_AT(p, 0x34), 0x40bc) = xy[0] + 8;
    ov14_021F1F24(p);
    return PCBox_WaitForUpdateFunc(p, ov14_021EA4C8, 0x7e);
}

u32 ov14_021F19F0(void *p, u32 value) {
    s16 xy[2];
    U8_AT(p, 0x21) = value;
    U16_AT(PTR_AT(p, 0x34), 0x88c8) = PCBox_GetSlotMonData(p, U8_AT(p, 0x21), 6, 0);
    PCBox_DrawSummaryButton(p, 0);
    if (U16_AT(PTR_AT(p, 0x34), 0x88c8) == 0) {
        GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), U8_AT(p, 0x21));
        GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
        if (PCBox_IsSummaryButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            return 0x82;
        }
        if (PCBox_IsChangeBoxButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            PCBox_SlideOutChangeBoxButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        }
        return PCBox_WaitForUpdateFunc(p, ov14_021E99A0, 0x82);
    }
    PlaySE(0x5eb);
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 11, 0);
    PCBox_MoveHeldItemIconToSlot(PTR_AT(p, 0x34), U8_AT(p, 0x21), 2);
    PCBox_LoadHeldItemIcon(PTR_AT(p, 0x34), U16_AT(PTR_AT(p, 0x34), 0x88c8));
    PCBox_CreateHeldItemIconOutline(PTR_AT(p, 0x34));
    PCBox_SetSlotIconTranslucent(PTR_AT(p, 0x34), U8_AT(p, 0x21), 1);
    ov14_021E88BC(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    if (PCBox_IsChangeBoxButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
        PCBox_SlideOutChangeBoxButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    } else if (PCBox_IsSummaryButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
        PCBox_SlideOutSummaryButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    }
    PCBox_GetSlotPosition(U8_AT(p, 0x21), &xy[1], &xy[0], 2);
    U32_AT(PTR_AT(p, 0x34), 0x40b8) = xy[1] + 8;
    U32_AT(PTR_AT(p, 0x34), 0x40bc) = xy[0] + 8;
    ov14_021F1F24(p);
    return PCBox_WaitForUpdateFunc(p, ov14_021EA778, 0x84);
}

u32 ov14_021F1B4C(void *p, u32 value) {
    U8_AT(p, 0x21) = value;
    U16_AT(PTR_AT(p, 0x34), 0x88ca) = value;
    U16_AT(PTR_AT(p, 0x34), 0x88c8) = PCBox_GetSlotMonData(p, U8_AT(p, 0x21), 6, 0);
    if (U16_AT(PTR_AT(p, 0x34), 0x88c8) == 0) {
        return 0x82;
    }
    PlaySE(0x5eb);
    U8_AT(PTR_AT(p, 0x34), 0x44b) = 1;
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 11, 0);
    ov14_021F39A0(PTR_AT(p, 0x34), U8_AT(p, 0x21), 2);
    PCBox_LoadHeldItemIcon(PTR_AT(p, 0x34), U16_AT(PTR_AT(p, 0x34), 0x88c8));
    PCBox_SetSlotIconTranslucent(PTR_AT(p, 0x34), U8_AT(p, 0x21), 1);
    ov14_021E88BC(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    if (PCBox_IsSummaryButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
        PCBox_SlideOutSummaryButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    }
    return PCBox_WaitForUpdateFunc(p, ov14_021EAA04, 0x88);
}
