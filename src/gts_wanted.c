#include "global.h"

#include "constants/pokemon.h"

#include "application/zukanlist/zkn_data/zukan_data.naix"

#include "assert.h"
#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gts_internal.h"
#include "heap.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "player_data.h"
#include "pokemon.h"
#include "render_window.h"
#include "save_wifi_history.h"
#include "screen_fade.h"
#include "sprite.h"
#include "string_util.h"
#include "system.h"
#include "text.h"
#include "wireless_status_icon.h"
#include "yes_no_prompt.h"

// The wanted Pokemon of a deposit (screen 6): after the Pokemon to deposit is chosen (screen 5), the player picks the
// species, the gender (unless the species has only one) and the level range with the selector; "Are these conditions
// OK?" then deposits (screen 7), B or No goes back to screen 5.
// The helpers that follow are shared by the screens: the texts of a wanted Pokemon, the level ranges, the species and
// country lists, the record of a deposit or trade.

typedef enum GtsWantedState {
    GTS_WANTED_STATE_START,
    GTS_WANTED_STATE_HANDLE_INPUT,
    GTS_WANTED_STATE_FADE_OUT,
    GTS_WANTED_STATE_OPEN_SPECIES_SELECTOR,
    GTS_WANTED_STATE_4,
    GTS_WANTED_STATE_5,
    GTS_WANTED_STATE_HANDLE_SPECIES_SELECTOR,
    GTS_WANTED_STATE_ASK_GENDER,
    GTS_WANTED_STATE_OPEN_GENDER_SELECTOR,
    GTS_WANTED_STATE_HANDLE_GENDER_SELECTOR,
    GTS_WANTED_STATE_ASK_LEVEL,
    GTS_WANTED_STATE_OPEN_LEVEL_SELECTOR,
    GTS_WANTED_STATE_HANDLE_LEVEL_SELECTOR,
    GTS_WANTED_STATE_ASK_CONFIRM,
    GTS_WANTED_STATE_SHOW_CONFIRM_PROMPT,
    GTS_WANTED_STATE_HANDLE_CONFIRM_PROMPT,
    GTS_WANTED_STATE_WAIT_TEXT,
} GtsWantedState;

// GtsWantedMon.gender: 1 male, 2 female, 3 either (or genderless).
#define GTS_GENDER_ANY 3

typedef struct GtsWantedWindowPosition {
    u16 x;
    u16 y;
} GtsWantedWindowPosition;

// A choice of the level list: kind 0 the levels a deposit asks for (12), kind 1 the levels SEEK POKEMON searches (11).
typedef struct GtsLevelRange {
    u32 msgId;
    s16 minLevel; // 0: none
    s16 maxLevel; // 0: none
} GtsLevelRange;

// The countries in list order, in groups by initial letter: {first index in sGtsCountries, group}.
typedef struct GtsCountryGroup {
    u8 start;
    u8 group;
} GtsCountryGroup;

// Sets the level of the wireless status icon (wireless_status_icon_2.c); not declared in a public header.
void WirelessStatusIcon_SetLevel(s32 level);

extern int (*const sGtsWantedStates[])(GtsApp *app);
extern const BgTemplate sGtsWantedBg0Template;
extern const BgTemplate sGtsWantedBg1Template;
extern const BgTemplate sGtsWantedBg2Template;
extern const BgTemplate sGtsWantedBg3Template;
extern const GtsWantedWindowPosition sGtsWantedWindowPositions[];
extern u32 sGtsGenderSymbolMsgs[];
// The text colors of the male and female symbols.
extern u32 sGtsGenderColors[];
extern const GtsLevelRange sGtsWantedLevelRanges[];
extern const GtsLevelRange sGtsSearchLevelRanges[];
// NARC_zukan_data_sort_order_letters_*.
extern const int sGtsSpeciesLetterFiles[];
extern const u16 sGtsCountries[];
extern const GtsCountryGroup sGtsCountryGroups[];

int GtsWanted_Init(GtsApp *app, int state);
int GtsWanted_Main(GtsApp *app, int state);
int GtsWanted_Exit(GtsApp *app, int state);
void GtsWanted_PrintMessage(GtsApp *app, int msgId, int textSpeed, int unused3, int unused4);
void GtsWanted_InitBgs(BgConfig *bgConfig);
void GtsWanted_FreeBgs(BgConfig *bgConfig);
void GtsWanted_LoadGraphics(GtsApp *app);
void GtsWanted_CreateWindows(GtsApp *app);
void GtsWanted_RemoveWindows(GtsApp *app);
void GtsWanted_CreateSprites(GtsApp *app);
void GtsWanted_DeleteSprites(GtsApp *app);
void GtsWanted_CreateData(GtsApp *app);
void GtsWanted_FreeData(GtsApp *app);
int GtsWanted_Start(GtsApp *app);
int GtsWanted_HandleInput(GtsApp *app);
int GtsWanted_OpenSpeciesSelector(GtsApp *app);
int ov70_0223EDCC(GtsApp *app);
int ov70_0223EDD8(GtsApp *app);
int GtsWanted_HandleSpeciesSelector(GtsApp *app);
int GtsWanted_AskGender(GtsApp *app);
int GtsWanted_OpenGenderSelector(GtsApp *app);
int GtsWanted_HandleGenderSelector(GtsApp *app);
int GtsWanted_AskLevel(GtsApp *app);
int GtsWanted_OpenLevelSelector(GtsApp *app);
int GtsWanted_HandleLevelSelector(GtsApp *app);
int GtsWanted_AskConfirm(GtsApp *app);
int GtsWanted_ShowConfirmPrompt(GtsApp *app);
int GtsWanted_HandleConfirmPrompt(GtsApp *app);
int GtsWanted_FadeOut(GtsApp *app);
int GtsWanted_WaitText(GtsApp *app);
u32 Gts_GetGenderColor(int gender, u32 color);
void GtsWanted_DrawOfferedMon(MsgData *msgData, MessageFormat *msgFormat, Window *windows, BoxPokemon *boxMon, GtsMonSummary *summary);
void GtsWanted_InitDepositRecord(GtsRecord *record, GtsApp *app);

#pragma force_active on

int GtsWanted_Init(GtsApp *app, int state) {
    GtsSelectorArgs args;

    GtsWanted_CreateData(app);
    GtsWanted_InitBgs(app->bgConfig);
    GtsWanted_LoadGraphics(app);
    GtsWanted_CreateWindows(app);
    GtsWanted_CreateSprites(app);
    BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_61);
    Gts_CreateWirelessStatusIcon(app);
    Gts_DrawWantedMon(app->gtsMsgData, app->speciesNames, app->msgFormat, &app->infoWindows[0], SPECIES_NONE, GTS_GENDER_ANY, -1);
    GtsWanted_DrawOfferedMon(app->gtsMsgData, app->msgFormat, &app->infoWindows[3], app->selectedMon, &app->depositSummary);
    args.bgConfig = app->bgConfig;
    args.windows = app->windows;
    args.messageWindow = &app->selectorWindow;
    args.sprites[0] = app->unk_0DD0;
    args.sprites[1] = app->arrowSprites[0];
    args.sprites[2] = app->arrowSprites[1];
    args.sprites[3] = NULL;
    args.gtsMsgData = app->gtsMsgData;
    args.speciesNames = app->speciesNames;
    args.countryNames = app->countryNames;
    args.pokedex = app->args->pokedex;
    args.unk_2C = app->searchCursor->unk_14;
    app->selector = GtsSelector_New(&args, 2, 0);
    app->state = GTS_WANTED_STATE_START;
    return 2;
}

#pragma force_active reset

int GtsWanted_Main(GtsApp *app, int state) {
    WirelessStatusIcon_SetLevel(Gts_GetWifiIconLevel());
    return sGtsWantedStates[app->state](app);
}

int GtsWanted_Exit(GtsApp *app, int state) {
    WirelessStatusIcon_Destroy();
    GtsWanted_DeleteSprites(app);
    GtsSelector_Delete(app->selector);
    GtsWanted_FreeData(app);
    GtsWanted_RemoveWindows(app);
    GtsWanted_FreeBgs(app->bgConfig);
    Sprite_SetDrawFlag(app->panelSprite, FALSE);
    Gts_SwitchScreen(app);
    return 1;
}

void GtsWanted_PrintMessage(GtsApp *app, int msgId, int textSpeed, int unused3, int unused4) {
    String *string = NewString_ReadMsgData(app->gtsMsgData, msgId);
    StringExpandPlaceholders(app->msgFormat, app->msgBuffer, string);
    FillWindowPixelBuffer(&app->waitingIconWindow, 15);
    DrawFrameAndWindow2(&app->waitingIconWindow, FALSE, 1, 14);
    app->textPrinterId = AddTextPrinterParameterized(&app->waitingIconWindow, 1, app->msgBuffer, 0, 0, textSpeed, NULL);
    String_Delete(string);
}

void GtsWanted_InitBgs(BgConfig *bgConfig) {
    BgTemplate bg0;
    BgTemplate bg1;
    BgTemplate bg2;
    BgTemplate bg3;

    bg0 = sGtsWantedBg0Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &bg0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    bg1 = sGtsWantedBg1Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &bg1, GF_BG_TYPE_TEXT);
    bg2 = sGtsWantedBg2Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &bg2, GF_BG_TYPE_TEXT);
    FillBgTilemapRect(bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 24, 0);
    BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_MAIN_2);
    bg3 = sGtsWantedBg3Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &bg3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_61);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 0x20, 0, HEAP_ID_61);
    Gts_InitSubScreenBgs(bgConfig, 0, FALSE);
}

void GtsWanted_FreeBgs(BgConfig *bgConfig) {
    Gts_FreeSubScreenBgs(bgConfig);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
}

void GtsWanted_LoadGraphics(GtsApp *app) {
    BgConfig *bgConfig = app->bgConfig;
    NARC *narc = NARC_New(NARC_a_1_0_0, HEAP_ID_61);
    GfGfxLoader_GXLoadPal(NARC_a_1_0_0, 1, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x40, HEAP_ID_61);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_61);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 1, 14, Options_GetFrame(app->args->options), HEAP_ID_61);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 0x1F, 11, 0, HEAP_ID_61);
    GfGfxLoader_LoadCharData(NARC_a_1_0_0, 12, bgConfig, GF_BG_LYR_MAIN_1, 0, 0xA00, TRUE, HEAP_ID_61);
    GfGfxLoader_LoadScrnData(NARC_a_1_0_0, 26, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, TRUE, HEAP_ID_61);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 15, bgConfig, GF_BG_LYR_MAIN_2, 0, 0x600, TRUE, HEAP_ID_61);
    Gts_ShowSubScreenPanel(app);
    NARC_Delete(narc);
}

#pragma force_active on

// The message window; the wanted Pokemon's lines (infoWindows[0..2]) and the offered Pokemon's (infoWindows[3..5]).
void GtsWanted_CreateWindows(GtsApp *app) {
    int i;
    int baseTile;
    const GtsWantedWindowPosition *position;
    Window *window;

    AddWindowParameterized(app->bgConfig, &app->waitingIconWindow, GF_BG_LYR_MAIN_0, 2, 21, 27, 2, 13, 96);
    FillWindowPixelBuffer(&app->waitingIconWindow, 0);
    position = sGtsWantedWindowPositions;
    i = 0;
    baseTile = 150;
    window = app->infoWindows;
    for (; i < 6; i++, baseTile += 22, position++, window++) {
        AddWindowParameterized(app->bgConfig, window, GF_BG_LYR_MAIN_3, (u8)position->x, (u8)position->y, 11, 2, 13, (u16)baseTile);
        FillWindowPixelBuffer(window, 0);
        CopyWindowToVram(window);
    }
    Gts_CreatePanelWindow(app, 3);
}

#pragma force_active reset

void GtsWanted_RemoveWindows(GtsApp *app) {
    int i;
    Window *window;

    RemoveWindow(&app->panelWindow);
    window = app->infoWindows;
    for (i = 0; i < 6; i++) {
        RemoveWindow(window);
        window++;
    }
    RemoveWindow(&app->waitingIconWindow);
}

// The selector's sprites.
void GtsWanted_CreateSprites(GtsApp *app) {
    SpriteTemplate template;

    Gts_InitSpriteTemplate(&template, app, &app->headers[0], NNS_G2D_VRAM_TYPE_2DMAIN);
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
}

void GtsWanted_DeleteSprites(GtsApp *app) {
    Sprite_Delete(app->unk_0DD0);
    Sprite_Delete(app->arrowSprites[0]);
    Sprite_Delete(app->arrowSprites[1]);
}

void GtsWanted_CreateData(GtsApp *app) {
    app->msgBuffer = String_New(180, HEAP_ID_61);
    app->searchCursor = Heap_Alloc(HEAP_ID_61, sizeof(GtsSearchCursor));
    MI_CpuClearFast(app->searchCursor, sizeof(GtsSearchCursor));
    app->searchCursor->alphabeticalSpecies = Gts_LoadAlphabeticalSpecies(HEAP_ID_61, 0, &app->searchCursor->alphabeticalSpeciesCount);
    app->searchCursor->unk_14 = Gts_CreateJohtoDexFlags(HEAP_ID_61);
    ov70_0223F948(&app->unk_1234);
}

void GtsWanted_FreeData(GtsApp *app) {
    Heap_Free(app->searchCursor->unk_14);
    Heap_Free(app->searchCursor->alphabeticalSpecies);
    Heap_Free(app->searchCursor);
    String_Delete(app->msgBuffer);
}

int GtsWanted_Start(GtsApp *app) {
    if (IsPaletteFadeFinished()) {
        GtsWanted_PrintMessage(app, 9, 1, 0, 0xF0F); // Please choose the Pokemon you want.
        Gts_SetState(app, GTS_WANTED_STATE_WAIT_TEXT, GTS_WANTED_STATE_HANDLE_INPUT);
    }
    return 3;
}

int GtsWanted_HandleInput(GtsApp *app) {
    if (gSystem.newKeys & PAD_BUTTON_B) {
        Gts_SetNextScreen(app, 5, 5);
        app->state = GTS_WANTED_STATE_FADE_OUT;
    }
    // Overwrites the state set by B: the species selector always opens.
    app->state = GTS_WANTED_STATE_OPEN_SPECIES_SELECTOR;
    return 3;
}

int GtsWanted_OpenSpeciesSelector(GtsApp *app) {
    GtsSelector_Start(app->selector, 0);
    app->state = GTS_WANTED_STATE_HANDLE_SPECIES_SELECTOR;
    return 3;
}

// States 4 and 5 are not used.
int ov70_0223EDCC(GtsApp *app) {
    GF_AssertFail();
    return 3;
}

int ov70_0223EDD8(GtsApp *app) {
    GF_AssertFail();
    return 3;
}

// The gender of a species with only one.
BOOL Gts_SetFixedGender(GtsWantedMon *wanted, u32 genderRatio) {
    switch (genderRatio) {
    case MON_RATIO_MALE:
        wanted->gender = 1;
        return TRUE;
    case MON_RATIO_FEMALE:
        wanted->gender = 2;
        return TRUE;
    case MON_RATIO_UNKNOWN:
        wanted->gender = GTS_GENDER_ANY;
        return TRUE;
    default:
        return FALSE;
    }
}

#pragma force_active on

int GtsWanted_HandleSpeciesSelector(GtsApp *app) {
    int selection = GtsSelector_Main(app->selector);
    switch (selection) {
    case -2:
        ClearFrameAndWindow2(&app->waitingIconWindow, FALSE);
        Gts_SetNextScreen(app, 5, 5);
        app->state = GTS_WANTED_STATE_FADE_OUT;
        ov70_0223F960(&app->unk_1234, app->searchCursor->unk_06 + app->searchCursor->unk_04, app->searchCursor->unk_0A, app->searchCursor->unk_08);
        break;
    case -1:
        break;
    default: {
        int gender;
        app->depositWanted.species = selection;
        app->searchCursor->genderRatio = GetMonBaseStat(selection, BASE_GENDER_RATIO);
        if (Gts_SetFixedGender(&app->depositWanted, app->searchCursor->genderRatio)) {
            app->state = GTS_WANTED_STATE_ASK_LEVEL;
            gender = app->depositWanted.gender;
        } else {
            app->state = GTS_WANTED_STATE_ASK_GENDER;
            gender = GTS_GENDER_ANY;
        }
        Gts_DrawWantedMon(app->gtsMsgData, app->speciesNames, app->msgFormat, app->infoWindows, app->depositWanted.species, gender, -1);
        ov70_0223F960(&app->unk_1234, app->searchCursor->unk_06 + app->searchCursor->unk_04, app->searchCursor->unk_0A, app->searchCursor->unk_08);
        break;
    }
    }
    return 3;
}

#pragma force_active reset

int GtsWanted_AskGender(GtsApp *app) {
    GtsWanted_PrintMessage(app, 10, 1, 0, 0xF0F); // Please choose its gender.
    Gts_SetState(app, GTS_WANTED_STATE_WAIT_TEXT, GTS_WANTED_STATE_OPEN_GENDER_SELECTOR);
    return 3;
}

int GtsWanted_OpenGenderSelector(GtsApp *app) {
    app->unk_0118 = 0xFFFF;
    GtsSelector_Start(app->selector, 1);
    app->state = GTS_WANTED_STATE_HANDLE_GENDER_SELECTOR;
    return 3;
}

#pragma force_active on

int GtsWanted_HandleGenderSelector(GtsApp *app) {
    u32 selection = GtsSelector_Main(app->selector);
    switch (selection) {
    case -2:
        ClearFrameAndWindow2(&app->waitingIconWindow, FALSE);
        app->state = GTS_WANTED_STATE_START;
        break;
    case 0:
    case 1:
    case 2:
        app->depositWanted.gender = selection + 1;
        app->state = GTS_WANTED_STATE_ASK_LEVEL;
        Gts_DrawWantedMon(app->gtsMsgData, app->speciesNames, app->msgFormat, app->infoWindows, app->depositWanted.species, app->depositWanted.gender, -1);
        break;
    }
    return 3;
}

#pragma force_active reset

int GtsWanted_AskLevel(GtsApp *app) {
    GtsWanted_PrintMessage(app, 11, 1, 0, 0xF0F); // Please choose the level.
    Gts_SetState(app, GTS_WANTED_STATE_WAIT_TEXT, GTS_WANTED_STATE_OPEN_LEVEL_SELECTOR);
    return 3;
}

int GtsWanted_OpenLevelSelector(GtsApp *app) {
    GtsSelector_Start(app->selector, 2);
    app->unk_0118 = 0xFFFF;
    app->state = GTS_WANTED_STATE_HANDLE_LEVEL_SELECTOR;
    return 3;
}

#pragma force_active on

// Canceling goes back to the gender, or to the species when it has only one gender.
int GtsWanted_HandleLevelSelector(GtsApp *app) {
    u32 selection = GtsSelector_Main(app->selector);
    switch (selection) {
    case 12: // past the 12 level ranges
    case -2:
        ClearFrameAndWindow2(&app->waitingIconWindow, FALSE);
        if (Gts_SetFixedGender(&app->depositWanted, app->searchCursor->genderRatio)) {
            app->state = GTS_WANTED_STATE_START;
        } else {
            app->state = GTS_WANTED_STATE_ASK_GENDER;
        }
        break;
    case -1:
        break;
    default:
        Gts_SetLevelRange(&app->depositWanted, selection, 0);
        app->state = GTS_WANTED_STATE_ASK_CONFIRM;
        {
            int levelRange = Gts_FindLevelRange(app->depositWanted.minLevel, app->depositWanted.maxLevel, 0);
            Gts_DrawWantedMon(app->gtsMsgData, app->speciesNames, app->msgFormat, app->infoWindows, app->depositWanted.species, app->depositWanted.gender, levelRange);
        }
        break;
    }
    return 3;
}

#pragma force_active reset

int GtsWanted_AskConfirm(GtsApp *app) {
    GtsWanted_PrintMessage(app, 23, 1, 0, 0xF0F); // Are these conditions OK?
    Gts_SetState(app, GTS_WANTED_STATE_WAIT_TEXT, GTS_WANTED_STATE_SHOW_CONFIRM_PROMPT);
    return 3;
}

int GtsWanted_ShowConfirmPrompt(GtsApp *app) {
    app->yesNoPrompt = Gts_CreateYesNoPrompt(app->bgConfig, 12, 0x11A, 3, FALSE);
    app->state = GTS_WANTED_STATE_HANDLE_CONFIRM_PROMPT;
    return 3;
}

#pragma force_active on

int GtsWanted_HandleConfirmPrompt(GtsApp *app) {
    int response = Gts_HandleYesNoPrompt(app);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        Gts_SetNextScreen(app, 7, 7);
        app->state = GTS_WANTED_STATE_FADE_OUT;
        app->unk_11FC = 1;
        GtsWanted_InitDepositRecord(&app->record, app);
    } else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        Gts_SetNextScreen(app, 5, 5);
        app->state = GTS_WANTED_STATE_FADE_OUT;
    }
    return 3;
}

int GtsWanted_FadeOut(GtsApp *app) {
    if (app->nextScreen == 0) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_61);
        app->unk_11FC = 1;
    } else {
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_61);
    }
    app->state = GTS_WANTED_STATE_START;
    return 4;
}

#pragma force_active reset

int GtsWanted_WaitText(GtsApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        app->state = app->nextState;
    }
    return 3;
}

// The texts of a wanted Pokemon (all screens).

void Gts_PrintSpeciesName(Window *window, MsgData *speciesNames, int species, int align, int y, u32 color) {
    String *string;
    if (species != SPECIES_NONE) {
        string = NewString_ReadMsgData(speciesNames, species);
        Gts_PrintAlignedFont0(window, string, 0, y, align, color);
        String_Delete(string);
    }
}

void Gts_PrintSpeciesNameNoTransfer(Window *window, MsgData *speciesNames, int species, int y, u32 color) {
    String *string;
    if (species != SPECIES_NONE) {
        string = NewString_ReadMsgData(speciesNames, species);
        AddTextPrinterParameterizedWithColor(window, 0, string, 0, y, TEXT_SPEED_NOTRANSFER, color, NULL);
        String_Delete(string);
    }
}

#pragma force_active on

// Country 0: ANY.
void Gts_PrintCountryName(Window *window, MsgData *countryNames, MsgData *msgData, u32 country, int align, int y, u32 color) {
    String *string;
    if (country) {
        string = NewString_ReadMsgData(countryNames, country);
        Gts_PrintAlignedFont0(window, string, 0, y, align, color);
        String_Delete(string);
    } else {
        string = NewString_ReadMsgData(msgData, 173); // ANY
        Gts_PrintAlignedFont0(window, string, 0, y, align, color);
        String_Delete(string);
    }
}

#pragma force_active reset

// The color of the male and female symbols; color for the others.
u32 Gts_GetGenderColor(int gender, u32 color) {
    if (gender == 1) {
        return sGtsGenderColors[0];
    }
    if (gender == 2) {
        color = sGtsGenderColors[1];
    }
    return color;
}

#pragma force_active on

// The gender symbol, or EITHER (shown with showAny); xOrAlign up to 3 aligns the text, above it is its x.
void Gts_PrintGender(Window *window, MsgData *msgData, int gender, BOOL showAny, int y, int xOrAlign, u32 color) {
    String *string;
    if (showAny || gender != GTS_GENDER_ANY) {
        string = NewString_ReadMsgData(msgData, sGtsGenderSymbolMsgs[gender]);
        if (xOrAlign > 3) {
            Gts_PrintAlignedFont0(window, string, xOrAlign, y, 0, Gts_GetGenderColor(gender, color));
        } else {
            Gts_PrintAlignedFont0(window, string, 0, y, xOrAlign, Gts_GetGenderColor(gender, color));
        }
        String_Delete(string);
    }
}

void Gts_PrintGenderNoTransfer(Window *window, MsgData *msgData, int gender, BOOL showAny, int x, int y, u32 color) {
    String *string;
    if (showAny || gender != GTS_GENDER_ANY) {
        string = NewString_ReadMsgData(msgData, sGtsGenderSymbolMsgs[gender]);
        AddTextPrinterParameterizedWithColor(window, 0, string, x, y, TEXT_SPEED_NOTRANSFER, Gts_GetGenderColor(gender, color), NULL);
        String_Delete(string);
    }
}

#pragma force_active reset

void Gts_PrintLevelRange(Window *window, MsgData *msgData, int levelRange, int align, int y, u32 color, int kind) {
    Gts_PrintLevelRangeAt(window, msgData, levelRange, align, 0, y, color, kind);
}

// levelRange -1: nothing.
void Gts_PrintLevelRangeAt(Window *window, MsgData *msgData, int levelRange, int align, int x, int y, u32 color, int kind) {
    const GtsLevelRange *ranges;
    String *string;
    if (levelRange != -1) {
        ranges = kind == 0 ? sGtsWantedLevelRanges : sGtsSearchLevelRanges;
        string = NewString_ReadMsgData(msgData, ranges[levelRange].msgId);
        Gts_PrintAlignedFont0(window, string, x, y, align, color);
        String_Delete(string);
    }
}

// WANTED, then the species and gender, then the level range (right-aligned).
void Gts_DrawWantedMon(MsgData *msgData, MsgData *speciesNames, MessageFormat *msgFormat, Window *windows, int species, int gender, int levelRange) {
    String *title;
    int i;
    Window *window;

    title = NewString_ReadMsgData(msgData, 101); // WANTED
    Gts_PrintAlignedFont0(windows, title, 0, 0, 0, MAKE_TEXT_COLOR(15, 2, 0));
    window = windows;
    i = 1;
    window++;
    do {
        FillWindowPixelBuffer(window, 0);
        i++;
        window++;
    } while (i < 3);
    Gts_PrintSpeciesName(&windows[1], speciesNames, species, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
    if ((u32)(gender - 1) <= 1) { // male or female
        Gts_PrintGender(&windows[1], msgData, gender, FALSE, 0, 70, MAKE_TEXT_COLOR(1, 2, 0));
    }
    Gts_PrintLevelRange(&windows[2], msgData, levelRange, 2, 0, MAKE_TEXT_COLOR(1, 2, 0), 0);
    String_Delete(title);
}

// The same with the level range left-aligned.
void Gts_DrawWantedMonLevelLeft(MsgData *msgData, MsgData *speciesNames, MessageFormat *msgFormat, Window *windows, int species, int gender, int levelRange) {
    String *title;
    int i;
    Window *window;

    title = NewString_ReadMsgData(msgData, 101); // WANTED
    Gts_PrintAlignedFont0(windows, title, 0, 0, 0, MAKE_TEXT_COLOR(15, 2, 0));
    window = windows;
    i = 1;
    window++;
    do {
        FillWindowPixelBuffer(window, 0);
        i++;
        window++;
    } while (i < 3);
    Gts_PrintSpeciesName(&windows[1], speciesNames, species, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
    if ((u32)(gender - 1) <= 1) { // male or female
        Gts_PrintGender(&windows[1], msgData, gender, FALSE, 0, 70, MAKE_TEXT_COLOR(1, 2, 0));
    }
    Gts_PrintLevelRange(&windows[2], msgData, levelRange, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), 0);
    String_Delete(title);
}

#pragma force_active on

// OFFER POKEMON, then the nickname and gender, then the level; fills the Pokemon's summary.
void GtsWanted_DrawOfferedMon(MsgData *msgData, MessageFormat *msgFormat, Window *windows, BoxPokemon *boxMon, GtsMonSummary *summary) {
    String *title;
    String *levelString;
    String *nickname;
    String *genderSymbol;
    int gender;
    int level;
    int i;
    Window *window;

    nickname = String_New(11, HEAP_ID_61);
    genderSymbol = String_New(11, HEAP_ID_61);
    GetBoxMonData(boxMon, MON_DATA_NICKNAME_STRING, nickname);
    gender = GetBoxMonData(boxMon, MON_DATA_GENDER, NULL) + 1;
    level = CalcBoxMonLevel(boxMon);
    title = NewString_ReadMsgData(msgData, 104); // OFFER POKEMON
    BufferIntegerAsString(msgFormat, 3, level, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    levelString = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, 108, HEAP_ID_61); // Lv. {level}
    if (gender != GTS_GENDER_ANY) {
        ReadMsgDataIntoString(msgData, sGtsGenderSymbolMsgs[gender], genderSymbol);
    }
    for (i = 0, window = windows; i < 3; i++, window++) {
        FillWindowPixelBuffer(window, 0);
    }
    Gts_PrintAlignedFont0(&windows[0], title, 0, 0, 0, MAKE_TEXT_COLOR(15, 2, 0));
    Gts_PrintAlignedFont0(&windows[1], nickname, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
    Gts_PrintAlignedFont0(&windows[2], levelString, 0, 0, 2, MAKE_TEXT_COLOR(1, 2, 0));
    if (gender != GTS_GENDER_ANY) {
        Gts_PrintAlignedFont0(&windows[1], genderSymbol, 70, 0, 0, sGtsGenderColors[gender - 1]);
    }
    summary->species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
    summary->gender = gender;
    summary->level = level;
    String_Delete(levelString);
    String_Delete(genderSymbol);
    String_Delete(nickname);
    String_Delete(title);
}

// The species lists of the species selector.

u16 *Gts_LoadAlphabeticalSpecies(enum HeapID heapID, int unused, u32 *count) {
    u32 size;
    u16 *species = GfGfxLoader_LoadFromNarc_GetSizeOut(NARC_application_zukanlist_zkn_data_zukan_data, NARC_zukan_data_sort_order_dex_order_alphabetical, FALSE, heapID, FALSE, &size);
    *count = size >> 1;
    return species;
}

u16 *Gts_LoadSpeciesByLetter(enum HeapID heapID, int letter, u32 *count) {
    u32 size;
    u16 *species = GfGfxLoader_LoadFromNarc_GetSizeOut(NARC_application_zukanlist_zkn_data_zukan_data, sGtsSpeciesLetterFiles[letter], FALSE, heapID, TRUE, &size);
    *count = size >> 1;
    return species;
}

#pragma force_active reset

// flags[species] is TRUE for the species of the Johto Pokedex.
void *Gts_CreateJohtoDexFlags(enum HeapID heapID) {
    u32 size;
    u32 count;
    int i;
    u16 *species;
    u8 *flags = Heap_Alloc(HEAP_ID_61, MAX_SPECIES + 1);
    MIi_CpuClearFast(0, (u32 *)flags, MAX_SPECIES + 1);
    species = GfGfxLoader_LoadFromNarc_GetSizeOut(NARC_application_zukanlist_zkn_data_zukan_data, NARC_zukan_data_sort_order_dex_order_johto, FALSE, heapID, FALSE, &size);
    count = size / 2;
    for (i = 0; (u32)i < count; i++) {
        if (species[i] < MAX_SPECIES + 1) {
            flags[species[i]] = TRUE;
        }
    }
    Heap_Free(species);
    return flags;
}

// The record of a deposit or trade: the chosen Pokemon (selectedMon, from unk_0120's box) and the player.
void Gts_InitRecord(GtsRecord *record, GtsApp *app) {
    if (Gts_IsPartyBox(app->selectedBox)) {
        // A party Pokemon (a Pokemon starts with its BoxPokemon).
        Mon_UpdateShayminForm((Pokemon *)app->selectedMon, 0);
        MIi_CpuCopyFast((const u32 *)app->selectedMon, (u32 *)&record->mon, SizeOfStructPokemon());
    } else {
        BoxMon_UpdateShayminForm(app->selectedMon, 0);
        CopyBoxPokemonToPokemon(app->selectedMon, &record->mon);
    }
    CopyU16StringArrayN(record->trainerName, PlayerProfile_GetNamePtr(app->args->profile), PLAYER_NAME_LENGTH + 1);
    record->trainerId = PlayerProfile_GetTrainerID_VisibleHalf(app->args->profile);
    record->country = WifiHistory_GetPlayerCountry(app->args->wifiHistory);
    record->city = WiFiHistory_GetPlayerRegion(app->args->wifiHistory);
    record->trainerClass = PlayerProfile_GetAvatar(app->args->profile);
    record->trainerGender = PlayerProfile_GetTrainerGender(app->args->profile);
    record->version = GAME_VERSION;
    record->language = GAME_LANGUAGE;
}

void GtsWanted_InitDepositRecord(GtsRecord *record, GtsApp *app) {
    Gts_InitRecord(record, app);
    record->summary = app->depositSummary;
    record->wanted = app->depositWanted;
}

#pragma force_active on

// The level list of the selector.
int Gts_CreateLevelRangeListItems(ListMenuItem **items, MsgData *msgData, int kind) {
    int i;
    const GtsLevelRange *ranges;
    const GtsLevelRange *range;
    int count;

    if (kind == 0) {
        ranges = sGtsWantedLevelRanges;
        count = 12;
    } else {
        ranges = sGtsSearchLevelRanges;
        count = 11;
    }
    *items = ListMenuItems_New(count, HEAP_ID_61);
    // An explicit guarded do-while (a for loop compiles differently).
    i = 0;
    range = ranges;
    if (i < count) {
        do {
            ListMenuItems_AppendFromMsgData(*items, msgData, range->msgId, i);
            i++;
            range++;
        } while (i < count);
    }
    return count;
}

void Gts_SetLevelRange(GtsWantedMon *wanted, int levelRange, int kind) {
    const GtsLevelRange *ranges;
    if (kind == 0) {
        ranges = sGtsWantedLevelRanges;
        GF_ASSERT(levelRange < 12);
    } else {
        ranges = sGtsSearchLevelRanges;
        GF_ASSERT(levelRange < 11);
    }
    wanted->minLevel = ranges[levelRange].minLevel;
    wanted->maxLevel = ranges[levelRange].maxLevel;
}

// The index of the level range, 0 (ANY) when there is none.
int Gts_FindLevelRange(int minLevel, int maxLevel, int kind) {
    const GtsLevelRange *ranges;
    int count;

    if (kind == 0) {
        ranges = sGtsWantedLevelRanges;
        count = 12;
    } else {
        ranges = sGtsSearchLevelRanges;
        count = 11;
    }
    // kind is reused as the index (register allocation).
    for (kind = 0; kind < count; kind++) {
        if (minLevel == ranges[kind].minLevel && maxLevel == ranges[kind].maxLevel) {
            return kind;
        }
    }
    return 0;
}

#pragma force_active reset

// index: 0 for any country, else 1 + the country's index in sGtsCountries.
void Gts_SetSearchCountry(GtsApp *app, int index) {
    if (index == 0) {
        app->searchCountry = 0;
        return;
    }
    index--;
    if ((u32)index < 130) { // the count of countries (ov70_0224590C)
        app->searchCountry = sGtsCountries[index];
    }
}

// The countries of a group: returns their count, *start their first index in sGtsCountries.
int Gts_GetCountryGroupRange(int group, int *start) {
    const GtsCountryGroup *entry = sGtsCountryGroups;
    int i = 0;
    for (; (u32)i < 22; i++, entry++) {
        if (group == entry->group) {
            *start = sGtsCountryGroups[i].start;
            return sGtsCountryGroups[i + 1].start - sGtsCountryGroups[i].start;
        }
    }
    return 0;
}

#pragma force_active on

// The country list of a group; each item's value is 1 + the country's index in sGtsCountries.
#pragma opt_rotateloops off
#pragma opt_common_subs off

int Gts_CreateCountryListItems(ListMenuItem **items, MsgData *msgData, int group) {
    int start;
    int count = Gts_GetCountryGroupRange(group, &start);
    int i;
    *items = ListMenuItems_New(count, HEAP_ID_61);
    for (i = 0; i < count; i++) {
        ListMenuItems_AppendFromMsgData(*items, msgData, sGtsCountries[start + i], start + i + 1);
    }
    return count;
}

#pragma opt_rotateloops reset
#pragma opt_common_subs reset

#pragma force_active reset

void ov70_0223F948(GtsUnk1234 *arg) {
    int i;
    for (i = 0; i < 10; i++) {
        arg->unk_04[i] = 0;
        arg->unk_18[i] = 0;
    }
    arg->unk_00 = 0;
    arg->unk_02 = 0;
}

void ov70_0223F960(GtsUnk1234 *arg, int index, u16 value1, u16 value2) {
    arg->unk_04[index] = value1;
    arg->unk_18[index] = value2;
}
