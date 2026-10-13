#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0019.h"

#include "bg_window.h"
#include "dex_mon_measures.h"
#include "filesystem.h"
#include "font.h"
#include "frontier_data.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_86.h"
#include "overlay_manager.h"
#include "palette.h"
#include "player_avatar.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_window.h"
#include "sprite_system.h"
#include "system.h"
#include "touchscreen.h"
#include "sound_playback.h"
#include "unk_0200FA24.h"
#include "palette_blink.h"
#include "grid_input_handler.h"
#include "frontier_data_access.h"
#include "frontier_save_access.h"
#include "unk_0205BFF0.h"

// Facilities, as passed in FrontierRecordsAppArgs.facility
#define FRONTIER_RECORDS_TOWER        1
#define FRONTIER_RECORDS_FACTORY_LV50 2
#define FRONTIER_RECORDS_FACTORY_OPEN 3
#define FRONTIER_RECORDS_CASTLE       4
#define FRONTIER_RECORDS_HALL         5
#define FRONTIER_RECORDS_ARCADE       6

// Main loop states
#define RECORDS_STATE_FADE_IN  0
#define RECORDS_STATE_RUN      1
#define RECORDS_STATE_FADE_OUT 2

// Sub-states of FrontierRecordsApp_Run
#define RUN_STATE_START             0
#define RUN_STATE_RECORDS           1
#define RUN_STATE_SHOW_LETTERS      2
#define RUN_STATE_LETTERS           3
#define RUN_STATE_SHOW_SPECIES      4
#define RUN_STATE_SPECIES           5
#define RUN_STATE_FLIP_PAGE         6
#define RUN_STATE_SHOW_HALL_RECORDS 7
#define RUN_STATE_HALL_RECORDS      8
#define RUN_STATE_BUTTON_FLASH      9
#define RUN_STATE_EXIT              10

// Pages of the Battle Hall species picker
#define HALL_PAGE_RECORDS 0
#define HALL_PAGE_LETTERS 1
#define HALL_PAGE_SPECIES 2

#define NUM_LETTERS         26
#define SPECIES_PER_PAGE    8
#define LETTER_GRID_RETURN  26
#define SPECIES_GRID_RETURN 8

// Text colors (foreground, shadow, background packed as in MAKE_TEXT_COLOR)
#define COLOR_HEADER 0x0F0200
#define COLOR_TEXT   0x010200
#define COLOR_BUTTON 0x0F0100
#define COLOR_PROMPT 0x01020F

typedef struct FrontierRecordsSpeciesGroup {
    u16 *species;
    u32 count;
} FrontierRecordsSpeciesGroup;

typedef struct FrontierRecordsWindowSet {
    const WindowTemplate *templates;
    u32 count;
} FrontierRecordsWindowSet;

// Palette flash of a pressed button: either a sprite or a rectangle of a background tilemap
typedef struct FrontierRecordsButtonFlash {
    u8 isBg : 1;
    u8 index : 7;
    u8 paletteOn : 4;
    u8 paletteOff : 4;
    u8 state;
    volatile u8 timer;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} FrontierRecordsButtonFlash;

// size: 0x394
typedef struct FrontierRecordsApp {
    OverlayManager *manager;                         // 0x000
    u8 state;                                        // 0x004
    u8 nextState;                                    // 0x005
    u8 mode;                                         // 0x006
    u8 facility;                                     // 0x007
    u16 species;                                     // 0x008
    u16 unk_00A;                                     // 0x00A
    BgConfig *bgConfig;                              // 0x00C
    Window windows[32];                              // 0x010
    MsgData *msgData;                                // 0x210
    MessageFormat *msgFormat;                        // 0x214
    String *string;                                  // 0x218
    PaletteData *paletteData;                        // 0x21C
    Options *options;                                // 0x220
    SaveData *saveData;                              // 0x224
    FrontierSave *frontierSave;                      // 0x228
    SpriteSystem *spriteSystem;                      // 0x22C
    SpriteManager *spriteManager;                    // 0x230
    ManagedSprite *sprites[2];                       // 0x234
    Window returnButtonWindow;                       // 0x23C
    FrontierRecordsButtonFlash flash;                // 0x24C
    GridInputHandler *gridInput;                     // 0x254
    u16 letter;                                      // 0x258
    u16 speciesIndex;                                // 0x25A
    s8 page;                                         // 0x25C
    s8 lastPage;                                     // 0x25D
    u16 speciesWindowBank;                           // 0x25E
    FrontierRecordsSpeciesGroup groups[NUM_LETTERS]; // 0x260
    PaletteBlink *scrollArrows;                // 0x330
    u16 pageArrowTiles[3][16];                       // 0x334
} FrontierRecordsApp;

extern const TouchscreenHitbox _021E7E98;
extern const TouchscreenHitbox ov86_021E7E9C[];
extern const GridCallbacks ov86_021E7EA8;
extern const GridCallbacks ov86_021E7EB8;
extern const GraphicsModes ov86_021E7EC8;
extern const OamCharTransferParam ov86_021E7ED8;
extern const SpriteResourceCountsListUnion ov86_021E7EEC;
extern const BgTemplate ov86_021E7F04;
extern const BgTemplate ov86_021E7F20;
extern const BgTemplate ov86_021E7F3C;
extern const BgTemplate ov86_021E7F58;
extern const OamManagerParam ov86_021E7F74;
extern const TouchscreenHitbox ov86_021E7F94[];
extern const GraphicsBanks ov86_021E7FBC;
extern const DpadMenuBox ov86_021E7FE4[];
extern const ManagedSpriteTemplate ov86_021E802C[];
extern const TouchscreenHitbox ov86_021E8094[];
extern const DpadMenuBox ov86_021E8104[];
extern const u8 ov86_021E81DC[NUM_LETTERS];
extern const FrontierRecordsWindowSet ov86_021E8230[];

// Not declared by the owners' headers yet
u32 BattleFactory_GetBestStreakStat(u8 levelMode, u8 type);
u32 BattleFactory_GetBestTradeCountStat(u8 levelMode, u8 type);
u32 BattleCastle_GetBestStreakStat(u8 type);
u32 BattleCastle_GetBestStreakPointsStat(u8 type);
u32 FactoryFlags_GetField(int flags, u32 field, u8 index, u32 a3);

// text.h is not included: it declares fontId as u8 (FontID), but this module passes it unnarrowed
u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, void *callback);

BOOL FrontierRecordsApp_FadeIn(FrontierRecordsApp *app);
BOOL FrontierRecordsApp_Run(FrontierRecordsApp *app);
BOOL FrontierRecordsApp_FadeOut(FrontierRecordsApp *app);
void FrontierRecordsApp_VBlankCallback(void *cbArg);
void FrontierRecordsApp_SetGraphicsBanks(FrontierRecordsApp *app);
void FrontierRecordsApp_InitBgLayers(BgConfig *bgConfig);
void FrontierRecordsApp_InitGraphics(FrontierRecordsApp *app);
void FrontierRecordsApp_FreeBgLayers(BgConfig *bgConfig);
void FrontierRecordsApp_SetMainState(FrontierRecordsApp *app, int *state, int nextState);
u32 FrontierRecordsApp_GetWindowSet(u32 facility);
void FrontierRecordsApp_LoadMainBgGraphics(FrontierRecordsApp *app, GFBgLayer layer);
void FrontierRecordsApp_LoadSubBgGraphics(FrontierRecordsApp *app, GFBgLayer layer);
void FrontierRecordsApp_BufferNumber(FrontierRecordsApp *app, u32 field, u32 value);
void FrontierRecordsApp_PrintStringAligned(Window *window, String *string, u32 x, u32 y, u32 fontId, u32 color, u32 align);
void FrontierRecordsApp_PrintMessage(FrontierRecordsApp *app, u32 windowId, u32 msgId, u32 x, u32 y, u32 fontId, u32 color, u32 align);
void FrontierRecordsApp_PrintFormattedMessage(FrontierRecordsApp *app, u32 windowId, u32 msgId, u32 x, u32 y, u32 fontId, u32 color, u32 align);
void FrontierRecordsApp_PrintRecords(FrontierRecordsApp *app);
void FrontierRecordsApp_PrintFactoryLevel50Records(FrontierRecordsApp *app);
void FrontierRecordsApp_PrintFactoryOpenLevelRecords(FrontierRecordsApp *app);
u32 FrontierRecordsApp_GetFactoryStreakLabel(FrontierRecordsApp *app, u32 level);
void FrontierRecordsApp_PrintHallRecords(FrontierRecordsApp *app);
void FrontierRecordsApp_BufferSpeciesName(FrontierRecordsApp *app, int species);
void FrontierRecordsApp_PrintCastleRecords(FrontierRecordsApp *app);
u32 FrontierRecordsApp_GetCastleStreakLabel(FrontierRecordsApp *app);
void FrontierRecordsApp_PrintArcadeRecords(FrontierRecordsApp *app);
u32 FrontierRecordsApp_GetArcadeStreakLabel(FrontierRecordsApp *app);
void FrontierRecordsApp_PrintTowerRecords(FrontierRecordsApp *app);
void FrontierRecordsApp_PrintTowerMultiRecords(FrontierRecordsApp *app);
u32 FrontierRecordsApp_GetTowerStreakLabel(FrontierRecordsApp *app, u32 mode);
void FrontierRecordsApp_PrintReturnButton(FrontierRecordsApp *app);
void FrontierRecordsApp_CreateSprites(FrontierRecordsApp *app);
void FrontierRecordsApp_FreeSprites(FrontierRecordsApp *app);
void FrontierRecordsApp_TickSprites(FrontierRecordsApp *app);
void FrontierRecordsApp_SetSpriteAnim(FrontierRecordsApp *app, int index, int anim);
void FrontierRecordsApp_SetSpritePosition(FrontierRecordsApp *app, int index, s16 x, s16 y);
int FrontierRecordsApp_RunButtonFlash(FrontierRecordsApp *app);
void FrontierRecordsApp_StartReturnButtonFlash(FrontierRecordsApp *app);
BOOL FrontierRecordsApp_IsHall(FrontierRecordsApp *app);
void FrontierRecordsApp_InitHallPicker(FrontierRecordsApp *app);
void FrontierRecordsApp_FreeHallPicker(FrontierRecordsApp *app);
void FrontierRecordsApp_ShowHallPage(FrontierRecordsApp *app, int page);
void FrontierRecordsApp_DrawPageArrows(FrontierRecordsApp *app);
void FrontierRecordsApp_FreeGridInput(FrontierRecordsApp *app);
void FrontierRecordsApp_GridCallback_Nop(void);
u16 *FrontierRecordsApp_LoadSortedSpecies(int letter, u32 *count);
void FrontierRecordsApp_InitSpeciesGroups(FrontierRecordsApp *app);
void FrontierRecordsApp_FreeSpeciesGroups(FrontierRecordsApp *app);
void FrontierRecordsApp_LoadWindowFrame(FrontierRecordsApp *app);
void FrontierRecordsApp_ShowPrompt(FrontierRecordsApp *app);
void FrontierRecordsApp_ClearPrompt(FrontierRecordsApp *app);
void FrontierRecordsApp_PrintLetters(FrontierRecordsApp *app);
void FrontierRecordsApp_MoveLetterCursor(FrontierRecordsApp *app, int selection);
void FrontierRecordsApp_CreateLetterGrid(FrontierRecordsApp *app);
void FrontierRecordsApp_LetterGrid_OnKeyMove(FrontierRecordsApp *app, int selection);
void FrontierRecordsApp_LetterGrid_OnTouch(FrontierRecordsApp *app, int selection);
void FrontierRecordsApp_StartLetterFlash(FrontierRecordsApp *app, int letter);
u8 FrontierRecordsApp_HandleLetterInput(FrontierRecordsApp *app);
void FrontierRecordsApp_GrayOutEmptyLetters(FrontierRecordsApp *app);
void FrontierRecordsApp_MoveSpeciesCursor(FrontierRecordsApp *app, int selection);
void FrontierRecordsApp_CreateSpeciesGrid(FrontierRecordsApp *app);
void FrontierRecordsApp_SpeciesGrid_OnKeyMove(FrontierRecordsApp *app, int selection);
void FrontierRecordsApp_SpeciesGrid_OnTouch(FrontierRecordsApp *app, int selection);
void FrontierRecordsApp_StartSpeciesFlash(FrontierRecordsApp *app, int slot);
void FrontierRecordsApp_StartArrowFlash(FrontierRecordsApp *app, u8 x, u8 y);
int FrontierRecordsApp_HandleSpeciesInput(FrontierRecordsApp *app);
int FrontierRecordsApp_FlipSpeciesPage(FrontierRecordsApp *app);
void FrontierRecordsApp_PrintSpeciesPage(FrontierRecordsApp *app);
void FrontierRecordsApp_ClearSpeciesWindows(FrontierRecordsApp *app);
void FrontierRecordsApp_PrintLetterTitle(FrontierRecordsApp *app);
void FrontierRecordsApp_PrintPageNumber(FrontierRecordsApp *app);
void FrontierRecordsApp_ResetCursor(FrontierRecordsApp *app);
void FrontierRecordsApp_SelectSpecies(FrontierRecordsApp *app);
void FrontierRecordsApp_ClearRecordWindows(FrontierRecordsApp *app);
void FrontierRecordsApp_AddWindows(BgConfig *bgConfig, Window *windows, int set);
void FrontierRecordsApp_RemoveWindows(Window *windows, int set);
void FrontierRecordsApp_AddReturnButtonWindow(BgConfig *bgConfig, Window *window);
void FrontierRecordsApp_RemoveReturnButtonWindow(Window *window);

BOOL FrontierRecordsApp_Init(OverlayManager *man, int *state) {
    FrontierRecordsApp *app;
    FrontierRecordsAppArgs *args;

    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
    Heap_Create(HEAP_ID_3, HEAP_ID_121, 0x30000);
    app = OverlayManager_CreateAndGetData(man, sizeof(FrontierRecordsApp), HEAP_ID_121);
    memset(app, 0, sizeof(FrontierRecordsApp));
    app->manager = man;
    app->bgConfig = BgConfig_Alloc(HEAP_ID_121);
    args = OverlayManager_GetArgs(man);
    app->saveData = args->saveData;
    app->frontierSave = Save_Frontier_GetStatic(app->saveData);
    app->mode = args->mode;
    app->facility = args->facility;
    app->species = args->species;
    app->options = Save_PlayerData_GetOptionsAddr(app->saveData);
    FontID_Alloc(4, HEAP_ID_121);
    FrontierRecordsApp_InitGraphics(app);
    app->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0019_bin, HEAP_ID_121);
    app->msgFormat = MessageFormat_New(HEAP_ID_121);
    app->string = String_New(800, HEAP_ID_121);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_121);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, HEAP_ID_121);
    FrontierRecordsApp_AddWindows(app->bgConfig, app->windows, FrontierRecordsApp_GetWindowSet(app->facility));
    FrontierRecordsApp_AddReturnButtonWindow(app->bgConfig, &app->returnButtonWindow);
    FrontierRecordsApp_PrintReturnButton(app);
    FrontierRecordsApp_CreateSprites(app);
    Main_SetVBlankIntrCB(FrontierRecordsApp_VBlankCallback, app);
    *state = RECORDS_STATE_FADE_IN;
    return TRUE;
}

BOOL FrontierRecordsApp_Main(OverlayManager *man, int *state) {
    FrontierRecordsApp *app = OverlayManager_GetData(man);

    switch (*state) {
    case RECORDS_STATE_FADE_IN:
        if (FrontierRecordsApp_FadeIn(app) == TRUE) {
            FrontierRecordsApp_SetMainState(app, state, RECORDS_STATE_RUN);
        }
        break;
    case RECORDS_STATE_RUN:
        if (FrontierRecordsApp_Run(app) == TRUE) {
            FrontierRecordsApp_SetMainState(app, state, RECORDS_STATE_FADE_OUT);
        }
        break;
    case RECORDS_STATE_FADE_OUT:
        if (FrontierRecordsApp_FadeOut(app) == TRUE) {
            return TRUE;
        }
        break;
    }
    FrontierRecordsApp_TickSprites(app);
    SpriteSystem_DrawSprites(app->spriteManager);
    return FALSE;
}

BOOL FrontierRecordsApp_Exit(OverlayManager *man, int *state) {
#pragma unused(state)
    FrontierRecordsApp *app = OverlayManager_GetData(man);

    FrontierRecordsApp_FreeSprites(app);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_BG);
    PaletteData_Free(app->paletteData);
    app->paletteData = NULL;
    DestroyMsgData(app->msgData);
    MessageFormat_Delete(app->msgFormat);
    String_Delete(app->string);
    FrontierRecordsApp_RemoveReturnButtonWindow(&app->returnButtonWindow);
    FrontierRecordsApp_RemoveWindows(app->windows, FrontierRecordsApp_GetWindowSet(app->facility));
    FrontierRecordsApp_FreeBgLayers(app->bgConfig);
    FontID_Release(4);
    OverlayManager_FreeData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_121);
    return TRUE;
}

BOOL FrontierRecordsApp_FadeIn(FrontierRecordsApp *app) {
    switch (app->state) {
    case 0:
        if (FrontierRecordsApp_IsHall(app) == TRUE) {
            FrontierRecordsApp_InitHallPicker(app);
        } else {
            FrontierRecordsApp_PrintRecords(app);
        }
        app->state++;
        break;
    case 1:
        BeginNormalPaletteFade(0, 1, 1, RGB_BLACK, 6, 3, HEAP_ID_121);
        app->state++;
        break;
    case 2:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL FrontierRecordsApp_Run(FrontierRecordsApp *app) {
    switch (app->state) {
    case RUN_STATE_START:
        if (FrontierRecordsApp_IsHall(app) == TRUE) {
            app->state = RUN_STATE_LETTERS;
        } else {
            app->state = RUN_STATE_RECORDS;
        }
        break;
    case RUN_STATE_RECORDS:
        if (TouchscreenHitbox_TouchNewIsIn(&_021E7E98) == TRUE || (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            PlaySE(SEQ_SE_DP_DECIDE);
            FrontierRecordsApp_StartReturnButtonFlash(app);
            app->nextState = RUN_STATE_EXIT;
            app->state = RUN_STATE_BUTTON_FLASH;
        }
        break;
    case RUN_STATE_SHOW_LETTERS:
        FrontierRecordsApp_ShowHallPage(app, HALL_PAGE_LETTERS);
        app->state = RUN_STATE_LETTERS;
        break;
    case RUN_STATE_LETTERS:
        app->state = FrontierRecordsApp_HandleLetterInput(app);
        break;
    case RUN_STATE_SHOW_SPECIES:
        FrontierRecordsApp_ShowHallPage(app, HALL_PAGE_SPECIES);
        app->state = RUN_STATE_SPECIES;
        break;
    case RUN_STATE_SPECIES:
        app->state = FrontierRecordsApp_HandleSpeciesInput(app);
        break;
    case RUN_STATE_FLIP_PAGE:
        app->state = FrontierRecordsApp_FlipSpeciesPage(app);
        break;
    case RUN_STATE_SHOW_HALL_RECORDS:
        FrontierRecordsApp_ShowHallPage(app, HALL_PAGE_RECORDS);
        app->state = RUN_STATE_HALL_RECORDS;
        break;
    case RUN_STATE_HALL_RECORDS:
        if (TouchscreenHitbox_TouchNewIsIn(&_021E7E98) == TRUE || (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            PlaySE(SEQ_SE_DP_DECIDE);
            FrontierRecordsApp_StartReturnButtonFlash(app);
            app->nextState = RUN_STATE_SHOW_SPECIES;
            app->state = RUN_STATE_BUTTON_FLASH;
        }
        break;
    case RUN_STATE_BUTTON_FLASH:
        if (FrontierRecordsApp_RunButtonFlash(app) == 0) {
            app->state = app->nextState;
        }
        break;
    case RUN_STATE_EXIT:
        return TRUE;
    }
    return FALSE;
}

BOOL FrontierRecordsApp_FadeOut(FrontierRecordsApp *app) {
    switch (app->state) {
    case 0:
        BeginNormalPaletteFade(0, 0, 0, RGB_BLACK, 6, 1, HEAP_ID_121);
        app->state++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void FrontierRecordsApp_VBlankCallback(void *cbArg) {
    FrontierRecordsApp *app = cbArg;

    if (app->paletteData != NULL) {
        PaletteData_PushTransparentBuffers(app->paletteData);
    }
    DoScheduledBgGpuUpdates(app->bgConfig);
    SpriteSystem_TransferOam();
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

void FrontierRecordsApp_SetGraphicsBanks(FrontierRecordsApp *app) {
    GraphicsBanks banks = ov86_021E7FBC;
    GfGfx_SetBanks(&banks);
}

#pragma force_active on
void FrontierRecordsApp_InitBgLayers(BgConfig *bgConfig) {
    GraphicsModes modes = ov86_021E7EC8;
    BgTemplate mainText;
    BgTemplate mainRecords;
    BgTemplate subText;
    BgTemplate subBackground;

    SetBothScreensModesAndDisable(&modes);
    mainText = ov86_021E7F04;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &mainText, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_121);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    mainRecords = ov86_021E7F20;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &mainRecords, GF_BG_TYPE_TEXT);
    subText = ov86_021E7F3C;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &subText, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_121);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    subBackground = ov86_021E7F58;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &subBackground, GF_BG_TYPE_TEXT);
}

void FrontierRecordsApp_InitGraphics(FrontierRecordsApp *app) {
    FrontierRecordsApp_SetGraphicsBanks(app);
    FrontierRecordsApp_InitBgLayers(app->bgConfig);
    app->paletteData = PaletteData_Init(HEAP_ID_121);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_OBJ, 0x200, HEAP_ID_121);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_121);
    FrontierRecordsApp_LoadMainBgGraphics(app, GF_BG_LYR_MAIN_2);
    FrontierRecordsApp_LoadSubBgGraphics(app, GF_BG_LYR_SUB_2);
}
#pragma force_active reset

void FrontierRecordsApp_FreeBgLayers(BgConfig *bgConfig) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_ALL, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_ALL, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    Heap_Free(bgConfig);
}

void FrontierRecordsApp_SetMainState(FrontierRecordsApp *app, int *state, int nextState) {
    app->state = 0;
    *state = nextState;
}

u32 FrontierRecordsApp_GetWindowSet(u32 facility) {
    u32 set = 0;

    switch (facility) {
    case FRONTIER_RECORDS_FACTORY_LV50:
    case FRONTIER_RECORDS_FACTORY_OPEN:
        set = 0;
        break;
    case FRONTIER_RECORDS_HALL:
        set = 1;
        break;
    case FRONTIER_RECORDS_CASTLE:
        set = 2;
        break;
    case FRONTIER_RECORDS_ARCADE:
        set = 3;
        break;
    case FRONTIER_RECORDS_TOWER:
        set = 4;
        break;
    }
    return set;
}

void FrontierRecordsApp_LoadMainBgGraphics(FrontierRecordsApp *app, GFBgLayer layer) {
    NNSG2dScreenData *screenData;
    NARC *narc = NARC_New(NARC_a_1_8_3, HEAP_ID_121);

    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x8C, app->bgConfig, layer, 0, 0, TRUE, HEAP_ID_121);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0xC3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x60, HEAP_ID_121);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x8B, app->bgConfig, layer, 0, 0, TRUE, HEAP_ID_121);
    void *arrowsFile = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 0x8F, TRUE, &screenData, HEAP_ID_121);
    MIi_CpuCopy16((const u16 *)screenData->rawData, (u16 *)app->pageArrowTiles, sizeof(app->pageArrowTiles));
    Heap_Free(arrowsFile);
    NARC_Delete(narc);
}

void FrontierRecordsApp_LoadSubBgGraphics(FrontierRecordsApp *app, GFBgLayer layer) {
    NARC *narc = NARC_New(NARC_a_1_8_3, HEAP_ID_121);

    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x8E, app->bgConfig, layer, 0, 0, TRUE, HEAP_ID_121);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x8D, app->bgConfig, layer, 0, 0, TRUE, HEAP_ID_121);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0xC4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_121);
    NARC_Delete(narc);
}

void FrontierRecordsApp_BufferNumber(FrontierRecordsApp *app, u32 field, u32 value) {
    BufferIntegerAsString(app->msgFormat, field, value, 4, PRINTING_MODE_RIGHT_ALIGN, TRUE);
}

// align: 0 = left, 1 = right of x, 2 = centered on x
void FrontierRecordsApp_PrintStringAligned(Window *window, String *string, u32 x, u32 y, u32 fontId, u32 color, u32 align) {
    if (align == 1) {
        x -= FontID_String_GetWidth(fontId, string, 0);
    } else if (align == 2) {
        x -= FontID_String_GetWidth(fontId, string, 0) / 2;
    }
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, 0xFF, color, NULL);
}

void FrontierRecordsApp_PrintMessage(FrontierRecordsApp *app, u32 windowId, u32 msgId, u32 x, u32 y, u32 fontId, u32 color, u32 align) {
    String *string = NewString_ReadMsgData(app->msgData, msgId);
    FrontierRecordsApp_PrintStringAligned(&app->windows[windowId], string, x, y, fontId, color, align);
    String_Delete(string);
}

void FrontierRecordsApp_PrintFormattedMessage(FrontierRecordsApp *app, u32 windowId, u32 msgId, u32 x, u32 y, u32 fontId, u32 color, u32 align) {
    String *string = NewString_ReadMsgData(app->msgData, msgId);
    StringExpandPlaceholders(app->msgFormat, app->string, string);
    FrontierRecordsApp_PrintStringAligned(&app->windows[windowId], app->string, x, y, fontId, color, align);
    String_Delete(string);
}

void FrontierRecordsApp_PrintRecords(FrontierRecordsApp *app) {
    switch (app->facility) {
    case FRONTIER_RECORDS_FACTORY_LV50:
    case FRONTIER_RECORDS_FACTORY_OPEN:
        FrontierRecordsApp_PrintFactoryLevel50Records(app);
        FrontierRecordsApp_PrintFactoryOpenLevelRecords(app);
        break;
    case FRONTIER_RECORDS_HALL:
        FrontierRecordsApp_PrintHallRecords(app);
        break;
    case FRONTIER_RECORDS_CASTLE:
        FrontierRecordsApp_PrintCastleRecords(app);
        break;
    case FRONTIER_RECORDS_ARCADE:
        FrontierRecordsApp_PrintArcadeRecords(app);
        break;
    case FRONTIER_RECORDS_TOWER:
        if (app->mode <= 1) {
            FrontierRecordsApp_PrintTowerRecords(app);
        } else {
            FrontierRecordsApp_PrintTowerMultiRecords(app);
        }
        break;
    default:
        FrontierRecordsApp_PrintArcadeRecords(app);
        break;
    }
}

void FrontierRecordsApp_PrintFactoryLevel50Records(FrontierRecordsApp *app) {
    u32 msgOrValue;

    FillWindowPixelBuffer(&app->windows[0], 0);
    FillWindowPixelBuffer(&app->windows[1], 0);
    FillWindowPixelBuffer(&app->windows[2], 0);
    FillWindowPixelBuffer(&app->windows[3], 0);
    FrontierRecordsApp_PrintMessage(app, 0, msg_0019_00022, 0, 0, 0, COLOR_HEADER, 0);
    if (app->mode == 0) {
        msgOrValue = msg_0019_00026;
    } else if (app->mode == 1) {
        msgOrValue = msg_0019_00027;
    } else {
        msgOrValue = msg_0019_00028;
    }
    FrontierRecordsApp_PrintMessage(app, 0, msgOrValue, 224, 0, 0, COLOR_HEADER, 1);
    FrontierRecordsApp_PrintMessage(app, 1, msg_0019_00046, 0, 0, 0, COLOR_TEXT, 0);
    FrontierRecordsApp_PrintMessage(app, 1, msg_0019_00048, 224, 0, 0, COLOR_TEXT, 1);
    FrontierRecordsApp_PrintMessage(app, 2, FrontierRecordsApp_GetFactoryStreakLabel(app, 0), 0, 0, 0, COLOR_TEXT, 0);
    msgOrValue = FrontierSave_GetStat(app->frontierSave, BattleFactory_GetWinStreakStat(0, app->mode), 0xFF);
    FrontierRecordsApp_BufferNumber(app, 0, msgOrValue);
    FrontierRecordsApp_PrintFormattedMessage(app, 2, msg_0019_00050, 112, 0, 0, COLOR_TEXT, 2);
    msgOrValue = FrontierSave_GetStat(app->frontierSave, BattleFactory_GetTradeCountStat(0, app->mode), 0xFF);
    FrontierRecordsApp_BufferNumber(app, 0, msgOrValue);
    FrontierRecordsApp_PrintFormattedMessage(app, 2, msg_0019_00052, 224, 0, 0, COLOR_TEXT, 1);
    FrontierRecordsApp_PrintMessage(app, 3, msg_0019_00043, 0, 0, 0, COLOR_TEXT, 0);
    msgOrValue = FrontierSave_GetStat(app->frontierSave, BattleFactory_GetBestStreakStat(0, app->mode), 0xFF);
    FrontierRecordsApp_BufferNumber(app, 0, msgOrValue);
    FrontierRecordsApp_PrintFormattedMessage(app, 3, msg_0019_00050, 112, 0, 0, COLOR_TEXT, 2);
    msgOrValue = FrontierSave_GetStat(app->frontierSave, BattleFactory_GetBestTradeCountStat(0, app->mode), 0xFF);
    FrontierRecordsApp_BufferNumber(app, 0, msgOrValue);
    FrontierRecordsApp_PrintFormattedMessage(app, 3, msg_0019_00052, 224, 0, 0, COLOR_TEXT, 1);
    ScheduleWindowCopyToVram(&app->windows[0]);
    ScheduleWindowCopyToVram(&app->windows[1]);
    ScheduleWindowCopyToVram(&app->windows[2]);
    ScheduleWindowCopyToVram(&app->windows[3]);
}

void FrontierRecordsApp_PrintFactoryOpenLevelRecords(FrontierRecordsApp *app) {
    u32 value;

    FillWindowPixelBuffer(&app->windows[4], 0);
    FillWindowPixelBuffer(&app->windows[5], 0);
    FillWindowPixelBuffer(&app->windows[6], 0);
    FrontierRecordsApp_PrintMessage(app, 4, msg_0019_00047, 0, 0, 0, COLOR_TEXT, 0);
    FrontierRecordsApp_PrintMessage(app, 4, msg_0019_00048, 224, 0, 0, COLOR_TEXT, 1);
    FrontierRecordsApp_PrintMessage(app, 5, FrontierRecordsApp_GetFactoryStreakLabel(app, 1), 0, 0, 0, COLOR_TEXT, 0);
    value = FrontierSave_GetStat(app->frontierSave, BattleFactory_GetWinStreakStat(1, app->mode), 0xFF);
    FrontierRecordsApp_BufferNumber(app, 0, value);
    FrontierRecordsApp_PrintFormattedMessage(app, 5, msg_0019_00050, 112, 0, 0, COLOR_TEXT, 2);
    value = FrontierSave_GetStat(app->frontierSave, BattleFactory_GetTradeCountStat(1, app->mode), 0xFF);
    FrontierRecordsApp_BufferNumber(app, 0, value);
    FrontierRecordsApp_PrintFormattedMessage(app, 5, msg_0019_00052, 224, 0, 0, COLOR_TEXT, 1);
    FrontierRecordsApp_PrintMessage(app, 6, msg_0019_00043, 0, 0, 0, COLOR_TEXT, 0);
    value = FrontierSave_GetStat(app->frontierSave, BattleFactory_GetBestStreakStat(1, app->mode), 0xFF);
    FrontierRecordsApp_BufferNumber(app, 0, value);
    FrontierRecordsApp_PrintFormattedMessage(app, 6, msg_0019_00050, 112, 0, 0, COLOR_TEXT, 2);
    value = FrontierSave_GetStat(app->frontierSave, BattleFactory_GetBestTradeCountStat(1, app->mode), 0xFF);
    FrontierRecordsApp_BufferNumber(app, 0, value);
    FrontierRecordsApp_PrintFormattedMessage(app, 6, msg_0019_00052, 224, 0, 0, COLOR_TEXT, 1);
    ScheduleWindowCopyToVram(&app->windows[4]);
    ScheduleWindowCopyToVram(&app->windows[5]);
    ScheduleWindowCopyToVram(&app->windows[6]);
}

// Returns CURRENT if the streak is still running, PREV otherwise
u32 FrontierRecordsApp_GetFactoryStreakLabel(FrontierRecordsApp *app, u32 level) {
    u32 stat;
    u32 result;

    if (level == 0) {
        stat = 0x66;
    } else {
        stat = 0x68;
    }
    u8 mode = app->mode;
    if (mode == 3) {
        FrontierSave *frontierSave = Save_Frontier_GetStatic(app->saveData);
        result = FrontierSave_GetStat(frontierSave, stat, FrontierStat_GetFriendIndex(stat));
    } else {
        int factory = Save_Frontier_GetFactoryFlags(app->saveData);
        result = FactoryFlags_GetField(factory, 10, (u8)(mode + level * 4), 0);
    }
    return (u8)result == 1 ? msg_0019_00030 : msg_0019_00029;
}

void FrontierRecordsApp_PrintHallRecords(FrontierRecordsApp *app) {
    FrontierSave *frontierSave;
    u32 stat;
    u32 value;
    u32 flag;
    u32 *records;

    FillWindowPixelBuffer(&app->windows[0], 0);
    FillWindowPixelBuffer(&app->windows[1], 0);
    FillWindowPixelBuffer(&app->windows[2], 0);
    FillWindowPixelBuffer(&app->windows[3], 0);
    FrontierRecordsApp_PrintMessage(app, 0, msg_0019_00024, 0, 0, 0, COLOR_HEADER, 0);
    if (app->mode == 0) {
        value = msg_0019_00026;
    } else if (app->mode == 1) {
        value = msg_0019_00027;
    } else {
        value = msg_0019_00028;
    }
    FrontierRecordsApp_PrintMessage(app, 0, value, 224, 0, 0, COLOR_HEADER, 1);
    FrontierRecordsApp_BufferSpeciesName(app, app->species);
    FrontierRecordsApp_PrintFormattedMessage(app, 1, msg_0019_00051, 0, 0, 0, COLOR_TEXT, 0);
    FrontierRecordsApp_PrintMessage(app, 2, msg_0019_00029, 0, 0, 0, COLOR_TEXT, 0);
    frontierSave = Save_Frontier_GetStatic(app->saveData);
    stat = BattleHall_GetSpeciesStat(app->mode);
    value = FrontierSave_GetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleHall_GetSpeciesStat(app->mode)));
    if (app->species != value) {
        value = 0;
    } else {
        value = FrontierSave_GetStat(app->frontierSave, BattleHall_GetWinStreakStat(app->mode), 0xFF);
    }
    FrontierRecordsApp_BufferNumber(app, 0, value);
    FrontierRecordsApp_PrintFormattedMessage(app, 2, msg_0019_00050, 112, 0, 0, COLOR_TEXT, 2);
    FrontierRecordsApp_PrintMessage(app, 3, msg_0019_00043, 0, 0, 0, COLOR_TEXT, 0);
    records = FrontierSpeciesRecords_Load(app->saveData, 11, &flag);
    if (flag != 1) {
        value = 0;
    } else {
        value = FrontierSpeciesRecords_Get(app->saveData, records, BattleHall_GetSpeciesRecordsType(app->mode), app->species);
    }
    if (records != NULL) {
        Heap_Free(records);
    }
    FrontierRecordsApp_BufferNumber(app, 0, value);
    FrontierRecordsApp_PrintFormattedMessage(app, 3, msg_0019_00050, 112, 0, 0, COLOR_TEXT, 2);
    ScheduleWindowCopyToVram(&app->windows[0]);
    ScheduleWindowCopyToVram(&app->windows[1]);
    ScheduleWindowCopyToVram(&app->windows[2]);
    ScheduleWindowCopyToVram(&app->windows[3]);
}

void FrontierRecordsApp_BufferSpeciesName(FrontierRecordsApp *app, int species) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0237_bin, HEAP_ID_121);
    String *name = NewString_ReadMsgData(msgData, species);
    DestroyMsgData(msgData);
    BufferString(app->msgFormat, 0, name, 0, 0, 2);
    String_Delete(name);
}

void FrontierRecordsApp_PrintCastleRecords(FrontierRecordsApp *app) {
    u32 msg, stat;
    FrontierSave *frontierSave;

    FillWindowPixelBuffer(&app->windows[0], 0);
    FillWindowPixelBuffer(&app->windows[1], 0);
    FillWindowPixelBuffer(&app->windows[2], 0);
    FillWindowPixelBuffer(&app->windows[3], 0);
    FrontierRecordsApp_PrintMessage(app, 0, msg_0019_00023, 0, 0, 0, COLOR_HEADER, 0);
    if (app->mode == 0) {
        msg = msg_0019_00026;
    } else if (app->mode == 1) {
        msg = msg_0019_00027;
    } else {
        msg = msg_0019_00028;
    }
    FrontierRecordsApp_PrintMessage(app, 0, msg, 224, 0, 0, COLOR_HEADER, 1);
    FrontierRecordsApp_PrintMessage(app, 1, msg_0019_00049, 112, 0, 0, COLOR_TEXT, 1);
    FrontierRecordsApp_PrintMessage(app, 2, FrontierRecordsApp_GetCastleStreakLabel(app), 0, 0, 0, COLOR_TEXT, 0);
    FrontierRecordsApp_BufferNumber(app, 0, FrontierSave_GetStat(app->frontierSave, BattleCastle_GetWinStreakStat(app->mode), 0xFF));
    FrontierRecordsApp_PrintFormattedMessage(app, 2, msg_0019_00054, 112, 0, 0, COLOR_TEXT, 2);
    frontierSave = Save_Frontier_GetStatic(app->saveData);
    stat = BattleCastle_GetPointsStat(app->mode);
    FrontierRecordsApp_BufferNumber(app, 0, FrontierSave_GetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(app->mode))));
    FrontierRecordsApp_PrintFormattedMessage(app, 2, msg_0019_00053, 224, 0, 0, COLOR_TEXT, 1);
    FrontierRecordsApp_PrintMessage(app, 3, msg_0019_00043, 0, 0, 0, COLOR_TEXT, 0);
    FrontierRecordsApp_BufferNumber(app, 0, FrontierSave_GetStat(app->frontierSave, BattleCastle_GetBestStreakStat(app->mode), 0xFF));
    FrontierRecordsApp_PrintFormattedMessage(app, 3, msg_0019_00054, 112, 0, 0, COLOR_TEXT, 2);
    frontierSave = Save_Frontier_GetStatic(app->saveData);
    stat = BattleCastle_GetBestStreakPointsStat(app->mode);
    FrontierRecordsApp_BufferNumber(app, 0, FrontierSave_GetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetBestStreakPointsStat(app->mode))));
    FrontierRecordsApp_PrintFormattedMessage(app, 3, msg_0019_00053, 224, 0, 0, COLOR_TEXT, 1);
    ScheduleWindowCopyToVram(&app->windows[0]);
    ScheduleWindowCopyToVram(&app->windows[1]);
    ScheduleWindowCopyToVram(&app->windows[2]);
    ScheduleWindowCopyToVram(&app->windows[3]);
}

u32 FrontierRecordsApp_GetCastleStreakLabel(FrontierRecordsApp *app) {
    u32 result;
    u8 mode = app->mode;

    if (mode == 3) {
        FrontierSave *frontierSave = Save_Frontier_GetStatic(app->saveData);
        result = FrontierSave_GetStat(frontierSave, 0x6C, FrontierStat_GetFriendIndex(0x6C));
    } else {
        result = CastleFlags_GetField(Save_Frontier_GetCastleFlags(app->saveData), 9, mode, 0, 0);
    }
    return (u8)result == 1 ? msg_0019_00030 : msg_0019_00029;
}

void FrontierRecordsApp_PrintArcadeRecords(FrontierRecordsApp *app) {
    u32 msg;

    FillWindowPixelBuffer(&app->windows[0], 0);
    FillWindowPixelBuffer(&app->windows[1], 0);
    FillWindowPixelBuffer(&app->windows[2], 0);
    FrontierRecordsApp_PrintMessage(app, 0, msg_0019_00025, 0, 0, 0, COLOR_HEADER, 0);
    if (app->mode == 0) {
        msg = msg_0019_00026;
    } else if (app->mode == 1) {
        msg = msg_0019_00027;
    } else {
        msg = msg_0019_00028;
    }
    FrontierRecordsApp_PrintMessage(app, 0, msg, 224, 0, 0, COLOR_HEADER, 1);
    FrontierRecordsApp_PrintMessage(app, 1, FrontierRecordsApp_GetArcadeStreakLabel(app), 0, 0, 0, COLOR_TEXT, 0);
    FrontierRecordsApp_BufferNumber(app, 0, FrontierSave_GetStat(app->frontierSave, BattleArcade_GetWinStreakStat(app->mode), 0xFF));
    FrontierRecordsApp_PrintFormattedMessage(app, 1, msg_0019_00055, 112, 0, 0, COLOR_TEXT, 2);
    FrontierRecordsApp_PrintMessage(app, 2, msg_0019_00043, 0, 0, 0, COLOR_TEXT, 0);
    FrontierRecordsApp_BufferNumber(app, 0, FrontierSave_GetStat(app->frontierSave, BattleArcade_GetBestStreakStat(app->mode), 0xFF));
    FrontierRecordsApp_PrintFormattedMessage(app, 2, msg_0019_00055, 112, 0, 0, COLOR_TEXT, 2);
    ScheduleWindowCopyToVram(&app->windows[0]);
    ScheduleWindowCopyToVram(&app->windows[1]);
    ScheduleWindowCopyToVram(&app->windows[2]);
}

u32 FrontierRecordsApp_GetArcadeStreakLabel(FrontierRecordsApp *app) {
    u32 result;
    u8 mode = app->mode;

    if (mode == 3) {
        FrontierSave *frontierSave = Save_Frontier_GetStatic(app->saveData);
        result = FrontierSave_GetStat(frontierSave, 0x6E, FrontierStat_GetFriendIndex(0x6E));
    } else {
        result = ArcadeFlags_GetField(Save_Frontier_GetArcadeFlags(app->saveData), 8, mode, 0, 0);
    }
    return (u8)result == 1 ? msg_0019_00030 : msg_0019_00029;
}

void FrontierRecordsApp_PrintTowerRecords(FrontierRecordsApp *app) {
    u32 msg;

    FillWindowPixelBuffer(&app->windows[0], 0);
    FillWindowPixelBuffer(&app->windows[2], 0);
    FillWindowPixelBuffer(&app->windows[3], 0);
    FrontierRecordsApp_PrintMessage(app, 0, msg_0019_00021, 0, 0, 0, COLOR_HEADER, 0);
    msg = app->mode == 0 ? msg_0019_00026 : msg_0019_00027;
    FrontierRecordsApp_PrintMessage(app, 0, msg, 224, 0, 0, COLOR_HEADER, 1);
    FrontierRecordsApp_PrintMessage(app, 2, FrontierRecordsApp_GetTowerStreakLabel(app, app->mode), 0, 0, 0, COLOR_TEXT, 0);
    FrontierRecordsApp_BufferNumber(app, 0, FrontierSave_GetStat(app->frontierSave, BattleTower_GetWinStreakStat(app->mode), 0xFF));
    FrontierRecordsApp_PrintFormattedMessage(app, 2, msg_0019_00050, 112, 0, 0, COLOR_TEXT, 2);
    FrontierRecordsApp_PrintMessage(app, 3, msg_0019_00043, 0, 0, 0, COLOR_TEXT, 0);
    FrontierRecordsApp_BufferNumber(app, 0, FrontierSave_GetStat(app->frontierSave, BattleTower_GetBestStreakStat(app->mode), 0xFF));
    FrontierRecordsApp_PrintFormattedMessage(app, 3, msg_0019_00050, 112, 0, 0, COLOR_TEXT, 2);
    ScheduleWindowCopyToVram(&app->windows[0]);
    ScheduleWindowCopyToVram(&app->windows[2]);
    ScheduleWindowCopyToVram(&app->windows[3]);
}

static inline void FrontierRecordsApp_PrintTowerStreak(FrontierRecordsApp *app, u32 windowId, u32 stat) {
    FrontierRecordsApp_BufferNumber(app, 0, FrontierSave_GetStat(app->frontierSave, stat, 0xFF));
    FrontierRecordsApp_PrintFormattedMessage(app, windowId, msg_0019_00050, 112, 0, 0, COLOR_TEXT, 2);
}

#pragma force_active on
void FrontierRecordsApp_PrintTowerMultiRecords(FrontierRecordsApp *app) {
    FillWindowPixelBuffer(&app->windows[0], 0);
    FillWindowPixelBuffer(&app->windows[1], 0);
    FillWindowPixelBuffer(&app->windows[4], 0);
    FillWindowPixelBuffer(&app->windows[5], 0);
    FillWindowPixelBuffer(&app->windows[6], 0);
    FillWindowPixelBuffer(&app->windows[7], 0);
    FillWindowPixelBuffer(&app->windows[8], 0);
    FrontierRecordsApp_PrintMessage(app, 0, msg_0019_00021, 0, 0, 0, COLOR_HEADER, 0);
    FrontierRecordsApp_PrintMessage(app, 0, msg_0019_00028, 224, 0, 0, COLOR_HEADER, 1);
    FrontierRecordsApp_PrintMessage(app, 1, msg_0019_00044, 0, 0, 0, COLOR_TEXT, 0);
    FrontierRecordsApp_PrintMessage(app, 4, FrontierRecordsApp_GetTowerStreakLabel(app, app->mode), 0, 0, 0, COLOR_TEXT, 0);
    FrontierRecordsApp_PrintTowerStreak(app, 4, BattleTower_GetWinStreakStat(app->mode));
    FrontierRecordsApp_PrintMessage(app, 5, msg_0019_00043, 0, 0, 0, COLOR_TEXT, 0);
    FrontierRecordsApp_PrintTowerStreak(app, 5, BattleTower_GetBestStreakStat(app->mode));
    FrontierRecordsApp_PrintMessage(app, 6, msg_0019_00045, 0, 0, 0, COLOR_TEXT, 0);
    FrontierRecordsApp_PrintMessage(app, 7, FrontierRecordsApp_GetTowerStreakLabel(app, 3), 0, 0, 0, COLOR_TEXT, 0);
    FrontierRecordsApp_PrintTowerStreak(app, 7, BattleTower_GetWinStreakStat(3));
    FrontierRecordsApp_PrintMessage(app, 8, msg_0019_00043, 0, 0, 0, COLOR_TEXT, 0);
    FrontierRecordsApp_PrintTowerStreak(app, 8, BattleTower_GetBestStreakStat(3));
    ScheduleWindowCopyToVram(&app->windows[0]);
    ScheduleWindowCopyToVram(&app->windows[1]);
    ScheduleWindowCopyToVram(&app->windows[4]);
    ScheduleWindowCopyToVram(&app->windows[5]);
    ScheduleWindowCopyToVram(&app->windows[6]);
    ScheduleWindowCopyToVram(&app->windows[7]);
    ScheduleWindowCopyToVram(&app->windows[8]);
}
#pragma force_active reset

u32 FrontierRecordsApp_GetTowerStreakLabel(FrontierRecordsApp *app, u32 mode) {
    FrontierData *frontierData = Save_FrontierData_Get(app->saveData);
    u16 index;

    switch (mode) {
    case 0:
        index = 8;
        break;
    case 1:
        index = 9;
        break;
    case 2:
        index = 10;
        break;
    case 3:
        index = 11;
        break;
    case 4:
        index = 12;
        break;
    case 6:
        index = 15;
        break;
    default:
        index = 8;
        break;
    }
    return FrontierData_FlagAction(frontierData, index, 0) == 1 ? msg_0019_00030 : msg_0019_00029;
}

void FrontierRecordsApp_PrintReturnButton(FrontierRecordsApp *app) {
    String *string = NewString_ReadMsgData(app->msgData, msg_0019_00039);
    FillWindowPixelBuffer(&app->returnButtonWindow, 0);
    FrontierRecordsApp_PrintStringAligned(&app->returnButtonWindow, string, GetWindowWidth(&app->returnButtonWindow) * 8 / 2, 0, 4, COLOR_BUTTON, 2);
    ScheduleWindowCopyToVram(&app->returnButtonWindow);
    String_Delete(string);
}

#pragma force_active on
void FrontierRecordsApp_CreateSprites(FrontierRecordsApp *app) {
    OamManagerParam oamParam;
    OamCharTransferParam transferParam;
    SpriteResourceCountsListUnion capacities;
    NARC *narc;
    u32 i;

    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    app->spriteSystem = SpriteSystem_Alloc(HEAP_ID_121);
    app->spriteManager = SpriteManager_New(app->spriteSystem);
    oamParam = ov86_021E7F74;
    transferParam = ov86_021E7ED8;
    SpriteSystem_Init(app->spriteSystem, &oamParam, &transferParam, 32);
    capacities = ov86_021E7EEC;
    SpriteSystem_InitSprites(app->spriteSystem, app->spriteManager, 2);
    SpriteSystem_InitManagerWithCapacities(app->spriteSystem, app->spriteManager, &capacities);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(app->spriteSystem), 0, FX32_CONST(512));
    narc = NARC_New(NARC_a_1_8_4, HEAP_ID_121);
    SpriteSystem_LoadCharResObjFromOpenNarc(app->spriteSystem, app->spriteManager, narc, 51, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 55500);
    SpriteSystem_LoadPlttResObjFromOpenNarc(app->spriteSystem, app->spriteManager, narc, 64, FALSE, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 55500);
    SpriteSystem_LoadCellResObjFromOpenNarc(app->spriteSystem, app->spriteManager, narc, 49, TRUE, 55500);
    SpriteSystem_LoadAnimResObjFromOpenNarc(app->spriteSystem, app->spriteManager, narc, 50, TRUE, 55500);
    NARC_Delete(narc);
    for (i = 0; i < 2; i++) {
        app->sprites[i] = SpriteSystem_NewSprite(app->spriteSystem, app->spriteManager, &ov86_021E802C[i]);
    }
}
#pragma force_active reset

void FrontierRecordsApp_FreeSprites(FrontierRecordsApp *app) {
    u32 i;

    for (i = 0; i < 2; i++) {
        if (app->sprites[i] != NULL) {
            Sprite_DeleteAndFreeResources(app->sprites[i]);
            app->sprites[i] = NULL;
        }
    }
    SpriteSystem_FreeResourcesAndManager(app->spriteSystem, app->spriteManager);
    SpriteSystem_Free(app->spriteSystem);
}

void FrontierRecordsApp_TickSprites(FrontierRecordsApp *app) {
    u32 i;

    for (i = 0; i < 2; i++) {
        if (app->sprites[i] != NULL) {
            ManagedSprite_TickFrame(app->sprites[i]);
        }
    }
}

void FrontierRecordsApp_SetSpriteAnim(FrontierRecordsApp *app, int index, int anim) {
    ManagedSprite_SetAnimationFrame(app->sprites[index], 0);
    ManagedSprite_SetAnim(app->sprites[index], anim);
}

void FrontierRecordsApp_SetSpritePosition(FrontierRecordsApp *app, int index, s16 x, s16 y) {
    ManagedSprite_SetPositionXY(app->sprites[index], x, y);
}

#pragma force_active on
// Returns 0 once the flash has finished
int FrontierRecordsApp_RunButtonFlash(FrontierRecordsApp *app) {
    switch (app->flash.state) {
    case 0:
        if (!app->flash.isBg) {
            ManagedSprite_SetPaletteOverride(app->sprites[app->flash.index], app->flash.paletteOn);
        } else {
            BgTilemapRectChangePalette(app->bgConfig, app->flash.index, app->flash.x, app->flash.y, app->flash.width, app->flash.height, app->flash.paletteOn);
            ScheduleBgTilemapBufferTransfer(app->bgConfig, app->flash.index);
        }
        app->flash.state++;
        break;
    case 1:
        app->flash.timer++;
        if (app->flash.timer != 4) {
            break;
        }
        if (!app->flash.isBg) {
            ManagedSprite_SetPaletteOverride(app->sprites[app->flash.index], app->flash.paletteOff);
        } else {
            BgTilemapRectChangePalette(app->bgConfig, app->flash.index, app->flash.x, app->flash.y, app->flash.width, app->flash.height, app->flash.paletteOff);
            ScheduleBgTilemapBufferTransfer(app->bgConfig, app->flash.index);
        }
        app->flash.timer = 0;
        app->flash.state++;
        break;
    case 2:
        app->flash.timer++;
        if (app->flash.timer == 2) {
            return 0;
        }
        break;
    }
    return 1;
}

void FrontierRecordsApp_StartReturnButtonFlash(FrontierRecordsApp *app) {
    app->flash.isBg = FALSE;
    app->flash.index = 0;
    app->flash.paletteOn = 1;
    app->flash.paletteOff = 0;
    app->flash.state = 0;
    app->flash.timer = 0;
}
#pragma force_active reset

BOOL FrontierRecordsApp_IsHall(FrontierRecordsApp *app) {
    return app->facility == FRONTIER_RECORDS_HALL;
}

void FrontierRecordsApp_InitHallPicker(FrontierRecordsApp *app) {
    FrontierRecordsApp_LoadWindowFrame(app);
    FrontierRecordsApp_InitSpeciesGroups(app);
    FrontierRecordsApp_PrintLetters(app);
    FrontierRecordsApp_ShowHallPage(app, HALL_PAGE_LETTERS);
    app->scrollArrows = PaletteBlink_New(NULL, PM_LCD_BOTTOM, 13, HEAP_ID_121);
}

void FrontierRecordsApp_FreeHallPicker(FrontierRecordsApp *app) {
    PaletteBlink_Delete(app->scrollArrows);
    FrontierRecordsApp_FreeSpeciesGroups(app);
    FrontierRecordsApp_FreeGridInput(app);
}

#pragma force_active on
void FrontierRecordsApp_ShowHallPage(FrontierRecordsApp *app, int page) {
    switch (page) {
    case HALL_PAGE_RECORDS:
        ScheduleSetBgPosText(app->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
        ScheduleSetBgPosText(app->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0);
        FrontierRecordsApp_ClearSpeciesWindows(app);
        FrontierRecordsApp_FreeGridInput(app);
        FrontierRecordsApp_ResetCursor(app);
        FrontierRecordsApp_SelectSpecies(app);
        FrontierRecordsApp_ClearPrompt(app);
        break;
    case HALL_PAGE_LETTERS:
        ScheduleSetBgPosText(app->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
        ScheduleSetBgPosText(app->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 256);
        FrontierRecordsApp_ClearSpeciesWindows(app);
        ScheduleWindowCopyToVram(&app->windows[4]);
        FrontierRecordsApp_FreeGridInput(app);
        FrontierRecordsApp_CreateLetterGrid(app);
        FrontierRecordsApp_GrayOutEmptyLetters(app);
        FrontierRecordsApp_ShowPrompt(app);
        break;
    case HALL_PAGE_SPECIES:
        ScheduleSetBgPosText(app->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 256);
        ScheduleSetBgPosText(app->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 256);
        ClearWindowTilemapAndScheduleTransfer(&app->windows[4]);
        FrontierRecordsApp_ClearRecordWindows(app);
        FrontierRecordsApp_PrintSpeciesPage(app);
        FrontierRecordsApp_PrintPageNumber(app);
        FrontierRecordsApp_DrawPageArrows(app);
        FrontierRecordsApp_PrintLetterTitle(app);
        FrontierRecordsApp_FreeGridInput(app);
        FrontierRecordsApp_CreateSpeciesGrid(app);
        FrontierRecordsApp_ShowPrompt(app);
        break;
    }
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_2);
}

// Arrow tiles: 0 = left, 1 = right, 2 = disabled
void FrontierRecordsApp_DrawPageArrows(FrontierRecordsApp *app) {
    if (app->lastPage == 0) {
        LoadRectToBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, app->pageArrowTiles[2], 34, 52, 4, 4);
        LoadRectToBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, app->pageArrowTiles[2], 38, 52, 4, 4);
    } else {
        LoadRectToBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, app->pageArrowTiles[0], 34, 52, 4, 4);
        LoadRectToBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, app->pageArrowTiles[1], 38, 52, 4, 4);
    }
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_2);
}
#pragma force_active reset

void FrontierRecordsApp_FreeGridInput(FrontierRecordsApp *app) {
    if (app->gridInput != NULL) {
        GridInputHandler_Free(app->gridInput);
        app->gridInput = NULL;
    }
}

void FrontierRecordsApp_GridCallback_Nop(void) {
}

#pragma force_active on
// Species of one letter in alphabetical order (members 18-43 of the Pokedex sort data)
u16 *FrontierRecordsApp_LoadSortedSpecies(int letter, u32 *count) {
    u32 size;
    u16 *species = GfGfxLoader_LoadFromNarc_GetSizeOut(GetPokedexDataNarcID(), letter + 18, FALSE, HEAP_ID_121, TRUE, &size);
    *count = size / 2;
    return species;
}

// Keeps the species that have a Battle Hall record
void FrontierRecordsApp_InitSpeciesGroups(FrontierRecordsApp *app) {
    u32 enabled;
    u32 count;
    u32 *records = FrontierSpeciesRecords_Load(app->saveData, 11, &enabled);
    u16 letter;

    if (enabled == 1) {
        for (letter = 0; letter < NUM_LETTERS; letter++) {
            u16 *species = FrontierRecordsApp_LoadSortedSpecies(letter, &count);
            u16 i;
            app->groups[letter].species = Heap_Alloc(HEAP_ID_121, count * 2);
            for (i = 0; i < count; i++) {
                if (FrontierSpeciesRecords_Get(app->saveData, records, BattleHall_GetSpeciesRecordsType(app->mode), species[i])) {
                    app->groups[letter].species[app->groups[letter].count] = species[i];
                    app->groups[letter].count++;
                }
            }
            Heap_Free(species);
        }
    }
    if (records != NULL) {
        Heap_Free(records);
    }
}
#pragma force_active reset

void FrontierRecordsApp_FreeSpeciesGroups(FrontierRecordsApp *app) {
    u32 i;

    for (i = 0; i < NUM_LETTERS; i++) {
        if (app->groups[i].species != NULL) {
            Heap_Free(app->groups[i].species);
        }
    }
}

void FrontierRecordsApp_LoadWindowFrame(FrontierRecordsApp *app) {
    u8 frame = Options_GetFrame(app->options);
    LoadUserFrameGfx2(app->bgConfig, GF_BG_LYR_SUB_0, 0x3E2, 14, frame, HEAP_ID_121);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_121);
}

#pragma force_active on
void FrontierRecordsApp_ShowPrompt(FrontierRecordsApp *app) {
    DrawFrameAndWindow2(&app->windows[23], TRUE, 0x3E2, 14);
    FillWindowPixelBuffer(&app->windows[23], 15);
    FrontierRecordsApp_PrintMessage(app, 23, msg_0019_00042, 0, 0, 1, COLOR_PROMPT, 0);
    ScheduleWindowCopyToVram(&app->windows[23]);
}
#pragma force_active reset

void FrontierRecordsApp_ClearPrompt(FrontierRecordsApp *app) {
    ClearFrameAndWindow2(&app->windows[23], TRUE);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_SUB_0);
}

#pragma force_active on
// Letter buttons are laid out 7 per row, 32 pixels apart
void FrontierRecordsApp_PrintLetters(FrontierRecordsApp *app) {
    u16 i;
    int position, column;

    for (i = 0; i < NUM_LETTERS; i++) {
        position = ov86_021E81DC[i];
        column = position % 7;
        FrontierRecordsApp_PrintMessage(app, 4, i + msg_0019_00056, (column << 5) + 24, (position / 7) << 5, 4, COLOR_BUTTON, 2);
    }
}
#pragma force_active reset

void FrontierRecordsApp_MoveLetterCursor(FrontierRecordsApp *app, int selection) {
    u8 x;
    u8 y;
    const DpadMenuBox *box = GridInputHandler_GetDpadBox(app->gridInput, selection);

    DpadMenuBox_GetPosition(box, &x, &y);
    FrontierRecordsApp_SetSpritePosition(app, 1, x, y);
    if (selection == LETTER_GRID_RETURN) {
        FrontierRecordsApp_SetSpriteAnim(app, 1, 1);
    } else {
        FrontierRecordsApp_SetSpriteAnim(app, 1, 2);
    }
}

#pragma force_active on
void FrontierRecordsApp_CreateLetterGrid(FrontierRecordsApp *app) {
    app->gridInput = GridInputHandler_Create(ov86_021E8094, ov86_021E8104, &ov86_021E7EB8, app, TRUE, (u8)app->letter, HEAP_ID_121);
    FrontierRecordsApp_MoveLetterCursor(app, app->letter);
}
#pragma force_active reset

void FrontierRecordsApp_LetterGrid_OnKeyMove(FrontierRecordsApp *app, int selection) {
    FrontierRecordsApp_MoveLetterCursor(app, selection);
}

void FrontierRecordsApp_LetterGrid_OnTouch(FrontierRecordsApp *app, int selection) {
    FrontierRecordsApp_MoveLetterCursor(app, selection);
}

static inline void FrontierRecordsApp_InitLetterFlash(FrontierRecordsApp *app) {
    app->flash.isBg = TRUE;
    app->flash.index = GF_BG_LYR_MAIN_2;
    app->flash.paletteOn = 2;
    app->flash.paletteOff = 0;
}

#pragma force_active on
#pragma push
#pragma opt_lifetimes off
void FrontierRecordsApp_StartLetterFlash(FrontierRecordsApp *app, int letter) {
    u32 position = letter;

    FrontierRecordsApp_InitLetterFlash(app);
    position = ov86_021E81DC[position];
    app->flash.x = ((position % 7) * 32 + 24) / 8;
    app->flash.y = ((position / 7) * 32 + 40) / 8 + 32;
    app->flash.width = 2;
    app->flash.height = 2;
    app->flash.state = 0;
    app->flash.timer = 0;
}
#pragma pop
#pragma force_active reset

u8 FrontierRecordsApp_HandleLetterInput(FrontierRecordsApp *app) {
    u32 input;
    FrontierRecordsSpeciesGroup *groups;

    input = GridInputHandler_HandleInput_NoHold(app->gridInput);
    switch (input) {
    case LETTER_GRID_RETURN:
    case GRID_MENU_CANCEL:
        PlaySE(SEQ_SE_DP_DECIDE);
        FrontierRecordsApp_StartReturnButtonFlash(app);
        FrontierRecordsApp_FreeHallPicker(app);
        app->nextState = RUN_STATE_EXIT;
        return RUN_STATE_BUTTON_FLASH;
    case GRID_MENU_CURSOR_MOVE:
        PlaySE(SEQ_SE_DP_SELECT);
        goto exit;
    case GRID_MENU_BUTTON_MODE:
    case GRID_MENU_NOTHING_CHOSEN:
        goto exit;
    default:
        groups = app->groups;
        if (groups[input].count == 0) {
            PlaySE(SEQ_SE_DP_CUSTOM06);
            goto exit;
        }
        PlaySE(SEQ_SE_DP_DECIDE);
        app->letter = input;
        app->speciesIndex = 0;
        app->page = 0;
        app->lastPage = (groups[input].count - 1) / SPECIES_PER_PAGE;
        FrontierRecordsApp_StartLetterFlash(app, app->letter);
        app->nextState = RUN_STATE_SHOW_SPECIES;
        return RUN_STATE_BUTTON_FLASH;
    }
exit:
    return RUN_STATE_LETTERS;
}

void FrontierRecordsApp_GrayOutEmptyLetters(FrontierRecordsApp *app) {
    u32 i;

    for (i = 0; i < NUM_LETTERS; i++) {
        if (app->groups[i].count == 0) {
            FrontierRecordsApp_StartLetterFlash(app, i);
            BgTilemapRectChangePalette(app->bgConfig, GF_BG_LYR_MAIN_2, app->flash.x, app->flash.y, app->flash.width, app->flash.height, 1);
        }
    }
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_2);
}

void FrontierRecordsApp_MoveSpeciesCursor(FrontierRecordsApp *app, int selection) {
    u8 x;
    u8 y;
    const DpadMenuBox *box = GridInputHandler_GetDpadBox(app->gridInput, selection);

    DpadMenuBox_GetPosition(box, &x, &y);
    FrontierRecordsApp_SetSpritePosition(app, 1, x, y);
    if (selection == SPECIES_GRID_RETURN) {
        FrontierRecordsApp_SetSpriteAnim(app, 1, 1);
    } else {
        FrontierRecordsApp_SetSpriteAnim(app, 1, 3);
    }
}

#pragma force_active on
void FrontierRecordsApp_CreateSpeciesGrid(FrontierRecordsApp *app) {
    int selection = app->speciesIndex;
    app->gridInput = GridInputHandler_Create(ov86_021E7F94, ov86_021E7FE4, &ov86_021E7EA8, app, TRUE, (u8)selection, HEAP_ID_121);
    FrontierRecordsApp_MoveSpeciesCursor(app, selection);
}
#pragma force_active reset

void FrontierRecordsApp_SpeciesGrid_OnKeyMove(FrontierRecordsApp *app, int selection) {
    FrontierRecordsApp_MoveSpeciesCursor(app, selection);
}

void FrontierRecordsApp_SpeciesGrid_OnTouch(FrontierRecordsApp *app, int selection) {
    FrontierRecordsApp_MoveSpeciesCursor(app, selection);
}

// Species buttons: two columns, 13 tiles apart
void FrontierRecordsApp_StartSpeciesFlash(FrontierRecordsApp *app, int slot) {
    unsigned int position;

    app->flash.isBg = TRUE;
    app->flash.index = GF_BG_LYR_MAIN_2;
    app->flash.paletteOn = 2;
    app->flash.paletteOff = 0;
    position = ov86_021E81DC[slot];
    app->flash.x = (position & 1) * 13 + 38;
    app->flash.y = (position / 2) * 4 + 37;
    app->flash.width = 9;
    app->flash.height = 2;
    app->flash.state = 0;
    app->flash.timer = 0;
}

void FrontierRecordsApp_StartArrowFlash(FrontierRecordsApp *app, u8 x, u8 y) {
    app->flash.isBg = TRUE;
    app->flash.index = GF_BG_LYR_MAIN_2;
    app->flash.paletteOn = 2;
    app->flash.paletteOff = 0;
    app->flash.x = x;
    app->flash.y = y;
    app->flash.width = 4;
    app->flash.height = 4;
    app->flash.state = 0;
    app->flash.timer = 0;
}

static inline void FrontierRecordsApp_PrevPage(FrontierRecordsApp *app) {
    app->page--;
    if (app->page < 0) {
        app->page = app->lastPage;
    }
}

static inline void FrontierRecordsApp_NextPage(FrontierRecordsApp *app) {
    app->page++;
    if (app->page > app->lastPage) {
        app->page = 0;
    }
}

#pragma force_active on
int FrontierRecordsApp_HandleSpeciesInput(FrontierRecordsApp *app) {
    int input = GridInputHandler_HandleInput_NoHold(app->gridInput);

    switch ((u32)input) {
    case SPECIES_GRID_RETURN:
    case GRID_MENU_CANCEL:
        PlaySE(SEQ_SE_DP_DECIDE);
        FrontierRecordsApp_StartReturnButtonFlash(app);
        app->nextState = RUN_STATE_SHOW_LETTERS;
        return RUN_STATE_BUTTON_FLASH;
    case GRID_MENU_CURSOR_MOVE:
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    case GRID_MENU_BUTTON_MODE:
        break;
    case GRID_MENU_NOTHING_CHOSEN:
        // Left/right at the edge of the grid flips the page
        switch (GridInputHandler_GetNextInput(app->gridInput)) {
        case 0:
        case 2:
        case 4:
        case 6:
            if ((gSystem.newKeys & PAD_KEY_LEFT) && app->lastPage != 0) {
                FrontierRecordsApp_PrevPage(app);
                PlaySE(SEQ_SE_DP_SELECT);
                FrontierRecordsApp_PrintSpeciesPage(app);
                FrontierRecordsApp_PrintPageNumber(app);
            }
            break;
        case 1:
        case 3:
        case 5:
        case 7:
            if ((gSystem.newKeys & PAD_KEY_RIGHT) && app->lastPage != 0) {
                FrontierRecordsApp_NextPage(app);
                PlaySE(SEQ_SE_DP_SELECT);
                FrontierRecordsApp_PrintSpeciesPage(app);
                FrontierRecordsApp_PrintPageNumber(app);
            }
            break;
        }
        break;
    default:
        if ((u32)(input + app->page * SPECIES_PER_PAGE) >= app->groups[app->letter].count) {
            PlaySE(SEQ_SE_DP_CUSTOM06);
            break;
        }
        app->speciesIndex = input;
        PlaySE(SEQ_SE_DP_DECIDE);
        FrontierRecordsApp_StartSpeciesFlash(app, input);
        app->nextState = RUN_STATE_SHOW_HALL_RECORDS;
        return RUN_STATE_BUTTON_FLASH;
    }
    switch (TouchscreenHitbox_FindRectAtTouchNew(ov86_021E7E9C)) {
    case 0:
        if (app->lastPage != 0) {
            FrontierRecordsApp_PrevPage(app);
            PlaySE(SEQ_SE_DP_SELECT78);
            FrontierRecordsApp_StartArrowFlash(app, 34, 52);
            app->nextState = RUN_STATE_FLIP_PAGE;
            return RUN_STATE_BUTTON_FLASH;
        }
        break;
    case 1:
        if (app->lastPage != 0) {
            FrontierRecordsApp_NextPage(app);
            PlaySE(SEQ_SE_DP_SELECT78);
            FrontierRecordsApp_StartArrowFlash(app, 38, 52);
            app->nextState = RUN_STATE_FLIP_PAGE;
            return RUN_STATE_BUTTON_FLASH;
        }
        break;
    }
    return RUN_STATE_SPECIES;
}
#pragma force_active reset

int FrontierRecordsApp_FlipSpeciesPage(FrontierRecordsApp *app) {
    FrontierRecordsApp_PrintSpeciesPage(app);
    FrontierRecordsApp_PrintPageNumber(app);
    return RUN_STATE_SPECIES;
}

#pragma force_active on
// The species page alternates between two sets of eight windows (5-12 and 13-20)
void FrontierRecordsApp_PrintSpeciesPage(FrontierRecordsApp *app) {
    int windowId;
    u16 start;
    u8 i;
    u8 palette;

    if (app->speciesWindowBank == 0) {
        windowId = 5;
    } else {
        windowId = 13;
    }
    start = app->page * SPECIES_PER_PAGE;
    for (i = 0; i < SPECIES_PER_PAGE; i++) {
        FillWindowPixelBuffer(&app->windows[windowId + i], 0);
        if (app->groups[app->letter].count > start + i) {
            FrontierRecordsApp_BufferSpeciesName(app, app->groups[app->letter].species[start + i]);
            FrontierRecordsApp_PrintFormattedMessage(app, windowId + i, msg_0019_00019, GetWindowWidth(&app->windows[windowId + i]) * 8 / 2, 0, 4, COLOR_BUTTON, 2);
            palette = 0;
        } else {
            palette = 1;
        }
        ScheduleWindowCopyToVram(&app->windows[windowId + i]);
        FrontierRecordsApp_StartSpeciesFlash(app, i);
        BgTilemapRectChangePalette(app->bgConfig, GF_BG_LYR_MAIN_2, app->flash.x, app->flash.y, app->flash.width, app->flash.height, palette);
    }
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_2);
    app->speciesWindowBank ^= 1;
}
#pragma force_active reset

void FrontierRecordsApp_ClearSpeciesWindows(FrontierRecordsApp *app) {
    u32 i = 5;
    Window *window = &app->windows[5];

    do {
        ClearWindowTilemapAndScheduleTransfer(window);
        i++;
        window++;
    } while (i <= 13);
    ClearWindowTilemapAndScheduleTransfer(&app->windows[21]);
    ClearWindowTilemapAndScheduleTransfer(&app->windows[22]);
}

#pragma force_active on
void FrontierRecordsApp_PrintLetterTitle(FrontierRecordsApp *app) {
    int width;

    FillWindowPixelBuffer(&app->windows[21], 0);
    width = GetWindowWidth(&app->windows[21]);
    FrontierRecordsApp_PrintMessage(app, 21, app->letter + msg_0019_00056, (width * 8) / 2, 0, 4, COLOR_BUTTON, 2);
    ScheduleWindowCopyToVram(&app->windows[21]);
}

void FrontierRecordsApp_PrintPageNumber(FrontierRecordsApp *app) {
    FillWindowPixelBuffer(&app->windows[22], 0);
    FrontierRecordsApp_BufferNumber(app, 0, app->page + 1);
    FrontierRecordsApp_PrintFormattedMessage(app, 22, msg_0019_00040, 8, 0, 0, COLOR_TEXT, 1);
    FrontierRecordsApp_PrintMessage(app, 22, msg_0019_00041, 16, 0, 0, COLOR_TEXT, 2);
    FrontierRecordsApp_BufferNumber(app, 0, app->lastPage + 1);
    FrontierRecordsApp_PrintFormattedMessage(app, 22, msg_0019_00040, 32, 0, 0, COLOR_TEXT, 1);
    ScheduleWindowCopyToVram(&app->windows[22]);
}
#pragma force_active reset

void FrontierRecordsApp_ResetCursor(FrontierRecordsApp *app) {
    FrontierRecordsApp_SetSpritePosition(app, 1, 224, 176);
    FrontierRecordsApp_SetSpriteAnim(app, 1, 1);
}

#pragma force_active on
void FrontierRecordsApp_SelectSpecies(FrontierRecordsApp *app) {
    u16 *species = app->groups[app->letter].species;
    app->species = species[app->speciesIndex + app->page * SPECIES_PER_PAGE];
    FrontierRecordsApp_PrintHallRecords(app);
}
#pragma force_active reset

void FrontierRecordsApp_ClearRecordWindows(FrontierRecordsApp *app) {
    ClearWindowTilemapAndScheduleTransfer(&app->windows[0]);
    ClearWindowTilemapAndScheduleTransfer(&app->windows[1]);
    ClearWindowTilemapAndScheduleTransfer(&app->windows[2]);
    ClearWindowTilemapAndScheduleTransfer(&app->windows[3]);
}

#pragma force_active on
void FrontierRecordsApp_AddWindows(BgConfig *bgConfig, Window *windows, int set) {
    u8 i;
    const WindowTemplate *templates = ov86_021E8230[set].templates;
    u32 count = ov86_021E8230[set].count;

    for (i = 0; i < count; i++) {
        AddWindow(bgConfig, &windows[i], &templates[i]);
        FillWindowPixelBuffer(&windows[i], 0);
    }
}
#pragma force_active reset

void FrontierRecordsApp_RemoveWindows(Window *windows, int set) {
    u16 i;
    u32 count = ov86_021E8230[set].count;

    for (i = 0; i < count; i++) {
        RemoveWindow(&windows[i]);
    }
}

void FrontierRecordsApp_AddReturnButtonWindow(BgConfig *bgConfig, Window *window) {
    AddWindowParameterized(bgConfig, window, GF_BG_LYR_MAIN_0, 25, 21, 6, 2, 13, 0x3F4);
}

void FrontierRecordsApp_RemoveReturnButtonWindow(Window *window) {
    RemoveWindow(window);
}
