#include "global.h"

#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "msgdata/msg/msg_0775.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "gts_internal.h"
#include "heap.h"
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
#include "wireless_status_icon.h"
#include "yes_no_prompt.h"

// GTS search screens.
//
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
// The results to the left and to the right of each result ({left, right} pairs).
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

typedef enum GtsSearchState {
    GTS_SEARCH_STATE_START,
    GTS_SEARCH_STATE_HANDLE_INPUT,
    GTS_SEARCH_STATE_FADE_OUT,
    GTS_SEARCH_STATE_ASK_SPECIES,
    GTS_SEARCH_STATE_OPEN_SPECIES_SELECTOR,
    GTS_SEARCH_STATE_HANDLE_SPECIES_SELECTOR,
    GTS_SEARCH_STATE_ASK_GENDER,
    GTS_SEARCH_STATE_OPEN_GENDER_SELECTOR,
    GTS_SEARCH_STATE_HANDLE_GENDER_SELECTOR,
    GTS_SEARCH_STATE_ASK_LEVEL,
    GTS_SEARCH_STATE_OPEN_LEVEL_SELECTOR,
    GTS_SEARCH_STATE_HANDLE_LEVEL_SELECTOR,
    GTS_SEARCH_STATE_ASK_COUNTRY,
    GTS_SEARCH_STATE_OPEN_COUNTRY_SELECTOR,
    GTS_SEARCH_STATE_HANDLE_COUNTRY_SELECTOR,
    GTS_SEARCH_STATE_START_SEARCH,
    GTS_SEARCH_STATE_SEND_SEARCH,
    GTS_SEARCH_STATE_WAIT_SEARCH,
    GTS_SEARCH_STATE_SHOW_RESULT_COUNT,
    GTS_SEARCH_STATE_WAIT_RESULTS_SHOWN,
    GTS_SEARCH_STATE_SEARCH_FAILED,
    GTS_SEARCH_STATE_WAIT_TEXT,
    GTS_SEARCH_STATE_WAIT_TEXT_AND_PAUSE,
    GTS_SEARCH_STATE_SHOW_EXIT_PROMPT,
    GTS_SEARCH_STATE_HANDLE_EXIT_PROMPT,
    GTS_SEARCH_STATE_25,
    GTS_SEARCH_STATE_26,
    GTS_SEARCH_STATE_SERVER_ERROR,
    GTS_SEARCH_STATE_SERVER_ERROR_EXIT,
    GTS_SEARCH_STATE_SCROLL_OUT,
    GTS_SEARCH_STATE_SCROLL_IN,
    GTS_SEARCH_STATE_SCROLLED_OUT,
} GtsSearchState;

typedef struct GtsWindowPosition {
    u16 x;
    u16 y;
} GtsWindowPosition;

// What search.asp is asked when the player recently deposited or traded (two more results per date).
typedef struct GtsSearchRequestEx {
    GtsWantedMon wanted;
    u8 count;
    u8 country;
} GtsSearchRequestEx;

extern int (*const sGtsSearchStates[])(GtsApp *app);
extern const BgTemplate sGtsSearchBg0Template;
extern const BgTemplate sGtsSearchBg1Template;
extern const BgTemplate sGtsSearchBg2Template;
extern const BgTemplate sGtsSearchBg3Template;
extern const TouchscreenHitbox sGtsSearchButtonHitboxes[];
extern const GtsWindowPosition sGtsSearchFieldWindowPositions[];
extern const GtsWindowPosition sGtsSearchLineWindowPositions[];
extern const GtsWindowPosition sGtsSearchValueWindowPositions[];
// The {column, row} of each button.
extern const u8 sGtsSearchButtonColumns[];
extern const u8 sGtsSearchButtonRows[];

// {x, y, anim} of the cursor on each button.
typedef struct GtsSearchCursorColumn {
    u16 value;
    u16 nextColumns[2];
} GtsSearchCursorColumn;

extern const GtsSearchCursorColumn sGtsSearchCursorX[];
extern const GtsSearchCursorColumn sGtsSearchCursorY[];
extern const GtsSearchCursorColumn sGtsSearchCursorAnims[];
extern int gGtsCountryCount[];

u32 GtsSave_GetDepositDate(void *gtsSave);
u32 GtsSave_GetTradeDate(void *gtsSave);
void DWC_GetDateTime(RTCDate *date, RTCTime *time);
void CommState_ShowRestartErrorAndHalt(void);

int GtsSearch_Init(GtsApp *app, int state);
int GtsSearch_Main(GtsApp *app, int state);
int GtsSearch_Exit(GtsApp *app, int state);
void GtsSearch_InitBgs(BgConfig *bgConfig);
void GtsSearch_FreeBgs(BgConfig *bgConfig);
void GtsSearch_LoadGraphics(GtsApp *app);
void GtsSearch_CreateSprites(GtsApp *app);
void GtsSearch_DeleteSprites(GtsApp *app);
void GtsSearch_CreateWindows(GtsApp *app);
void GtsSearch_RemoveWindows(GtsApp *app);
void GtsSearch_CreateData(GtsApp *app);
void GtsSearch_FreeData(GtsApp *app);
int GtsSearch_Start(GtsApp *app);
int GtsSearch_GetTouchedButton(void);
void GtsSearch_SelectButton(GtsApp *app, int button);
int GtsSearch_HandleInput(GtsApp *app);
int GtsSearch_StartSearch(GtsApp *app);
BOOL GtsSearch_IsDateRecent(GtsApp *app, int which);
int GtsSearch_SendSearch(GtsApp *app);
int GtsSearch_WaitSearch(GtsApp *app);
int GtsSearch_ShowResultCount(GtsApp *app);
int GtsSearch_WaitResultsShown(GtsApp *app);
int GtsSearch_SearchFailed(GtsApp *app);
int GtsSearch_ServerError(GtsApp *app);
int GtsSearch_ServerErrorExit(GtsApp *app);
int GtsSearch_GetCursorButton(GtsApp *app);
void GtsSearch_MoveCursor(GtsApp *app);
void GtsSearch_SetCursor(GtsApp *app, int button);
int GtsSearch_FadeOut(GtsApp *app);
int GtsSearch_AskSpecies(GtsApp *app);
int GtsSearch_OpenSpeciesSelector(GtsApp *app);
int GtsSearch_HandleSpeciesSelector(GtsApp *app);
int GtsSearch_AskGender(GtsApp *app);
int GtsSearch_OpenGenderSelector(GtsApp *app);
int GtsSearch_HandleGenderSelector(GtsApp *app);
int GtsSearch_AskLevel(GtsApp *app);
int GtsSearch_OpenLevelSelector(GtsApp *app);
int GtsSearch_HandleLevelSelector(GtsApp *app);
int GtsSearch_AskCountry(GtsApp *app);
int GtsSearch_OpenCountrySelector(GtsApp *app);
int GtsSearch_HandleCountrySelector(GtsApp *app);
int GtsSearch_ShowExitPrompt(GtsApp *app);
int GtsSearch_HandleExitPrompt(GtsApp *app);
int GtsSearch_ReturnToInput(GtsApp *app);

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
    GtsSearchResult_PrintLabel(app->gtsMsgData, &app->infoWindows[14], msg_0775_00077);  // WANTED
    GtsSearchResult_PrintLabel(app->gtsMsgData, &app->infoWindows[9], msg_0775_00081);   // OT'S LOCATION
    GtsSearchResult_PrintButtonLabel(app->gtsMsgData, &app->windows[0], msg_0775_00088); // TRADE
    GtsSearchResult_PrintButtonLabel(app->gtsMsgData, &app->windows[1], msg_0775_00109); // CANCEL
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

void GtsSearchResult_DeleteMonSprite(GtsApp *app) {
    Sprite_Delete(app->monSprite);
}

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
    app->infoStrings[0] = ReadMsgData_ExpandPlaceholders(app->msgFormat, app->gtsMsgData, msg_0775_00082, HEAP_ID_61);
    app->infoStrings[1] = ReadMsgData_ExpandPlaceholders(app->msgFormat, app->gtsMsgData, msg_0775_00083, HEAP_ID_61);
}

void GtsSearchResult_DeleteStrings(GtsApp *app) {
    String_Delete(app->infoStrings[0]);
    String_Delete(app->infoStrings[1]);
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
        GtsSearchResult_PrintMessage(app, msg_0775_00016, 1, 0, 0xF0F); // Trade Pokemon with this Trainer?
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
            GtsSearchResult_PrintMessage(app, msg_0775_00016, 1, 0, 0xF0F); // Trade Pokemon with this Trainer?
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
        GtsSearchResult_PrintLabel(app->gtsMsgData, &app->infoWindows[14], msg_0775_00077); // WANTED
        GtsSearchResult_PrintLabel(app->gtsMsgData, &app->infoWindows[9], msg_0775_00081);  // OT'S LOCATION
        GtsSearchResult_DrawDetails(app);
    } else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        ClearFrameAndWindow2(&app->waitingIconWindow, FALSE);
        app->state = GTS_SEARCH_RESULT_STATE_HANDLE_INPUT;
        GtsSearchResult_PrintButtonLabel(app->gtsMsgData, &app->windows[0], msg_0775_00088); // TRADE
        GtsSearchResult_PrintButtonLabel(app->gtsMsgData, &app->windows[1], msg_0775_00109); // CANCEL
    }
    return 3;
}

void GtsSearchResult_DrawDetails(GtsApp *app) {
    GfGfxLoader_LoadScrnData(NARC_a_1_0_0, 36, app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, TRUE, HEAP_ID_61);
    GtsSearchResult_DrawLocation(&app->infoWindows[10], app->infoStrings[0], app->infoStrings[1]);
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

// SEEK POKEMON (screen 4): the player sets the wanted Pokemon (species, gender, level, the trainer's country) with
// the selector, SEARCH asks search.asp, the trainers found stand on the top screen; touching one, or the results
// button, shows it (screen 3).

#pragma force_active on

int GtsSearch_Init(GtsApp *app, int state) {
    GtsSelectorArgs args;

    GtsSearch_CreateData(app);
    GtsSearch_InitBgs(app->bgConfig);
    GtsSearch_LoadGraphics(app);
    GtsSearch_CreateWindows(app);
    GtsSearch_CreateSprites(app);
    args.bgConfig = app->bgConfig;
    args.windows = app->windows;
    args.messageWindow = &app->selectorWindow;
    args.sprites[0] = app->selectorCursorSprite;
    args.sprites[1] = app->arrowSprites[0];
    args.sprites[2] = app->arrowSprites[1];
    args.sprites[3] = app->cursorSprite;
    args.gtsMsgData = app->gtsMsgData;
    args.speciesNames = app->speciesNames;
    args.countryNames = app->countryNames;
    args.pokedex = app->args->pokedex;
    args.johtoDexFlags = app->searchCursor->johtoDexFlags;
    app->selector = GtsSelector_New(&args, 2, 1);
    GtsSearch_DrawFieldLabels(app->infoWindows, &app->wideWindows[0], app->gtsMsgData);
    GtsSearch_DrawResultsButton(app->bgConfig, &app->infoWindows[8], app->gtsMsgData, app->hasSearchResults);
    Gts_PrintSpeciesName(&app->infoWindows[1], app->speciesNames, app->searchCriteria.species, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
    Gts_PrintGender(&app->infoWindows[3], app->gtsMsgData, app->searchCriteria.gender, TRUE, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
    Gts_PrintLevelRange(&app->infoWindows[5], app->gtsMsgData, Gts_FindLevelRange(app->searchCriteria.minLevel, app->searchCriteria.maxLevel, 1), 0, 0, MAKE_TEXT_COLOR(1, 2, 0), 1);
    Gts_PrintCountryName(&app->wideWindows[1], app->countryNames, app->gtsMsgData, app->searchCountry, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
    app->vblankFunc = GtsSearch_ScrollBgs;
    if (app->nextScreenArg == 13) {
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_61);
    }
    app->state = GTS_SEARCH_STATE_START;
    return 2;
}

int GtsSearch_Main(GtsApp *app, int state) {
    int ret = sGtsSearchStates[app->state](app);
    int i;
    int x;
    int y;

    for (i = 0; i < 8; i++) {
        Gts_SetSpritePosition(app->avatarSprites[i], app->avatarPositions[i][0], app->scrollY + app->avatarPositions[i][1]);
        Gts_SetSpritePosition(app->panelSprite, 55, app->scrollY + 424);
    }
    x = GtsSearch_GetCursorButton(app);
    y = GtsSearch_GetCursorButton(app);
    Gts_SetSpritePosition(app->cursorSprite, sGtsSearchCursorX[x].value, sGtsSearchCursorY[y].value - app->scrollY);
    return ret;
}

#pragma force_active reset

int GtsSearch_Exit(GtsApp *app, int state) {
    app->vblankFunc = NULL;
    GtsSelector_Delete(app->selector);
    GtsSearch_DeleteSprites(app);
    GtsSearch_FreeData(app);
    GtsSearch_RemoveWindows(app);
    GtsSearch_FreeBgs(app->bgConfig);
    Sprite_SetDrawFlag(app->panelSprite, FALSE);
    Gts_SwitchScreen(app);
    return 1;
}

void GtsSearch_InitBgs(BgConfig *bgConfig) {
    BgTemplate bg0 = sGtsSearchBg0Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &bg0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    {
        BgTemplate bg1 = sGtsSearchBg1Template;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &bg1, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    }
    {
        BgTemplate bg2 = sGtsSearchBg2Template;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &bg2, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    }
    {
        BgTemplate bg3 = sGtsSearchBg3Template;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &bg3, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    }
    Gts_InitSubScreenBgs(bgConfig, 0, FALSE);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 0x20, 0, HEAP_ID_61);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_61);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 0x20, 0, HEAP_ID_61);
}

void GtsSearch_FreeBgs(BgConfig *bgConfig) {
    Gts_FreeSubScreenBgs(bgConfig);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
}

void GtsSearch_LoadGraphics(GtsApp *app) {
    BgConfig *bgConfig = app->bgConfig;
    NARC *narc = NARC_New(NARC_a_1_0_0, HEAP_ID_61);

    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x60, HEAP_ID_61);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 5, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x100, HEAP_ID_61);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_61);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 1, 14, Options_GetFrame(app->args->options), HEAP_ID_61);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 0x1F, 11, 0, HEAP_ID_61);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 14, bgConfig, GF_BG_LYR_MAIN_1, 0, 0xA00, TRUE, HEAP_ID_61);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 29, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, TRUE, HEAP_ID_61);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 15, bgConfig, GF_BG_LYR_MAIN_2, 0, 0x600, TRUE, HEAP_ID_61);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_1_OFFSET, HEAP_ID_61);
    Gts_ShowSubScreenPanel(app);
    NARC_Delete(narc);
}

// The cursor and the selector's sprites.
void GtsSearch_CreateSprites(GtsApp *app) {
    SpriteTemplate template;

    Gts_InitSpriteTemplate(&template, app, &app->headers[0], NNS_G2D_VRAM_TYPE_2DMAIN);
    template.position.x = ((const u16 *)sGtsSearchButtonColumns)[7] << FX32_SHIFT;
    template.position.y = ((const u16 *)sGtsSearchButtonColumns)[8] << FX32_SHIFT;
    app->cursorSprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(app->cursorSprite, TRUE);
    Sprite_SetAnimCtrlSeq(app->cursorSprite, 45);
    Sprite_SetPriority(app->cursorSprite, 1);
    Sprite_SetOamMode(app->cursorSprite, GX_OAM_MODE_XLU);

    template.position.x = 10 << 16;
    template.position.y = 2 << 16;
    app->selectorCursorSprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimCtrlSeq(app->selectorCursorSprite, 47);
    Sprite_SetDrawFlag(app->selectorCursorSprite, FALSE);

    template.position.x = 57 << 14;
    template.position.y = 117 * FX32_ONE;
    app->arrowSprites[0] = Sprite_CreateAffine(&template);
    Sprite_SetAnimCtrlSeq(app->arrowSprites[0], 38);
    Sprite_SetDrawFlag(app->arrowSprites[0], FALSE);

    template.position.x = 35 << 14;
    app->arrowSprites[1] = Sprite_CreateAffine(&template);
    Sprite_SetAnimCtrlSeq(app->arrowSprites[1], 39);
    Sprite_SetDrawFlag(app->arrowSprites[1], FALSE);
    WirelessStatusIcon_Create();
}

void GtsSearch_DeleteSprites(GtsApp *app) {
    Sprite_Delete(app->cursorSprite);
    Sprite_Delete(app->selectorCursorSprite);
    Sprite_Delete(app->arrowSprites[0]);
    Sprite_Delete(app->arrowSprites[1]);
}

// The message window; the field labels and values (infoWindows[0..5]), the two wide lines (wideWindows) and three
// more windows (infoWindows[6..8]).
void GtsSearch_CreateWindows(GtsApp *app) {
    int i;
    int baseTile;
    const GtsWindowPosition *position;
    Window *window;

    AddWindowParameterized(app->bgConfig, &app->waitingIconWindow, GF_BG_LYR_MAIN_0, 2, 21, 27, 2, 13, 96);
    FillWindowPixelBuffer(&app->waitingIconWindow, 0);
    position = sGtsSearchFieldWindowPositions;
    i = 0;
    baseTile = 1;
    window = app->infoWindows;
    for (; i < 6; i++, baseTile += 22, position++, window++) {
        AddWindowParameterized(app->bgConfig, window, GF_BG_LYR_MAIN_3, (u8)position->x, (u8)position->y, 11, 2, 13, (u16)baseTile);
        FillWindowPixelBuffer(window, 0);
    }
    position = sGtsSearchLineWindowPositions;
    i = 0;
    baseTile = 133;
    window = app->wideWindows;
    for (; i < 2; i++, baseTile += 56, position++, window++) {
        AddWindowParameterized(app->bgConfig, window, GF_BG_LYR_MAIN_3, (u8)position->x, (u8)position->y, 28, 2, 13, (u16)baseTile);
        FillWindowPixelBuffer(window, 0);
    }
    i = 0;
    window = app->infoWindows;
    baseTile = 285;
    position = sGtsSearchValueWindowPositions;
    for (; i < 3; i++, baseTile += 20, position++) {
        int index = i + 6;
        AddWindowParameterized(app->bgConfig, &window[index], GF_BG_LYR_MAIN_3, (u8)position->x, (u8)position->y, 10, 2, 13, (u16)baseTile);
        FillWindowPixelBuffer(&window[index], 0);
    }
    Gts_CreatePanelWindow(app, 4);
}

void GtsSearch_RemoveWindows(GtsApp *app) {
    int i;
    Window *window;

    RemoveWindow(&app->panelWindow);
    RemoveWindow(&app->waitingIconWindow);
    for (i = 0, window = app->infoWindows; i < 9; i++, window++) {
        RemoveWindow(window);
    }
    for (i = 0, window = app->wideWindows; i < 2; i++, window++) {
        RemoveWindow(window);
    }
}

void GtsSearch_CreateData(GtsApp *app) {
    app->msgBuffer = String_New(180, HEAP_ID_61);
    app->titleString = NewString_ReadMsgData(app->gtsMsgData, msg_0775_00042); // SEEK POKEMON
    app->searchCursor = Heap_Alloc(HEAP_ID_61, sizeof(GtsSearchCursor));
    MIi_CpuClearFast(0, (u32 *)app->searchCursor, sizeof(GtsSearchCursor));
    app->searchCursor->johtoDexFlags = Gts_CreateJohtoDexFlags(HEAP_ID_61);
    GtsCursorValueTable_Clear(&app->unk_1234);
}

void GtsSearch_FreeData(GtsApp *app) {
    Heap_Free(app->searchCursor->johtoDexFlags);
    Heap_Free(app->searchCursor);
    String_Delete(app->msgBuffer);
    String_Delete(app->titleString);
}

int GtsSearch_Start(GtsApp *app) {
    if (app->nextScreenArg == 15) {
        GtsSearch_PrintMessage(app, msg_0775_00032, 0, 0, 0xF0F); // Touch "SELECT" to set trade partner.
        if (GX_GetMasterBrightness() == -16) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 16, 1, HEAP_ID_61);
            app->state = GTS_SEARCH_STATE_SCROLL_IN;
        } else {
            app->state = GTS_SEARCH_STATE_HANDLE_INPUT;
        }
    } else {
        GtsSearch_PrintMessage(app, msg_0775_00008, 1, 0, 0xF0F); // What kind of Pokemon are you seeking?
        Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_HANDLE_INPUT);
    }
    Sprite_SetDrawFlag(app->panelSprite, TRUE);
    return 3;
}

int GtsSearch_GetTouchedButton(void) {
    return TouchscreenHitbox_FindRectAtTouchNew(sGtsSearchButtonHitboxes);
}

// Buttons: 0 POKEMON, 1 GENDER, 2 LEVEL, 3 country, 4 the results, 5 SEARCH, 6 BACK.
void GtsSearch_SelectButton(GtsApp *app, int button) {
    switch (button) {
    case 0:
        app->state = GTS_SEARCH_STATE_ASK_SPECIES;
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    case 1:
        if (app->searchCriteria.species != SPECIES_NONE) {
            u32 genderRatio = GetMonBaseStat(app->searchCriteria.species, BASE_GENDER_RATIO);
            app->searchCursor->genderRatio = genderRatio;
            if (Gts_SetFixedGender(&app->searchCriteria, app->searchCursor->genderRatio)) {
                PlaySE(SEQ_SE_DP_SELECT);
                break;
            }
        }
        app->state = GTS_SEARCH_STATE_ASK_GENDER;
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    case 2:
        app->state = GTS_SEARCH_STATE_ASK_LEVEL;
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    case 3:
        app->state = GTS_SEARCH_STATE_ASK_COUNTRY;
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    case 4:
        if (app->searchResultCount != 0) {
            app->state = GTS_SEARCH_STATE_SCROLL_OUT;
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 16, 1, HEAP_ID_61);
            PlaySE(SEQ_SE_DP_SELECT);
        }
        break;
    case 5:
        app->state = GTS_SEARCH_STATE_START_SEARCH;
        break;
    case 6:
        GtsSearch_PrintMessage(app, msg_0775_00015, 1, 0, 0xF0F); // Stop searching?
        Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_SHOW_EXIT_PROMPT);
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    }
}

int GtsSearch_HandleInput(GtsApp *app) {
    int button = GtsSearch_GetTouchedButton();
    if (button != -1) {
        GtsSearch_SetCursor(app, button);
        GtsSearch_SelectButton(app, button);
    } else {
        GtsSearch_MoveCursor(app);
        if (gSystem.newKeys & PAD_BUTTON_A) {
            GtsSearch_SelectButton(app, GtsSearch_GetCursorButton(app));
        } else if (gSystem.newKeys & PAD_BUTTON_B) {
            GtsSearch_PrintMessage(app, msg_0775_00015, 1, 0, 0xF0F); // Stop searching?
            Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_SHOW_EXIT_PROMPT);
            PlaySE(SEQ_SE_DP_SELECT);
        }
    }
    return 3;
}

int GtsSearch_StartSearch(GtsApp *app) {
    if (app->searchCriteria.species == SPECIES_NONE) {
        GtsSearch_PrintMessage(app, msg_0775_00012, 1, 0, 0xF0F); // Please choose the Pokemon you want!
        Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_HANDLE_INPUT);
        PlaySE(SEQ_SE_DP_BOX03);
    } else if (GtsSearch_IsSameSearch(&app->searchCriteria, &app->lastSearch, app->searchCountry, app->lastSearchCountry)) {
        GtsSearch_PrintMessage(app, msg_0775_00033, 1, 0, 0xF0F); // Please change your conditions.
        Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_HANDLE_INPUT);
        PlaySE(SEQ_SE_DP_BOX03);
    } else {
        PlaySE(SEQ_SE_DP_Z_SEARCH);
        GtsSearch_PrintMessage(app, msg_0775_00013, 1, 0, 0xF0F); // Searching for the Pokemon you want...
        Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_SEND_SEARCH);
        Gts_StopAvatarAnims(app);
    }
    return 3;
}

// Whether the player deposited (which 1) or traded (which 0) on the GTS in the last three days.
BOOL GtsSearch_IsDateRecent(GtsApp *app, int which) {
    RTCDate saved;
    RTCDate current;
    RTCTime time;
    u32 packed;
    s32 days;

    if (which == 1) {
        packed = GtsSave_GetTradeDate((void *)app->args->gtsSave);
    } else {
        packed = GtsSave_GetDepositDate((void *)app->args->gtsSave);
    }
    saved.year = (packed >> 24) & 0xFF;
    saved.month = (packed >> 16) & 0xFF;
    saved.day = (packed >> 8) & 0xFF;
    saved.week = (RTCWeek)(packed & 0xFF);
    DWC_GetDateTime(&current, &time);
    days = RTC_ConvertDateToDay(&current) - RTC_ConvertDateToDay(&saved);
    return days >= 0 && days < 3;
}

// Three results, two more for each recent deposit or trade.
int GtsSearch_SendSearch(GtsApp *app) {
    int count = 3;
    GtsSearchRequestEx request;

    if (GtsSearch_IsDateRecent(app, 1)) {
        count += 2;
    }
    if (GtsSearch_IsDateRecent(app, 0)) {
        count += 2;
    }
    if (app->searchCountry == 0) {
        GtsServer_RequestSearch(&app->searchCriteria, count, app->searchResults);
    } else {
        MI_CpuFill8(&request, 0, sizeof(request));
        request.wanted.species = app->searchCriteria.species;
        request.wanted.gender = app->searchCriteria.gender;
        request.wanted.minLevel = app->searchCriteria.minLevel;
        request.wanted.maxLevel = app->searchCriteria.maxLevel;
        request.wanted.unk_5 = app->searchCriteria.unk_5;
        request.count = count;
        request.country = app->searchCountry;
        GtsServer_RequestSearchEx(&request, app->searchResults);
    }
    // Copied as halfword pairs.
    *(u16 *)&app->lastSearch.species = *(u16 *)&app->searchCriteria.species;
    *(u16 *)&app->lastSearch.gender = *(u16 *)&app->searchCriteria.gender;
    *(u16 *)&app->lastSearch.maxLevel = *(u16 *)&app->searchCriteria.maxLevel;
    app->lastSearchCountry = app->searchCountry;
    app->requestFrames = 0;
    app->state = GTS_SEARCH_STATE_WAIT_SEARCH;
    app->hasSearchResults = FALSE;
    return 3;
}

int GtsSearch_WaitSearch(GtsApp *app) {
    if (GtsServer_IsRequestDone()) {
        int result = GtsServer_GetResult();
        app->requestFrames = 0;
        switch (result) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            app->searchResultCount = result;
            StopSE(SEQ_SE_DP_Z_SEARCH, 0);
            Gts_ShowResultAvatars(app, result, 1);
            if (result == 0) {
                GtsSearch_DrawResultsButton(app->bgConfig, &app->infoWindows[8], app->gtsMsgData, FALSE);
            } else {
                GtsSearch_DrawResultsButton(app->bgConfig, &app->infoWindows[8], app->gtsMsgData, TRUE);
            }
            app->state = GTS_SEARCH_STATE_SHOW_RESULT_COUNT;
            break;
        case -14:
        case -2:
            app->state = GTS_SEARCH_STATE_SERVER_ERROR;
            break;
        case -13:
            CommState_ShowRestartErrorAndHalt();
            break;
        case -15:
        case -12:
            app->state = GTS_SEARCH_STATE_SEARCH_FAILED;
            break;
        }
    } else {
        app->requestFrames++;
        if (app->requestFrames == 60 * 60) {
            CommState_ShowRestartErrorAndHalt();
        }
    }
    return 3;
}

int GtsSearch_ShowResultCount(GtsApp *app) {
    if (app->searchResultCount == 0) {
        GtsSearch_PrintMessage(app, msg_0775_00014, 1, 0, 0xF0F); // None were found!
        Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_HANDLE_INPUT);
        PlaySE(SEQ_SE_DP_BOX03);
    } else {
        GtsSearch_PrintMessage(app, msg_0775_00030, 1, 0, 0xF0F); // A trade partner has been found!
        Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_WAIT_RESULTS_SHOWN);
        app->waitFrames = 0;
    }
    return 3;
}

int GtsSearch_WaitResultsShown(GtsApp *app) {
    app->waitFrames++;
    if (app->waitFrames > 45) {
        GtsSearch_PrintMessage(app, msg_0775_00032, 1, 0, 0xF0F); // Touch "SELECT" to set trade partner.
        Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_HANDLE_INPUT);
        app->hasSearchResults = TRUE;
    }
    return 3;
}

int GtsSearch_SearchFailed(GtsApp *app) {
    app->lastSearch.species = SPECIES_NONE;
    GtsSearch_PrintMessage(app, msg_0775_00038, 1, 0, 0xF0F); // GTS check failed.
    Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_HANDLE_INPUT);
    PlaySE(SEQ_SE_DP_BOX03);
    return 3;
}

int GtsSearch_ServerError(GtsApp *app) {
    GtsSearch_PrintMessage(app, msg_0775_00153, 4, 0, 0xF0F);
    Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT_AND_PAUSE, GTS_SEARCH_STATE_SERVER_ERROR_EXIT);
    app->waitFrames = 0;
    PlaySE(SEQ_SE_DP_BOX03);
    return 3;
}

int GtsSearch_ServerErrorExit(GtsApp *app) {
    GtsSearch_PrintMessage(app, msg_0775_00158, 4, 0, 0xF0F);
    Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT_AND_PAUSE, GTS_SEARCH_STATE_FADE_OUT);
    Gts_SetNextScreen(app, 0, 0);
    PlaySE(SEQ_SE_DP_BOX03);
    return 3;
}

// The button under the cursor: rows 0..3 of the left column, then rows 0..2 of the right one.
int GtsSearch_GetCursorButton(GtsApp *app) {
    GtsSearchCursor *cursor = app->searchCursor;
    if (cursor->column == 0) {
        return cursor->leftRow;
    }
    return cursor->rightRow + 4;
}

void GtsSearch_MoveCursor(GtsApp *app) {
    int keys = gSystem.newKeys;
    GtsSearchCursor *cursor;
    int x;
    int y;

    if (keys & PAD_KEY_UP) {
        cursor = app->searchCursor;
        if (cursor->column == 0) {
            if (cursor->leftRow > 0) {
                cursor->leftRow--;
                PlaySE(SEQ_SE_DP_SELECT);
            }
        } else {
            if (cursor->rightRow > 0) {
                PlaySE(SEQ_SE_DP_SELECT);
                app->searchCursor->rightRow--;
            }
        }
    } else if (keys & PAD_KEY_DOWN) {
        cursor = app->searchCursor;
        if (cursor->column == 0) {
            if (cursor->leftRow < 3) {
                cursor->leftRow++;
                PlaySE(SEQ_SE_DP_SELECT);
            }
        } else {
            if (cursor->rightRow < 2) {
                PlaySE(SEQ_SE_DP_SELECT);
                app->searchCursor->rightRow++;
            }
        }
    } else if (keys & PAD_KEY_RIGHT) {
        if (app->searchCursor->column != 1) {
            PlaySE(SEQ_SE_DP_SELECT);
        }
        app->searchCursor->column = 1;
    } else if (keys & PAD_KEY_LEFT) {
        if (app->searchCursor->column != 0) {
            PlaySE(SEQ_SE_DP_SELECT);
        }
        app->searchCursor->column = 0;
    }
    x = GtsSearch_GetCursorButton(app);
    y = GtsSearch_GetCursorButton(app);
    Gts_SetSpritePosition(app->cursorSprite, sGtsSearchCursorX[x].value, sGtsSearchCursorY[y].value);
    Sprite_SetAnimCtrlSeq(app->cursorSprite, sGtsSearchCursorAnims[GtsSearch_GetCursorButton(app)].value);
}

void GtsSearch_SetCursor(GtsApp *app, int button) {
    int x;
    int y;

    if (sGtsSearchButtonColumns[button * 2] == 0) {
        app->searchCursor->column = 0;
        app->searchCursor->leftRow = sGtsSearchButtonRows[button * 2];
    } else {
        app->searchCursor->column = 1;
        app->searchCursor->rightRow = sGtsSearchButtonRows[button * 2];
    }
    x = GtsSearch_GetCursorButton(app);
    y = GtsSearch_GetCursorButton(app);
    Gts_SetSpritePosition(app->cursorSprite, sGtsSearchCursorX[x].value, sGtsSearchCursorY[y].value);
    y = GtsSearch_GetCursorButton(app);
    Sprite_SetAnimCtrlSeq(app->cursorSprite, sGtsSearchCursorAnims[y].value);
}

int GtsSearch_FadeOut(GtsApp *app) {
    if (app->scrollY == 0) {
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_61);
    }
    app->state = GTS_SEARCH_STATE_START;
    return 4;
}

int GtsSearch_AskSpecies(GtsApp *app) {
    GtsSearch_PrintMessage(app, msg_0775_00009, 1, 0, 0xF0F); // Please choose the Pokemon you want.
    Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_OPEN_SPECIES_SELECTOR);
    return 3;
}

int GtsSearch_OpenSpeciesSelector(GtsApp *app) {
    GtsSelector_Start(app->selector, 0);
    GtsSearch_SetBlendBrightness(9);
    app->state = GTS_SEARCH_STATE_HANDLE_SPECIES_SELECTOR;
    return 3;
}

#pragma force_active on

int GtsSearch_HandleSpeciesSelector(GtsApp *app) {
    int selection = GtsSelector_Main(app->selector);
    switch (selection) {
    case -2:
        GtsSearch_SetBlendBrightness(0);
        app->state = GTS_SEARCH_STATE_START;
        GtsCursorValueTable_SetEntry(&app->unk_1234, app->searchCursor->unk_06 + app->searchCursor->unk_04, app->searchCursor->unk_0A, app->searchCursor->unk_08);
        break;
    case -1:
        break;
    default:
        GtsSearch_SetBlendBrightness(0);
        app->searchCriteria.species = selection;
        app->state = GTS_SEARCH_STATE_START;
        FillWindowPixelBuffer(&app->infoWindows[1], 0);
        Gts_PrintSpeciesName(&app->infoWindows[1], app->speciesNames, selection, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
        app->searchCursor->genderRatio = GetMonBaseStat(selection, BASE_GENDER_RATIO);
        GtsCursorValueTable_SetEntry(&app->unk_1234, app->searchCursor->unk_06 + app->searchCursor->unk_04, app->searchCursor->unk_0A, app->searchCursor->unk_08);
        if (Gts_SetFixedGender(&app->searchCriteria, app->searchCursor->genderRatio)) {
            FillWindowPixelBuffer(&app->infoWindows[3], 0);
            Gts_PrintGender(&app->infoWindows[3], app->gtsMsgData, app->searchCriteria.gender, TRUE, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
        }
        break;
    }
    return 3;
}

#pragma force_active reset

int GtsSearch_AskGender(GtsApp *app) {
    GtsSearch_PrintMessage(app, msg_0775_00010, 1, 0, 0xF0F); // Please choose its gender.
    Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_OPEN_GENDER_SELECTOR);
    return 3;
}

int GtsSearch_OpenGenderSelector(GtsApp *app) {
    GtsSelector_Start(app->selector, 1);
    GtsSearch_SetBlendBrightness(9);
    app->state = GTS_SEARCH_STATE_HANDLE_GENDER_SELECTOR;
    return 3;
}

#pragma force_active on

int GtsSearch_HandleGenderSelector(GtsApp *app) {
    u32 selection = GtsSelector_Main(app->selector);
    switch (selection) {
    case (u32)-2:
        GtsSearch_SetBlendBrightness(0);
        ClearFrameAndWindow2(&app->waitingIconWindow, FALSE);
        app->state = GTS_SEARCH_STATE_START;
        break;
    case 0:
    case 1:
    case 2:
        GtsSearch_SetBlendBrightness(0);
        app->searchCriteria.gender = selection + 1;
        app->state = GTS_SEARCH_STATE_START;
        FillWindowPixelBuffer(&app->infoWindows[3], 0);
        Gts_PrintGender(&app->infoWindows[3], app->gtsMsgData, app->searchCriteria.gender, TRUE, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
        break;
    }
    return 3;
}

#pragma force_active reset

int GtsSearch_AskLevel(GtsApp *app) {
    GtsSearch_PrintMessage(app, msg_0775_00011, 1, 0, 0xF0F); // Please choose the level.
    Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_OPEN_LEVEL_SELECTOR);
    return 3;
}

int GtsSearch_OpenLevelSelector(GtsApp *app) {
    app->unk_0118 = 0xFFFF;
    GtsSelector_Start(app->selector, 2);
    GtsSearch_SetBlendBrightness(9);
    app->state = GTS_SEARCH_STATE_HANDLE_LEVEL_SELECTOR;
    return 3;
}

#pragma force_active on

int GtsSearch_HandleLevelSelector(GtsApp *app) {
    u32 selection = GtsSelector_Main(app->selector);
    switch (selection) {
    case 11:
    case (u32)-2:
        GtsSearch_SetBlendBrightness(0);
        app->state = GTS_SEARCH_STATE_START;
        break;
    case (u32)-1:
        break;
    default:
        GtsSearch_SetBlendBrightness(0);
        Gts_SetLevelRange(&app->searchCriteria, selection, 1);
        app->state = GTS_SEARCH_STATE_START;
        FillWindowPixelBuffer(&app->infoWindows[5], 0);
        Gts_PrintLevelRange(&app->infoWindows[5], app->gtsMsgData, selection, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), 1);
        break;
    }
    return 3;
}

#pragma force_active reset

int GtsSearch_AskCountry(GtsApp *app) {
    GtsSearch_PrintMessage(app, msg_0775_00176, 1, 0, 0xF0F);
    Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_OPEN_COUNTRY_SELECTOR);
    return 3;
}

int GtsSearch_OpenCountrySelector(GtsApp *app) {
    app->unk_0118 = 0xFFFF;
    GtsSelector_Start(app->selector, 3);
    GtsSearch_SetBlendBrightness(9);
    app->state = GTS_SEARCH_STATE_HANDLE_COUNTRY_SELECTOR;
    return 3;
}

#pragma force_active on

int GtsSearch_HandleCountrySelector(GtsApp *app) {
    int selection = GtsSelector_Main(app->selector);
    if (selection != -1) {
        if (selection == -2 || selection == gGtsCountryCount[0] + 1) {
            GtsSearch_SetBlendBrightness(0);
            ClearFrameAndWindow2(&app->waitingIconWindow, FALSE);
            app->state = GTS_SEARCH_STATE_START;
        } else {
            GtsSearch_SetBlendBrightness(0);
            Gts_SetSearchCountry(app, selection);
            app->state = GTS_SEARCH_STATE_START;
            FillWindowPixelBuffer(&app->wideWindows[1], 0);
            Gts_PrintCountryName(&app->wideWindows[1], app->countryNames, app->gtsMsgData, app->searchCountry, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
        }
    }
    return 3;
}

#pragma force_active reset

int GtsSearch_ShowExitPrompt(GtsApp *app) {
    app->yesNoPrompt = Gts_CreateYesNoPrompt(app->bgConfig, 12, 0x120, 3, 1);
    app->state = GTS_SEARCH_STATE_HANDLE_EXIT_PROMPT;
    return 3;
}

int GtsSearch_HandleExitPrompt(GtsApp *app) {
    int response = Gts_HandleYesNoPrompt(app);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        app->state = GTS_SEARCH_STATE_FADE_OUT;
        Gts_SetNextScreen(app, 1, 0);
        Gts_StopAvatarAnims(app);
        app->searchResultCount = 0;
    } else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        app->state = GTS_SEARCH_STATE_START;
    }
    return 3;
}

int GtsSearch_ReturnToInput(GtsApp *app) {
    app->state = GTS_SEARCH_STATE_HANDLE_INPUT;
    return 3;
}
