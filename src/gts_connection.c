#include "global.h"

#include "msgdata/msg.naix"

#include "bg_window.h"
#include "font.h"
#include "friend_roster.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "gts_internal.h"
#include "gts_server.h"
#include "heap.h"
#include "list_menu_2d.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "palette.h"
#include "render_window.h"
#include "sav_system_info.h"
#include "screen_fade.h"
#include "string_util.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "unk_0203170C.h"
#include "wireless_status_icon.h"
#include "wifi_connection.h"
#include "yes_no_prompt.h"

// Connecting to Nintendo Wi-Fi Connection (screen 0, the first and last screen of the GTS). Entered at the start it
// asks whether to connect (unless GtsArgs.unk3C says to connect directly), connects, authenticates, starts the GTS
// server client, asks info.asp and sends the profile (setProfile.asp), then opens the main menu (screen 1). Entered
// again from the GTS while connected it disconnects and leaves the app. Wi-Fi errors are shown with their error code,
// server errors with a GTS message and a disconnection.
// The text helpers every screen uses follow it.

typedef enum GtsConnectionState {
    GTS_CONNECTION_STATE_ASK_CONNECT,
    GTS_CONNECTION_STATE_HANDLE_CONNECT_PROMPT,
    GTS_CONNECTION_STATE_START_CONNECT,
    GTS_CONNECTION_STATE_WAIT_CONNECT,
    GTS_CONNECTION_STATE_START_AUTH,
    GTS_CONNECTION_STATE_WAIT_AUTH,
    GTS_CONNECTION_STATE_INIT_SERVER,
    GTS_CONNECTION_STATE_SEND_INFO,
    GTS_CONNECTION_STATE_WAIT_INFO,
    GTS_CONNECTION_STATE_SEND_SET_PROFILE,
    GTS_CONNECTION_STATE_WAIT_SET_PROFILE,
    GTS_CONNECTION_STATE_FADE_OUT,
    GTS_CONNECTION_STATE_WAIT_TEXT,
    GTS_CONNECTION_STATE_WAIT_TEXT_AND_SHOW_YES_NO,
    GTS_CONNECTION_STATE_HANDLE_EXIT_PROMPT,
    GTS_CONNECTION_STATE_ASK_DISCONNECT,
    GTS_CONNECTION_STATE_HANDLE_DISCONNECT_MENU,
    GTS_CONNECTION_STATE_PRINT_DISCONNECTING,
    GTS_CONNECTION_STATE_DISCONNECT,
    GTS_CONNECTION_STATE_PRINT_DISCONNECTED,
    GTS_CONNECTION_STATE_WAIT_TEXT_AND_DELAY,
    GTS_CONNECTION_STATE_PRINT_SERVER_ERROR,
    GTS_CONNECTION_STATE_DISCONNECT_AFTER_ERROR,
    GTS_CONNECTION_STATE_SHOW_CONNECTION_ERROR,
    GTS_CONNECTION_STATE_WAIT_CLOSE_ERROR,
} GtsConnectionState;

// The selector's tables of the letter groups (gts_selector.c): the first letter of each group, {first letter,
// letter count} of each group, and the same table read from its second word.
typedef struct GtsLetterGroup {
    int first;
    int count;
} GtsLetterGroup;

extern const int ov70_02245E84[];
extern const GtsLetterGroup ov70_02245F58[];
extern const int ov70_02245F5C[][2];

extern int (*const sGtsConnectionStates[])(GtsApp *app);
extern const BgTemplate sGtsConnectionBg0Template;
extern const BgTemplate sGtsConnectionBg1Template;
extern const BgTemplate sGtsConnectionSubBg0Template;
extern const BgTemplate sGtsConnectionSubBg1Template;
// GTS, GTS TRADE POKEMON, GTS SUMMARY, GTS DEPOSIT POKEMON, GTS SEEK POKEMON.
extern const u32 sGtsPanelCaptionMsgIds[];

// Network library (OVY_0), not declared in a public header.
int ov00_021EC5B4(void);
void ov00_021EC3F0(void *connection, int priority, int unused, int sslPriority);
void ov00_021EC454(int server);
void ov00_021EC4A4(void);
void ov00_021EC60C(void);
int ov00_021EC724(void);
int ov00_021EC11C(int *errorCode, int *errorType);
void ov00_021EC210(void);
void ov00_021EC8D8(void);
int ov00_021EC0FC(int *errorCode);
int ov00_021EC9E0(void *apInfo);
int ov00_021ECD04(void);
int ov00_021ECDC8(void);
void ov00_021FA0D8(void);
void ov00_021ED9B4(void);
u64 DWC_CreateFriendKey(void *userData);
// Not declared in a public header.
void sub_02039418(SaveData *saveData);
void sub_0203946C(void);
void sub_020399EC(void);
void WirelessStatusIcon_SetLevel(s32 level);

int GtsConnection_Init(GtsApp *app, int state);
int GtsConnection_Main(GtsApp *app, int state);
int GtsConnection_Exit(GtsApp *app, int state);
void GtsConnection_InitBgs(BgConfig *bgConfig);
void GtsConnection_FreeBgs(BgConfig *bgConfig);
void GtsConnection_LoadGraphics(GtsApp *app);
void GtsConnection_AnimatePaletteTask(SysTask *task, GtsPaletteAnim *anim);
void GtsConnection_CreateWindows(GtsApp *app);
void GtsConnection_RemoveWindows(GtsApp *app);
void GtsConnection_CreateStrings(GtsApp *app);
void GtsConnection_DeleteStrings(GtsApp *app);
int GtsConnection_AskConnect(GtsApp *app);
int GtsConnection_HandleConnectPrompt(GtsApp *app);
int GtsConnection_AskDisconnect(GtsApp *app);
int GtsConnection_HandleDisconnectMenu(GtsApp *app);
int GtsConnection_PrintDisconnecting(GtsApp *app);
int GtsConnection_Disconnect(GtsApp *app);
int GtsConnection_PrintDisconnected(GtsApp *app);
void GtsConnection_StartConnect(GtsApp *app);
int GtsConnection_WaitConnect(GtsApp *app);
int GtsConnection_StartAuth(GtsApp *app);
int GtsConnection_WaitAuth(GtsApp *app);
int GtsConnection_InitServer(GtsApp *app);
int GtsConnection_SendInfo(GtsApp *app);
int GtsConnection_WaitInfo(GtsApp *app);
int GtsConnection_SendSetProfile(GtsApp *app);
int GtsConnection_WaitSetProfile(GtsApp *app);
int GtsConnection_ShowConnectionError(GtsApp *app);
int GtsConnection_WaitCloseError(GtsApp *app);
int GtsConnection_FadeOut(GtsApp *app);
int GtsConnection_HandleExitPrompt(GtsApp *app);
int GtsConnection_PrintServerError(GtsApp *app);
int GtsConnection_DisconnectAfterError(GtsApp *app);
int GtsConnection_WaitText(GtsApp *app);
int GtsConnection_WaitTextAndDelay(GtsApp *app);
int GtsConnection_WaitTextAndShowYesNo(GtsApp *app);
int Gts_GetAlignedX(Window *window, String *string, int x, int align, u32 unused, u32 fontId);
void Gts_PrintAlignedFont1(Window *window, String *string, int x, int y, int align, u32 color);
void GtsConnection_PrintError(GtsApp *app, int msgId);
void GtsConnection_PrintErrorCode(GtsApp *app, int msgId, int errorCode);

// The last function of the selector (gts_selector.c), linked with screen 0.
BOOL GtsSelector_HasCountriesInGroup(GtsSelector *selector, int group) {
    int first = ov70_02245E84[group];
    int count = ov70_02245F5C[group][0];
    if (count > 0) {
        int i = 0;
        if (i < count) {
            do {
                if (GtsSelector_CountCountriesOfLetter(selector, first + i) > 0) {
                    return TRUE;
                }
                i++;
            } while (i < ov70_02245F58[group].count);
        }
    } else if (GtsSelector_CountCountriesOfLetter(selector, first) > 0) {
        return TRUE;
    }
    return FALSE;
}

int GtsConnection_Init(GtsApp *app, int state) {
    GtsArgs *args;

    GtsConnection_CreateStrings(app);
    reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_61);
    GtsConnection_InitBgs(app->bgConfig);
    GtsConnection_LoadGraphics(app);
    GtsConnection_CreateWindows(app);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    if (ov00_021EC5B4() == 0) {
        args = app->args;
        if (args->unk3C != 0) {
            // Connect without asking.
            sub_02039418(args->saveData);
            Gts_PrintMessage(app, app->wifiMsgData, 1, 1, 0xF0F); // Connecting to Nintendo Wi-Fi Connection...
            Gts_SetState(app, GTS_CONNECTION_STATE_WAIT_TEXT, GTS_CONNECTION_STATE_START_CONNECT);
            Gts_CreateWaitingIcon(app);
        } else {
            Sys_ClearSleepDisableFlag(4);
            app->state = GTS_CONNECTION_STATE_ASK_CONNECT;
        }
    } else {
        // Back from the GTS: disconnect.
        args = app->args;
        sub_02039418(args->saveData);
        Gts_CreateWirelessStatusIcon(app);
        app->state = GTS_CONNECTION_STATE_PRINT_DISCONNECTING;
    }
    return 2;
}

#pragma force_active on

int GtsConnection_Main(GtsApp *app, int state) {
    int prevState;
    int result;

    WirelessStatusIcon_SetLevel(Gts_GetWifiIconLevel());
    prevState = app->state;
    result = sGtsConnectionStates[prevState](app);
    if (prevState != app->state) {
        app->subState = 0;
        app->subTimer = 0;
    }
    return result;
}

#pragma force_active reset

int GtsConnection_Exit(GtsApp *app, int state) {
    if (app->paletteAnim.task != NULL) {
        SysTask_Destroy(app->paletteAnim.task);
        app->paletteAnim.task = NULL;
        app->paletteAnim.active = FALSE;
    }
    GtsConnection_DeleteStrings(app);
    GtsConnection_RemoveWindows(app);
    GtsConnection_FreeBgs(app->bgConfig);
    app->paletteAnim.active = FALSE;
    Gts_SwitchScreen(app);
    if (app->screen == 0) {
        return 5;
    }
    return 1;
}

#pragma force_active on

void GtsConnection_InitBgs(BgConfig *bgConfig) {
    BgTemplate bg0 = sGtsConnectionBg0Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &bg0, GF_BG_TYPE_TEXT);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    {
        BgTemplate bg1 = sGtsConnectionBg1Template;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &bg1, GF_BG_TYPE_TEXT);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    }
    {
        BgTemplate subBg0 = sGtsConnectionSubBg0Template;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &subBg0, GF_BG_TYPE_TEXT);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    }
    {
        BgTemplate subBg1 = sGtsConnectionSubBg1Template;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &subBg1, GF_BG_TYPE_TEXT);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    }
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_61);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_61);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
}

#pragma force_active reset

void GtsConnection_FreeBgs(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
}

#pragma force_active on

void GtsConnection_LoadGraphics(GtsApp *app) {
    NNSG2dPaletteData *pltt;
    NARC *narc;
    int group;
    int step;
    int fraction;
    int last;
    u16 *dest;
    u16 *src;
    BgConfig *bgConfig = app->bgConfig;
    void *plttFile;
    int color;

    narc = NARC_New(NARC_a_0_8_8, HEAP_ID_61);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_61);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_61);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_61);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_61);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 1, 14, Options_GetFrame(app->args->options), HEAP_ID_61);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 31, 11, 0, HEAP_ID_61);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_61);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, FALSE, HEAP_ID_61);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 11, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_61);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 12, bgConfig, GF_BG_LYR_SUB_1, 0, 0x600, FALSE, HEAP_ID_61);
    BG_SetMaskColor(GF_BG_LYR_MAIN_0, RGB_BLACK);
    BG_SetMaskColor(GF_BG_LYR_SUB_0, RGB_BLACK);

    MI_CpuFill8(&app->paletteAnim, 0, sizeof(GtsPaletteAnim));
    plttFile = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 5, &pltt, HEAP_ID_61);
    MIi_CpuCopy16(pltt->pRawData, app->paletteAnim.basePltt, sizeof(app->paletteAnim.basePltt));
    MIi_CpuCopy16(pltt->pRawData, app->paletteAnim.fadePltt, sizeof(app->paletteAnim.basePltt));
    Heap_Free(plttFile);

    step = 0;
    group = 0;
    dest = app->paletteAnim.fadePltt;
    src = app->paletteAnim.basePltt;
    while (group < 3) {
        fraction = 0;
        last = 0;
        while (TRUE) {
            GF_ASSERT(step < 21);
            for (color = 1; color < 16; color++) {
                BlendPalette(&src[color], &dest[color], 1, fraction >> 8, app->paletteAnim.basePltt[(group + 1) * 16 + color]);
            }
            dest += 16;
            step++;
            if (last == 1) {
                break;
            }
            fraction += 0x300;
            if (fraction >= 0x1000) {
                fraction = 0x1000;
                last = 1;
            }
        }
        src += 16;
        group++;
    }
    DC_FlushRange(app->paletteAnim.fadePltt, sizeof(app->paletteAnim.fadePltt));
    app->paletteAnim.active = TRUE;
    app->paletteAnim.screens = 0;
    app->paletteAnim.task = SysTask_CreateOnVBlankQueue((SysTaskFunc)GtsConnection_AnimatePaletteTask, &app->paletteAnim, 20);
    NARC_Delete(narc);
}

#pragma force_active reset

void GtsConnection_AnimatePaletteTask(SysTask *task, GtsPaletteAnim *anim) {
    if (!anim->active) {
        return;
    }
    anim->skipFrame ^= 1;
    if (anim->skipFrame & 1) {
        return;
    }
    if (anim->screens <= 1) {
        GX_LoadBGPltt(&anim->fadePltt[anim->frame * 16], 0, 32);
    }
    if (anim->screens == 0 || anim->screens == 2) {
        GXS_LoadBGPltt(&anim->fadePltt[anim->frame * 16], 0, 32);
    }
    if (anim->reverse == 0) {
        anim->frame++;
        if (anim->frame >= 21) {
            anim->frame = 19;
            anim->reverse ^= 1;
        }
    } else {
        anim->frame--;
        if (anim->frame < 0) {
            anim->frame = 1;
            anim->reverse ^= 1;
        }
    }
}

void GtsConnection_CreateWindows(GtsApp *app) {
    AddWindowParameterized(app->bgConfig, &app->boxNameWindow, GF_BG_LYR_MAIN_0, 4, 4, 23, 16, 13, 0x94);
    FillWindowPixelBuffer(&app->boxNameWindow, 0);
    AddWindowParameterized(app->bgConfig, &app->titleWindow, GF_BG_LYR_MAIN_0, 4, 1, 24, 2, 13, 0x204);
    FillWindowPixelBuffer(&app->titleWindow, 0);
    Gts_PrintAlignedFont1(&app->titleWindow, app->titleString, 0, 1, 1, MAKE_TEXT_COLOR(15, 14, 0));
    AddWindowParameterized(app->bgConfig, &app->waitingIconWindow, GF_BG_LYR_MAIN_0, 2, 19, 27, 4, 13, 0x28);
    FillWindowPixelBuffer(&app->waitingIconWindow, 0);
}

void GtsConnection_RemoveWindows(GtsApp *app) {
    RemoveWindow(&app->waitingIconWindow);
    RemoveWindow(&app->titleWindow);
    RemoveWindow(&app->boxNameWindow);
}

void GtsConnection_CreateStrings(GtsApp *app) {
    app->msgBuffer = String_New(180, HEAP_ID_61);
    app->errorBuffer = String_New(256, HEAP_ID_61);
    app->titleString = NewString_ReadMsgData(app->gtsMsgData, 31); // Nintendo Wi-Fi Connection
}

void GtsConnection_DeleteStrings(GtsApp *app) {
    String_Delete(app->titleString);
    String_Delete(app->errorBuffer);
    String_Delete(app->msgBuffer);
}

// Also starts counting the Pokemon in the PC while the player answers (without resetting the count).
int GtsConnection_AskConnect(GtsApp *app) {
    Gts_PrintMessage(app, app->wifiErrorMsgData, 17, 1, 0xF0F); // Connect to Nintendo Wi-Fi Connection?
    Gts_SetState(app, GTS_CONNECTION_STATE_WAIT_TEXT_AND_SHOW_YES_NO, GTS_CONNECTION_STATE_HANDLE_CONNECT_PROMPT);
    app->countingBox = 1;
    return 3;
}

int GtsConnection_HandleConnectPrompt(GtsApp *app) {
    int response = Gts_HandleYesNoPrompt(app);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        Gts_PrintMessage(app, app->wifiMsgData, 1, 1, 0xF0F); // Connecting to Nintendo Wi-Fi Connection...
        Gts_SetState(app, GTS_CONNECTION_STATE_WAIT_TEXT, GTS_CONNECTION_STATE_START_CONNECT);
        Gts_CreateWaitingIcon(app);
    } else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        sub_0203946C();
        Gts_SetNextScreen(app, 0, 0);
        app->state = GTS_CONNECTION_STATE_FADE_OUT;
    }
    return 3;
}

int GtsConnection_AskDisconnect(GtsApp *app) {
    Gts_PrintMessage(app, app->gtsMsgData, 7, 1, 0xF0F); // Is it OK to disconnect from Nintendo Wi-Fi Connection?
    Gts_SetState(app, GTS_CONNECTION_STATE_WAIT_TEXT_AND_SHOW_YES_NO, GTS_CONNECTION_STATE_HANDLE_DISCONNECT_MENU);
    return 3;
}

// Reads a 2D menu that nothing in the GTS creates (the state before shows a Yes/No prompt instead). Cancel goes
// back to the GTS (screen 7 checks its status) when connected, anything else disconnects and leaves the app.
int GtsConnection_HandleDisconnectMenu(GtsApp *app) {
    int result = Handle2dMenuInput_DeleteOnFinish(app->unk_11B0, HEAP_ID_61);
    if (result == -1) {
        goto done;
    }
    if (result == -2) {
        if (ov00_021EC5B4() == 0) {
            app->state = GTS_CONNECTION_STATE_ASK_CONNECT;
            goto done;
        }
        Gts_SetNextScreen(app, 7, 11);
        app->unk_001C = 1;
        app->state = GTS_CONNECTION_STATE_FADE_OUT;
        goto done;
    }
    if (ov00_021EC5B4() != 0) {
        ov00_021EC8D8();
    }
    sub_0203946C();
    Gts_SetNextScreen(app, 0, 0);
    app->state = GTS_CONNECTION_STATE_FADE_OUT;
done:
    return 3;
}

int GtsConnection_PrintDisconnecting(GtsApp *app) {
    Gts_PrintMessage(app, app->wifiErrorMsgData, 26, 1, 0xF0F); // Disconnecting from Nintendo Wi-Fi Connection.
    Gts_SetState(app, GTS_CONNECTION_STATE_WAIT_TEXT, GTS_CONNECTION_STATE_DISCONNECT);
    return 3;
}

int GtsConnection_Disconnect(GtsApp *app) {
    sub_0203946C();
    ov00_021EC8D8();
    Gts_SetNextScreen(app, 0, 0);
    app->state = GTS_CONNECTION_STATE_PRINT_DISCONNECTED;
    return 3;
}

int GtsConnection_PrintDisconnected(GtsApp *app) {
    WirelessStatusIcon_Destroy();
    Gts_PrintMessage(app, app->wifiErrorMsgData, 27, 1, 0xF0F); // You have been disconnected from Nintendo Wi-Fi Connection.
    Gts_SetState(app, GTS_CONNECTION_STATE_WAIT_TEXT_AND_DELAY, GTS_CONNECTION_STATE_FADE_OUT);
    return 3;
}

void GtsConnection_StartConnect(GtsApp *app) {
    sub_02039418(app->args->saveData);
    Sys_SetSleepDisableFlag(4);
    Gts_CreateWirelessStatusIcon(app);
    ov00_021EC3F0(app->wifiConnection, 2, 1, 20);
    ov00_021EC454(2);
    ov00_021EC4A4();
    app->state = GTS_CONNECTION_STATE_WAIT_CONNECT;
}

int GtsConnection_WaitConnect(GtsApp *app) {
    // The original kept these locals in one stack frame struct.
    struct {
        int error;
        int errorType;
        int errorCode;
        u8 apInfo[60];
    } locals;

    ov00_021EC60C();
    if (ov00_021EC5B4() != 0) {
        switch (ov00_021EC724()) {
        case 7:
            app->connectionResult = ov00_021EC11C(&locals.errorCode, &locals.errorType);
            app->connectionErrorCode = locals.errorCode;
            app->connectionErrorType = locals.errorType;
            ov00_021EC210();
            ov00_021EC8D8();
            Gts_DeleteWaitingIcon(app);
            app->state = GTS_CONNECTION_STATE_SHOW_CONNECTION_ERROR;
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
        case 8:
        default:
            ov00_021EC0FC(&locals.error);
            Gts_DeleteWaitingIcon(app);
            app->state = GTS_CONNECTION_STATE_PRINT_SERVER_ERROR;
            app->serverResult = -2;
            break;
        case 4:
            ov00_021EC9E0(locals.apInfo);
            app->state = GTS_CONNECTION_STATE_START_AUTH;
            break;
        }
    }
    return 3;
}

int GtsConnection_StartAuth(GtsApp *app) {
    ov00_021ECD04();
    app->state = GTS_CONNECTION_STATE_WAIT_AUTH;
    return 3;
}

int GtsConnection_WaitAuth(GtsApp *app) {
    int errorCode;
    int errorType;
    int result;

    switch (ov00_021ECDC8()) {
    case 3:
        app->state = GTS_CONNECTION_STATE_INIT_SERVER;
        break;
    case 0:
    case 4:
    case 5:
        Gts_DeleteWaitingIcon(app);
        result = ov00_021EC11C(&errorCode, &errorType);
        app->connectionResult = result;
        app->connectionErrorCode = errorCode;
        ov00_021EC210();
        ov00_021EC8D8();
        app->state = GTS_CONNECTION_STATE_SHOW_CONNECTION_ERROR;
        switch (errorType) {
        case 1:
        case 2:
            app->state = GTS_CONNECTION_STATE_SHOW_CONNECTION_ERROR;
            break;
        case 4:
            ov00_021FA0D8();
            app->state = GTS_CONNECTION_STATE_SHOW_CONNECTION_ERROR;
            break;
        case 6:
            app->state = GTS_CONNECTION_STATE_SHOW_CONNECTION_ERROR;
            break;
        case 3:
            ov00_021ED9B4();
            app->state = GTS_CONNECTION_STATE_SHOW_CONNECTION_ERROR;
            break;
        case 5:
        case 7:
            sub_020399EC();
            break;
        }
        if (errorCode < -20000 && errorCode >= -29999) {
            app->state = GTS_CONNECTION_STATE_SHOW_CONNECTION_ERROR;
        }
        break;
    }
    return 3;
}

int GtsConnection_InitServer(GtsApp *app) {
    void *userData;
    u32 profileId;

    userData = FriendRoster_GetUserData(app->args->unk14);
    if (Save_SysInfo_GetDwcProfileId(app->args->sysInfo) == 0) {
        Save_SysInfo_SetDwcProfileId(app->args->sysInfo, (s32)app->args->unk34);
    }
    profileId = Save_SysInfo_GetDwcProfileId(app->args->sysInfo);
    {
        u64 friendKey = DWC_CreateFriendKey(userData);
        GtsServer_Init(profileId, (u32)friendKey, (u32)(friendKey >> 32));
    }
    app->state = GTS_CONNECTION_STATE_SEND_INFO;
    return 3;
}

int GtsConnection_SendInfo(GtsApp *app) {
    GtsServer_RequestInfo();
    app->state = GTS_CONNECTION_STATE_WAIT_INFO;
    app->requestFrames = 0;
    return 3;
}

// Each request is given a minute; then the connection is dropped (sub_020399EC).
int GtsConnection_WaitInfo(GtsApp *app) {
    if (GtsServer_IsRequestDone()) {
        int result = GtsServer_GetResult();
        app->requestFrames = 0;
        switch (result) {
        case 0:
            app->state = GTS_CONNECTION_STATE_SEND_SET_PROFILE;
            break;
        case 1:
            Gts_DeleteWaitingIcon(app);
            app->serverResult = result;
            app->state = GTS_CONNECTION_STATE_PRINT_SERVER_ERROR;
            break;
        case -1:
        case 2:
            Gts_DeleteWaitingIcon(app);
            app->serverResult = result;
            app->state = GTS_CONNECTION_STATE_PRINT_SERVER_ERROR;
            break;
        case -15:
        case -12:
            Gts_DeleteWaitingIcon(app);
            app->serverResult = result;
            app->state = GTS_CONNECTION_STATE_PRINT_SERVER_ERROR;
            break;
        case -14:
        case -2:
            Gts_DeleteWaitingIcon(app);
            app->serverResult = result;
            app->state = GTS_CONNECTION_STATE_PRINT_SERVER_ERROR;
            break;
        default:
            Gts_DeleteWaitingIcon(app);
            sub_020399EC();
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

int GtsConnection_SendSetProfile(GtsApp *app) {
    sub_0203189C(app->args->saveData, &app->profile);
    GtsServer_RequestSetProfile(&app->profile, app->profileResponse);
    app->state = GTS_CONNECTION_STATE_WAIT_SET_PROFILE;
    app->requestFrames = 0;
    return 3;
}

// The answer of setProfile.asp: [0] 0 accepted, 1 and 2 errors; [1] when accepted: 0 fine, 1..3 errors. The errors
// become the server results -5000 .. -5005 that GtsConnection_PrintServerError has messages for.
int GtsConnection_WaitSetProfile(GtsApp *app) {
    if (GtsServer_IsRequestDone()) {
        int result = GtsServer_GetResult();
        app->requestFrames = 0;
        switch (result) {
        case 0:
            Gts_DeleteWaitingIcon(app);
            switch (app->profileResponse[0]) {
            case 0:
                switch (app->profileResponse[1]) {
                case 0:
                    Gts_SetNextScreen(app, 1, 0);
                    app->state = GTS_CONNECTION_STATE_FADE_OUT;
                    break;
                case 3:
                    app->serverResult = -5003;
                    app->state = GTS_CONNECTION_STATE_PRINT_SERVER_ERROR;
                    break;
                case 1:
                    app->serverResult = -5000;
                    app->state = GTS_CONNECTION_STATE_PRINT_SERVER_ERROR;
                    break;
                case 2:
                    app->serverResult = -5001;
                    app->state = GTS_CONNECTION_STATE_PRINT_SERVER_ERROR;
                    break;
                default:
                    sub_020399EC();
                    break;
                }
                break;
            case 1:
                app->serverResult = -5004;
                app->state = GTS_CONNECTION_STATE_PRINT_SERVER_ERROR;
                break;
            case 2:
                app->serverResult = -5005;
                app->state = GTS_CONNECTION_STATE_PRINT_SERVER_ERROR;
                break;
            default:
                Gts_DeleteWaitingIcon(app);
                sub_020399EC();
                break;
            }
            break;
        case 1:
            Gts_DeleteWaitingIcon(app);
            app->serverResult = result;
            app->state = GTS_CONNECTION_STATE_PRINT_SERVER_ERROR;
            break;
        case -1:
        case 2:
            Gts_DeleteWaitingIcon(app);
            app->serverResult = result;
            app->state = GTS_CONNECTION_STATE_PRINT_SERVER_ERROR;
            break;
        case -15:
        case -12:
            Gts_DeleteWaitingIcon(app);
            app->serverResult = result;
            app->state = GTS_CONNECTION_STATE_PRINT_SERVER_ERROR;
            break;
        case -14:
        case -2:
            Gts_DeleteWaitingIcon(app);
            app->serverResult = result;
            app->state = GTS_CONNECTION_STATE_PRINT_SERVER_ERROR;
            break;
        default:
            Gts_DeleteWaitingIcon(app);
            sub_020399EC();
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

int GtsConnection_ShowConnectionError(GtsApp *app) {
    int msgId = WifiConnection_ClassifyErrorCode(-app->connectionErrorCode, app->connectionErrorType);
    GtsConnection_PrintErrorCode(app, msgId, -app->connectionErrorCode);
    app->state = GTS_CONNECTION_STATE_WAIT_CLOSE_ERROR;
    return 3;
}

int GtsConnection_WaitCloseError(GtsApp *app) {
    if ((gSystem.newKeys & PAD_BUTTON_A) || (gSystem.newKeys & PAD_BUTTON_B) || gSystem.touchNew != 0) {
        EraseFrameAndWindow1(&app->boxNameWindow, FALSE);
        app->state = GTS_CONNECTION_STATE_ASK_CONNECT;
    }
    return 3;
}

int GtsConnection_FadeOut(GtsApp *app) {
    WirelessStatusIcon_Destroy();
    Gts_DeleteWaitingIcon(app);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_61);
    app->state = GTS_CONNECTION_STATE_ASK_CONNECT;
    app->unk_11FC = 1;
    return 4;
}

// Nothing sets this state: Yes leaves the app, No asks to connect again.
int GtsConnection_HandleExitPrompt(GtsApp *app) {
    int response = Gts_HandleYesNoPrompt(app);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        Gts_SetNextScreen(app, 0, 0);
        app->state = GTS_CONNECTION_STATE_FADE_OUT;
    } else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        app->state = GTS_CONNECTION_STATE_ASK_CONNECT;
    }
    return 3;
}

int GtsConnection_PrintServerError(GtsApp *app) {
    int msgId;

    switch (app->serverResult) {
    case 1:
        msgId = 150; // The GTS is currently undergoing maintenance.
        break;
    case -1:
    case 2:
        msgId = 151; // The GTS is very crowded now.
        break;
    case -14:
    case -2:
        msgId = 156; // You were disconnected from the GTS.
        break;
    case -15:
    case -12:
    case -5:
    case -3:
    default:
        msgId = 155; // Unable to connect to the GTS.
        break;
    case -5003:
        msgId = 185; // There was a Wii message authentication error.
        break;
    case -5000:
        msgId = 182; // There was a communication error.
        break;
    case -5001:
        msgId = 182;
        break;
    case -5004:
        msgId = 183; // The data was corrupted. It could not be sent.
        break;
    case -5005:
        msgId = 184; // The server is undergoing maintenance.
        break;
    }
    Gts_PrintMessage(app, app->gtsMsgData, msgId, 1, 0xF0F);
    Gts_SetState(app, GTS_CONNECTION_STATE_WAIT_TEXT, GTS_CONNECTION_STATE_DISCONNECT_AFTER_ERROR);
    return 3;
}

int GtsConnection_DisconnectAfterError(GtsApp *app) {
    switch (app->subState) {
    case 0:
        Gts_PrintMessage(app, app->gtsMsgData, 186, 1, 0xF0F); // Disconnecting from Nintendo Wi-Fi Connection...
        app->subState++;
        break;
    case 1:
        if (!TextPrinterCheckActive(app->textPrinterId)) {
            sub_0203946C();
            ov00_021EC8D8();
            app->subState++;
        }
        break;
    case 2:
        Gts_PrintMessage(app, app->gtsMsgData, 187, 1, 0xF0F); // You have disconnected from Nintendo Wi-Fi Connection.
        app->subState++;
        break;
    case 3:
        if (!TextPrinterCheckActive(app->textPrinterId)) {
            app->subState++;
        }
        break;
    default:
        app->subTimer++;
        if (app->subTimer > 30) {
            Gts_SetNextScreen(app, 0, 0);
            app->state = GTS_CONNECTION_STATE_FADE_OUT;
        }
        break;
    }
    return 3;
}

int GtsConnection_WaitText(GtsApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        app->state = app->nextState;
    }
    return 3;
}

int GtsConnection_WaitTextAndDelay(GtsApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        if (app->waitFrames > 30) {
            app->state = app->nextState;
        }
        app->waitFrames++;
    }
    return 3;
}

int GtsConnection_WaitTextAndShowYesNo(GtsApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        app->yesNoPrompt = Gts_CreateYesNoPrompt(app->bgConfig, 10, 0x234, 8, 0);
        app->state = app->nextState;
    }
    return 3;
}

void Gts_PrintMessage(GtsApp *app, MsgData *msgData, int msgId, int textSpeed, u32 unused4) {
    String *string = NewString_ReadMsgData(msgData, msgId);
    StringExpandPlaceholders(app->msgFormat, app->msgBuffer, string);
    String_Delete(string);
    FillWindowPixelBuffer(&app->waitingIconWindow, 15);
    DrawFrameAndWindow2(&app->waitingIconWindow, FALSE, 1, 14);
    app->textPrinterId = AddTextPrinterParameterized(&app->waitingIconWindow, 1, app->msgBuffer, 0, 0, textSpeed, NULL);
    app->waitFrames = 0;
}

int Gts_GetAlignedX(Window *window, String *string, int x, int align, u32 unused, u32 fontId) {
    switch (align) {
    case 1: {
        int width = FontID_String_GetWidth(fontId, string, 0);
        x = (window->width * 8 - width) / 2;
        break;
    }
    case 2: {
        int width = FontID_String_GetWidth(fontId, string, 0);
        x = window->width * 8 - width;
        break;
    }
    }
    return x;
}

void Gts_PrintAlignedFont1(Window *window, String *string, int x, int y, int align, u32 color) {
    x = Gts_GetAlignedX(window, string, x, align, color, 1);
    AddTextPrinterParameterizedWithColor(window, 1, string, x, y, TEXT_SPEED_INSTANT, color, NULL);
}

void Gts_PrintAlignedFont0(Window *window, String *string, int x, int y, int align, u32 color) {
    x = Gts_GetAlignedX(window, string, x, align, color, 0);
    AddTextPrinterParameterizedWithColor(window, 0, string, x, y, TEXT_SPEED_INSTANT, color, NULL);
}

void Gts_PrintAlignedFont4(Window *window, String *string, int x, int y, int align, u32 color) {
    x = Gts_GetAlignedX(window, string, x, align, color, 0);
    AddTextPrinterParameterizedWithColor(window, 4, string, x, y, TEXT_SPEED_INSTANT, color, NULL);
}

void Gts_PrintPanelCaption(Window *window, MsgData *msgData, int caption) {
    String *string = NewString_ReadMsgData(msgData, sGtsPanelCaptionMsgIds[caption]);
    Gts_PrintAlignedFont0(window, string, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
    String_Delete(string);
}

void Gts_CreateWirelessStatusIcon(GtsApp *app) {
    WirelessStatusIcon_Create();
}

void GtsConnection_PrintError(GtsApp *app, int msgId) {
    String *string = String_New(256, HEAP_ID_61);
    ReadMsgDataIntoString(app->wifiErrorMsgData, msgId, string);
    StringExpandPlaceholders(app->msgFormat, app->errorBuffer, string);
    FillWindowPixelBuffer(&app->boxNameWindow, 15);
    DrawFrameAndWindow1(&app->boxNameWindow, TRUE, 31, 11);
    app->textPrinterId = AddTextPrinterParameterized(&app->boxNameWindow, 1, app->errorBuffer, 0, 0, TEXT_SPEED_INSTANT, NULL);
    String_Delete(string);
}

void GtsConnection_PrintErrorCode(GtsApp *app, int msgId, int errorCode) {
    if (msgId == -1) {
        msgId = 11; // Communication error. Error code: ...
    }
    BufferIntegerAsString(app->msgFormat, 0, errorCode, 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
    ClearFrameAndWindow2(&app->waitingIconWindow, TRUE);
    GtsConnection_PrintError(app, msgId);
}
