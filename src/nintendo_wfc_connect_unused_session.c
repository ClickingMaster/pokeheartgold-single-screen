#include "constants/sndseq.h"

#include "msgdata/msg/msg_0778.h"
#include "msgdata/msg/msg_0799.h"
#include "msgdata/msg/msg_0800.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "frontier_data_access.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "nintendo_wfc_connect_unused.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "sav_system_info.h"
#include "save.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sys_task_api.h"
#include "text.h"
#include "wireless_status_icon.h"

extern int (*const sNintendoWfcUnusedStates[])(NintendoWfcUnused *wfc);

static void NintendoWfcUnused_InitBgs(BgConfig *bgConfig);
static void NintendoWfcUnused_FreeBgs(BgConfig *bgConfig);
static void NintendoWfcUnused_LoadBgGraphics(NintendoWfcUnused *wfc);
static void NintendoWfcUnused_PaletteCycleTask(SysTask *task, void *data);
static void NintendoWfcUnused_AddWindows(NintendoWfcUnused *wfc);
static void NintendoWfcUnused_RemoveWindows(NintendoWfcUnused *wfc);
static void NintendoWfcUnused_InitStrings(NintendoWfcUnused *wfc);
static void NintendoWfcUnused_FreeStrings(NintendoWfcUnused *wfc);
static void NintendoWfcUnused_PrintFormattedMessage(NintendoWfcUnused *wfc, MsgData *msgData, int msgNo, int textSpeed, int unused);
static void NintendoWfcUnused_PrintMessage(NintendoWfcUnused *wfc, MsgData *msgData, int msgNo, int textSpeed, int unused);
static int NintendoWfcUnused_GetAlignedTextX(const Window *window, String *string, int x, int alignment, int unused, int fontId);
static void NintendoWfcUnused_PrintAlignedText(Window *window, String *string, int x, int y, int alignment, u32 color);
static void NintendoWfcUnused_PrintAlignedTextSmall(Window *window, String *string, int x, int y, int alignment, u32 color);
static void NintendoWfcUnused_PrintNumber(Window *window, String *string, int number, int digits);
static void NintendoWfcUnused_CreateWirelessIcon(NintendoWfcUnused *wfc);
static void NintendoWfcUnused_SetArrowsVisible(NintendoWfcUnused *wfc, u32 which, BOOL visible);
static void NintendoWfcUnused_ShowWaitingIcon(NintendoWfcUnused *wfc);
static void NintendoWfcUnused_HideWaitingIcon(NintendoWfcUnused *wfc);
static void NintendoWfcUnused_PrintErrorMessage(NintendoWfcUnused *wfc, int msgNo);
static void NintendoWfcUnused_ShowErrorMessage(NintendoWfcUnused *wfc, int msgNo, int errorCode);

static const BgTemplate sNintendoWfcUnusedBgTemplate_Main0 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf800,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sNintendoWfcUnusedBgTemplate_Main1 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf000,
    .charBase = GX_BG_CHARBASE_0x08000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 1,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sNintendoWfcUnusedBgTemplate_Sub1 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xd800,
    .charBase = GX_BG_CHARBASE_0x08000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sNintendoWfcUnusedBgTemplate_Sub0 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf000,
    .charBase = GX_BG_CHARBASE_0x10000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

int NintendoWfcUnused_SessionInit(NintendoWfcUnused *wfc, int state) {
    NintendoWfcUnused_InitStrings(wfc);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 8, 1, HEAP_ID_150);
    NintendoWfcUnused_InitBgs(wfc->bgConfig);
    NintendoWfcUnused_LoadBgGraphics(wfc);
    NintendoWfcUnused_AddWindows(wfc);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, TRUE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, TRUE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, TRUE);
    CommState_InitWfcSession(wfc->args->saveData);
    NintendoWfcUnused_CreateWirelessIcon(wfc);
    wfc->ownRank = FrontierData_WifiRankAction(wfc->args->frontierData, 0);
    wfc->rank = wfc->ownRank;
    wfc->room = 1;
    wfc->waitingIcon = NULL;
    if (DWC_CheckInet() == 0) {
        if (wfc->args->connectNow != 0) {
            NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataWfc, msg_0778_00001, 1, 0xF0F);
            NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_WAIT_MESSAGE, NINTENDO_WFC_UNUSED_STATE_START_CONNECT);
            NintendoWfcUnused_ShowWaitingIcon(wfc);
        } else {
            wfc->state = NINTENDO_WFC_UNUSED_STATE_ASK_CONNECT;
        }
    } else {
        wfc->state = NINTENDO_WFC_UNUSED_STATE_ASK_LOG_OUT;
    }
    return NINTENDO_WFC_UNUSED_MAIN_WAIT_FADE_IN;
}

int NintendoWfcUnused_SessionMain(NintendoWfcUnused *wfc, int state) {
    int prevState;
    int newState;
    int result;

    WirelessStatusIcon_SetLevel(NintendoWfcUnused_GetWifiIconLevel(wfc));
    prevState = wfc->state;
    result = sNintendoWfcUnusedStates[prevState](wfc);
    newState = wfc->state;
    if (prevState != newState) {
        wfc->subState = 0;
        wfc->subTimer = 0;
    }
    return result;
}

int NintendoWfcUnused_SessionExit(NintendoWfcUnused *wfc, int state) {
    if (wfc->paletteCycle.task != NULL) {
        SysTask_Destroy(wfc->paletteCycle.task);
        wfc->paletteCycle.task = NULL;
        wfc->paletteCycle.active = 0;
    }
    NintendoWfcUnused_FreeStrings(wfc);
    NintendoWfcUnused_RemoveWindows(wfc);
    NintendoWfcUnused_FreeBgs(wfc->bgConfig);
    wfc->paletteCycle.active = 0;
    wfc->mode = wfc->nextMode;
    if (wfc->mode == 0) {
        return NINTENDO_WFC_UNUSED_MAIN_DONE;
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_INIT;
}

static void NintendoWfcUnused_InitBgs(BgConfig *bgConfig) {
    BgTemplate main0 = sNintendoWfcUnusedBgTemplate_Main0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &main0, GF_BG_TYPE_TEXT);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, FALSE);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    {
        BgTemplate main1 = sNintendoWfcUnusedBgTemplate_Main1;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &main1, GF_BG_TYPE_TEXT);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, FALSE);
        {
            BgTemplate sub0 = sNintendoWfcUnusedBgTemplate_Sub0;
            InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &sub0, GF_BG_TYPE_TEXT);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, FALSE);
            BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
            {
                BgTemplate sub1 = sNintendoWfcUnusedBgTemplate_Sub1;
                InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &sub1, GF_BG_TYPE_TEXT);
            }
        }
    }
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, FALSE);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_150);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_150);
}

static void NintendoWfcUnused_FreeBgs(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
}

static void NintendoWfcUnused_LoadBgGraphics(NintendoWfcUnused *wfc) {
    NNSG2dPaletteData *plttData;
    NARC *narc;
    int outer;
    int count;
    int blend;
    int last;
    u16 *dest;
    u16 *src;
    BgConfig *bgConfig = wfc->bgConfig;
    void *raw;
    int i;
    narc = NARC_New(NARC_a_0_8_8, HEAP_ID_150);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_150);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_150);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0x1A0, HEAP_ID_150);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0x1A0, HEAP_ID_150);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 1, 14, Options_GetFrame(wfc->args->options), HEAP_ID_150);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 31, 11, 0, HEAP_ID_150);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_150);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, FALSE, HEAP_ID_150);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 11, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_150);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 12, bgConfig, GF_BG_LYR_SUB_1, 0, 0x600, FALSE, HEAP_ID_150);
    BG_SetMaskColor(GF_BG_LYR_MAIN_0, 0);
    BG_SetMaskColor(GF_BG_LYR_SUB_0, 0);
    MI_CpuFill8(&wfc->paletteCycle, 0, sizeof(NintendoWfcUnusedPaletteCycle));
    raw = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 5, &plttData, HEAP_ID_150);
    MIi_CpuCopy16(plttData->pRawData, wfc->paletteCycle.basePltt, 0x80);
    MIi_CpuCopy16(plttData->pRawData, wfc->paletteCycle.cyclePltt, 0x80);
    Heap_Free(raw);
    // 21 palettes: three blends of 7 steps each between consecutive base palettes
    count = 0;
    outer = 0;
    dest = wfc->paletteCycle.cyclePltt;
    src = wfc->paletteCycle.basePltt;
    while (outer < 3) {
        blend = 0;
        last = 0;
        while (TRUE) {
            GF_ASSERT(count < 21);
            for (i = 1; i < 16; i++) {
                BlendPalette(&src[i], &dest[i], 1, blend >> 8, wfc->paletteCycle.basePltt[(outer + 1) * 16 + i]);
            }
            dest += 16;
            count++;
            if (last == 1) {
                break;
            }
            blend += 0x300;
            if (blend >= 0x1000) {
                blend = 0x1000;
                last = 1;
            }
        }
        src += 16;
        outer++;
    }
    DC_FlushRange(wfc->paletteCycle.cyclePltt, sizeof(wfc->paletteCycle.cyclePltt));
    wfc->paletteCycle.active = 1;
    wfc->paletteCycle.screens = 0;
    wfc->paletteCycle.task = SysTask_CreateOnVBlankQueue(NintendoWfcUnused_PaletteCycleTask, &wfc->paletteCycle, 20);
    NARC_Delete(narc);
}

static void NintendoWfcUnused_PaletteCycleTask(SysTask *task, void *data) {
    NintendoWfcUnusedPaletteCycle *cycle = data;
    if (cycle->active != 0) {
        cycle->tick ^= 1;
        if ((cycle->tick & 1) == 0) {
            if (cycle->screens <= 1) {
                GX_LoadBGPltt(cycle->cyclePltt + cycle->index * 16, 0, 0x20);
            }
            if (cycle->screens == 0 || cycle->screens == 2) {
                GXS_LoadBGPltt(cycle->cyclePltt + cycle->index * 16, 0, 0x20);
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

static void NintendoWfcUnused_AddWindows(NintendoWfcUnused *wfc) {
    AddWindowParameterized(wfc->bgConfig, &wfc->errorWindow, GF_BG_LYR_MAIN_0, 4, 4, 23, 16, 13, 0xA4);
    FillWindowPixelBuffer(&wfc->errorWindow, 0);
    AddWindowParameterized(wfc->bgConfig, &wfc->roomWindow, GF_BG_LYR_MAIN_0, 26, 14, 4, 2, 13, 0x94);
    FillWindowPixelBuffer(&wfc->roomWindow, 15);
    AddWindowParameterized(wfc->bgConfig, &wfc->rankWindow, GF_BG_LYR_MAIN_0, 26, 14, 4, 2, 13, 0x9C);
    FillWindowPixelBuffer(&wfc->rankWindow, 15);
    AddWindowParameterized(wfc->bgConfig, &wfc->titleWindow, GF_BG_LYR_MAIN_0, 4, 1, 24, 2, 13, 0x214);
    FillWindowPixelBuffer(&wfc->titleWindow, 0);
    NintendoWfcUnused_PrintAlignedText(&wfc->titleWindow, wfc->titleString, 0, 1, 1, MAKE_TEXT_COLOR(15, 14, 0));
    AddWindowParameterized(wfc->bgConfig, &wfc->msgWindow, GF_BG_LYR_MAIN_0, 2, 19, 27, 4, 13, 0x28);
    FillWindowPixelBuffer(&wfc->msgWindow, 15);
}

static void NintendoWfcUnused_RemoveWindows(NintendoWfcUnused *wfc) {
    RemoveWindow(&wfc->msgWindow);
    RemoveWindow(&wfc->titleWindow);
    RemoveWindow(&wfc->rankWindow);
    RemoveWindow(&wfc->roomWindow);
    RemoveWindow(&wfc->errorWindow);
}

static void NintendoWfcUnused_InitStrings(NintendoWfcUnused *wfc) {
    wfc->msgString = String_New(180, HEAP_ID_150);
    wfc->roomString = String_New(4, HEAP_ID_150);
    wfc->rankString = String_New(3, HEAP_ID_150);
    wfc->titleString = NewString_ReadMsgData(wfc->msgDataTower, msg_0799_00010);
    wfc->errorString = String_New(256, HEAP_ID_150);
}

static void NintendoWfcUnused_FreeStrings(NintendoWfcUnused *wfc) {
    String_Delete(wfc->errorString);
    String_Delete(wfc->titleString);
    String_Delete(wfc->rankString);
    String_Delete(wfc->roomString);
    String_Delete(wfc->msgString);
}

int NintendoWfcUnused_AskConnect(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataError, msg_0800_00017, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_OPEN_YES_NO_PROMPT, NINTENDO_WFC_UNUSED_STATE_HANDLE_CONNECT_CHOICE);
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_HandleConnectChoice(NintendoWfcUnused *wfc) {
    int response = NintendoWfcUnused_HandleYesNoPrompt(wfc->yesNoPrompt);
    if (response) {
        if (response == 2) {
            CommState_FreeWfcSession();
            NintendoWfcUnused_SetNextMode(wfc, 0, 0);
            wfc->state = NINTENDO_WFC_UNUSED_STATE_START_EXIT;
        } else {
            NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataWfc, msg_0778_00001, 1, 0xF0F);
            NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_WAIT_MESSAGE, NINTENDO_WFC_UNUSED_STATE_START_CONNECT);
            NintendoWfcUnused_ShowWaitingIcon(wfc);
        }
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_AskLogOut(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataWfc, msg_0778_00012, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_OPEN_YES_NO_PROMPT, NINTENDO_WFC_UNUSED_STATE_HANDLE_LOG_OUT_CHOICE);
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_HandleLogOutChoice(NintendoWfcUnused *wfc) {
    int response = NintendoWfcUnused_HandleYesNoPrompt(wfc->yesNoPrompt);
    if (response != 0) {
        if (response == 2) {
            if (DWC_CheckInet() == 0) {
                wfc->state = NINTENDO_WFC_UNUSED_STATE_ASK_CONNECT;
            } else {
                wfc->state = NINTENDO_WFC_UNUSED_STATE_PRINT_DISCONNECTING;
            }
        } else {
            if (DWC_CheckInet() == 0) {
                NintendoWfcUnused_SetNextMode(wfc, 0, 0);
                wfc->state = NINTENDO_WFC_UNUSED_STATE_START_EXIT;
            } else {
                wfc->state = NINTENDO_WFC_UNUSED_STATE_PRINT_DISCONNECTING;
            }
            wfc->args->incomplete = 1;
        }
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

void NintendoWfcUnused_StartConnect(NintendoWfcUnused *wfc) {
    DWC_InitInetEx(wfc->connection, 2, 1, 20);
    DWC_SetAuthServer(2);
    DWC_ConnectInetAsync();
    wfc->state = NINTENDO_WFC_UNUSED_STATE_WAIT_CONNECT;
}

int NintendoWfcUnused_WaitConnect(NintendoWfcUnused *wfc) {
    int errorCode, ignored;
    u8 info[60];

    DWC_ProcessInet();
    if (DWC_CheckInet()) {
        switch (DWC_GetInetStatus()) {
        case 7:
            wfc->dwcError = DWC_GetLastError(&errorCode);
            wfc->dwcErrorCode = errorCode;
            DWC_ClearError();
            DWC_CleanupInet();
            NintendoWfcUnused_HideWaitingIcon(wfc);
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_DWC_ERROR;
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
        case 8:
        default:
            DWC_GetLastError(&ignored);
            NintendoWfcUnused_HideWaitingIcon(wfc);
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            wfc->httpResult = -2;
            break;
        case 4:
            DWC_GetApInfo(info);
            wfc->state = NINTENDO_WFC_UNUSED_STATE_START_AUTH;
            break;
        }
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_StartAuth(NintendoWfcUnused *wfc) {
    DWC_NASLoginAsync();
    wfc->state = NINTENDO_WFC_UNUSED_STATE_WAIT_AUTH;
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_WaitAuth(NintendoWfcUnused *wfc) {
    int errorCode, errorKind;
    switch (DWC_NASLoginProcess()) {
    case 3:
        wfc->state = NINTENDO_WFC_UNUSED_STATE_INIT_HTTP;
        break;
    case 0:
    case 4:
    case 5:
        NintendoWfcUnused_HideWaitingIcon(wfc);
        wfc->dwcError = DWC_GetLastErrorEx(&errorCode, &errorKind);
        wfc->dwcErrorCode = errorCode;
        DWC_ClearError();
        DWC_CleanupInet();
        wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_DWC_ERROR;
        switch (errorKind) {
        case 1:
        case 2:
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_DWC_ERROR;
            break;
        case 4:
            DWC_ShutdownGHTTP();
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_DWC_ERROR;
            break;
        case 6:
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_DWC_ERROR;
            break;
        case 3:
            DWC_ShutdownFriendsMatch();
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_DWC_ERROR;
            break;
        case 5:
        case 7:
            CommState_ShowRestartErrorAndHalt();
            break;
        }
        if (errorCode < -20000 && errorCode >= -29999) {
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_DWC_ERROR;
        }
        break;
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_InitHttp(NintendoWfcUnused *wfc) {
    if (Save_SysInfo_GetDwcProfileId(wfc->args->sysInfo) == 0) {
        Save_SysInfo_SetDwcProfileId(wfc->args->sysInfo, (u32)wfc->args->dwcProfileId);
    }
    u32 profileId = Save_SysInfo_GetDwcProfileId(wfc->args->sysInfo);
    u64 friendKey = DWC_CreateFriendKey(wfc->args->dwcUserData);
    NintendoWfcHttp_Init(profileId, (u32)friendKey, (u32)(friendKey >> 32));
    wfc->state = NINTENDO_WFC_UNUSED_STATE_REQUEST_INFO;
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_RequestInfo(NintendoWfcUnused *wfc) {
    NintendoWfcHttp_RequestInfo();
    wfc->state = NINTENDO_WFC_UNUSED_STATE_WAIT_INFO;
    wfc->timeoutCounter = 0;
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_WaitInfo(NintendoWfcUnused *wfc) {
    if (NintendoWfcHttp_IsDone()) {
        int result = NintendoWfcHttp_GetResult();
        wfc->timeoutCounter = 0;
        switch (result) {
        case 0:
            NintendoWfcUnused_HideWaitingIcon(wfc);
            switch (wfc->args->mode) {
            case 0:
                NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_REQUEST_ROOM_COUNT, NINTENDO_WFC_UNUSED_STATE_ASK_CHALLENGE_ROOM);
                break;
            case 1:
                wfc->state = NINTENDO_WFC_UNUSED_STATE_PRINT_SENDING_RECORD;
                break;
            case 2:
                wfc->state = NINTENDO_WFC_UNUSED_STATE_ASK_LEADERS_RANK;
                break;
            }
            break;
        case 1:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -1:
        case 2:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -7:
        case -4:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -6:
        case -2:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        default:
            NintendoWfcUnused_HideWaitingIcon(wfc);
            CommState_ShowRestartErrorAndHalt();
            break;
        }
    } else {
        wfc->timeoutCounter++;
        if (wfc->timeoutCounter == 3600) {
            CommState_ShowRestartErrorAndHalt();
        }
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_RequestSetProfile(NintendoWfcUnused *wfc) {
    Save_WiiMessage_GetServerProfile(wfc->args->saveData, &wfc->profile);
    NintendoWfcHttp_RequestSetProfile(&wfc->profile, wfc->profileResponse);
    wfc->state = NINTENDO_WFC_UNUSED_STATE_WAIT_SET_PROFILE;
    wfc->timeoutCounter = 0;
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_WaitSetProfile(NintendoWfcUnused *wfc) {
    if (NintendoWfcHttp_IsDone()) {
        int result = NintendoWfcHttp_GetResult();
        wfc->timeoutCounter = 0;
        switch (result) {
        case 0:
            NintendoWfcUnused_HideWaitingIcon(wfc);
            switch (wfc->profileResponse[0]) {
            case 0:
                switch (wfc->profileResponse[1]) {
                case 0:
                    switch (wfc->args->mode) {
                    case 0:
                        NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_REQUEST_ROOM_COUNT, NINTENDO_WFC_UNUSED_STATE_ASK_CHALLENGE_ROOM);
                        break;
                    case 1:
                        wfc->state = NINTENDO_WFC_UNUSED_STATE_PRINT_SENDING_RECORD;
                        break;
                    case 2:
                        wfc->state = NINTENDO_WFC_UNUSED_STATE_ASK_LEADERS_RANK;
                        break;
                    }
                    break;
                case 1:
                case 2:
                case 3:
                    wfc->httpResult = result;
                    wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
                    NintendoWfcUnused_HideWaitingIcon(wfc);
                    break;
                }
                break;
            case 1:
            case 2:
                wfc->httpResult = result;
                wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
                NintendoWfcUnused_HideWaitingIcon(wfc);
                break;
            default:
                NintendoWfcUnused_HideWaitingIcon(wfc);
                CommState_ShowRestartErrorAndHalt();
                break;
            }
            break;
        case 1:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -1:
        case 2:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -7:
        case -4:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -6:
        case -2:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        default:
            NintendoWfcUnused_HideWaitingIcon(wfc);
            CommState_ShowRestartErrorAndHalt();
            break;
        }
    } else {
        wfc->timeoutCounter++;
        if (wfc->timeoutCounter == 3600) {
            CommState_ShowRestartErrorAndHalt();
        }
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_RequestRoomCount(NintendoWfcUnused *wfc) {
    NintendoWfcHttp_RequestRoomCount(wfc->rank);
    wfc->state = NINTENDO_WFC_UNUSED_STATE_WAIT_ROOM_COUNT;
    wfc->timeoutCounter = 0;
    NintendoWfcUnused_ShowWaitingIcon(wfc);
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_WaitRoomCount(NintendoWfcUnused *wfc) {
    if (NintendoWfcHttp_IsDone()) {
        int result = NintendoWfcHttp_GetResult();
        wfc->timeoutCounter = 0;
        switch (result) {
        case -1:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            break;
        case -2:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            break;
        case -3:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            break;
        case -6:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            break;
        case -7:
        case -4:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            break;
        case -5:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            CommState_ShowRestartErrorAndHalt();
            break;
        default:
            if (result > 0) {
                wfc->roomCount = result;
                wfc->state = wfc->nextState;
            }
            break;
        }
        NintendoWfcUnused_HideWaitingIcon(wfc);
    } else {
        wfc->timeoutCounter++;
        if (wfc->timeoutCounter == 3600) {
            CommState_ShowRestartErrorAndHalt();
        }
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_AskChallengeRoom(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, msg_0799_00000, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_WAIT_MESSAGE, NINTENDO_WFC_UNUSED_STATE_START_CHALLENGE_ROOM_INPUT);
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_StartChallengeRoomInput(NintendoWfcUnused *wfc) {
    DrawFrameAndWindow1(&wfc->roomWindow, FALSE, 31, 14);
    NintendoWfcUnused_PrintNumber(&wfc->roomWindow, wfc->roomString, wfc->room, 3);
    NintendoWfcUnused_SetArrowsVisible(wfc, 0, TRUE);
    CopyWindowToVram(&wfc->roomWindow);
    wfc->state = NINTENDO_WFC_UNUSED_STATE_HANDLE_CHALLENGE_ROOM_INPUT;
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

// Chooses the room with UP/DOWN; A: download it (or show it if it was downloaded already), B: back.
int NintendoWfcUnused_HandleChallengeRoomInput(NintendoWfcUnused *wfc) {
    u32 time[3];
    u32 date[4];

    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        wfc->room++;
        if ((int)wfc->room > wfc->roomCount) {
            wfc->room = 1;
        }
        NintendoWfcUnused_PrintNumber(&wfc->roomWindow, wfc->roomString, wfc->room, 3);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        wfc->room--;
        if ((int)wfc->room < 1) {
            wfc->room = wfc->roomCount;
        }
        NintendoWfcUnused_PrintNumber(&wfc->roomWindow, wfc->roomString, wfc->room, 3);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        DWC_GetDateTime(date, time);
        // BattleRoomDownload_IsRoomMarked takes u32 group/entry; they are passed as u8 values
        if (BattleRoomDownload_IsRoomMarked((BattleRoomDownload *)wfc->args->battleRoomDownload, (u8)wfc->rank, (u8)wfc->room, date)) {
            wfc->state = NINTENDO_WFC_UNUSED_STATE_PRINT_ROOM_UNAVAILABLE;
        } else {
            wfc->state = NINTENDO_WFC_UNUSED_STATE_PRINT_RECEIVING_TRAINERS;
        }
        NintendoWfcUnused_SetArrowsVisible(wfc, 0, FALSE);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        NintendoWfcUnused_SetArrowsVisible(wfc, 0, FALSE);
        wfc->state = NINTENDO_WFC_UNUSED_STATE_ASK_CANCEL_CHALLENGE;
        PlaySE(SEQ_SE_DP_SELECT);
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_PrintReceivingTrainers(NintendoWfcUnused *wfc) {
    BufferIntegerAsString(wfc->msgFormat, 0, wfc->room, 3, PRINTING_MODE_LEADING_ZEROS, TRUE);
    BufferIntegerAsString(wfc->msgFormat, 1, wfc->rank, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
    NintendoWfcUnused_PrintFormattedMessage(wfc, wfc->msgDataTower, msg_0799_00001, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_WAIT_MESSAGE, NINTENDO_WFC_UNUSED_STATE_REQUEST_TRAINERS_DOWNLOAD);
    MIi_CpuClearFast(0, (u32 *)wfc->downloadBuffer, sizeof(wfc->downloadBuffer));
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_PrintRoomUnavailable(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintFormattedMessage(wfc, wfc->msgDataTower, msg_0799_00002, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_WAIT_MESSAGE, NINTENDO_WFC_UNUSED_STATE_ASK_CHALLENGE_ROOM);
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_RequestTrainersDownload(NintendoWfcUnused *wfc) {
    NintendoWfcHttp_RequestDownload(wfc->ownRank, wfc->room, wfc->downloadBuffer);
    NintendoWfcUnused_ShowWaitingIcon(wfc);
    wfc->state = NINTENDO_WFC_UNUSED_STATE_WAIT_TRAINERS_DOWNLOAD;
    wfc->timeoutCounter = 0;
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_WaitTrainersDownload(NintendoWfcUnused *wfc) {
    int result;
    u32 time[3];
    u32 date[4];

    if (NintendoWfcHttp_IsDone()) {
        result = NintendoWfcHttp_GetResult();
        wfc->timeoutCounter = 0;
        switch (result) {
        case 0:
            // BattleRoomDownload_SetTrainers / BattleRoomDownload_MarkRoom take u32; rank and room are passed as u8 values
            BattleRoomDownload_SetTrainers((BattleRoomDownload *)wfc->args->battleRoomDownload, wfc->downloadBuffer, (u8)wfc->ownRank, (u8)wfc->room);
            NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_START_SAVE, NINTENDO_WFC_UNUSED_STATE_PRINT_TRAINERS_RECEIVED);
            DWC_GetDateTime(date, time);
            BattleRoomDownload_MarkRoom((BattleRoomDownload *)wfc->args->battleRoomDownload, (u8)wfc->rank, (u8)wfc->room, date);
            break;
        case -1:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -2:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -3:
        case -4:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_REQUEST_ROOM_COUNT;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -6:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -7:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -5:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            CommState_ShowRestartErrorAndHalt();
            break;
        }
    } else {
        wfc->timeoutCounter++;
        if (wfc->timeoutCounter == 3600) {
            CommState_ShowRestartErrorAndHalt();
        }
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_PrintTrainersReceived(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, msg_0799_00003, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_WAIT_MESSAGE, NINTENDO_WFC_UNUSED_STATE_START_DISCONNECT);
    NintendoWfcUnused_HideWaitingIcon(wfc);
    wfc->args->incomplete = 0;
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_PrintSendingRecord(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, msg_0799_00007, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_WAIT_MESSAGE, NINTENDO_WFC_UNUSED_STATE_REQUEST_UPLOAD);
    NintendoWfcUnused_ShowWaitingIcon(wfc);
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_RequestUpload(NintendoWfcUnused *wfc) {
    int record = FrontierData_GetWifiWinCount(wfc->args->frontierData);
    u8 rankRoom[4];
    BattleRoomDownload_GetTrainersRoom((BattleRoomDownload *)wfc->args->battleRoomDownload, rankRoom);
    Frontier_FillWifiUploadRecord(wfc->args->saveData, 1, &wfc->uploadRecord);
    NintendoWfcHttp_RequestUpload(rankRoom[0], rankRoom[1], record, &wfc->uploadRecord);
    wfc->state = NINTENDO_WFC_UNUSED_STATE_WAIT_UPLOAD;
    wfc->timeoutCounter = 0;
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_WaitUpload(NintendoWfcUnused *wfc) {
    if (NintendoWfcHttp_IsDone()) {
        int result = NintendoWfcHttp_GetResult();
        wfc->timeoutCounter = 0;
        switch (result) {
        case 0:
            FrontierData_FlagAction(wfc->args->frontierData, 5, 2);
            NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_START_SAVE, NINTENDO_WFC_UNUSED_STATE_PRINT_RECORD_SENT);
            break;
        case -1:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -2:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -3:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -6:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -7:
        case -4:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -5:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            CommState_ShowRestartErrorAndHalt();
            break;
        }
    } else {
        wfc->timeoutCounter++;
        if (wfc->timeoutCounter == 3600) {
            CommState_ShowRestartErrorAndHalt();
        }
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_PrintRecordSent(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, msg_0799_00008, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_WAIT_MESSAGE, NINTENDO_WFC_UNUSED_STATE_START_DISCONNECT);
    NintendoWfcUnused_HideWaitingIcon(wfc);
    wfc->args->incomplete = 0;
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_AskLeadersRank(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, msg_0799_00005, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_WAIT_MESSAGE, NINTENDO_WFC_UNUSED_STATE_START_LEADERS_RANK_INPUT);
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_StartLeadersRankInput(NintendoWfcUnused *wfc) {
    wfc->rank = 1;
    DrawFrameAndWindow1(&wfc->rankWindow, FALSE, 31, 14);
    NintendoWfcUnused_PrintNumber(&wfc->rankWindow, wfc->rankString, wfc->rank, 2);
    NintendoWfcUnused_SetArrowsVisible(wfc, 1, TRUE);
    CopyWindowToVram(&wfc->rankWindow);
    wfc->state = NINTENDO_WFC_UNUSED_STATE_HANDLE_LEADERS_RANK_INPUT;
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

// Chooses the rank (1-10) with UP/DOWN; A: continue, B: back.
int NintendoWfcUnused_HandleLeadersRankInput(NintendoWfcUnused *wfc) {
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        wfc->rank++;
        if (*(volatile int *)&wfc->rank > 10) {
            wfc->rank = 1;
        }
        NintendoWfcUnused_PrintNumber(&wfc->rankWindow, wfc->rankString, wfc->rank, 2);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        wfc->rank--;
        if (*(volatile int *)&wfc->rank < 1) {
            wfc->rank = 10;
        }
        NintendoWfcUnused_PrintNumber(&wfc->rankWindow, wfc->rankString, wfc->rank, 2);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        NintendoWfcUnused_SetArrowsVisible(wfc, 0, FALSE);
        NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_REQUEST_ROOM_COUNT, NINTENDO_WFC_UNUSED_STATE_ASK_LEADERS_ROOM);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        NintendoWfcUnused_SetArrowsVisible(wfc, 0, FALSE);
        wfc->state = NINTENDO_WFC_UNUSED_STATE_ASK_CANCEL_DOWNLOAD;
        PlaySE(SEQ_SE_DP_SELECT);
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_AskLeadersRoom(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, msg_0799_00006, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_WAIT_MESSAGE, NINTENDO_WFC_UNUSED_STATE_START_LEADERS_ROOM_INPUT);
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_StartLeadersRoomInput(NintendoWfcUnused *wfc) {
    wfc->room = 1;
    DrawFrameAndWindow1(&wfc->roomWindow, FALSE, 31, 14);
    NintendoWfcUnused_PrintNumber(&wfc->roomWindow, wfc->roomString, wfc->room, 3);
    NintendoWfcUnused_SetArrowsVisible(wfc, 0, TRUE);
    CopyWindowToVram(&wfc->roomWindow);
    wfc->state = NINTENDO_WFC_UNUSED_STATE_HANDLE_LEADERS_ROOM_INPUT;
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

// Chooses the room (1-roomCount) with UP/DOWN; A: continue, B: back.
int NintendoWfcUnused_HandleLeadersRoomInput(NintendoWfcUnused *wfc) {
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        wfc->room++;
        if (wfc->room == wfc->roomCount + 1) {
            wfc->room = 1;
        }
        NintendoWfcUnused_PrintNumber(&wfc->roomWindow, wfc->roomString, wfc->room, 3);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        wfc->room--;
        if (*(volatile int *)&wfc->room < 1) {
            wfc->room = wfc->roomCount;
        }
        NintendoWfcUnused_PrintNumber(&wfc->roomWindow, wfc->roomString, wfc->room, 3);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        NintendoWfcUnused_SetArrowsVisible(wfc, 0, FALSE);
        wfc->state = NINTENDO_WFC_UNUSED_STATE_PRINT_RECEIVING_LEADERS;
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        NintendoWfcUnused_SetArrowsVisible(wfc, 0, FALSE);
        wfc->state = NINTENDO_WFC_UNUSED_STATE_ASK_LEADERS_RANK;
        PlaySE(SEQ_SE_DP_SELECT);
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_PrintReceivingLeaders(NintendoWfcUnused *wfc) {
    BufferIntegerAsString(wfc->msgFormat, 0, wfc->room, 3, PRINTING_MODE_LEADING_ZEROS, TRUE);
    BufferIntegerAsString(wfc->msgFormat, 1, wfc->rank, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
    NintendoWfcUnused_PrintFormattedMessage(wfc, wfc->msgDataTower, msg_0799_00001, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_WAIT_MESSAGE, NINTENDO_WFC_UNUSED_STATE_REQUEST_LEADERS_DOWNLOAD);
    wfc->state = NINTENDO_WFC_UNUSED_STATE_REQUEST_LEADERS_DOWNLOAD;
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_RequestLeadersDownload(NintendoWfcUnused *wfc) {
    NintendoWfcHttp_RequestDownload(wfc->rank, wfc->room, wfc->downloadBuffer);
    wfc->state = NINTENDO_WFC_UNUSED_STATE_WAIT_LEADERS_DOWNLOAD;
    wfc->timeoutCounter = 0;
    NintendoWfcUnused_ShowWaitingIcon(wfc);
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_WaitLeadersDownload(NintendoWfcUnused *wfc) {
    if (NintendoWfcHttp_IsDone()) {
        int result = NintendoWfcHttp_GetResult();
        wfc->timeoutCounter = 0;
        switch (result) {
        case 0:
            // BattleRoomDownload_SetLeaders takes u32; rank and room are passed as u8 values
            BattleRoomDownload_SetLeaders((BattleRoomDownload *)wfc->args->battleRoomDownload, wfc->downloadBuffer + 0x63C, (u8)wfc->rank, (u8)wfc->room);
            NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_START_SAVE, NINTENDO_WFC_UNUSED_STATE_PRINT_LEADERS_RECEIVED);
            break;
        case -1:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -2:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -3:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_REQUEST_ROOM_COUNT;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -6:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -4:
        case -7:
            wfc->state = NINTENDO_WFC_UNUSED_STATE_REQUEST_ROOM_COUNT;
            wfc->httpResult = result;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -5:
            wfc->httpResult = result;
            wfc->state = NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            CommState_ShowRestartErrorAndHalt();
            break;
        }
    } else {
        if (++wfc->timeoutCounter == 3600) {
            CommState_ShowRestartErrorAndHalt();
        }
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_PrintLeadersReceived(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, msg_0799_00003, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_WAIT_MESSAGE, NINTENDO_WFC_UNUSED_STATE_START_DISCONNECT);
    NintendoWfcUnused_HideWaitingIcon(wfc);
    wfc->args->incomplete = 0;
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_StartDisconnect(NintendoWfcUnused *wfc) {
    wfc->state = NINTENDO_WFC_UNUSED_STATE_PRINT_DISCONNECTING;
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_StartSave(NintendoWfcUnused *wfc) {
    Save_PrepareForAsyncWrite(wfc->args->saveData, 2);
    wfc->state = NINTENDO_WFC_UNUSED_STATE_SAVE;
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_Save(NintendoWfcUnused *wfc) {
    if (Save_WriteFileAsync(wfc->args->saveData) == 2) {
        wfc->state = wfc->nextState;
        NintendoWfcUnused_HideWaitingIcon(wfc);
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_AskCancelChallenge(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, msg_0799_00004, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_OPEN_YES_NO_PROMPT, NINTENDO_WFC_UNUSED_STATE_HANDLE_CANCEL_CHALLENGE_CHOICE);
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_HandleCancelChallengeChoice(NintendoWfcUnused *wfc) {
    int response = NintendoWfcUnused_HandleYesNoPrompt(wfc->yesNoPrompt);
    if (response) {
        if (response == 2) {
            wfc->state = NINTENDO_WFC_UNUSED_STATE_ASK_CHALLENGE_ROOM;
        } else {
            wfc->state = NINTENDO_WFC_UNUSED_STATE_PRINT_DISCONNECTING;
            wfc->args->incomplete = 1;
        }
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_AskCancelDownload(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, msg_0799_00009, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_OPEN_YES_NO_PROMPT, NINTENDO_WFC_UNUSED_STATE_HANDLE_CANCEL_DOWNLOAD_CHOICE);
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_HandleCancelDownloadChoice(NintendoWfcUnused *wfc) {
    int response = NintendoWfcUnused_HandleYesNoPrompt(wfc->yesNoPrompt);
    if (response) {
        if (response == 2) {
            wfc->state = NINTENDO_WFC_UNUSED_STATE_ASK_LEADERS_RANK;
        } else {
            wfc->state = NINTENDO_WFC_UNUSED_STATE_PRINT_DISCONNECTING;
            wfc->args->incomplete = 1;
        }
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_PrintDisconnecting(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataError, msg_0800_00026, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_WAIT_MESSAGE, NINTENDO_WFC_UNUSED_STATE_CLOSE_CONNECTION);
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_CloseConnection(NintendoWfcUnused *wfc) {
    DWC_CleanupInet();
    NintendoWfcUnused_SetNextMode(wfc, 0, 0);
    wfc->state = NINTENDO_WFC_UNUSED_STATE_PRINT_DISCONNECTED;
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_PrintDisconnected(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataError, msg_0800_00027, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_WAIT_MESSAGE_AND_DELAY, NINTENDO_WFC_UNUSED_STATE_START_EXIT);
    wfc->messageTimer = 0;
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_UnusedState_Idle(void) {
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_StartExit(NintendoWfcUnused *wfc) {
    WirelessStatusIcon_Destroy();
    CommState_FreeWfcSession();
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 8, 1, HEAP_ID_150);
    wfc->state = NINTENDO_WFC_UNUSED_STATE_ASK_CONNECT;
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_EXIT;
}

int NintendoWfcUnused_UnusedState_HandleYesNo(NintendoWfcUnused *wfc) {
    int response = NintendoWfcUnused_HandleYesNoPrompt(wfc->yesNoPrompt);
    if (response) {
        if (response == 2) {
            wfc->state = NINTENDO_WFC_UNUSED_STATE_ASK_CONNECT;
        } else {
            wfc->state = NINTENDO_WFC_UNUSED_STATE_PRINT_DISCONNECTING;
        }
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_ShowHttpError(NintendoWfcUnused *wfc) {
    int msgNo = msg_0799_00000;
    switch (wfc->httpResult) {
    case -6:
    case -2:
        msgNo = msg_0799_00013;
        break;
    case 1:
        msgNo = msg_0799_00011;
        break;
    case -1:
    case 2:
        msgNo = msg_0799_00012;
        break;
    case -7:
    case -4:
        msgNo = msg_0799_00014;
        break;
    case -3:
        msgNo = msg_0799_00015;
        break;
    }
    NintendoWfcUnused_HideWaitingIcon(wfc);
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, msgNo, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, NINTENDO_WFC_UNUSED_STATE_WAIT_MESSAGE, NINTENDO_WFC_UNUSED_STATE_DISCONNECT);
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_Disconnect(NintendoWfcUnused *wfc) {
    switch (wfc->subState) {
    case 0:
        NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, msg_0799_00016, 1, 0xF0F);
        wfc->subState++;
        break;
    case 1:
        if (!TextPrinterCheckActive((u8)wfc->textPrinterId)) {
            CommState_FreeWfcSession();
            DWC_CleanupInet();
            wfc->subState++;
        }
        break;
    case 2:
        NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, msg_0799_00017, 1, 0xF0F);
        wfc->subState++;
        break;
    case 3:
        if (!TextPrinterCheckActive((u8)wfc->textPrinterId)) {
            wfc->subState++;
        }
        break;
    default:
        wfc->subTimer++;
        if (wfc->subTimer > 30) {
            NintendoWfcUnused_SetNextMode(wfc, 0, 0);
            wfc->state = NINTENDO_WFC_UNUSED_STATE_START_EXIT;
        }
        break;
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_WaitMessage(NintendoWfcUnused *wfc) {
    if (!TextPrinterCheckActive((u8)wfc->textPrinterId)) {
        wfc->state = wfc->nextState;
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_WaitMessageAndDelay(NintendoWfcUnused *wfc) {
    if (!TextPrinterCheckActive((u8)wfc->textPrinterId)) {
        if (wfc->messageTimer > 30) {
            wfc->state = wfc->nextState;
        }
        ++wfc->messageTimer;
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_OpenYesNoPrompt(NintendoWfcUnused *wfc) {
    if (!TextPrinterCheckActive((u8)wfc->textPrinterId)) {
        wfc->yesNoPrompt = NintendoWfcUnused_CreateYesNoPrompt(wfc->bgConfig, 10, 0x244);
        wfc->state = wfc->nextState;
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

static void NintendoWfcUnused_PrintFormattedMessage(NintendoWfcUnused *wfc, MsgData *msgData, int msgNo, int textSpeed, int unused) {
    String *string = NewString_ReadMsgData(msgData, msgNo);
    StringExpandPlaceholders(wfc->msgFormat, wfc->msgString, string);
    FillWindowPixelBuffer(&wfc->msgWindow, 15);
    DrawFrameAndWindow2(&wfc->msgWindow, FALSE, 1, 14);
    wfc->textPrinterId = AddTextPrinterParameterized(&wfc->msgWindow, 1, wfc->msgString, 0, 0, textSpeed, NULL);
    String_Delete(string);
}

int NintendoWfcUnused_ShowDwcError(NintendoWfcUnused *wfc) {
    int msgNo = RecordMixing_State_AbortLeave(-wfc->dwcErrorCode, wfc->dwcErrorType);
    NintendoWfcUnused_ShowErrorMessage(wfc, msgNo, -wfc->dwcErrorCode);
    wfc->state = NINTENDO_WFC_UNUSED_STATE_WAIT_ERROR_DISMISS;
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

int NintendoWfcUnused_WaitErrorDismiss(NintendoWfcUnused *wfc) {
    if ((gSystem.newKeys & PAD_BUTTON_A) || (gSystem.newKeys & PAD_BUTTON_B)) {
        EraseFrameAndWindow1(&wfc->errorWindow, FALSE);
        wfc->state = NINTENDO_WFC_UNUSED_STATE_ASK_CONNECT;
    }
    return NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
}

static void NintendoWfcUnused_PrintMessage(NintendoWfcUnused *wfc, MsgData *msgData, int msgNo, int textSpeed, int unused) {
    ReadMsgDataIntoString(msgData, msgNo, wfc->msgString);
    FillWindowPixelBuffer(&wfc->msgWindow, 15);
    DrawFrameAndWindow2(&wfc->msgWindow, FALSE, 1, 14);
    wfc->textPrinterId = AddTextPrinterParameterized(&wfc->msgWindow, 1, wfc->msgString, 0, 0, textSpeed, NULL);
}

static int NintendoWfcUnused_GetAlignedTextX(const Window *window, String *string, int x, int alignment, int unused, int fontId) {
    int width;
    switch (alignment) {
    case 1:
        width = FontID_String_GetWidth(fontId, string, 0);
        x = ((window->width << 3) - width) / 2;
        break;
    case 2:
        width = FontID_String_GetWidth(fontId, string, 0);
        x = (window->width << 3) - width;
        break;
    }
    return x;
}

static void NintendoWfcUnused_PrintAlignedText(Window *window, String *string, int x, int y, int alignment, u32 color) {
    x = NintendoWfcUnused_GetAlignedTextX(window, string, x, alignment, color, 1);
    AddTextPrinterParameterizedWithColor(window, 1, string, x, y, TEXT_SPEED_INSTANT, color, NULL);
}

static void NintendoWfcUnused_PrintAlignedTextSmall(Window *window, String *string, int x, int y, int alignment, u32 color) {
    x = NintendoWfcUnused_GetAlignedTextX(window, string, x, alignment, color, 0);
    AddTextPrinterParameterizedWithColor(window, 0, string, x, y, TEXT_SPEED_INSTANT, color, NULL);
}

static void NintendoWfcUnused_PrintNumber(Window *window, String *string, int number, int digits) {
    FillWindowPixelBuffer(window, 15);
    String16_FormatInteger(string, number, digits, PRINTING_MODE_LEADING_ZEROS, TRUE);
    NintendoWfcUnused_PrintAlignedTextSmall(window, string, 0, 1, 1, MAKE_TEXT_COLOR(1, 2, 0));
}

static void NintendoWfcUnused_CreateWirelessIcon(NintendoWfcUnused *wfc) {
    WirelessStatusIcon_Create();
}

static void NintendoWfcUnused_SetArrowsVisible(NintendoWfcUnused *wfc, u32 which, BOOL visible) {
    if (which <= 1) {
        Sprite_SetDrawFlag(wfc->arrows[0], visible);
        Sprite_SetDrawFlag(wfc->arrows[1], visible);
    }
}

static void NintendoWfcUnused_ShowWaitingIcon(NintendoWfcUnused *wfc) {
    if (!wfc->waitingIcon) {
        wfc->waitingIcon = WaitingIcon_New(&wfc->msgWindow, 1);
    }
}

static void NintendoWfcUnused_HideWaitingIcon(NintendoWfcUnused *wfc) {
    if (wfc->waitingIcon) {
        WaitingIcon_Delete(wfc->waitingIcon);
        wfc->waitingIcon = NULL;
    }
}

static void NintendoWfcUnused_PrintErrorMessage(NintendoWfcUnused *wfc, int msgNo) {
    String *string = String_New(256, HEAP_ID_150);

    ReadMsgDataIntoString(wfc->msgDataError, msgNo, string);
    StringExpandPlaceholders(wfc->msgFormat, wfc->errorString, string);
    FillWindowPixelBuffer(&wfc->errorWindow, 15);
    DrawFrameAndWindow1(&wfc->errorWindow, TRUE, 31, 11);
    wfc->textPrinterId = AddTextPrinterParameterized(&wfc->errorWindow, 1, wfc->errorString, 0, 0, TEXT_SPEED_INSTANT, NULL);
    String_Delete(string);
}

static void NintendoWfcUnused_ShowErrorMessage(NintendoWfcUnused *wfc, int msgNo, int errorCode) {
    if (msgNo == -1) {
        msgNo = 11;
    }
    BufferIntegerAsString(wfc->msgFormat, 0, errorCode, 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
    ClearFrameAndWindow2(&wfc->msgWindow, TRUE);
    NintendoWfcUnused_PrintErrorMessage(wfc, msgNo);
}
