#include "global.h"

#include "constants/sndseq.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "gts_internal.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "pokemon.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_0203A3B0.h"
#include "yes_no_prompt.h"

// A search result (screen 3): the Pokemon, its trainer and where they live, the wanted Pokemon; left/right (or
// touching the trainers) switches between the results, TRADE asks to trade with the trainer (screen 5 picks the
// Pokemon to offer), CANCEL scrolls back up to the search screen (4).

typedef enum GtsSearchResultState {
    GTS_SEARCH_RESULT_STATE_START,
    GTS_SEARCH_RESULT_STATE_HANDLE_INPUT,
    GTS_SEARCH_RESULT_STATE_FADE_OUT,
    GTS_SEARCH_RESULT_STATE_WAIT_TEXT,
    GTS_SEARCH_RESULT_STATE_SHOW_TRADE_PROMPT,
    GTS_SEARCH_RESULT_STATE_HANDLE_TRADE_PROMPT,
    GTS_SEARCH_RESULT_STATE_REDRAW,
    GTS_SEARCH_RESULT_STATE_SCROLL_IN,
    GTS_SEARCH_RESULT_STATE_SCROLL_OUT,
} GtsSearchResultState;

typedef struct GtsWindowTemplate {
    u32 x;
    u32 y;
    u32 width;
    u32 height;
    u32 bgId;
} GtsWindowTemplate;

extern int (*const sGtsSearchResultStates[])(GtsApp *app);
extern const BgTemplate sGtsSearchResultBg0Template;
extern const BgTemplate sGtsSearchResultBg1Template;
extern const GtsWindowTemplate sGtsSearchResultInfoWindows[16];
// The results to the left and to the right of each result ({left, right} pairs; the asm labels both columns).
extern const u8 sGtsSearchResultLeft[];
extern const u8 sGtsSearchResultRight[];
extern const TouchscreenHitbox sGtsSearchResultButtonHitboxes[];

int GtsSearchResult_Init(GtsApp *app, int state);
int GtsSearchResult_Main(GtsApp *app, int state);
int GtsSearchResult_Exit(GtsApp *app, int state);
void GtsSearchResult_InitBgs(BgConfig *bgConfig, int subBg1Y);
void GtsSearchResult_FreeBgs(BgConfig *bgConfig);
void GtsSearchResult_LoadGraphics(GtsApp *app);
void GtsSearchResult_CreateMonSprite(GtsApp *app);
void GtsSearchResult_DeleteMonSprite(GtsApp *app);
void GtsSearchResult_CreateWindows(GtsApp *app);
void GtsSearchResult_RemoveWindows(GtsApp *app);
void GtsSearchResult_CreateStrings(GtsApp *app);
void GtsSearchResult_DeleteStrings(GtsApp *app);
int GtsSearchResult_Start(GtsApp *app);
void GtsSearchResult_Select(GtsApp *app, int result);
int GtsSearchResult_HandleInput(GtsApp *app);
int GtsSearchResult_FadeOut(GtsApp *app);
int GtsSearchResult_ShowTradePrompt(GtsApp *app);
int GtsSearchResult_HandleTradePrompt(GtsApp *app);
void GtsSearchResult_DrawDetails(GtsApp *app);
int GtsSearchResult_Redraw(GtsApp *app);
int GtsSearchResult_ScrollIn(GtsApp *app);
int GtsSearchResult_ScrollOut(GtsApp *app);
int GtsSearchResult_WaitText(GtsApp *app);
void GtsSearchResult_PrintMessage(GtsApp *app, int msgId, int textSpeed, int unused3, u32 unused4);
void GtsSearchResult_PrintLabel(MsgData *msgData, Window *window, int msgId);
void GtsSearchResult_PrintButtonLabel(MsgData *msgData, Window *window, int msgId);
void GtsSearchResult_DrawWanted(Window *window, MsgData *msgData, MsgData *speciesNames, GtsWantedMon *wanted);
void GtsSearchResult_DrawLocation(Window *windows, String *country, String *city);
void GtsSearchResult_ScrollBgs(GtsApp *app);

#pragma force_active on

int GtsSearchResult_Init(GtsApp *app, int state) {
    GtsSearchResult_CreateStrings(app);
    GtsSearchResult_InitBgs(app->bgConfig, -32 - app->scrollY);
    GtsSearchResult_LoadGraphics(app);
    GtsSearchResult_CreateWindows(app);
    GtsSearchResult_CreateMonSprite(app);
    Gts_DrawMonSummary(app->gtsMsgData, app->speciesNames, app->msgFormat, app->infoWindows, Mon_GetBoxMon(&app->searchResults[app->selectedResult].mon), &app->searchResults[app->selectedResult].summary);
    Gts_DrawTrainerInfo(app->gtsMsgData, &app->infoWindows[7], app->searchResults[app->selectedResult].trainerName, &app->searchResults[app->selectedResult].mon, &app->infoWindows[12]);
    Gts_LoadMonPicture(&app->searchResults[app->selectedResult].mon);
    GtsSearchResult_PrintLabel(app->gtsMsgData, &app->infoWindows[14], 77);   // WANTED
    GtsSearchResult_PrintLabel(app->gtsMsgData, &app->infoWindows[9], 81);    // OT'S LOCATION
    GtsSearchResult_PrintButtonLabel(app->gtsMsgData, &app->windows[0], 88);  // TRADE
    GtsSearchResult_PrintButtonLabel(app->gtsMsgData, &app->windows[1], 109); // CANCEL
    GtsSearchResult_DrawDetails(app);
    Gts_SetAvatarMarker(app, app->selectedResult, -app->scrollY);
    app->vblankFunc = GtsSearchResult_ScrollBgs;
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    if (app->nextScreenArg == 17) {
        if (GX_GetMasterBrightness() == -16 && GXS_GetMasterBrightness() != -16) {
            BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_61);
        } else if (GX_GetMasterBrightness() != -16 && GXS_GetMasterBrightness() == -16) {
            BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_61);
        } else {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_61);
        }
    }
    app->state = GTS_SEARCH_RESULT_STATE_START;
    return 2;
}

#pragma force_active reset

int GtsSearchResult_Main(GtsApp *app, int state) {
    int ret = sGtsSearchResultStates[app->state](app);
    int i;

    for (i = 0; i < 8; i++) {
        Gts_SetSpritePosition(app->avatarSprites[i], app->avatarPositions[i][0], app->scrollY + app->avatarPositions[i][1] + 32);
    }
    {
        Sprite *sprite = app->monSprite;
        Gts_SetSpritePosition(sprite, 208, 58 - app->scrollY);
    }
    Gts_SetAvatarMarker(app, app->selectedResult, app->scrollY);
    return ret;
}

int GtsSearchResult_Exit(GtsApp *app, int state) {
    app->vblankFunc = NULL;
    GtsSearchResult_DeleteMonSprite(app);
    GtsSearchResult_DeleteStrings(app);
    GtsSearchResult_RemoveWindows(app);
    GtsSearchResult_FreeBgs(app->bgConfig);
    Sprite_SetDrawFlag(app->panelSprite, FALSE);
    Gts_SwitchScreen(app);
    return 1;
}

void GtsSearchResult_InitBgs(BgConfig *bgConfig, int subBg1Y) {
    BgTemplate bg0;
    BgTemplate bg1;

    bg0 = sGtsSearchResultBg0Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &bg0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    bg1 = sGtsSearchResultBg1Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &bg1, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_61);
    if (GXS_GetMasterBrightness() == 0) {
        Gts_InitSubScreenBgs(bgConfig, subBg1Y, TRUE);
    } else {
        Gts_InitSubScreenBgs(bgConfig, subBg1Y, FALSE);
    }
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
}

void GtsSearchResult_FreeBgs(BgConfig *bgConfig) {
    Gts_FreeSubScreenBgs(bgConfig);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
}

#pragma force_active on

void GtsSearchResult_LoadGraphics(GtsApp *app) {
    BgConfig *bgConfig = app->bgConfig;

    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_61);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_SUB_0, 1, 2, Options_GetFrame(app->args->options), HEAP_ID_61);
    GfGfxLoader_LoadCharData(NARC_a_1_0_0, 20, bgConfig, GF_BG_LYR_MAIN_1, 0, 0xC00, TRUE, HEAP_ID_61);
    GfGfxLoader_GXLoadPal(NARC_a_1_0_0, 8, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x60, HEAP_ID_61);
    GfGfxLoader_LoadCharData(NARC_a_1_0_0, 18, bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_61);
    GfGfxLoader_LoadScrnData(NARC_a_1_0_0, 42, bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_61);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_1_OFFSET, HEAP_ID_61);
}

void GtsSearchResult_CreateMonSprite(GtsApp *app) {
    SpriteTemplate template;

    Gts_InitSpriteTemplate(&template, app, &app->headers[0], NNS_G2D_VRAM_TYPE_2DMAIN);
    template.position.x = 208 * FX32_ONE;
    template.position.y = 58 * FX32_ONE;
    app->monSprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(app->monSprite, TRUE);
    Sprite_SetAnimCtrlSeq(app->monSprite, 37);
    WirelessStatusIcon_Create();
}

#pragma force_active reset

void GtsSearchResult_DeleteMonSprite(GtsApp *app) {
    Sprite_Delete(app->monSprite);
}

#pragma force_active on

// The message window and the TRADE / CANCEL buttons on the sub screen, the info windows on both screens.
void GtsSearchResult_CreateWindows(GtsApp *app) {
    int i;
    int mainBaseTile;
    int subBaseTile;
    const GtsWindowTemplate *template;
    Window *window;

    AddWindowParameterized(app->bgConfig, &app->waitingIconWindow, GF_BG_LYR_SUB_0, 2, 19, 27, 4, 1, 40);
    FillWindowPixelBuffer(&app->waitingIconWindow, 0);
    AddWindowParameterized(app->bgConfig, &app->windows[0], GF_BG_LYR_SUB_0, 1, 21, 13, 2, 1, 148);
    FillWindowPixelBuffer(&app->windows[0], 0);
    AddWindowParameterized(app->bgConfig, &app->windows[1], GF_BG_LYR_SUB_0, 17, 21, 13, 2, 1, 174);
    FillWindowPixelBuffer(&app->windows[1], 0);
    mainBaseTile = 1;
    i = 0;
    template = sGtsSearchResultInfoWindows;
    subBaseTile = 200;
    window = app->infoWindows;
    for (; i < 16; i++, template ++, window++) {
        if (template->bgId == GF_BG_LYR_MAIN_0) {
            AddWindowParameterized(app->bgConfig, window, (u8) template->bgId, (u8) template->x, (u8) template->y, (u8) template->width, (u8) template->height, 13, (u16)mainBaseTile);
            mainBaseTile += template->width * template->height;
        } else {
            AddWindowParameterized(app->bgConfig, window, (u8) template->bgId, (u8) template->x, (u8) template->y, (u8) template->width, (u8) template->height, 1, (u16)subBaseTile);
            subBaseTile += template->width * template->height;
        }
        FillWindowPixelBuffer(window, 0);
    }
}

#pragma force_active reset

void GtsSearchResult_RemoveWindows(GtsApp *app) {
    int i;
    Window *window;

    RemoveWindow(&app->waitingIconWindow);
    ClearWindowTilemapAndCopyToVram(&app->windows[1]);
    ClearWindowTilemapAndCopyToVram(&app->windows[0]);
    RemoveWindow(&app->windows[1]);
    RemoveWindow(&app->windows[0]);
    for (i = 0, window = app->infoWindows; i < 16; i++, window++) {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
    }
}

#pragma force_active on

// The country and city of the result's trainer (messages 82 and 83).
void GtsSearchResult_CreateStrings(GtsApp *app) {
    GtsRecord *result = &app->searchResults[app->selectedResult];

    app->msgBuffer = String_New(180, HEAP_ID_61);
    MessageFormat_ResetBuffers(app->msgFormat);
    if (result->country) {
        BufferCountryName(app->msgFormat, 8, result->country);
    }
    if (result->city) {
        BufferCityName(app->msgFormat, 9, result->country, result->city);
    }
    app->unk_0BC4[0] = ReadMsgData_ExpandPlaceholders(app->msgFormat, app->gtsMsgData, 82, HEAP_ID_61);
    app->unk_0BC4[1] = ReadMsgData_ExpandPlaceholders(app->msgFormat, app->gtsMsgData, 83, HEAP_ID_61);
}

#pragma force_active reset

void GtsSearchResult_DeleteStrings(GtsApp *app) {
    String_Delete(app->unk_0BC4[0]);
    String_Delete(app->unk_0BC4[1]);
    String_Delete(app->msgBuffer);
}

int GtsSearchResult_Start(GtsApp *app) {
    Sprite_SetDrawFlag(app->panelSprite, FALSE);
    if (app->nextScreenArg == 16) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 16, 1, HEAP_ID_61);
        app->state = GTS_SEARCH_RESULT_STATE_SCROLL_IN;
    } else {
        app->state = GTS_SEARCH_RESULT_STATE_HANDLE_INPUT;
        Sprite_SetDrawFlag(app->avatarMarkerSprite, TRUE);
    }
    return 3;
}

#pragma force_active on

// Switches to another result: the screen is entered again (arg 17) for it.
void GtsSearchResult_Select(GtsApp *app, int result) {
    if (result != app->selectedResult && result >= 0) {
        Sprite_SetAnimCtrlSeq(app->avatarSprites[result + 1], result * 4 + 16);
        app->state = GTS_SEARCH_RESULT_STATE_FADE_OUT;
        Gts_SetNextScreen(app, 3, 17);
        app->selectedResult = result;
        Gts_SetAvatarMarker(app, result, 0);
        PlaySE(SEQ_SE_DP_SELECT);
    }
}

int GtsSearchResult_HandleInput(GtsApp *app) {
    if (gSystem.newKeys & PAD_BUTTON_A) {
        GtsSearchResult_PrintMessage(app, 16, 1, 0, 0xF0F); // Trade Pokemon with this Trainer?
        Gts_SetState(app, GTS_SEARCH_RESULT_STATE_WAIT_TEXT, GTS_SEARCH_RESULT_STATE_SHOW_TRADE_PROMPT);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 16, 1, HEAP_ID_61);
        app->state = GTS_SEARCH_RESULT_STATE_SCROLL_OUT;
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newKeys & PAD_KEY_LEFT) {
        int result = app->selectedResult;
        int next = sGtsSearchResultLeft[2 * result];
        if (result != next && (int)app->searchResultCount >= next + 1) {
            GtsSearchResult_Select(app, next);
        }
    } else if (gSystem.newKeys & PAD_KEY_RIGHT) {
        int result = app->selectedResult;
        int next = sGtsSearchResultRight[2 * result];
        if (result != next && (int)app->searchResultCount >= next + 1) {
            GtsSearchResult_Select(app, next);
        }
    } else {
        int next = Gts_GetTouchedAvatar(app->searchResultCount);
        int button;
        if (next != -1) {
            GtsSearchResult_Select(app, next);
        }
        button = TouchscreenHitbox_FindRectAtTouchNew(sGtsSearchResultButtonHitboxes);
        if (button == 0) {
            GtsSearchResult_PrintMessage(app, 16, 1, 0, 0xF0F); // Trade Pokemon with this Trainer?
            Gts_SetState(app, GTS_SEARCH_RESULT_STATE_WAIT_TEXT, GTS_SEARCH_RESULT_STATE_SHOW_TRADE_PROMPT);
            PlaySE(SEQ_SE_DP_SELECT);
        } else if (button == 1) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 16, 1, HEAP_ID_61);
            app->state = GTS_SEARCH_RESULT_STATE_SCROLL_OUT;
            PlaySE(SEQ_SE_DP_SELECT);
        }
    }
    return 3;
}

#pragma force_active reset

int GtsSearchResult_FadeOut(GtsApp *app) {
    Sprite_SetDrawFlag(app->avatarMarkerSprite, FALSE);
    if (app->nextScreenArg != 15) {
        if (app->nextScreenArg == 6) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_61);
        } else {
            BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_61);
        }
    }
    app->state = GTS_SEARCH_RESULT_STATE_START;
    return 4;
}

int GtsSearchResult_ShowTradePrompt(GtsApp *app) {
    app->yesNoPrompt = Gts_CreateYesNoPromptOnBg(app->bgConfig, 10, 0x12E, 3, GF_BG_LYR_SUB_0, 1);
    app->state = GTS_SEARCH_RESULT_STATE_HANDLE_TRADE_PROMPT;
    return 3;
}

int GtsSearchResult_HandleTradePrompt(GtsApp *app) {
    int response = Gts_HandleYesNoPrompt(app);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        app->state = GTS_SEARCH_RESULT_STATE_FADE_OUT;
        Gts_SetNextScreen(app, 5, 6);
        GtsSearchResult_PrintLabel(app->gtsMsgData, &app->infoWindows[14], 77); // WANTED
        GtsSearchResult_PrintLabel(app->gtsMsgData, &app->infoWindows[9], 81);  // OT'S LOCATION
        GtsSearchResult_DrawDetails(app);
    } else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        ClearFrameAndWindow2(&app->waitingIconWindow, FALSE);
        app->state = GTS_SEARCH_RESULT_STATE_HANDLE_INPUT;
        GtsSearchResult_PrintButtonLabel(app->gtsMsgData, &app->windows[0], 88);  // TRADE
        GtsSearchResult_PrintButtonLabel(app->gtsMsgData, &app->windows[1], 109); // CANCEL
    }
    return 3;
}

void GtsSearchResult_DrawDetails(GtsApp *app) {
    GfGfxLoader_LoadScrnData(NARC_a_1_0_0, 36, app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, TRUE, HEAP_ID_61);
    GtsSearchResult_DrawLocation(&app->infoWindows[10], app->unk_0BC4[0], app->unk_0BC4[1]);
    GtsSearchResult_DrawWanted(&app->infoWindows[15], app->gtsMsgData, app->speciesNames, &app->searchResults[app->selectedResult].wanted);
}

int GtsSearchResult_Redraw(GtsApp *app) {
    GtsSearchResult_DrawDetails(app);
    app->state = GTS_SEARCH_RESULT_STATE_HANDLE_INPUT;
    return 3;
}

// Scrolls the screens back into place after the search screen (screen 4) scrolled them away.
int GtsSearchResult_ScrollIn(GtsApp *app) {
    app->scrollY++;
    if (IsPaletteFadeFinished()) {
        app->scrollY = 0;
        app->state = GTS_SEARCH_RESULT_STATE_HANDLE_INPUT;
        Sprite_SetDrawFlag(app->avatarMarkerSprite, TRUE);
    }
    return 3;
}

int GtsSearchResult_ScrollOut(GtsApp *app) {
    app->scrollY--;
    if (IsPaletteFadeFinished()) {
        reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
        app->state = GTS_SEARCH_RESULT_STATE_FADE_OUT;
        Gts_SetNextScreen(app, 4, 15);
        app->scrollY = 16;
    }
    return 3;
}

int GtsSearchResult_WaitText(GtsApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        app->state = app->nextState;
    }
    return 3;
}

void GtsSearchResult_PrintMessage(GtsApp *app, int msgId, int textSpeed, int unused3, u32 unused4) {
    ReadMsgDataIntoString(app->gtsMsgData, msgId, app->msgBuffer);
    FillWindowPixelBuffer(&app->waitingIconWindow, 15);
    DrawFrameAndWindow2(&app->waitingIconWindow, FALSE, 1, 2);
    app->textPrinterId = AddTextPrinterParameterized(&app->waitingIconWindow, 1, app->msgBuffer, 0, 0, textSpeed, NULL);
}

void GtsSearchResult_PrintLabel(MsgData *msgData, Window *window, int msgId) {
    String *string = NewString_ReadMsgData(msgData, msgId);
    Gts_PrintAlignedFont0(window, string, 0, 0, 0, MAKE_TEXT_COLOR(15, 2, 0));
    String_Delete(string);
}

void GtsSearchResult_PrintButtonLabel(MsgData *msgData, Window *window, int msgId) {
    String *string = NewString_ReadMsgData(msgData, msgId);
    Gts_PrintAlignedFont4(window, string, 0, 0, 1, MAKE_TEXT_COLOR(15, 2, 0));
    String_Delete(string);
}

void GtsSearchResult_DrawWanted(Window *window, MsgData *msgData, MsgData *speciesNames, GtsWantedMon *wanted) {
    int levelRange;

    FillWindowPixelBuffer(window, 0);
    Gts_PrintSpeciesNameNoTransfer(window, speciesNames, wanted->species, 0, MAKE_TEXT_COLOR(1, 2, 0));
    Gts_PrintGenderNoTransfer(window, msgData, wanted->gender, FALSE, 73, 0, MAKE_TEXT_COLOR(1, 2, 0));
    levelRange = Gts_FindLevelRange(wanted->minLevel, wanted->maxLevel, 0);
    Gts_PrintLevelRangeAt(window, msgData, levelRange, 0, 104, 0, MAKE_TEXT_COLOR(1, 2, 0), 0);
}

void GtsSearchResult_DrawLocation(Window *windows, String *country, String *city) {
    FillWindowPixelBuffer(&windows[0], 0);
    FillWindowPixelBuffer(&windows[1], 0);
    if (country != NULL) {
        Gts_PrintAlignedFont0(&windows[0], country, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
    }
    if (city != NULL) {
        Gts_PrintAlignedFont0(&windows[1], city, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
    }
}

// vblankFunc while the screen is shown: every BG follows scrollY (the sub screen BGs the other way).
void GtsSearchResult_ScrollBgs(GtsApp *app) {
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_Y, app->scrollY);
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, app->scrollY);
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, app->scrollY);
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, app->scrollY);
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_Y, -app->scrollY);
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, -32 - app->scrollY);
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, -app->scrollY);
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_Y, -app->scrollY);
}
