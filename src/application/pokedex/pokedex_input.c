#include "global.h"

#include "constants/sndseq.h"

#include "application/pokedex/pokedex_internal.h"

#include "menu_input_state.h"
#include "pokedex.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "system.h"
#include "touchscreen.h"
#include "grid_input_handler.h"
#include "dpad_menu_box.h"

extern const TouchscreenHitbox ov18_021FB72C[];
extern const int ov18_021FB6F0[];
extern const TouchscreenHitbox ov18_021FB8A4[];
extern const int ov18_021FB84C[];
extern const TouchscreenHitbox ov18_021FB704[];
extern const int ov18_021FB698[];
extern const TouchscreenHitbox ov18_021FB934[];
extern const int ov18_021FB904[];
extern const TouchscreenHitbox ov18_021FB804[];
extern const int ov18_021FB760[];
extern const TouchscreenHitbox ov18_021FB7E0[];
extern const int ov18_021FB7C0[];
extern const TouchscreenHitbox ov18_021FB630[];
extern const TouchscreenHitbox ov18_021FB718[]; // height / weight slider handles

extern const TouchscreenHitbox sSearchMenuHitboxes[];
extern const DpadMenuBox sSearchMenuDpadBoxes[];
extern const GridCallbacks sSearchMenuGridCallbacks;
extern const u8 sSearchMenuCursorAnims[];
extern const TouchscreenHitbox sSearchOrderHitboxes[];
extern const DpadMenuBox sSearchOrderDpadBoxes[];
extern const GridCallbacks sSearchOrderGridCallbacks;
extern const u8 sSearchOrderCursorAnims[];
extern const TouchscreenHitbox sSearchNameHitboxes[];
extern const DpadMenuBox sSearchNameDpadBoxes[];
extern const GridCallbacks sSearchNameGridCallbacks;
extern const TouchscreenHitbox sSearchTypeHitboxes[];
extern const DpadMenuBox sSearchTypeDpadBoxes[];
extern const GridCallbacks sSearchTypeGridCallbacks;
extern const u8 sSearchTypeCursorAnims[];
extern const TouchscreenHitbox sSearchHeightHitboxes[];
extern const DpadMenuBox sSearchHeightDpadBoxes[];
extern const GridCallbacks sSearchHeightGridCallbacks;
extern const u8 sSearchHeightCursorAnims[];
extern const TouchscreenHitbox sSearchWeightHitboxes[];
extern const DpadMenuBox sSearchWeightDpadBoxes[];
extern const GridCallbacks sSearchWeightGridCallbacks;
extern const u8 sSearchWeightCursorAnims[];
extern const TouchscreenHitbox sSearchAreaHitboxes[];
extern const DpadMenuBox sSearchAreaDpadBoxes[];
extern const GridCallbacks sSearchAreaGridCallbacks;
extern const u8 sSearchAreaCursorAnims[];
extern const TouchscreenHitbox sSearchFormHitboxes[];
extern const DpadMenuBox sSearchFormDpadBoxes[];
extern const GridCallbacks sSearchFormGridCallbacks;
extern const u8 sSearchFormCursorAnims[];

// Entry screen tabs and list buttons (touch), A / B / Start (keys). In the national dex (unk_1860) left and
// right switch between the Johto and national lists.
int PokedexApp_HandleListMenuInput(PokedexAppData *pokedexApp) {
    pokedexApp->unk_0864 = 0;
    if (System_GetTouchNew() == TRUE) {
        int hit = TouchscreenHitbox_FindRectAtTouchNew(ov18_021FB72C);
        if (hit == -1) {
            return -1;
        }
        int input = ov18_021FB6F0[hit];
        if (input == 6 && pokedexApp->unk_1860 == 0) {
            return 5;
        }
        return input;
    } else {
        pokedexApp->unk_0864 = 1;
        if (pokedexApp->unk_1860 == 1) {
            if (gSystem.newKeys & PAD_KEY_LEFT) {
                if (pokedexApp->unk_1858 == 0) {
                    return 3;
                }
                return -1;
            }
            if (gSystem.newKeys & PAD_KEY_RIGHT) {
                if (pokedexApp->unk_1858 == 1) {
                    return 4;
                }
                return -1;
            }
        }
        if (gSystem.newKeys & PAD_BUTTON_A || gSystem.newKeys & PAD_BUTTON_START) {
            return 0;
        }
        if (gSystem.newKeys & PAD_BUTTON_B) {
            return 2;
        }
        if (gSystem.newKeys & PAD_BUTTON_Y) {
            return 1;
        }
    }
    return -1;
}

// The list: a 5x3 icon grid per page (unk_1859 page, unk_185A cursor).
int PokedexApp_HandleListInput(PokedexAppData *pokedexApp, int mode) {
    u32 x;
    u32 y;
    u32 input;

    pokedexApp->unk_0864 = 0;
    if (System_GetTouchNewCoords(&x, &y) == TRUE) {
        int hit = TouchscreenHitbox_FindRectAtTouchNew(ov18_021FB8A4);
        if (hit == -1) {
            return -1;
        }
        input = ov18_021FB84C[hit];
        if (input == 0) {
            u8 selection;
            x -= 27;
            y -= 4;
            selection = x / 40 + 5 * (y / 40);
            if (selection == pokedexApp->listCursor) {
                return 4;
            }
            pokedexApp->listCursor = selection;
            PlaySE(SEQ_SE_GS_ZKN04);
        } else if (input == 14) {
            PlaySE(SEQ_SE_GS_BUTTON01);
        } else if (input == 2) {
            PlaySE(SEQ_SE_GS_ZKN04);
        } else if (input == 5) {
            PlaySE(SEQ_SE_GS_GEARCANCEL);
        }
        return input;
    }
    pokedexApp->unk_0864 = 1;
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        if (pokedexApp->listCursor < 5) {
            return 10;
        }
        pokedexApp->listCursor -= 5;
        PlaySE(SEQ_SE_GS_ZKN03);
        return 0;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        if (pokedexApp->listCursor >= 10 && pokedexApp->listCursor < 15) {
            return 12;
        }
        pokedexApp->listCursor += 5;
        PlaySE(SEQ_SE_GS_ZKN03);
        return 0;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        if (pokedexApp->listCursor != 0) {
            pokedexApp->listCursor--;
            PlaySE(SEQ_SE_GS_ZKN03);
            return 0;
        }
        if (pokedexApp->listPage != 0) {
            pokedexApp->listCursor = 14;
            return 9;
        }
        return -1;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        int selection = pokedexApp->listCursor + 1;
        if (selection != 15) {
            pokedexApp->listCursor = selection;
            PlaySE(SEQ_SE_GS_ZKN03);
            return 0;
        }
        if (pokedexApp->listPage + 1 <= PokedexApp_GetLastListPage(pokedexApp, mode)) {
            pokedexApp->listCursor = 0;
            return 11;
        }
        return -1;
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        return 4;
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return 6;
    }
    if (gSystem.newKeys & PAD_BUTTON_X) {
        return 3;
    }
    if (gSystem.newKeys & PAD_BUTTON_Y) {
        PlaySE(SEQ_SE_GS_ZKN04);
        return 2;
    }
    if (gSystem.newAndRepeatedKeys & PAD_BUTTON_L) {
        return 9;
    }
    if (gSystem.newAndRepeatedKeys & PAD_BUTTON_R) {
        return 11;
    }
    if (gSystem.newKeys & PAD_BUTTON_SELECT) {
        return 8;
    }
    if (gSystem.newKeys & PAD_BUTTON_START) {
        PlaySE(SEQ_SE_GS_BUTTON01);
        return 1;
    }
    return -1;
}

// The search pages: each has a grid input handler (unk_1864) whose callbacks move the cursor sprite 0.

void PokedexApp_CreateSearchMenuGrid(PokedexAppData *pokedexApp, int initialPos) {
    pokedexApp->unk_1864 = GridInputHandler_Create(sSearchMenuHitboxes, sSearchMenuDpadBoxes, &sSearchMenuGridCallbacks, pokedexApp, TRUE, initialPos, HEAP_ID_POKEDEX_APP);
    PokedexApp_SetSpriteVisible(pokedexApp, 0, TRUE);
    PokedexApp_UpdateSearchMenuCursor(pokedexApp, initialPos);
}

#pragma force_active on

u32 PokedexApp_HandleSearchMenuInput(PokedexAppData *pokedexApp) {
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (gSystem.newKeys & PAD_BUTTON_START) {
        return PAD_BUTTON_START;
    }
    return GridInputHandler_HandleInput_AllowHold(pokedexApp->unk_1864);
}

#pragma force_active reset

void PokedexApp_UpdateSearchMenuCursor(PokedexAppData *pokedexApp, int target) {
    u8 x;
    u8 y;

    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(pokedexApp->unk_1864, target), &x, &y);
    ManagedSprite_SetPositionXY(pokedexApp->sprites[0], x, y);
    PokedexApp_SetSpriteAnim(pokedexApp, 0, sSearchMenuCursorAnims[target]);
}

void PokedexApp_OnSearchMenuGridMove(PokedexAppData *pokedexApp, int target, int input) {
    PokedexApp_UpdateSearchMenuCursor(pokedexApp, target);
    PlaySE(SEQ_SE_GS_ZKN03);
}

void PokedexApp_OnSearchMenuGridTouch(PokedexAppData *pokedexApp, int target, int input) {
    PokedexApp_UpdateSearchMenuCursor(pokedexApp, target);
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
}

#pragma force_active on

void PokedexApp_CreateSearchOrderGrid(PokedexAppData *pokedexApp) {
    pokedexApp->unk_1864 = GridInputHandler_Create(sSearchOrderHitboxes, sSearchOrderDpadBoxes, &sSearchOrderGridCallbacks, pokedexApp, TRUE, 0, HEAP_ID_POKEDEX_APP);
    PokedexApp_SetSpriteVisible(pokedexApp, 0, TRUE);
    PokedexApp_UpdateSearchOrderCursor(pokedexApp, 0);
}

u32 PokedexApp_HandleSearchOrderInput(PokedexAppData *pokedexApp) {
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (gSystem.newKeys & PAD_BUTTON_START) {
        return 6;
    }
    return GridInputHandler_HandleInput_AllowHold(pokedexApp->unk_1864);
}

void PokedexApp_UpdateSearchOrderCursor(PokedexAppData *pokedexApp, int target) {
    struct {
        u8 y, x;
    } pos;

    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(pokedexApp->unk_1864, target), &pos.x, &pos.y);
    ManagedSprite_SetPositionXY(pokedexApp->sprites[0], pos.x, pos.y);
    PokedexApp_SetSpriteAnim(pokedexApp, 0, sSearchOrderCursorAnims[target]);
}

#pragma force_active reset

void PokedexApp_OnSearchOrderGridMove(PokedexAppData *pokedexApp, int target, int input) {
    PokedexApp_UpdateSearchOrderCursor(pokedexApp, target);
    PlaySE(SEQ_SE_GS_ZKN03);
}

void PokedexApp_OnSearchOrderGridTouch(PokedexAppData *pokedexApp, int target, int input) {
    PokedexApp_UpdateSearchOrderCursor(pokedexApp, target);
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
}

void PokedexApp_CreateSearchNameGrid(PokedexAppData *pokedexApp) {
    pokedexApp->unk_1864 = GridInputHandler_Create(sSearchNameHitboxes, sSearchNameDpadBoxes, &sSearchNameGridCallbacks, pokedexApp, TRUE, 0, HEAP_ID_POKEDEX_APP);
    PokedexApp_SetSpriteVisible(pokedexApp, 0, TRUE);
    PokedexApp_UpdateSearchNameCursor(pokedexApp, 0);
}

u32 PokedexApp_HandleSearchNameInput(PokedexAppData *pokedexApp) {
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (gSystem.newKeys & PAD_BUTTON_START) {
        return 27;
    }
    return GridInputHandler_HandleInput_AllowHold(pokedexApp->unk_1864);
}

// 27 letter buttons, then OK / cancel
void PokedexApp_UpdateSearchNameCursor(PokedexAppData *pokedexApp, int target) {
    u8 pos[2];
    const DpadMenuBox *box = GridInputHandler_GetDpadBox(pokedexApp->unk_1864, target);

    DpadMenuBox_GetPosition(box, &pos[1], &pos[0]);
    ManagedSprite_SetPositionXY(pokedexApp->sprites[0], pos[1], pos[0]);
    if (target >= 27) {
        PokedexApp_SetSpriteAnim(pokedexApp, 0, 35);
    } else {
        PokedexApp_SetSpriteAnim(pokedexApp, 0, 40);
    }
}

// These calls were compiled against a prototype with u8 inputs (the arguments are narrowed once).
typedef void (*GridSetInputsU8Func)(GridInputHandler *inputHandler, u8 nextInput, u8 lastInput, u8 unk0F);

// Moving up or down from OK / cancel lands in the letter column the cursor left from.
void PokedexApp_OnSearchNameGridMove(PokedexAppData *pokedexApp, int target, int input) {
    if ((u32)(input - 27) <= 1) {
        int previous = pokedexApp->prevGridInput;
        u8 left = sSearchNameDpadBoxes[previous].left;
        if (target == 0 || target == 6) {
            int i;
            for (i = 0; i <= 26; i++) {
                if (left == sSearchNameDpadBoxes[i].left) {
                    target = i;
                    ((GridSetInputsU8Func)GridInputHandler_SetNextLastUnk0FInputs)(pokedexApp->unk_1864, target, input, input);
                    break;
                }
            }
        } else if (target == 21 || target == 26) {
            int i;
            for (i = 26; i >= 0; i--) {
                if (left == sSearchNameDpadBoxes[i].left) {
                    target = i;
                    ((GridSetInputsU8Func)GridInputHandler_SetNextLastUnk0FInputs)(pokedexApp->unk_1864, target, input, input);
                    break;
                }
            }
        }
    }
    PokedexApp_UpdateSearchNameCursor(pokedexApp, target);
    pokedexApp->prevGridInput = input;
    PlaySE(SEQ_SE_GS_ZKN03);
}

void PokedexApp_OnSearchNameGridTouch(PokedexAppData *pokedexApp, int target, int input) {
    PokedexApp_UpdateSearchNameCursor(pokedexApp, target);
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
    pokedexApp->prevGridInput = input;
}

void PokedexApp_CreateSearchTypeGrid(PokedexAppData *pokedexApp) {
    pokedexApp->unk_1864 = GridInputHandler_Create(sSearchTypeHitboxes, sSearchTypeDpadBoxes, &sSearchTypeGridCallbacks, pokedexApp, TRUE, 0, HEAP_ID_POKEDEX_APP);
    PokedexApp_SetSpriteVisible(pokedexApp, 0, TRUE);
    PokedexApp_UpdateSearchTypeCursor(pokedexApp, 0);
}

u32 PokedexApp_HandleSearchTypeInput(PokedexAppData *pokedexApp) {
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (gSystem.newKeys & PAD_BUTTON_START) {
        return 18;
    }
    return GridInputHandler_HandleInput_AllowHold(pokedexApp->unk_1864);
}

void PokedexApp_UpdateSearchTypeCursor(PokedexAppData *pokedexApp, int target) {
    u8 position[2];
    const DpadMenuBox *box = GridInputHandler_GetDpadBox(pokedexApp->unk_1864, target);

    DpadMenuBox_GetPosition(box, &position[1], &position[0]);
    ManagedSprite_SetPositionXY(pokedexApp->sprites[0], position[1], position[0]);
    PokedexApp_SetSpriteAnim(pokedexApp, 0, sSearchTypeCursorAnims[target]);
}

// Moving into the OK (18) / cancel (19) buttons and back returns to the column the cursor came from.
void PokedexApp_OnSearchTypeGridMove(PokedexAppData *pokedexApp, int target, int input) {
    if (input == 18) {
        if (target == 0) {
            s32 previous = pokedexApp->prevGridInput;
            if (previous >= 16 && previous <= 17) {
                target = previous % 4;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)target, (u8)input, 18);
            } else if (previous >= 0 && previous <= 1) {
                target = previous;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)previous, (u8)input, 18);
            }
        } else if (target == 16) {
            s32 previous = pokedexApp->prevGridInput;
            if (previous >= 0 && previous <= 1) {
                target = previous + 16;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)target, (u8)input, 18);
            } else if (previous >= 16 && previous <= 17) {
                target = previous;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)previous, (u8)input, 18);
            }
        }
    } else if (input == 19) {
        if (target == 3) {
            s32 previous = pokedexApp->prevGridInput;
            if (previous >= 14 && previous <= 15) {
                target = previous % 4;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)target, (u8)input, 19);
            } else if (previous >= 2 && previous <= 3) {
                target = previous;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)previous, (u8)input, 19);
            }
        } else if (target == 15) {
            s32 previous = pokedexApp->prevGridInput;
            if (previous >= 2 && previous <= 3) {
                target = previous + 12;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)target, (u8)input, 19);
            } else if (previous >= 14 && previous <= 15) {
                target = previous;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)previous, (u8)input, 19);
            }
        }
    }
    PokedexApp_UpdateSearchTypeCursor(pokedexApp, target);
    pokedexApp->prevGridInput = input;
    PlaySE(SEQ_SE_GS_ZKN03);
}

void PokedexApp_OnSearchTypeGridTouch(PokedexAppData *pokedexApp, int target, int input) {
    PokedexApp_UpdateSearchTypeCursor(pokedexApp, target);
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
    pokedexApp->prevGridInput = input;
}

#pragma force_active on

void PokedexApp_CreateSearchHeightGrid(PokedexAppData *pokedexApp) {
    pokedexApp->unk_1864 = GridInputHandler_Create(sSearchHeightHitboxes, sSearchHeightDpadBoxes, &sSearchHeightGridCallbacks, pokedexApp, TRUE, 0, HEAP_ID_POKEDEX_APP);
    PokedexApp_SetSpriteVisible(pokedexApp, 0, TRUE);
    PokedexApp_UpdateSearchHeightCursor(pokedexApp, 0);
}

#pragma opt_common_subs off

// Two slider handles (targets 0 and 1): left / right move the selected one; dragging a handle selects it.
u32 PokedexApp_HandleSearchHeightInput(PokedexAppData *pokedexApp) {
    int next = GridInputHandler_GetNextInput(pokedexApp->unk_1864);
    unsigned hit, index;

    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (gSystem.newKeys & PAD_BUTTON_START) {
        return 2;
    }
    if (next == 0) {
        if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
            return 5;
        }
        if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
            return 4;
        }
    }
    if (next == 1) {
        if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
            return 7;
        }
        if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
            return 6;
        }
    }
    hit = TouchscreenHitbox_FindRectAtTouchHeld(ov18_021FB718);
    if (hit != (unsigned)-1) {
        MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
        index = hit >> 1;
        GridInputHandler_SetNextInput(pokedexApp->unk_1864, (u8)index);
        PokedexApp_UpdateSearchHeightCursor(pokedexApp, index);
        return hit + 4;
    }
    return GridInputHandler_HandleInput_AllowHold(pokedexApp->unk_1864);
}

#pragma opt_common_subs reset

// The handles sit at their slider values.
void PokedexApp_UpdateSearchHeightCursor(PokedexAppData *pokedexApp, int target) {
    u8 x, y;

    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(pokedexApp->unk_1864, target), &x, &y);
    if (target == 0) {
        x = PokedexApp_SliderValueToX(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX]);
    } else if (target == 1) {
        x = PokedexApp_SliderValueToX(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN]);
    }
    ManagedSprite_SetPositionXY(pokedexApp->sprites[0], x, y);
    PokedexApp_SetSpriteAnim(pokedexApp, 0, sSearchHeightCursorAnims[target]);
}

#pragma force_active reset

void PokedexApp_OnSearchHeightGridMove(PokedexAppData *pokedexApp, int target, int input) {
    PokedexApp_UpdateSearchHeightCursor(pokedexApp, target);
    PlaySE(SEQ_SE_GS_ZKN03);
}

void PokedexApp_OnSearchHeightGridTouch(PokedexAppData *pokedexApp, int target, int input) {
    PokedexApp_UpdateSearchHeightCursor(pokedexApp, target);
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
}

#pragma force_active on

void PokedexApp_CreateSearchWeightGrid(PokedexAppData *pokedexApp) {
    pokedexApp->unk_1864 = GridInputHandler_Create(sSearchWeightHitboxes, sSearchWeightDpadBoxes, &sSearchWeightGridCallbacks, pokedexApp, TRUE, 0, HEAP_ID_POKEDEX_APP);
    PokedexApp_SetSpriteVisible(pokedexApp, 0, TRUE);
    PokedexApp_UpdateSearchWeightCursor(pokedexApp, 0);
}

#pragma opt_common_subs off

// Two slider handles (targets 0 and 1): left / right move the selected one; dragging a handle selects it.
u32 PokedexApp_HandleSearchWeightInput(PokedexAppData *pokedexApp) {
    int next = GridInputHandler_GetNextInput(pokedexApp->unk_1864);
    unsigned hit, index;

    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (gSystem.newKeys & PAD_BUTTON_START) {
        return 2;
    }
    if (next == 0) {
        if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
            return 5;
        }
        if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
            return 4;
        }
    }
    if (next == 1) {
        if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
            return 7;
        }
        if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
            return 6;
        }
    }
    hit = TouchscreenHitbox_FindRectAtTouchHeld(ov18_021FB718);
    if (hit != (unsigned)-1) {
        MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
        index = hit >> 1;
        GridInputHandler_SetNextInput(pokedexApp->unk_1864, (u8)index);
        PokedexApp_UpdateSearchWeightCursor(pokedexApp, index);
        return hit + 4;
    }
    return GridInputHandler_HandleInput_AllowHold(pokedexApp->unk_1864);
}

#pragma opt_common_subs reset

// The handles sit at their slider values.
void PokedexApp_UpdateSearchWeightCursor(PokedexAppData *pokedexApp, int target) {
    u8 x, y;

    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(pokedexApp->unk_1864, target), &x, &y);
    if (target == 0) {
        x = PokedexApp_SliderValueToX(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX]);
    } else if (target == 1) {
        x = PokedexApp_SliderValueToX(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN]);
    }
    ManagedSprite_SetPositionXY(pokedexApp->sprites[0], x, y);
    PokedexApp_SetSpriteAnim(pokedexApp, 0, sSearchWeightCursorAnims[target]);
}

#pragma force_active reset

void PokedexApp_OnSearchWeightGridMove(PokedexAppData *pokedexApp, int target, int input) {
    PokedexApp_UpdateSearchWeightCursor(pokedexApp, target);
    PlaySE(SEQ_SE_GS_ZKN03);
}

void PokedexApp_OnSearchWeightGridTouch(PokedexAppData *pokedexApp, int target, int input) {
    PokedexApp_UpdateSearchWeightCursor(pokedexApp, target);
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
}

#pragma force_active on

u32 PokedexApp_GetTouchHeldX(u32 *x) {
    u32 y;
    return System_GetTouchHeldCoords(x, &y);
}

void PokedexApp_CreateSearchAreaGrid(PokedexAppData *pokedexApp) {
    pokedexApp->unk_1864 = GridInputHandler_Create(sSearchAreaHitboxes, sSearchAreaDpadBoxes, &sSearchAreaGridCallbacks, pokedexApp, TRUE, 0, HEAP_ID_POKEDEX_APP);
    PokedexApp_SetSpriteVisible(pokedexApp, 0, TRUE);
    PokedexApp_UpdateSearchAreaCursor(pokedexApp, 0);
}

u32 PokedexApp_HandleSearchAreaInput(PokedexAppData *pokedexApp) {
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (gSystem.newKeys & PAD_BUTTON_START) {
        return 4;
    }
    return GridInputHandler_HandleInput_AllowHold(pokedexApp->unk_1864);
}

void PokedexApp_UpdateSearchAreaCursor(PokedexAppData *pokedexApp, int target) {
    u8 x, y;
    const DpadMenuBox *box = GridInputHandler_GetDpadBox(pokedexApp->unk_1864, target);

    DpadMenuBox_GetPosition(box, &x, &y);
    ManagedSprite_SetPositionXY(pokedexApp->sprites[0], x, y);
    PokedexApp_SetSpriteAnim(pokedexApp, 0, sSearchAreaCursorAnims[target]);
}

#pragma force_active reset

void PokedexApp_OnSearchAreaGridMove(PokedexAppData *pokedexApp, int target, int input) {
    PokedexApp_UpdateSearchAreaCursor(pokedexApp, target);
    PlaySE(SEQ_SE_GS_ZKN03);
}

void PokedexApp_OnSearchAreaGridTouch(PokedexAppData *pokedexApp, int target, int input) {
    PokedexApp_UpdateSearchAreaCursor(pokedexApp, target);
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
}

void PokedexApp_CreateSearchFormGrid(PokedexAppData *pokedexApp) {
    pokedexApp->unk_1864 = GridInputHandler_Create(sSearchFormHitboxes, sSearchFormDpadBoxes, &sSearchFormGridCallbacks, pokedexApp, TRUE, 0, HEAP_ID_POKEDEX_APP);
    PokedexApp_SetSpriteVisible(pokedexApp, 0, TRUE);
    PokedexApp_UpdateSearchFormCursor(pokedexApp, 0);
}

u32 PokedexApp_HandleSearchFormInput(PokedexAppData *pokedexApp) {
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (gSystem.newKeys & PAD_BUTTON_START) {
        return 15;
    }
    return GridInputHandler_HandleInput_AllowHold(pokedexApp->unk_1864);
}

void PokedexApp_UpdateSearchFormCursor(PokedexAppData *pokedexApp, int target) {
    u8 pos[2];
    const DpadMenuBox *box = GridInputHandler_GetDpadBox(pokedexApp->unk_1864, target);

    DpadMenuBox_GetPosition(box, &pos[1], &pos[0]);
    ManagedSprite_SetPositionXY(pokedexApp->sprites[0], pos[1], pos[0]);
    PokedexApp_SetSpriteAnim(pokedexApp, 0, sSearchFormCursorAnims[target]);
}

// Moving into the OK (15) / cancel (16) buttons and back returns to the column the cursor came from.
void PokedexApp_OnSearchFormGridMove(PokedexAppData *pokedexApp, int target, int input) {
    int previous;

    if (input == 15) {
        if (target == 0) {
            previous = pokedexApp->prevGridInput;
            if (previous >= 0 && previous <= 2) {
                target = previous;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)target, (u8)input, 15);
            } else if (previous >= 10 && previous <= 12) {
                target = previous % 5;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)target, (u8)input, 15);
            }
        } else if (target == 10) {
            previous = pokedexApp->prevGridInput;
            if (previous >= 0 && previous <= 2) {
                target = previous + 10;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)target, (u8)input, 15);
            } else if (previous >= 10 && previous <= 12) {
                target = previous;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)target, (u8)input, 15);
            }
        }
    } else if (input == 16) {
        if (target == 4) {
            previous = pokedexApp->prevGridInput;
            if (previous >= 3 && previous <= 4) {
                target = previous;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)target, (u8)input, 16);
            } else if (previous >= 13 && previous <= 14) {
                target = previous % 5;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)target, (u8)input, 16);
            }
        } else if (target == 14) {
            previous = pokedexApp->prevGridInput;
            if (previous >= 3 && previous <= 4) {
                target = previous + 10;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)target, (u8)input, 16);
            } else if (previous >= 13 && previous <= 14) {
                target = previous;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)target, (u8)input, 16);
            }
        }
    }
    PokedexApp_UpdateSearchFormCursor(pokedexApp, target);
    pokedexApp->prevGridInput = input;
    PlaySE(SEQ_SE_GS_ZKN03);
}

void PokedexApp_OnSearchFormGridTouch(PokedexAppData *pokedexApp, int target, int input) {
    PokedexApp_UpdateSearchFormCursor(pokedexApp, target);
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
    pokedexApp->prevGridInput = input;
}

// The list in the search results (as PokedexApp_HandleListInput; the first entry of the first page
// leaves the results).
u32 PokedexApp_HandleSearchResultListInput(PokedexAppData *pokedexApp, int mode) {
    u32 x;
    u32 y;
    u32 input;

    if (System_GetTouchNewCoords(&x, &y) == TRUE) {
        int hit = TouchscreenHitbox_FindRectAtTouchNew(ov18_021FB8A4);
        if (hit == -1) {
            return -1;
        }
        input = ov18_021FB84C[hit];
        if (input == 0) {
            u8 selection;
            x -= 27;
            y -= 4;
            selection = x / 40 + 5 * (y / 40);
            if (selection == pokedexApp->listCursor) {
                if (pokedexApp->listCursor == 0 && pokedexApp->listPage == 0) {
                    PlaySE(SEQ_SE_GS_ZKN04);
                    return 2;
                }
                return 4;
            }
            pokedexApp->listCursor = selection;
            PlaySE(SEQ_SE_GS_ZKN04);
        } else if (input == 2) {
            PlaySE(SEQ_SE_GS_ZKN04);
        } else if (input == 5) {
            PlaySE(SEQ_SE_GS_GEARCANCEL);
        }
        return input;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        if (pokedexApp->listCursor < 5) {
            return 10;
        }
        pokedexApp->listCursor -= 5;
        PlaySE(SEQ_SE_GS_ZKN03);
        return 0;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        if (pokedexApp->listCursor >= 10 && pokedexApp->listCursor < 15) {
            return 12;
        }
        pokedexApp->listCursor += 5;
        PlaySE(SEQ_SE_GS_ZKN03);
        return 0;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        if (pokedexApp->listCursor != 0) {
            pokedexApp->listCursor--;
            PlaySE(SEQ_SE_GS_ZKN03);
            return 0;
        }
        if (pokedexApp->listPage != 0) {
            pokedexApp->listCursor = 14;
            return 9;
        }
        return -1;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        int selection = pokedexApp->listCursor + 1;
        if (selection != 15) {
            pokedexApp->listCursor = selection;
            PlaySE(SEQ_SE_GS_ZKN03);
            return 0;
        }
        if (pokedexApp->listPage + 1 <= PokedexApp_GetLastListPage(pokedexApp, mode)) {
            pokedexApp->listCursor = 0;
            return 11;
        }
        return -1;
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        return 4;
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return 6;
    }
    if (gSystem.newKeys & PAD_BUTTON_X) {
        return 3;
    }
    if (gSystem.newKeys & PAD_BUTTON_Y) {
        PlaySE(SEQ_SE_GS_ZKN04);
        return 2;
    }
    if (gSystem.newAndRepeatedKeys & PAD_BUTTON_L) {
        return 9;
    }
    if (gSystem.newAndRepeatedKeys & PAD_BUTTON_R) {
        return 11;
    }
    if (gSystem.newKeys & PAD_BUTTON_SELECT) {
        return 8;
    }
    return -1;
}

void PokedexApp_FreeGridInput(PokedexAppData *pokedexApp) {
    if (pokedexApp->unk_1864 != NULL) {
        GridInputHandler_Free(pokedexApp->unk_1864);
        pokedexApp->unk_1864 = NULL;
    }
}

void PokedexApp_DummyGridCallback(PokedexAppData *pokedexApp, int target, int input) {
}

// The tabs of the entry screens (touch) and B. The size tab needs the species caught.
int PokedexApp_HandleEntryTabTouch(PokedexAppData *pokedexApp, u32 *touched) {
    int hit;
    u32 coords[2];

    if (System_GetTouchNew() == TRUE) {
        hit = TouchscreenHitbox_FindRectAtTouchNew(ov18_021FB704);
        if (hit == -1) {
            return -1;
        }
        System_GetTouchNewCoords(&coords[1], &coords[0]);
        hit = ov18_021FB698[hit];
        if (hit == 1 && !Pokedex_CheckMonCaughtFlag(pokedexApp->args->pokedex, pokedexApp->curSpecies)) {
            return -1;
        }
        *touched = 1;
        return hit;
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        *touched = 0;
        return 3;
    }
    return -1;
}

// Area screen: up / down scroll the area list, A cycles the time of day, Select / L / R switch Johto / Kanto.
u32 PokedexApp_HandleAreaInput(PokedexAppData *pokedexApp, u32 *touched) {
    int ret = PokedexApp_HandleEntryTabTouch(pokedexApp, touched);
    if (ret != -1) {
        return ret;
    }
    if (System_GetTouchNew() == TRUE) {
        ret = TouchscreenHitbox_FindHitboxAtTouchNew(ov18_021FB934);
        if (ret == -1) {
            return -1;
        }
        *touched = 1;
        return ov18_021FB904[ret];
    }
    *touched = 0;
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        return 5;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        return 7;
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        if (pokedexApp->unk_18C9 == 0) {
            return 14;
        }
        if (pokedexApp->unk_18C9 == 1) {
            return 15;
        }
        return 13;
    }
    if (gSystem.newKeys & PAD_BUTTON_SELECT) {
        if (pokedexApp->unk_18C8 == 0) {
            return 12;
        }
        return 11;
    }
    if (pokedexApp->unk_18C8 == 0) {
        if (gSystem.newKeys & PAD_BUTTON_R) {
            return 12;
        }
    } else if (gSystem.newKeys & PAD_BUTTON_L) {
        return 11;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        return 16;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        if (Pokedex_CheckMonCaughtFlag(pokedexApp->args->pokedex, pokedexApp->curSpecies)) {
            return 1;
        }
        return 2;
    }
    return -1;
}

#pragma force_active on

u32 PokedexApp_HandleSizeCheckInput(PokedexAppData *pokedexApp, u32 *touched) {
    int value = PokedexApp_HandleEntryTabTouch(pokedexApp, touched), fallback = -1;

    if (value == fallback) {
        value = 0;
        *touched = 0;
        if (!(gSystem.newAndRepeatedKeys & PAD_KEY_LEFT)) {
            if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
                fallback = 2;
            }
            value = fallback;
        }
    }
    return value;
}

#pragma force_active reset

u32 PokedexApp_HandleFormsInput(PokedexAppData *pokedexApp, u32 *touched) {
    int ret = PokedexApp_HandleEntryTabTouch(pokedexApp, touched);
    if (ret != -1) {
        return ret;
    }
    if (System_GetTouchNew() == TRUE) {
        ret = TouchscreenHitbox_FindRectAtTouchNew(ov18_021FB804);
        if (ret == -1) {
            return -1;
        }
        *touched = 1;
        return ov18_021FB760[ret];
    }
    *touched = 0;
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        return 5;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        return 7;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        if (Pokedex_CheckMonCaughtFlag(pokedexApp->args->pokedex, pokedexApp->curSpecies)) {
            return 1;
        }
        return 0;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        return 12;
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        return 11;
    }
    return -1;
}

u32 PokedexApp_HandleFormCompareInput(PokedexAppData *pokedexApp, u32 *touched) {
    int ret;

    if (System_GetTouchNew() == TRUE) {
        ret = TouchscreenHitbox_FindRectAtTouchNew(ov18_021FB7E0);
        if (ret == -1) {
            return -1;
        }
        *touched = 1;
        return ov18_021FB7C0[ret];
    }
    *touched = 0;
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        if (pokedexApp->unk_18C7_0 == 0) {
            return 0;
        }
        return 2;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        if (pokedexApp->unk_18C7_0 == 0) {
            return 1;
        }
        return 3;
    }
    if (gSystem.newKeys & PAD_KEY_LEFT) {
        return 6;
    }
    if (gSystem.newKeys & PAD_KEY_RIGHT) {
        return 7;
    }
    if (gSystem.newKeys & PAD_BUTTON_X) {
        return 4;
    }
    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        return 5;
    }
    return -1;
}

u32 PokedexApp_HandleEntryMenuInput(PokedexAppData *pokedexApp, u32 *touched) {
    int ret = PokedexApp_HandleEntryTabTouch(pokedexApp, touched);
    if (ret != -1) {
        return ret;
    }
    if (System_GetTouchNew() == TRUE) {
        ret = TouchscreenHitbox_FindHitboxAtTouchNew(ov18_021FB630);
        if (ret == -1) {
            return -1;
        }
        *touched = 1;
        return 4;
    }
    *touched = 0;
    if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        return 2;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        return 0;
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        return 3;
    }
    return -1;
}
