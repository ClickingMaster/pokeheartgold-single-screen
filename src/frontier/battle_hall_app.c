#include "frontier/battle_hall_app.h"

#include "global.h"

#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "frontier/battle_hall_data.h"
#include "frontier/battle_hall_setup.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0441.h"

#include "battle_frontier_communication.h"
#include "bg_window.h"
#include "communication_core.h"
#include "filesystem.h"
#include "font.h"
#include "frontier_save_access.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "link_synchronization.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "party.h"
#include "player_data.h"
#include "pm_string.h"
#include "poke_overlay.h"
#include "pokedex_util.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "pokemon_summary.h"
#include "render_window.h"
#include "save_special_ribbons.h"
#include "screen_fade_legacy.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sprite_resource.h"
#include "sprite_resource_header.h"
#include "sprite_transfer.h"
#include "system.h"
#include "vram_transfer_manager.h"
#include "wireless_status_icon.h"
#include "yes_no_prompt.h"

FS_EXTERN_OVERLAY(OVY_80);

#define TYPE_GRID_COLUMNS 4
#define TYPE_GRID_ROWS    5
#define TYPE_CELL_EMPTY   0xFE
#define TYPE_CELL_MATRON  19 // the "???" cell, used for the Hall Matron battle
#define NO_LINK_CHOICE    0xFF
#define NO_SAVED_CURSOR   117
#define MAX_TYPE_RANK     10

enum HallCellState {
    HALL_CELL_NORMAL,
    HALL_CELL_SELECTED,
    HALL_CELL_PARTNER_CHOICE,
    HALL_CELL_UNAVAILABLE,
};

// Main loop states
#define HALL_STATE_FADE_IN      0
#define HALL_STATE_SUMMARY      1
#define HALL_STATE_SELECT       2
#define HALL_STATE_LINK_DECIDE  3
#define HALL_STATE_LINK_SYNC    4
#define HALL_STATE_FADE_OUT     5
#define HALL_STATE_CLOSE_PROMPT 6

typedef struct BattleHallApp {
    OverlayManager *manager;
    OverlayManager *summaryApp;
    u8 subState;
    u8 mode;
    u8 printerId;
    u8 openSummary;
    u8 prevCursor;
    u8 cursor;
    u8 unk_00E;
    u8 yesNoActive;
    u16 summaryPending;
    u16 linkReady;
    s16 brightness;
    u8 recvCount;
    u8 linkDecided;
    u8 linkChoice;
    u8 reselect;
    u8 linkCursor;
    u8 timer;
    u16 battleNo;
    u8 matronMode;
    u8 savedCursor;
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *expandedString;
    String *rawString;
    String *strings[2];
    u8 unk_038[0x10];
    BgConfig *bgConfig;
    Window windows[4];
    YesNoPrompt *yesNoPrompt;
    u8 usedTouch;
    u8 unk_091[3];
    PaletteData *paletteData;
    MessagePrinter *messagePrinter;
    Options *options;
    SaveData *saveData;
    PokemonSummaryArgs *summaryArgs;
    BattleHallSpriteGfx spriteGfx;
    BattleHallSprite *cursorSprite;
    BattleHallSprite *monIcon;
    u32 summaryResult;
    u16 *resultOut;
    Party *party;
    void *hallRecords;
    u16 *partnerLevel;
    NARC *narc;
    u16 linkCommand;
    u16 linkArg;
    u16 linkChoiceSent;
    u16 linkLevel;
    u8 unk_22C[0x50];
    u8 partnerCursor;
    u8 partnerAnswer;
    u8 unk_27E[6];
} BattleHallApp;

void Frontier_BufferPartnerName(MessageFormat *msgFmt, u32 field);

#define MAKE_TEXT_COLOR(fg, sh, bg) ((((fg) & 0xFF) << 16) | (((sh) & 0xFF) << 8) | (((bg) & 0xFF) << 0))
u8 AddTextPrinterParameterizedWithColor(Window *window, int fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, void *callback);
u8 TextPrinterCheckActive(u8 printerId);
void TextFlags_SetCanTouchSpeedUpPrint(BOOL enable);

static BOOL BattleHallApp_FadeIn(BattleHallApp *app);
static void BattleHallApp_PrintTypeGrid(BattleHallApp *app);
static void BattleHallApp_PrintTitle(BattleHallApp *app);
static void BattleHallApp_DrawFrame(BgConfig *bgConfig, u8 bgId, u8 x, u8 y, u8 width, u8 height, u16 tile, u16 palette);
static BOOL BattleHallApp_RunSummary(BattleHallApp *app);
static BOOL BattleHallApp_SelectType(BattleHallApp *app);
static BOOL BattleHallApp_LinkDecide(BattleHallApp *app);
static BOOL BattleHallApp_LinkSync(BattleHallApp *app);
static BOOL BattleHallApp_FadeOut(BattleHallApp *app);
static BOOL BattleHallApp_ClosePrompt(BattleHallApp *app);
static void BattleHallApp_FreeGraphics(BattleHallApp *app);
static void BattleHallApp_ResumeAfterSummary(BattleHallApp *app);
static void BattleHallApp_DisableDisplay(void);
static void BattleHallApp_InitGraphics(BattleHallApp *app);
static void BattleHallApp_InitBgGraphics(BattleHallApp *app);
static void BattleHallApp_InitSprites(BattleHallApp *app);
static void BattleHallApp_FreeBgLayers(BgConfig *bgConfig);
static void BattleHallApp_VBlankCallback(BattleHallApp *app);
static void BattleHallApp_SetGraphicsBanks(void);
static void BattleHallApp_InitBgLayers(BgConfig *bgConfig);
static void BattleHallApp_InitLinkWindow(BattleHallApp *app);
static void BattleHallApp_LoadTypeGridGraphics(BattleHallApp *app, u8 bgId);
static void BattleHallApp_LoadBgPalette(void);
static void BattleHallApp_LoadSubBgGraphics(BattleHallApp *app, u32 bgId);
static void BattleHallApp_LoadTitleFrameGraphics(BattleHallApp *app, u32 bgId);
static u8 BattleHallApp_PrintMessage(BattleHallApp *app, Window *window, u32 msgId, u32 x, u32 y, u32 textSpeed, u8 fg, u8 shadow, u8 bg, u8 fontId);
static u8 BattleHallApp_ShowMessage(BattleHallApp *app, u32 msgId, u8 fontId);
static void BattleHallApp_BufferNumber(BattleHallApp *app, u32 field, u32 value);
static void BattleHallApp_PrintMonName(BattleHallApp *app, Window *window, int x, int y, u8 fg, u8 shadow, u8 bg, u8 fontId);
static void BattleHallApp_PrintTypeNames(BattleHallApp *app, Window *window, int fg, int shadow, u8 bg, u8 fontId);
static void BattleHallApp_PrintNumber(BattleHallApp *app, Window *window, u32 value, u32 x, u32 y);
static void BattleHallApp_PrintTypeRanks(BattleHallApp *app, Window *window);
static void BattleHallApp_PrintSelectPrompt(BattleHallApp *app, Window *window, u32 x, u32 y, u8 fg, u8 shadow, u8 bg, u8 fontId);
static void BattleHallApp_CreateSummaryArgs(BattleHallApp *app);
static void BattleHallApp_SetMainState(BattleHallApp *app, int *state, int nextState);
static void BattleHallApp_HandleDpad(BattleHallApp *app, int unused);
static BOOL BattleHallApp_HandleTouch(BattleHallApp *app);
static BOOL BattleHallApp_HandleConfirm(BattleHallApp *app);
static u16 BattleHallApp_GetCursorX(BattleHallApp *app);
static u16 BattleHallApp_GetCursorY(BattleHallApp *app);
static void BattleHallApp_GrayOutTypes(BattleHallApp *app, BgConfig *bgConfig);
static void BattleHallApp_SetCellPalette(BgConfig *bgConfig, int index, int state);
static u8 BattleHallApp_GetRecordIndex(u8 index);
static BOOL BattleHallApp_IsMatronBattle(BattleHallApp *app);
static u8 BattleHallApp_IsMatronMode(BattleHallApp *app);
static BOOL BattleHallApp_SendLinkCommand(BattleHallApp *app, u16 command, u16 arg);
static void BattleHallApp_PrepareLinkPing(BattleHallApp *app, u16 command);
static void BattleHallApp_PrepareLinkChoice(BattleHallApp *app, u16 command, u16 choice);
static void BattleHallApp_PrepareLinkAnswer(BattleHallApp *app, u16 command, u16 answer);
static void BattleHallApp_ResetYesNo(BattleHallApp *app);
static void BattleHallApp_ShowConfirmPrompt(BattleHallApp *app);
static void BattleHallApp_CancelConfirm(BattleHallApp *app);
static void BattleHallApp_ClearMessage(BattleHallApp *app);
static void BattleHallApp_SetBrightness(s32 brightness);

static const u8 sBattleHallAppSummaryPages[] = { 0, 1, 2, 3, 4 };

static const GraphicsModes sBattleHallAppGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_2D,
};

static const BgTemplate sBattleHallAppBg1Template = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0x0800,
    .charBase = GX_BG_CHARBASE_0x0c000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 1,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sBattleHallAppBg0Template = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0x0000,
    .charBase = GX_BG_CHARBASE_0x04000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sBattleHallAppBg3Template = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0x2000,
    .charBase = GX_BG_CHARBASE_0x08000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sBattleHallAppSubBg0Template = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0x7800,
    .charBase = GX_BG_CHARBASE_0x04000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sBattleHallAppSubBg1Template = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0x7000,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 1,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const GraphicsBanks sBattleHallAppGraphicsBanks = {
    .bg = GX_VRAM_BG_128_C,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_32_H,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_64_E,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_01_AB,
    .texpltt = GX_VRAM_TEXPLTT_01_FG,
};

BOOL BattleHallApp_Init(OverlayManager *man, int *state) {
    BattleHallApp *app;
    BattleHallAppArgs *args;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_80), OVY_LOAD_ASYNC);
    BattleHallApp_DisableDisplay();
    Heap_Create(HEAP_ID_3, HEAP_ID_105, 0x20000);
    app = OverlayManager_CreateAndGetData(man, sizeof(BattleHallApp), HEAP_ID_105);
    memset(app, 0, sizeof(BattleHallApp));
    app->bgConfig = BgConfig_Alloc(HEAP_ID_105);
    app->manager = man;
    args = OverlayManager_GetArgs(man);
    app->saveData = args->saveData;
    app->mode = args->mode;
    app->resultOut = &args->result;
    app->options = Save_PlayerData_GetOptionsAddr(app->saveData);
    app->party = args->party;
    app->hallRecords = args->hallRecords;
    app->partnerLevel = args->partnerLevel;
    app->battleNo = args->battleNo + 1;
    app->cursor = args->cursor;
    app->partnerCursor = NO_LINK_CHOICE;
    app->linkChoice = NO_LINK_CHOICE;
    app->savedCursor = NO_SAVED_CURSOR;
    BattleHallApp_InitGraphics(app);
    if (BattleHall_IsMultiType(app->mode) == TRUE) {
        FrontierLink_RegisterCommands(app);
    }
    *state = HALL_STATE_FADE_IN;
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    return TRUE;
}

BOOL BattleHallApp_Main(OverlayManager *man, int *state) {
    BattleHallApp *app = OverlayManager_GetData(man);

    if (app->linkChoice != NO_LINK_CHOICE && *state == HALL_STATE_SELECT && app->summaryPending == 0) {
        BattleHallApp_ResetYesNo(app);
        BattleHallApp_SetMainState(app, state, HALL_STATE_LINK_DECIDE);
    }
    switch (*state) {
    case HALL_STATE_SUMMARY:
        if (BattleHallApp_RunSummary(app) == TRUE) {
            BattleHallApp_SetMainState(app, state, HALL_STATE_FADE_IN);
        } else {
            return FALSE;
        }
    case HALL_STATE_FADE_IN:
        if (BattleHallApp_FadeIn(app) == TRUE) {
            BattleHallApp_SetMainState(app, state, HALL_STATE_SELECT);
        }
        break;
    case HALL_STATE_SELECT:
        if (BattleHallApp_SelectType(app) == TRUE) {
            if (app->linkDecided == 1) {
                BattleHallApp_SetMainState(app, state, HALL_STATE_LINK_DECIDE);
            } else if (app->openSummary == 1) {
                BattleHallApp_SetMainState(app, state, HALL_STATE_SUMMARY);
                return FALSE;
            } else if (BattleHall_IsMultiType(app->mode) == TRUE) {
                BattleHallApp_SetMainState(app, state, HALL_STATE_LINK_SYNC);
            } else {
                BattleHallApp_SetMainState(app, state, HALL_STATE_FADE_OUT);
            }
        }
        break;
    case HALL_STATE_LINK_DECIDE:
        if (BattleHallApp_LinkDecide(app) == TRUE) {
            if (app->reselect == 1) {
                app->reselect = 0;
                BattleHallApp_SetMainState(app, state, HALL_STATE_SELECT);
            } else if (BattleHall_IsMultiType(app->mode) == TRUE) {
                BattleHallApp_SetMainState(app, state, HALL_STATE_LINK_SYNC);
            } else {
                BattleHallApp_SetMainState(app, state, HALL_STATE_FADE_OUT);
            }
        }
        break;
    case HALL_STATE_LINK_SYNC:
        if (BattleHallApp_LinkSync(app) == TRUE) {
            BattleHallApp_SetMainState(app, state, HALL_STATE_FADE_OUT);
        }
        break;
    case HALL_STATE_FADE_OUT:
        if (BattleHallApp_FadeOut(app) == TRUE) {
            return TRUE;
        }
        break;
    case HALL_STATE_CLOSE_PROMPT:
        if (BattleHallApp_ClosePrompt(app) == TRUE) {
            BattleHallApp_SetMainState(app, state, HALL_STATE_LINK_SYNC);
        }
        break;
    }
    SpriteList_RenderAndAnimateSprites(app->spriteGfx.spriteList);
    return FALSE;
}

BOOL BattleHallApp_Exit(OverlayManager *man, int *state) {
    BattleHallApp *app = OverlayManager_GetData(man);

    *app->resultOut = app->cursor;
    BattleHallApp_FreeGraphics(app);
    OverlayManager_FreeData(man);
    reg_GX_POWCNT |= 0x8000;
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_105);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_80));
    return TRUE;
}

enum {
    HALL_FADE_IN_START_SYNC,
    HALL_FADE_IN_WAIT_SYNC,
    HALL_FADE_IN_DRAW,
    HALL_FADE_IN_WAIT_FADE,
};

static BOOL BattleHallApp_FadeIn(BattleHallApp *app) {
    switch (app->subState) {
    case HALL_FADE_IN_START_SYNC:
        if (app->linkReady == 0 && BattleHall_IsMultiType(app->mode) == TRUE) {
            LinkSync_ClearPackets();
            LinkSync_StartSync(0x70);
        }
        app->subState++;
        break;
    case HALL_FADE_IN_WAIT_SYNC:
        if (app->linkReady == 0 && BattleHall_IsMultiType(app->mode) == TRUE) {
            if (LinkSync_IsSynced(0x70) == TRUE) {
                LinkSync_ClearPackets();
                app->linkReady = 1;
                app->subState++;
            }
        } else {
            app->subState++;
        }
        break;
    case HALL_FADE_IN_DRAW:
        BattleHallApp_PrintTypeGrid(app);
        BattleHallApp_PrintTitle(app);
        BeginNormalPaletteFade(0, 1, 1, RGB_BLACK, 6, 3, HEAP_ID_105);
        app->subState++;
        break;
    case HALL_FADE_IN_WAIT_FADE:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static void BattleHallApp_PrintTypeGrid(BattleHallApp *app) {
    BattleHallApp_PrintTypeNames(app, &app->windows[2], 1, 2, 0, 0);
    BattleHallApp_PrintTypeRanks(app, &app->windows[2]);
    BattleHallApp_PrintMonName(app, &app->windows[1], 0, 0, 1, 2, 0, 0);
}

static void BattleHallApp_PrintTitle(BattleHallApp *app) {
    u8 x = GetWindowX(&app->windows[3]) - 1;
    u8 y = GetWindowY(&app->windows[3]) - 1;
    u8 width = GetWindowWidth(&app->windows[3]) + 2;
    u8 height = GetWindowHeight(&app->windows[3]) + 2;
    u8 bgId = GetWindowBgId(&app->windows[3]);

    BattleHallApp_PrintSelectPrompt(app, &app->windows[3], 0, 0, 1, 2, 15, 0);
    BattleHallApp_LoadTitleFrameGraphics(app, GF_BG_LYR_SUB_0);
    BattleHallApp_DrawFrame(app->bgConfig, bgId, x, y, width, height, 1, 1);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, bgId);
}

// Frame tiles: corners at tile+0/2/6/8, edges at tile+1/3/5/7
static void BattleHallApp_DrawFrame(BgConfig *bgConfig, u8 bgId, u8 x, u8 y, u8 width, u8 height, u16 tile, u16 palette) {
    FillBgTilemapRect(bgConfig, bgId, tile, x, y, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 2, x + width - 1, y, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 6, x, y + height - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 8, x + width - 1, y + height - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 1, x + 1, y, width - 2, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 7, x + 1, y + height - 1, width - 2, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 3, x, y + 1, 1, height - 2, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 5, x + width - 1, y + 1, 1, height - 2, palette);
}

static BOOL BattleHallApp_RunSummary(BattleHallApp *app) {
    if (app->subState == 0) {
        if (OverlayManager_Run(app->summaryApp) == TRUE) {
            app->summaryResult = app->summaryArgs->partySlot;
            Heap_Free(app->summaryArgs);
            Heap_Free(app->summaryApp);
            app->summaryApp = NULL;
            BattleHallApp_ResumeAfterSummary(app);
            app->summaryPending = 0;
            return TRUE;
        }
    }
    return FALSE;
}

enum {
    HALL_SELECT_INIT,
    HALL_SELECT_INPUT,
    HALL_SELECT_DIM,
    HALL_SELECT_HANDLE_YES_NO,
    HALL_SELECT_UNDIM,
    HALL_SELECT_MATRON_MESSAGE,
    HALL_SELECT_WAIT_MATRON_MESSAGE,
    HALL_SELECT_OPEN_SUMMARY,
};

static BOOL BattleHallApp_SelectType(BattleHallApp *app) {
    switch (app->subState) {
    case HALL_SELECT_INIT:
        app->openSummary = 0;
        app->subState = HALL_SELECT_INPUT;
        break;
    case HALL_SELECT_INPUT:
        BattleHallApp_HandleDpad(app, gSystem.newKeys);
        if (!BattleHallApp_HandleConfirm(app) && !BattleHallApp_HandleTouch(app)) {
            break;
        }
        if (BattleHall_GetCellType(app->cursor) == TYPE_CELL_EMPTY) {
            // The empty cell opens the Pokemon's summary
            PlaySE(SEQ_SE_DP_DECIDE);
            BeginNormalPaletteFade(0, 0, 0, RGB_BLACK, 6, 1, HEAP_ID_105);
            app->summaryPending = 1;
            app->subState = HALL_SELECT_OPEN_SUMMARY;
            break;
        }
        if (BattleHallApp_IsMatronMode(app) == 1) {
            if (BattleHall_GetCellType(app->cursor) != TYPE_MYSTERY) {
                PlaySE(SEQ_SE_DP_BOX03);
                return FALSE;
            }
        } else {
            if (HallTypeRanks_Get(BattleHallApp_GetRecordIndex(app->cursor), app->hallRecords) >= MAX_TYPE_RANK) {
                PlaySE(SEQ_SE_DP_BOX03);
                return FALSE;
            }
            if (BattleHall_GetCellType(app->cursor) == TYPE_MYSTERY) {
                PlaySE(SEQ_SE_DP_BOX03);
                return FALSE;
            }
        }
        PlaySE(SEQ_SE_DP_DECIDE);
        if (!BattleHall_IsMultiType(app->mode)) {
            BattleHallApp_SetCellPalette(app->bgConfig, app->cursor, HALL_CELL_SELECTED);
            ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_3);
        }
        app->brightness = 0;
        app->subState = HALL_SELECT_DIM;
        break;
    case HALL_SELECT_DIM:
        app->brightness++;
        BattleHallApp_SetBrightness(-app->brightness);
        if (app->brightness == 8) {
            if (!BattleHall_IsMultiType(app->mode)) {
                BattleHallApp_ShowConfirmPrompt(app);
                app->subState = HALL_SELECT_HANDLE_YES_NO;
            } else {
                BattleHallApp_DrawMessageFrame(&app->windows[0], Options_GetFrame(app->options));
                app->printerId = BattleHallApp_ShowMessage(app, msg_0441_00000, 1);
                app->linkCursor = app->cursor;
                app->linkDecided = 1;
                return TRUE;
            }
        }
        break;
    case HALL_SELECT_HANDLE_YES_NO:
        switch (BattleHallApp_HandleYesNo(app->yesNoPrompt)) {
        case YESNORESPONSE_WAIT:
            break;
        case YESNORESPONSE_YES:
            BattleHallApp_ResetYesNo(app);
            if (BattleHallApp_IsMatronBattle(app) == 1 && BattleHallApp_IsMatronMode(app) == 0) {
                app->savedCursor = app->cursor;
                app->subState = HALL_SELECT_MATRON_MESSAGE;
                break;
            }
            StopSE(SEQ_SE_DP_SELECT, 0);
            PlaySE(SEQ_SE_DP_UG_020);
            if (app->savedCursor != NO_SAVED_CURSOR) {
                app->cursor = app->savedCursor;
            }
            return TRUE;
        case YESNORESPONSE_NO:
            BattleHallApp_ResetYesNo(app);
            app->subState = HALL_SELECT_UNDIM;
            break;
        }
        break;
    case HALL_SELECT_UNDIM:
        app->brightness--;
        BattleHallApp_SetBrightness(-app->brightness);
        if (app->brightness <= 0) {
            BattleHallApp_CancelConfirm(app);
            if (app->matronMode == TRUE) {
                app->subState = HALL_SELECT_INIT;
            } else if (app->matronMode == FALSE) {
                app->subState = HALL_SELECT_INPUT;
            } else {
                GF_AssertFail();
            }
        }
        break;
    case HALL_SELECT_MATRON_MESSAGE:
        // The Hall Matron interrupts the selection
        BattleHallApp_DrawMessageFrame(&app->windows[0], Options_GetFrame(app->options));
        app->printerId = BattleHallApp_PrintMessage(app, &app->windows[0], msg_0441_00032, 1, 1, Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(app->saveData)), 1, 2, 15, 1);
        CopyWindowToVram(&app->windows[0]);
        app->subState = HALL_SELECT_WAIT_MATRON_MESSAGE;
        break;
    case HALL_SELECT_WAIT_MATRON_MESSAGE:
        if (!TextPrinterCheckActive(app->printerId)) {
            int x, y;
            app->matronMode = TRUE;
            BattleHallApp_ClearMessage(app);
            BattleHallSprite_SetAnim(app->cursorSprite, 1);
            BattleHallApp_SetCellPalette(app->bgConfig, TYPE_CELL_MATRON, HALL_CELL_NORMAL);
            BattleHallApp_GrayOutTypes(app, app->bgConfig);
            app->cursor = TYPE_CELL_MATRON;
            x = BattleHallApp_GetCursorX(app);
            y = BattleHallApp_GetCursorY(app);
            BattleHallSprite_SetPosition(app->cursorSprite, x, y);
            app->subState = HALL_SELECT_UNDIM;
        }
        break;
    case HALL_SELECT_OPEN_SUMMARY:
        if (IsPaletteFadeFinished() == TRUE) {
            BattleHallApp_CreateSummaryArgs(app);
            BattleHallApp_FreeGraphics(app);
            app->summaryApp = OverlayManager_New(&gOverlayTemplate_PokemonSummary, app->summaryArgs, HEAP_ID_105);
            app->openSummary = 1;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

enum {
    HALL_LINK_CMD_PING = 4,
    HALL_LINK_CMD_CHOICE,
    HALL_LINK_CMD_ANSWER,
};

enum {
    HALL_LINK_DECIDE_SEND_CHOICE,
    HALL_LINK_DECIDE_WAIT_PARTNER,
    HALL_LINK_DECIDE_CONFIRM,
    HALL_LINK_DECIDE_SHOW_PARTNER_CHOICE,
    HALL_LINK_DECIDE_WAIT_PARTNER_ANSWER,
    HALL_LINK_DECIDE_HANDLE_YES_NO,
    HALL_LINK_DECIDE_SEND_ACCEPT,
    HALL_LINK_DECIDE_SEND_DECLINE,
    HALL_LINK_DECIDE_START_SYNC,
    HALL_LINK_DECIDE_WAIT_SYNC,
};

static BOOL BattleHallApp_LinkDecide(BattleHallApp *app) {
    switch (app->subState) {
    case HALL_LINK_DECIDE_SEND_CHOICE:
        if (BattleHallApp_SendLinkCommand(app, HALL_LINK_CMD_CHOICE, BattleHallApp_GetRecordIndex(app->linkCursor)) == TRUE) {
            app->timer = 0;
            app->linkCursor = 0;
            app->linkDecided = 0;
            app->subState = HALL_LINK_DECIDE_WAIT_PARTNER;
        }
        break;
    case HALL_LINK_DECIDE_WAIT_PARTNER:
        if (app->timer != 0) {
            app->timer--;
        } else if (app->recvCount >= 2) {
            app->recvCount = 0;
            // The parent's choice wins unless the child chose first
            if (app->linkChoice < 20) {
                if (Comm_GetLocalNetId() == 0) {
                    app->subState = HALL_LINK_DECIDE_CONFIRM;
                } else {
                    app->subState = HALL_LINK_DECIDE_SHOW_PARTNER_CHOICE;
                }
            } else {
                if (Comm_GetLocalNetId() == 0) {
                    app->subState = HALL_LINK_DECIDE_SHOW_PARTNER_CHOICE;
                } else {
                    app->subState = HALL_LINK_DECIDE_CONFIRM;
                }
            }
        }
        break;
    case HALL_LINK_DECIDE_SHOW_PARTNER_CHOICE:
        BattleHallApp_SetBrightness(0);
        app->brightness = 0;
        BattleHallSprite_SetVisible(app->monIcon, FALSE);
        BattleHallApp_DrawMessageFrame(&app->windows[0], Options_GetFrame(app->options));
        Frontier_BufferPartnerName(app->msgFormat, 0);
        app->printerId = BattleHallApp_ShowMessage(app, msg_0441_00011, 1);
        app->timer = 10;
        BattleHallApp_SetCellPalette(app->bgConfig, app->partnerCursor, HALL_CELL_PARTNER_CHOICE);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_3);
        app->subState = HALL_LINK_DECIDE_WAIT_PARTNER_ANSWER;
        break;
    case HALL_LINK_DECIDE_WAIT_PARTNER_ANSWER:
        if (app->partnerAnswer != 0) {
            if (app->partnerAnswer == 1) {
                app->cursor = app->partnerCursor;
                return TRUE;
            }
            BattleHallApp_ClearMessage(app);
            BattleHallApp_SetCellPalette(app->bgConfig, app->partnerCursor, HALL_CELL_NORMAL);
            ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_3);
            app->partnerCursor = NO_LINK_CHOICE;
            app->partnerAnswer = 0;
            app->linkChoice = NO_LINK_CHOICE;
            app->reselect = 1;
            return TRUE;
        }
        break;
    case HALL_LINK_DECIDE_CONFIRM:
        BattleHallApp_SetCellPalette(app->bgConfig, app->cursor, HALL_CELL_SELECTED);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_3);
        BattleHallApp_ShowConfirmPrompt(app);
        app->subState = HALL_LINK_DECIDE_HANDLE_YES_NO;
        break;
    case HALL_LINK_DECIDE_HANDLE_YES_NO:
        switch (BattleHallApp_HandleYesNo(app->yesNoPrompt)) {
        case YESNORESPONSE_WAIT:
            break;
        case YESNORESPONSE_YES:
            StopSE(SEQ_SE_DP_SELECT, 0);
            PlaySE(SEQ_SE_DP_UG_020);
            BattleHallApp_ResetYesNo(app);
            app->subState = HALL_LINK_DECIDE_SEND_ACCEPT;
            break;
        case YESNORESPONSE_NO:
            BattleHallApp_ResetYesNo(app);
            app->subState = HALL_LINK_DECIDE_SEND_DECLINE;
            break;
        }
        break;
    case HALL_LINK_DECIDE_SEND_ACCEPT:
        if (BattleHallApp_SendLinkCommand(app, HALL_LINK_CMD_ANSWER, 1) == TRUE) {
            return TRUE;
        }
        break;
    case HALL_LINK_DECIDE_SEND_DECLINE:
        if (app->brightness > 0) {
            app->brightness--;
            BattleHallApp_SetBrightness(-app->brightness);
        } else if (BattleHallApp_SendLinkCommand(app, HALL_LINK_CMD_ANSWER, 2) == TRUE) {
            BattleHallApp_CancelConfirm(app);
            app->partnerCursor = NO_LINK_CHOICE;
            app->linkChoice = NO_LINK_CHOICE;
            app->reselect = 1;
            return TRUE;
        }
        break;
    case HALL_LINK_DECIDE_START_SYNC:
        LinkSync_ClearPackets();
        LinkSync_StartSync(0x68);
        app->subState = HALL_LINK_DECIDE_WAIT_SYNC;
        break;
    case HALL_LINK_DECIDE_WAIT_SYNC:
        if (LinkSync_IsSynced(0x68) == TRUE) {
            LinkSync_ClearPackets();
            LinkSync_Init(HEAP_ID_105);
            app->linkChoice = NO_LINK_CHOICE;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

enum {
    HALL_LINK_SYNC_START,
    HALL_LINK_SYNC_WAIT,
};

static BOOL BattleHallApp_LinkSync(BattleHallApp *app) {
    switch (app->subState) {
    case HALL_LINK_SYNC_START:
        LinkSync_ClearPackets();
        LinkSync_StartSync(0x68);
        app->subState++;
        break;
    case HALL_LINK_SYNC_WAIT:
        if (LinkSync_IsSynced(0x68) == 1) {
            LinkSync_ClearPackets();
            return TRUE;
        }
        break;
    }
    return FALSE;
}

enum {
    HALL_FADE_OUT_DELAY,
    HALL_FADE_OUT_START,
    HALL_FADE_OUT_WAIT,
};

static BOOL BattleHallApp_FadeOut(BattleHallApp *app) {
    switch (app->subState) {
    case HALL_FADE_OUT_DELAY:
        app->timer = 10;
        app->subState++;
        break;
    case HALL_FADE_OUT_START:
        app->timer--;
        if (app->timer == 0) {
            BattleHallApp_ClearMessage(app);
            BeginNormalPaletteFade(0, 0, 0, RGB_BLACK, 6, 1, HEAP_ID_105);
            app->subState++;
        }
        break;
    case HALL_FADE_OUT_WAIT:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

enum {
    HALL_CLOSE_PROMPT_CLOSE,
    HALL_CLOSE_PROMPT_DONE,
};

static BOOL BattleHallApp_ClosePrompt(BattleHallApp *app) {
    switch (app->subState) {
    case HALL_CLOSE_PROMPT_CLOSE:
        BattleHallApp_ResetYesNo(app);
        BattleHallSprite_SetVisible(app->monIcon, TRUE);
        ClearFrameAndWindow2(&app->windows[0], FALSE);
        app->subState++;
        break;
    case HALL_CLOSE_PROMPT_DONE:
        return TRUE;
    }
    return FALSE;
}

static void BattleHallApp_FreeGraphics(BattleHallApp *app) {
    int i;

    if (app->cursorSprite != NULL) {
        BattleHallSprite_Delete(app->cursorSprite);
    }
    if (app->monIcon != NULL) {
        BattleHallSprite_Delete(app->monIcon);
    }
    BattleHallApp_DestroyYesNo(app->yesNoPrompt);
    WirelessStatusIcon_Destroy();
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_BG);
    PaletteData_Free(app->paletteData);
    app->paletteData = NULL;
    BattleHallSpriteGfx_Free(&app->spriteGfx);
    DestroyMsgData(app->msgData);
    MessageFormat_Delete(app->msgFormat);
    String_Delete(app->expandedString);
    String_Delete(app->rawString);
    MessagePrinter_Delete(app->messagePrinter);
    i = 0;
    do {
        String_Delete(app->strings[i]);
        i++;
    } while (i < 2);
    BattleHallApp_RemoveWindows(app->windows);
    BattleHallApp_FreeBgLayers(app->bgConfig);
    NARC_Delete(app->narc);
}

static void BattleHallApp_ResumeAfterSummary(BattleHallApp *app) {
    BattleHallApp_DisableDisplay();
    app->bgConfig = BgConfig_Alloc(HEAP_ID_105);
    BattleHallApp_InitGraphics(app);
    if (BattleHall_GetCellType(app->cursor) == TYPE_CELL_EMPTY) {
        BattleHallSprite_SetAnim(app->cursorSprite, 2);
        BattleHallSprite_SetPosition(app->cursorSprite, 128, 168);
    }
}

static void BattleHallApp_DisableDisplay(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
}

static void BattleHallApp_InitGraphics(BattleHallApp *app) {
    int i, x;
    u16 y;

    app->narc = NARC_New(NARC_a_1_8_3, HEAP_ID_105);
    BattleHallApp_InitBgGraphics(app);
    BattleHallApp_InitSprites(app);
    app->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0441_bin, HEAP_ID_105);
    app->msgFormat = MessageFormat_New(HEAP_ID_105);
    app->expandedString = String_New(600, HEAP_ID_105);
    app->rawString = String_New(600, HEAP_ID_105);
    for (i = 0; i < 2; i++) {
        app->strings[i] = String_New(32, HEAP_ID_105);
    }
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_105);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, HEAP_ID_105);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_2_OFFSET, HEAP_ID_105);
    app->messagePrinter = MessagePrinter_New(15, 14, 0, HEAP_ID_105);
    BattleHallApp_AddWindows(app->bgConfig, app->windows);
    BattleHallApp_GrayOutTypes(app, app->bgConfig);
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
    GfGfx_BothDispOn();
    x = BattleHallApp_GetCursorX(app);
    y = BattleHallApp_GetCursorY(app);
    app->cursorSprite = BattleHallSprite_Create(&app->spriteGfx, 0, 1, x, y, NULL);
    app->monIcon = BattleHallSprite_Create(&app->spriteGfx, 1, 1, 160, 160, NULL);
    BattleHallSprite_SetIconPalette(app->monIcon, Party_GetMonByIndex(app->party, 0));
    if (Comm_IsActive()) {
        G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
        G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
        WirelessStatusIcon_Create();
    }
    app->yesNoPrompt = BattleHallApp_CreateYesNo(HEAP_ID_105);
    Main_SetVBlankIntrCB((GFIntrCB)BattleHallApp_VBlankCallback, app);
}

static void BattleHallApp_InitBgGraphics(BattleHallApp *app) {
    BattleHallApp_SetGraphicsBanks();
    BattleHallApp_InitBgLayers(app->bgConfig);
    BattleHallApp_InitLinkWindow(app);
    app->paletteData = PaletteData_Init(HEAP_ID_105);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_OBJ, 0x200, HEAP_ID_105);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_105);
    BattleHallApp_LoadTypeGridGraphics(app, GF_BG_LYR_MAIN_3);
    BattleHallApp_LoadBgPalette();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    BattleHallApp_LoadSubBgGraphics(app, GF_BG_LYR_SUB_1);
}

static void BattleHallApp_InitSprites(BattleHallApp *app) {
    Pokemon *mon = Party_GetMonByIndex(app->party, 0);
    BattleHallSpriteGfx_Init(&app->spriteGfx, mon);
}

static void BattleHallApp_FreeBgLayers(BgConfig *bgConfig) {
    reg_GX_DISPCNT &= ~(REG_GX_DISPCNT_W0_MASK | REG_GX_DISPCNT_W1_MASK | REG_GX_DISPCNT_OW_MASK);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_ALL, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_ALL, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    Heap_Free(bgConfig);
}

static void BattleHallApp_VBlankCallback(BattleHallApp *app) {
    if (app->summaryApp == NULL) {
        if (app->paletteData != NULL) {
            PaletteData_PushTransparentBuffers(app->paletteData);
        }
        DoScheduledBgGpuUpdates(app->bgConfig);
        GF_RunVramTransferTasks();
        OamManager_ApplyAndResetBuffers();
        OS_SetIrqCheckFlag(OS_IE_V_BLANK);
    }
}

static void BattleHallApp_SetGraphicsBanks(void) {
    GraphicsBanks banks = sBattleHallAppGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

static void BattleHallApp_InitBgLayers(BgConfig *bgConfig) {
    GraphicsModes modes = sBattleHallAppGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    {
        BgTemplate bg = sBattleHallAppBg0Template;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &bg, GF_BG_TYPE_TEXT);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_105);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    }
    {
        BgTemplate bg = sBattleHallAppBg1Template;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &bg, GF_BG_TYPE_TEXT);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, HEAP_ID_105);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    }
    {
        BgTemplate bg = sBattleHallAppBg3Template;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &bg, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    }
    {
        BgTemplate bg = sBattleHallAppSubBg1Template;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &bg, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    }
    {
        BgTemplate bg = sBattleHallAppSubBg0Template;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &bg, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    }
    G2_SetBG0Priority(0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, TRUE);
}

// In link mode the top 16 lines of the main screen are masked
static void BattleHallApp_InitLinkWindow(BattleHallApp *app) {
    if (BattleHall_IsMultiType(app->mode) != FALSE) {
        reg_GX_DISPCNT &= ~(REG_GX_DISPCNT_W0_MASK | REG_GX_DISPCNT_W1_MASK | REG_GX_DISPCNT_OW_MASK);
        G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, FALSE);
        G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
        G2_SetWnd0Position(240, 0, 0, 16);
        GX_SetVisibleWnd(GX_WNDMASK_W0);
    }
}

static void BattleHallApp_LoadTypeGridGraphics(BattleHallApp *app, u8 bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 0x17, app->bgConfig, (GFBgLayer)bgId, 0, 0, TRUE, HEAP_ID_105);
    GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 0x18, app->bgConfig, (GFBgLayer)bgId, 0, 0, TRUE, HEAP_ID_105);
}

static void BattleHallApp_LoadBgPalette(void) {
    NNSG2dPaletteData *plttData;
    void *file = GfGfxLoader_GetPlttData(NARC_a_1_8_3, 0x99, &plttData, HEAP_ID_105);
    DC_FlushRange(plttData->pRawData, 0xC0);
    GX_LoadBGPltt(plttData->pRawData, 0, 0xC0);
    Heap_Free(file);
}

static void BattleHallApp_LoadSubBgGraphics(BattleHallApp *app, u32 bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 0x81, app->bgConfig, (GFBgLayer)bgId, 0, 0, TRUE, HEAP_ID_105);
    GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 0x82, app->bgConfig, (GFBgLayer)bgId, 0, 0, TRUE, HEAP_ID_105);
    GfGfxLoader_GXLoadPalFromOpenNarc(app->narc, 0xBE, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_105);
}

static void BattleHallApp_LoadTitleFrameGraphics(BattleHallApp *app, u32 bgId) {
    BG_ClearCharDataRange((u8)bgId, 0x20, 0, HEAP_ID_105);
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 0x85, app->bgConfig, (GFBgLayer)bgId, 1, 0, TRUE, HEAP_ID_105);
    GfGfxLoader_GXLoadPalFromOpenNarc(app->narc, 0xC0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_1_OFFSET, 0x20, HEAP_ID_105);
}

static u8 BattleHallApp_PrintMessage(BattleHallApp *app, Window *window, u32 msgId, u32 x, u32 y, u32 textSpeed, u8 fg, u8 shadow, u8 bg, u8 fontId) {
    FillWindowPixelBuffer(window, bg);
    ReadMsgDataIntoString(app->msgData, msgId, app->rawString);
    StringExpandPlaceholders(app->msgFormat, app->expandedString, app->rawString);
    return AddTextPrinterParameterizedWithColor(window, fontId, app->expandedString, x, y, textSpeed, MAKE_TEXT_COLOR(fg, shadow, bg), NULL);
}

static u8 BattleHallApp_ShowMessage(BattleHallApp *app, u32 msgId, u8 fontId) {
    u8 printerId = BattleHallApp_PrintMessage(app, &app->windows[0], msgId, 1, 1, 0, 1, 2, 15, fontId);
    CopyWindowToVram(&app->windows[0]);
    return printerId;
}

static void BattleHallApp_BufferNumber(BattleHallApp *app, u32 field, u32 value) {
    BufferIntegerAsString(app->msgFormat, field, value, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
}

static void BattleHallApp_PrintMonName(BattleHallApp *app, Window *window, int x, int y, u8 fg, u8 shadow, u8 bg, u8 fontId) {
    u16 name[12];
    String *string;

    GetMonData(Party_GetMonByIndex(app->party, 0), MON_DATA_SPECIES_NAME, name);
    FillWindowPixelBuffer(window, bg);
    string = String_New(11, HEAP_ID_105);
    CopyU16ArrayToString(string, name);
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, 0, MAKE_TEXT_COLOR(fg, shadow, bg), NULL);
    String_Delete(string);
    CopyWindowToVram(window);
}

static void BattleHallApp_PrintTypeNames(BattleHallApp *app, Window *window, int fg, int shadow, u8 bg, u8 fontId) {
    int row;
    MsgData *msgData;
    int base;
    int y;
    int column;
    int index;
    String *string;
    int x;

    FillWindowPixelBuffer(window, bg);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0735_bin, HEAP_ID_105);
    string = String_New(11, HEAP_ID_105);
    for (row = 0, base = 0, y = 16; row < TYPE_GRID_ROWS; base += TYPE_GRID_COLUMNS, y += 36, row++) {
        for (column = 0, x = 1; column < TYPE_GRID_COLUMNS; column++, x += 64) {
            index = column + base;
            if (BattleHall_GetCellType(index) != TYPE_CELL_EMPTY) {
                String_SetEmpty(string);
                ReadMsgDataIntoString(msgData, BattleHall_GetCellType(index), string);
                AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, 0, MAKE_TEXT_COLOR(fg, shadow, bg), NULL);
            }
        }
    }
    String_Delete(string);
    DestroyMsgData(msgData);
    CopyWindowToVram(window);
}

static void BattleHallApp_PrintNumber(BattleHallApp *app, Window *window, u32 value, u32 x, u32 y) {
    PrintUIntOnWindow(app->messagePrinter, value, 2, PRINTING_MODE_LEFT_ALIGN, window, x, y);
    ScheduleWindowCopyToVram(window);
}

static void BattleHallApp_PrintTypeRanks(BattleHallApp *app, Window *window) {
    int row;
    int base;
    int y;

    row = 0;
    base = 0;
    y = 4;
    do {
        int column = 0;
        int x = 18;
        do {
            int index = column + base;
            u32 type = BattleHall_GetCellType((u8)index);
            if (type != TYPE_CELL_EMPTY && type != TYPE_MYSTERY) {
                u8 rank = (u8)(HallTypeRanks_Get(BattleHallApp_GetRecordIndex((u8)index), app->hallRecords) + 1);
                if (rank > MAX_TYPE_RANK) {
                    rank = MAX_TYPE_RANK;
                }
                BattleHallApp_PrintNumber(app, window, rank, x, y);
            }
            column++;
            x += 64;
        } while (column < TYPE_GRID_COLUMNS);
        base += TYPE_GRID_COLUMNS;
        y += 36;
        row++;
    } while (row < TYPE_GRID_ROWS);
}

static void BattleHallApp_PrintSelectPrompt(BattleHallApp *app, Window *window, u32 x, u32 y, u8 fg, u8 shadow, u8 bg, u8 fontId) {
    MsgData *msgData;
    String *string;

    FillWindowPixelBuffer(window, bg);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0441_bin, HEAP_ID_105);
    string = NewString_ReadMsgData(msgData, msg_0441_00037);
    FillWindowPixelBuffer(window, 15);
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, 0, MAKE_TEXT_COLOR(fg, shadow, bg), NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    CopyWindowToVram(window);
}

static void BattleHallApp_CreateSummaryArgs(BattleHallApp *app) {
    app->summaryArgs = Heap_Alloc(HEAP_ID_105, sizeof(PokemonSummaryArgs));
    memset(app->summaryArgs, 0, sizeof(PokemonSummaryArgs));
    app->summaryArgs->party = app->party;
    app->summaryArgs->monDataType = 1;
    app->summaryArgs->options = app->options;
    app->summaryArgs->mode = 1;
    app->summaryArgs->partyCount = Party_GetCount(app->party);
    app->summaryArgs->partySlot = 0;
    app->summaryArgs->moveToLearn = 0;
    app->summaryArgs->natDexEnabled = SaveArray_IsNatDexEnabled(app->saveData);
    app->summaryArgs->unk2C = PokemonSummary_GetDefaultUnk2C(app->saveData);
    app->summaryArgs->ribbons = Save_SpecialRibbons_Get(app->saveData);
    app->summaryArgs->isFlag982Set = 0;
    PokemonSummary_SetPages(app->summaryArgs, sBattleHallAppSummaryPages);
    PokemonSummaryArgs_SetPlayer(app->summaryArgs, Save_PlayerData_GetProfile(app->saveData));
}

static void BattleHallApp_SetMainState(BattleHallApp *app, int *state, int nextState) {
    app->subState = 0;
    *state = nextState;
}

// The grid is 4 columns by 5 rows; the empty cells (row 4) select the summary button
static void BattleHallApp_HandleDpad(BattleHallApp *app, int unused) {
    BOOL moved = FALSE;
    u8 type;

    if (gSystem.newKeys & PAD_KEY_LEFT) {
        if (BattleHall_GetCellType(app->cursor) != TYPE_CELL_EMPTY) {
            app->prevCursor = app->cursor;
        }
        if (app->cursor % 4 == 0) {
            app->cursor += 3;
        } else if (BattleHall_GetCellType(app->cursor) == TYPE_CELL_EMPTY) {
            app->cursor = 16;
        } else {
            app->cursor--;
        }
        moved = TRUE;
    }
    if (gSystem.newKeys & PAD_KEY_RIGHT) {
        if (BattleHall_GetCellType(app->cursor) != TYPE_CELL_EMPTY) {
            app->prevCursor = app->cursor;
        }
        if (app->cursor % 4 == 3) {
            app->cursor -= 3;
        } else if (BattleHall_GetCellType(app->cursor) == TYPE_CELL_EMPTY) {
            app->cursor = 19;
        } else {
            app->cursor++;
        }
        moved = TRUE;
    }
    if (gSystem.newKeys & PAD_KEY_UP) {
        if (BattleHall_GetCellType(app->cursor) != TYPE_CELL_EMPTY) {
            app->prevCursor = app->cursor;
        }
        if (app->cursor < 4) {
            app->cursor += 16;
        } else if (BattleHall_GetCellType(app->cursor) == TYPE_CELL_EMPTY) {
            if (app->prevCursor == 16) {
                app->cursor = 13;
            } else if (app->prevCursor == 19) {
                app->cursor = 14;
            } else if (app->prevCursor == 13 || app->prevCursor == 1) {
                app->cursor = 13;
            } else if (app->prevCursor == 14 || app->prevCursor == 2) {
                app->cursor = 14;
            } else {
                app->cursor = 13;
            }
        } else {
            app->cursor -= 4;
        }
        moved = TRUE;
    }
    if (gSystem.newKeys & PAD_KEY_DOWN) {
        if (BattleHall_GetCellType(app->cursor) != TYPE_CELL_EMPTY) {
            app->prevCursor = app->cursor;
        }
        if (app->cursor >= 16) {
            app->cursor -= 16;
        } else {
            app->cursor += 4;
        }
        moved = TRUE;
    }
    if (moved == TRUE) {
        PlaySE(SEQ_SE_DP_SELECT);
        BattleHallSprite_SetPosition(app->cursorSprite, BattleHallApp_GetCursorX(app), BattleHallApp_GetCursorY(app));
    }
    type = BattleHall_GetCellType(app->cursor);
    if (type == TYPE_CELL_EMPTY) {
        BattleHallSprite_SetAnim(app->cursorSprite, 2);
        BattleHallSprite_SetPosition(app->cursorSprite, 128, 168);
    } else {
        BattleHallSprite_SetAnim(app->cursorSprite, 1);
    }
}

static BOOL BattleHallApp_HandleTouch(BattleHallApp *app) {
    u16 x, y;
    int row, column;
    int top, bottom, left, right;

    if (gSystem.touchNew != 0) {
        x = gSystem.touchX;
        y = gSystem.touchY;
        for (row = 0; row < TYPE_GRID_ROWS; row++) {
            top = row * 36 + 3;
            bottom = top + 35;
            for (column = 0; column < TYPE_GRID_COLUMNS; column++) {
                left = column * 64 + 1;
                right = left + 63;
                if (left <= x && x <= right && top <= y && y <= bottom) {
                    app->cursor = column + row * 4;
                    BattleHallSprite_SetPosition(app->cursorSprite, BattleHallApp_GetCursorX(app), BattleHallApp_GetCursorY(app));
                    if (BattleHall_GetCellType(app->cursor) == TYPE_CELL_EMPTY) {
                        BattleHallSprite_SetAnim(app->cursorSprite, 2);
                        BattleHallSprite_SetPosition(app->cursorSprite, 128, 168);
                    } else {
                        BattleHallSprite_SetAnim(app->cursorSprite, 1);
                    }
                    app->usedTouch = 1;
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

static BOOL BattleHallApp_HandleConfirm(BattleHallApp *app) {
    if (gSystem.newKeys & PAD_BUTTON_A) {
        app->usedTouch = 0;
        return TRUE;
    }
    return FALSE;
}

static u16 BattleHallApp_GetCursorX(BattleHallApp *app) {
    return (u16)((app->cursor % 4) * 64 + 32);
}

static u16 BattleHallApp_GetCursorY(BattleHallApp *app) {
    return (u16)((app->cursor / 4) * 36 + 16);
}

static void BattleHallApp_GrayOutTypes(BattleHallApp *app, BgConfig *bgConfig) {
    int i;

    if (BattleHallApp_IsMatronMode(app) == 1) {
        for (i = 0; i < 17; i++) {
            BattleHallApp_SetCellPalette(bgConfig, (u8)i, HALL_CELL_UNAVAILABLE);
        }
    } else {
        for (i = 0; i < 17; i++) {
            if (HallTypeRanks_Get((u8)i, app->hallRecords) >= MAX_TYPE_RANK) {
                BattleHallApp_SetCellPalette(bgConfig, (u8)i, HALL_CELL_UNAVAILABLE);
            }
        }
        BattleHallApp_SetCellPalette(bgConfig, TYPE_CELL_MATRON, HALL_CELL_UNAVAILABLE);
    }
    ScheduleBgTilemapBufferTransfer(bgConfig, GF_BG_LYR_MAIN_3);
}

// state: 0 = normal, 1 = selected, 2 = partner's choice, 3 = unavailable
static void BattleHallApp_SetCellPalette(BgConfig *bgConfig, int index, int state) {
    u8 palette;
    int x;
    u8 cellX;
    int column;
    u8 height;
    u8 y;

    if (state == HALL_CELL_NORMAL) {
        palette = 0;
    } else if (state == HALL_CELL_SELECTED) {
        palette = 5;
    } else if (state == HALL_CELL_PARTNER_CHOICE) {
        palette = 4;
    } else {
        palette = 3;
    }
    x = (index % 4) * 8;
    cellX = x;
    column = index % 8;
    if (column < 4) {
        height = 5;
    } else {
        height = 4;
    }
    if ((u32)index < 4) {
        y = 0;
    } else if ((u32)index < 8) {
        y = 5;
    } else if ((u32)index < 12) {
        y = 9;
    } else if ((u32)index < 16) {
        y = 14;
    } else {
        y = 18;
    }
    BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_MAIN_3, cellX, y, 8, height, palette);
    if (state == HALL_CELL_NORMAL) {
        cellX = x;
        if (column < 4) {
            height = 2;
        } else {
            height = 3;
        }
        if ((u32)index < 4) {
            y = 2;
        } else if ((u32)index < 8) {
            y = 6;
        } else if ((u32)index < 12) {
            y = 11;
        } else if ((u32)index < 16) {
            y = 15;
        } else {
            y = 20;
        }
        if ((u32)index < 9) {
            BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_MAIN_3, cellX, y, 1, height, 1);
        } else {
            BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_MAIN_3, cellX, y, 1, height, 2);
        }
    }
}

static u8 BattleHallApp_GetRecordIndex(u8 index) {
    if (index >= 17) {
        index = 17;
    }
    return index;
}

// The Hall Matron appears in the 50th and 170th single battles
static BOOL BattleHallApp_IsMatronBattle(BattleHallApp *app) {
    if (app->mode == 0) {
        if (app->battleNo == 50 || app->battleNo == 170) {
            return TRUE;
        }
    }
    return FALSE;
}

static u8 BattleHallApp_IsMatronMode(BattleHallApp *app) {
    return app->matronMode;
}

static BOOL BattleHallApp_SendLinkCommand(BattleHallApp *app, u16 command, u16 arg) {
    u32 id;

    if (BattleHall_IsMultiType(app->mode) == FALSE) {
        return FALSE;
    }
    switch (command) {
    case HALL_LINK_CMD_PING:
        id = 0x27;
        BattleHallApp_PrepareLinkPing(app, command);
        break;
    case HALL_LINK_CMD_CHOICE:
        id = 0x28;
        BattleHallApp_PrepareLinkChoice(app, command, arg);
        break;
    case HALL_LINK_CMD_ANSWER:
        id = 0x29;
        BattleHallApp_PrepareLinkAnswer(app, command, arg);
        break;
    }
    if (Comm_SendCommand(id, &app->linkCommand, 0x2C) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

static void BattleHallApp_PrepareLinkPing(BattleHallApp *app, u16 command) {
    Save_PlayerData_GetProfile(app->saveData);
    app->linkCommand = command;
}

void BattleHall_ReceivePing(u32 netId) {
    if (netId == Comm_GetLocalNetId()) {
        return;
    }
}

static void BattleHallApp_PrepareLinkChoice(BattleHallApp *app, u16 command, u16 choice) {
    Pokemon *mon;

    app->linkCommand = command;
    app->linkArg = choice;
    if (Comm_GetLocalNetId() == 0 && app->linkChoice == NO_LINK_CHOICE) {
        app->linkChoice = (u8)choice;
    }
    app->linkChoiceSent = app->linkChoice;
    mon = Party_GetMonByIndex(app->party, 0);
    app->linkLevel = (u16)GetMonData(mon, MON_DATA_LEVEL, NULL);
}

void BattleHall_ReceiveChoice(u32 netId, u32 unused, const u16 *msg, BattleHallApp *app) {
    app->recvCount++;
    if (netId == Comm_GetLocalNetId()) {
        return;
    }
    app->partnerCursor = (u8)msg[1];
    if (Comm_GetLocalNetId() == 0) {
        if (app->linkChoice != NO_LINK_CHOICE) {
            app->partnerCursor = 0;
        } else {
            app->linkChoice = (u8)(app->partnerCursor + 20);
        }
    } else {
        app->linkChoice = (u8)msg[2];
    }
    *app->partnerLevel = msg[3];
}

static void BattleHallApp_PrepareLinkAnswer(BattleHallApp *app, u16 command, u16 answer) {
    app->linkCommand = command;
    app->linkArg = answer;
}

void BattleHall_ReceiveAnswer(u32 netId, u32 unused, const u16 *msg, BattleHallApp *app) {
    u16 answer;

    if (netId == Comm_GetLocalNetId()) {
        return;
    }
    answer = msg[1];
    if (answer != 0) {
        app->partnerAnswer = (u8)answer;
    }
}

static void BattleHallApp_ResetYesNo(BattleHallApp *app) {
    if (app->yesNoActive == 1) {
        app->yesNoActive = 0;
        YesNoPrompt_Reset(app->yesNoPrompt);
    }
}

static void BattleHallApp_ShowConfirmPrompt(BattleHallApp *app) {
    u8 rank;
    int msgId;

    BattleHallSprite_SetVisible(app->monIcon, FALSE);
    BattleHallApp_DrawMessageFrame(&app->windows[0], Options_GetFrame(app->options));
    BufferTypeName(app->msgFormat, 0, BattleHall_GetCellType(app->cursor));
    rank = HallTypeRanks_Get(BattleHallApp_GetRecordIndex(app->cursor), app->hallRecords) + 1;
    if (rank > MAX_TYPE_RANK) {
        rank = MAX_TYPE_RANK;
    }
    BattleHallApp_BufferNumber(app, 1, rank);
    if (BattleHallApp_IsMatronMode(app) == 1) {
        msgId = msg_0441_00031;
    } else {
        msgId = msg_0441_00024;
    }
    app->printerId = BattleHallApp_ShowMessage(app, msgId, 1);
    BattleHallSprite_SetAnim(app->cursorSprite, 0);
    BattleHallApp_InitYesNo(app->yesNoPrompt, app->bgConfig, app->usedTouch);
    app->yesNoActive = 1;
}

static void BattleHallApp_CancelConfirm(BattleHallApp *app) {
    BattleHallApp_ClearMessage(app);
    BattleHallApp_SetCellPalette(app->bgConfig, app->cursor, HALL_CELL_NORMAL);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_3);
    BattleHallSprite_SetAnim(app->cursorSprite, 1);
}

static void BattleHallApp_ClearMessage(BattleHallApp *app) {
    BattleHallSprite_SetVisible(app->monIcon, TRUE);
    ClearFrameAndWindow2(&app->windows[0], FALSE);
    BattleHallApp_PrintMonName(app, &app->windows[1], 0, 0, 1, 2, 0, 0);
}

static void BattleHallApp_SetBrightness(s32 brightness) {
    G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, brightness);
}
