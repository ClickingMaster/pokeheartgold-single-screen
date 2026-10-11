#include "overlay_14_state_input_handlers_6b8c_private.h"

#define RESET_7AC4(name)                       \
    void name(void *p, u32 a1, u32 a2) {       \
        PCBox_StartCursorSlide(WORK(p), a1, a2);        \
        PCBox_SetPendingUpdateFunc(WORK(p), PCBox_UpdateCursorSlide); \
    }

#define SET_NOTIFY(name)                                \
    void name(void *p, u32 a1) {                        \
        GridInputHandler_SetNextInput(GRID(p), (u8)a1); \
        PCBox_PlaceCursorOnGridInput(p, a1);                           \
    }

#define SET_NOTIFY2(name, k1, k2)                       \
    void name(void *p, u32 a1) {                        \
        GridInputHandler_SetNextInput(GRID(p), (u8)a1); \
        if (a1 != k1 && a1 != k2) {                     \
            PCBox_PlaceCursorOnGridInput(p, a1);                       \
        }                                               \
    }

RESET_7AC4(ov14_021F6B8C)
SET_NOTIFY(ov14_021F6BA4)

u32 PCBox_GetDepositMenuInput(void *p) {
    u32 nextInput = GridInputHandler_GetNextInput(GRID(p));
    u32 input = GridInputHandler_HandleInput_AllowHold(GRID(p));
    if (input < 12) {
        if (PCBox_IsActionMenuOpen(UNK2F0(p)) == 0) {
            if (input >= 7 && input <= 11) {
                GridInputHandler_SetNextInput(GRID(p), nextInput);
                PCBox_PlaceCursorOnGridInput(p, nextInput);
                return -1;
            }
        } else if (input == 6) {
            return 11;
        }
        return input;
    }
    if (PCBox_IsActionMenuOpen(UNK2F0(p)) == 1) {
        if (input == 6 || input == -2) {
            return 11;
        }
    }
    return input;
}

void ov14_021F6C3C(void *p, u32 a1, u32 a2) {
    u32 v = a1;
    if (a2 != -1) {
        if (PCBox_IsActionMenuOpen(UNK2F0(p)) == 0) {
            v = U8_AT(p, 0x21);
            if (v != 0xFF && v >= 30) {
                v -= 30;
            } else {
                v = 0;
            }
        } else if (v != 8 && v != 9 && v != 10) {
            v = 7;
        }
    }
    GridInputHandler_SetNextInput(GRID(p), (u8)v);
    PCBox_PlaceCursorOnGridInput(p, v);
}

void ov14_021F6C94(void *p, u32 a1) {
    u8 xy[2];
    if (a1 - 6 <= 1) {
        a1 = U8_AT(p, 0x25) % 6;
        U32_AT(WORK(p), 0x43c) = a1;
    }
    GridInputHandler_SetNextInput(GRID(p), (u8)a1);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(GRID(p), a1), &xy[1], &xy[0]);
    ManagedSprite_SetPositionXY(PTR_AT(WORK(p), 0x320), xy[1], xy[0]);
    PCBox_SetSpriteVisible(WORK(p), 9, 1);
    if ((s32)a1 >= 0 && (s32)a1 <= 5) {
        PCBox_SetSpriteAnim(WORK(p), 9, 14);
    } else {
        PCBox_SetSpriteAnim(WORK(p), 9, 8);
    }
}

void ov14_021F6D14(void *p, u32 a1, u32 a2) {
    if ((s32)a1 >= 0 && (s32)a1 <= 5) {
        PCBox_SetSpriteAnim(WORK(p), 9, 14);
    } else {
        PCBox_SetSpriteAnim(WORK(p), 9, 8);
    }
    if (a2 == 8 && a1 == 0) {
        a1 = U32_AT(WORK(p), 0x43c);
        GridInputHandler_SetNextLastUnk0FInputs(GRID(p), (u8)a1, 8, 8);
    }
    if ((s32)a1 >= 0 && (s32)a1 <= 5) {
        U32_AT(WORK(p), 0x43c) = a1;
    }
    if (a1 == 0 && a2 == 5) {
        if (U8_AT(p, 0x25) + 1 >= 18) {
            U8_AT(p, 0x25) = 0;
        } else {
            U8_AT(p, 0x25) += 1;
        }
        PCBox_LoadTrayPageThumbnails(p);
        PCBox_UpdateTrayCursor(p);
        PCBox_UpdateCurrentBoxMarker(p);
        PCBox_DrawTrayBoxNameAndCount(p);
        PCBox_SetSpriteAnim(WORK(p), 5, 4);
        PCBox_StartCursorSlide(WORK(p), a1, a2);
        PCBox_SetPendingUpdateFunc(WORK(p), PCBox_UpdateCursorSlide);
        return;
    }
    if (a1 == 5 && a2 == 0) {
        if (U8_AT(p, 0x25) - 1 < 0) {
            U8_AT(p, 0x25) = 17;
        } else {
            U8_AT(p, 0x25) -= 1;
        }
        PCBox_LoadTrayPageThumbnails(p);
        PCBox_UpdateTrayCursor(p);
        PCBox_UpdateCurrentBoxMarker(p);
        PCBox_DrawTrayBoxNameAndCount(p);
        PCBox_SetSpriteAnim(WORK(p), 4, 2);
        PCBox_StartCursorSlide(WORK(p), a1, a2);
        PCBox_SetPendingUpdateFunc(WORK(p), PCBox_UpdateCursorSlide);
        return;
    }
    if ((s32)a1 >= 0 && (s32)a1 <= 5 && a2 != 8) {
        U8_AT(p, 0x25) = a1 + (U8_AT(p, 0x25) / 6) * 6;
        PCBox_UpdateTrayCursor(p);
        PCBox_DrawTrayBoxNameAndCount(p);
    }
    PCBox_StartCursorSlide(WORK(p), a1, a2);
    PCBox_SetPendingUpdateFunc(WORK(p), PCBox_UpdateCursorSlide);
}

SET_NOTIFY2(ov14_021F6E68, 6, 7)

u32 PCBox_GetWithdrawMenuInput(void *p) {
    u32 nextInput = GridInputHandler_GetNextInput(GRID(p));
    u32 input = GridInputHandler_HandleInput_AllowHold(GRID(p));
    if (input < 39) {
        if (PCBox_IsActionMenuOpen(UNK2F0(p)) == 0) {
            if (input >= 34 && input <= 38) {
                GridInputHandler_SetNextInput(GRID(p), nextInput);
                PCBox_PlaceCursorOnGridInput(p, nextInput);
                return -1;
            }
        } else if (input == 33) {
            return 38;
        }
        return input;
    }
    if (PCBox_IsActionMenuOpen(UNK2F0(p)) == 1) {
        if (input == 33 || input == -2) {
            return 38;
        }
    }
    return input;
}

void ov14_021F6F08(void *p, u32 a1, u32 a2) {
    u32 v = a1;
    if (a2 != -1) {
        if (PCBox_IsActionMenuOpen(UNK2F0(p)) == 0) {
            if (v - 31 <= 1) {
                v = 30;
            } else if (v != 30 && v != 33) {
                v = U8_AT(p, 0x21);
                if (v == 0xFF || v >= 30) {
                    v = 0;
                }
            }
        } else if (v != 35 && v != 36 && v != 37) {
            v = 34;
        }
    }
    GridInputHandler_SetNextInput(GRID(p), (u8)v);
    PCBox_PlaceCursorOnGridInput(p, v);
}

SET_NOTIFY2(ov14_021F6F70, 31, 32)

u32 PCBox_GetMoveModeMenuInput(void *p) {
    u32 nextInput = GridInputHandler_GetNextInput(GRID(p));
    u32 input = GridInputHandler_HandleInput_AllowHold(GRID(p));
    if (PCBox_IsActionMenuOpen(UNK2F0(p)) == 0) {
        if (input >= 36 && input <= 40) {
            GridInputHandler_SetNextInput(GRID(p), nextInput);
            PCBox_PlaceCursorOnGridInput(p, nextInput);
            return -1;
        }
    } else {
        if (input - 33 <= 1) {
            GridInputHandler_SetNextInput(GRID(p), nextInput);
            PCBox_PlaceCursorOnGridInput(p, nextInput);
            return -1;
        }
        if (input == 35 || input == -2) {
            return 41;
        }
    }
    return input;
}

void ov14_021F7010(void *p, u32 a1, u32 a2) {
    u32 v = a1;
    if (a2 != -1) {
        if (PCBox_IsActionMenuOpen(UNK2F0(p)) == 0) {
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
                v = U8_AT(p, 0x21);
                if (v >= 30) {
                    v = 0;
                }
                break;
            }
        } else if (v != 36 || v != 37 || v != 38 || v != 39 || v != 40) {
            v = 36;
        }
    }
    GridInputHandler_SetNextInput(GRID(p), (u8)v);
    PCBox_PlaceCursorOnGridInput(p, v);
}

RESET_7AC4(ov14_021F708C)
SET_NOTIFY(ov14_021F70A4)

u32 PCBox_GetBoxTrayInput(void *p) {
    u32 input;
    if (PCBox_IsChangeBoxButtonShown(UNK2F0(p)) == 0) {
        GridInputHandler_ClearEnabledFlag(GRID(p), 45);
    }
    input = GridInputHandler_HandleInput_AllowHold(GRID(p));
    GridInputHandler_SetAllEnabled(GRID(p));
    return input;
}

void ov14_021F70F4(void *p, u32 a1) {
    u8 xy[2];
    if (a1 - 43 <= 1) {
        a1 = U8_AT(p, 0x25) % 6 + 37;
        U32_AT(WORK(p), 0x43c) = a1;
    }
    GridInputHandler_SetNextInput(GRID(p), (u8)a1);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(GRID(p), a1), &xy[1], &xy[0]);
    ManagedSprite_SetPositionXY(PTR_AT(WORK(p), 0x320), xy[1], xy[0]);
    PCBox_SetSpriteVisible(WORK(p), 9, 1);
    if (U8_AT(WORK(p), 0x44b) == 0) {
        if ((s32)a1 >= 37 && (s32)a1 <= 42) {
            PCBox_SetSpriteAnim(WORK(p), 9, 14);
        } else {
            PCBox_SetSpriteAnim(WORK(p), 9, 8);
        }
    }
}

void ov14_021F7184(void *p, u32 a1, u32 a2) {
    if (U8_AT(WORK(p), 0x44b) == 0) {
        if ((s32)a1 >= 37 && (s32)a1 <= 42) {
            PCBox_SetSpriteAnim(WORK(p), 9, 14);
        } else {
            PCBox_SetSpriteAnim(WORK(p), 9, 8);
        }
    }
    if (a1 == 37
        && (((s32)a2 >= 0 && (s32)a2 <= 5) || ((s32)a2 >= 24 && (s32)a2 <= 29) || ((s32)a2 >= 30 && (s32)a2 <= 31) || a2 == 36)) {
        a1 = U32_AT(WORK(p), 0x43c);
        GridInputHandler_SetNextLastUnk0FInputs(GRID(p), (u8)a1, (u8)a2, GridInputHandler_GetUnk0F(GRID(p)));
    }
    if ((s32)a1 >= 37 && (s32)a1 <= 42) {
        U32_AT(WORK(p), 0x43c) = a1;
    }
    if (a1 == 37 && a2 == 42) {
        if (U8_AT(p, 0x25) + 1 >= 18) {
            U8_AT(p, 0x25) = 0;
        } else {
            U8_AT(p, 0x25) += 1;
        }
        PCBox_LoadTrayPageThumbnails(p);
        PCBox_UpdateTrayCursor(p);
        PCBox_UpdateCurrentBoxMarker(p);
        PCBox_DrawTrayBoxNameAndCount(p);
        PCBox_SetSpriteAnim(WORK(p), 5, 4);
        PCBox_StartCursorSlide(WORK(p), a1, a2);
        PCBox_SetPendingUpdateFunc(WORK(p), PCBox_UpdateCursorSlide);
        return;
    }
    if (a1 == 42 && a2 == 37) {
        if (U8_AT(p, 0x25) - 1 < 0) {
            U8_AT(p, 0x25) = 17;
        } else {
            U8_AT(p, 0x25) -= 1;
        }
        PCBox_LoadTrayPageThumbnails(p);
        PCBox_UpdateTrayCursor(p);
        PCBox_UpdateCurrentBoxMarker(p);
        PCBox_DrawTrayBoxNameAndCount(p);
        PCBox_SetSpriteAnim(WORK(p), 4, 2);
        PCBox_StartCursorSlide(WORK(p), a1, a2);
        PCBox_SetPendingUpdateFunc(WORK(p), PCBox_UpdateCursorSlide);
        return;
    }
    if ((s32)a1 >= 37 && (s32)a1 <= 42 && (s32)a2 >= 37 && (s32)a2 <= 42) {
        U8_AT(p, 0x25) = a1 + (U8_AT(p, 0x25) / 6) * 6 - 37;
        PCBox_UpdateTrayCursor(p);
        PCBox_DrawTrayBoxNameAndCount(p);
    }
    PCBox_StartCursorSlide(WORK(p), a1, a2);
    PCBox_SetPendingUpdateFunc(WORK(p), PCBox_UpdateCursorSlide);
}

void ov14_021F7308(void *p, u32 a1) {
    if ((s32)a1 >= 37 && (s32)a1 <= 42) {
        U32_AT(WORK(p), 0x43c) = a1;
    }
    if (a1 != 43 && a1 != 44) {
        GridInputHandler_SetNextInput(GRID(p), (u8)a1);
        PCBox_PlaceCursorOnGridInput(p, a1);
    }
}

u32 ov14_021F7340(void *p) {
    u32 input;
    if (PCBox_IsChangeBoxButtonShown(UNK2F0(p)) == 0) {
        GridInputHandler_ClearEnabledFlag(GRID(p), 45);
    }
    GridInputHandler_ClearEnabledFlag(GRID(p), 43);
    GridInputHandler_ClearEnabledFlag(GRID(p), 44);
    input = GridInputHandler_HandleInput_AllowHold(GRID(p));
    GridInputHandler_SetAllEnabled(GRID(p));
    return input;
}

u32 PCBox_GetMoveModePartyMenuInput(void *p) {
    u32 nextInput = GridInputHandler_GetNextInput(GRID(p));
    u32 input = GridInputHandler_HandleInput_AllowHold(GRID(p));
    if (PCBox_IsActionMenuOpen(UNK2F0(p)) == 0) {
        if (input >= 8 && input <= 12) {
            GridInputHandler_SetNextInput(GRID(p), nextInput);
            PCBox_PlaceCursorOnGridInput(p, nextInput);
            return -1;
        }
    } else if (input == 7 || input == -2) {
        return 13;
    }
    return input;
}

void ov14_021F73E4(void *p, u32 a1, u32 a2) {
    u32 v = a1;
    if (a2 != -1) {
        if (PCBox_IsActionMenuOpen(UNK2F0(p)) == 0) {
            if ((s32)v >= 8) {
                v = U8_AT(p, 0x21);
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
    GridInputHandler_SetNextInput(GRID(p), (u8)v);
    PCBox_PlaceCursorOnGridInput(p, v);
}

void ov14_021F7444(void *p, u32 a1, u32 a2) {
    if (PCBox_IsActionMenuOpen(UNK2F0(p)) == 1) {
        if ((s32)a1 <= 6 && (s32)a2 >= 7) {
            PCBox_ClearMonInfoAndSelection(p);
            PCBox_StartCursorSlide(WORK(p), a1, a2);
            PCBox_SetPendingUpdateFunc(WORK(p), ov14_021EA180);
            return;
        }
    } else if (a1 == 12 && a2 == 7) {
        GridInputHandler_SetNextInput(GRID(p), 7);
        a1 = 7;
    }
    PCBox_StartCursorSlide(WORK(p), a1, a2);
    PCBox_SetPendingUpdateFunc(WORK(p), PCBox_UpdateCursorSlide);
}

u32 PCBox_GetItemModeMenuInput(void *p) {
    u32 nextInput = GridInputHandler_GetNextInput(GRID(p));
    u32 input = GridInputHandler_HandleInput_AllowHold(GRID(p));
    if (PCBox_IsActionMenuOpen(UNK2F0(p)) == 0) {
        if (input == 36) {
            GridInputHandler_SetNextInput(GRID(p), nextInput);
            PCBox_PlaceCursorOnGridInput(p, nextInput);
            return -1;
        }
    } else {
        if (input - 33 <= 1) {
            GridInputHandler_SetNextInput(GRID(p), nextInput);
            PCBox_PlaceCursorOnGridInput(p, nextInput);
            return -1;
        }
        if (input == 35 || input == -2) {
            return 37;
        }
    }
    return input;
}

void ov14_021F7528(void *p, u32 a1, u32 a2) {
    u32 v = a1;
    if (a2 != -1) {
        if (PCBox_IsActionMenuOpen(UNK2F0(p)) == 0) {
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
                v = U8_AT(p, 0x21);
                if (v >= 30) {
                    v = 0;
                }
                break;
            }
        } else if (v != 36) {
            v = 36;
        }
    }
    GridInputHandler_SetNextInput(GRID(p), (u8)v);
    PCBox_PlaceCursorOnGridInput(p, v);
}

RESET_7AC4(ov14_021F7594)
SET_NOTIFY(ov14_021F75AC)

u32 PCBox_GetItemModePartyMenuInput(void *p) {
    u32 nextInput = GridInputHandler_GetNextInput(GRID(p));
    u32 input = GridInputHandler_HandleInput_AllowHold(GRID(p));
    if (PCBox_IsActionMenuOpen(UNK2F0(p)) == 0) {
        if (input == 8) {
            GridInputHandler_SetNextInput(GRID(p), nextInput);
            PCBox_PlaceCursorOnGridInput(p, nextInput);
            return -1;
        }
    } else if (input == 7 || input == -2) {
        return 9;
    }
    return input;
}

void ov14_021F7620(void *p, u32 a1, u32 a2) {
    u32 v = a1;
    if (a2 != -1) {
        if (PCBox_IsActionMenuOpen(UNK2F0(p)) == 0) {
            if ((s32)v >= 8) {
                v = U8_AT(p, 0x21);
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
    GridInputHandler_SetNextInput(GRID(p), (u8)v);
    PCBox_PlaceCursorOnGridInput(p, v);
}

RESET_7AC4(ov14_021F7670)

void ov14_021F7688(void *p, u32 a1) {
    u8 xy[2];
    if (a1 - 6 <= 1) {
        a1 = U8_AT(p, 0x25) % 6;
        U32_AT(WORK(p), 0x43c) = a1;
    }
    GridInputHandler_SetNextInput(GRID(p), (u8)a1);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(GRID(p), a1), &xy[1], &xy[0]);
    ManagedSprite_SetPositionXY(PTR_AT(WORK(p), 0x320), xy[1], xy[0]);
    if ((s32)a1 >= 0 && (s32)a1 <= 5) {
        PCBox_SetSpriteAnim(WORK(p), 9, 14);
    } else {
        PCBox_SetSpriteAnim(WORK(p), 9, 8);
    }
}

void ov14_021F7700(void *p, u32 a1, u32 a2) {
    if ((s32)a1 >= 0 && (s32)a1 <= 5) {
        PCBox_SetSpriteAnim(WORK(p), 9, 14);
    } else {
        PCBox_SetSpriteAnim(WORK(p), 9, 8);
    }
    if (a2 == 8 && a1 == 0) {
        a1 = U32_AT(WORK(p), 0x43c);
        GridInputHandler_SetNextLastUnk0FInputs(GRID(p), (u8)a1, 8, 8);
    }
    if ((s32)a1 >= 0 && (s32)a1 <= 5) {
        U32_AT(WORK(p), 0x43c) = a1;
    }
    if (a1 == 0 && a2 == 5) {
        if (U8_AT(p, 0x25) + 1 >= 18) {
            U8_AT(p, 0x25) = 0;
        } else {
            U8_AT(p, 0x25) += 1;
        }
        PCBox_LoadTrayPageThumbnails(p);
        PCBox_UpdateTrayCursor(p);
        PCBox_UpdateCurrentBoxMarker(p);
        PCBox_DrawTrayBoxNameAndCount(p);
        PCBox_SetSpriteAnim(WORK(p), 5, 4);
        PCBox_StartCursorSlide(WORK(p), a1, a2);
        PCBox_SetPendingUpdateFunc(WORK(p), PCBox_UpdateCursorSlide);
        return;
    }
    if (a1 == 5 && a2 == 0) {
        if (U8_AT(p, 0x25) - 1 < 0) {
            U8_AT(p, 0x25) = 17;
        } else {
            U8_AT(p, 0x25) -= 1;
        }
        PCBox_LoadTrayPageThumbnails(p);
        PCBox_UpdateTrayCursor(p);
        PCBox_UpdateCurrentBoxMarker(p);
        PCBox_DrawTrayBoxNameAndCount(p);
        PCBox_SetSpriteAnim(WORK(p), 4, 2);
        PCBox_StartCursorSlide(WORK(p), a1, a2);
        PCBox_SetPendingUpdateFunc(WORK(p), PCBox_UpdateCursorSlide);
        return;
    }
    if ((s32)a1 >= 0 && (s32)a1 <= 5 && a2 != 8) {
        U8_AT(p, 0x25) = a1 + (U8_AT(p, 0x25) / 6) * 6;
        PCBox_UpdateTrayCursor(p);
        PCBox_DrawTrayBoxNameAndCount(p);
    }
    PCBox_StartCursorSlide(WORK(p), a1, a2);
    PCBox_SetPendingUpdateFunc(WORK(p), PCBox_UpdateCursorSlide);
}

SET_NOTIFY2(ov14_021F7854, 6, 7)

void ov14_021F7878(void *p, u32 a1) {
    u8 xy[2];
    if (a1 - 4 <= 1) {
        a1 = U8_AT(WORK(p), 0x44d) % 4;
        U32_AT(WORK(p), 0x43c) = a1;
    }
    GridInputHandler_SetNextInput(GRID(p), (u8)a1);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(GRID(p), a1), &xy[1], &xy[0]);
    ManagedSprite_SetPositionXY(PTR_AT(WORK(p), 0x320), xy[1], xy[0]);
    if ((s32)a1 >= 0 && (s32)a1 <= 3) {
        PCBox_SetSpriteAnim(WORK(p), 9, 14);
    } else {
        PCBox_SetSpriteAnim(WORK(p), 9, 8);
    }
}

void ov14_021F78F8(void *p, u32 a1, u32 a2) {
    if ((s32)a1 >= 0 && (s32)a1 <= 3) {
        PCBox_SetSpriteAnim(WORK(p), 9, 14);
    } else {
        PCBox_SetSpriteAnim(WORK(p), 9, 8);
    }
    if (a2 == 6 && a1 == 0) {
        a1 = U32_AT(WORK(p), 0x43c);
        GridInputHandler_SetNextLastUnk0FInputs(GRID(p), (u8)a1, 6, 6);
    }
    if ((s32)a1 >= 0 && (s32)a1 <= 3) {
        U32_AT(WORK(p), 0x43c) = a1;
    }
    if (a1 == 0 && a2 == 3) {
        if (U8_AT(WORK(p), 0x44d) + 1 >= 24) {
            U8_AT(WORK(p), 0x44d) = 0;
        } else {
            U8_AT(WORK(p), 0x44d) = U8_AT(WORK(p), 0x44d) + 1;
        }
        PCBox_LoadWallpaperThumbnails(p);
        PCBox_UpdateCurrentWallpaperMarker(p);
        PCBox_UpdateWallpaperCursor(p);
        PCBox_DrawWallpaperName(p);
        PCBox_SetSpriteAnim(WORK(p), 5, 4);
        PCBox_StartCursorSlide(WORK(p), a1, a2);
        PCBox_SetPendingUpdateFunc(WORK(p), PCBox_UpdateCursorSlide);
        return;
    }
    if (a1 == 3 && a2 == 0) {
        if (U8_AT(WORK(p), 0x44d) - 1 < 0) {
            U8_AT(WORK(p), 0x44d) = 23;
        } else {
            U8_AT(WORK(p), 0x44d) = U8_AT(WORK(p), 0x44d) - 1;
        }
        PCBox_LoadWallpaperThumbnails(p);
        PCBox_UpdateCurrentWallpaperMarker(p);
        PCBox_UpdateWallpaperCursor(p);
        PCBox_DrawWallpaperName(p);
        PCBox_SetSpriteAnim(WORK(p), 4, 2);
        PCBox_StartCursorSlide(WORK(p), a1, a2);
        PCBox_SetPendingUpdateFunc(WORK(p), PCBox_UpdateCursorSlide);
        return;
    }
    if ((s32)a1 >= 0 && (s32)a1 <= 3 && a2 != 6) {
        U8_AT(WORK(p), 0x44d) = a1 + (U8_AT(WORK(p), 0x44d) / 4) * 4;
        PCBox_UpdateWallpaperCursor(p);
        PCBox_DrawWallpaperName(p);
    }
    PCBox_StartCursorSlide(WORK(p), a1, a2);
    PCBox_SetPendingUpdateFunc(WORK(p), PCBox_UpdateCursorSlide);
}

SET_NOTIFY2(ov14_021F7A2C, 4, 5)

u32 ov14_021F7A50(void *p) {
    u32 input;
    GridInputHandler_ClearEnabledFlag(GRID(p), 43);
    GridInputHandler_ClearEnabledFlag(GRID(p), 44);
    GridInputHandler_ClearEnabledFlag(GRID(p), 45);
    GridInputHandler_ClearEnabledFlag(GRID(p), 37);
    GridInputHandler_ClearEnabledFlag(GRID(p), 38);
    GridInputHandler_ClearEnabledFlag(GRID(p), 39);
    GridInputHandler_ClearEnabledFlag(GRID(p), 40);
    GridInputHandler_ClearEnabledFlag(GRID(p), 41);
    GridInputHandler_ClearEnabledFlag(GRID(p), 42);
    input = GridInputHandler_HandleInput_AllowHold(GRID(p));
    GridInputHandler_SetAllEnabled(GRID(p));
    return input;
}

void PCBox_StartCursorSlide(void *q, u32 a1, u32 a2) {
    u8 s[4];
    void *w;
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(PTR_AT(q, 0x2c), a1), &s[3], &s[2]);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(PTR_AT(q, 0x2c), a2), &s[1], &s[0]);
    w = Heap_Alloc(10, 8);
    U32_AT(w, 4) = (U32_AT(w, 4) & 3) | 16;
    U8_AT(w, 0) = s[3];
    U8_AT(w, 1) = s[2];
    if (s[3] >= s[1]) {
        U8_AT(w, 2) = s[3] - s[1];
        U32_AT(w, 4) &= ~1;
    } else {
        U8_AT(w, 2) = s[1] - s[3];
        U32_AT(w, 4) = (U32_AT(w, 4) & ~1) | 1;
    }
    if (s[2] >= s[0]) {
        U8_AT(w, 3) = s[2] - s[0];
        U32_AT(w, 4) &= ~2;
    } else {
        U8_AT(w, 3) = s[0] - s[2];
        U32_AT(w, 4) |= 2;
    }
    U8_AT(w, 2) = ((U8_AT(w, 2) << 8) / (U32_AT(w, 4) >> 2)) >> 8;
    U8_AT(w, 3) = ((U8_AT(w, 3) << 8) / (U32_AT(w, 4) >> 2)) >> 8;
    PTR_AT(q, 0xc) = w;
}

u32 PCBox_IsSummaryShortcutPressed(void *p) {
    if (PCBox_IsSummaryButtonShown(UNK2F0(p)) == 0) {
        return 0;
    }
    if (gSystem[0x12] & 0x800) {
        return 1;
    }
    if (TouchscreenHitbox_TouchNewIsIn(ov14_021F86C8) == 1) {
        return 1;
    }
    return 0;
}
