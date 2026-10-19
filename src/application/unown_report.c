#include "unown_report.h"

#include "global.h"

#include "constants/species.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0001.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "options.h"
#include "player_data.h"
#include "pokedex.h"
#include "pokemon_icon_idx.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite.h"
#include "sprite_system.h"
#include "string_util.h"
#include "sys_vars.h"
#include "system.h"
#include "text.h"
#include "touchpad.h"
#include "touchscreen.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

// Unown Report: a book listing the Unown forms the player has seen (seven icons per page, two
// pages per spread) after a cover page, followed by research pages unlocked by the report level.

#define UNOWN_ICONS_PER_SPREAD 14

enum UnownReportSetupState {
    UNOWN_REPORT_SETUP_CREATE,
    UNOWN_REPORT_SETUP_DRAW_FIRST_PAGE,
};

enum UnownReportFadeState {
    UNOWN_REPORT_FADE_START,
    UNOWN_REPORT_FADE_WAIT,
};

enum UnownReportTurnPageState {
    UNOWN_REPORT_TURN_PAGE_FADE_OUT,
    UNOWN_REPORT_TURN_PAGE_DRAW,
    UNOWN_REPORT_TURN_PAGE_FADE_IN,
    UNOWN_REPORT_TURN_PAGE_WAIT,
};

enum UnownReportWindow {
    UNOWN_REPORT_WIN_TITLE,
    UNOWN_REPORT_WIN_LEFT_PAGE,
    UNOWN_REPORT_WIN_RIGHT_PAGE,
    UNOWN_REPORT_WIN_FOOTER,
    UNOWN_REPORT_WIN_COUNT,
};

enum UnownReportSprite {
    UNOWN_REPORT_SPRITE_BOOK,
    UNOWN_REPORT_SPRITE_PREV_ARROW,
    UNOWN_REPORT_SPRITE_NEXT_ARROW,
    UNOWN_REPORT_SPRITE_COUNT,
};

enum UnownReportMainState {
    UNOWN_REPORT_MAIN_FADE_IN,
    UNOWN_REPORT_MAIN_FADE_OUT,
    UNOWN_REPORT_MAIN_INPUT,
    UNOWN_REPORT_MAIN_TURN_PAGE,
    UNOWN_REPORT_MAIN_EXIT,
};

typedef struct UnownReportScreen {
    void *file;
    NNSG2dScreenData *screenData;
    void *rawData;
    u8 width; // in tiles
    u8 height;
} UnownReportScreen;

typedef struct UnownReportApp {
    enum HeapID heapID;
    UnownReportArgs *args;
    u32 unk_08;
    Pokedex *pokedex;
    u16 subState;
    MenuInputState menuInputState;
    u8 textFrameDelay;
    u8 frame;
    u8 page;
    u8 numPages;
    u8 numFormSpreads;
    u8 numInfoPages;
    u8 unk_1E;
    u8 numSeenForms;
    u8 seenForms[28];
    u8 reportLevel;
    BgConfig *bgConfig;
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *template;
    String *buffer;
    String *title;
    String *pageNumberTemplate;
    String *playerName;
    u8 unk_60[8];
    Window windows[UNOWN_REPORT_WIN_COUNT];
    YesNoPrompt *yesNoPrompt;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    SpriteManager *iconManager;
    Sprite *sprites[UNOWN_REPORT_SPRITE_COUNT];
    ManagedSprite *icons[UNOWN_ICONS_PER_SPREAD];
    NARC *iconNarc;
    UnownReportScreen mainScreens[3]; // cover, Unown forms, research pages
    UnownReportScreen subScreens[3];
} UnownReportApp;

// Research text of one info page: two blocks of message lines, each shown once the report
// level reaches its threshold.
typedef struct UnownReportTextBlock {
    u16 firstMsg : 6;
    u16 numMsgs : 2;
    u16 minLevel : 8;
} UnownReportTextBlock;

typedef struct UnownReportInfoPage {
    UnownReportTextBlock left;
    UnownReportTextBlock right;
} UnownReportInfoPage;

extern const ResdatIdList gUnownReportResdatIds;
extern const TouchscreenHitbox gUnownReportHitboxes[];

static void UnownReport_ResetDisplay();
static int UnownReport_GetSeenPunctuationForms(Pokedex *pokedex, u8 *forms);
static void UnownReport_InitData(UnownReportApp *app);
static void UnownReport_RestoreMenuInputState(UnownReportApp *app);
static BOOL UnownReport_SetupGraphics(UnownReportApp *app);
static BOOL UnownReport_Teardown(UnownReportApp *app);
static int UnownReport_FadeIn(UnownReportApp *app);
static int UnownReport_FadeOut(UnownReportApp *app);
static int UnownReport_HandleInput(UnownReportApp *app);
static int UnownReport_TurnPage(UnownReportApp *app);
static void UnownReport_CreateGraphics(UnownReportApp *app);
static void UnownReport_DestroyGraphics(UnownReportApp *app);
static void UnownReport_DrawFirstPage(UnownReportApp *app);
static int UnownReport_ProcessInput(UnownReportApp *app);
static u8 UnownReport_GetNumInfoPages(UnownReportApp *app);
static void UnownReport_DrawPage(UnownReportApp *app, int unused);
static void UnownReport_DrawTitle(UnownReportApp *app, int bookAnimSeq);
static BOOL UnownReport_IsBookAnimating(UnownReportApp *app);
static int UnownReport_HandleButton(UnownReportApp *app, u32 button);
static int UnownReport_HandleKeys(UnownReportApp *app);
static int UnownReport_HandleTouch(UnownReportApp *app, BOOL *touched);
static void UnownReport_DrawCoverPage(UnownReportApp *app);
static void UnownReport_DrawFormSpread(UnownReportApp *app, u8 spread);
static void UnownReport_DrawInfoPage(UnownReportApp *app, u8 infoPage);
static void UnownReport_ShowScreen(UnownReportApp *app, UnownReportScreen *screen, u32 bgId);
static void UnownReport_PrintText(Window *window, String *string, FontID fontId, int x, u16 y);
static void UnownReport_PrintTextFont0(Window *window, String *string, int x, u16 y);
static void UnownReport_VBlankCallback(void *data);
static void UnownReport_SetVramBanks(UnownReportApp *app);
static void UnownReport_InitBgs(UnownReportApp *app);
static void UnownReport_FreeBgs(UnownReportApp *app);
static void UnownReport_LoadScreen(UnownReportScreen *screen, NARC *narc, int member, enum HeapID heapID);
static void UnownReport_LoadBgGraphics(UnownReportApp *app);
static void UnownReport_FreeScreens(UnownReportApp *app);
static void UnownReport_InitMessages(UnownReportApp *app);
static void UnownReport_FreeMessages(UnownReportApp *app);
static void UnownReport_InitWindows(UnownReportApp *app);
static void UnownReport_FreeWindows(UnownReportApp *app);
static void UnownReport_InitSpriteSystem(UnownReportApp *app);
static void UnownReport_FreeSpriteSystem(UnownReportApp *app);
static void UnownReport_CreateSprites(UnownReportApp *app);
static void UnownReport_DestroySprites(UnownReportApp *app);
static void UnownReport_CreateSpriteObjects(UnownReportApp *app);
static void UnownReport_DeleteSpriteObjects(UnownReportApp *app);
static void UnownReport_LoadIcon(UnownReportApp *app, int icon, int form);
static void UnownReport_UpdateArrows(UnownReportApp *app);

static const GraphicsModes sUnownReportGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_2D,
};

static const OamCharTransferParam sUnownReportOamCharTransferParam = {
    .maxTasks = 16,
    .sizeMain = 0x10000,
    .sizeSub = 0x4000,
    .charModeMain = GX_OBJVRAMMODE_CHAR_1D_32K,
    .charModeSub = GX_OBJVRAMMODE_CHAR_1D_32K,
};

static const SpriteResourceCountsListUnion sUnownReportSpriteResourceCounts = {
    .numChar = 14,
    .numPltt = 1,
    .numCell = 1,
    .numAnim = 1,
    .numMcel = 0,
    .numManm = 0,
};

static const UnownReportInfoPage sUnownReportInfoPages[6] = {
    { .left = { 6, 1, 1 },  .right = { 7, 1, 1 }  },
    { .left = { 8, 1, 2 },  .right = { 9, 1, 3 }  },
    { .left = { 10, 1, 4 }, .right = { 11, 1, 4 } },
    { .left = { 12, 1, 5 }, .right = { 13, 1, 6 } },
    { .left = { 14, 3, 7 }, .right = { 17, 1, 7 } },
    { .left = { 18, 1, 8 }, .right = { 19, 1, 8 } },
};

static const BgTemplate sUnownReportBgTemplate_Main3 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xe800,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 3,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sUnownReportBgTemplate_Sub1 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf800,
    .charBase = GX_BG_CHARBASE_0x10000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 1,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sUnownReportBgTemplate_Sub2 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf000,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sUnownReportBgTemplate_Main1 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf800,
    .charBase = GX_BG_CHARBASE_0x10000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 1,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sUnownReportBgTemplate_Sub3 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xe800,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 3,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sUnownReportBgTemplate_Main2 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf000,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const OamManagerParam sUnownReportOamManagerParam = {
    .fromOBJmain = 0,
    .numOBJmain = 128,
    .fromAffineMain = 0,
    .numAffineMain = 32,
    .fromOBJsub = 0,
    .numOBJsub = 128,
    .fromAffineSub = 0,
    .numAffineSub = 32,
};

static const WindowTemplate sUnownReportWindowTemplates[4] = {
    {
     .bgId = GF_BG_LYR_MAIN_1,
     .left = 25,
     .top = 21,
     .width = 6,
     .height = 2,
     .palette = 12,
     .baseTile = 0x1E1,
     },
    {
     .bgId = GF_BG_LYR_SUB_1,
     .left = 2,
     .top = 3,
     .width = 24,
     .height = 20,
     .palette = 12,
     .baseTile = 1,
     },
    {
     .bgId = GF_BG_LYR_MAIN_1,
     .left = 2,
     .top = 1,
     .width = 24,
     .height = 20,
     .palette = 12,
     .baseTile = 1,
     },
    {
     .bgId = GF_BG_LYR_MAIN_1,
     .left = 28,
     .top = 9,
     .width = 3,
     .height = 2,
     .palette = 12,
     .baseTile = 0x1ED,
     },
};

static const GraphicsBanks sUnownReportGraphicsBanks = {
    .bg = GX_VRAM_BG_128_A,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_128_C,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_64_E,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_NONE,
    .texpltt = GX_VRAM_TEXPLTT_NONE,
};

static const UnmanagedSpriteTemplate sUnownReportSpriteTemplates[3] = {
    {
     .resourceSet = 0,
     .x = 224,
     .y = 176,
     .z = 0,
     .animation = 0,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .paletteMode = 1,
     .unk_1C = 0,
     .unk_20 = 0,
     .unk_24 = 0,
     },
    {
     .resourceSet = 0,
     .x = 236,
     .y = 28,
     .z = 0,
     .animation = 2,
     .drawPriority = 1,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .paletteMode = 1,
     .unk_1C = 0,
     .unk_20 = 0,
     .unk_24 = 0,
     },
    {
     .resourceSet = 0,
     .x = 236,
     .y = 132,
     .z = 0,
     .animation = 5,
     .drawPriority = 2,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .paletteMode = 1,
     .unk_1C = 0,
     .unk_20 = 0,
     .unk_24 = 0,
     },
};

BOOL UnownReport_Init(OverlayManager *man, int *state) {
    UnownReportApp *app;

    switch (*state) {
    case 0:
        UnownReport_ResetDisplay(man);
        Heap_Create(HEAP_ID_3, HEAP_ID_152, 0x20000);
        app = OverlayManager_CreateAndGetData(man, sizeof(UnownReportApp), HEAP_ID_152);
        MI_CpuFill8(app, 0, sizeof(UnownReportApp));
        app->heapID = HEAP_ID_152;
        app->args = OverlayManager_GetArgs(man);
        UnownReport_InitData(app);
        (*state)++;
        break;
    case 1:
        if (UnownReport_SetupGraphics(OverlayManager_GetData(man))) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL UnownReport_Main(OverlayManager *man, int *state) {
    UnownReportApp *app = OverlayManager_GetData(man);

    switch (*state) {
    case UNOWN_REPORT_MAIN_FADE_IN:
        *state = UnownReport_FadeIn(app);
        break;
    case UNOWN_REPORT_MAIN_FADE_OUT:
        *state = UnownReport_FadeOut(app);
        break;
    case UNOWN_REPORT_MAIN_INPUT:
        *state = UnownReport_HandleInput(app);
        break;
    case UNOWN_REPORT_MAIN_TURN_PAGE:
        *state = UnownReport_TurnPage(app);
        break;
    case UNOWN_REPORT_MAIN_EXIT:
        return TRUE;
    }
    SpriteSystem_DrawSprites(app->spriteManager);
    SpriteSystem_DrawSprites(app->iconManager);
    return FALSE;
}

BOOL UnownReport_Exit(OverlayManager *man, int *state) {
    UnownReportApp *app = OverlayManager_GetData(man);

    if (!UnownReport_Teardown(app)) {
        return FALSE;
    }
    UnownReport_ResetDisplay();
    UnownReport_RestoreMenuInputState(app);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_152);
    return TRUE;
}

static void UnownReport_ResetDisplay(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
    SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
}

// The "!" and "?" forms (26 and 27)
static int UnownReport_GetSeenPunctuationForms(Pokedex *pokedex, u8 *forms) {
    int count = 0;
    int i;

    for (i = 0; i < 28; i++) {
        u8 form = Pokedex_GetSeenFormByIdx_Unown(pokedex, i, TRUE);
        if ((u8)(form + 230) <= 1) { // form - 26 <= 1
            forms[count++] = form;
        }
    }
    return count;
}

static void UnownReport_InitData(UnownReportApp *app) {
    UnownReportArgs *args = app->args;
    Options *options;
    int i;

    app->menuInputState = MenuInputStateMgr_GetState(args->menuInputStatePtr);
    options = Save_PlayerData_GetOptionsAddr(app->args->saveData);
    app->textFrameDelay = Options_GetTextFrameDelay(options);
    app->frame = Options_GetFrame(options);
    app->pokedex = Save_Pokedex_Get(app->args->saveData);
    app->reportLevel = Save_VarsFlags_GetUnownReportLevel(Save_VarsFlags_Get(app->args->saveData));
    app->numInfoPages = UnownReport_GetNumInfoPages(app);
    app->numSeenForms = Pokedex_GetSeenFormNum_Unown(app->pokedex, TRUE);
    if (app->reportLevel >= 4) {
        u8 punctuation[4];
        u8 count;

        for (i = 0; i <= 25; i++) {
            app->seenForms[i] = i;
        }
        count = (u8)UnownReport_GetSeenPunctuationForms(app->pokedex, punctuation);
        for (i = 0; i < count; i++) {
            app->seenForms[26 + i] = punctuation[i];
        }
    } else {
        for (i = 0; i < app->numSeenForms; i++) {
            app->seenForms[i] = Pokedex_GetSeenFormByIdx_Unown(app->pokedex, i, TRUE);
        }
    }
    app->numFormSpreads = (int)app->numSeenForms / UNOWN_ICONS_PER_SPREAD;
    if (app->numSeenForms && (int)app->numSeenForms % UNOWN_ICONS_PER_SPREAD) {
        app->numFormSpreads++;
    }
    app->numPages = app->numFormSpreads + app->numInfoPages + 1;
}

static void UnownReport_RestoreMenuInputState(UnownReportApp *app) {
    UnownReportArgs *args = app->args;
    MenuInputStateMgr_SetState(args->menuInputStatePtr, app->menuInputState);
}

static BOOL UnownReport_SetupGraphics(UnownReportApp *app) {
    switch (app->subState) {
    case UNOWN_REPORT_SETUP_CREATE:
        UnownReport_CreateGraphics(app);
        app->subState++;
        break;
    case UNOWN_REPORT_SETUP_DRAW_FIRST_PAGE:
        UnownReport_DrawFirstPage(app);
        GF_TouchpadResetSampling();
        GF_TouchpadStartSampling(1);
        Sound_SetSceneAndPlayBGM(69, 0, 0);
        app->subState = UNOWN_REPORT_SETUP_CREATE;
        return TRUE;
    }
    return FALSE;
}

static BOOL UnownReport_Teardown(UnownReportApp *app) {
    GF_TouchpadStopSampling();
    UnownReport_DestroyGraphics(app);
    return TRUE;
}

static int UnownReport_FadeIn(UnownReportApp *app) {
    switch (app->subState) {
    case UNOWN_REPORT_FADE_START:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, app->heapID);
        app->subState++;
        break;
    case UNOWN_REPORT_FADE_WAIT:
        if (IsPaletteFadeFinished()) {
            app->subState = UNOWN_REPORT_FADE_START;
            return UNOWN_REPORT_MAIN_INPUT;
        }
        break;
    }
    return UNOWN_REPORT_MAIN_FADE_IN;
}

static int UnownReport_FadeOut(UnownReportApp *app) {
    switch (app->subState) {
    case UNOWN_REPORT_FADE_START:
        if (UnownReport_IsBookAnimating(app) == FALSE) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, app->heapID);
            app->subState++;
        }
        break;
    case UNOWN_REPORT_FADE_WAIT:
        if (IsPaletteFadeFinished()) {
            app->subState = UNOWN_REPORT_FADE_START;
            return UNOWN_REPORT_MAIN_EXIT;
        }
        break;
    }
    return UNOWN_REPORT_MAIN_FADE_OUT;
}

static int UnownReport_HandleInput(UnownReportApp *app) {
    return UnownReport_ProcessInput(app);
}

static int UnownReport_TurnPage(UnownReportApp *app) {
    switch (app->subState) {
    case UNOWN_REPORT_TURN_PAGE_FADE_OUT:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_UPWARD_OUT, FADE_TYPE_DOWNWARD_OUT, RGB_BLACK, 6, 1, app->heapID);
        app->subState++;
        break;
    case UNOWN_REPORT_TURN_PAGE_DRAW:
        if (IsPaletteFadeFinished()) {
            UnownReport_DrawPage(app, app->page);
            app->subState++;
        }
        break;
    case UNOWN_REPORT_TURN_PAGE_FADE_IN:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_UNK_5, RGB_BLACK, 6, 1, app->heapID);
        app->subState++;
        break;
    case UNOWN_REPORT_TURN_PAGE_WAIT:
        if (IsPaletteFadeFinished()) {
            app->subState = UNOWN_REPORT_TURN_PAGE_FADE_OUT;
            return UNOWN_REPORT_MAIN_INPUT;
        }
        break;
    }
    return UNOWN_REPORT_MAIN_TURN_PAGE;
}

static void UnownReport_CreateGraphics(UnownReportApp *app) {
    UnownReport_InitBgs(app);
    UnownReport_LoadBgGraphics(app);
    UnownReport_InitMessages(app);
    UnownReport_InitWindows(app);
    UnownReport_CreateSprites(app);
    Main_SetVBlankIntrCB(UnownReport_VBlankCallback, app);
}

static void UnownReport_DestroyGraphics(UnownReportApp *app) {
    UnownReport_DestroySprites(app);
    UnownReport_FreeWindows(app);
    UnownReport_FreeMessages(app);
    UnownReport_FreeScreens(app);
    UnownReport_FreeBgs(app);
}

static void UnownReport_DrawFirstPage(UnownReportApp *app) {
    UnownReport_DrawTitle(app, 0);
    UnownReport_DrawPage(app, app->page);
}

static int UnownReport_ProcessInput(UnownReportApp *app) {
    BOOL touched = FALSE;
    int nextState = UnownReport_HandleTouch(app, &touched);

    if (!touched) {
        nextState = UnownReport_HandleKeys(app);
    }
    return nextState;
}

static u8 UnownReport_GetNumInfoPages(UnownReportApp *app) {
    u8 level = app->reportLevel;
    int i;

    for (i = 0; i < 6; i++) {
        if ((s32)(((u32)((const u16(*)[2])sUnownReportInfoPages)[i][0] << 16) >> 24) > (s32)level) { // left.minLevel
            break;
        }
    }
    return i;
}

static void UnownReport_DrawPage(UnownReportApp *app, int unused) {
    FillWindowPixelBuffer(&app->windows[UNOWN_REPORT_WIN_LEFT_PAGE], 0);
    FillWindowPixelBuffer(&app->windows[UNOWN_REPORT_WIN_RIGHT_PAGE], 0);
    FillWindowPixelBuffer(&app->windows[UNOWN_REPORT_WIN_FOOTER], 0);
    if (app->page == 0) {
        UnownReport_DrawCoverPage(app);
    } else if (app->page - 1 < app->numFormSpreads) {
        UnownReport_DrawFormSpread(app, app->page - 1);
    } else {
        UnownReport_DrawInfoPage(app, app->page - (app->numFormSpreads + 1));
    }
    BufferIntegerAsString(app->msgFormat, 0, app->page + 1, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
    BufferIntegerAsString(app->msgFormat, 1, app->numPages, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(app->msgFormat, app->buffer, app->pageNumberTemplate);
    AddTextPrinterParameterizedWithColor(&app->windows[UNOWN_REPORT_WIN_FOOTER], 0, app->buffer, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 2, 0), NULL);
    ScheduleWindowCopyToVram(&app->windows[UNOWN_REPORT_WIN_LEFT_PAGE]);
    ScheduleWindowCopyToVram(&app->windows[UNOWN_REPORT_WIN_RIGHT_PAGE]);
    ScheduleWindowCopyToVram(&app->windows[UNOWN_REPORT_WIN_FOOTER]);
    UnownReport_UpdateArrows(app);
}

static void UnownReport_DrawTitle(UnownReportApp *app, int bookAnimSeq) {
    u32 x;
    u32 width;

    Sprite_SetAnimCtrlSeq(app->sprites[UNOWN_REPORT_SPRITE_BOOK], bookAnimSeq);
    Sprite_ResetAnimCtrlState(app->sprites[UNOWN_REPORT_SPRITE_BOOK]);
    FillWindowPixelBuffer(&app->windows[UNOWN_REPORT_WIN_TITLE], 0);
    width = FontID_String_GetWidth(4, app->title, 0);
    x = (48 - width) / 2;
    AddTextPrinterParameterizedWithColor(&app->windows[UNOWN_REPORT_WIN_TITLE], 4, app->title, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(7, 15, 0), NULL);
    ScheduleWindowCopyToVram(&app->windows[UNOWN_REPORT_WIN_TITLE]);
}

static BOOL UnownReport_IsBookAnimating(UnownReportApp *app) {
    return Sprite_IsAnimated(app->sprites[UNOWN_REPORT_SPRITE_BOOK]);
}

// button: 0 close, 1 previous page, 2 next page (also the touch hitbox index)
static int UnownReport_HandleButton(UnownReportApp *app, u32 button) {
    switch (button) {
    case 0:
        PlaySE(SEQ_SE_DP_DECIDE);
        UnownReport_DrawTitle(app, 1);
        return UNOWN_REPORT_MAIN_FADE_OUT;
    case 1:
        if (app->page == 0) {
            return UNOWN_REPORT_MAIN_INPUT;
        }
        app->page--;
        Sprite_SetAnimCtrlSeq(app->sprites[UNOWN_REPORT_SPRITE_PREV_ARROW], 3);
        PlaySE(SEQ_SE_DP_SELECT);
        return UNOWN_REPORT_MAIN_TURN_PAGE;
    case 2:
        if (app->page >= app->numPages - 1) {
            return UNOWN_REPORT_MAIN_INPUT;
        }
        app->page++;
        Sprite_SetAnimCtrlSeq(app->sprites[UNOWN_REPORT_SPRITE_NEXT_ARROW], 6);
        PlaySE(SEQ_SE_DP_SELECT);
        return UNOWN_REPORT_MAIN_TURN_PAGE;
    default:
        return UNOWN_REPORT_MAIN_INPUT;
    }
}

static int UnownReport_HandleKeys(UnownReportApp *app) {
    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_KEY_RIGHT | PAD_KEY_LEFT | PAD_KEY_UP | PAD_KEY_DOWN | PAD_BUTTON_X | PAD_BUTTON_Y)) {
        app->menuInputState = MENU_INPUT_STATE_BUTTONS;
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        return UnownReport_HandleButton(app, 0);
    }
    if (gSystem.newKeys & PAD_KEY_UP) {
        return UnownReport_HandleButton(app, 1);
    }
    if (gSystem.newKeys & PAD_KEY_DOWN) {
        return UnownReport_HandleButton(app, 2);
    }
    return UNOWN_REPORT_MAIN_INPUT;
}

static int UnownReport_HandleTouch(UnownReportApp *app, BOOL *touched) {
    int hitbox = TouchscreenHitbox_FindHitboxAtTouchNew(gUnownReportHitboxes);

    if (hitbox == -1) {
        return UNOWN_REPORT_MAIN_INPUT;
    }
    *touched = TRUE;
    app->menuInputState = MENU_INPUT_STATE_TOUCH;
    return UnownReport_HandleButton(app, hitbox);
}

static void UnownReport_DrawCoverPage(UnownReportApp *app) {
    s32 i;
    u32 width;

    UnownReport_ShowScreen(app, &app->subScreens[0], GF_BG_LYR_SUB_2);
    UnownReport_ShowScreen(app, &app->mainScreens[0], GF_BG_LYR_MAIN_2);
    for (i = 0; i < UNOWN_ICONS_PER_SPREAD; i++) {
        ManagedSprite_SetDrawFlag(app->icons[i], FALSE);
    }
    ReadMsgDataIntoString(app->msgData, msg_0001_00001, app->buffer);
    UnownReport_PrintTextFont0(&app->windows[UNOWN_REPORT_WIN_LEFT_PAGE], app->buffer, 0, 56);
    width = FontID_String_GetWidth(0, app->playerName, 0);
    UnownReport_PrintTextFont0(&app->windows[UNOWN_REPORT_WIN_LEFT_PAGE], app->playerName, (u16)(176 - width), 72);
    ReadMsgDataIntoString(app->msgData, msg_0001_00003, app->buffer);
    UnownReport_PrintTextFont0(&app->windows[UNOWN_REPORT_WIN_RIGHT_PAGE], app->buffer, 4, 60);
    ReadMsgDataIntoString(app->msgData, msg_0001_00004, app->template);
    BufferIntegerAsString(app->msgFormat, 0, app->numSeenForms, 2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    StringExpandPlaceholders(app->msgFormat, app->buffer, app->template);
    UnownReport_PrintTextFont0(&app->windows[UNOWN_REPORT_WIN_RIGHT_PAGE], app->buffer, 4, 76);
}

static void UnownReport_DrawFormSpread(UnownReportApp *app, u8 spread) {
    Window *rightWindow;
    Window *leftWindow;
    int first;
    UnownReportApp *slot;
    int i;
    int y;

    UnownReport_ShowScreen(app, &app->subScreens[1], GF_BG_LYR_SUB_2);
    UnownReport_ShowScreen(app, &app->mainScreens[1], GF_BG_LYR_MAIN_2);
    first = UNOWN_ICONS_PER_SPREAD * spread;
    rightWindow = &app->windows[UNOWN_REPORT_WIN_RIGHT_PAGE];
    i = 0;
    leftWindow = (Window *)app;
    slot = app;
    y = 0;
    leftWindow = (Window *)((u8 *)leftWindow + offsetof(UnownReportApp, windows[UNOWN_REPORT_WIN_LEFT_PAGE]));
    for (; i < UNOWN_ICONS_PER_SPREAD; i++, slot = (UnownReportApp *)((u8 *)slot + sizeof(ManagedSprite *)), y += 24) {
        int index = first + i;

        if (index >= app->numSeenForms) {
            ManagedSprite_SetDrawFlag(slot->icons[0], FALSE);
        } else {
            ManagedSprite_SetDrawFlag(slot->icons[0], TRUE);
            UnownReport_LoadIcon(app, i, app->seenForms[index]);
            ReadMsgDataIntoString(app->msgData, app->seenForms[index] + 20, app->buffer);
            if (i < 7) {
                UnownReport_PrintTextFont0(leftWindow, app->buffer, 48, (u16)y);
            } else {
                UnownReport_PrintTextFont0(rightWindow, app->buffer, 48, (u16)(y - 168));
            }
        }
    }
}

static void UnownReport_DrawInfoPage(UnownReportApp *app, u8 infoPage) {
    const UnownReportInfoPage *page;
    int i;

    if (infoPage == 0) {
        UnownReport_ShowScreen(app, &app->subScreens[2], GF_BG_LYR_SUB_2);
        UnownReport_ShowScreen(app, &app->mainScreens[2], GF_BG_LYR_MAIN_2);
        for (i = 0; i < UNOWN_ICONS_PER_SPREAD; i++) {
            ManagedSprite_SetDrawFlag(app->icons[i], FALSE);
        }
    }
    page = &sUnownReportInfoPages[infoPage];
    if (page->left.minLevel <= app->reportLevel) {
        for (i = 0; i < page->left.numMsgs; i++) {
            ReadMsgDataIntoString(app->msgData, page->left.firstMsg + i, app->buffer);
            if (page->left.firstMsg + i == 15) {
                UnownReport_PrintText(&app->windows[UNOWN_REPORT_WIN_LEFT_PAGE], app->buffer, 3, 4, 0);
            } else {
                UnownReport_PrintTextFont0(&app->windows[UNOWN_REPORT_WIN_LEFT_PAGE], app->buffer, 4, 24);
            }
        }
    }
    if (page->right.minLevel <= app->reportLevel) {
        for (i = 0; i < page->right.numMsgs; i++) {
            ReadMsgDataIntoString(app->msgData, page->right.firstMsg + i, app->buffer);
            UnownReport_PrintTextFont0(&app->windows[UNOWN_REPORT_WIN_RIGHT_PAGE], app->buffer, 4, 24);
        }
    }
}

static void UnownReport_ShowScreen(UnownReportApp *app, UnownReportScreen *screen, u32 bgId) {
    CopyToBgTilemapRect(app->bgConfig, (u8)bgId, 0, 0, 32, 24, screen->rawData, 0, 0, screen->width, screen->height);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, (u8)bgId);
}

static void UnownReport_PrintText(Window *window, String *string, FontID fontId, int x, u16 y) {
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
}

static void UnownReport_PrintTextFont0(Window *window, String *string, int x, u16 y) {
    UnownReport_PrintText(window, string, 0, x, y);
}

static void UnownReport_VBlankCallback(void *data) {
    vu32 *dtcm = (vu32 *)0x027E0000;
    UnownReportApp *app = data;

    if (app->spriteSystem) {
        SpriteSystem_TransferOam();
    }
    GF_RunVramTransferTasks();
    DoScheduledBgGpuUpdates(app->bgConfig);
    dtcm[0x3FF8 / 4] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag(OS_IE_V_BLANK)
}

static void UnownReport_SetVramBanks(UnownReportApp *app) {
    GraphicsBanks banks = sUnownReportGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

static void UnownReport_InitBgs(UnownReportApp *app) {
    GraphicsModes modes;
    BgTemplate subBg1;
    BgTemplate subBg2;
    BgTemplate subBg3;
    BgTemplate mainBg1;
    BgTemplate mainBg2;
    BgTemplate mainBg3;

    UnownReport_SetVramBanks(app);
    app->bgConfig = BgConfig_Alloc(app->heapID);
    modes = sUnownReportGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    reg_GX_POWCNT &= 0xFFFF7FFF;
    subBg1 = sUnownReportBgTemplate_Sub1;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_SUB_1, &subBg1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_1);
    subBg2 = sUnownReportBgTemplate_Sub2;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_SUB_2, &subBg2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_2);
    subBg3 = sUnownReportBgTemplate_Sub3;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_SUB_3, &subBg3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_3);
    mainBg1 = sUnownReportBgTemplate_Main1;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_1, &mainBg1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1);
    mainBg2 = sUnownReportBgTemplate_Main2;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_2, &mainBg2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_2);
    mainBg3 = sUnownReportBgTemplate_Main3;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_3, &mainBg3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_3);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 32, 0, app->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 32, 0, app->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_3, 32, 0, app->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 32, 0, app->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 32, 0, app->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 32, 0, app->heapID);
}

static void UnownReport_FreeBgs(UnownReportApp *app) {
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_3);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_1);
    Heap_Free(app->bgConfig);
    {
        u32 powcnt = reg_GX_POWCNT;
        powcnt |= 0x8000;
        reg_GX_POWCNT = powcnt;
    }
}

static void UnownReport_LoadScreen(UnownReportScreen *screen, NARC *narc, int member, enum HeapID heapID) {
    screen->file = GfGfxLoader_GetScrnDataFromOpenNarc(narc, member, TRUE, &screen->screenData, heapID);
    screen->width = (u32)screen->screenData->screenWidth >> 3;
    screen->height = (u32)screen->screenData->screenHeight >> 3;
    screen->rawData = screen->screenData->rawData;
}

static void UnownReport_LoadBgGraphics(UnownReportApp *app) {
    NARC *narc = NARC_New(NARC_a_2_3_9, app->heapID);

    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 5, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, app->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 7, app->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, app->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 9, app->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, app->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, app->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 6, app->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, app->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 8, app->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, app->heapID);
    UnownReport_LoadScreen(&app->subScreens[0], narc, 10, app->heapID);
    UnownReport_LoadScreen(&app->subScreens[1], narc, 12, app->heapID);
    UnownReport_LoadScreen(&app->subScreens[2], narc, 14, app->heapID);
    UnownReport_LoadScreen(&app->mainScreens[0], narc, 11, app->heapID);
    UnownReport_LoadScreen(&app->mainScreens[1], narc, 13, app->heapID);
    UnownReport_LoadScreen(&app->mainScreens[2], narc, 15, app->heapID);
    NARC_Delete(narc);
    LoadUserFrameGfx2(app->bgConfig, GF_BG_LYR_MAIN_1, 1, 13, app->frame, app->heapID);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, app->heapID);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_12_OFFSET, app->heapID);
}

static void UnownReport_FreeScreens(UnownReportApp *app) {
    Heap_Free(app->mainScreens[2].file);
    Heap_Free(app->mainScreens[1].file);
    Heap_Free(app->mainScreens[0].file);
    Heap_Free(app->subScreens[2].file);
    Heap_Free(app->subScreens[1].file);
    Heap_Free(app->subScreens[0].file);
}

static void UnownReport_InitMessages(UnownReportApp *app) {
    FontID_Alloc(4, app->heapID);
    app->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0001_bin, app->heapID);
    app->msgFormat = MessageFormat_New_Custom(16, 16, app->heapID);
    app->template = String_New(512, app->heapID);
    app->buffer = String_New(512, app->heapID);
    app->title = NewString_ReadMsgData(app->msgData, msg_0001_00000);
    app->pageNumberTemplate = NewString_ReadMsgData(app->msgData, msg_0001_00005);
    app->playerName = PlayerProfile_GetPlayerName_NewString(Save_PlayerData_GetProfile(app->args->saveData), app->heapID);
}

static void UnownReport_FreeMessages(UnownReportApp *app) {
    String_Delete(app->playerName);
    String_Delete(app->pageNumberTemplate);
    String_Delete(app->title);
    String_Delete(app->buffer);
    String_Delete(app->template);
    MessageFormat_Delete(app->msgFormat);
    DestroyMsgData(app->msgData);
    FontID_Release(4);
}

static void UnownReport_InitWindows(UnownReportApp *app) {
    const WindowTemplate *template = sUnownReportWindowTemplates;
    Window *window;
    int i;

    i = 0;
    window = app->windows;
    for (; i < UNOWN_REPORT_WIN_COUNT; i++, template ++, window++) {
        AddWindow(app->bgConfig, window, template);
        FillWindowPixelBuffer(window, 0);
    }
    app->yesNoPrompt = YesNoPrompt_Create(app->heapID);
}

static void UnownReport_FreeWindows(UnownReportApp *app) {
    int i;
    Window *window;

    i = 0;
    window = app->windows;
    for (; i < UNOWN_REPORT_WIN_COUNT; i++, window++) {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
    }
    YesNoPrompt_Destroy(app->yesNoPrompt);
}

static void UnownReport_InitSpriteSystem(UnownReportApp *app) {
    SpriteResourceCountsListUnion capacities;
    int i;

    GF_CreateVramTransferManager(32, app->heapID);
    app->spriteSystem = SpriteSystem_Alloc(app->heapID);
    SpriteSystem_Init(app->spriteSystem, &sUnownReportOamManagerParam, &sUnownReportOamCharTransferParam, 2);
    thunk_ClearMainOAM(app->heapID);
    thunk_ClearSubOAM(app->heapID);
    app->spriteManager = SpriteManager_New(app->spriteSystem);
    SpriteSystem_InitSprites(app->spriteSystem, app->spriteManager, 17);
    SpriteSystem_LoadResourceDataFromFileIdListEx(app->spriteSystem, app->spriteManager, &gUnownReportResdatIds, 0, 0);
    capacities = sUnownReportSpriteResourceCounts;
    app->iconManager = SpriteManager_New(app->spriteSystem);
    SpriteSystem_InitManagerWithCapacities(app->spriteSystem, app->iconManager, &capacities);
    SpriteManager_SetSpriteList(app->iconManager, SpriteManager_GetSpriteList(app->spriteManager));
    app->iconNarc = NARC_New(NARC_poketool_icongra_poke_icon, app->heapID);
    SpriteSystem_LoadPlttResObjFromOpenNarc(app->spriteSystem, app->iconManager, app->iconNarc, GetMonIconPlttNaix(), FALSE, 1, 3, 1);
    SpriteSystem_LoadCellResObjFromOpenNarc(app->spriteSystem, app->iconManager, app->iconNarc, GetMonIconStaticCellNaix(), FALSE, 1);
    SpriteSystem_LoadAnimResObj(app->spriteSystem, app->iconManager, NARC_poketool_icongra_poke_icon, GetMonIconStaticAnimNaix(), FALSE, 1);
    for (i = 0; i < 7; i++) {
        SpriteSystem_LoadCharResObjFromOpenNarc(app->spriteSystem, app->iconManager, app->iconNarc, GetMonIconNaixEx(SPECIES_UNOWN, FALSE, i), FALSE, NNS_G2D_VRAM_TYPE_2DSUB, i + 1);
    }
    for (i = 7; i < UNOWN_ICONS_PER_SPREAD; i++) {
        SpriteSystem_LoadCharResObjFromOpenNarc(app->spriteSystem, app->iconManager, app->iconNarc, GetMonIconNaixEx(SPECIES_UNOWN, FALSE, i), FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, i + 1);
    }
}

static void UnownReport_FreeSpriteSystem(UnownReportApp *app) {
    SpriteSystem_FreeResourcesAndManager(app->spriteSystem, app->iconManager);
    SpriteSystem_DestroySpriteManager(app->spriteSystem, app->spriteManager);
    app->iconManager = NULL;
    app->spriteManager = NULL;
    NARC_Delete(app->iconNarc);
    SpriteSystem_Free(app->spriteSystem);
    app->spriteSystem = NULL;
    GF_DestroyVramTransferManager();
    thunk_ClearMainOAM(app->heapID);
}

static void UnownReport_CreateSprites(UnownReportApp *app) {
    UnownReport_InitSpriteSystem(app);
    UnownReport_CreateSpriteObjects(app);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

static void UnownReport_DestroySprites(UnownReportApp *app) {
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
    UnownReport_DeleteSpriteObjects(app);
    UnownReport_FreeSpriteSystem(app);
}

static void UnownReport_CreateSpriteObjects(UnownReportApp *app) {
    ManagedSpriteTemplate template;
    int i;
    int y;
    int upperY;

    for (i = 0; i < 3; i++) {
        app->sprites[i] = SpriteSystem_CreateSpriteFromResourceHeader(app->spriteSystem, app->spriteManager, &sUnownReportSpriteTemplates[i]);
        Sprite_SetDrawFlag(app->sprites[i], TRUE);
        Sprite_SetAnimActiveFlag(app->sprites[i], TRUE);
        Sprite_SetPriority(app->sprites[i], 2);
    }
    MI_CpuFill8(&template, 0, sizeof(template));
    template.bgPriority = 1;
    {
        int pal = GetMonIconPaletteEx(SPECIES_UNOWN, 0, FALSE);
        i = 0;
        template.pal = pal;
    }
    template.vramTransfer = i;
    template.x = 40;
    template.resIdList[1] = 1;
    template.resIdList[2] = 1;
    template.resIdList[3] = 1;
    template.resIdList[4] = -1;
    template.resIdList[5] = -1;
    upperY = 29;
    y = i;
    for (; i < UNOWN_ICONS_PER_SPREAD; i++) {
        if (i < 7) {
            template.y = upperY;
            template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
        } else {
            template.y = y - 155;
            template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
        }
        template.resIdList[0] = i + 1;
        app->icons[i] = SpriteSystem_NewSprite(app->spriteSystem, app->iconManager, &template);
        ManagedSprite_SetDrawFlag(app->icons[i], FALSE);
        upperY += 24;
        y += 24;
    }
}

static void UnownReport_DeleteSpriteObjects(UnownReportApp *app) {
    int i;

    for (i = 0; i < UNOWN_ICONS_PER_SPREAD; i++) {
        if (app->icons[i] != NULL) {
            Sprite_DeleteAndFreeResources(app->icons[i]);
        }
    }
    for (i = 0; i < 3; i++) {
        if (app->sprites[i] != NULL) {
            Sprite_Delete(app->sprites[i]);
        }
    }
}

// The icon graphics of slot `icon` are replaced with those of `form`
static void UnownReport_LoadIcon(UnownReportApp *app, int icon, int form) {
    u32 fileId = GetMonIconNaixEx(SPECIES_UNOWN, FALSE, form);
    SpriteSystem_ReplaceCharResObjFromOpenNarc(app->spriteSystem, app->iconManager, app->iconNarc, fileId, FALSE, icon + 1);
}

static void UnownReport_UpdateArrows(UnownReportApp *app) {
    if (app->numPages == 1 || app->page == 0) {
        Sprite_SetAnimCtrlSeq(app->sprites[UNOWN_REPORT_SPRITE_PREV_ARROW], 4);
    } else {
        Sprite_SetAnimCtrlSeq(app->sprites[UNOWN_REPORT_SPRITE_PREV_ARROW], 2);
    }
    if (app->numPages == 1 || app->page == app->numPages - 1) {
        Sprite_SetAnimCtrlSeq(app->sprites[UNOWN_REPORT_SPRITE_NEXT_ARROW], 7);
    } else {
        Sprite_SetAnimCtrlSeq(app->sprites[UNOWN_REPORT_SPRITE_NEXT_ARROW], 5);
    }
}
