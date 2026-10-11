#include "global.h"

#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gts_internal.h"
#include "heap.h"
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

// SEEK POKEMON (screen 4): the player sets the wanted Pokemon (species, gender, level, the trainer's country) with
// the selector, SEARCH asks search.asp, the trainers found stand on the top screen; touching one, or the results
// button, shows it (screen 3).

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
// The {column, row} of each button; the asm labels the row column separately.
extern const u8 sGtsSearchButtonColumns[];
extern const u8 sGtsSearchButtonRows[];

// {x, y, anim} of the cursor on each button; the asm labels the three columns separately, so each label is read
// as an array of rows starting at its column.
typedef struct GtsSearchCursorColumn {
    u16 value;
    u16 nextColumns[2];
} GtsSearchCursorColumn;

extern const GtsSearchCursorColumn sGtsSearchCursorX[];
extern const GtsSearchCursorColumn sGtsSearchCursorY[];
extern const GtsSearchCursorColumn sGtsSearchCursorAnims[];
extern int ov70_0224590C[];

// Not declared in a public header.
u32 sub_0202DB80(void *gtsSave);
u32 sub_0202DB90(void *gtsSave);
void ov00_021ECB94(RTCDate *date, RTCTime *time);
void sub_020399EC(void);

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
int ov70_0223C928(GtsApp *app);

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
    args.sprites[0] = app->unk_0DD0;
    args.sprites[1] = app->arrowSprites[0];
    args.sprites[2] = app->arrowSprites[1];
    args.sprites[3] = app->cursorSprite;
    args.gtsMsgData = app->gtsMsgData;
    args.speciesNames = app->speciesNames;
    args.countryNames = app->countryNames;
    args.pokedex = app->args->pokedex;
    args.unk_2C = app->searchCursor->unk_14;
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

#pragma force_active on

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

#pragma force_active reset

void GtsSearch_FreeBgs(BgConfig *bgConfig) {
    Gts_FreeSubScreenBgs(bgConfig);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
}

#pragma force_active on

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
    // The first cursor position, read through the button table's label (as a u16 array).
    template.position.x = ((const u16 *)sGtsSearchButtonColumns)[7] << FX32_SHIFT;
    template.position.y = ((const u16 *)sGtsSearchButtonColumns)[8] << FX32_SHIFT;
    app->cursorSprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(app->cursorSprite, TRUE);
    Sprite_SetAnimCtrlSeq(app->cursorSprite, 45);
    Sprite_SetPriority(app->cursorSprite, 1);
    Sprite_SetOamMode(app->cursorSprite, GX_OAM_MODE_XLU);

    template.position.x = 10 << 16;
    template.position.y = 2 << 16;
    app->unk_0DD0 = Sprite_CreateAffine(&template);
    Sprite_SetAnimCtrlSeq(app->unk_0DD0, 47);
    Sprite_SetDrawFlag(app->unk_0DD0, FALSE);

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

#pragma force_active reset

void GtsSearch_DeleteSprites(GtsApp *app) {
    Sprite_Delete(app->cursorSprite);
    Sprite_Delete(app->unk_0DD0);
    Sprite_Delete(app->arrowSprites[0]);
    Sprite_Delete(app->arrowSprites[1]);
}

#pragma force_active on

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

#pragma force_active reset

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
    app->titleString = NewString_ReadMsgData(app->gtsMsgData, 42); // SEEK POKEMON
    app->searchCursor = Heap_Alloc(HEAP_ID_61, sizeof(GtsSearchCursor));
    MIi_CpuClearFast(0, (u32 *)app->searchCursor, sizeof(GtsSearchCursor));
    app->searchCursor->unk_14 = Gts_CreateJohtoDexFlags(HEAP_ID_61);
    ov70_0223F948(&app->unk_1234);
}

void GtsSearch_FreeData(GtsApp *app) {
    Heap_Free(app->searchCursor->unk_14);
    Heap_Free(app->searchCursor);
    String_Delete(app->msgBuffer);
    String_Delete(app->titleString);
}

int GtsSearch_Start(GtsApp *app) {
    if (app->nextScreenArg == 15) {
        GtsSearch_PrintMessage(app, 32, 0, 0, 0xF0F); // Touch "SELECT" to set trade partner.
        if (GX_GetMasterBrightness() == -16) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 16, 1, HEAP_ID_61);
            app->state = GTS_SEARCH_STATE_SCROLL_IN;
        } else {
            app->state = GTS_SEARCH_STATE_HANDLE_INPUT;
        }
    } else {
        GtsSearch_PrintMessage(app, 8, 1, 0, 0xF0F); // What kind of Pokemon are you seeking?
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
        GtsSearch_PrintMessage(app, 15, 1, 0, 0xF0F); // Stop searching?
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
            GtsSearch_PrintMessage(app, 15, 1, 0, 0xF0F); // Stop searching?
            Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_SHOW_EXIT_PROMPT);
            PlaySE(SEQ_SE_DP_SELECT);
        }
    }
    return 3;
}

int GtsSearch_StartSearch(GtsApp *app) {
    if (app->searchCriteria.species == SPECIES_NONE) {
        GtsSearch_PrintMessage(app, 12, 1, 0, 0xF0F); // Please choose the Pokemon you want!
        Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_HANDLE_INPUT);
        PlaySE(SEQ_SE_DP_BOX03);
    } else if (GtsSearch_IsSameSearch(&app->searchCriteria, &app->lastSearch, app->searchCountry, app->lastSearchCountry)) {
        GtsSearch_PrintMessage(app, 33, 1, 0, 0xF0F); // Please change your conditions.
        Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_HANDLE_INPUT);
        PlaySE(SEQ_SE_DP_BOX03);
    } else {
        PlaySE(SEQ_SE_DP_Z_SEARCH);
        GtsSearch_PrintMessage(app, 13, 1, 0, 0xF0F); // Searching for the Pokemon you want...
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
        packed = sub_0202DB90((void *)app->args->unk0);
    } else {
        packed = sub_0202DB80((void *)app->args->unk0);
    }
    saved.year = (packed >> 24) & 0xFF;
    saved.month = (packed >> 16) & 0xFF;
    saved.day = (packed >> 8) & 0xFF;
    saved.week = (RTCWeek)(packed & 0xFF);
    ov00_021ECB94(&current, &time);
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
            ((void (*)(u16))StopSE)(SEQ_SE_DP_Z_SEARCH); // called without its second argument
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
            sub_020399EC();
            break;
        case -15:
        case -12:
            app->state = GTS_SEARCH_STATE_SEARCH_FAILED;
            break;
        }
    } else {
        app->requestFrames++;
        if (app->requestFrames == 60 * 60) {
            sub_020399EC();
        }
    }
    return 3;
}

int GtsSearch_ShowResultCount(GtsApp *app) {
    if (app->searchResultCount == 0) {
        GtsSearch_PrintMessage(app, 14, 1, 0, 0xF0F); // None were found!
        Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_HANDLE_INPUT);
        PlaySE(SEQ_SE_DP_BOX03);
    } else {
        GtsSearch_PrintMessage(app, 30, 1, 0, 0xF0F); // A trade partner has been found!
        Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_WAIT_RESULTS_SHOWN);
        app->waitFrames = 0;
    }
    return 3;
}

int GtsSearch_WaitResultsShown(GtsApp *app) {
    app->waitFrames++;
    if (app->waitFrames > 45) {
        GtsSearch_PrintMessage(app, 32, 1, 0, 0xF0F); // Touch "SELECT" to set trade partner.
        Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_HANDLE_INPUT);
        app->hasSearchResults = TRUE;
    }
    return 3;
}

int GtsSearch_SearchFailed(GtsApp *app) {
    app->lastSearch.species = SPECIES_NONE;
    GtsSearch_PrintMessage(app, 38, 1, 0, 0xF0F); // GTS check failed.
    Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_HANDLE_INPUT);
    PlaySE(SEQ_SE_DP_BOX03);
    return 3;
}

int GtsSearch_ServerError(GtsApp *app) {
    GtsSearch_PrintMessage(app, 153, 4, 0, 0xF0F);
    Gts_SetState(app, GTS_SEARCH_STATE_WAIT_TEXT_AND_PAUSE, GTS_SEARCH_STATE_SERVER_ERROR_EXIT);
    app->waitFrames = 0;
    PlaySE(SEQ_SE_DP_BOX03);
    return 3;
}

int GtsSearch_ServerErrorExit(GtsApp *app) {
    GtsSearch_PrintMessage(app, 158, 4, 0, 0xF0F);
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

#pragma force_active on

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

#pragma force_active reset

int GtsSearch_FadeOut(GtsApp *app) {
    if (app->scrollY == 0) {
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_61);
    }
    app->state = GTS_SEARCH_STATE_START;
    return 4;
}

int GtsSearch_AskSpecies(GtsApp *app) {
    GtsSearch_PrintMessage(app, 9, 1, 0, 0xF0F); // Please choose the Pokemon you want.
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
        ov70_0223F960(&app->unk_1234, app->searchCursor->unk_06 + app->searchCursor->unk_04, app->searchCursor->unk_0A, app->searchCursor->unk_08);
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
        ov70_0223F960(&app->unk_1234, app->searchCursor->unk_06 + app->searchCursor->unk_04, app->searchCursor->unk_0A, app->searchCursor->unk_08);
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
    GtsSearch_PrintMessage(app, 10, 1, 0, 0xF0F); // Please choose its gender.
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
    GtsSearch_PrintMessage(app, 11, 1, 0, 0xF0F); // Please choose the level.
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
    GtsSearch_PrintMessage(app, 176, 1, 0, 0xF0F);
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
        if (selection == -2 || selection == ov70_0224590C[0] + 1) {
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

int ov70_0223C928(GtsApp *app) {
    app->state = GTS_SEARCH_STATE_HANDLE_INPUT;
    return 3;
}
