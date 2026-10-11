#include "overlay14_box_input_state_prefix_private.h"

u32 PCBox_State_BoxTrayInput(void *p) {
    u32 v = ov14_021F6A34();
    if (v != -1) {
        if (PCBox_GetSlotMonData(p, v + 0x1e, 0xac, 0)) {
            PlaySE(0x5eb);
            System_GetTouchNewCoords((u32 *)((u8 *)PTR_AT(p, 0x34) + 0x40b8), (u32 *)((u8 *)PTR_AT(p, 0x34) + 0x40bc));
            PCBox_ShowSlotMonInfo(p, v + 0x1e);
            PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 0);
            return PCBox_StartBoxTrayDrag(p, v + 0x1e);
        }
        PCBox_ClearMonInfo(p);
        GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), (u8)(v + 0x1e));
        GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
        if (PCBox_IsChangeBoxButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            return ov14_021F0EE8(p, 0x29);
        }
        if (PCBox_IsSummaryButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            return PCBox_HideInfoPanelAndWait(p, 0x29);
        }
        return 0x29;
    }
    v = PCBox_FindTouchedBoxSlot();
    if (v != -1) {
        if (PCBox_GetSlotMonData(p, v, 0xac, 0)) {
            PlaySE(0x5eb);
            System_GetTouchNewCoords((u32 *)((u8 *)PTR_AT(p, 0x34) + 0x40b8), (u32 *)((u8 *)PTR_AT(p, 0x34) + 0x40bc));
            PCBox_ShowSlotMonInfo(p, v);
            PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 0);
            return PCBox_StartBoxTrayDrag(p, v);
        }
        PCBox_ClearMonInfo(p);
        GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), (u8)v);
        GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
        if (PCBox_IsChangeBoxButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            return ov14_021F0EE8(p, 0x29);
        }
        if (PCBox_IsSummaryButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            return PCBox_HideInfoPanelAndWait(p, 0x29);
        }
        return 0x29;
    }
    if (PCBox_IsSummaryShortcutPressed(p) == 1) {
        v = GridInputHandler_GetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c));
        if (PCBox_GetSlotMonData(p, v, 0xac, 0)) {
            PlaySE(0x5dd);
            U8_AT(p, 0x21) = (u8)v;
            U8_AT(p, 0x26) = 1;
            return ov14_021F2330(p, 0xf, 0x97);
        }
        return 0x29;
    }
    v = PCBox_GetBoxTrayInput(p);
    switch (v) {
    case 0x25:
        PlaySE(0x5dd);
        PCBox_ClearMonInfoAndSelection(p);
        return ov14_021F0D58(p, 0);
    case 0x26:
        PlaySE(0x5dd);
        PCBox_ClearMonInfoAndSelection(p);
        return ov14_021F0D58(p, 1);
    case 0x27:
        PlaySE(0x5dd);
        PCBox_ClearMonInfoAndSelection(p);
        return ov14_021F0D58(p, 2);
    case 0x28:
        PlaySE(0x5dd);
        PCBox_ClearMonInfoAndSelection(p);
        return ov14_021F0D58(p, 3);
    case 0x29:
        PlaySE(0x5dd);
        PCBox_ClearMonInfoAndSelection(p);
        return ov14_021F0D58(p, 4);
    case 0x2a:
        PlaySE(0x5dd);
        PCBox_ClearMonInfoAndSelection(p);
        return ov14_021F0D58(p, 5);
    case 0x2b:
        PlaySE(0x5dc);
        PCBox_ClearMonInfoAndSelection(p);
        PCBox_ChangeTrayPage(p, -1);
        GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), (u8)(U8_AT(p, 0x25) % 6 + 0x25));
        GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
        if (PCBox_IsChangeBoxButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            return ov14_021F0EE8(p, 0x29);
        }
        if (PCBox_IsSummaryButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            return PCBox_HideInfoPanelAndWait(p, 0x29);
        }
        return 0x29;
    case 0x2c:
        PlaySE(0x5dc);
        PCBox_ClearMonInfoAndSelection(p);
        PCBox_ChangeTrayPage(p, 1);
        GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), (u8)(U8_AT(p, 0x25) % 6 + 0x25));
        GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
        if (PCBox_IsChangeBoxButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            return ov14_021F0EE8(p, 0x29);
        }
        if (PCBox_IsSummaryButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            return PCBox_HideInfoPanelAndWait(p, 0x29);
        }
        return 0x29;
    case 0x2d:
        PlaySE(0x5dc);
        GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), (u8)(U8_AT(p, 0x25) % 6 + 0x25));
        return PCBox_FlashPanel(p, 0xe, 0xa0);
    case -3: {
        u32 n = GridInputHandler_GetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c));
        if (n < 0x25) {
            if (PCBox_ShowSlotMonInfo(p, n) == 1) {
                if (PCBox_IsSummaryButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 0) {
                    PCBox_DrawSummaryButton(p, 0);
                    PCBox_SlideInSummaryButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
                }
            } else if (PCBox_IsSummaryButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
                PCBox_SlideOutSummaryButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
            }
        } else {
            PCBox_ClearMonInfo(p);
            if (PCBox_IsSummaryButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
                PCBox_SlideOutSummaryButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
            }
        }
        PlaySE(0x5dc);
        if (U32_AT(PTR_AT(p, 0x34), 8) != 0) {
            return PCBox_WaitForPendingUpdateFunc(p, 0x4b);
        }
        break;
    }
    case 0x24:
        PlaySE(0x633);
        PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 9, 8);
        return ov14_021F2490(p, 1, 0xa1);
    case -2:
        if (PCBox_IsChangeBoxButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 0) {
            PlaySE(0x633);
            return ov14_021F2490(p, 1, 0xa1);
        }
        PlaySE(0x5dc);
        GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), (u8)(U8_AT(p, 0x25) % 6 + 0x25));
        GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
        return ov14_021F0EE8(p, 0x29);
    default:
        if (PCBox_GetSlotMonData(p, v, 0xac, 0)) {
            PlaySE(0x5eb);
            PCBox_ShowSlotMonInfo(p, v);
            return ov14_021F1580(p, v);
        }
        break;
    case -4:
    case -1:
        break;
    }
    return 0x29;
}
