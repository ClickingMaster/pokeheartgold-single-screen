#include "global.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0775.h"
#include "msgdata/msg/msg_0778.h"
#include "msgdata/msg/msg_0800.h"

#include "assert.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "global_terminal_net.h"
#include "heap.h"
#include "launch_application.h"
#include "message_format.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "options.h"
#include "overlay_00_thumb.h"
#include "overlay_manager.h"
#include "palette.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_text.h"
#include "render_window.h"
#include "sav_system_info.h"
#include "screen_fade.h"
#include "string_util.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "friend_roster.h"
#include "comm_state.h"
#include "wireless_status_icon.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

// Global Terminal: connects to (and disconnects from) Nintendo Wi-Fi Connection around the VS Recorder

enum GlobalTerminalConnectState {
    GTC_STATE_ASK_CONNECT,
    GTC_STATE_START_CONNECT,
    GTC_STATE_WAIT_CONNECT,
    GTC_STATE_3,
    GTC_STATE_4,
    GTC_STATE_SAVE_PROFILE_ID,
    GTC_STATE_SHOW_ERROR,
    GTC_STATE_WAIT_ERROR_ACK,
    GTC_STATE_EXIT,
    GTC_STATE_EXIT_CONNECTED,
    GTC_STATE_SHOW_SERVER_ERROR,
    GTC_STATE_SERVER_ERROR_DISCONNECT,
    GTC_STATE_WAIT_MESSAGE,
    GTC_STATE_DISCONNECT,
};

// view of GlobalTerminalApp (include/overlay_app_manager_internal.h defines a file-scope static, so it is not included)
typedef struct GlobalTerminalConnectArgs {
    GlobalTerminalLaunchArgs *launchArgs; // 0x00
    BOOL disconnect;                // 0x04, second run after the VS Recorder: only disconnect
    u8 unk_08[0x14 - 0x08];         // 0x08
    u8 wifiConnection[0x7C - 0x14]; // 0x14
    BOOL isConnected;               // 0x7C
} GlobalTerminalConnectArgs;

typedef struct GlobalTerminalConnectBgTemplates {
    BgTemplate templates[2];
} GlobalTerminalConnectBgTemplates;

// SysTask data: the BG palette cycles through 21 palettes blended between the 4 base palettes
typedef struct GlobalTerminalConnectPaletteCycle {
    SysTask *task;           // 0x000
    u32 enabled;             // 0x004
    u16 basePalettes[4][16]; // 0x008
    u16 palettes[21][16];    // 0x088
    s16 index;               // 0x328
    s8 direction;            // 0x32A
    u8 frame;                // 0x32B
    u32 screens;             // 0x32C, 0 = both, 1 = main, 2 = sub
} GlobalTerminalConnectPaletteCycle;

typedef struct GlobalTerminalConnectWork {
    GlobalTerminalConnectArgs *args;                // 0x00
    BgConfig *bgConfig;                             // 0x04
    u32 state;                                      // 0x08
    u32 nextState;                                  // 0x0C
    s32 serverError;                                // 0x10
    s32 dwcResult;                                  // 0x14
    s32 errorCode;                                  // 0x18
    s32 errorType;                                  // 0x1C
    MessageFormat *msgFormat;                       // 0x20
    MsgData *msgDataGts;                            // 0x24
    MsgData *msgDataConnecting;                     // 0x28
    MsgData *msgDataWifi;                           // 0x2C
    u8 unk_30[4];                                   // 0x30
    String *string;                                 // 0x34
    String *title;                                  // 0x38
    String *errorString;                            // 0x3C
    u32 printerId;                                  // 0x40
    Window messageWindow;                           // 0x44
    Window titleWindow;                             // 0x54
    Window errorWindow;                             // 0x64
    u8 unk_74[0x84 - 0x74];                         // 0x74
    YesNoPrompt *yesNoPrompt;                       // 0x84
    WaitingIcon *waitingIcon;                       // 0x88
    u32 unk_8C;                                     // 0x8C
    u32 textSpeed;                                  // 0x90
    u32 substate;                                   // 0x94
    s32 timer;                                      // 0x98
    u32 unk_9C;                                     // 0x9C
    u8 unk_A0[8];                                   // 0xA0
    GlobalTerminalConnectPaletteCycle paletteCycle; // 0xA8
} GlobalTerminalConnectWork;                        // size 0x3D8

// no public header
void GF_TouchpadResetSampling(void);
void GF_TouchpadStartSampling(int frequency);
void GF_TouchpadStopSampling(void);
void DWC_GetLastError(void *out);
int DWC_GetLastErrorEx(int *errorCode, u32 *errorType);
void DWC_ClearError(void);
void DWC_InitInetEx(void *a0, u32 a1, u32 a2, u32 a3);
void DWC_ConnectInetAsync(void);
int DWC_CheckInet(void);
void DWC_ProcessInet(void);
int DWC_GetInetStatus(void);
void DWC_CleanupInet(void);
void DWC_GetApInfo(void *out);
void DWC_NASLoginAsync(GlobalTerminalConnectWork *work);
u32 DWC_NASLoginProcess(void);
void DWC_ShutdownFriendsMatch(void);
void DWC_ShutdownGHTTP(void);
void CommState_InitWfcSession(SaveData *saveData);
void CommState_FreeWfcSession(void);
void CommState_ShowRestartErrorAndHalt(void);
u32 FashionEntry_sizeof(void);

extern const GraphicsModes sGlobalTerminalConnectGraphicsModes;
extern const GraphicsBanks sGlobalTerminalConnectGraphicsBanks;
extern const GlobalTerminalConnectBgTemplates sGlobalTerminalConnectMainBgTemplates;
extern const GlobalTerminalConnectBgTemplates sGlobalTerminalConnectSubBgTemplates;
extern BOOL (*sGlobalTerminalConnectStates[])(GlobalTerminalConnectWork *work);

BOOL GlobalTerminalConnect_Init(OverlayManager *manager, int *state);
BOOL GlobalTerminalConnect_Main(OverlayManager *manager, int *state);
BOOL GlobalTerminalConnect_Exit(OverlayManager *manager);
void GlobalTerminalConnect_VBlankCB(GlobalTerminalConnectWork *work);
void GlobalTerminalConnect_InitBgs(BgConfig *bgConfig);
void GlobalTerminalConnect_FreeBgs(BgConfig *bgConfig);
void GlobalTerminalConnect_LoadGraphics(GlobalTerminalConnectWork *work);
void GlobalTerminalConnect_PaletteCycleTask(SysTask *task, GlobalTerminalConnectPaletteCycle *cycle);
void GlobalTerminalConnect_CreateWindows(GlobalTerminalConnectWork *work);
void GlobalTerminalConnect_RemoveWindows(GlobalTerminalConnectWork *work);
int GlobalTerminalConnect_GetAlignedX(Window *window, String *string, int x, int alignment, int unused, int fontId);
void GlobalTerminalConnect_PrintAligned(Window *window, String *string, int x, int y, int alignment, u32 color);
void GlobalTerminalConnect_ShowErrorWindow(GlobalTerminalConnectWork *work, int msgId);
void GlobalTerminalConnect_ShowErrorCode(GlobalTerminalConnectWork *work, int msgId, int errorNumber);
BOOL GlobalTerminalConnect_IsPrinting(u32 printerId);
void GlobalTerminalConnect_ShowWaitingIcon(GlobalTerminalConnectWork *work);
void GlobalTerminalConnect_HideWaitingIcon(GlobalTerminalConnectWork *work);
YesNoPrompt *GlobalTerminalConnect_CreateYesNoPrompt(BgConfig *bgConfig, int tileStart);
void GlobalTerminalConnect_PrintMessage(GlobalTerminalConnectWork *work, MsgData *msgData, int msgId, int textSpeed, u32 color);
void GlobalTerminalConnect_SetStateAndNext(GlobalTerminalConnectWork *work, u32 state, u32 nextState);
BOOL GlobalTerminalConnect_ErrorAllowsRetry(int errorCode, int errorType);
BOOL GlobalTerminalConnect_StateAskConnect(GlobalTerminalConnectWork *work);
BOOL GlobalTerminalConnect_StateStartConnect(GlobalTerminalConnectWork *work);
BOOL GlobalTerminalConnect_StateWaitConnect(GlobalTerminalConnectWork *work);
BOOL GlobalTerminalConnect_StateStartAuth(GlobalTerminalConnectWork *work);
BOOL GlobalTerminalConnect_StateWaitAuth(GlobalTerminalConnectWork *work);
BOOL GlobalTerminalConnect_StateSaveProfileId(GlobalTerminalConnectWork *work);
BOOL GlobalTerminalConnect_StateShowError(GlobalTerminalConnectWork *work);
BOOL GlobalTerminalConnect_StateWaitErrorAck(GlobalTerminalConnectWork *work);
BOOL GlobalTerminalConnect_StateExit(GlobalTerminalConnectWork *work);
BOOL GlobalTerminalConnect_StateShowServerError(GlobalTerminalConnectWork *work);
BOOL GlobalTerminalConnect_StateServerErrorDisconnect(GlobalTerminalConnectWork *work);
BOOL GlobalTerminalConnect_StateWaitMessage(GlobalTerminalConnectWork *work);
BOOL GlobalTerminalConnect_StateDisconnect(GlobalTerminalConnectWork *work);

#pragma force_active on

BOOL GlobalTerminalConnect_Init(OverlayManager *manager, int *state) {
    GlobalTerminalConnectWork *work;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(GX_PLANEMASK_NONE);
    GXS_SetVisiblePlane(GX_PLANEMASK_NONE);
    GX_SetVisibleWnd(0);
    GXS_SetVisibleWnd(0);
    G2_BlendNone();
    G2S_BlendNone();
    Heap_Create(HEAP_ID_3, HEAP_ID_124, 0x50000);
    work = OverlayManager_CreateAndGetData(manager, sizeof(GlobalTerminalConnectWork), HEAP_ID_124);
    MI_CpuFill8(work, 0, sizeof(GlobalTerminalConnectWork));
    work->args = OverlayManager_GetArgs(manager);
    work->bgConfig = BgConfig_Alloc(HEAP_ID_124);
    GF_CreateVramTransferManager(64, HEAP_ID_124);
    SetKeyRepeatTimers(4, 8);
    GlobalTerminalConnect_InitBgs(work->bgConfig);
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    {
        MessageFormat *msgFormat = MessageFormat_New_Custom(11, 64, HEAP_ID_124);
        work->msgFormat = msgFormat;
    }
    work->msgDataGts = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0775_bin, HEAP_ID_124);
    work->msgDataConnecting = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0778_bin, HEAP_ID_124);
    work->msgDataWifi = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0800_bin, HEAP_ID_124);
    work->string = String_New(180, HEAP_ID_124);
    work->errorString = String_New(256, HEAP_ID_124);
    work->title = NewString_ReadMsgData(work->msgDataGts, msg_0775_00031);
    GlobalTerminalConnect_LoadGraphics(work);
    GlobalTerminalConnect_CreateWindows(work);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_124);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    gSystem.screensFlipped = 1;
    GfGfx_SwapDisplay();
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    Main_SetVBlankIntrCB((GFIntrCB)GlobalTerminalConnect_VBlankCB, work);
    if (work->args->disconnect) {
        WirelessStatusIcon_Create();
        GlobalTerminalConnect_SetStateAndNext(work, GTC_STATE_DISCONNECT, GTC_STATE_EXIT);
    } else {
        if (work->args->launchArgs->connectDirectly != 0) {
            work->state = GTC_STATE_START_CONNECT;
        } else {
            work->state = GTC_STATE_ASK_CONNECT;
        }
    }
    work->textSpeed = Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(work->args->launchArgs->saveData));
    work->args->isConnected = FALSE;
    return TRUE;
}

#pragma force_active reset

BOOL GlobalTerminalConnect_Main(OverlayManager *manager, int *state) {
    GlobalTerminalConnectWork *work = OverlayManager_GetData(manager);
    BOOL result;
    u32 oldState;

    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished() == TRUE) {
            *state = 1;
        }
        break;
    case 1:
        oldState = work->state;
        result = sGlobalTerminalConnectStates[oldState](work);
        if (oldState != work->state) {
            work->substate = 0;
            work->unk_9C = 0;
            work->timer = 0;
        }
        if (result == TRUE) {
            *state = 2;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL GlobalTerminalConnect_Exit(OverlayManager *manager) {
    GlobalTerminalConnectWork *work = OverlayManager_GetData(manager);

    if (work->paletteCycle.task != NULL) {
        SysTask_Destroy(work->paletteCycle.task);
        work->paletteCycle.task = NULL;
        work->paletteCycle.enabled = 0;
    }
    DestroyMsgData(work->msgDataWifi);
    DestroyMsgData(work->msgDataConnecting);
    DestroyMsgData(work->msgDataGts);
    MessageFormat_Delete(work->msgFormat);
    String_Delete(work->title);
    String_Delete(work->errorString);
    String_Delete(work->string);
    GlobalTerminalConnect_RemoveWindows(work);
    Heap_Free(work->bgConfig);
    GlobalTerminalConnect_FreeBgs(work->bgConfig);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GF_DestroyVramTransferManager();
    GF_TouchpadStopSampling();
    TextFlags_SetCanABSpeedUpPrint(FALSE);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    WirelessStatusIcon_Destroy();
    OverlayManager_FreeData(manager);
    Heap_Destroy(HEAP_ID_124);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    return TRUE;
}

void GlobalTerminalConnect_VBlankCB(GlobalTerminalConnectWork *work) {
    vu32 *dtcm = (vu32 *)0x027E0000; // OS_SetIrqCheckFlag, with the DTCM address as a constant

    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    DoScheduledBgGpuUpdates(work->bgConfig);
    dtcm[0x3FF8 / sizeof(u32)] |= OS_IE_V_BLANK;
}

void GlobalTerminalConnect_InitBgs(BgConfig *bgConfig) {
    GraphicsBanks banks;
    GraphicsModes modes;
    GlobalTerminalConnectBgTemplates mainTemplates;
    GlobalTerminalConnectBgTemplates subTemplates;

    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    banks = sGlobalTerminalConnectGraphicsBanks;
    GfGfx_SetBanks(&banks);
    MI_CpuClear32((void *)HW_BG_VRAM, HW_BG_VRAM_SIZE);
    MI_CpuClear32((void *)HW_DB_BG_VRAM, HW_DB_BG_VRAM_SIZE);
    MI_CpuClear32((void *)HW_OBJ_VRAM, HW_OBJ_VRAM_SIZE);
    MI_CpuClear32((void *)HW_DB_OBJ_VRAM, HW_DB_OBJ_VRAM_SIZE);
    modes = sGlobalTerminalConnectGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    mainTemplates = sGlobalTerminalConnectMainBgTemplates;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &mainTemplates.templates[0], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_Y, 0);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &mainTemplates.templates[1], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, 0);
    subTemplates = sGlobalTerminalConnectSubBgTemplates;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &subTemplates.templates[0], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_Y, 0);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &subTemplates.templates[1], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, 0);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_124);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_124);
}

void GlobalTerminalConnect_FreeBgs(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
}

void GlobalTerminalConnect_LoadGraphics(GlobalTerminalConnectWork *work) {
    NNSG2dPaletteData *plttData;
    u8 *colors;
    volatile int coefficient;
    NARC *narc;
    void *raw;
    BgConfig *bgConfig = work->bgConfig;
    int outer;
    int palette;

    narc = NARC_New(NARC_a_0_8_8, HEAP_ID_124);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_124);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_124);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_124);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_124);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 1, 10, Options_GetFrame(Save_PlayerData_GetOptionsAddr(work->args->launchArgs->saveData)), HEAP_ID_124);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 0x1F, 11, 0, HEAP_ID_124);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_124);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, FALSE, HEAP_ID_124);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 11, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_124);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 12, bgConfig, GF_BG_LYR_SUB_1, 0, 0x600, FALSE, HEAP_ID_124);
    BG_SetMaskColor(GF_BG_LYR_MAIN_0, RGB_BLACK);
    BG_SetMaskColor(GF_BG_LYR_SUB_0, RGB_BLACK);
    MI_CpuFill8(&work->paletteCycle, 0, sizeof(GlobalTerminalConnectPaletteCycle));
    raw = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 5, &plttData, HEAP_ID_124);
    // the first 4 palettes of the file are the cycle's base palettes; palette 0 is also the first blended one
    MIi_CpuCopy16(plttData->pRawData, work->paletteCycle.basePalettes[0], 0x80);
    MIi_CpuCopy16(plttData->pRawData, work->paletteCycle.palettes[0], 0x80);
    Heap_Free(raw);
    palette = 0;
    outer = 0;
    {
        // 3 x 7 palettes: base palette n blended towards base palette n+1 in steps of 3/16 (color 0 unchanged);
        // the source and target colors are addressed relative to the work as in the original
        int blend;
        int last;
        u8 *dest = (u8 *)work + 0x130;
        u8 *src = (u8 *)work + 0xB0;
        while (outer < 3) {
            blend = 0;
            last = 0;
            colors = (u8 *)work + (outer + 1) * 0x20;
            do {
                int i;
                u8 *color;
                u8 *dst;
                u8 *source;
                if (palette >= 21) {
                    GF_AssertFail();
                }
                color = colors + 2;
                dst = dest + 2;
                source = src + 2;
                coefficient = (u8)(blend >> 8);
                for (i = 1; i < 16; i++) {
                    // called with an int fraction (no u8 truncation)
                    ((void (*)(const u16 *, u16 *, u16, int, u16))BlendPalette)((u16 *)source, (u16 *)dst, 1, coefficient, *(u16 *)(color + 0xB0));
                    color += 2;
                    dst += 2;
                    source += 2;
                }
                dest += 0x20;
                palette++;
                if (last == 1) {
                    break;
                }
                blend += 0x300;
                if (blend >= 0x1000) {
                    blend = 0x1000;
                    last = 1;
                }
            } while (1);
            src += 0x20;
            outer++;
        }
    }
    DC_FlushRange(work->paletteCycle.palettes, sizeof(work->paletteCycle.palettes));
    work->paletteCycle.enabled = 1;
    work->paletteCycle.screens = 0;
    work->paletteCycle.task = SysTask_CreateOnVBlankQueue((SysTaskFunc)GlobalTerminalConnect_PaletteCycleTask, &work->paletteCycle, 20);
    NARC_Delete(narc);
}

void GlobalTerminalConnect_PaletteCycleTask(SysTask *task, GlobalTerminalConnectPaletteCycle *cycle) {
    if (cycle->enabled != 0) {
        cycle->frame ^= 1;
        if (!(cycle->frame & 1)) {
            if (cycle->screens <= 1) {
                GX_LoadBGPltt(cycle->palettes[cycle->index], 0, 0x20);
            }
            if (cycle->screens == 0 || cycle->screens == 2) {
                GXS_LoadBGPltt(cycle->palettes[cycle->index], 0, 0x20);
            }
            if (cycle->direction == 0) {
                cycle->index++;
                if (cycle->index >= 21) {
                    cycle->index = 19;
                    cycle->direction ^= 1;
                }
            } else {
                cycle->index--;
                if (cycle->index < 0) {
                    cycle->index = 1;
                    cycle->direction ^= 1;
                }
            }
        }
    }
}

void GlobalTerminalConnect_CreateWindows(GlobalTerminalConnectWork *work) {
    AddWindowParameterized(work->bgConfig, &work->errorWindow, GF_BG_LYR_MAIN_0, 4, 4, 23, 16, 13, 0x94);
    FillWindowPixelBuffer(&work->errorWindow, 0);
    AddWindowParameterized(work->bgConfig, &work->titleWindow, GF_BG_LYR_MAIN_0, 4, 1, 24, 2, 13, 0x204);
    FillWindowPixelBuffer(&work->titleWindow, 0);
    GlobalTerminalConnect_PrintAligned(&work->titleWindow, work->title, 0, 1, 1, MAKE_TEXT_COLOR(15, 14, 0));
    AddWindowParameterized(work->bgConfig, &work->messageWindow, GF_BG_LYR_MAIN_0, 2, 19, 27, 4, 13, 0x28);
    FillWindowPixelBuffer(&work->messageWindow, 0);
}

void GlobalTerminalConnect_RemoveWindows(GlobalTerminalConnectWork *work) {
    RemoveWindow(&work->messageWindow);
    RemoveWindow(&work->titleWindow);
    RemoveWindow(&work->errorWindow);
}

// alignment 1 = centred, 2 = right-aligned, otherwise x
int GlobalTerminalConnect_GetAlignedX(Window *window, String *string, int x, int alignment, int unused, int fontId) {
    switch (alignment) {
    case 1:
        x = ((window->width << 3) - (int)FontID_String_GetWidth(fontId, string, 0)) / 2;
        break;
    case 2:
        x = (window->width << 3) - FontID_String_GetWidth(fontId, string, 0);
        break;
    }
    return x;
}

void GlobalTerminalConnect_PrintAligned(Window *window, String *string, int x, int y, int alignment, u32 color) {
    x = GlobalTerminalConnect_GetAlignedX(window, string, x, alignment, color, 1);
    AddTextPrinterParameterizedWithColor(window, 1, string, x, y, TEXT_SPEED_INSTANT, color, NULL);
}

void GlobalTerminalConnect_ShowErrorWindow(GlobalTerminalConnectWork *work, int msgId) {
    String *string = String_New(256, HEAP_ID_124);
    ReadMsgDataIntoString(work->msgDataWifi, msgId, string);
    StringExpandPlaceholders(work->msgFormat, work->errorString, string);
    FillWindowPixelBuffer(&work->errorWindow, 15);
    DrawFrameAndWindow1(&work->errorWindow, TRUE, 0x1F, 11);
    AddTextPrinterParameterized(&work->errorWindow, 1, work->errorString, 0, 0, TEXT_SPEED_INSTANT, NULL);
    work->printerId = 0xFF;
    String_Delete(string);
}

void GlobalTerminalConnect_ShowErrorCode(GlobalTerminalConnectWork *work, int msgId, int errorNumber) {
    if (msgId == -1) {
        msgId = 11;
    }
    BufferIntegerAsString(work->msgFormat, 0, errorNumber, 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
    ClearFrameAndWindow2(&work->messageWindow, TRUE);
    GlobalTerminalConnect_ShowErrorWindow(work, msgId);
}

BOOL GlobalTerminalConnect_IsPrinting(u32 printerId) {
    if (printerId == 0xFF || !TextPrinterCheckActive(printerId)) {
        return FALSE;
    }
    return TRUE;
}

void GlobalTerminalConnect_ShowWaitingIcon(GlobalTerminalConnectWork *work) {
    if (work->waitingIcon == NULL) {
        work->waitingIcon = WaitingIcon_New(&work->messageWindow, 1);
    }
}

void GlobalTerminalConnect_HideWaitingIcon(GlobalTerminalConnectWork *work) {
    if (work->waitingIcon != NULL) {
        WaitingIcon_Delete(work->waitingIcon);
        work->waitingIcon = NULL;
    }
}

#pragma force_active on

YesNoPrompt *GlobalTerminalConnect_CreateYesNoPrompt(BgConfig *bgConfig, int tileStart) {
    YesNoPromptTemplate template;
    YesNoPrompt *prompt = YesNoPrompt_Create(HEAP_ID_124);

    template.bgConfig = bgConfig;
    template.bgId = GF_BG_LYR_MAIN_0;
    template.tileStart = tileStart;
    template.plttSlot = 14;
    template.x = 25;
    template.y = 10;
    template.ignoreTouchFlag = 0;
    template.initialCursorPos = 0;
    template.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(prompt, &template);
    return prompt;
}

// color is not used: the frame and printer use their defaults
void GlobalTerminalConnect_PrintMessage(GlobalTerminalConnectWork *work, MsgData *msgData, int msgId, int textSpeed, u32 color) {
    String *string = NewString_ReadMsgData(msgData, msgId);
    StringExpandPlaceholders(work->msgFormat, work->string, string);
    String_Delete(string);
    FillWindowPixelBuffer(&work->messageWindow, 15);
    DrawFrameAndWindow2(&work->messageWindow, FALSE, 1, 10);
    work->printerId = AddTextPrinterParameterized(&work->messageWindow, 1, work->string, 0, 0, textSpeed, NULL);
    work->unk_8C = 0;
    if (textSpeed == TEXT_SPEED_NOTRANSFER || textSpeed == TEXT_SPEED_INSTANT) {
        work->printerId = 0xFF;
    }
}

#pragma force_active reset

void GlobalTerminalConnect_SetStateAndNext(GlobalTerminalConnectWork *work, u32 state, u32 nextState) {
    work->state = state;
    work->nextState = nextState;
}

BOOL GlobalTerminalConnect_ErrorAllowsRetry(int errorCode, int errorType) {
    switch (WifiConnection_ClassifyErrorCode(errorCode, errorType)) {
    case 1:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        return TRUE;
    case 0:
    case 2:
    case 3:
    default:
        return FALSE;
    }
}

BOOL GlobalTerminalConnect_StateAskConnect(GlobalTerminalConnectWork *work) {
    int result;

    if (GlobalTerminalConnect_IsPrinting(work->printerId) == TRUE) {
        return FALSE;
    }
    switch (work->substate) {
    case 0:
        work->substate++;
        break;
    case 1:
        GlobalTerminalConnect_PrintMessage(work, work->msgDataWifi, msg_0800_00017, work->textSpeed, MAKE_TEXT_COLOR(0, 15, 15));
        work->substate++;
        break;
    case 2:
        work->yesNoPrompt = GlobalTerminalConnect_CreateYesNoPrompt(work->bgConfig, 0x234);
        work->substate++;
        break;
    case 3:
        result = YesNoPrompt_HandleInput(work->yesNoPrompt);
        if (result == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(work->yesNoPrompt);
            work->substate++;
        } else if (result == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(work->yesNoPrompt);
            work->state = GTC_STATE_EXIT;
        }
        break;
    case 4:
        work->state = GTC_STATE_START_CONNECT;
        break;
    }
    return FALSE;
}

BOOL GlobalTerminalConnect_StateStartConnect(GlobalTerminalConnectWork *work) {
    switch (work->substate) {
    case 0:
        CommState_InitWfcSession(work->args->launchArgs->saveData);
        Sys_SetSleepDisableFlag(4);
        WirelessStatusIcon_Create();
        GlobalTerminalConnect_PrintMessage(work, work->msgDataConnecting, msg_0778_00001, work->textSpeed, MAKE_TEXT_COLOR(0, 15, 15));
        GlobalTerminalConnect_ShowWaitingIcon(work);
        work->substate++;
        break;
    case 1:
        if (GlobalTerminalConnect_IsPrinting(work->printerId) == FALSE) {
            work->substate++;
        }
        break;
    case 2:
        DWC_InitInetEx(work->args->wifiConnection, 2, 1, 20);
        DWC_SetAuthServer(2);
        DWC_ConnectInetAsync();
        work->state = GTC_STATE_WAIT_CONNECT;
        break;
    }
    return FALSE;
}

BOOL GlobalTerminalConnect_StateWaitConnect(GlobalTerminalConnectWork *work) {
    u8 buffer[0x48];

    DWC_ProcessInet();
    if (DWC_CheckInet() != 0) {
        switch (DWC_GetInetStatus()) {
        case 7:
            work->dwcResult = DWC_GetLastErrorEx((int *)(buffer + 8), (u32 *)(buffer + 4));
            work->errorCode = *(u32 *)(buffer + 8);
            work->errorType = *(u32 *)(buffer + 4);
            DWC_ClearError();
            DWC_CleanupInet();
            GlobalTerminalConnect_HideWaitingIcon(work);
            work->state = GTC_STATE_SHOW_ERROR;
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
        case 8:
        default:
            DWC_GetLastError(buffer);
            GlobalTerminalConnect_HideWaitingIcon(work);
            work->state = GTC_STATE_SHOW_SERVER_ERROR;
            work->serverError = -2;
            break;
        case 4:
            DWC_GetApInfo(buffer + 12);
            work->state = GTC_STATE_3;
            break;
        }
    }
    return FALSE;
}

BOOL GlobalTerminalConnect_StateStartAuth(GlobalTerminalConnectWork *work) {
    DWC_NASLoginAsync(work);
    work->state = GTC_STATE_4;
    return FALSE;
}

#pragma force_active on

BOOL GlobalTerminalConnect_StateWaitAuth(GlobalTerminalConnectWork *work) {
    int errorCode;
    u32 errorType;

    switch (DWC_NASLoginProcess()) {
    case 3:
        work->state = GTC_STATE_SAVE_PROFILE_ID;
        break;
    case 0:
    case 4:
    case 5:
        GlobalTerminalConnect_HideWaitingIcon(work);
        work->dwcResult = DWC_GetLastErrorEx(&errorCode, &errorType);
        work->errorCode = errorCode;
        work->errorType = errorType;
        DWC_ClearError();
        DWC_CleanupInet();
        work->state = GTC_STATE_SHOW_ERROR;
        switch (errorType) {
        case 1:
        case 2:
            work->state = GTC_STATE_SHOW_ERROR;
            break;
        case 4:
            DWC_ShutdownGHTTP();
            work->state = GTC_STATE_SHOW_ERROR;
            break;
        case 6:
            work->state = GTC_STATE_SHOW_ERROR;
            break;
        case 3:
            DWC_ShutdownFriendsMatch();
            work->state = GTC_STATE_SHOW_ERROR;
            break;
        case 5:
        case 7:
            CommState_ShowRestartErrorAndHalt();
            break;
        }
        if (errorCode < -20000 && errorCode >= -29999) {
            work->state = GTC_STATE_SHOW_ERROR;
        }
        break;
    }
    return FALSE;
}

BOOL GlobalTerminalConnect_StateSaveProfileId(GlobalTerminalConnectWork *work) {
    FriendRoster *wifiList = Save_FriendRoster_Get(work->args->launchArgs->saveData);
    SysInfo *sysInfo = Save_SysInfo_Get(work->args->launchArgs->saveData);

    FriendRoster_GetUserData(wifiList);
    if (Save_SysInfo_GetDwcProfileId(sysInfo) == 0) {
        Save_SysInfo_SetDwcProfileId(sysInfo, (s32)Comm_GetOwnGsProfileId(wifiList));
    }
    Save_SysInfo_GetDwcProfileId(sysInfo);
    work->state = GTC_STATE_EXIT_CONNECTED;
    work->args->isConnected = TRUE;
    return FALSE;
}

#pragma force_active reset

BOOL GlobalTerminalConnect_StateShowError(GlobalTerminalConnectWork *work) {
    int msgId = WifiConnection_ClassifyErrorCode(-work->errorCode, work->errorType);
    GlobalTerminalConnect_ShowErrorCode(work, msgId, -work->errorCode);
    work->state = GTC_STATE_WAIT_ERROR_ACK;
    return FALSE;
}

BOOL GlobalTerminalConnect_StateWaitErrorAck(GlobalTerminalConnectWork *work) {
    if ((gSystem.newKeys & PAD_BUTTON_A) || (gSystem.newKeys & PAD_BUTTON_B) || System_GetTouchNew() == TRUE) {
        EraseFrameAndWindow1(&work->errorWindow, FALSE);
        work->substate = 0;
        if (GlobalTerminalConnect_ErrorAllowsRetry(-work->errorCode, work->errorType) == TRUE) {
            work->state = GTC_STATE_ASK_CONNECT;
        } else {
            work->state = GTC_STATE_EXIT;
        }
    }
    return FALSE;
}

BOOL GlobalTerminalConnect_StateExit(GlobalTerminalConnectWork *work) {
    if (work->state != GTC_STATE_EXIT_CONNECTED) {
        CommState_FreeWfcSession();
    }
    WirelessStatusIcon_Destroy();
    GlobalTerminalConnect_HideWaitingIcon(work);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_124);
    work->state = GTC_STATE_ASK_CONNECT;
    return TRUE;
}

BOOL GlobalTerminalConnect_StateShowServerError(GlobalTerminalConnectWork *work) {
    int msgId = 0;

    switch (work->serverError + 15) {
    case 16:
        msgId = 150;
        break;
    case 14:
    case 17:
        msgId = 151;
        break;
    case 1:
    case 13:
        msgId = 156;
        break;
    case 0:
    case 3:
    case 10:
    case 12:
        msgId = 155;
        break;
    }
    GlobalTerminalConnect_PrintMessage(work, work->msgDataGts, msgId, work->textSpeed, MAKE_TEXT_COLOR(0, 15, 15));
    GlobalTerminalConnect_SetStateAndNext(work, GTC_STATE_WAIT_MESSAGE, GTC_STATE_SERVER_ERROR_DISCONNECT);
    return FALSE;
}

BOOL GlobalTerminalConnect_StateServerErrorDisconnect(GlobalTerminalConnectWork *work) {
    switch (work->substate) {
    case 0:
        GlobalTerminalConnect_PrintMessage(work, work->msgDataGts, msg_0775_00186, work->textSpeed, MAKE_TEXT_COLOR(0, 15, 15));
        work->substate++;
        break;
    case 1:
        if (GlobalTerminalConnect_IsPrinting(work->printerId) == FALSE) {
            CommState_FreeWfcSession();
            DWC_CleanupInet();
            work->substate++;
        }
        break;
    case 2:
        GlobalTerminalConnect_PrintMessage(work, work->msgDataGts, msg_0775_00187, work->textSpeed, MAKE_TEXT_COLOR(0, 15, 15));
        work->substate++;
        break;
    case 3:
        if (GlobalTerminalConnect_IsPrinting(work->printerId) == FALSE) {
            work->substate++;
        }
        break;
    default:
        work->timer++;
        if (work->timer > 30) {
            work->state = GTC_STATE_EXIT;
        }
        break;
    }
    return FALSE;
}

BOOL GlobalTerminalConnect_StateWaitMessage(GlobalTerminalConnectWork *work) {
    if (!GlobalTerminalConnect_IsPrinting(work->printerId)) {
        work->state = work->nextState;
    }
    return FALSE;
}

BOOL GlobalTerminalConnect_StateDisconnect(GlobalTerminalConnectWork *work) {
    if (GlobalTerminalConnect_IsPrinting(work->printerId) == TRUE) {
        return FALSE;
    }
    switch (work->substate) {
    case 0:
        GlobalTerminalConnect_PrintMessage(work, work->msgDataWifi, msg_0800_00026, work->textSpeed, MAKE_TEXT_COLOR(0, 15, 15));
        work->substate++;
        break;
    case 1:
        DWC_CleanupInet();
        CommState_FreeWfcSession();
        work->substate++;
        break;
    case 2:
        WirelessStatusIcon_Destroy();
        GlobalTerminalConnect_PrintMessage(work, work->msgDataWifi, msg_0800_00027, work->textSpeed, MAKE_TEXT_COLOR(0, 15, 15));
        work->substate++;
        break;
    case 3:
        work->timer++;
        if (work->timer > 30) {
            work->state = work->nextState;
        }
        break;
    }
    return FALSE;
}

void GlobalTerminalNet_DressUpToUploadForm(const GlobalTerminalNetDressUpRecord *src, GlobalTerminalNetDressUpForm *dst) {
    int i;

    MI_CpuFill8(dst, 0, sizeof(GlobalTerminalNetDressUpForm));
    dst->unk_00 = src->unk_00;
    dst->unk_04 = src->unk_04;
    dst->unk_08 = src->unk_08;
    dst->unk_0C = src->unk_0C;
    for (i = 0; i < 8; i++) {
        dst->unk_0E[i] = src->unk_24[i];
    }
    dst->unk_1E = src->unk_34;
    dst->unk_1F = src->unk_35;
    dst->unk_20 = src->unk_36;
    dst->unk_21 = src->unk_37;
    dst->unk_24 = src->unk_3C;
    dst->unk_28 = src->unk_40;
    dst->unk_2A = src->unk_42;
    dst->unk_2C = src->unk_44;
    dst->unk_2E = src->unk_46;
    {
        // 10 groups of 4 bytes, the destination stepped as a whole struct pointer
        const u8 *s = src->unk_48;
        GlobalTerminalNetDressUpForm *d = dst;
        for (i = 0; i < 10; i++) {
            d->unk_30[0] = s[0];
            d->unk_30[1] = s[1];
            d->unk_30[2] = s[2];
            d->unk_30[3] = s[3];
            s += 4;
            d = (GlobalTerminalNetDressUpForm *)((u8 *)d + 4);
        }
    }
    dst->unk_58 = src->unk_70;
    dst->unk_59 = src->unk_71;
}

void GlobalTerminalNet_UploadFormToDressUp(const GlobalTerminalNetDressUpForm *src, GlobalTerminalNetDressUpRecord *dst) {
    int i;

    MI_CpuFill8(dst, 0, FashionEntry_sizeof());
    dst->unk_00 = src->unk_00;
    dst->unk_04 = src->unk_04;
    dst->unk_08 = src->unk_08;
    dst->unk_0C = src->unk_0C;
    for (i = 0; i < 8; i++) {
        dst->unk_24[i] = src->unk_0E[i];
    }
    dst->unk_34 = src->unk_1E;
    dst->unk_35 = src->unk_1F;
    dst->unk_36 = src->unk_20;
    dst->unk_37 = src->unk_21;
    dst->unk_3C = src->unk_24;
    dst->unk_40 = src->unk_28;
    dst->unk_42 = src->unk_2A;
    dst->unk_44 = src->unk_2C;
    dst->unk_46 = src->unk_2E;
    {
        const u8 *s = src->unk_30;
        GlobalTerminalNetDressUpRecord *d = dst;
        for (i = 0; i < 10; i++) {
            d->unk_48[0] = s[0];
            d->unk_48[1] = s[1];
            d->unk_48[2] = s[2];
            d->unk_48[3] = s[3];
            s += 4;
            d = (GlobalTerminalNetDressUpRecord *)((u8 *)d + 4);
        }
    }
    dst->unk_70 = src->unk_58;
    dst->unk_71 = src->unk_59;
    {
        GlobalTerminalNetDressUpRecord *p = dst;
        for (i = 0; i < 11; i++) {
            *(u16 *)&p->unk_0E[0] = 0xFFFF;
            p = (GlobalTerminalNetDressUpRecord *)((u8 *)p + 2);
        }
    }
    dst->unk_38[0] = 0;
}
