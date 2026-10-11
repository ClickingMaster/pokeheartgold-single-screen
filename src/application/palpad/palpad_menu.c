#include "global.h"

#include "constants/sndseq.h"

#include "application/palpad/palpad_internal.h"

#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "options.h"
#include "player_data.h"
#include "render_text.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"

enum PalPadMenuItem {
    PALPAD_MENU_ROSTER,
    PALPAD_MENU_REGISTER,
    PALPAD_MENU_FRIEND_CODE,
    PALPAD_MENU_EXIT,
};

extern const TouchscreenHitbox sPalPadMenuHitboxes[];
extern const u8 sPalPadMenuRegisterErrorMsgIds[];
extern const u8 sPalPadMenuItemScreens[];

void PalPadMenu_Show(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx);
BOOL PalPadMenu_HandleKeys(u32 *selection, u32 *inputState, PalPadGfx *gfx);
BOOL PalPadMenu_HandleTouch(u32 *selection, u32 *inputState, PalPadGfx *gfx, BOOL *touched);
BOOL PalPadMenu_HandleInput(u32 *selection, u32 *inputState, PalPadGfx *gfx);
void PalPadMenu_MoveCursor(u32 *selection, PalPadGfx *gfx);
void PalPadMenu_HighlightSelection(u32 *selection, PalPadGfx *gfx);
void PalPadMenu_ClearHighlight(PalPadMenu *menu, PalPadGfx *gfx);
void PalPadMenu_SetNextScreen(u32 *selection, PalPadShared *shared);
void PalPadMenu_Hide(PalPadMenu *menu, PalPadGfx *gfx);
void PalPadMenu_PrintMessage(PalPadMenu *menu, PalPadGfx *gfx, int msgId, enum HeapID heapId);
BOOL PalPadMenu_IsMessageDone(PalPadMenu *menu, u32 *inputState);
void PalPadMenu_ClearMessage(PalPadMenu *menu);
BOOL PalPadMenu_CanSelect(u32 *selection, PalPadShared *shared);

#pragma force_active on

void PalPadMenu_Init(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    PalPadMenu_Setup(menu, shared, gfx, heapId);
}

#pragma force_active reset

#pragma opt_pointer_analysis off
#pragma force_active on

int PalPadMenu_Main(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    int valid;

    switch (shared->subState) {
    case 0:
        PalPadMenu_Show(menu, shared, gfx);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, (enum FadeType)17, (enum FadeType)17, RGB_BLACK, 6, 1, heapId);
        shared->subState = 1;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            shared->subState = 3;
        }
        break;
    case 2:
        PalPadMenu_Show(menu, shared, gfx);
        shared->subState = 3;
        break;
    case 3:
        // the result goes through the stack variable `valid` in the original
        valid = PalPadMenu_HandleInput(&menu->selection, &shared->inputState, gfx);
        if (valid) {
            menu->timer = 2;
            shared->subState = 4;
        }
        break;
    case 4: {
        u32 timer = menu->timer;
        menu->timer = timer - 1;
        if (timer == 0) {
            menu->timer = 0;
            if (PalPadMenu_CanSelect(&menu->selection, shared) == TRUE) {
                PalPadMenu_SetNextScreen(&menu->selection, shared);
                if (menu->selection == PALPAD_MENU_EXIT || menu->selection == PALPAD_MENU_REGISTER) {
                    shared->subState = 5;
                } else {
                    PalPadMenu_Hide(menu, gfx);
                    return 1;
                }
            } else if (menu->selection == PALPAD_MENU_REGISTER) {
                PalPadMenu_PrintMessage(menu, gfx, 58, heapId); // roster is full
                shared->subState = 11;
            }
        }
    } break;
    case 5:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, (enum FadeType)16, (enum FadeType)16, RGB_BLACK, 6, 1, heapId);
        shared->subState++;
        break;
    case 6:
        if (IsPaletteFadeFinished() == TRUE) {
            return 1;
        }
        break;
    case 7: // back from the name / code entry
        PalPadMenu_Show(menu, shared, gfx);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, (enum FadeType)17, (enum FadeType)17, RGB_BLACK, 6, 1, heapId);
        shared->subState = 8;
        break;
    case 8:
        if (IsPaletteFadeFinished()) {
            PlayerProfile *profile = PlayerProfile_New(heapId);
            u64 friendCode;

            Save_Profile_PlayerName_Set(profile, String_cstr(shared->name));
            BufferPlayersName(gfx->msgFormat, 0, profile);
            Heap_Free(profile);
            friendCode = String_atoi(shared->code, &valid);
            if (valid) {
                PalPadGfx_BufferFriendCode(gfx, friendCode);
                PalPadMenu_PrintMessage(menu, gfx, 55, heapId); // register this friend?
                shared->subState = 9;
            } else {
                PalPadMenu_PrintMessage(menu, gfx, 56, heapId); // invalid friend code
                shared->subState = 11;
            }
        }
        break;
    case 9:
        if (PalPadMenu_IsMessageDone(menu, &shared->inputState)) {
            PalPadGfx_StartYesNo(&shared->inputState, gfx, 0);
            shared->subState = 10;
        }
        break;
    case 10:
        switch (PalPadGfx_HandleYesNo(&shared->inputState, gfx)) {
        case YESNORESPONSE_YES: {
            int result = PalPadShared_RegisterFriendCode(shared, shared->code, shared->name);
            if (result == 0) {
                PalPadMenu_ClearMessage(menu);
                shared->subState = 3;
            } else {
                PalPadMenu_PrintMessage(menu, gfx, sPalPadMenuRegisterErrorMsgIds[result], heapId);
                shared->subState = 11;
            }
            break;
        }
        case YESNORESPONSE_NO:
            PalPadMenu_ClearMessage(menu);
            shared->subState = 3;
            break;
        }
        break;
    case 11:
        if (PalPadMenu_IsMessageDone(menu, &shared->inputState) && ((gSystem.newKeys & PAD_BUTTON_A) || (gSystem.newKeys & PAD_BUTTON_B) || System_GetTouchNew())) {
            PalPadMenu_ClearMessage(menu);
            PalPadMenu_ClearHighlight(menu, gfx);
            shared->subState = 3;
        }
        break;
    }
    return 0;
}

#pragma force_active reset
#pragma opt_pointer_analysis reset

void PalPadMenu_Exit(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx) {
    PalPadMenu_Teardown(menu, shared, gfx);
}

#pragma force_active on

void PalPadMenu_Setup(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    String *text, *buffer;
    int i, y;

    InitWindow(&menu->menuWindow);
    AddWindowParameterized(gfx->bgConfig, &menu->menuWindow, GF_BG_LYR_MAIN_3, 4, 4, 24, 20, 11, 1);
    InitWindow(&menu->msgWindow);
    AddWindowParameterized(gfx->bgConfig, &menu->msgWindow, GF_BG_LYR_MAIN_1, 2, 19, 27, 4, 11, 172);
    FillWindowPixelBuffer(&menu->menuWindow, 0);
    FillWindowPixelBuffer(&menu->msgWindow, 0);
    menu->msgText = String_New(0x80, heapId);
    menu->textSpeed = Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(shared->saveData));
    text = String_New(0x80, heapId);
    buffer = String_New(0x80, heapId);
    BufferPlayersName(gfx->msgFormat, 0, Save_PlayerData_GetProfile(shared->saveData));
    for (i = 0, y = 8; i < 4; i++, y += 40) {
        u32 width;

        ReadMsgDataIntoString(gfx->msgData, i + 1, buffer);
        StringExpandPlaceholders(gfx->msgFormat, text, buffer);
        width = FontID_String_GetWidth(4, text, 0);
        AddTextPrinterParameterizedWithColor(&menu->menuWindow, 4, text, (192 - width) / 2, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 15, 0), NULL);
    }
    String_Delete(text);
    String_Delete(buffer);
    menu->screenFile = GfGfxLoader_GetScrnDataFromOpenNarc(gfx->narc, 11, TRUE, &menu->screen, heapId);
}

#pragma force_active reset

void PalPadMenu_Teardown(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx) {
    String_Delete(menu->msgText);
    RemoveWindow(&menu->menuWindow);
    RemoveWindow(&menu->msgWindow);
    Heap_Free(menu->screenFile);
}

void PalPadMenu_Show(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx) {
    NNSG2dScreenData *screen;

    PalPadGfx_PrintTitle(gfx, &gfx->titleWindow, 0);
    ScheduleWindowCopyToVram(&menu->menuWindow);
    screen = menu->screen;
    LoadRectToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_2, screen->rawData, 0, 0, screen->screenWidth >> 3, screen->screenHeight >> 3);
    ScheduleSetBgPosText(gfx->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
    ScheduleSetBgPosText(gfx->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0);
    PalPadGfx_SetCursor(gfx, TRUE, 0);
    PalPadGfx_SetCursorPriority(gfx, 1, 0);
    PalPadMenu_MoveCursor(&menu->selection, gfx);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
}

BOOL PalPadMenu_HandleKeys(u32 *selection, u32 *inputState, PalPadGfx *gfx) {
    BOOL moved = FALSE;

    if (gSystem.newKeys & PALPAD_KEYS_ANY) {
        *inputState = 0;
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        PalPadMenu_HighlightSelection(selection, gfx);
        PlaySE(SEQ_SE_DP_DECIDE);
        return TRUE;
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_DP_DECIDE);
        *selection = PALPAD_MENU_EXIT;
        return TRUE;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        if (*selection != 0) {
            (*selection)--;
            moved = TRUE;
        } else {
            *selection = PALPAD_MENU_EXIT;
            moved = TRUE;
        }
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        if (*selection < PALPAD_MENU_EXIT) {
            (*selection)++;
            moved = TRUE;
        } else {
            *selection = 0;
            moved = TRUE;
        }
    }
    if (moved == TRUE) {
        PlaySE(SEQ_SE_DP_BUTTON3);
        PalPadMenu_MoveCursor(selection, gfx);
        ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
    }
    return FALSE;
}

BOOL PalPadMenu_HandleTouch(u32 *selection, u32 *inputState, PalPadGfx *gfx, BOOL *touched) {
    int hit = TouchscreenHitbox_FindHitboxAtTouchNew(sPalPadMenuHitboxes);

    if (hit == -1) {
        return FALSE;
    }
    *touched = TRUE;
    *inputState = TRUE;
    *selection = hit;
    PlaySE(SEQ_SE_DP_DECIDE);
    PalPadMenu_HighlightSelection(selection, gfx);
    PalPadMenu_MoveCursor(selection, gfx);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
    return TRUE;
}

BOOL PalPadMenu_HandleInput(u32 *selection, u32 *inputState, PalPadGfx *gfx) {
    BOOL touched = FALSE;
    BOOL result = PalPadMenu_HandleTouch(selection, inputState, gfx, &touched);

    if (touched != FALSE) {
        *inputState = TRUE;
        return result;
    }
    return PalPadMenu_HandleKeys(selection, inputState, gfx);
}

void PalPadMenu_MoveCursor(u32 *selection, PalPadGfx *gfx) {
    Sprite_SetPositionXY(gfx->cursor, 128, *selection * 40 + 48);
}

void PalPadMenu_HighlightSelection(u32 *selection, PalPadGfx *gfx) {
    int i = 0;
    int y = 4;

    while (i < 4) {
        int palette = (*selection == i) ? 9 : 2;
        // called through an int-typed pointer in the original: the palette is not truncated to u8
        ((void (*)(BgConfig *, int, int, int, int, int, int))BgTilemapRectChangePalette)(gfx->bgConfig, GF_BG_LYR_MAIN_2, 2, (u8)y, 28, 4, palette);
        i++;
        y += 5;
    }
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
}

#pragma force_active on

void PalPadMenu_ClearHighlight(PalPadMenu *menu, PalPadGfx *gfx) {
    int i;

    for (i = 0; i < 4; i++) {
        int y = i * 5 + 4;
        BgTilemapRectChangePalette(gfx->bgConfig, GF_BG_LYR_MAIN_2, 2, y, 28, 4, 2);
        ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
    }
}

#pragma force_active reset

void PalPadMenu_SetNextScreen(u32 *selection, PalPadShared *shared) {
    PalPadShared_SetNextScreen(shared, sPalPadMenuItemScreens[*selection], 0);
}

void PalPadMenu_Hide(PalPadMenu *menu, PalPadGfx *gfx) {
    PalPadGfx_ClearTitle(gfx);
    FillBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 24, 0);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
    FillBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, 0, 32, 24, 0);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_3);
    ClearWindowTilemapAndScheduleTransfer(&menu->menuWindow);
    PalPadGfx_SetCursor(gfx, FALSE, 0xFF);
}

void PalPadMenu_PrintMessage(PalPadMenu *menu, PalPadGfx *gfx, int msgId, enum HeapID heapId) {
    String *buffer = String_New(0x80, heapId);

    FillWindowPixelBuffer(&menu->msgWindow, 15);
    ReadMsgDataIntoString(gfx->msgData, msgId, buffer);
    StringExpandPlaceholders(gfx->msgFormat, menu->msgText, buffer);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    menu->printerId = AddTextPrinterParameterizedWithColor(&menu->msgWindow, 1, menu->msgText, 0, 0, menu->textSpeed, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    DrawFrameAndWindow2(&menu->msgWindow, TRUE, 70, 12);
    ScheduleWindowCopyToVram(&menu->msgWindow);
    String_Delete(buffer);
}

BOOL PalPadMenu_IsMessageDone(PalPadMenu *menu, u32 *inputState) {
    if (!TextPrinterCheckActive(menu->printerId)) {
        TextFlags_SetCanTouchSpeedUpPrint(FALSE);
        *inputState = TextFlags_GetIsTouchSpeedingUpPrint();
        return TRUE;
    }
    return FALSE;
}

void PalPadMenu_ClearMessage(PalPadMenu *menu) {
    ClearFrameAndWindow2(&menu->msgWindow, TRUE);
    ClearWindowTilemapAndScheduleTransfer(&menu->msgWindow);
}

#pragma force_active on

// Registering needs a free roster slot.
BOOL PalPadMenu_CanSelect(u32 *selection, PalPadShared *shared) {
    int i;
    UnkStruct_021D2230 *wifiList;

    if (*selection == PALPAD_MENU_REGISTER) {
        wifiList = sub_0202C6F4(shared->saveData);
        for (i = 0; i < PALPAD_FRIEND_MAX; i++) {
            if (sub_0202C2DC(wifiList, i) == 0) {
                return TRUE;
            }
        }
    } else {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset
