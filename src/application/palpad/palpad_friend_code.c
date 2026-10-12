#include "global.h"

#include "constants/sndseq.h"

#include "application/palpad/palpad_internal.h"

#include "ascii_util.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "player_data.h"
#include "sound_playback.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"

// Screen 2 runs the naming screen (friend name) and the number entry (friend code) as nested
// applications; screen 3 shows the player's own friend code.

// number entry application (no public header); only the entered text is read back
typedef struct PalPadCodeEntryArgs {
    u8 unk_00[0x1C];
    String *text;
} PalPadCodeEntryArgs;

extern const OverlayManagerTemplate gOverlayTemplate_CodeEntry;
PalPadCodeEntryArgs *CodeEntryArgs_New(enum HeapID heapId, u32 maxLen, const u32 *params, Options *options, u32 unk4, u32 unk5);
void CodeEntryArgs_Delete(PalPadCodeEntryArgs *args);

// palettes of the return button: normal, pressed
typedef struct PalPadButtonPalettes {
    u8 palettes[2];
} PalPadButtonPalettes;

// the first byte belongs to the previous table
extern const struct {
    u8 unk0;
    PalPadButtonPalettes returnButton;
} _0222ED5C;

extern const TouchscreenHitbox sPalPadFriendCodeHitboxes[];

NamingScreenArgs *PalPadNaming_CreateNamingArgs(PalPadNaming *naming, PalPadShared *shared, enum HeapID heapId);
BOOL PalPadFriendCode_HandleInput(PalPadFriendCode *friendCode, PalPadShared *shared);
void PalPadFriendCode_Show(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx);
void PalPadFriendCode_Hide(PalPadFriendCode *friendCode, PalPadGfx *gfx);
void PalPadFriendCode_HighlightReturn(PalPadFriendCode *friendCode, PalPadGfx *gfx, int highlight);

int PalPadNaming_Main(PalPadNaming *naming, PalPadWork *work, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    u32 codeEntryParams[4];

    switch (shared->subState) {
    case 0: // register a friend: name, then friend code
        naming->namingArgs = PalPadNaming_CreateNamingArgs(naming, shared, heapId);
        codeEntryParams[0] = 4;
        codeEntryParams[1] = 4;
        codeEntryParams[2] = 4;
        codeEntryParams[3] = 0;
        naming->codeEntryArgs = CodeEntryArgs_New(heapId, 12, codeEntryParams, Save_PlayerData_GetOptionsAddr(shared->saveData), 0, 0);
        PalPad_TeardownScreens(work);
        naming->manager = OverlayManager_New(&gOverlayTemplate_NamingScreen, naming->namingArgs, heapId);
        shared->subState = 1;
        break;
    case 1:
        if (OverlayManager_Run(naming->manager)) {
            OverlayManager_Delete(naming->manager);
            if (naming->namingArgs->noInput == FALSE) {
                naming->manager = OverlayManager_New(&gOverlayTemplate_CodeEntry, naming->codeEntryArgs, heapId);
                shared->subState = 2;
            } else {
                PalPadShared_SetNextScreen(shared, PALPAD_SCREEN_MENU, 0);
                shared->subState = 3;
            }
        }
        break;
    case 2:
        if (OverlayManager_Run(naming->manager)) {
            OverlayManager_Delete(naming->manager);
            PalPadShared_SetNameAndCode(shared, naming->namingArgs->nameInputString, ((PalPadCodeEntryArgs *)naming->codeEntryArgs)->text);
            PalPadShared_SetNextScreen(shared, PALPAD_SCREEN_MENU, 7);
            shared->subState = 3;
        }
        break;
    case 3:
        NamingScreen_DeleteArgs(naming->namingArgs);
        CodeEntryArgs_Delete(naming->codeEntryArgs);
        PalPad_SetupScreens(work, heapId);
        return 1;
    case 4: // rewrite a friend's name
        naming->namingArgs = PalPadNaming_CreateNamingArgs(naming, shared, heapId);
        PalPad_TeardownScreens(work);
        naming->manager = OverlayManager_New(&gOverlayTemplate_NamingScreen, naming->namingArgs, heapId);
        shared->subState = 5;
        break;
    case 5:
        if (OverlayManager_Run(naming->manager)) {
            OverlayManager_Delete(naming->manager);
            if (naming->namingArgs->noInput == FALSE) {
                PalPadShared_SetNextScreen(shared, PALPAD_SCREEN_ROSTER, 12);
                PalPadShared_SetNameAndCode(shared, naming->namingArgs->nameInputString, NULL);
                shared->subState = 6;
            } else {
                PalPadShared_SetNextScreen(shared, PALPAD_SCREEN_ROSTER, 13);
                shared->subState = 6;
            }
        }
        break;
    case 6:
        NamingScreen_DeleteArgs(naming->namingArgs);
        PalPad_SetupScreens(work, heapId);
        return 1;
    }
    return 0;
}

void PalPadNaming_Exit(PalPadNaming *naming, PalPadShared *shared, PalPadGfx *gfx) {
    PalPadNaming_Teardown(naming, shared, gfx);
}

void PalPadNaming_Setup(PalPadNaming *naming, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
}

void PalPadNaming_Teardown(PalPadNaming *naming, PalPadShared *shared, PalPadGfx *gfx) {
}

NamingScreenArgs *PalPadNaming_CreateNamingArgs(PalPadNaming *naming, PalPadShared *shared, enum HeapID heapId) {
    Options *options = Save_PlayerData_GetOptionsAddr(shared->saveData);
    NamingScreenArgs *args = NamingScreen_CreateArgs(heapId, NAME_SCREEN_UNK7, 0, 7, options, NULL);

    if (shared->useInitialName == TRUE) {
        if (!sub_02020B78(shared->initialName[0])) {
            CopyU16ArrayToString(args->nameInputString, shared->initialName);
        } else {
            String_SetEmpty(args->nameInputString);
        }
        shared->useInitialName = FALSE;
    }
    return args;
}

void PalPadFriendCode_Init(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    PalPadFriendCode_Setup(friendCode, shared, gfx, heapId);
}

int PalPadFriendCode_Main(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx) {
    switch (shared->subState) {
    case 0:
        PalPadFriendCode_Show(friendCode, shared, gfx);
        shared->subState = 1;
        break;
    case 1:
        if (PalPadFriendCode_HandleInput(friendCode, shared)) {
            PlaySE(SEQ_SE_DP_DECIDE);
            PalPadFriendCode_HighlightReturn(friendCode, gfx, TRUE);
            friendCode->timer = 4;
            shared->subState = 2;
        }
        break;
    case 2:
        friendCode->timer--;
        if (friendCode->timer <= 0) {
            PalPadFriendCode_HighlightReturn(friendCode, gfx, FALSE);
            PalPadShared_SetNextScreen(shared, PALPAD_SCREEN_MENU, 2);
            PalPadFriendCode_Hide(friendCode, gfx);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void PalPadFriendCode_Exit(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx) {
    PalPadFriendCode_Teardown(friendCode, shared, gfx);
}

BOOL PalPadFriendCode_HandleInput(PalPadFriendCode *friendCode, PalPadShared *shared) {
    if (TouchscreenHitbox_FindHitboxAtTouchNew(sPalPadFriendCodeHitboxes) == 0) {
        shared->inputState = TRUE;
        return TRUE;
    }
    PalPadShared_ResetInputStateOnKey(&shared->inputState);
    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

void PalPadFriendCode_Setup(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    String *text, *buffer;
    u64 ownCode;
    u32 x;

    InitWindow(&friendCode->windows[0]);
    AddWindowParameterized(gfx->bgConfig, &friendCode->windows[0], GF_BG_LYR_MAIN_3, 4, 11, 24, 8, 11, 1);
    InitWindow(&friendCode->windows[1]);
    AddWindowParameterized(gfx->bgConfig, &friendCode->windows[1], GF_BG_LYR_MAIN_3, 9, 6, 14, 2, 11, 193);
    InitWindow(&friendCode->windows[2]);
    AddWindowParameterized(gfx->bgConfig, &friendCode->windows[2], GF_BG_LYR_MAIN_3, 21, 21, 9, 2, 11, 221);
    FillWindowPixelBuffer(&friendCode->windows[0], 0);
    FillWindowPixelBuffer(&friendCode->windows[1], 0);
    FillWindowPixelBuffer(&friendCode->windows[2], 0);
    text = String_New(0x80, heapId);
    buffer = String_New(0x80, heapId);
    ownCode = DWC_CreateFriendKey(FriendRoster_GetUserData(Save_FriendRoster_Get(shared->saveData)));
    if (ownCode) {
        ReadMsgDataIntoString(gfx->msgData, 53, text);
        x = (192 - FontID_String_GetWidthMultiline(0, text, 0)) / 2;
        AddTextPrinterParameterizedWithColor(&friendCode->windows[0], 0, text, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        PalPadGfx_BufferFriendCode(gfx, ownCode);
        ReadMsgDataIntoString(gfx->msgData, 52, buffer);
        StringExpandPlaceholders(gfx->msgFormat, text, buffer);
        AddTextPrinterParameterizedWithColor(&friendCode->windows[1], 0, text, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 2, 0), NULL);
    } else { // no friend code yet (never connected)
        ReadMsgDataIntoString(gfx->msgData, 54, text);
        x = (192 - FontID_String_GetWidthMultiline(0, text, 0)) / 2;
        AddTextPrinterParameterizedWithColor(&friendCode->windows[0], 0, text, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    }
    ReadMsgDataIntoString(gfx->msgData, 51, text);
    AddTextPrinterParameterizedWithColor(&friendCode->windows[2], 4, text, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 15, 0), NULL);
    String_Delete(text);
    String_Delete(buffer);
    friendCode->screenFile = GfGfxLoader_GetScrnDataFromOpenNarc(gfx->narc, 14, TRUE, &friendCode->screen, heapId);
}

#pragma force_active reset

void PalPadFriendCode_Teardown(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx) {
    RemoveWindow(&friendCode->windows[0]);
    RemoveWindow(&friendCode->windows[1]);
    RemoveWindow(&friendCode->windows[2]);
    Heap_Free(friendCode->screenFile);
}

void PalPadFriendCode_Show(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx) {
    NNSG2dScreenData *screen;

    BufferPlayersName(gfx->msgFormat, 0, Save_PlayerData_GetProfile(shared->saveData));
    PalPadGfx_PrintTitle(gfx, &gfx->titleWindow, 50);
    screen = friendCode->screen;
    LoadRectToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_2, screen->rawData, 0, 0, screen->screenWidth >> 3, screen->screenHeight >> 3);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
    ScheduleWindowCopyToVram(&friendCode->windows[0]);
    ScheduleWindowCopyToVram(&friendCode->windows[1]);
    ScheduleWindowCopyToVram(&friendCode->windows[2]);
}

void PalPadFriendCode_Hide(PalPadFriendCode *friendCode, PalPadGfx *gfx) {
    PalPadGfx_ClearTitle(gfx);
    FillBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 24, 0);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
    ClearWindowTilemapAndScheduleTransfer(&friendCode->windows[0]);
    ClearWindowTilemapAndScheduleTransfer(&friendCode->windows[1]);
    ClearWindowTilemapAndScheduleTransfer(&friendCode->windows[2]);
    FillBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, 0, 32, 24, 0);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_3);
}

#pragma force_active on

void PalPadFriendCode_HighlightReturn(PalPadFriendCode *friendCode, PalPadGfx *gfx, int highlight) {
    PalPadButtonPalettes button = _0222ED5C.returnButton;

    BgTilemapRectChangePalette(gfx->bgConfig, GF_BG_LYR_MAIN_2, 17, 20, 14, 4, button.palettes[highlight]);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
}

#pragma force_active reset
