#include "global.h"

#include "constants/sndseq.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "gts_internal.h"
#include "msgdata.h"
#include "options.h"
#include "player_data.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0203A3B0.h"
#include "yes_no_prompt.h"

// The GTS main menu (screen 1): DEPOSIT POKEMON (SUMMARY once a Pokemon is deposited), SEEK POKEMON, EXIT.

typedef enum GtsMainMenuState {
    GTS_MAIN_MENU_STATE_WAIT_ENTER_ANIM,
    GTS_MAIN_MENU_STATE_1,
    GTS_MAIN_MENU_STATE_2,
    GTS_MAIN_MENU_STATE_WAIT_FADE_IN,
    GTS_MAIN_MENU_STATE_WAIT_FADE_IN_2,
    GTS_MAIN_MENU_STATE_SHOW_PROMPT,
    GTS_MAIN_MENU_STATE_HANDLE_INPUT,
    GTS_MAIN_MENU_STATE_START_EXIT_ANIM,
    GTS_MAIN_MENU_STATE_WAIT_EXIT_ANIM,
    GTS_MAIN_MENU_STATE_FADE_OUT,
    GTS_MAIN_MENU_STATE_WAIT_TEXT,
    GTS_MAIN_MENU_STATE_WAIT_TEXT_AND_PAUSE,
    GTS_MAIN_MENU_STATE_SHOW_EXIT_PROMPT,
    GTS_MAIN_MENU_STATE_HANDLE_EXIT_PROMPT,
} GtsMainMenuState;

// Sets the level of the wireless status icon (wireless_status_icon_2.c); not declared in a public header.
void sub_0203A930(s32 level);

extern int (*const sGtsMainMenuStates[])(GtsApp *app);
extern const GraphicsModes sGtsMainMenuGraphicsModes;
extern const BgTemplate sGtsMainMenuBg0Template;
extern const BgTemplate sGtsMainMenuBg1Template;
extern const BgTemplate sGtsMainMenuBg2Template;
extern const BgTemplate sGtsSubBg0Template;
extern const BgTemplate sGtsSubBg1Template;
extern const BgTemplate sGtsSubBg2Template;
extern const BgTemplate sGtsSubBg3Template;
// The cursor positions {x, y} of the three buttons; the asm labels the y column separately.
extern const u16 sGtsMainMenuCursorX[];
extern const u16 sGtsMainMenuCursorY[];
extern const TouchscreenHitbox sGtsMainMenuButtonHitboxes[];
// The button labels, [hasDepositedMon][button].
extern const u32 sGtsMainMenuButtonMsgs[2][3];

int GtsMainMenu_Init(GtsApp *app, int state);
int GtsMainMenu_Main(GtsApp *app, int state);
BOOL GtsMainMenu_Exit(GtsApp *app, int state);
void GtsMainMenu_InitBgs(BgConfig *bgConfig);
void GtsMainMenu_FreeBgs(BgConfig *bgConfig);
void GtsMainMenu_HideMainScreen(GtsApp *app);
void GtsMainMenu_LoadGraphics(GtsApp *app);
void GtsMainMenu_CreateCursor(GtsApp *app);
void GtsMainMenu_DeleteCursor(GtsApp *app);
void GtsMainMenu_CreateWindows(GtsApp *app);
void GtsMainMenu_RemoveWindows(GtsApp *app);
void GtsMainMenu_CreateStrings(GtsApp *app);
void GtsMainMenu_DeleteStrings(GtsApp *app);
int GtsMainMenu_GetTouchedButton(void);
void GtsMainMenu_SelectButton(GtsApp *app, int button);
void GtsMainMenu_DrawButtons(GtsApp *app);
void GtsMainMenu_PrintMessage(GtsApp *app, int msgId, int textSpeed, int unused3, u32 unused4);
void GtsMainMenu_PrintPrompt(GtsApp *app, int msgId, int textSpeed, int unused3, u32 unused4);
void GtsMainMenu_PrintButtonLabel(Window *window, MsgData *msgData, int fontId, int msgId, u32 unused);
int GtsMainMenu_WaitEnterAnim(GtsApp *app);
int ov70_02239698(GtsApp *app);
void ov70_022396A8(GtsApp *app);
int GtsMainMenu_WaitFadeIn(GtsApp *app);
int GtsMainMenu_WaitFadeIn2(GtsApp *app);
int GtsMainMenu_ShowPrompt(GtsApp *app);
int GtsMainMenu_HandleInput(GtsApp *app);
int GtsMainMenu_StartExitAnim(GtsApp *app);
int GtsMainMenu_WaitExitAnim(GtsApp *app);
int GtsMainMenu_FadeOut(GtsApp *app);
int GtsMainMenu_ShowExitPrompt(GtsApp *app);
int GtsMainMenu_HandleExitPrompt(GtsApp *app);
int GtsMainMenu_WaitText(GtsApp *app);
int GtsMainMenu_WaitTextAndPause(GtsApp *app);

int GtsMainMenu_Init(GtsApp *app, int state) {
    GtsMainMenu_CreateStrings(app);
    reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
    GtsMainMenu_InitBgs(app->bgConfig);
    GtsMainMenu_LoadGraphics(app);
    GtsMainMenu_CreateWindows(app);
    GtsMainMenu_CreateCursor(app);
    Gts_CreateWirelessStatusIcon(app);
    GtsMainMenu_DrawButtons(app);
    Sound_SetSceneAndPlayBGM(0x17, SEQ_GS_BLD_GTC, 1);
    if (app->mainMenuShown == FALSE) {
        GtsMainMenu_HideMainScreen(app);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_61);
        app->state = GTS_MAIN_MENU_STATE_WAIT_ENTER_ANIM;
        app->mainMenuShown = TRUE;
        Gts_StartPlayerEnterAnim(app, PlayerProfile_GetTrainerGender(app->args->profile));
    } else {
        if (app->unk_11FC == 1 && GXS_GetMasterBrightness() != 0) {
            Gts_ResetAvatarPositions(app);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_61);
        } else {
            BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_61);
        }
        app->state = GTS_MAIN_MENU_STATE_SHOW_PROMPT;
    }
    app->unk_11FC = 0;
    return 2;
}

int GtsMainMenu_Main(GtsApp *app, int state) {
    sub_0203A930(Gts_GetWifiIconLevel());
    return sGtsMainMenuStates[app->state](app);
}

BOOL GtsMainMenu_Exit(GtsApp *app, int state) {
    WirelessStatusIcon_Destroy();
    GtsMainMenu_DeleteCursor(app);
    GtsMainMenu_DeleteStrings(app);
    GtsMainMenu_RemoveWindows(app);
    GtsMainMenu_FreeBgs(app->bgConfig);
    Sprite_SetDrawFlag(app->panelSprite, FALSE);
    Gts_SwitchScreen(app);
    return TRUE;
}

void GtsMainMenu_InitBgs(BgConfig *bgConfig) {
    GraphicsModes modes;
    BgTemplate bg0;
    BgTemplate bg1;
    BgTemplate bg2;

    modes = sGtsMainMenuGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    bg0 = sGtsMainMenuBg0Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &bg0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    bg1 = sGtsMainMenuBg1Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &bg1, GF_BG_TYPE_TEXT);
    bg2 = sGtsMainMenuBg2Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &bg2, GF_BG_TYPE_TEXT);
    Gts_InitSubScreenBgs(bgConfig, 0, FALSE);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_61);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

// The sub screen backgrounds, shared by the screens; sub BG 1 is placed at y.
void Gts_InitSubScreenBgs(BgConfig *bgConfig, int bg1Y, BOOL keepBg2Tilemap) {
    BgTemplate bg0;
    BgTemplate bg2;
    BgTemplate bg1;
    BgTemplate bg3;

    bg0 = sGtsSubBg0Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &bg0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    bg2 = sGtsSubBg2Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &bg2, GF_BG_TYPE_TEXT);
    if (!keepBg2Tilemap) {
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    }
    bg1 = sGtsSubBg1Template;
    bg1.y = bg1Y;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &bg1, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_61);
    bg3 = sGtsSubBg3Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &bg3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
}

void Gts_FreeSubScreenBgs(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_3);
}

void GtsMainMenu_FreeBgs(BgConfig *bgConfig) {
    Gts_FreeSubScreenBgs(bgConfig);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
}

void GtsMainMenu_HideMainScreen(GtsApp *app) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    Sprite_SetDrawFlag(app->cursorSprite, FALSE);
}

void GtsMainMenu_LoadGraphics(GtsApp *app) {
    BgConfig *bgConfig = app->bgConfig;
    NARC *narc = NARC_New(NARC_a_1_0_0, HEAP_ID_61);

    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x60, HEAP_ID_61);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_61);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 1, 14, Options_GetFrame(app->args->options), HEAP_ID_61);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 0x1F, 11, 0, HEAP_ID_61);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 16, bgConfig, GF_BG_LYR_MAIN_1, 0, 0xC00, TRUE, HEAP_ID_61);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 39, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, TRUE, HEAP_ID_61);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 38, bgConfig, GF_BG_LYR_MAIN_2, 0, 0x600, TRUE, HEAP_ID_61);
    Gts_LoadSubScreenBg(app);
    if (app->mainMenuShown) {
        Gts_ShowSubScreenPanel(app);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    } else {
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    }
    NARC_Delete(narc);
}

void GtsMainMenu_CreateCursor(GtsApp *app) {
    SpriteTemplate template;
    SpriteResourcesHeader header;

    CreateSpriteResourcesHeader(&header, 0, 0, 0, 0, -1, -1, 0, 1, app->resourceManagers[GF_GFX_RES_TYPE_CHAR], app->resourceManagers[GF_GFX_RES_TYPE_PLTT], app->resourceManagers[GF_GFX_RES_TYPE_CELL], app->resourceManagers[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    Gts_InitSpriteTemplate(&template, app, &header, NNS_G2D_VRAM_TYPE_2DMAIN);
    template.position.x = sGtsMainMenuCursorX[app->menuCursor * 2] << FX32_SHIFT;
    template.position.y = sGtsMainMenuCursorY[app->menuCursor * 2] << FX32_SHIFT;
    app->cursorSprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(app->cursorSprite, TRUE);
    Sprite_SetAnimCtrlSeq(app->cursorSprite, 43);
}

void GtsMainMenu_DeleteCursor(GtsApp *app) {
    Sprite_Delete(app->cursorSprite);
}

void GtsMainMenu_CreateWindows(GtsApp *app) {
    int i;
    int baseTile;
    int y;
    Window *window;

    AddWindowParameterized(app->bgConfig, &app->titleWindow, GF_BG_LYR_MAIN_1, 2, 1, 28, 2, 13, 0x28);
    FillWindowPixelBuffer(&app->titleWindow, 0);
    AddTextPrinterParameterizedWithColor(&app->titleWindow, 1, app->titleString, 0, 2, 0, MAKE_TEXT_COLOR(15, 6, 0), NULL);

    i = 0;
    baseTile = 0x61;
    y = 6;
    window = app->windows; // the three buttons
    for (; i < 3; i++) {
        AddWindowParameterized(app->bgConfig, window, GF_BG_LYR_MAIN_1, 9, (u8)y, 15, 2, 1, (u16)baseTile);
        FillWindowPixelBuffer(window, 0);
        baseTile += 30;
        y += 5;
        window++;
    }

    AddWindowParameterized(app->bgConfig, &app->waitingIconWindow, GF_BG_LYR_MAIN_0, 2, 21, 27, 2, 13, 0x60);
    FillWindowPixelBuffer(&app->waitingIconWindow, 15);
    AddWindowParameterized(app->bgConfig, &app->promptWindow, GF_BG_LYR_MAIN_0, 2, 19, 27, 4, 13, 0x96);
    FillWindowPixelBuffer(&app->promptWindow, 15);
    Gts_CreatePanelWindow(app, 0);
}

void GtsMainMenu_RemoveWindows(GtsApp *app) {
    int i;
    Window *window;

    RemoveWindow(&app->panelWindow);
    RemoveWindow(&app->promptWindow);
    RemoveWindow(&app->waitingIconWindow);
    window = app->windows;
    for (i = 0; i < 3; i++) {
        RemoveWindow(window);
        window++;
    }
    RemoveWindow(&app->titleWindow);
}

void GtsMainMenu_CreateStrings(GtsApp *app) {
    app->msgBuffer = String_New(180, HEAP_ID_61);
    app->titleString = NewString_ReadMsgData(app->gtsMsgData, 39); // GLOBAL TRADE SYSTEM
}

void GtsMainMenu_DeleteStrings(GtsApp *app) {
    String_Delete(app->msgBuffer);
    String_Delete(app->titleString);
}

#pragma force_active on

// Waits for the player to walk in, then checks the GTS's status (screen 7).
int GtsMainMenu_WaitEnterAnim(GtsApp *app) {
    if (app->playerAnimDone) {
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        Gts_SetNextScreen(app, 7, 11);
        app->unk_001C = 1;
        app->state = GTS_MAIN_MENU_STATE_FADE_OUT;
    }
    return 3;
}

#pragma force_active reset

int ov70_02239698(GtsApp *app) {
    Gts_SetState(app, GTS_MAIN_MENU_STATE_WAIT_TEXT, GTS_MAIN_MENU_STATE_2);
    return 3;
}

#pragma force_active on

// Returns nothing although it is a state function (the state table ignores it: Gts_Main state 3).
void ov70_022396A8(GtsApp *app) {
    BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_61);
    app->state = GTS_MAIN_MENU_STATE_WAIT_FADE_IN;
}

#pragma force_active reset

int GtsMainMenu_WaitFadeIn(GtsApp *app) {
    if (IsPaletteFadeFinished()) {
        app->state = GTS_MAIN_MENU_STATE_SHOW_PROMPT;
    }
    return 3;
}

int GtsMainMenu_WaitFadeIn2(GtsApp *app) {
    if (IsPaletteFadeFinished()) {
        app->state = GTS_MAIN_MENU_STATE_SHOW_PROMPT;
    }
    return 3;
}

int GtsMainMenu_ShowPrompt(GtsApp *app) {
    GtsMainMenu_PrintMessage(app, 4, 1, 0, 0xF0F); // Are you seeking or offering a Pokemon?
    Gts_SetState(app, GTS_MAIN_MENU_STATE_WAIT_TEXT, GTS_MAIN_MENU_STATE_HANDLE_INPUT);
    Sprite_SetAnimActiveFlag(app->cursorSprite, TRUE);
    Gts_StartCountingBoxMons(app);
    return 3;
}

int GtsMainMenu_GetTouchedButton(void) {
    return TouchscreenHitbox_FindRectAtTouchNew(sGtsMainMenuButtonHitboxes);
}

void GtsMainMenu_SelectButton(GtsApp *app, int button) {
    switch (button) {
    case 0:
        if (app->hasDepositedMon == FALSE) {
            Gts_SetNextScreen(app, 5, 5);
            app->state = GTS_MAIN_MENU_STATE_FADE_OUT;
            PlaySE(SEQ_SE_DP_SELECT);
        } else if (app->timer == 0) {
            Gts_SetNextScreen(app, 7, 11);
            app->unk_001C = 2;
            app->state = GTS_MAIN_MENU_STATE_FADE_OUT;
            app->timer = 30 * 60;
            PlaySE(SEQ_SE_DP_SELECT);
        } else {
            Sprite_SetAnimActiveFlag(app->cursorSprite, FALSE);
            GtsMainMenu_PrintMessage(app, 34, 1, 0, 0xF0F); // Please wait a while.
            Gts_SetState(app, GTS_MAIN_MENU_STATE_WAIT_TEXT_AND_PAUSE, GTS_MAIN_MENU_STATE_SHOW_PROMPT);
            PlaySE(SEQ_SE_DP_BOX03);
            app->waitFrames = 0;
        }
        break;
    case 1:
        app->hasSearchResults = 0;
        Gts_SetNextScreen(app, 4, 13);
        app->state = GTS_MAIN_MENU_STATE_FADE_OUT;
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    case 2:
        GtsMainMenu_PrintPrompt(app, 7, Gts_GetTextFrameDelay(app), 0, 0xF0F); // Disconnect?
        Gts_SetState(app, GTS_MAIN_MENU_STATE_WAIT_TEXT, GTS_MAIN_MENU_STATE_SHOW_EXIT_PROMPT);
        Sprite_SetAnimActiveFlag(app->cursorSprite, FALSE);
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    }
}

int GtsMainMenu_HandleInput(GtsApp *app) {
    int button = GtsMainMenu_GetTouchedButton();
    int cursor;

    if (button != -1) {
        app->menuCursor = button;
        PlaySE(SEQ_SE_DP_SELECT);
        cursor = app->menuCursor;
        Gts_SetSpritePositionRounded(app->cursorSprite, sGtsMainMenuCursorX[cursor * 2], sGtsMainMenuCursorY[cursor * 2]);
        GtsMainMenu_SelectButton(app, button);
        return 3;
    }

    if (gSystem.newKeys & PAD_BUTTON_B) {
        GtsMainMenu_PrintPrompt(app, 7, Gts_GetTextFrameDelay(app), 0, 0xF0F); // Disconnect?
        Gts_SetState(app, GTS_MAIN_MENU_STATE_WAIT_TEXT, GTS_MAIN_MENU_STATE_SHOW_EXIT_PROMPT);
        Sprite_SetAnimActiveFlag(app->cursorSprite, FALSE);
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        GtsMainMenu_SelectButton(app, app->menuCursor);
    } else if (gSystem.newKeys & PAD_KEY_UP) {
        if (app->menuCursor != 0) {
            app->menuCursor -= 1;
            PlaySE(SEQ_SE_DP_SELECT);
            button = app->menuCursor;
            Gts_SetSpritePositionRounded(app->cursorSprite, sGtsMainMenuCursorX[button * 2], sGtsMainMenuCursorY[button * 2]);
        }
    } else if ((gSystem.newKeys & PAD_KEY_DOWN) && app->menuCursor < 2) {
        app->menuCursor += 1;
        PlaySE(SEQ_SE_DP_SELECT);
        button = app->menuCursor;
        Gts_SetSpritePositionRounded(app->cursorSprite, sGtsMainMenuCursorX[button * 2], sGtsMainMenuCursorY[button * 2]);
    }

    return 3;
}

int GtsMainMenu_StartExitAnim(GtsApp *app) {
    int gender = PlayerProfile_GetTrainerGender(app->args->profile);
    Gts_StartPlayerExitAnim(app, gender);
    app->state = GTS_MAIN_MENU_STATE_WAIT_EXIT_ANIM;
    app->playerAnimDone = FALSE;
    return 3;
}

int GtsMainMenu_WaitExitAnim(GtsApp *app) {
    if (app->playerAnimDone) {
        app->state = GTS_MAIN_MENU_STATE_FADE_OUT;
    }
    return 3;
}

int GtsMainMenu_FadeOut(GtsApp *app) {
    if (app->nextScreen == 0) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_61);
    } else {
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_61);
    }
    app->state = GTS_MAIN_MENU_STATE_WAIT_ENTER_ANIM;
    return 4;
}

int GtsMainMenu_ShowExitPrompt(GtsApp *app) {
    app->yesNoPrompt = Gts_CreateYesNoPrompt(app->bgConfig, 10, 258, 3, 1);
    app->state = GTS_MAIN_MENU_STATE_HANDLE_EXIT_PROMPT;
    Sprite_SetDrawFlag(app->cursorSprite, FALSE);
    return 3;
}

int GtsMainMenu_HandleExitPrompt(GtsApp *app) {
    int response = Gts_HandleYesNoPrompt(app);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        ClearFrameAndWindow2(&app->promptWindow, TRUE);
        ClearWindowTilemapAndCopyToVram(&app->promptWindow);
        ClearWindowTilemapAndCopyToVram(&app->panelWindow);
        ToggleBgLayer(GF_BG_LYR_SUB_2, GF_PLANE_TOGGLE_OFF);
        Sprite_SetDrawFlag(app->panelSprite, FALSE);
        Gts_SetNextScreen(app, 0, 0);
        app->state = GTS_MAIN_MENU_STATE_START_EXIT_ANIM;
    } else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        ClearFrameAndWindow2(&app->promptWindow, TRUE);
        ClearWindowTilemapAndCopyToVram(&app->promptWindow);
        Sprite_SetAnimActiveFlag(app->cursorSprite, TRUE);
        app->state = GTS_MAIN_MENU_STATE_SHOW_PROMPT;
        Sprite_SetDrawFlag(app->cursorSprite, TRUE);
    }
    return 3;
}

int GtsMainMenu_WaitText(GtsApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        app->state = app->nextState;
    }
    return 3;
}

int GtsMainMenu_WaitTextAndPause(GtsApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        app->waitFrames++;
        if (app->waitFrames > 45) {
            app->waitFrames = 0;
            app->state = app->nextState;
        }
    }
    return 3;
}

void GtsMainMenu_DrawButtons(GtsApp *app) {
    Window *window = app->windows;
    const u32 *msgIds = sGtsMainMenuButtonMsgs[app->hasDepositedMon];
    int i;

    for (i = 0; i < 3; i++) {
        FillWindowPixelRect(window, 15, 0, 0, 120, 8);
        FillWindowPixelRect(window, 14, 0, 8, 120, 2);
        FillWindowPixelRect(window, 13, 0, 10, 120, 6);
        GtsMainMenu_PrintButtonLabel(window, app->gtsMsgData, 4, msgIds[i], 0);
        CopyWindowToVram(window);
        window++;
    }
}

void GtsMainMenu_PrintMessage(GtsApp *app, int msgId, int textSpeed, int unused3, u32 unused4) {
    ReadMsgDataIntoString(app->gtsMsgData, msgId, app->msgBuffer);
    FillWindowPixelBuffer(&app->waitingIconWindow, 15);
    DrawFrameAndWindow2(&app->waitingIconWindow, FALSE, 1, 14);
    app->textPrinterId = AddTextPrinterParameterized(&app->waitingIconWindow, 1, app->msgBuffer, 0, 0, textSpeed, NULL);
}

void GtsMainMenu_PrintPrompt(GtsApp *app, int msgId, int textSpeed, int unused3, u32 unused4) {
    ReadMsgDataIntoString(app->gtsMsgData, msgId, app->msgBuffer);
    FillWindowPixelBuffer(&app->promptWindow, 15);
    DrawFrameAndWindow2(&app->promptWindow, FALSE, 1, 14);
    app->textPrinterId = AddTextPrinterParameterized(&app->promptWindow, 1, app->msgBuffer, 0, 0, textSpeed, NULL);
}

void GtsMainMenu_PrintButtonLabel(Window *window, MsgData *msgData, int fontId, int msgId, u32 unused) {
    String *string = NewString_ReadMsgData(msgData, msgId);
    // The font id is passed without truncation to u8: the callee takes it as u32 (see the FontID TODO).
    ((u8(*)(Window *, u32, String *, u32, u32, u32, u32, PrinterCallback_t))AddTextPrinterParameterizedWithColor)(window, fontId, string, 0, 0, 0, MAKE_TEXT_COLOR(10, 9, 0), NULL);
    String_Delete(string);
}
