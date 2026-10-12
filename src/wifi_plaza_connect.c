#include "global.h"

#include "constants/game_stats.h"
#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0778.h"
#include "msgdata/msg/msg_0800.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "game_stats.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "gf_rtc.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_window.h"
#include "save.h"
#include "screen_fade.h"
#include "sound_system.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "comm_state.h"
#include "wireless_status_icon.h"
#include "yes_no_prompt.h"

// Wi-Fi Plaza connect / disconnect screens, two OverlayManager templates started by the Wi-Fi Plaza
// lobby overlay (OVY_45): "Connect to Nintendo Wi-Fi Connection?" + DWC login with error handling and
// reconnect, and "Disconnecting from Nintendo Wi-Fi Connection." + logout. The BG palette of both
// screens cycles through 21 pre-blended palettes on a VBlank task.

#define WIFI_PLAZA_CONNECT_NUM_BG_LAYERS     3
#define WIFI_PLAZA_CONNECT_NUM_PALETTES      21
#define WIFI_PLAZA_CONNECT_MSG_STRING_LENGTH 0x100

// Arguments of the connect screen (built by OVY_45).
typedef struct WiFiPlazaConnectArgs {
    SaveData *saveData;
    void *lobby;
    BOOL skipPrompt;
    s64 *loginTime;
    BOOL connected;
} WiFiPlazaConnectArgs;

// Arguments of the disconnect screen (built by OVY_45).
typedef struct WiFiPlazaDisconnectArgs {
    SaveData *saveData;
    void *lobby;
    BOOL unk_08;
} WiFiPlazaDisconnectArgs;

typedef struct WiFiPlazaConnectMessageBox {
    MessageFormat *format;
    MsgData *msgData;
    Window window;
    String *expanded;
    String *raw;
    int frameType; // 0: standard frame, 1: frame 2 with a waiting icon
    WaitingIcon *waitingIcon;
    int textSpeed;
    int printerId;
} WiFiPlazaConnectMessageBox; // size: 0x30

// Block handed to the VBlank palette-cycle task.
typedef struct WiFiPlazaConnectPaletteCycle {
    SysTask *task;
    BOOL active;
    u16 baseColors[4][16];
    u16 blendedPalettes[WIFI_PLAZA_CONNECT_NUM_PALETTES][16];
    s16 index; // ping-pongs 0..20
    s8 direction;
    u8 frameParity;             // the palette is loaded on every second frame
    u32 mode;                   // 0: both screens, 1: main screen, 2: sub screen
} WiFiPlazaConnectPaletteCycle; // size: 0x330

typedef struct WiFiPlazaConnectWork {
    SaveData *saveData;
    u32 unk_04;
    u32 timer;
    BgConfig *bgConfig;
    WiFiPlazaConnectMessageBox titleBox;  // msg_0778
    WiFiPlazaConnectMessageBox msgBox;    // msg_0778 when connecting, msg_0800 when disconnecting
    WiFiPlazaConnectMessageBox statusBox; // msg_0800
    WiFiPlazaConnectMessageBox errorBox;  // msg_0800
    YesNoPrompt *yesNoPrompt;
    WiFiPlazaConnectPaletteCycle paletteCycle;
} WiFiPlazaConnectWork; // size: 0x404

// network_communication_state_full.c and the OVY_45 lobby have no public headers.
extern void CommState_StartWifiPlaza(SaveData *saveData, s32 profileId);
extern void CommState_LeaveWifiPlaza(void);
extern int CommState_HasPlazaLobbyStarted(void);
extern BOOL CommState_HasPlazaLobbyFailed(void);
extern void *WiFiPlazaWork_GetMyPlayer(void *lobby);
extern void ov45_0222B244(void *lobby);
extern void ov45_0222B270(void *lobby);
extern int ov45_0222D7CC(u32 errorCode, u32 errorType);
extern BOOL ov45_0222D7FC(u32 errorCode, u32 errorType);
extern void ov45_0222E7CC(void);
extern u32 ov45_0222E7FC(void);
extern void ov45_0222ED7C(void);
extern BOOL ov45_0222EDA8(void);

extern const u32 sWiFiPlazaConnectBgLayers[WIFI_PLAZA_CONNECT_NUM_BG_LAYERS];
extern const GraphicsModes sWiFiPlazaConnectGraphicsModes;
extern const GraphicsBanks sWiFiPlazaConnectGraphicsBanks;
extern const BgTemplate sWiFiPlazaConnectBgTemplates[WIFI_PLAZA_CONNECT_NUM_BG_LAYERS];

BOOL WiFiPlazaConnect_Init(OverlayManager *man, int *state);
BOOL WiFiPlazaConnect_Main(OverlayManager *man, int *state);
BOOL WiFiPlazaConnect_Exit(OverlayManager *man, int *state);
BOOL WiFiPlazaConnect_DisconnectInit(OverlayManager *man, int *state);
BOOL WiFiPlazaConnect_DisconnectMain(OverlayManager *man, int *state);
BOOL WiFiPlazaConnect_DisconnectExit(OverlayManager *man, int *state);
void WiFiPlazaConnect_VBlankCB(void *data);
void WiFiPlazaConnect_InitGraphics(WiFiPlazaConnectWork *work, enum HeapID heapID);
void WiFiPlazaConnect_PaletteCycleTask(SysTask *task, void *data);
void WiFiPlazaConnect_FreeBgs(WiFiPlazaConnectWork *work);
void WiFiPlazaConnect_DoBgUpdates(WiFiPlazaConnectWork *work);
void WiFiPlazaConnect_InitMessageBox(WiFiPlazaConnectMessageBox *box, BgConfig *bgConfig, int frameType, int msgBank, int x, int y, int width, int height, int baseTile, SaveData *saveData, enum HeapID heapID);
void WiFiPlazaConnect_PrintMessage(WiFiPlazaConnectMessageBox *box, int msgId);
void WiFiPlazaConnect_ClearMessageBox(WiFiPlazaConnectMessageBox *box);
void WiFiPlazaConnect_StartWaitingIcon(WiFiPlazaConnectMessageBox *box);
void WiFiPlazaConnect_StopWaitingIcon(WiFiPlazaConnectMessageBox *box);
void WiFiPlazaConnect_FreeMessageBox(WiFiPlazaConnectMessageBox *box);
void WiFiPlazaConnect_PrintCenteredTitle(WiFiPlazaConnectMessageBox *box, int msgId);
void WiFiPlazaConnect_BufferErrorCode(WiFiPlazaConnectMessageBox *box, int errorCode);
YesNoPrompt *WiFiPlazaConnect_CreateYesNoPrompt(BgConfig *bgConfig, int tileStart);

BOOL WiFiPlazaConnect_Init(OverlayManager *man, int *state) {
    WiFiPlazaConnectArgs *args;
    WiFiPlazaConnectWork *work;

    Heap_Create(HEAP_ID_3, HEAP_ID_119, 0x20000);
    work = OverlayManager_CreateAndGetData(man, sizeof(WiFiPlazaConnectWork), HEAP_ID_119);
    memset(work, 0, sizeof(WiFiPlazaConnectWork));
    args = OverlayManager_GetArgs(man);
    work->saveData = args->saveData;
    Sound_SetSceneAndPlayBGM(11, SEQ_GS_WIFI_ACCESS, 0);
    args->connected = FALSE;
    WiFiPlazaConnect_InitGraphics(work, HEAP_ID_119);
    WiFiPlazaConnect_InitMessageBox(&work->msgBox, work->bgConfig, 1, NARC_msg_msg_0778_bin, 2, 19, 27, 4, 0x28, work->saveData, HEAP_ID_119);
    WiFiPlazaConnect_InitMessageBox(&work->statusBox, work->bgConfig, 1, NARC_msg_msg_0800_bin, 2, 19, 27, 4, 0x28, work->saveData, HEAP_ID_119);
    WiFiPlazaConnect_InitMessageBox(&work->errorBox, work->bgConfig, 0, NARC_msg_msg_0800_bin, 4, 4, 23, 16, 0x94, work->saveData, HEAP_ID_119);
    WiFiPlazaConnect_InitMessageBox(&work->titleBox, work->bgConfig, 1, NARC_msg_msg_0778_bin, 5, 1, 22, 2, 0x204, work->saveData, HEAP_ID_119);
    WiFiPlazaConnect_PrintCenteredTitle(&work->titleBox, msg_0778_00021);
    Main_SetVBlankIntrCB(WiFiPlazaConnect_VBlankCB, work);
    HBlankInterruptDisable();
    gSystem.screensFlipped = TRUE;
    GfGfx_SwapDisplay();
    return TRUE;
}

BOOL WiFiPlazaConnect_Main(OverlayManager *man, int *state) {
    WiFiPlazaConnectWork *work;
    WiFiPlazaConnectArgs *args;

    work = OverlayManager_GetData(man);
    args = OverlayManager_GetArgs(man);
    switch (*state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0xFFFF, 6, 1, HEAP_ID_119);
        (*state)++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            (*state)++;
        }
        break;
    case 2:
        if (!args->skipPrompt) {
            WiFiPlazaConnect_PrintMessage(&work->statusBox, msg_0800_00017);
            work->yesNoPrompt = WiFiPlazaConnect_CreateYesNoPrompt(work->bgConfig, 0x230);
            (*state)++;
        } else {
            *state = 4;
        }
        break;
    case 3: {
        int result = YesNoPrompt_HandleInput(work->yesNoPrompt);
        if (result == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(work->yesNoPrompt);
            *state = 4;
        } else if (result == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(work->yesNoPrompt);
            WiFiPlazaConnect_ClearMessageBox(&work->msgBox);
            *state = 7;
        }
        break;
    }
    case 4:
        CommState_StartWifiPlaza(work->saveData, (s32)WiFiPlazaWork_GetMyPlayer(args->lobby));
        WiFiPlazaConnect_PrintMessage(&work->statusBox, msg_0800_00023);
        WiFiPlazaConnect_StartWaitingIcon(&work->statusBox);
        (*state)++;
        break;
    case 5:
        if (CommState_HasWifiFailed() || CommState_HasPlazaLobbyFailed()) {
            WiFiPlazaConnect_StopWaitingIcon(&work->statusBox);
            *state = 9;
        }
        if (CommState_HasPlazaLobbyStarted() == 1) {
            ov45_0222B244(args->lobby);
            (*state)++;
        }
        break;
    case 6:
        if (CommState_HasWifiFailed() || CommState_HasPlazaLobbyFailed()) {
            WiFiPlazaConnect_StopWaitingIcon(&work->statusBox);
            *state = 9;
        }
        if (CommState_IsWifiJoiningLobby()) {
            s64 seconds;
            WiFiPlazaConnect_StopWaitingIcon(&work->statusBox);
            GameStats_AddScore(Save_GameStats_Get(work->saveData), SCORE_EVENT_WIFI_PLAZA_ACCESSED);
            seconds = GF_RTC_DateTimeToSec();
            *args->loginTime = seconds;
            args->connected = TRUE;
            (*state)++;
        }
        break;
    case 9: {
        u32 errorCode;
        int msgId;
        if (CommState_HasWifiFailed()) {
            u32 *error = CommState_GetWifiError();
            msgId = ov45_0222D7CC(error[0], error[1]);
            errorCode = error[0];
        } else {
            ov45_0222E7CC();
            errorCode = ov45_0222E7FC();
            msgId = msg_0800_00032;
        }
        ov45_0222B270(args->lobby);
        WiFiPlazaConnect_ClearMessageBox(&work->msgBox);
        WiFiPlazaConnect_ClearMessageBox(&work->statusBox);
        WiFiPlazaConnect_BufferErrorCode(&work->errorBox, errorCode);
        WiFiPlazaConnect_PrintMessage(&work->errorBox, msgId);
        (*state)++;
        break;
    }
    case 10:
        if ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) || System_GetTouchNew() == TRUE) {
            if (CommState_HasWifiFailed()) {
                u32 *error = CommState_GetWifiError();
                if (ov45_0222D7FC(error[0], error[1]) == FALSE) {
                    *state = 11;
                } else {
                    *state = 14;
                }
            } else {
                *state = 11;
            }
        }
        break;
    case 11:
        WiFiPlazaConnect_ClearMessageBox(&work->errorBox);
        WiFiPlazaConnect_PrintMessage(&work->msgBox, msg_0778_00088);
        work->yesNoPrompt = WiFiPlazaConnect_CreateYesNoPrompt(work->bgConfig, 0x230);
        (*state)++;
        break;
    case 12: {
        int result = YesNoPrompt_HandleInput(work->yesNoPrompt);
        if (result == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(work->yesNoPrompt);
            CommState_LeaveWifiPlaza();
            *state = 13;
        } else if (result == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(work->yesNoPrompt);
            *state = 14;
        }
        break;
    }
    case 13:
        if (!CommState_Exists()) {
            *state = 4;
        }
        break;
    case 14:
        WiFiPlazaConnect_ClearMessageBox(&work->msgBox);
        WiFiPlazaConnect_ClearMessageBox(&work->statusBox);
        WiFiPlazaConnect_ClearMessageBox(&work->errorBox);
        CommState_LeaveWifiPlaza();
        (*state)++;
        break;
    case 15:
        if (!CommState_Exists()) {
            WiFiPlazaConnect_ClearMessageBox(&work->msgBox);
            WiFiPlazaConnect_ClearMessageBox(&work->statusBox);
            *state = 7;
        }
        break;
    case 7:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_119);
        (*state)++;
        break;
    case 8:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    return FALSE;
}

BOOL WiFiPlazaConnect_Exit(OverlayManager *man, int *state) {
    WiFiPlazaConnectWork *work = OverlayManager_GetData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    if (work->paletteCycle.task != NULL) {
        SysTask_Destroy(work->paletteCycle.task);
        work->paletteCycle.task = NULL;
        work->paletteCycle.active = FALSE;
    }
    WiFiPlazaConnect_FreeMessageBox(&work->msgBox);
    WiFiPlazaConnect_FreeMessageBox(&work->statusBox);
    WiFiPlazaConnect_FreeMessageBox(&work->errorBox);
    WiFiPlazaConnect_FreeMessageBox(&work->titleBox);
    WiFiPlazaConnect_FreeBgs(work);
    work->paletteCycle.active = FALSE;
    Heap_Free(work);
    Heap_Destroy(HEAP_ID_119);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    return TRUE;
}

BOOL WiFiPlazaConnect_DisconnectInit(OverlayManager *man, int *state) {
    WiFiPlazaConnectWork *work;
    WiFiPlazaDisconnectArgs *args;

    Heap_Create(HEAP_ID_3, HEAP_ID_119, 0x20000);
    work = OverlayManager_CreateAndGetData(man, sizeof(WiFiPlazaConnectWork), HEAP_ID_119);
    memset(work, 0, sizeof(WiFiPlazaConnectWork));
    args = OverlayManager_GetArgs(man);
    work->saveData = args->saveData;
    WiFiPlazaConnect_InitGraphics(work, HEAP_ID_119);
    WiFiPlazaConnect_InitMessageBox(&work->msgBox, work->bgConfig, 1, NARC_msg_msg_0800_bin, 2, 19, 27, 4, 0x28, work->saveData, HEAP_ID_119);
    WiFiPlazaConnect_InitMessageBox(&work->errorBox, work->bgConfig, 0, NARC_msg_msg_0800_bin, 4, 4, 23, 16, 0x94, work->saveData, HEAP_ID_119);
    WiFiPlazaConnect_InitMessageBox(&work->titleBox, work->bgConfig, 1, NARC_msg_msg_0778_bin, 5, 1, 22, 2, 0x204, work->saveData, HEAP_ID_119);
    WiFiPlazaConnect_PrintCenteredTitle(&work->titleBox, msg_0778_00021);
    WirelessStatusIcon_Create();
    Main_SetVBlankIntrCB(WiFiPlazaConnect_VBlankCB, work);
    HBlankInterruptDisable();
    gSystem.screensFlipped = TRUE;
    GfGfx_SwapDisplay();
    return TRUE;
}

BOOL WiFiPlazaConnect_DisconnectMain(OverlayManager *man, int *state) {
    WiFiPlazaConnectWork *work;
    WiFiPlazaDisconnectArgs *args;

    work = OverlayManager_GetData(man);
    args = OverlayManager_GetArgs(man);
    switch (*state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0xFFFF, 6, 1, HEAP_ID_119);
        (*state)++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            *state = 2;
        }
        break;
    case 2:
        WiFiPlazaConnect_PrintMessage(&work->msgBox, msg_0800_00026);
        if (CommState_HasWifiFailed() || CommState_HasPlazaLobbyFailed()) {
            *state = 5;
            WiFiPlazaConnect_StartWaitingIcon(&work->msgBox);
        } else {
            *state = 3;
            WiFiPlazaConnect_StartWaitingIcon(&work->msgBox);
        }
        break;
    case 3:
        ov45_0222ED7C();
        work->timer = 900;
        *state = 4;
        break;
    case 4:
        --work->timer;
        if (ov45_0222EDA8() || work->timer == 0) {
            *state = 5;
        }
        break;
    case 5:
        CommState_LeaveWifiPlaza();
        *state = 6;
        break;
    case 6:
        if (!CommState_Exists()) {
            // the status box is not set up on this screen (frame type 0): this does nothing
            WiFiPlazaConnect_StopWaitingIcon(&work->statusBox);
            ov45_0222B270(args->lobby);
            *state = 7;
        }
        break;
    case 7:
        WiFiPlazaConnect_PrintMessage(&work->msgBox, msg_0800_00027);
        work->timer = 90;
        *state = 8;
        break;
    case 8:
        if (--work->timer == 0) {
            *state = 9;
        }
        break;
    case 9:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_119);
        (*state)++;
        break;
    case 10:
        if (IsPaletteFadeFinished()) {
            WiFiPlazaConnect_StopWaitingIcon(&work->msgBox);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL WiFiPlazaConnect_DisconnectExit(OverlayManager *man, int *state) {
    WiFiPlazaConnectWork *work = OverlayManager_GetData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    if (work->paletteCycle.task != NULL) {
        SysTask_Destroy(work->paletteCycle.task);
        work->paletteCycle.task = NULL;
        work->paletteCycle.active = FALSE;
    }
    WiFiPlazaConnect_FreeMessageBox(&work->msgBox);
    WiFiPlazaConnect_FreeMessageBox(&work->errorBox);
    WiFiPlazaConnect_FreeMessageBox(&work->titleBox);
    WiFiPlazaConnect_FreeBgs(work);
    work->paletteCycle.active = FALSE;
    Heap_Free(work);
    Heap_Destroy(HEAP_ID_119);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    return TRUE;
}

void WiFiPlazaConnect_VBlankCB(void *data) {
    WiFiPlazaConnect_DoBgUpdates(data);
}

#pragma force_active on

void WiFiPlazaConnect_InitGraphics(WiFiPlazaConnectWork *work, enum HeapID heapID) {
    int i;
    int j;
    const BgTemplate *template;
    NARC *narc;
    int target;
    int count;
    int amount;
    BOOL final;
    u16 *dst;
    u16 *src;
    u8 blend;
    NNSG2dPaletteData *plttData;
    void *plttFile;
    u8 frame;

    G2_BlendNone();
    G2S_BlendNone();
    GfGfx_SetBanks(&sWiFiPlazaConnectGraphicsBanks);
    BG_SetMaskColor(GF_BG_LYR_MAIN_0, RGB_BLACK);
    SetBothScreensModesAndDisable(&sWiFiPlazaConnectGraphicsModes);
    work->bgConfig = BgConfig_Alloc(heapID);
    for (i = 0; i < WIFI_PLAZA_CONNECT_NUM_BG_LAYERS; i++) {
        template = &sWiFiPlazaConnectBgTemplates[i];
        InitBgFromTemplate(work->bgConfig, (u8)sWiFiPlazaConnectBgLayers[i], template, GF_BG_TYPE_TEXT);
        BG_ClearCharDataRange((u8)sWiFiPlazaConnectBgLayers[i], 32, 0, heapID);
        BgClearTilemapBufferAndCommit(work->bgConfig, (u8)sWiFiPlazaConnectBgLayers[i]);
    }
    frame = Options_GetFrame(Save_PlayerData_GetOptionsAddr(work->saveData));
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_10_OFFSET, heapID);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_9_OFFSET, heapID);
    LoadUserFrameGfx1(work->bgConfig, GF_BG_LYR_MAIN_1, 31, 12, 0, heapID);
    LoadUserFrameGfx2(work->bgConfig, GF_BG_LYR_MAIN_1, 1, 11, frame, heapID);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_8, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x120, heapID);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_8, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x120, heapID);
    GfGfxLoader_LoadCharData(NARC_a_0_8_8, 2, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, heapID);
    GfGfxLoader_LoadCharData(NARC_a_0_8_8, 11, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, heapID);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_8, 6, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, heapID);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_8, 12, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, heapID);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    narc = NARC_New(NARC_a_0_8_8, heapID);
    MI_CpuFill8(&work->paletteCycle, 0, sizeof(WiFiPlazaConnectPaletteCycle));
    plttFile = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 5, &plttData, heapID);
    MIi_CpuCopy16(plttData->pRawData, work->paletteCycle.baseColors[0], sizeof(work->paletteCycle.baseColors));
    MIi_CpuCopy16(plttData->pRawData, work->paletteCycle.blendedPalettes[0], sizeof(work->paletteCycle.baseColors));
    Heap_Free(plttFile);

    // Blend each of the first three base palettes toward the next one in 6 steps (21 palettes).
    count = 0;
    target = 0;
    dst = work->paletteCycle.blendedPalettes[0];
    src = work->paletteCycle.baseColors[0];
    do {
        WiFiPlazaConnectWork *next;
        amount = 0;
        final = FALSE;
        // a work pointer shifted by one palette per step: next->paletteCycle.baseColors[0] is baseColors[target + 1]
        next = (WiFiPlazaConnectWork *)((u8 *)work + (target + 1) * 32);
        while (TRUE) {
            GF_ASSERT(count < WIFI_PLAZA_CONNECT_NUM_PALETTES);
            for (j = 1; j < 16; j++) {
                u16 *srcColor = src + j;
                u16 *dstColor = dst + j;
                blend = amount >> 8;
                BlendPalette(srcColor, dstColor, 1, blend, next->paletteCycle.baseColors[0][j]);
            }
            dst += 16;
            count++;
            if (final == TRUE) {
                break;
            }
            amount += 0x300;
            if (amount >= 0x1000) {
                amount = 0x1000;
                final = TRUE;
            }
        }
        src += 16;
        target++;
    } while (target < 3);
    DC_FlushRange(work->paletteCycle.blendedPalettes, sizeof(work->paletteCycle.blendedPalettes));
    work->paletteCycle.active = TRUE;
    work->paletteCycle.mode = 0;
    work->paletteCycle.task = SysTask_CreateOnVBlankQueue(WiFiPlazaConnect_PaletteCycleTask, &work->paletteCycle, 20);
    NARC_Delete(narc);
}

#pragma force_active reset

void WiFiPlazaConnect_PaletteCycleTask(SysTask *task, void *data) {
    WiFiPlazaConnectPaletteCycle *cycle = data;
    if (!cycle->active) {
        return;
    }
    cycle->frameParity ^= 1;
    if (cycle->frameParity & 1) {
        return;
    }
    if (cycle->mode <= 1) {
        GX_LoadBGPltt(cycle->blendedPalettes[cycle->index], 0, sizeof(cycle->blendedPalettes[0]));
    }
    if (cycle->mode == 0 || cycle->mode == 2) {
        GXS_LoadBGPltt(cycle->blendedPalettes[cycle->index], 0, sizeof(cycle->blendedPalettes[0]));
    }
    if (cycle->direction == 0) {
        cycle->index++;
        if (cycle->index >= WIFI_PLAZA_CONNECT_NUM_PALETTES) {
            cycle->index = WIFI_PLAZA_CONNECT_NUM_PALETTES - 2;
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

void WiFiPlazaConnect_FreeBgs(WiFiPlazaConnectWork *work) {
    int i;
    for (i = 0; i < WIFI_PLAZA_CONNECT_NUM_BG_LAYERS; i++) {
        FreeBgTilemapBuffer(work->bgConfig, (u8)sWiFiPlazaConnectBgLayers[i]);
    }
    Heap_Free(work->bgConfig);
}

void WiFiPlazaConnect_DoBgUpdates(WiFiPlazaConnectWork *work) {
    DoScheduledBgGpuUpdates(work->bgConfig);
}

void WiFiPlazaConnect_InitMessageBox(WiFiPlazaConnectMessageBox *box, BgConfig *bgConfig, int frameType, int msgBank, int x, int y, int width, int height, int baseTile, SaveData *saveData, enum HeapID heapID) {
    box->format = MessageFormat_New(heapID);
    box->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, msgBank, heapID);
    box->expanded = String_New(WIFI_PLAZA_CONNECT_MSG_STRING_LENGTH, heapID);
    box->raw = String_New(WIFI_PLAZA_CONNECT_MSG_STRING_LENGTH, heapID);
    box->frameType = frameType;
    box->textSpeed = Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(saveData));
    box->printerId = 0;
    AddWindowParameterized(bgConfig, &box->window, GF_BG_LYR_MAIN_1, (u8)x, (u8)y, (u8)width, (u8)height, 9, (u16)baseTile);
}

void WiFiPlazaConnect_PrintMessage(WiFiPlazaConnectMessageBox *box, int msgId) {
    if (TextPrinterCheckActive((u8)box->printerId)) {
        RemoveTextPrinter((u8)box->printerId);
    }
    FillWindowPixelBuffer(&box->window, 15);
    ReadMsgDataIntoString(box->msgData, msgId, box->raw);
    StringExpandPlaceholders(box->format, box->expanded, box->raw);
    AddTextPrinterParameterized(&box->window, 1, box->expanded, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
    if (box->frameType == 0) {
        DrawFrameAndWindow1(&box->window, TRUE, 31, 12);
    } else {
        DrawFrameAndWindow2(&box->window, TRUE, 1, 11);
    }
    ScheduleWindowCopyToVram(&box->window);
}

void WiFiPlazaConnect_ClearMessageBox(WiFiPlazaConnectMessageBox *box) {
    if (TextPrinterCheckActive((u8)box->printerId)) {
        RemoveTextPrinter((u8)box->printerId);
    }
    if (box->frameType == 0) {
        EraseFrameAndWindow1(&box->window, TRUE);
        ClearWindowTilemapAndScheduleTransfer(&box->window);
        return;
    }
    if (box->waitingIcon != NULL) {
        WiFiPlazaConnect_StopWaitingIcon(box);
    }
    ClearFrameAndWindow2(&box->window, TRUE);
    ClearWindowTilemapAndScheduleTransfer(&box->window);
}

void WiFiPlazaConnect_StartWaitingIcon(WiFiPlazaConnectMessageBox *box) {
    if (box->frameType == 1) {
        GF_ASSERT(box->waitingIcon == NULL);
        box->waitingIcon = WaitingIcon_New(&box->window, 1);
    }
}

void WiFiPlazaConnect_StopWaitingIcon(WiFiPlazaConnectMessageBox *box) {
    if (box->frameType == 1) {
        GF_ASSERT(box->waitingIcon != NULL);
        WaitingIcon_Delete(box->waitingIcon);
        box->waitingIcon = NULL;
    }
}

void WiFiPlazaConnect_FreeMessageBox(WiFiPlazaConnectMessageBox *box) {
    if (TextPrinterCheckActive((u8)box->printerId)) {
        RemoveTextPrinter((u8)box->printerId);
    }
    if (box->waitingIcon != NULL) {
        WiFiPlazaConnect_StopWaitingIcon(box);
    }
    RemoveWindow(&box->window);
    String_Delete(box->raw);
    String_Delete(box->expanded);
    DestroyMsgData(box->msgData);
    MessageFormat_Delete(box->format);
}

void WiFiPlazaConnect_PrintCenteredTitle(WiFiPlazaConnectMessageBox *box, int msgId) {
    int x;
    FillWindowPixelBuffer(&box->window, 0);
    ReadMsgDataIntoString(box->msgData, msgId, box->raw);
    StringExpandPlaceholders(box->format, box->expanded, box->raw);
    x = FontID_String_GetCenterAlignmentX(0, box->expanded, 0, 0xB0);
    AddTextPrinterParameterizedWithColor(&box->window, 1, box->expanded, x, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(15, 14, 0), NULL);
}

void WiFiPlazaConnect_BufferErrorCode(WiFiPlazaConnectMessageBox *box, int errorCode) {
    BufferIntegerAsString(box->format, 0, errorCode, 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
}

YesNoPrompt *WiFiPlazaConnect_CreateYesNoPrompt(BgConfig *bgConfig, int tileStart) {
    YesNoPromptTemplate template;
    YesNoPrompt *prompt = YesNoPrompt_Create(HEAP_ID_119);
    template.bgConfig = bgConfig;
    template.bgId = GF_BG_LYR_MAIN_1;
    template.tileStart = tileStart;
    template.plttSlot = 13;
    template.x = 25;
    template.y = 10;
    template.ignoreTouchFlag = FALSE;
    template.initialCursorPos = 0;
    template.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(prompt, &template);
    return prompt;
}
