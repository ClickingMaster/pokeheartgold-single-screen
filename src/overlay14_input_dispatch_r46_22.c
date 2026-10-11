#include "overlay14_input_dispatch_r46_22_private.h"

u32 PCBox_State_MoveModeInput(void *work) {
    u32 input = PCBox_FindTouchedBoxSlot();

    if (input != -1) {
        if (PCBox_GetSlotMonData(work, input, 0xac, 0) != 0) {
            PlaySE(0x5eb);
            System_GetTouchNewCoords((u32 *)((u8 *)PTR_AT(work, 0x34) + 0x40b8),
                (u32 *)((u8 *)PTR_AT(work, 0x34) + 0x40bc));
            if (PCBox_IsActionMenuOpen(PTR_AT(PTR_AT(work, 0x34), 0x2f0)) == 0) {
                PCBox_DrawActionMenu(work, ov14_021F7D3C, 5);
            }
            PCBox_ShowSlotMonInfo(work, input);
            PCBox_SetSpriteVisible(PTR_AT(work, 0x34), 9, 0);
            return PCBox_StartMoveModeBoxDrag(work, input);
        }
        if (PCBox_IsActionMenuOpen(PTR_AT(PTR_AT(work, 0x34), 0x2f0)) == 1) {
            u32 old = U8_AT(work, 0x21);
            u32 next = GridInputHandler_GetNextInput(PTR_AT(PTR_AT(work, 0x34), 0x2c));
            PCBox_StartCursorSlide(PTR_AT(work, 0x34), old, next);
            GridInputHandler_SetNextInput(PTR_AT(PTR_AT(work, 0x34), 0x2c), old);
            PCBox_SlideOutActionMenu(PTR_AT(PTR_AT(work, 0x34), 0x2f0));
            PCBox_SlideInPartyButton(PTR_AT(PTR_AT(work, 0x34), 0x2f0));
            ov14_021E82A8(PTR_AT(PTR_AT(work, 0x34), 0x2f0));
            ov14_021E8328(PTR_AT(PTR_AT(work, 0x34), 0x2f0));
            ov14_021E884C(PTR_AT(work, 0x34));
            PCBox_SetSelectedIconOutlineVisible(work, 0);
            return PCBox_WaitForUpdateFunc(work, (void *)ov14_021EA180, 0x4a);
        }
        GridInputHandler_SetNextInput(PTR_AT(PTR_AT(work, 0x34), 0x2c), (u8)input);
        GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(work, 0x34), 0x2c), 1);
        PCBox_ClearMonInfo(work);
        return 0xc;
    }

    input = PCBox_GetMoveModeMenuInput(work);
    switch (input) {
    case 30:
        PlaySE(0x5dd);
        PCBox_ClearMonInfoAndSelection(work);
        return ov14_021F1128(work);
    case 31:
        PlaySE(0x5dc);
        PCBox_ClearMonInfoAndSelection(work);
        PCBox_SetSpriteVisible(PTR_AT(work, 0x34), 9, 0);
        GridInputHandler_SetNextInput(PTR_AT(PTR_AT(work, 0x34), 0x2c), 0x1e);
        return PCBox_ScrollToPrevBox(work, 0xc);
    case 32:
        PlaySE(0x5dc);
        PCBox_ClearMonInfoAndSelection(work);
        PCBox_SetSpriteVisible(PTR_AT(work, 0x34), 9, 0);
        GridInputHandler_SetNextInput(PTR_AT(PTR_AT(work, 0x34), 0x2c), 0x1e);
        return PCBox_ScrollToNextBox(work, 0xc);
    case 33:
        PlaySE(0x632);
        return PCBox_FlashPanel(work, 8, 0x95);
    case 34:
        PlaySE(0x632);
        U8_AT(work, 0x2a) = 0;
        U8_AT(work, 0x2b) = 0;
        return ov14_021F2330(work, 9, 0x96);
    case 36:
        PlaySE(0x5dd);
        if (System_GetTouchNew() == 0) {
            U8_AT(work, 0x2a) = 1;
        }
        U8_AT(work, 0x2b) = U8_AT(work, 0x21);
        return ov14_021F2330(work, 3, 0xb2);
    case 37:
        PlaySE(0x5dd);
        U32_AT(work, 0x2c) = 0x25;
        return PCBox_FlashPanel(work, 4, 0x97);
    case 38:
        PlaySE(0x5dd);
        return PCBox_FlashPanel(work, 5, 0x98);
    case 39:
        PlaySE(0x5dd);
        return PCBox_FlashPanel(work, 6, 0x99);
    case 40:
        PlaySE(0x5dd);
        return PCBox_FlashPanel(work, 7, 0x9b);
    case 41: {
        u32 old;
        PlaySE(0x5dc);
        old = U8_AT(work, 0x21);
        PCBox_StartCursorSlide(PTR_AT(work, 0x34), old, GridInputHandler_GetNextInput(PTR_AT(PTR_AT(work, 0x34), 0x2c)));
        GridInputHandler_SetNextInput(PTR_AT(PTR_AT(work, 0x34), 0x2c), old);
        return PCBox_FlashPanel(work, 0xb, 0x9c);
    }
    case -1: {
        u32 next = GridInputHandler_GetNextInput(PTR_AT(PTR_AT(work, 0x34), 0x2c));
        if (next == 0x1e) {
            if (U32_AT(gSystem, 0x48) & 0x20) {
                PlaySE(0x5dc);
                PCBox_ClearMonInfoAndSelection(work);
                return PCBox_ScrollToPrevBox(work, 0xc);
            }
            if (U32_AT(gSystem, 0x48) & 0x10) {
                PlaySE(0x5dc);
                PCBox_ClearMonInfoAndSelection(work);
                return PCBox_ScrollToNextBox(work, 0xc);
            }
        }
        break;
    }
    case -3: {
        u32 next = GridInputHandler_GetNextInput(PTR_AT(PTR_AT(work, 0x34), 0x2c));
        if (next < 0x1e) {
            PCBox_ShowSlotMonInfo(work, next);
        } else if (next != 0x24 && next != 0x25 && next != 0x26 && next != 0x27 && next != 0x28 && next != 0x29) {
            PCBox_ClearMonInfo(work);
        }
        PlaySE(0x5dc);
        return PCBox_WaitForPendingUpdateFunc(work, 0x4a);
    }
    case 35:
        PlaySE(0x5dd);
        PCBox_ClearMonInfo(work);
        return PCBox_FlashPanel(work, 10, 0x93);
    case -2:
        PlaySE(0x5dd);
        return PCBox_FlashPanel(work, 10, 0x94);
    case -4: {
        u32 next = GridInputHandler_GetNextInput(PTR_AT(PTR_AT(work, 0x34), 0x2c));
        if (next < 0x1e) {
            PCBox_ShowSlotMonInfo(work, next);
        } else if (next != 0x24 && next != 0x25 && next != 0x26 && next != 0x27 && next != 0x28 && next != 0x29) {
            PCBox_ClearMonInfo(work);
        }
        PlaySE(0x5dc);
        break;
    }
    default:
        if (PCBox_GetSlotMonData(work, input, 0xac, 0) != 0) {
            PlaySE(0x5dd);
            PCBox_DrawActionMenu(work, ov14_021F7D3C, 5);
            PCBox_ShowSlotMonInfo(work, input);
            PCBox_StartCursorSlide(PTR_AT(work, 0x34), 0x24, GridInputHandler_GetNextInput(PTR_AT(PTR_AT(work, 0x34), 0x2c)));
            GridInputHandler_SetNextInput(PTR_AT(PTR_AT(work, 0x34), 0x2c), 0x24);
            return ov14_021F0530(work, input);
        }
        break;
    }
    return 0xc;
}
