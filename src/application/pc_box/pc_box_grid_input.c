#include "global.h"

#include "application/pc_box/pc_box_internal.h"

#include "dpad_menu_box.h"
#include "grid_input_handler.h"
#include "heap.h"
#include "sprite_system.h"
#include "system.h"
#include "touchscreen.h"

extern const TouchscreenHitbox sPCBoxSummaryShortcutHitbox[];

void PCBox_GridOnKeyMove(PCBoxWork *work, u32 newTarget, u32 prevTarget) {
    PCBox_StartCursorSlide(work->view, newTarget, prevTarget);
    PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
}

void PCBox_GridOnTouch(PCBoxWork *work, u32 newTarget) {
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)newTarget);
    PCBox_PlaceCursorOnGridInput(work, newTarget);
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

void PCBox_DepositGridOnButton(PCBoxWork *work, u32 newTarget, u32 prevTarget) {
    u32 v = newTarget;
    if (prevTarget != -1) {
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

void PCBox_DepositBoxGridOnButton(PCBoxWork *work, u32 newTarget) {
    u8 xy[2];
    if (newTarget - 6 <= 1) {
        newTarget = work->trayBox % 6;
        work->view->rowCursorPos = newTarget;
    }
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)newTarget);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(work->view->gridInput, newTarget), &xy[1], &xy[0]);
    ManagedSprite_SetPositionXY(work->view->sprites[9], xy[1], xy[0]);
    PCBox_SetSpriteVisible(work->view, 9, 1);
    if ((s32)newTarget >= 0 && (s32)newTarget <= 5) {
        PCBox_SetSpriteAnim(work->view, 9, 14);
    } else {
        PCBox_SetSpriteAnim(work->view, 9, 8);
    }
}

void PCBox_DepositBoxGridOnKeyMove(PCBoxWork *work, u32 newTarget, u32 prevTarget) {
    if ((s32)newTarget >= 0 && (s32)newTarget <= 5) {
        PCBox_SetSpriteAnim(work->view, 9, 14);
    } else {
        PCBox_SetSpriteAnim(work->view, 9, 8);
    }
    if (prevTarget == 8 && newTarget == 0) {
        newTarget = work->view->rowCursorPos;
        GridInputHandler_SetNextLastReturnInputs(work->view->gridInput, (u8)newTarget, 8, 8);
    }
    if ((s32)newTarget >= 0 && (s32)newTarget <= 5) {
        work->view->rowCursorPos = newTarget;
    }
    if (newTarget == 0 && prevTarget == 5) {
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
        PCBox_StartCursorSlide(work->view, newTarget, prevTarget);
        PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
        return;
    }
    if (newTarget == 5 && prevTarget == 0) {
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
        PCBox_StartCursorSlide(work->view, newTarget, prevTarget);
        PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
        return;
    }
    if ((s32)newTarget >= 0 && (s32)newTarget <= 5 && prevTarget != 8) {
        work->trayBox = newTarget + (work->trayBox / 6) * 6;
        PCBox_UpdateTrayCursor(work);
        PCBox_DrawTrayBoxNameAndCount(work);
    }
    PCBox_StartCursorSlide(work->view, newTarget, prevTarget);
    PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
}

void PCBox_DepositBoxGridOnTouch(PCBoxWork *work, u32 newTarget) {
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)newTarget);
    if (newTarget != (6) && newTarget != (7)) {
        PCBox_PlaceCursorOnGridInput(work, newTarget);
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

void PCBox_WithdrawGridOnButton(PCBoxWork *work, u32 newTarget, u32 prevTarget) {
    u32 v = newTarget;
    if (prevTarget != -1) {
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

void PCBox_WithdrawGridOnTouch(PCBoxWork *work, u32 newTarget) {
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)newTarget);
    if (newTarget != (31) && newTarget != (32)) {
        PCBox_PlaceCursorOnGridInput(work, newTarget);
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

void PCBox_MoveModeGridOnButton(PCBoxWork *work, u32 newTarget, u32 prevTarget) {
    u32 v = newTarget;
    if (prevTarget != -1) {
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

void PCBox_MoveModeGridOnKeyMove(PCBoxWork *work, u32 newTarget, u32 prevTarget) {
    PCBox_StartCursorSlide(work->view, newTarget, prevTarget);
    PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
}

void PCBox_MoveModeGridOnTouch(PCBoxWork *work, u32 newTarget) {
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)newTarget);
    PCBox_PlaceCursorOnGridInput(work, newTarget);
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

void PCBox_TrayGridOnButton(PCBoxWork *work, u32 newTarget) {
    u8 xy[2];
    if (newTarget - 43 <= 1) {
        newTarget = work->trayBox % 6 + 37;
        work->view->rowCursorPos = newTarget;
    }
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)newTarget);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(work->view->gridInput, newTarget), &xy[1], &xy[0]);
    ManagedSprite_SetPositionXY(work->view->sprites[9], xy[1], xy[0]);
    PCBox_SetSpriteVisible(work->view, 9, 1);
    if (work->view->cursorHolding == 0) {
        if ((s32)newTarget >= 37 && (s32)newTarget <= 42) {
            PCBox_SetSpriteAnim(work->view, 9, 14);
        } else {
            PCBox_SetSpriteAnim(work->view, 9, 8);
        }
    }
}

void PCBox_TrayGridOnKeyMove(PCBoxWork *work, u32 newTarget, u32 prevTarget) {
    if (work->view->cursorHolding == 0) {
        if ((s32)newTarget >= 37 && (s32)newTarget <= 42) {
            PCBox_SetSpriteAnim(work->view, 9, 14);
        } else {
            PCBox_SetSpriteAnim(work->view, 9, 8);
        }
    }
    if (newTarget == 37
        && (((s32)prevTarget >= 0 && (s32)prevTarget <= 5) || ((s32)prevTarget >= 24 && (s32)prevTarget <= 29) || ((s32)prevTarget >= 30 && (s32)prevTarget <= 31) || prevTarget == 36)) {
        newTarget = work->view->rowCursorPos;
        GridInputHandler_SetNextLastReturnInputs(work->view->gridInput, (u8)newTarget, (u8)prevTarget, GridInputHandler_GetReturnInput(work->view->gridInput));
    }
    if ((s32)newTarget >= 37 && (s32)newTarget <= 42) {
        work->view->rowCursorPos = newTarget;
    }
    if (newTarget == 37 && prevTarget == 42) {
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
        PCBox_StartCursorSlide(work->view, newTarget, prevTarget);
        PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
        return;
    }
    if (newTarget == 42 && prevTarget == 37) {
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
        PCBox_StartCursorSlide(work->view, newTarget, prevTarget);
        PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
        return;
    }
    if ((s32)newTarget >= 37 && (s32)newTarget <= 42 && (s32)prevTarget >= 37 && (s32)prevTarget <= 42) {
        work->trayBox = newTarget + (work->trayBox / 6) * 6 - 37;
        PCBox_UpdateTrayCursor(work);
        PCBox_DrawTrayBoxNameAndCount(work);
    }
    PCBox_StartCursorSlide(work->view, newTarget, prevTarget);
    PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
}

void PCBox_TrayGridOnTouch(PCBoxWork *work, u32 newTarget) {
    if ((s32)newTarget >= 37 && (s32)newTarget <= 42) {
        work->view->rowCursorPos = newTarget;
    }
    if (newTarget != 43 && newTarget != 44) {
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)newTarget);
        PCBox_PlaceCursorOnGridInput(work, newTarget);
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

void PCBox_MoveModePartyGridOnButton(PCBoxWork *work, u32 newTarget, u32 prevTarget) {
    u32 v = newTarget;
    if (prevTarget != -1) {
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

void PCBox_MoveModePartyGridOnKeyMove(PCBoxWork *work, u32 newTarget, u32 prevTarget) {
    if (PCBox_IsActionMenuOpen(work->view->panels) == 1) {
        if ((s32)newTarget <= 6 && (s32)prevTarget >= 7) {
            PCBox_ClearMonInfoAndSelection(work);
            PCBox_StartCursorSlide(work->view, newTarget, prevTarget);
            PCBox_SetPendingUpdateFunc(work->view, PCBox_WaitDeselectThenButtons);
            return;
        }
    } else if (newTarget == 12 && prevTarget == 7) {
        GridInputHandler_SetNextInput(work->view->gridInput, 7);
        newTarget = 7;
    }
    PCBox_StartCursorSlide(work->view, newTarget, prevTarget);
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

void PCBox_ItemModeGridOnButton(PCBoxWork *work, u32 newTarget, u32 prevTarget) {
    u32 v = newTarget;
    if (prevTarget != -1) {
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

void PCBox_ItemModeGridOnKeyMove(PCBoxWork *work, u32 newTarget, u32 prevTarget) {
    PCBox_StartCursorSlide(work->view, newTarget, prevTarget);
    PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
}

void PCBox_ItemModeGridOnTouch(PCBoxWork *work, u32 newTarget) {
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)newTarget);
    PCBox_PlaceCursorOnGridInput(work, newTarget);
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

void PCBox_ItemModePartyGridOnButton(PCBoxWork *work, u32 newTarget, u32 prevTarget) {
    u32 v = newTarget;
    if (prevTarget != -1) {
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

void PCBox_ItemModePartyGridOnKeyMove(PCBoxWork *work, u32 newTarget, u32 prevTarget) {
    PCBox_StartCursorSlide(work->view, newTarget, prevTarget);
    PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
}

void PCBox_BoxMenuGridOnButton(PCBoxWork *work, u32 newTarget) {
    u8 xy[2];
    if (newTarget - 6 <= 1) {
        newTarget = work->trayBox % 6;
        work->view->rowCursorPos = newTarget;
    }
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)newTarget);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(work->view->gridInput, newTarget), &xy[1], &xy[0]);
    ManagedSprite_SetPositionXY(work->view->sprites[9], xy[1], xy[0]);
    if ((s32)newTarget >= 0 && (s32)newTarget <= 5) {
        PCBox_SetSpriteAnim(work->view, 9, 14);
    } else {
        PCBox_SetSpriteAnim(work->view, 9, 8);
    }
}

void PCBox_BoxMenuGridOnKeyMove(PCBoxWork *work, u32 newTarget, u32 prevTarget) {
    if ((s32)newTarget >= 0 && (s32)newTarget <= 5) {
        PCBox_SetSpriteAnim(work->view, 9, 14);
    } else {
        PCBox_SetSpriteAnim(work->view, 9, 8);
    }
    if (prevTarget == 8 && newTarget == 0) {
        newTarget = work->view->rowCursorPos;
        GridInputHandler_SetNextLastReturnInputs(work->view->gridInput, (u8)newTarget, 8, 8);
    }
    if ((s32)newTarget >= 0 && (s32)newTarget <= 5) {
        work->view->rowCursorPos = newTarget;
    }
    if (newTarget == 0 && prevTarget == 5) {
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
        PCBox_StartCursorSlide(work->view, newTarget, prevTarget);
        PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
        return;
    }
    if (newTarget == 5 && prevTarget == 0) {
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
        PCBox_StartCursorSlide(work->view, newTarget, prevTarget);
        PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
        return;
    }
    if ((s32)newTarget >= 0 && (s32)newTarget <= 5 && prevTarget != 8) {
        work->trayBox = newTarget + (work->trayBox / 6) * 6;
        PCBox_UpdateTrayCursor(work);
        PCBox_DrawTrayBoxNameAndCount(work);
    }
    PCBox_StartCursorSlide(work->view, newTarget, prevTarget);
    PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
}

void PCBox_BoxMenuGridOnTouch(PCBoxWork *work, u32 newTarget) {
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)newTarget);
    if (newTarget != (6) && newTarget != (7)) {
        PCBox_PlaceCursorOnGridInput(work, newTarget);
    }
}

void PCBox_WallpaperGridOnButton(PCBoxWork *work, u32 newTarget) {
    u8 xy[2];
    if (newTarget - 4 <= 1) {
        newTarget = work->view->wallpaperSelection % 4;
        work->view->rowCursorPos = newTarget;
    }
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)newTarget);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(work->view->gridInput, newTarget), &xy[1], &xy[0]);
    ManagedSprite_SetPositionXY(work->view->sprites[9], xy[1], xy[0]);
    if ((s32)newTarget >= 0 && (s32)newTarget <= 3) {
        PCBox_SetSpriteAnim(work->view, 9, 14);
    } else {
        PCBox_SetSpriteAnim(work->view, 9, 8);
    }
}

void PCBox_WallpaperGridOnKeyMove(PCBoxWork *work, u32 newTarget, u32 prevTarget) {
    if ((s32)newTarget >= 0 && (s32)newTarget <= 3) {
        PCBox_SetSpriteAnim(work->view, 9, 14);
    } else {
        PCBox_SetSpriteAnim(work->view, 9, 8);
    }
    if (prevTarget == 6 && newTarget == 0) {
        newTarget = work->view->rowCursorPos;
        GridInputHandler_SetNextLastReturnInputs(work->view->gridInput, (u8)newTarget, 6, 6);
    }
    if ((s32)newTarget >= 0 && (s32)newTarget <= 3) {
        work->view->rowCursorPos = newTarget;
    }
    if (newTarget == 0 && prevTarget == 3) {
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
        PCBox_StartCursorSlide(work->view, newTarget, prevTarget);
        PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
        return;
    }
    if (newTarget == 3 && prevTarget == 0) {
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
        PCBox_StartCursorSlide(work->view, newTarget, prevTarget);
        PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
        return;
    }
    if ((s32)newTarget >= 0 && (s32)newTarget <= 3 && prevTarget != 6) {
        work->view->wallpaperSelection = newTarget + (work->view->wallpaperSelection / 4) * 4;
        PCBox_UpdateWallpaperCursor(work);
        PCBox_DrawWallpaperName(work);
    }
    PCBox_StartCursorSlide(work->view, newTarget, prevTarget);
    PCBox_SetPendingUpdateFunc(work->view, PCBox_UpdateCursorSlide);
}

void PCBox_WallpaperGridOnTouch(PCBoxWork *work, u32 newTarget) {
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)newTarget);
    if (newTarget != (4) && newTarget != (5)) {
        PCBox_PlaceCursorOnGridInput(work, newTarget);
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

void PCBox_StartCursorSlide(PCBoxView *view, u32 newTarget, u32 prevTarget) {
    u8 pos[4];
    PCBoxCursorSlide *slide;
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(view->gridInput, newTarget), &pos[3], &pos[2]);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(view->gridInput, prevTarget), &pos[1], &pos[0]);
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
    if (TouchscreenHitbox_TouchNewIsIn(sPCBoxSummaryShortcutHitbox) == TRUE) {
        return 1;
    }
    return 0;
}
