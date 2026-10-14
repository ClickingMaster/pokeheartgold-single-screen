#include "battle_room_results.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0028.h"

#include "bg_window.h"
#include "font.h"
#include "frontier_data.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "mail_message.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "screen_fade.h"
#include "sprite_system.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "sound_playback.h"
#include "sprite_resource_header.h"
#include "grid_input_handler.h"
#include "frontier_data_access.h"
#include "frontier_save_access.h"

#define BATTLE_ROOM_RESULTS_NUM_LEADERS        30
#define BATTLE_ROOM_RESULTS_LEADERS_PER_PAGE   12
#define BATTLE_ROOM_RESULTS_NUM_PAGES          3
#define BATTLE_ROOM_RESULTS_NUM_RESULT_WINDOWS 10
#define BATTLE_ROOM_RESULTS_NUM_LEADER_WINDOWS 35
#define BATTLE_ROOM_RESULTS_CANCEL             12

// One downloaded Battle Room leader.
typedef struct BattleRoomLeader {
    String *name;         // 0x0
    u8 gender;            // 0x4
    u8 country;           // 0x5
    u8 region;            // 0x6
    u8 useDefaultName;    // 0x7
    MailMessage greeting; // 0x8
} BattleRoomLeader;       // size: 0x10

// Brief palette flash of a pressed button: a sprite, or a rectangle of a bg layer.
typedef struct BattleRoomResultsFlash {
    u8 isBgRect : 1;    // 0x0
    u8 target : 7;      // sprite index or bg layer
    u8 pressedPltt : 4; // 0x1
    u8 normalPltt : 4;
    u8 state;             // 0x2
    u8 timer;             // 0x3
    u8 x;                 // 0x4
    u8 y;                 // 0x5
    u8 width;             // 0x6
    u8 height;            // 0x7
} BattleRoomResultsFlash; // size: 0x8

typedef struct BattleRoomResults {
    enum HeapID heapID;                                        // 0x000
    u32 subState;                                              // 0x004
    u16 showLeaders;                                           // 0x008
    u8 unk_0A;                                                 // 0x00A
    u8 filler_0B[5];                                           // 0x00B
    BgConfig *bgConfig;                                        // 0x010
    u8 filler_14[4];                                           // 0x014
    Options *options;                                          // 0x018
    FrontierSave *frontierSave;                                // 0x01C
    FrontierData *frontierData;                                // 0x020
    u32 downloadData;                                          // 0x024, BattleRoomDownload (Save_BattleRoomDownload_Get)
    MsgData *msgData;                                          // 0x028
    MessageFormat *msgFormat;                                  // 0x02C
    String *string;                                            // 0x030
    u8 filler_34[0x30];                                        // 0x034
    String *defaultNames[2];                                   // 0x064
    Window windows[BATTLE_ROOM_RESULTS_NUM_LEADER_WINDOWS];    // 0x06C
    BattleRoomLeader leaders[BATTLE_ROOM_RESULTS_NUM_LEADERS]; // 0x29C
    SpriteSystem *spriteSystem;                                // 0x47C
    SpriteManager *spriteManager;                              // 0x480
    ManagedSprite *sprites[4];                                 // 0x484
    BattleRoomResultsFlash flash;                              // 0x494
    int nextState;                                             // 0x49C
    u16 page;                                                  // 0x4A0
    u8 nameWindowSet;                                          // 0x4A2
    u8 infoWindowSet;                                          // 0x4A3
    GridInputHandler *gridInput;                               // 0x4A4
    int pageChange;                                            // 0x4A8
} BattleRoomResults;                                           // size: 0x4AC

// The leader records as downloaded.
typedef struct BattleRoomLeaderRecord {
    u16 name[PLAYER_NAME_LENGTH + 1]; // 0x00
    u8 filler_10[2];                  // 0x10
    u8 country;                       // 0x12
    u8 region;                        // 0x13
    u8 filler_14[4];                  // 0x14
    u8 greeting[8];                   // 0x18
    u8 useDefaultName : 1;            // 0x20
    u8 gender : 1;
    u8 unk_20_2 : 6;
    u8 unk_21;            // 0x21
} BattleRoomLeaderRecord; // size: 0x22

typedef struct BattleRoomDownload BattleRoomDownload;

// Defined in frontier_data.c; not declared in its header.
void *BattleRoomDownload_CopyLeaders(BattleRoomDownload *data, enum HeapID heapID);

typedef struct BattleRoomResultsGraphicsBanks {
    u32 banks[10];
} BattleRoomResultsGraphicsBanks;

extern const TouchscreenHitbox gBattleRoomResultsHitbox[];
extern const TouchscreenHitbox sBattleRoomResultsPageHitboxes[];
extern const GraphicsModes sBattleRoomResultsGraphicsModes;
extern const GridCallbacks sBattleRoomResultsGridCallbacks;
extern const OamCharTransferParam sBattleRoomResultsOamCharTransferParam;
extern const SpriteResourceCountsListUnion sBattleRoomResultsSpriteResourceCounts;
extern const BgTemplate sBattleRoomResultsBgTemplate_Main0;
extern const BgTemplate sBattleRoomResultsBgTemplate_Sub0;
extern const BgTemplate sBattleRoomResultsBgTemplate_Sub1;
extern const BgTemplate sBattleRoomResultsBgTemplate_Main1;
extern const OamManagerParam sBattleRoomResultsOamManagerParam;
extern const BattleRoomResultsGraphicsBanks sBattleRoomResultsGraphicsBanks;
extern const TouchscreenHitbox sBattleRoomResultsGridHitboxes[];
extern const WindowTemplate sBattleRoomResultsWindowTemplates[];
extern const DpadMenuBox sBattleRoomResultsGridDpadBoxes[];
extern const ManagedSpriteTemplate sBattleRoomResultsSpriteTemplates[];
extern const WindowTemplate sBattleRoomResultsLeaderWindowTemplates[];

BOOL BattleRoomResults_SetupScreens(BattleRoomResults *results);
BOOL BattleRoomResults_TeardownScreens(BattleRoomResults *results);
void BattleRoomResults_SetGfxBanks(void);
void BattleRoomResults_VBlankCB(BattleRoomResults *results);
int BattleRoomResults_HandleResultsInput(BattleRoomResults *results);
int BattleRoomResults_HandleLeadersInput(BattleRoomResults *results);
void BattleRoomResults_InitBgs(BattleRoomResults *results);
void BattleRoomResults_FreeBgs(BattleRoomResults *results);
void BattleRoomResults_LoadBgGraphics(BattleRoomResults *results);
void BattleRoomResults_AddWindows(BattleRoomResults *results);
void BattleRoomResults_RemoveWindows(BattleRoomResults *results);
void BattleRoomResults_InitStrings(BattleRoomResults *results);
void BattleRoomResults_FreeStrings(BattleRoomResults *results);
void BattleRoomResults_PrintString(Window *window, String *string, u32 x, int y, int fontId, u32 color, int alignment);
void BattleRoomResults_PrintMessage(BattleRoomResults *results, int windowIndex, int msgId, u32 x, int y, int fontId, u32 color, int alignment);
void BattleRoomResults_PrintFormattedMessage(BattleRoomResults *results, int windowIndex, int msgId, u32 x, int y, int fontId, u32 color, int alignment);
void BattleRoomResults_PrintTitle(BattleRoomResults *results);
void BattleRoomResults_PrintResults(BattleRoomResults *results);
void BattleRoomResults_LoadLeaders(BattleRoomResults *results);
void BattleRoomResults_FreeLeaderNames(BattleRoomResults *results);
void BattleRoomResults_PrintLeaderNames(BattleRoomResults *results);
void BattleRoomResults_PrintPageNumber(BattleRoomResults *results);
void BattleRoomResults_PrintLeadersScreen(BattleRoomResults *results);
void BattleRoomResults_PrintLeaderInfo(BattleRoomResults *results, int leader);
void BattleRoomResults_LeadersDummy(BattleRoomResults *results);
void BattleRoomResults_InitSprites(BattleRoomResults *results);
void BattleRoomResults_FreeSprites(BattleRoomResults *results);
void BattleRoomResults_UpdateSprites(BattleRoomResults *results);
void BattleRoomResults_TransferOam(void);
void BattleRoomResults_SetSpriteAnim(BattleRoomResults *results, int index, int anim);
void BattleRoomResults_SetSpritePosition(BattleRoomResults *results, int index, int x, int y);
BOOL BattleRoomResults_UpdateButtonFlash(BattleRoomResults *results);
void BattleRoomResults_StartSpriteFlash(BattleRoomResults *results, int sprite);
void BattleRoomResults_StartExitButtonFlash(BattleRoomResults *results);
void BattleRoomResults_CreateGridInput(BattleRoomResults *results);
void BattleRoomResults_FreeGridInput(BattleRoomResults *results);
void BattleRoomResults_MoveCursor(BattleRoomResults *results, int input);
void BattleRoomResults_GridNoop(void);
void BattleRoomResults_GridOnKeyMove(BattleRoomResults *results, int newInput, int prevInput);
void BattleRoomResults_GridOnTouch(BattleRoomResults *results, int input);

BOOL BattleRoomResults_Init(OverlayManager *man, int *state) {
    BattleRoomResultsArgs *args = OverlayManager_GetArgs(man);
    BattleRoomResults *results;

    Heap_Create(HEAP_ID_3, HEAP_ID_72, 0x20000);
    results = OverlayManager_CreateAndGetData(man, sizeof(BattleRoomResults), HEAP_ID_72);
    memset(results, 0, sizeof(BattleRoomResults));
    results->showLeaders = args->showLeaders;
    results->unk_0A = args->unk_6;
    results->options = Save_PlayerData_GetOptionsAddr(args->saveData);
    results->frontierSave = Save_Frontier_GetStatic(args->saveData);
    results->frontierData = Save_FrontierData_Get(args->saveData);
    results->downloadData = Save_BattleRoomDownload_Get(args->saveData);
    results->heapID = HEAP_ID_72;
    SetKeyRepeatTimers(4, 8);
    return TRUE;
}

BOOL BattleRoomResults_Exit(OverlayManager *man, int *state) {
    BattleRoomResults *results = OverlayManager_GetData(man);

    OverlayManager_FreeData(man);
    Heap_Destroy(results->heapID);
    return TRUE;
}

BOOL BattleRoomResults_Main(OverlayManager *man, int *state) {
    BattleRoomResults *results = OverlayManager_GetData(man);

    switch (*state) {
    case 0:
        if (BattleRoomResults_SetupScreens(results)) {
            *state = 1;
        }
        break;
    case 1:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, results->heapID);
        *state = 2;
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            if (results->showLeaders == FALSE) {
                *state = 3;
            } else {
                *state = 4;
            }
        }
        break;
    case 3:
        *state = BattleRoomResults_HandleResultsInput(results);
        break;
    case 4:
        *state = BattleRoomResults_HandleLeadersInput(results);
        break;
    case 5:
        if (!BattleRoomResults_UpdateButtonFlash(results)) {
            *state = results->nextState;
        }
        break;
    case 6:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, results->heapID);
        *state = 7;
        break;
    case 7:
        if (IsPaletteFadeFinished()) {
            *state = 8;
        }
        break;
    case 8:
        if (BattleRoomResults_TeardownScreens(results)) {
            return TRUE;
        }
        break;
    }

    if (*state >= 1 && *state <= 7) {
        BattleRoomResults_UpdateSprites(results);
    }
    return FALSE;
}

BOOL BattleRoomResults_SetupScreens(BattleRoomResults *results) {
    switch (results->subState) {
    case 0:
        Main_SetVBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        GX_SetVisiblePlane(GX_PLANEMASK_NONE);
        reg_GXS_DB_DISPCNT = ~REG_GXS_DB_DISPCNT_DISPLAY_MASK & reg_GXS_DB_DISPCNT;
        reg_GX_POWCNT = ~REG_GX_POWCNT_DSEL_MASK & reg_GX_POWCNT;
        BattleRoomResults_SetGfxBanks();
        ResetVisibleHardwareWindows(PM_LCD_TOP);
        ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
        BattleRoomResults_InitBgs(results);
        break;
    case 1:
        BattleRoomResults_LoadBgGraphics(results);
        BattleRoomResults_InitStrings(results);
        break;
    case 2:
        BattleRoomResults_InitSprites(results);
        break;
    case 3:
        BattleRoomResults_AddWindows(results);
        if (results->showLeaders == FALSE) {
            BattleRoomResults_PrintResults(results);
        } else {
            BattleRoomResults_LoadLeaders(results);
            BattleRoomResults_PrintLeadersScreen(results);
            BattleRoomResults_LeadersDummy(results);
            BattleRoomResults_CreateGridInput(results);
        }
        break;
    case 4:
        Main_SetVBlankIntrCB((GFIntrCB)BattleRoomResults_VBlankCB, results);
        results->subState = 0;
        return TRUE;
    }
    results->subState++;
    return FALSE;
}

BOOL BattleRoomResults_TeardownScreens(BattleRoomResults *results) {
    switch (results->subState) {
    case 0:
        if (results->showLeaders != FALSE) {
            BattleRoomResults_FreeGridInput(results);
            BattleRoomResults_FreeLeaderNames(results);
        }
        BattleRoomResults_FreeSprites(results);
        BattleRoomResults_RemoveWindows(results);
        BattleRoomResults_FreeStrings(results);
        BattleRoomResults_FreeBgs(results);
        break;
    case 1:
        Main_SetVBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        GX_SetVisiblePlane(GX_PLANEMASK_NONE);
        GXS_SetVisiblePlane(GX_PLANEMASK_NONE);
        results->subState = 0;
        return TRUE;
    }
    results->subState++;
    return FALSE;
}

void BattleRoomResults_SetGfxBanks(void) {
    BattleRoomResultsGraphicsBanks banks = sBattleRoomResultsGraphicsBanks;
    GfGfx_SetBanks((GraphicsBanks *)&banks);
}

#pragma force_active on

void BattleRoomResults_VBlankCB(BattleRoomResults *results) {
    BattleRoomResults_TransferOam();
    DoScheduledBgGpuUpdates(results->bgConfig);
    ((u32 *)0x027E0000)[(HW_INTR_CHECK_BUF - HW_DTCM) / 4] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag
}

#pragma force_active reset

int BattleRoomResults_HandleResultsInput(BattleRoomResults *results) {
    if (TouchscreenHitbox_TouchNewIsIn(gBattleRoomResultsHitbox) == TRUE || (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B))) {
        PlaySE(SEQ_SE_DP_DECIDE);
        BattleRoomResults_StartExitButtonFlash(results);
        results->nextState = 6;
        return 5;
    }
    return 3;
}

#pragma force_active on

int BattleRoomResults_HandleLeadersInput(BattleRoomResults *results) {
    switch ((u32)GridInputHandler_HandleInput_NoHold(results->gridInput)) {
    case BATTLE_ROOM_RESULTS_CANCEL:
    case GRID_MENU_CANCEL:
        PlaySE(SEQ_SE_DP_DECIDE);
        BattleRoomResults_StartExitButtonFlash(results);
        results->nextState = 6;
        return 5;
    case GRID_MENU_CURSOR_MOVE:
        PlaySE(SEQ_SE_DP_SELECT);
        if (results->pageChange == 1) {
            BattleRoomResults_StartSpriteFlash(results, 2);
            results->nextState = 4;
            return 5;
        } else if (results->pageChange == 2) {
            BattleRoomResults_StartSpriteFlash(results, 3);
            results->nextState = 4;
            return 5;
        }
        break;
    case GRID_MENU_BUTTON_MODE:
    case GRID_MENU_NOTHING_CHOSEN:
        break;
    default:
        if (System_GetTouchNew() == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
        }
        break;
    }
    switch (TouchscreenHitbox_FindRectAtTouchNew(sBattleRoomResultsPageHitboxes)) {
    case 0:
        if (results->page) {
            results->page--;
        } else {
            results->page = BATTLE_ROOM_RESULTS_NUM_PAGES - 1;
        }
        PlaySE(SEQ_SE_DP_SELECT78);
        BattleRoomResults_PrintLeaderNames(results);
        BattleRoomResults_PrintLeaderInfo(results, results->page * BATTLE_ROOM_RESULTS_LEADERS_PER_PAGE + GridInputHandler_GetNextInput(results->gridInput));
        BattleRoomResults_PrintPageNumber(results);
        BattleRoomResults_StartSpriteFlash(results, 2);
        results->nextState = 4;
        return 5;
    case 1:
        if (results->page < BATTLE_ROOM_RESULTS_NUM_PAGES - 1) {
            results->page++;
        } else {
            results->page = 0;
        }
        PlaySE(SEQ_SE_DP_SELECT78);
        BattleRoomResults_PrintLeaderNames(results);
        BattleRoomResults_PrintLeaderInfo(results, results->page * BATTLE_ROOM_RESULTS_LEADERS_PER_PAGE + GridInputHandler_GetNextInput(results->gridInput));
        BattleRoomResults_PrintPageNumber(results);
        BattleRoomResults_StartSpriteFlash(results, 3);
        results->nextState = 4;
        return 5;
    }
    return 4;
}

#pragma force_active reset

void BattleRoomResults_InitBgs(BattleRoomResults *results) {
    GraphicsModes modes;
    BgTemplate bg0;
    BgTemplate bg1;
    BgTemplate bg4;
    BgTemplate bg5;

    results->bgConfig = BgConfig_Alloc(results->heapID);
    modes = sBattleRoomResultsGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    bg0 = sBattleRoomResultsBgTemplate_Main0;
    InitBgFromTemplate(results->bgConfig, GF_BG_LYR_MAIN_0, &bg0, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, results->heapID);
    BgClearTilemapBufferAndCommit(results->bgConfig, GF_BG_LYR_MAIN_0);
    bg1 = sBattleRoomResultsBgTemplate_Main1;
    InitBgFromTemplate(results->bgConfig, GF_BG_LYR_MAIN_1, &bg1, GF_BG_TYPE_TEXT);
    bg4 = sBattleRoomResultsBgTemplate_Sub0;
    InitBgFromTemplate(results->bgConfig, GF_BG_LYR_SUB_0, &bg4, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, results->heapID);
    BgClearTilemapBufferAndCommit(results->bgConfig, GF_BG_LYR_SUB_0);
    bg5 = sBattleRoomResultsBgTemplate_Sub1;
    InitBgFromTemplate(results->bgConfig, GF_BG_LYR_SUB_1, &bg5, GF_BG_TYPE_TEXT);
}

void BattleRoomResults_FreeBgs(BattleRoomResults *results) {
    FreeBgTilemapBuffer(results->bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(results->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(results->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(results->bgConfig, GF_BG_LYR_MAIN_0);
    Heap_Free(results->bgConfig);
}

void BattleRoomResults_LoadBgGraphics(BattleRoomResults *results) {
    NARC *narc;

    if (results->showLeaders == FALSE) {
        narc = NARC_New(NARC_a_1_1_8, results->heapID);
        GfGfxLoader_LoadCharDataFromOpenNarc(narc, 4, results->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, results->heapID);
        GfGfxLoader_GXLoadPalFromOpenNarc(narc, 7, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20, results->heapID);
        GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, results->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, results->heapID);
        NARC_Delete(narc);
        narc = NARC_New(NARC_a_1_8_3, results->heapID);
        GfGfxLoader_LoadCharDataFromOpenNarc(narc, 142, results->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, results->heapID);
        GfGfxLoader_GXLoadPalFromOpenNarc(narc, 196, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20, results->heapID);
        GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 141, results->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, results->heapID);
        NARC_Delete(narc);
    } else {
        narc = NARC_New(NARC_a_1_1_8, results->heapID);
        GfGfxLoader_LoadCharDataFromOpenNarc(narc, 4, results->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, results->heapID);
        GfGfxLoader_GXLoadPalFromOpenNarc(narc, 7, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20, results->heapID);
        GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 5, results->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, results->heapID);
        GfGfxLoader_LoadCharDataFromOpenNarc(narc, 8, results->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, results->heapID);
        GfGfxLoader_GXLoadPalFromOpenNarc(narc, 10, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20, results->heapID);
        GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 9, results->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, results->heapID);
        NARC_Delete(narc);
    }
}

void BattleRoomResults_AddWindows(BattleRoomResults *results) {
    if (results->showLeaders == FALSE) {
        const WindowTemplate *template = sBattleRoomResultsWindowTemplates;
        Window *window;
        int i = 0;
        window = results->windows;
        do {
            AddWindow(results->bgConfig, window, template);
            i++;
            template ++;
            window++;
        } while (i < BATTLE_ROOM_RESULTS_NUM_RESULT_WINDOWS);
    } else {
        const WindowTemplate *template = sBattleRoomResultsLeaderWindowTemplates;
        Window *window;
        int i = 0;
        window = results->windows;
        do {
            AddWindow(results->bgConfig, window, template);
            FillWindowPixelBuffer(window, 0);
            i++;
            template ++;
            window++;
        } while (i < BATTLE_ROOM_RESULTS_NUM_LEADER_WINDOWS);
    }
    FontID_Alloc(4, results->heapID);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, results->heapID);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, results->heapID);
}

void BattleRoomResults_RemoveWindows(BattleRoomResults *results) {
    int i;
    int numWindows;
    Window *window;

    FontID_Release(4);
    if (results->showLeaders == FALSE) {
        numWindows = BATTLE_ROOM_RESULTS_NUM_RESULT_WINDOWS;
    } else {
        numWindows = BATTLE_ROOM_RESULTS_NUM_LEADER_WINDOWS;
    }
    i = 0;
    if (numWindows > 0) {
        window = results->windows;
        do {
            ClearWindowTilemapAndCopyToVram(window);
            RemoveWindow(window);
            i++;
            window++;
        } while (i < numWindows);
    }
}

void BattleRoomResults_InitStrings(BattleRoomResults *results) {
    results->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0028_bin, results->heapID);
    results->msgFormat = MessageFormat_New_Custom(2, 64, results->heapID);
    results->string = String_New(512, results->heapID);
    results->defaultNames[0] = NewString_ReadMsgData(results->msgData, msg_0028_00033);
    results->defaultNames[1] = NewString_ReadMsgData(results->msgData, msg_0028_00034);
}

void BattleRoomResults_FreeStrings(BattleRoomResults *results) {
    String_Delete(results->defaultNames[1]);
    String_Delete(results->defaultNames[0]);
    String_Delete(results->string);
    MessageFormat_Delete(results->msgFormat);
    DestroyMsgData(results->msgData);
}

void BattleRoomResults_PrintString(Window *window, String *string, u32 x, int y, int fontId, u32 color, int alignment) {
    if (alignment == 1) {
        x -= FontID_String_GetWidth(fontId, string, 0);
    } else if (alignment == 2) {
        x -= FontID_String_GetWidth(fontId, string, 0) >> 1;
    }
    // The font id is passed without truncation to u8: the callee takes it as u32 (see the FontID TODO).
    ((u8(*)(Window *, u32, String *, u32, u32, u32, u32, PrinterCallback_t))AddTextPrinterParameterizedWithColor)(window, fontId, string, x, y, TEXT_SPEED_NOTRANSFER, color, NULL);
}

void BattleRoomResults_PrintMessage(BattleRoomResults *results, int windowIndex, int msgId, u32 x, int y, int fontId, u32 color, int alignment) {
    String *string = NewString_ReadMsgData(results->msgData, msgId);

    BattleRoomResults_PrintString(&results->windows[windowIndex], string, x, y, fontId, color, alignment);
    String_Delete(string);
}

void BattleRoomResults_PrintFormattedMessage(BattleRoomResults *results, int windowIndex, int msgId, u32 x, int y, int fontId, u32 color, int alignment) {
    String *string = NewString_ReadMsgData(results->msgData, msgId);

    StringExpandPlaceholders(results->msgFormat, results->string, string);
    BattleRoomResults_PrintString(&results->windows[windowIndex], results->string, x, y, fontId, color, alignment);
    String_Delete(string);
}

void BattleRoomResults_PrintTitle(BattleRoomResults *results) {
    int width = GetWindowWidth(&results->windows[0]);
    BattleRoomResults_PrintMessage(results, 0, msg_0028_00024, (width * 8) / 2, 0, 4, MAKE_TEXT_COLOR(15, 1, 0), 2);
}

#pragma force_active on

void BattleRoomResults_PrintResults(BattleRoomResults *results) {
    u32 i;
    u32 j;
    u8 width;

    for (i = 0; i < BATTLE_ROOM_RESULTS_NUM_RESULT_WINDOWS; i++) {
        FillWindowPixelBuffer(&results->windows[i], 0);
    }
    BattleRoomResults_PrintTitle(results);
    width = GetWindowWidth(&results->windows[1]);
    BattleRoomResults_PrintMessage(results, 1, msg_0028_00002, (width * 8) / 2, 0, 0, MAKE_TEXT_COLOR(15, 2, 0), 2);
    BattleRoomResults_PrintMessage(results, 2, msg_0028_00005, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), 0);
    BattleRoomResults_PrintMessage(results, 3, msg_0028_00008, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), 0);
    BattleRoomResults_PrintMessage(results, 4, msg_0028_00012, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), 0);
    if (FrontierData_FlagAction(results->frontierData, 12, 0) == 0) {
        BattleRoomResults_PrintMessage(results, 6, msg_0028_00009, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), 0);
    } else {
        BattleRoomResults_PrintMessage(results, 6, msg_0028_00010, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), 0);
    }
    BattleRoomResults_PrintMessage(results, 8, msg_0028_00011, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), 0);
    BufferIntegerAsString(results->msgFormat, 0, FrontierData_WifiRankAction(results->frontierData, 0), 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
    BattleRoomResults_PrintFormattedMessage(results, 5, msg_0028_00014, GetWindowWidth(&results->windows[5]) * 8, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), 1);
    BufferIntegerAsString(results->msgFormat, 0, FrontierSave_GetStat(results->frontierSave, 9, 255), 4, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    width = GetWindowWidth(&results->windows[7]);
    BattleRoomResults_PrintFormattedMessage(results, 7, msg_0028_00015, (width * 8) / 2, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), 2);
    BufferIntegerAsString(results->msgFormat, 0, FrontierSave_GetStat(results->frontierSave, 8, 255), 4, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    width = GetWindowWidth(&results->windows[9]);
    BattleRoomResults_PrintFormattedMessage(results, 9, msg_0028_00015, (width * 8) / 2, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), 2);
    for (j = 0; j < BATTLE_ROOM_RESULTS_NUM_RESULT_WINDOWS; j++) {
        ScheduleWindowCopyToVram(&results->windows[j]);
    }
}

void BattleRoomResults_LoadLeaders(BattleRoomResults *results) {
    struct {
        int index;
        BattleRoomLeaderRecord *records;
    } query; // kept in one struct: matches the original stack layout

    BattleRoomLeader *leader;
    BattleRoomLeaderRecord *record;
    u8 *nameRecord;

    query.records = BattleRoomDownload_CopyLeaders((BattleRoomDownload *)results->downloadData, results->heapID);
    query.index = 0;
    leader = results->leaders;
    record = query.records;
    nameRecord = (u8 *)query.records;
    for (; query.index < BATTLE_ROOM_RESULTS_NUM_LEADERS; leader++, record++, nameRecord += sizeof(BattleRoomLeaderRecord), query.index++) {
        leader->gender = record->gender;
        leader->country = record->country;
        leader->region = record->region;
        leader->useDefaultName = record->useDefaultName;
        MI_CpuCopy8(record->greeting, &leader->greeting, sizeof(MailMessage));
        leader->name = String_New(PLAYER_NAME_LENGTH + 1, results->heapID);
        if (leader->useDefaultName) {
            String_Copy(leader->name, results->defaultNames[leader->gender]);
        } else {
            CopyU16ArrayToString(leader->name, (const u16 *)nameRecord);
        }
    }
    Heap_Free(query.records);
}

#pragma force_active reset

void BattleRoomResults_FreeLeaderNames(BattleRoomResults *results) {
    int i;
    BattleRoomResults *shifted = results; // shifted by i leaders: leaders[0] through it is leaders[i]

    for (i = 0; i < BATTLE_ROOM_RESULTS_NUM_LEADERS; i++, shifted = (BattleRoomResults *)((u8 *)shifted + sizeof(BattleRoomLeader))) {
        String_Delete(shifted->leaders[0].name);
    }
}

void BattleRoomResults_PrintLeaderNames(BattleRoomResults *results) {
    u16 leader;
    Window *windows;
    u16 i;

    if (results->nameWindowSet == 0) {
        windows = &results->windows[11];
    } else {
        windows = &results->windows[23];
    }
    leader = results->page * BATTLE_ROOM_RESULTS_LEADERS_PER_PAGE;
    for (i = 0; i < BATTLE_ROOM_RESULTS_LEADERS_PER_PAGE; leader++, i++) {
        FillWindowPixelBuffer(&windows[i], 0);
        if (leader < BATTLE_ROOM_RESULTS_NUM_LEADERS) {
            u32 color;
            if (results->leaders[leader].gender == 0) {
                color = MAKE_TEXT_COLOR(7, 8, 0);
            } else {
                color = MAKE_TEXT_COLOR(3, 4, 0);
            }
            BattleRoomResults_PrintString(&windows[i], results->leaders[leader].name, 0, 0, 0, color, 0);
        }
        ScheduleWindowCopyToVram(&windows[i]);
    }
    results->nameWindowSet ^= 1;
}

void BattleRoomResults_PrintPageNumber(BattleRoomResults *results) {
    FillWindowPixelBuffer(&results->windows[4], 0);
    BufferIntegerAsString(results->msgFormat, 0, results->page + 1, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
    BattleRoomResults_PrintFormattedMessage(results, 4, msg_0028_00020, 8, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), 1);
    BattleRoomResults_PrintMessage(results, 4, msg_0028_00021, 16, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), 2);
    BufferIntegerAsString(results->msgFormat, 0, BATTLE_ROOM_RESULTS_NUM_PAGES, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
    BattleRoomResults_PrintFormattedMessage(results, 4, msg_0028_00020, 24, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ScheduleWindowCopyToVram(&results->windows[4]);
}

void BattleRoomResults_PrintLeadersScreen(BattleRoomResults *results) {
    u32 i;
    Window *window;
    Window *window2;
    u32 j;
    u8 values[4];

    for (i = 0, window = results->windows; i <= 3; i++, window++) {
        FillWindowPixelBuffer(window, 0);
    }
    BattleRoomResults_PrintTitle(results);
    BattleRoomDownload_GetLeadersRoom(results->downloadData, values);
    BufferIntegerAsString(results->msgFormat, 0, values[0], 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
    BattleRoomResults_PrintFormattedMessage(results, 1, msg_0028_00017, 0, 0, 0, MAKE_TEXT_COLOR(15, 2, 0), 0);
    BufferIntegerAsString(results->msgFormat, 1, values[1], 3, PRINTING_MODE_LEADING_ZEROS, TRUE);
    BattleRoomResults_PrintFormattedMessage(results, 2, msg_0028_00018, 60, 0, 0, MAKE_TEXT_COLOR(15, 2, 0), 2);
    BattleRoomResults_PrintMessage(results, 3, msg_0028_00019, 0, 0, 0, MAKE_TEXT_COLOR(15, 2, 0), 0);
    for (j = 0, window2 = results->windows; j <= 3; j++, window2++) {
        ScheduleWindowCopyToVram(window2);
    }
    BattleRoomResults_PrintLeaderNames(results);
    BattleRoomResults_PrintLeaderInfo(results, 0);
    BattleRoomResults_PrintPageNumber(results);
}

// The leader's fields are addressed as byte offsets from the work struct; the region byte in particular
// must be read as *(u8 *)(base + (field offset + offset)), any other spelling swaps two registers.
void BattleRoomResults_PrintLeaderInfo(BattleRoomResults *results, int leader) {
    u32 windowIndex;
    u8 *windows;
    u16 i;

    if (results->infoWindowSet == 0) {
        windowIndex = 5;
    } else {
        windowIndex = 8;
    }
    windows = (u8 *)results->windows;
    windows = windows + windowIndex * sizeof(Window);
    for (i = 0; i < 3; i++) {
        FillWindowPixelBuffer((Window *)(windows + i * sizeof(Window)), 0);
    }
    if ((u32)leader < BATTLE_ROOM_RESULTS_NUM_LEADERS) {
        u32 offset = leader * sizeof(BattleRoomLeader);
        u8 *genders = (u8 *)results + offsetof(BattleRoomResults, leaders[0].gender);
        u8 *countries;
        u8 *greetings;
        String *string;

        BufferString(results->msgFormat, 0, *(String **)((u8 *)results + (offsetof(BattleRoomResults, leaders[0].name) + offset)), genders[offset], 1, 2);
        if (genders[offset] == 0) {
            BattleRoomResults_PrintFormattedMessage(results, windowIndex, msg_0028_00022, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), 0);
        } else {
            BattleRoomResults_PrintFormattedMessage(results, windowIndex, msg_0028_00023, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), 0);
        }
        BattleRoomResults_PrintMessage(results, windowIndex + 1, msg_0028_00028, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), 0);
        countries = (u8 *)results + offsetof(BattleRoomResults, leaders[0].country);
        if (countries[offset] == 0) {
            BattleRoomResults_PrintMessage(results, windowIndex + 1, msg_0028_00031, 0, 16, 0, MAKE_TEXT_COLOR(1, 2, 0), 0);
        } else {
            BufferCountryName(results->msgFormat, 0, countries[offset]);
            BattleRoomResults_PrintFormattedMessage(results, windowIndex + 1, msg_0028_00029, 0, 16, 0, MAKE_TEXT_COLOR(1, 2, 0), 0);
        }
        if (*(u8 *)((u8 *)results + (offsetof(BattleRoomResults, leaders[0].region) + offset)) != 0) {
            BufferCityName(results->msgFormat, 0, countries[offset], *(u8 *)((u8 *)results + (offsetof(BattleRoomResults, leaders[0].region) + offset)));
            BattleRoomResults_PrintFormattedMessage(results, windowIndex + 1, msg_0028_00030, 0, 32, 0, MAKE_TEXT_COLOR(1, 2, 0), 0);
        }
        BattleRoomResults_PrintMessage(results, windowIndex + 2, msg_0028_00032, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), 0);
        greetings = (u8 *)results + offsetof(BattleRoomResults, leaders[0].greeting);
        string = MailMsg_GetExpandedString((MailMessage *)(greetings + offset), results->heapID);
        BattleRoomResults_PrintString((Window *)(windows + 2 * sizeof(Window)), string, 0, 16, 0, MAKE_TEXT_COLOR(1, 2, 0), 0);
        String_Delete(string);
    }
    for (i = 0; i < 3; i++) {
        ScheduleWindowCopyToVram((Window *)(windows + i * sizeof(Window)));
    }
    results->infoWindowSet ^= 1;
}

void BattleRoomResults_LeadersDummy(BattleRoomResults *results) {
}

#pragma force_active on

void BattleRoomResults_InitSprites(BattleRoomResults *results) {
    OamManagerParam oamParam;
    OamCharTransferParam charTransferParam;
    SpriteResourceCountsListUnion resourceCounts;
    NARC *narc;
    u32 i;

    results->spriteSystem = SpriteSystem_Alloc(results->heapID);
    results->spriteManager = SpriteManager_New(results->spriteSystem);
    oamParam = sBattleRoomResultsOamManagerParam;
    charTransferParam = sBattleRoomResultsOamCharTransferParam;
    SpriteSystem_Init(results->spriteSystem, &oamParam, &charTransferParam, 32);
    SpriteSystem_InitSprites(results->spriteSystem, results->spriteManager, 4);
    resourceCounts = sBattleRoomResultsSpriteResourceCounts;
    SpriteSystem_InitManagerWithCapacities(results->spriteSystem, results->spriteManager, &resourceCounts);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(results->spriteSystem), 0, FX32_CONST(512));
    narc = NARC_New(NARC_a_1_1_8, results->heapID);
    SpriteSystem_LoadCharResObjFromOpenNarc(results->spriteSystem, results->spriteManager, narc, 0, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 0xD158);
    SpriteSystem_LoadPlttResObjFromOpenNarc(results->spriteSystem, results->spriteManager, narc, 3, FALSE, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0xD158);
    SpriteSystem_LoadCellResObjFromOpenNarc(results->spriteSystem, results->spriteManager, narc, 1, TRUE, 0xD158);
    SpriteSystem_LoadAnimResObjFromOpenNarc(results->spriteSystem, results->spriteManager, narc, 2, TRUE, 0xD158);
    NARC_Delete(narc);
    for (i = 0; i < 4; i++) {
        results->sprites[i] = SpriteSystem_NewSprite(results->spriteSystem, results->spriteManager, &sBattleRoomResultsSpriteTemplates[i]);
    }
    if (results->showLeaders == FALSE) {
        ManagedSprite_SetDrawFlag(results->sprites[2], FALSE);
        ManagedSprite_SetDrawFlag(results->sprites[3], FALSE);
    }
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

#pragma force_active reset

void BattleRoomResults_FreeSprites(BattleRoomResults *results) {
    u32 i;
    BattleRoomResults *shifted = results; // shifted by i sprites: sprites[0] through it is sprites[i]

    for (i = 0; i < 4; i++, shifted = (BattleRoomResults *)((u8 *)shifted + sizeof(ManagedSprite *))) {
        ManagedSprite *sprite = shifted->sprites[0];
        if (sprite) {
            Sprite_DeleteAndFreeResources(sprite);
            shifted->sprites[0] = NULL;
        }
    }
    SpriteSystem_FreeResourcesAndManager(results->spriteSystem, results->spriteManager);
    SpriteSystem_Free(results->spriteSystem);
}

void BattleRoomResults_UpdateSprites(BattleRoomResults *results) {
    u32 i = 0;
    BattleRoomResults *shifted = results; // shifted by i sprites: sprites[0] through it is sprites[i]

    for (; i < 4; i++, shifted = (BattleRoomResults *)((u8 *)shifted + sizeof(ManagedSprite *))) {
        ManagedSprite_TickFrame(shifted->sprites[0]);
    }
    SpriteSystem_DrawSprites(results->spriteManager);
}

void BattleRoomResults_TransferOam(void) {
    SpriteSystem_TransferOam();
}

void BattleRoomResults_SetSpriteAnim(BattleRoomResults *results, int index, int anim) {
    ManagedSprite_SetAnimationFrame(results->sprites[index], 0);
    ManagedSprite_SetAnim(results->sprites[index], anim);
}

void BattleRoomResults_SetSpritePosition(BattleRoomResults *results, int index, int x, int y) {
    // x and y are passed as ints: the callee does not need them truncated (s16 in sprite_system.h).
    ((void (*)(ManagedSprite *, int, int))ManagedSprite_SetPositionXY)(results->sprites[index], x, y);
}

#pragma force_active on

BOOL BattleRoomResults_UpdateButtonFlash(BattleRoomResults *results) {
    switch (results->flash.state) {
    case 0:
        if (results->flash.isBgRect == FALSE) {
            ManagedSprite_SetPaletteOverride(results->sprites[results->flash.target], results->flash.pressedPltt);
        } else {
            BgTilemapRectChangePalette(results->bgConfig, results->flash.target, results->flash.x, results->flash.y, results->flash.width, results->flash.height, results->flash.pressedPltt);
            ScheduleBgTilemapBufferTransfer(results->bgConfig, results->flash.target);
        }
        results->flash.state++;
        break;
    case 1:
        results->flash.timer++;
        if (results->flash.timer == 4) {
            if (results->flash.isBgRect == FALSE) {
                ManagedSprite_SetPaletteOverride(results->sprites[results->flash.target], results->flash.normalPltt);
            } else {
                BgTilemapRectChangePalette(results->bgConfig, results->flash.target, results->flash.x, results->flash.y, results->flash.width, results->flash.height, results->flash.normalPltt);
                ScheduleBgTilemapBufferTransfer(results->bgConfig, results->flash.target);
            }
            results->flash.timer = 0;
            results->flash.state++;
        }
        break;
    case 2:
        results->flash.timer++;
        if (results->flash.timer == 2) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

void BattleRoomResults_StartSpriteFlash(BattleRoomResults *results, int sprite) {
    results->flash.isBgRect = FALSE;
    results->flash.target = (u8)sprite;
    results->flash.pressedPltt = 1;
    results->flash.normalPltt = 0;
    results->flash.state = 0;
    results->flash.timer = 0;
}

#pragma force_active reset

void BattleRoomResults_StartExitButtonFlash(BattleRoomResults *results) {
    BattleRoomResults_StartSpriteFlash(results, 1);
}

void BattleRoomResults_CreateGridInput(BattleRoomResults *results) {
    results->gridInput = GridInputHandler_Create(sBattleRoomResultsGridHitboxes, sBattleRoomResultsGridDpadBoxes, &sBattleRoomResultsGridCallbacks, results, TRUE, 0, results->heapID);
    BattleRoomResults_MoveCursor(results, 0);
}

void BattleRoomResults_FreeGridInput(BattleRoomResults *results) {
    GridInputHandler_Free(results->gridInput);
}

void BattleRoomResults_MoveCursor(BattleRoomResults *results, int input) {
    u8 position[2];
    const DpadMenuBox *box = GridInputHandler_GetDpadBox(results->gridInput, input);

    DpadMenuBox_GetPosition(box, &position[1], &position[0]);
    BattleRoomResults_SetSpritePosition(results, 0, position[1], position[0]);
    if (input == BATTLE_ROOM_RESULTS_CANCEL) {
        BattleRoomResults_SetSpriteAnim(results, 0, 2);
    } else {
        BattleRoomResults_SetSpriteAnim(results, 0, 7);
    }
}

void BattleRoomResults_GridNoop(void) {
}

#pragma force_active on

// Grid key move: moving off the left/right edge of the 3x4 name grid turns the page.
void BattleRoomResults_GridOnKeyMove(BattleRoomResults *results, int newInput, int prevInput) {
    results->pageChange = 0;
    if ((newInput == 2 && prevInput == 0) || (newInput == 5 && prevInput == 3) || (newInput == 8 && prevInput == 6) || (newInput == 11 && prevInput == 9)) {
        if (results->page != 0) {
            results->page--;
        } else {
            results->page = BATTLE_ROOM_RESULTS_NUM_PAGES - 1;
        }
        BattleRoomResults_PrintLeaderNames(results);
        BattleRoomResults_PrintPageNumber(results);
        results->pageChange = 1;
    }
    if ((newInput == 0 && prevInput == 2) || (newInput == 3 && prevInput == 5) || (newInput == 6 && prevInput == 8) || (newInput == 9 && prevInput == 11)) {
        if (results->page < BATTLE_ROOM_RESULTS_NUM_PAGES - 1) {
            results->page++;
        } else {
            results->page = 0;
        }
        BattleRoomResults_PrintLeaderNames(results);
        BattleRoomResults_PrintPageNumber(results);
        results->pageChange = 2;
    }
    BattleRoomResults_PrintLeaderInfo(results, newInput == BATTLE_ROOM_RESULTS_CANCEL ? BATTLE_ROOM_RESULTS_NUM_LEADERS : newInput + BATTLE_ROOM_RESULTS_LEADERS_PER_PAGE * results->page);
    BattleRoomResults_MoveCursor(results, newInput);
}

#pragma force_active reset

void BattleRoomResults_GridOnTouch(BattleRoomResults *results, int input) {
    int leader;

    if (input == BATTLE_ROOM_RESULTS_CANCEL) {
        leader = BATTLE_ROOM_RESULTS_NUM_LEADERS;
    } else {
        leader = input + BATTLE_ROOM_RESULTS_LEADERS_PER_PAGE * results->page;
    }
    BattleRoomResults_PrintLeaderInfo(results, leader);
    BattleRoomResults_MoveCursor(results, input);
}
