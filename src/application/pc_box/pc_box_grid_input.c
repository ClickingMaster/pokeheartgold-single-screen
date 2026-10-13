#include "global.h"

#include "application/pc_box/pc_box_internal.h"

#include "dpad_menu_box.h"
#include "grid_input_handler.h"
#include "heap.h"
#include "sprite_system.h"
#include "system.h"
#include "touchscreen.h"

extern const TouchscreenHitbox ov14_021F86C8[];

void PCBox_GridOnKeyMove(PCBoxWork *work, u32 a1, u32 a2) {
    PCBox_StartCursorSlide(work->view, a1, a2);
    PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
}

void PCBox_GridOnTouch(PCBoxWork *work, u32 a1) {
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)a1);
    PCBox_PlaceCursorOnGridInput(work, a1);
}

u32 PCBox_GetDepositMenuInput(PCBoxWork *work) {
    u32 nextInput = GridInputHandler_GetNextInput(work->view->gridInput);
    u32 input = GridInputHandler_HandleInput_AllowHold(work->view->gridInput);
    if (input < 12) {
        if (PCBox_IsActionMenuOpen(work->view->panels) == 0) {
            if (input >= 7 && input <= 11) {
                GridInputHandler_SetNextInput(work->view->gridInput, nextInput);
                PCBox_PlaceCursorOnGridInput(work, nextInput);
                return -1;
            }
        } else if (input == 6) {
            return 11;
        }
        return input;
    }
    if (PCBox_IsActionMenuOpen(work->view->panels) == 1) {
        if (input == 6 || input == -2) {
            return 11;
        }
    }
    return input;
}

void PCBox_DepositGridOnButton(PCBoxWork *work, u32 a1, u32 a2) {
    u32 v = a1;
    if (a2 != -1) {
        if (PCBox_IsActionMenuOpen(work->view->panels) == 0) {
            v = work->selectedSlot;
            if (v != 0xFF && v >= 30) {
                v -= 30;
            } else {
                v = 0;
            }
        } else if (v != 8 && v != 9 && v != 10) {
            v = 7;
        }
    }
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)v);
    PCBox_PlaceCursorOnGridInput(work, v);
}

void PCBox_DepositBoxGridOnButton(PCBoxWork *work, u32 a1) {
    u8 xy[2];
    if (a1 - 6 <= 1) {
        a1 = work->trayBox % 6;
        work->view->rowCursorPos = a1;
    }
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)a1);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(work->view->gridInput, a1), &xy[1], &xy[0]);
    ManagedSprite_SetPositionXY(work->view->sprites[9], xy[1], xy[0]);
    PCBox_SetSpriteVisible(work->view, 9, 1);
    if ((s32)a1 >= 0 && (s32)a1 <= 5) {
        PCBox_SetSpriteAnim(work->view, 9, 14);
    } else {
        PCBox_SetSpriteAnim(work->view, 9, 8);
    }
}

void PCBox_DepositBoxGridOnKeyMove(PCBoxWork *work, u32 a1, u32 a2) {
    if ((s32)a1 >= 0 && (s32)a1 <= 5) {
        PCBox_SetSpriteAnim(work->view, 9, 14);
    } else {
        PCBox_SetSpriteAnim(work->view, 9, 8);
    }
    if (a2 == 8 && a1 == 0) {
        a1 = work->view->rowCursorPos;
        GridInputHandler_SetNextLastReturnInputs(work->view->gridInput, (u8)a1, 8, 8);
    }
    if ((s32)a1 >= 0 && (s32)a1 <= 5) {
        work->view->rowCursorPos = a1;
    }
    if (a1 == 0 && a2 == 5) {
        if (work->trayBox + 1 >= 18) {
            work->trayBox = 0;
        } else {
            work->trayBox += 1;
        }
        PCBox_LoadTrayPageThumbnails(work);
        PCBox_UpdateTrayCursor(work);
        PCBox_UpdateCurrentBoxMarker(work);
        PCBox_DrawTrayBoxNameAndCount(work);
        PCBox_SetSpriteAnim(work->view, 5, 4);
        PCBox_StartCursorSlide(work->view, a1, a2);
        PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
        return;
    }
    if (a1 == 5 && a2 == 0) {
        if (work->trayBox - 1 < 0) {
            work->trayBox = 17;
        } else {
            work->trayBox -= 1;
        }
        PCBox_LoadTrayPageThumbnails(work);
        PCBox_UpdateTrayCursor(work);
        PCBox_UpdateCurrentBoxMarker(work);
        PCBox_DrawTrayBoxNameAndCount(work);
        PCBox_SetSpriteAnim(work->view, 4, 2);
        PCBox_StartCursorSlide(work->view, a1, a2);
        PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
        return;
    }
    if ((s32)a1 >= 0 && (s32)a1 <= 5 && a2 != 8) {
        work->trayBox = a1 + (work->trayBox / 6) * 6;
        PCBox_UpdateTrayCursor(work);
        PCBox_DrawTrayBoxNameAndCount(work);
    }
    PCBox_StartCursorSlide(work->view, a1, a2);
    PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
}

void PCBox_DepositBoxGridOnTouch(PCBoxWork *work, u32 a1) {
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)a1);
    if (a1 != (6) && a1 != (7)) {
        PCBox_PlaceCursorOnGridInput(work, a1);
    }
}

u32 PCBox_GetWithdrawMenuInput(PCBoxWork *work) {
    u32 nextInput = GridInputHandler_GetNextInput(work->view->gridInput);
    u32 input = GridInputHandler_HandleInput_AllowHold(work->view->gridInput);
    if (input < 39) {
        if (PCBox_IsActionMenuOpen(work->view->panels) == 0) {
            if (input >= 34 && input <= 38) {
                GridInputHandler_SetNextInput(work->view->gridInput, nextInput);
                PCBox_PlaceCursorOnGridInput(work, nextInput);
                return -1;
            }
        } else if (input == 33) {
            return 38;
        }
        return input;
    }
    if (PCBox_IsActionMenuOpen(work->view->panels) == 1) {
        if (input == 33 || input == -2) {
            return 38;
        }
    }
    return input;
}

void PCBox_WithdrawGridOnButton(PCBoxWork *work, u32 a1, u32 a2) {
    u32 v = a1;
    if (a2 != -1) {
        if (PCBox_IsActionMenuOpen(work->view->panels) == 0) {
            if (v - 31 <= 1) {
                v = 30;
            } else if (v != 30 && v != 33) {
                v = work->selectedSlot;
                if (v == 0xFF || v >= 30) {
                    v = 0;
                }
            }
        } else if (v != 35 && v != 36 && v != 37) {
            v = 34;
        }
    }
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)v);
    PCBox_PlaceCursorOnGridInput(work, v);
}

void PCBox_WithdrawGridOnTouch(PCBoxWork *work, u32 a1) {
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)a1);
    if (a1 != (31) && a1 != (32)) {
        PCBox_PlaceCursorOnGridInput(work, a1);
    }
}

u32 PCBox_GetMoveModeMenuInput(PCBoxWork *work) {
    u32 nextInput = GridInputHandler_GetNextInput(work->view->gridInput);
    u32 input = GridInputHandler_HandleInput_AllowHold(work->view->gridInput);
    if (PCBox_IsActionMenuOpen(work->view->panels) == 0) {
        if (input >= 36 && input <= 40) {
            GridInputHandler_SetNextInput(work->view->gridInput, nextInput);
            PCBox_PlaceCursorOnGridInput(work, nextInput);
            return -1;
        }
    } else {
        if (input - 33 <= 1) {
            GridInputHandler_SetNextInput(work->view->gridInput, nextInput);
            PCBox_PlaceCursorOnGridInput(work, nextInput);
            return -1;
        }
        if (input == 35 || input == -2) {
            return 41;
        }
    }
    return input;
}

void PCBox_MoveModeGridOnButton(PCBoxWork *work, u32 a1, u32 a2) {
    u32 v = a1;
    if (a2 != -1) {
        if (PCBox_IsActionMenuOpen(work->view->panels) == 0) {
            switch (v) {
            case 31:
            case 32:
                v = 30;
                break;
            case 30:
            case 33:
            case 34:
            case 35:
                break;
            default:
                v = work->selectedSlot;
                if (v >= 30) {
                    v = 0;
                }
                break;
            }
        } else if (v != 36 || v != 37 || v != 38 || v != 39 || v != 40) {
            v = 36;
        }
    }
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)v);
    PCBox_PlaceCursorOnGridInput(work, v);
}

void PCBox_MoveModeGridOnKeyMove(PCBoxWork *work, u32 a1, u32 a2) {
    PCBox_StartCursorSlide(work->view, a1, a2);
    PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
}

void PCBox_MoveModeGridOnTouch(PCBoxWork *work, u32 a1) {
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)a1);
    PCBox_PlaceCursorOnGridInput(work, a1);
}

u32 PCBox_GetBoxTrayInput(PCBoxWork *work) {
    u32 input;
    if (PCBox_IsChangeBoxButtonShown(work->view->panels) == 0) {
        GridInputHandler_ClearEnabledFlag(work->view->gridInput, 45);
    }
    input = GridInputHandler_HandleInput_AllowHold(work->view->gridInput);
    GridInputHandler_SetAllEnabled(work->view->gridInput);
    return input;
}

void PCBox_TrayGridOnButton(PCBoxWork *work, u32 a1) {
    u8 xy[2];
    if (a1 - 43 <= 1) {
        a1 = work->trayBox % 6 + 37;
        work->view->rowCursorPos = a1;
    }
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)a1);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(work->view->gridInput, a1), &xy[1], &xy[0]);
    ManagedSprite_SetPositionXY(work->view->sprites[9], xy[1], xy[0]);
    PCBox_SetSpriteVisible(work->view, 9, 1);
    if (work->view->cursorHolding == 0) {
        if ((s32)a1 >= 37 && (s32)a1 <= 42) {
            PCBox_SetSpriteAnim(work->view, 9, 14);
        } else {
            PCBox_SetSpriteAnim(work->view, 9, 8);
        }
    }
}

void PCBox_TrayGridOnKeyMove(PCBoxWork *work, u32 a1, u32 a2) {
    if (work->view->cursorHolding == 0) {
        if ((s32)a1 >= 37 && (s32)a1 <= 42) {
            PCBox_SetSpriteAnim(work->view, 9, 14);
        } else {
            PCBox_SetSpriteAnim(work->view, 9, 8);
        }
    }
    if (a1 == 37
        && (((s32)a2 >= 0 && (s32)a2 <= 5) || ((s32)a2 >= 24 && (s32)a2 <= 29) || ((s32)a2 >= 30 && (s32)a2 <= 31) || a2 == 36)) {
        a1 = work->view->rowCursorPos;
        GridInputHandler_SetNextLastReturnInputs(work->view->gridInput, (u8)a1, (u8)a2, GridInputHandler_GetReturnInput(work->view->gridInput));
    }
    if ((s32)a1 >= 37 && (s32)a1 <= 42) {
        work->view->rowCursorPos = a1;
    }
    if (a1 == 37 && a2 == 42) {
        if (work->trayBox + 1 >= 18) {
            work->trayBox = 0;
        } else {
            work->trayBox += 1;
        }
        PCBox_LoadTrayPageThumbnails(work);
        PCBox_UpdateTrayCursor(work);
        PCBox_UpdateCurrentBoxMarker(work);
        PCBox_DrawTrayBoxNameAndCount(work);
        PCBox_SetSpriteAnim(work->view, 5, 4);
        PCBox_StartCursorSlide(work->view, a1, a2);
        PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
        return;
    }
    if (a1 == 42 && a2 == 37) {
        if (work->trayBox - 1 < 0) {
            work->trayBox = 17;
        } else {
            work->trayBox -= 1;
        }
        PCBox_LoadTrayPageThumbnails(work);
        PCBox_UpdateTrayCursor(work);
        PCBox_UpdateCurrentBoxMarker(work);
        PCBox_DrawTrayBoxNameAndCount(work);
        PCBox_SetSpriteAnim(work->view, 4, 2);
        PCBox_StartCursorSlide(work->view, a1, a2);
        PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
        return;
    }
    if ((s32)a1 >= 37 && (s32)a1 <= 42 && (s32)a2 >= 37 && (s32)a2 <= 42) {
        work->trayBox = a1 + (work->trayBox / 6) * 6 - 37;
        PCBox_UpdateTrayCursor(work);
        PCBox_DrawTrayBoxNameAndCount(work);
    }
    PCBox_StartCursorSlide(work->view, a1, a2);
    PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
}

void PCBox_TrayGridOnTouch(PCBoxWork *work, u32 a1) {
    if ((s32)a1 >= 37 && (s32)a1 <= 42) {
        work->view->rowCursorPos = a1;
    }
    if (a1 != 43 && a1 != 44) {
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)a1);
        PCBox_PlaceCursorOnGridInput(work, a1);
    }
}

u32 PCBox_GetTrayHoldingInput(PCBoxWork *work) {
    u32 input;
    if (PCBox_IsChangeBoxButtonShown(work->view->panels) == 0) {
        GridInputHandler_ClearEnabledFlag(work->view->gridInput, 45);
    }
    GridInputHandler_ClearEnabledFlag(work->view->gridInput, 43);
    GridInputHandler_ClearEnabledFlag(work->view->gridInput, 44);
    input = GridInputHandler_HandleInput_AllowHold(work->view->gridInput);
    GridInputHandler_SetAllEnabled(work->view->gridInput);
    return input;
}

u32 PCBox_GetMoveModePartyMenuInput(PCBoxWork *work) {
    u32 nextInput = GridInputHandler_GetNextInput(work->view->gridInput);
    u32 input = GridInputHandler_HandleInput_AllowHold(work->view->gridInput);
    if (PCBox_IsActionMenuOpen(work->view->panels) == 0) {
        if (input >= 8 && input <= 12) {
            GridInputHandler_SetNextInput(work->view->gridInput, nextInput);
            PCBox_PlaceCursorOnGridInput(work, nextInput);
            return -1;
        }
    } else if (input == 7 || input == -2) {
        return 13;
    }
    return input;
}

void PCBox_MoveModePartyGridOnButton(PCBoxWork *work, u32 a1, u32 a2) {
    u32 v = a1;
    if (a2 != -1) {
        if (PCBox_IsActionMenuOpen(work->view->panels) == 0) {
            if ((s32)v >= 8) {
                v = work->selectedSlot;
                if (v >= 30) {
                    v -= 30;
                } else {
                    v = 0;
                }
            }
        } else if (v != 8 || v != 9 || v != 10 || v != 11 || v != 12) {
            v = 8;
        }
    }
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)v);
    PCBox_PlaceCursorOnGridInput(work, v);
}

void PCBox_MoveModePartyGridOnKeyMove(PCBoxWork *work, u32 a1, u32 a2) {
    if (PCBox_IsActionMenuOpen(work->view->panels) == 1) {
        if ((s32)a1 <= 6 && (s32)a2 >= 7) {
            PCBox_ClearMonInfoAndSelection(work);
            PCBox_StartCursorSlide(work->view, a1, a2);
            PCBox_SetPendingUpdateFunc(work->view, PCBox_WaitDeselectThenButtons);
            return;
        }
    } else if (a1 == 12 && a2 == 7) {
        GridInputHandler_SetNextInput(work->view->gridInput, 7);
        a1 = 7;
    }
    PCBox_StartCursorSlide(work->view, a1, a2);
    PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
}

u32 PCBox_GetItemModeMenuInput(PCBoxWork *work) {
    u32 nextInput = GridInputHandler_GetNextInput(work->view->gridInput);
    u32 input = GridInputHandler_HandleInput_AllowHold(work->view->gridInput);
    if (PCBox_IsActionMenuOpen(work->view->panels) == 0) {
        if (input == 36) {
            GridInputHandler_SetNextInput(work->view->gridInput, nextInput);
            PCBox_PlaceCursorOnGridInput(work, nextInput);
            return -1;
        }
    } else {
        if (input - 33 <= 1) {
            GridInputHandler_SetNextInput(work->view->gridInput, nextInput);
            PCBox_PlaceCursorOnGridInput(work, nextInput);
            return -1;
        }
        if (input == 35 || input == -2) {
            return 37;
        }
    }
    return input;
}

void PCBox_ItemModeGridOnButton(PCBoxWork *work, u32 a1, u32 a2) {
    u32 v = a1;
    if (a2 != -1) {
        if (PCBox_IsActionMenuOpen(work->view->panels) == 0) {
            switch (v) {
            case 31:
            case 32:
                v = 30;
                break;
            case 30:
            case 33:
            case 34:
            case 35:
                break;
            default:
                v = work->selectedSlot;
                if (v >= 30) {
                    v = 0;
                }
                break;
            }
        } else if (v != 36) {
            v = 36;
        }
    }
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)v);
    PCBox_PlaceCursorOnGridInput(work, v);
}

void PCBox_ItemModeGridOnKeyMove(PCBoxWork *work, u32 a1, u32 a2) {
    PCBox_StartCursorSlide(work->view, a1, a2);
    PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
}

void PCBox_ItemModeGridOnTouch(PCBoxWork *work, u32 a1) {
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)a1);
    PCBox_PlaceCursorOnGridInput(work, a1);
}

u32 PCBox_GetItemModePartyMenuInput(PCBoxWork *work) {
    u32 nextInput = GridInputHandler_GetNextInput(work->view->gridInput);
    u32 input = GridInputHandler_HandleInput_AllowHold(work->view->gridInput);
    if (PCBox_IsActionMenuOpen(work->view->panels) == 0) {
        if (input == 8) {
            GridInputHandler_SetNextInput(work->view->gridInput, nextInput);
            PCBox_PlaceCursorOnGridInput(work, nextInput);
            return -1;
        }
    } else if (input == 7 || input == -2) {
        return 9;
    }
    return input;
}

void PCBox_ItemModePartyGridOnButton(PCBoxWork *work, u32 a1, u32 a2) {
    u32 v = a1;
    if (a2 != -1) {
        if (PCBox_IsActionMenuOpen(work->view->panels) == 0) {
            if ((s32)v >= 8) {
                v = work->selectedSlot;
                if (v >= 30) {
                    v -= 30;
                } else {
                    v = 0;
                }
            }
        } else if (v != 8) {
            v = 8;
        }
    }
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)v);
    PCBox_PlaceCursorOnGridInput(work, v);
}

void PCBox_ItemModePartyGridOnKeyMove(PCBoxWork *work, u32 a1, u32 a2) {
    PCBox_StartCursorSlide(work->view, a1, a2);
    PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
}

void PCBox_BoxMenuGridOnButton(PCBoxWork *work, u32 a1) {
    u8 xy[2];
    if (a1 - 6 <= 1) {
        a1 = work->trayBox % 6;
        work->view->rowCursorPos = a1;
    }
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)a1);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(work->view->gridInput, a1), &xy[1], &xy[0]);
    ManagedSprite_SetPositionXY(work->view->sprites[9], xy[1], xy[0]);
    if ((s32)a1 >= 0 && (s32)a1 <= 5) {
        PCBox_SetSpriteAnim(work->view, 9, 14);
    } else {
        PCBox_SetSpriteAnim(work->view, 9, 8);
    }
}

void PCBox_BoxMenuGridOnKeyMove(PCBoxWork *work, u32 a1, u32 a2) {
    if ((s32)a1 >= 0 && (s32)a1 <= 5) {
        PCBox_SetSpriteAnim(work->view, 9, 14);
    } else {
        PCBox_SetSpriteAnim(work->view, 9, 8);
    }
    if (a2 == 8 && a1 == 0) {
        a1 = work->view->rowCursorPos;
        GridInputHandler_SetNextLastReturnInputs(work->view->gridInput, (u8)a1, 8, 8);
    }
    if ((s32)a1 >= 0 && (s32)a1 <= 5) {
        work->view->rowCursorPos = a1;
    }
    if (a1 == 0 && a2 == 5) {
        if (work->trayBox + 1 >= 18) {
            work->trayBox = 0;
        } else {
            work->trayBox += 1;
        }
        PCBox_LoadTrayPageThumbnails(work);
        PCBox_UpdateTrayCursor(work);
        PCBox_UpdateCurrentBoxMarker(work);
        PCBox_DrawTrayBoxNameAndCount(work);
        PCBox_SetSpriteAnim(work->view, 5, 4);
        PCBox_StartCursorSlide(work->view, a1, a2);
        PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
        return;
    }
    if (a1 == 5 && a2 == 0) {
        if (work->trayBox - 1 < 0) {
            work->trayBox = 17;
        } else {
            work->trayBox -= 1;
        }
        PCBox_LoadTrayPageThumbnails(work);
        PCBox_UpdateTrayCursor(work);
        PCBox_UpdateCurrentBoxMarker(work);
        PCBox_DrawTrayBoxNameAndCount(work);
        PCBox_SetSpriteAnim(work->view, 4, 2);
        PCBox_StartCursorSlide(work->view, a1, a2);
        PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
        return;
    }
    if ((s32)a1 >= 0 && (s32)a1 <= 5 && a2 != 8) {
        work->trayBox = a1 + (work->trayBox / 6) * 6;
        PCBox_UpdateTrayCursor(work);
        PCBox_DrawTrayBoxNameAndCount(work);
    }
    PCBox_StartCursorSlide(work->view, a1, a2);
    PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
}

void PCBox_BoxMenuGridOnTouch(PCBoxWork *work, u32 a1) {
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)a1);
    if (a1 != (6) && a1 != (7)) {
        PCBox_PlaceCursorOnGridInput(work, a1);
    }
}

void PCBox_WallpaperGridOnButton(PCBoxWork *work, u32 a1) {
    u8 xy[2];
    if (a1 - 4 <= 1) {
        a1 = work->view->wallpaperSelection % 4;
        work->view->rowCursorPos = a1;
    }
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)a1);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(work->view->gridInput, a1), &xy[1], &xy[0]);
    ManagedSprite_SetPositionXY(work->view->sprites[9], xy[1], xy[0]);
    if ((s32)a1 >= 0 && (s32)a1 <= 3) {
        PCBox_SetSpriteAnim(work->view, 9, 14);
    } else {
        PCBox_SetSpriteAnim(work->view, 9, 8);
    }
}

void PCBox_WallpaperGridOnKeyMove(PCBoxWork *work, u32 a1, u32 a2) {
    if ((s32)a1 >= 0 && (s32)a1 <= 3) {
        PCBox_SetSpriteAnim(work->view, 9, 14);
    } else {
        PCBox_SetSpriteAnim(work->view, 9, 8);
    }
    if (a2 == 6 && a1 == 0) {
        a1 = work->view->rowCursorPos;
        GridInputHandler_SetNextLastReturnInputs(work->view->gridInput, (u8)a1, 6, 6);
    }
    if ((s32)a1 >= 0 && (s32)a1 <= 3) {
        work->view->rowCursorPos = a1;
    }
    if (a1 == 0 && a2 == 3) {
        if (work->view->wallpaperSelection + 1 >= 24) {
            work->view->wallpaperSelection = 0;
        } else {
            work->view->wallpaperSelection = work->view->wallpaperSelection + 1;
        }
        PCBox_LoadWallpaperThumbnails(work);
        PCBox_UpdateCurrentWallpaperMarker(work);
        PCBox_UpdateWallpaperCursor(work);
        PCBox_DrawWallpaperName(work);
        PCBox_SetSpriteAnim(work->view, 5, 4);
        PCBox_StartCursorSlide(work->view, a1, a2);
        PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
        return;
    }
    if (a1 == 3 && a2 == 0) {
        if (work->view->wallpaperSelection - 1 < 0) {
            work->view->wallpaperSelection = 23;
        } else {
            work->view->wallpaperSelection = work->view->wallpaperSelection - 1;
        }
        PCBox_LoadWallpaperThumbnails(work);
        PCBox_UpdateCurrentWallpaperMarker(work);
        PCBox_UpdateWallpaperCursor(work);
        PCBox_DrawWallpaperName(work);
        PCBox_SetSpriteAnim(work->view, 4, 2);
        PCBox_StartCursorSlide(work->view, a1, a2);
        PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
        return;
    }
    if ((s32)a1 >= 0 && (s32)a1 <= 3 && a2 != 6) {
        work->view->wallpaperSelection = a1 + (work->view->wallpaperSelection / 4) * 4;
        PCBox_UpdateWallpaperCursor(work);
        PCBox_DrawWallpaperName(work);
    }
    PCBox_StartCursorSlide(work->view, a1, a2);
    PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
}

void PCBox_WallpaperGridOnTouch(PCBoxWork *work, u32 a1) {
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)a1);
    if (a1 != (4) && a1 != (5)) {
        PCBox_PlaceCursorOnGridInput(work, a1);
    }
}

u32 PCBox_GetItemTrayHoldingInput(PCBoxWork *work) {
    u32 input;
    GridInputHandler_ClearEnabledFlag(work->view->gridInput, 43);
    GridInputHandler_ClearEnabledFlag(work->view->gridInput, 44);
    GridInputHandler_ClearEnabledFlag(work->view->gridInput, 45);
    GridInputHandler_ClearEnabledFlag(work->view->gridInput, 37);
    GridInputHandler_ClearEnabledFlag(work->view->gridInput, 38);
    GridInputHandler_ClearEnabledFlag(work->view->gridInput, 39);
    GridInputHandler_ClearEnabledFlag(work->view->gridInput, 40);
    GridInputHandler_ClearEnabledFlag(work->view->gridInput, 41);
    GridInputHandler_ClearEnabledFlag(work->view->gridInput, 42);
    input = GridInputHandler_HandleInput_AllowHold(work->view->gridInput);
    GridInputHandler_SetAllEnabled(work->view->gridInput);
    return input;
}

void PCBox_StartCursorSlide(PCBoxView *view, u32 from, u32 to) {
    u8 pos[4];
    PCBoxCursorSlide *slide;
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(view->gridInput, from), &pos[3], &pos[2]);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(view->gridInput, to), &pos[1], &pos[0]);
    slide = Heap_Alloc(HEAP_ID_10, sizeof(PCBoxCursorSlide));
    slide->frames = 4;
    slide->x = pos[3];
    slide->y = pos[2];
    if (pos[3] >= pos[1]) {
        slide->dx = pos[3] - pos[1];
        slide->xNegative = 0;
    } else {
        slide->dx = pos[1] - pos[3];
        slide->xNegative = 1;
    }
    if (pos[2] >= pos[0]) {
        slide->dy = pos[2] - pos[0];
        slide->yNegative = 0;
    } else {
        slide->dy = pos[0] - pos[2];
        slide->yNegative = 1;
    }
    slide->dx = ((slide->dx << 8) / slide->frames) >> 8;
    slide->dy = ((slide->dy << 8) / slide->frames) >> 8;
    view->temp = slide;
}

u32 PCBox_IsSummaryShortcutPressed(PCBoxWork *work) {
    if (PCBox_IsSummaryButtonShown(work->view->panels) == 0) {
        return 0;
    }
    if (gSystem.newKeys & PAD_BUTTON_Y) {
        return 1;
    }
    if (TouchscreenHitbox_TouchNewIsIn(ov14_021F86C8) == 1) {
        return 1;
    }
    return 0;
}
