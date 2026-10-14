#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0038.h"
#include "msgdata/msg/msg_0188.h"
#include "msgdata/msg/msg_0775.h"
#include "msgdata/msg/msg_0778.h"
#include "msgdata/msg/msg_0800.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "list_menu.h"
#include "list_menu_cursor.h"
#include "list_menu_items.h"
#include "main.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "wifi_club_app.h"
#include "overlay_manager.h"
#include "palette.h"
#include "player_data.h"
#include "pm_string.h"
#include "poke_overlay.h"
#include "render_text.h"
#include "render_window.h"
#include "sav_system_info.h"
#include "save.h"
#include "save_vars_flags.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "string_util.h"
#include "sys_flags.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "title_screen.h"
#include "oam_manager.h"
#include "touchpad.h"
#include "friend_roster.h"
#include "save_wii_message.h"
#include "comm_wireless.h"
#include "comm_state.h"
#include "wireless_status_icon.h"
#include "network_overlay_loader.h"
#include "vram_transfer_manager.h"
#include "wii_number.h"
#include "yes_no_prompt.h"

FS_EXTERN_OVERLAY(OVY_70);

// The sub-applications the Wii message settings app runs, indexes into sWiiMessageSettingsApps.
enum WiiMessageSettingsApp {
    WII_MESSAGE_SETTINGS_APP_SCREEN,
    WII_MESSAGE_SETTINGS_APP_WII_NUMBER_ENTRY,
    WII_MESSAGE_SETTINGS_APP_REGISTRATION_CODE_ENTRY,
    WII_MESSAGE_SETTINGS_APP_SET_PASSWORD,
    WII_MESSAGE_SETTINGS_APP_CHECK_PASSWORD,
    WII_MESSAGE_SETTINGS_APP_OV44,
    WII_MESSAGE_SETTINGS_APP_QUIT,
};

// Result of the Wii number entry (entered twice for confirmation).
enum WiiNumberEntryState {
    WII_NUMBER_ENTRY_CONFIRMED,
    WII_NUMBER_ENTRY_CANCELLED, // empty or all zeros
    WII_NUMBER_ENTRY_ENTERED_ONCE,
    WII_NUMBER_ENTRY_MISMATCH,
};

// States of WiiMessageSettingsScreen_Main, indexes into sWiiMessageSettingsScreenStates.
enum WiiMessageSettingsScreenState {
    WMS_STATE_MAIN_MENU,
    WMS_STATE_CHECK_WII_NUMBER,
    WMS_STATE_SET_MESSAGE_RECEPTION,
    WMS_STATE_DELETE_SETTINGS,
    WMS_STATE_REGISTER_WII_NUMBER,
    WMS_STATE_START_WII_NUMBER_ENTRY,
    WMS_STATE_CHECK_WII_NUMBER_ENTRY,
    WMS_STATE_CONFIRM_CONNECT,
    WMS_STATE_8,
    WMS_STATE_PRINT_DISCONNECTING,
    WMS_STATE_DISCONNECT,
    WMS_STATE_PRINT_DISCONNECTED,
    WMS_STATE_START_CONNECT,
    WMS_STATE_WAIT_CONNECT,
    WMS_STATE_14,
    WMS_STATE_15,
    WMS_STATE_START_SERVER_SESSION,
    WMS_STATE_SEND_REGISTRATION_REQUEST,
    WMS_STATE_WAIT_REGISTRATION_RESPONSE,
    WMS_STATE_REGISTRATION_CODE_SENT,
    WMS_STATE_START_REGISTRATION_CODE_ENTRY,
    WMS_STATE_CONFIRM_CANCEL_REGISTRATION,
    WMS_STATE_CONFIRM_REGISTRATION_CODE,
    WMS_STATE_START_PASSWORD_ENTRY,
    WMS_STATE_SAVE_SETTINGS,
    WMS_STATE_SAVE_GAME,
    WMS_STATE_DISCONNECT_AND_WAIT,
    WMS_STATE_SHOW_CONNECTION_ERROR,
    WMS_STATE_WAIT_CLOSE_ERROR,
    WMS_STATE_FADE_OUT,
    WMS_STATE_FADE_OUT_ONLINE, // like WMS_STATE_FADE_OUT, but stays connected
    WMS_STATE_SHOW_YES_NO,
    WMS_STATE_SHOW_REGISTRATION_ERROR,
    WMS_STATE_DISCONNECT_AND_QUIT,
    WMS_STATE_WAIT_MESSAGE,
    WMS_STATE_WAIT_MESSAGE_AND_DELAY,
    WMS_STATE_WAIT_MESSAGE_AND_SHOW_YES_NO,
    WMS_STATE_PRINT_WII_NUMBER_REENTRY,
    WMS_STATE_CHECK_PASSWORD,
};

#define WII_MESSAGE_SETTINGS_TEXT_PRINTER_NONE 0xFF

// Data of the Wii message settings app (OverlayManager data, HEAP_ID_115).
typedef struct WiiMessageSettings {
    OverlayManager *child;      // 0x000
    SaveData *saveData;         // 0x004
    Options *options;           // 0x008
    void *expHeapMemory;        // 0x00C
    NNSFndHeapHandle expHeap;   // 0x010
    u8 unk_14[0x64];            // 0x014, passed to DWC_InitInetEx when connecting
    u16 registrationCodePrefix; // 0x078
    u8 screenStartState;        // 0x07A
    u8 unk_7B;                  // 0x07B
    u32 networkLoaded;          // 0x07C
    u32 currentApp;             // 0x080
    u32 unk_84;                 // 0x084
    u32 nextApp;                // 0x088
    u32 unk_8C;                 // 0x08C, set together with nextApp, never read
    u32 unk_90;                 // 0x090
    void *childArgs;            // 0x094
    WifiServerProfile request;        // 0x098
    int responseStatus;         // 0x0FC, written by GtsServer_RequestSetProfile
    int responseResult;         // 0x100
    u32 password;               // 0x104
    u32 registrationCode;       // 0x108, last 4 digits; -1: entry cancelled
    u8 wiiNumberEntryState;     // 0x10C
    String *wiiNumber;          // 0x110, the first of the two Wii number entries
    String *unk_114;            // 0x114
    u32 serverSessionStarted;   // 0x118
} WiiMessageSettings;           // size: 0x11C

typedef struct WiiMessageSettingsAppEntry {
    void *(*createArgs)(WiiMessageSettings *app);
    void (*destroyArgs)(WiiMessageSettings *app);
    const OverlayManagerTemplate *template;
    u32 unloadNetwork;
} WiiMessageSettingsAppEntry;

// Palette cycle of the background, run as a VBlank task.
typedef struct WiiMessageSettingsPaletteAnim {
    SysTask *task;               // 0x000
    BOOL active;                 // 0x004
    u16 basePltt[4 * 16];        // 0x008
    u16 fadePltt[21 * 16];       // 0x088, 21 blend steps of palette 0
    s16 frame;                   // 0x328
    s8 reverse;                  // 0x32A
    u8 skipFrame;                // 0x32B
    u32 screens;                 // 0x32C, 0: both, 1: main, 2: sub
} WiiMessageSettingsPaletteAnim; // size: 0x330

// The main screen of the app (OverlayManager data, HEAP_ID_116).
typedef struct WiiMessageSettingsScreen {
    WiiMessageSettings *app;                   // 0x000
    BgConfig *bgConfig;                        // 0x004
    int state;                                 // 0x008
    int nextState;                             // 0x00C
    int registrationError;                     // 0x010
    int connectionResult;                      // 0x014
    int connectionErrorCode;                   // 0x018
    int connectionErrorType;                   // 0x01C
    MessageFormat *msgFormat;                  // 0x020
    MsgData *gtsMsgData;                       // 0x024
    MsgData *speciesNamesMsgData;              // 0x028
    MsgData *wfcMsgData;                       // 0x02C
    MsgData *wfcErrorMsgData;                  // 0x030
    MsgData *msgData;                          // 0x034
    String *messageBuffer;                     // 0x038
    String *title;                             // 0x03C
    String *errorBuffer;                       // 0x040
    u32 textPrinterId;                         // 0x044
    Window messageWindow;                      // 0x048
    Window titleWindow;                        // 0x058
    Window errorWindow;                        // 0x068
    Window menuWindow;                         // 0x078
    YesNoPrompt *yesNoPrompt;                  // 0x088
    WaitingIcon *waitingIcon;                  // 0x08C
    int timer;                                 // 0x090
    int subState;                              // 0x094
    int waitTimer;                             // 0x098
    int menuSelection;                         // 0x09C
    ListMenuItem *menuItems;                   // 0x0A0
    ListMenu *listMenu;                        // 0x0A4
    int explanationState;                      // 0x0A8
    int explanationScroll;                     // 0x0AC
    int registerChoice;                        // 0x0B0, 0: register, 1: don't register
    int explanationNumLines;                   // 0x0B4
    int explanationResult;                     // 0x0B8
    int arrowTimer;                            // 0x0BC
    int arrowFrame;                            // 0x0C0
    Window explanationWindow;                  // 0x0C4
    Window choiceWindow;                       // 0x0D4
    struct ListMenuCursor *cursor;             // 0x0E4
    int timeoutTimer;                          // 0x0E8
    WiiMessageSettingsPaletteAnim paletteAnim; // 0x0EC
} WiiMessageSettingsScreen;                    // size: 0x41C

typedef struct WiiMessageSettingsMenuEntry {
    u32 msgId;
    u32 nextState;
} WiiMessageSettingsMenuEntry;

typedef struct WiiMessageSettingsWindowRect {
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} WiiMessageSettingsWindowRect;

typedef struct WiiMessageSettingsChoicePosition {
    u32 x;
    u32 y;
} WiiMessageSettingsChoicePosition;

typedef struct WiiMessageSettingsMainBgTemplates {
    BgTemplate bg0;
    BgTemplate bg1;
    BgTemplate bg2;
    BgTemplate bg3;
} WiiMessageSettingsMainBgTemplates;

typedef struct WiiMessageSettingsSubBgTemplates {
    BgTemplate bg0;
    BgTemplate bg1;
} WiiMessageSettingsSubBgTemplates;

// Defined in unk_020850F4 (number entry app gOverlayTemplate_CodeEntry); not declared in a header.
typedef struct NumberEntryArgs {
    u8 filler_00[0x1C];
    String *string; // 0x1C
} NumberEntryArgs;

NumberEntryArgs *CodeEntryArgs_New(enum HeapID heapID, u32 length, const u32 *groups, Options *options, u32 promptMsg, u32 showWirelessIcon);
NumberEntryArgs *CodeEntryArgs_NewWithFixedDigits(enum HeapID heapID, u32 length, const u32 *groups, Options *options, u32 promptMsg, u32 showWirelessIcon, u32 fixedGroups, u32 fixedValue);
void CodeEntryArgs_Delete(NumberEntryArgs *args);

// Network and server functions without a header.
BOOL LoadOVY38(void);
void UnloadOVY38(void);
void CommWireless_Terminate(void);
void WirelessStatusIcon_SetLevel(int level);
void CommState_InitWiiMessageWfcSession(SaveData *saveData);
void CommState_FreeWiiMessageWfcSession(void);
void CommState_ShowRestartErrorAndHalt(void);
void DWC_SetMemFunc(void *(*alloc)(int, u32, int), void (*free)(int, void *));
int DWC_GetLinkLevel(void);
void DWC_UpdateConnection(void);
void DWC_CleanupInet(void);
int DWC_CheckInet(void);
void DWC_InitInetEx(void *a0, int a1, int a2, int a3);
void DWC_SetAuthServer(int a0);
void DWC_ConnectInetAsync(void);
void DWC_ProcessInet(void);
int DWC_GetInetStatus(void);
int DWC_GetLastErrorEx(int *errorCode, u32 *errorType);
void DWC_ClearError(void);
void DWC_GetLastError(int *a0);
void DWC_GetApInfo(void *a0);
void DWC_NASLoginAsync(void);
int DWC_NASLoginProcess(void);
void DWC_ShutdownGHTTP(void);
void DWC_ShutdownFriendsMatch(void);
s32 WifiConnection_ClassifyErrorCode(s32 errorCode, u32 errorType);
u64 DWC_CreateFriendKey(void *userData);
void GtsServer_Update(void);
void GtsServer_Init(u32 profileId, u32 friendKeyLo, u32 friendKeyHi);
void GtsServer_Exit(void);
BOOL GtsServer_IsRequestDone(void);
int GtsServer_GetResult(void);
void GtsServer_RequestSetProfile(WifiServerProfile *request, int *response);

extern NNSFndHeapHandle sWiiMessageSettingsHeap;
extern const WiiMessageSettingsAppEntry sWiiMessageSettingsApps[];
extern const OverlayManagerTemplate gOverlayTemplate_CodeEntry;
extern const WiiMessageSettingsWindowRect sWiiMessageSettingsRegisteredMenuWindow;
extern const WiiMessageSettingsWindowRect sWiiMessageSettingsUnregisteredMenuWindow;
extern const GraphicsModes sWiiMessageSettingsGraphicsModes;
extern const WiiMessageSettingsMenuEntry sWiiMessageSettingsUnregisteredMenuItems[];
extern const ListMenuTemplate sWiiMessageSettingsUnregisteredMenuTemplate;
extern const WiiMessageSettingsMenuEntry sWiiMessageSettingsRegisteredMenuItems[];
extern const ListMenuTemplate sWiiMessageSettingsRegisteredMenuTemplate;
extern const GraphicsBanks sWiiMessageSettingsGraphicsBanks;
extern const WiiMessageSettingsSubBgTemplates sWiiMessageSettingsSubBgTemplates;
extern const WiiMessageSettingsMainBgTemplates sWiiMessageSettingsMainBgTemplates;
extern BOOL (*sWiiMessageSettingsScreenStates[])(WiiMessageSettingsScreen *screen);

BOOL WiiMessageSettings_Init(OverlayManager *man, int *state);
BOOL WiiMessageSettings_Main(OverlayManager *man, int *state);
BOOL WiiMessageSettings_Exit(OverlayManager *man, int *state);
void WiiMessageSettings_LoadNetworkOverlays(WiiMessageSettings *app);
void WiiMessageSettings_UnloadNetworkOverlays(WiiMessageSettings *app);
void WiiMessageSettings_SetNextApp(WiiMessageSettings *app, int nextApp, int unused);
void WiiMessageSettings_Quit(WiiMessageSettings *app);
void WiiMessageSettings_SetScreenStartState(WiiMessageSettings *app, int state);
u8 WiiMessageSettings_GetScreenStartState(WiiMessageSettings *app);
void *WiiMessageSettings_Alloc(int unused, u32 size, int alignment);
void WiiMessageSettings_Free(int unused, void *ptr);
void WiiMessageSettings_SaveWiiAddress(WiiMessageSettings *app);
void WiiMessageSettings_SaveRegistrationCode(WiiMessageSettings *app);
void WiiMessageSettings_SavePassword(WiiMessageSettings *app);
void WiiMessageSettings_InitRequest(WiiMessageSettings *app);
void WiiMessageSettings_SetRequestWiiAddress(WiiMessageSettings *app);
void WiiMessageSettings_SetRequestRegistrationCode(WiiMessageSettings *app);
void WiiMessageSettings_SetRegistrationCodePrefix(WiiMessageSettings *app, int prefix);
u32 WiiMessageSettings_GetRegistrationCode(WiiMessageSettings *app);
u32 WiiMessageSettings_GetPassword(WiiMessageSettings *app);
void WiiMessageSettings_SetWiiNumberEntryState(WiiMessageSettings *app, u8 entryState);
u8 WiiMessageSettings_GetWiiNumberEntryState(WiiMessageSettings *app);
void *WiiMessageSettings_CreateScreenArgs(WiiMessageSettings *app);
void WiiMessageSettings_DestroyScreenArgs(void);
void *WiiMessageSettings_CreateWiiNumberEntryArgs(WiiMessageSettings *app);
void WiiMessageSettings_FinishWiiNumberEntry(WiiMessageSettings *app);
void *WiiMessageSettings_CreateRegistrationCodeEntryArgs(WiiMessageSettings *app);
void WiiMessageSettings_FinishRegistrationCodeEntry(WiiMessageSettings *app);
void *WiiMessageSettings_CreatePasswordEntryArgs(WiiMessageSettings *app);
void WiiMessageSettings_FinishPasswordEntry(WiiMessageSettings *app);
void *WiiMessageSettings_CreateWifiClubArgs(WiiMessageSettings *app);
void WiiMessageSettings_FinishWifiClub(WiiMessageSettings *app);
BOOL WiiMessageSettingsScreen_Init(OverlayManager *man, int *state);
BOOL WiiMessageSettingsScreen_Main(OverlayManager *man, int *state);
BOOL WiiMessageSettingsScreen_Exit(OverlayManager *man, int *state);
void WiiMessageSettingsScreen_VBlankCB(void *data);
void WiiMessageSettingsScreen_InitBgs(BgConfig *bgConfig);
void WiiMessageSettingsScreen_FreeBgs(BgConfig *bgConfig);
void WiiMessageSettingsScreen_LoadGraphics(WiiMessageSettingsScreen *screen);
void WiiMessageSettingsScreen_AnimatePaletteTask(SysTask *task, WiiMessageSettingsPaletteAnim *anim);
void WiiMessageSettingsScreen_AddWindows(WiiMessageSettingsScreen *screen);
void WiiMessageSettingsScreen_RemoveWindows(WiiMessageSettingsScreen *screen);
void WiiMessageSettingsScreen_SetState(WiiMessageSettingsScreen *screen, int state, int nextState);
void WiiMessageSettingsScreen_ShowWaitingIcon(WiiMessageSettingsScreen *screen);
void WiiMessageSettingsScreen_HideWaitingIcon(WiiMessageSettingsScreen *screen);
YesNoPrompt *WiiMessageSettingsScreen_CreateYesNoPrompt(BgConfig *bgConfig, int tileStart, int initialCursorPos);
BOOL WiiMessageSettingsScreen_MainMenu(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_StartCheckWiiNumber(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_SetMessageReception(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_DeleteSettings(WiiMessageSettingsScreen *screen);
void WiiMessageSettingsScreen_PrintRegisterChoice(WiiMessageSettingsScreen *screen);
void WiiMessageSettingsScreen_ClearRegisterChoice(WiiMessageSettingsScreen *screen);
void WiiMessageSettingsScreen_AnimateScrollArrows(WiiMessageSettingsScreen *screen);
void WiiMessageSettingsScreen_PrintExplanation(WiiMessageSettingsScreen *screen);
int WiiMessageSettingsScreen_RunExplanation(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_RegisterWiiNumber(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_StartWiiNumberEntry(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_CheckWiiNumberEntry(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_ConfirmConnect(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_HandleConnectYesNo(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_PrintDisconnecting(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_Disconnect(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_PrintDisconnected(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_StartConnect(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_WaitConnect(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_StartAuth(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_WaitAuth(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_StartServerSession(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_SendRegistrationRequest(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_WaitRegistrationResponse(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_RegistrationCodeSent(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_StartRegistrationCodeEntry(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_ConfirmCancelRegistration(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_ConfirmRegistrationCode(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_StartPasswordEntry(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_SaveSettings(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_SaveGame(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_DisconnectAndWait(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_ShowConnectionError(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_WaitCloseError(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_FadeOut(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_ShowYesNoPrompt(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_ShowRegistrationError(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_DisconnectAndQuit(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_WaitMessage(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_WaitMessageAndDelay(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_WaitMessageAndShowYesNo(WiiMessageSettingsScreen *screen);
void WiiMessageSettingsScreen_PrintMessage(WiiMessageSettingsScreen *screen, MsgData *msgData, int msgId, u32 textSpeed, u32 unused);
BOOL WiiMessageSettingsScreen_IsPrinterActive(u32 printerId);
BOOL WiiMessageSettingsScreen_PrintWiiNumberReentry(WiiMessageSettingsScreen *screen);
BOOL WiiMessageSettingsScreen_CheckPassword(WiiMessageSettingsScreen *screen);
int WiiMessageSettingsScreen_GetAlignedX(Window *window, String *string, int x, int alignment, u32 unused, u32 fontId);
int WiiMessageSettingsScreen_PrintAligned(Window *window, String *string, int x, int y, int alignment, u32 color);
void WiiMessageSettingsScreen_PrintError(WiiMessageSettingsScreen *screen, int msgId);
void WiiMessageSettingsScreen_PrintErrorCode(WiiMessageSettingsScreen *screen, int msgId, int errorCode);
void WiiMessageSettingsScreen_RegisteredMenuMoveCursor(ListMenu *menu, s32 index, BOOL onInit);
void WiiMessageSettingsScreen_UnregisteredMenuMoveCursor(ListMenu *menu, s32 index, BOOL onInit);

BOOL WiiMessageSettings_Init(OverlayManager *man, int *state) {
    WiiMessageSettings *app;
    void **args;

    Heap_Create(HEAP_ID_3, HEAP_ID_115, 0x28000);
    Heap_Create(HEAP_ID_DEFAULT, HEAP_ID_89, 0x570);
    app = OverlayManager_CreateAndGetData(man, sizeof(WiiMessageSettings), HEAP_ID_115);
    MI_CpuFill8(app, 0, sizeof(WiiMessageSettings));
    args = OverlayManager_GetArgs(man);
    app->saveData = args[2];
    app->options = Save_PlayerData_GetOptionsAddr(app->saveData);
    app->wiiNumber = String_New(100, HEAP_ID_115);
    app->unk_114 = String_New(100, HEAP_ID_115);
    Sound_SetSceneAndPlayBGM(17, SEQ_GS_WIFI_ACCESS, 1);
    app->nextApp = WII_MESSAGE_SETTINGS_APP_SCREEN;
    return TRUE;
}

BOOL WiiMessageSettings_Main(OverlayManager *man, int *state) {
    WiiMessageSettings *app = OverlayManager_GetData(man);
    void *args;

    if (app->networkLoaded == TRUE) {
        DWC_UpdateConnection();
        GtsServer_Update();
        WirelessStatusIcon_SetLevel(3 - DWC_GetLinkLevel());
    }

    switch (*state) {
    case 0:
        WiiMessageSettings_LoadNetworkOverlays(app);
        *state = 1;
        break;
    case 1:
        if (CommWireless_IsStartedUp()) {
            sWiiMessageSettingsHeap = app->expHeap;
            DWC_SetMemFunc(WiiMessageSettings_Alloc, WiiMessageSettings_Free);
            app->networkLoaded = TRUE;
            *state = 2;
        }
        break;
    case 2:
        args = sWiiMessageSettingsApps[app->nextApp].createArgs(app);
        app->child = OverlayManager_New(sWiiMessageSettingsApps[app->nextApp].template, args, HEAP_ID_115);
        app->currentApp = app->nextApp;
        app->nextApp = WII_MESSAGE_SETTINGS_APP_QUIT;
        *state = 3;
        break;
    case 3:
        if (OverlayManager_Run(app->child) == TRUE) {
            sWiiMessageSettingsApps[app->currentApp].destroyArgs(app);
            OverlayManager_Delete(app->child);
            if (app->nextApp == WII_MESSAGE_SETTINGS_APP_QUIT) {
                *state = 4;
            } else if (sWiiMessageSettingsApps[app->nextApp].unloadNetwork == TRUE) {
                WiiMessageSettings_UnloadNetworkOverlays(app);
                *state = 2;
            } else if (app->networkLoaded == TRUE) {
                *state = 2;
            } else {
                *state = 0;
            }
        }
        break;
    case 4:
        return TRUE;
    }
    return FALSE;
}

BOOL WiiMessageSettings_Exit(OverlayManager *man, int *state) {
    WiiMessageSettings *app = OverlayManager_GetData(man);

    WiiMessageSettings_UnloadNetworkOverlays(app);
    String_Delete(app->unk_114);
    String_Delete(app->wiiNumber);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_115);
    Heap_Destroy(HEAP_ID_89);
    RegisterMainOverlay(FS_OVERLAY_ID(intro_title), &gApplication_TitleScreen);
    return TRUE;
}

void WiiMessageSettings_LoadNetworkOverlays(WiiMessageSettings *app) {
    if (app->networkLoaded == FALSE) {
        HandleLoadOverlay(FS_OVERLAY_ID(OVY_70), OVY_LOAD_ASYNC);
        LoadDwcOverlay();
        LoadOVY38();
        Comm_InitDwc(HEAP_ID_115);
        app->expHeapMemory = Heap_Alloc(HEAP_ID_115, 0x20020);
        app->expHeap = NNS_FndCreateExpHeapEx((void *)(((u32)app->expHeapMemory + 0x1F) & ~0x1F), 0x20000, 0);
        CommWireless_StartUp();
        Sys_ClearSleepDisableFlag(4);
    }
}

void WiiMessageSettings_UnloadNetworkOverlays(WiiMessageSettings *app) {
    if (app->networkLoaded == TRUE) {
        NNS_FndDestroyExpHeap(app->expHeap);
        Heap_Free(app->expHeapMemory);
        UnloadOVY38();
        UnloadDwcOverlay();
        CommWireless_Terminate();
        UnloadOverlayByID(FS_OVERLAY_ID(OVY_70));
        app->networkLoaded = FALSE;
    }
}

void WiiMessageSettings_SetNextApp(WiiMessageSettings *app, int nextApp, int unused) {
    app->nextApp = nextApp;
    app->unk_8C = unused;
}

void WiiMessageSettings_Quit(WiiMessageSettings *app) {
    app->nextApp = WII_MESSAGE_SETTINGS_APP_QUIT;
}

void WiiMessageSettings_SetScreenStartState(WiiMessageSettings *app, int state) {
    app->screenStartState = state;
}

u8 WiiMessageSettings_GetScreenStartState(WiiMessageSettings *app) {
    return app->screenStartState;
}

void *WiiMessageSettings_Alloc(int unused, u32 size, int alignment) {
    OSIntrMode intrMode = OS_DisableInterrupts();
    void *ptr = NNS_FndAllocFromExpHeapEx(sWiiMessageSettingsHeap, size, alignment);
    OS_RestoreInterrupts(intrMode);
    return ptr;
}

void WiiMessageSettings_Free(int unused, void *ptr) {
    if (ptr) {
        OSIntrMode intrMode = OS_DisableInterrupts();
        NNS_FndFreeToExpHeap(sWiiMessageSettingsHeap, ptr);
        OS_RestoreInterrupts(intrMode);
    }
}

#pragma force_active on

void WiiMessageSettings_SaveWiiAddress(WiiMessageSettings *app) {
    char *address = Heap_Alloc(HEAP_ID_115, 100);
    WiiNumber_ToWiiAddress(app->wiiNumber, address, HEAP_ID_115);
    Save_WiiMessage_SetWiiAddress(app->saveData, address);
    Heap_Free(address);
}

#pragma force_active reset

void WiiMessageSettings_SaveRegistrationCode(WiiMessageSettings *app) {
    Save_WiiMessage_SetParam(app->saveData, 1, app->registrationCodePrefix);
    Save_WiiMessage_SetParam(app->saveData, 2, app->registrationCode);
}

void WiiMessageSettings_SavePassword(WiiMessageSettings *app) {
    Save_WiiMessage_SetParam(app->saveData, 3, app->password);
}

void WiiMessageSettings_InitRequest(WiiMessageSettings *app) {
    s32 prefix = Save_WiiMessage_InitRegistrationProfile(app->saveData, &app->request);
    WiiMessageSettings_SetRegistrationCodePrefix(app, prefix);
}

void WiiMessageSettings_SetRequestWiiAddress(WiiMessageSettings *app) {
    WiiNumber_ToWiiAddress(app->wiiNumber, app->request.wiiAddress, HEAP_ID_115);
}

void WiiMessageSettings_SetRequestRegistrationCode(WiiMessageSettings *app) {
    app->request.registrationCode = app->registrationCode;
}

void WiiMessageSettings_SetRegistrationCodePrefix(WiiMessageSettings *app, int prefix) {
    app->registrationCodePrefix = prefix;
}

u32 WiiMessageSettings_GetRegistrationCode(WiiMessageSettings *app) {
    return app->registrationCode;
}

u32 WiiMessageSettings_GetPassword(WiiMessageSettings *app) {
    return app->password;
}

void WiiMessageSettings_SetWiiNumberEntryState(WiiMessageSettings *app, u8 entryState) {
    app->wiiNumberEntryState = entryState;
}

u8 WiiMessageSettings_GetWiiNumberEntryState(WiiMessageSettings *app) {
    return app->wiiNumberEntryState;
}

void *WiiMessageSettings_CreateScreenArgs(WiiMessageSettings *app) {
    return app;
}

void WiiMessageSettings_DestroyScreenArgs(void) {
}

// The Wii number: 16 digits in groups of 4.
void *WiiMessageSettings_CreateWiiNumberEntryArgs(WiiMessageSettings *app) {
    u32 groups[4];
    groups[0] = 4;
    groups[1] = 4;
    groups[2] = 4;
    groups[3] = 4;
    app->childArgs = CodeEntryArgs_New(HEAP_ID_115, 16, groups, Save_PlayerData_GetOptionsAddr(app->saveData), msg_0038_00004, 0);
    return app->childArgs;
}

void WiiMessageSettings_FinishWiiNumberEntry(WiiMessageSettings *app) {
    NumberEntryArgs *args = app->childArgs;
    if (WiiNumber_IsZero(args->string, HEAP_ID_115)) {
        WiiMessageSettings_SetWiiNumberEntryState(app, WII_NUMBER_ENTRY_CANCELLED);
    } else if (WiiMessageSettings_GetWiiNumberEntryState(app) == WII_NUMBER_ENTRY_ENTERED_ONCE) {
        if (String_Compare(app->wiiNumber, args->string)) {
            WiiMessageSettings_SetWiiNumberEntryState(app, WII_NUMBER_ENTRY_MISMATCH);
        } else {
            WiiMessageSettings_SetWiiNumberEntryState(app, WII_NUMBER_ENTRY_CONFIRMED);
        }
    } else {
        String_Copy(app->wiiNumber, args->string);
        WiiMessageSettings_SetWiiNumberEntryState(app, WII_NUMBER_ENTRY_ENTERED_ONCE);
    }
    CodeEntryArgs_Delete(args);
    WiiMessageSettings_SetNextApp(app, WII_MESSAGE_SETTINGS_APP_SCREEN, 0);
}

// The registration code: the 3-digit prefix sent with the request, then the 4 digits the player enters.
void *WiiMessageSettings_CreateRegistrationCodeEntryArgs(WiiMessageSettings *app) {
    u32 groups[4];
    groups[0] = 3;
    groups[1] = 4;
    groups[2] = 0;
    groups[3] = 0;
    app->childArgs = CodeEntryArgs_NewWithFixedDigits(HEAP_ID_115, 7, groups, Save_PlayerData_GetOptionsAddr(app->saveData), msg_0038_00005, 1, 1, app->registrationCodePrefix);
    return app->childArgs;
}

void WiiMessageSettings_FinishRegistrationCodeEntry(WiiMessageSettings *app) {
    BOOL success;
    NumberEntryArgs *args = app->childArgs;
    app->registrationCode = (u64)String_atoi(args->string, &success) % 10000;
    if (!success) {
        GF_AssertFail();
    }
    CodeEntryArgs_Delete(args);
    WiiMessageSettings_SetNextApp(app, WII_MESSAGE_SETTINGS_APP_SCREEN, 0);
}

void *WiiMessageSettings_CreatePasswordEntryArgs(WiiMessageSettings *app) {
    u32 groups[4];
    groups[0] = 4;
    groups[1] = 0;
    groups[2] = 0;
    groups[3] = 0;
    app->childArgs = CodeEntryArgs_New(HEAP_ID_115, 4, groups, Save_PlayerData_GetOptionsAddr(app->saveData), msg_0038_00006, 0);
    return app->childArgs;
}

void WiiMessageSettings_FinishPasswordEntry(WiiMessageSettings *app) {
    BOOL success;
    NumberEntryArgs *args = app->childArgs;
    app->password = String_atoi(args->string, &success);
    if (!success) {
        GF_AssertFail();
    }
    CodeEntryArgs_Delete(args);
    WiiMessageSettings_SetNextApp(app, WII_MESSAGE_SETTINGS_APP_SCREEN, 0);
}

void *WiiMessageSettings_CreateWifiClubArgs(WiiMessageSettings *app) {
    WifiClubArgs *args = Heap_Alloc(HEAP_ID_115, sizeof(WifiClubArgs));
    MI_CpuFill8(args, 0, sizeof(WifiClubArgs));
    args->saveData = app->saveData;
    args->choice = 1;
    app->networkLoaded = FALSE;
    app->childArgs = args;
    return app->childArgs;
}

void WiiMessageSettings_FinishWifiClub(WiiMessageSettings *app) {
    if (Comm_HasWifiProfile(app->saveData)) {
        SetFlag970(Save_VarsFlags_Get(app->saveData));
    }
    Heap_Free(app->childArgs);
    WiiMessageSettings_SetNextApp(app, WII_MESSAGE_SETTINGS_APP_SCREEN, 0);
}

#pragma force_active on

BOOL WiiMessageSettingsScreen_Init(OverlayManager *man, int *state) {
    WiiMessageSettingsScreen *screen;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(GX_PLANEMASK_NONE);
    GXS_SetVisiblePlane(GX_PLANEMASK_NONE);
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    G2_BlendNone();
    G2S_BlendNone();
    Heap_Create(HEAP_ID_3, HEAP_ID_116, 0x70000);
    screen = OverlayManager_CreateAndGetData(man, sizeof(WiiMessageSettingsScreen), HEAP_ID_116);
    MI_CpuFill8(screen, 0, sizeof(WiiMessageSettingsScreen));
    screen->app = OverlayManager_GetArgs(man);
    screen->bgConfig = BgConfig_Alloc(HEAP_ID_116);
    GF_CreateVramTransferManager(64, HEAP_ID_116);
    SetKeyRepeatTimers(4, 8);
    WiiMessageSettingsScreen_InitBgs(screen->bgConfig);
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    screen->msgFormat = MessageFormat_New_Custom(11, 64, HEAP_ID_116);
    screen->gtsMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0775_bin, HEAP_ID_116);
    screen->wfcMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0778_bin, HEAP_ID_116);
    screen->wfcErrorMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0800_bin, HEAP_ID_116);
    screen->speciesNamesMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0237_bin, HEAP_ID_116);
    screen->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0188_bin, HEAP_ID_116);
    screen->messageBuffer = String_New(270, HEAP_ID_116);
    screen->errorBuffer = String_New(256, HEAP_ID_116);
    screen->title = NewString_ReadMsgData(screen->gtsMsgData, msg_0775_00031); // "Nintendo Wi-Fi Connection"
    WiiMessageSettingsScreen_LoadGraphics(screen);
    WiiMessageSettingsScreen_AddWindows(screen);

    switch (WiiMessageSettings_GetScreenStartState(screen->app)) {
    case WMS_STATE_START_CONNECT:
        if (DWC_CheckInet() == 0 && Comm_HasWifiProfile(screen->app->saveData) == TRUE) {
            screen->state = WMS_STATE_START_CONNECT;
            CommState_InitWiiMessageWfcSession(screen->app->saveData);
            WirelessStatusIcon_Create();
        } else {
            screen->state = WMS_STATE_MAIN_MENU;
        }
        break;
    case WMS_STATE_CONFIRM_REGISTRATION_CODE: {
        // the original reset the timer through a pointer
        int *timeoutTimer = &screen->timeoutTimer;
        *timeoutTimer = 0;
        WirelessStatusIcon_Create();
    }
        screen->state = WiiMessageSettings_GetScreenStartState(screen->app);
        break;
    default:
        screen->state = WiiMessageSettings_GetScreenStartState(screen->app);
        break;
    }
    WiiMessageSettings_SetScreenStartState(screen->app, WMS_STATE_MAIN_MENU);

    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_116);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    gSystem.screensFlipped = TRUE;
    GfGfx_SwapDisplay();
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    Main_SetVBlankIntrCB(WiiMessageSettingsScreen_VBlankCB, screen);
    return TRUE;
}

#pragma force_active reset

BOOL WiiMessageSettingsScreen_Main(OverlayManager *man, int *state) {
    WiiMessageSettingsScreen *screen = OverlayManager_GetData(man);
    int prevState;
    BOOL done;

    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished() == TRUE) {
            *state = 1;
        }
        break;
    case 1:
        prevState = screen->state;
        done = sWiiMessageSettingsScreenStates[prevState](screen);
        if (prevState != screen->state) {
            screen->subState = 0;
            screen->menuSelection = 0;
            screen->waitTimer = 0;
        }
        if (done == TRUE) {
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

BOOL WiiMessageSettingsScreen_Exit(OverlayManager *man, int *state) {
    WiiMessageSettingsScreen *screen = OverlayManager_GetData(man);

    if (screen->paletteAnim.task != NULL) {
        SysTask_Destroy(screen->paletteAnim.task);
        screen->paletteAnim.task = NULL;
        screen->paletteAnim.active = FALSE;
    }
    DestroyMsgData(screen->msgData);
    DestroyMsgData(screen->speciesNamesMsgData);
    DestroyMsgData(screen->wfcErrorMsgData);
    DestroyMsgData(screen->wfcMsgData);
    DestroyMsgData(screen->gtsMsgData);
    MessageFormat_Delete(screen->msgFormat);
    String_Delete(screen->title);
    String_Delete(screen->errorBuffer);
    String_Delete(screen->messageBuffer);
    WiiMessageSettingsScreen_RemoveWindows(screen);
    Heap_Free(screen->bgConfig);
    WiiMessageSettingsScreen_FreeBgs(screen->bgConfig);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GF_DestroyVramTransferManager();
    GF_TouchpadStopSampling();
    TextFlags_SetCanABSpeedUpPrint(FALSE);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    WirelessStatusIcon_Destroy();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_116);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    return TRUE;
}

void WiiMessageSettingsScreen_VBlankCB(void *data) {
    WiiMessageSettingsScreen *screen = data;
    vu32 *dtcm = (vu32 *)0x027E0000;

    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    DoScheduledBgGpuUpdates(screen->bgConfig);
    dtcm[(HW_INTR_CHECK_BUF - HW_DTCM) / 4] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag, through the DTCM base address
}

#pragma force_active on

void WiiMessageSettingsScreen_InitBgs(BgConfig *bgConfig) {
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    {
        GraphicsBanks banks = sWiiMessageSettingsGraphicsBanks;
        GfGfx_SetBanks(&banks);
    }
    MIi_CpuClear32(0, (void *)HW_BG_VRAM, HW_BG_VRAM_SIZE);
    MIi_CpuClear32(0, (void *)HW_DB_BG_VRAM, HW_DB_BG_VRAM_SIZE);
    MIi_CpuClear32(0, (void *)HW_OBJ_VRAM, HW_OBJ_VRAM_SIZE);
    MIi_CpuClear32(0, (void *)HW_DB_OBJ_VRAM, HW_DB_OBJ_VRAM_SIZE);
    {
        GraphicsModes modes = sWiiMessageSettingsGraphicsModes;
        SetBothScreensModesAndDisable(&modes);
    }
    {
        WiiMessageSettingsMainBgTemplates templates = sWiiMessageSettingsMainBgTemplates;

        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &templates.bg0, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_Y, 0);
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &templates.bg1, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, 0);
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &templates.bg2, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0);
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &templates.bg3, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
    }
    {
        WiiMessageSettingsSubBgTemplates templates = sWiiMessageSettingsSubBgTemplates;

        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &templates.bg0, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_Y, 0);
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &templates.bg1, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, 0);
    }
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, HEAP_ID_116);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 32, 0, HEAP_ID_116);
}

#pragma force_active reset

void WiiMessageSettingsScreen_FreeBgs(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
}

#pragma force_active on

void WiiMessageSettingsScreen_LoadGraphics(WiiMessageSettingsScreen *screen) {
    BgConfig *bgConfig = screen->bgConfig;
    NARC *narc = NARC_New(NARC_a_0_8_8, HEAP_ID_116);
    void *plttFile;
    NNSG2dPaletteData *pltt;
    int group;
    int step;
    int fraction;
    int last;
    u16 *dest;
    u16 *src;

    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0, 0, HEAP_ID_116);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0, 0, HEAP_ID_116);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0x1A0, HEAP_ID_116);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0x1A0, HEAP_ID_116);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 1, 10, Options_GetFrame(screen->app->options), HEAP_ID_116);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 31, 11, 0, HEAP_ID_116);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_2, 31, 11, 0, HEAP_ID_116);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_116);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, FALSE, HEAP_ID_116);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 11, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_116);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 12, bgConfig, GF_BG_LYR_SUB_1, 0, 0x600, FALSE, HEAP_ID_116);
    BG_SetMaskColor(GF_BG_LYR_MAIN_0, RGB_BLACK);
    BG_SetMaskColor(GF_BG_LYR_SUB_0, RGB_BLACK);

    MI_CpuFill8(&screen->paletteAnim, 0, sizeof(WiiMessageSettingsPaletteAnim));
    plttFile = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 5, &pltt, HEAP_ID_116);
    MIi_CpuCopy16(pltt->pRawData, screen->paletteAnim.basePltt, sizeof(screen->paletteAnim.basePltt));
    MIi_CpuCopy16(pltt->pRawData, screen->paletteAnim.fadePltt, sizeof(screen->paletteAnim.basePltt));
    Heap_Free(plttFile);

    // fadePltt: palette 0 blended in 7 steps towards each of palettes 1, 2 and 3
    step = 0;
    group = 0;
    dest = screen->paletteAnim.fadePltt;
    src = screen->paletteAnim.basePltt;
    for (; group < 3; group++) {
        fraction = 0;
        last = 0;
        for (;;) {
            int color;
            if (step >= 21) {
                GF_AssertFail();
            }
            for (color = 1; color < 16; color++) {
                BlendPalette(src + color, dest + color, 1, fraction >> 8, screen->paletteAnim.basePltt[(group + 1) * 16 + color]);
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
    }
    DC_FlushRange(screen->paletteAnim.fadePltt, 21 * 32);
    screen->paletteAnim.active = TRUE;
    screen->paletteAnim.screens = 0;
    screen->paletteAnim.task = SysTask_CreateOnVBlankQueue((SysTaskFunc)WiiMessageSettingsScreen_AnimatePaletteTask, &screen->paletteAnim, 20);
    NARC_Delete(narc);

    narc = NARC_New(NARC_a_1_9_7, HEAP_ID_116);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 11, bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_116);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 10, bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_116);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 12, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_9_OFFSET, 0x20, HEAP_ID_116);
    NARC_Delete(narc);
}

#pragma force_active reset

void WiiMessageSettingsScreen_AnimatePaletteTask(SysTask *task, WiiMessageSettingsPaletteAnim *anim) {
    if (anim->active) {
        anim->skipFrame ^= 1;
        if ((anim->skipFrame & 1) == 0) {
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
    }
}

void WiiMessageSettingsScreen_AddWindows(WiiMessageSettingsScreen *screen) {
    AddWindowParameterized(screen->bgConfig, &screen->errorWindow, GF_BG_LYR_MAIN_0, 4, 4, 23, 16, 13, 148);
    FillWindowPixelBuffer(&screen->errorWindow, 0);
    AddWindowParameterized(screen->bgConfig, &screen->titleWindow, GF_BG_LYR_MAIN_0, 4, 1, 24, 2, 13, 516);
    FillWindowPixelBuffer(&screen->titleWindow, 0);
    WiiMessageSettingsScreen_PrintAligned(&screen->titleWindow, screen->title, 0, 1, 1, MAKE_TEXT_COLOR(15, 14, 0));
    AddWindowParameterized(screen->bgConfig, &screen->messageWindow, GF_BG_LYR_MAIN_0, 2, 19, 27, 4, 13, 40);
    FillWindowPixelBuffer(&screen->messageWindow, 0);
}

void WiiMessageSettingsScreen_RemoveWindows(WiiMessageSettingsScreen *screen) {
    RemoveWindow(&screen->messageWindow);
    RemoveWindow(&screen->titleWindow);
    RemoveWindow(&screen->errorWindow);
}

void WiiMessageSettingsScreen_SetState(WiiMessageSettingsScreen *screen, int state, int nextState) {
    screen->state = state;
    screen->nextState = nextState;
}

void WiiMessageSettingsScreen_ShowWaitingIcon(WiiMessageSettingsScreen *screen) {
    if (!screen->waitingIcon) {
        screen->waitingIcon = WaitingIcon_New(&screen->messageWindow, 1);
    }
}

void WiiMessageSettingsScreen_HideWaitingIcon(WiiMessageSettingsScreen *screen) {
    if (screen->waitingIcon) {
        WaitingIcon_Delete(screen->waitingIcon);
        screen->waitingIcon = NULL;
    }
}

#pragma force_active on

YesNoPrompt *WiiMessageSettingsScreen_CreateYesNoPrompt(BgConfig *bgConfig, int tileStart, int initialCursorPos) {
    YesNoPrompt *prompt = YesNoPrompt_Create(HEAP_ID_116);
    YesNoPromptTemplate template;

    template.bgConfig = bgConfig;
    template.bgId = GF_BG_LYR_MAIN_0;
    template.tileStart = tileStart;
    template.plttSlot = 14;
    template.x = 25;
    template.y = 10;
    template.ignoreTouchFlag = FALSE;
    template.initialCursorPos = initialCursorPos;
    template.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(prompt, &template);
    return prompt;
}

BOOL WiiMessageSettingsScreen_MainMenu(WiiMessageSettingsScreen *screen) {
    ListMenuTemplate template;
    const WiiMessageSettingsWindowRect *rect;
    const WiiMessageSettingsMenuEntry *entries;
    const ListMenuTemplate *srcTemplate;
    int count;
    int i;
    int input;

    switch (screen->subState) {
    case 0:
        if (Save_WiiMessage_IsRegistered(screen->app->saveData) == TRUE) {
            rect = &sWiiMessageSettingsRegisteredMenuWindow;
            entries = sWiiMessageSettingsRegisteredMenuItems;
            count = 4;
            srcTemplate = &sWiiMessageSettingsRegisteredMenuTemplate;
        } else {
            rect = &sWiiMessageSettingsUnregisteredMenuWindow;
            entries = sWiiMessageSettingsUnregisteredMenuItems;
            srcTemplate = &sWiiMessageSettingsUnregisteredMenuTemplate;
            count = 2;
        }
        template = *srcTemplate;
        AddWindowParameterized(screen->bgConfig, &screen->menuWindow, GF_BG_LYR_MAIN_0, rect->x, rect->y, rect->width, rect->height, 13, 148);
        screen->menuItems = ListMenuItems_New(count, HEAP_ID_116);
        i = 0;
        if (count > 0) {
            do {
                ListMenuItems_AppendFromMsgData(screen->menuItems, screen->msgData, entries->msgId, entries->nextState);
                i++;
                entries++;
            } while (i < count);
        }
        template.window = &screen->menuWindow;
        template.items = screen->menuItems;
        screen->listMenu = ListMenuInit(&template, 0, 0, HEAP_ID_116);
        DrawFrameAndWindow1(&screen->menuWindow, TRUE, 31, 11);
        ClearFrameAndWindow2(&screen->messageWindow, TRUE);
        CopyWindowToVram(&screen->menuWindow);
        screen->subState++;
        break;
    case 1:
        input = ListMenu_ProcessInput(screen->listMenu);
        switch (input) {
        case LIST_CANCEL:
            PlaySE(SEQ_SE_DP_SELECT);
            WiiMessageSettings_SetNextApp(screen->app, WII_MESSAGE_SETTINGS_APP_QUIT, 0);
            screen->menuSelection = WMS_STATE_FADE_OUT;
            screen->subState++;
            break;
        case LIST_NOTHING_CHOSEN:
            break;
        default:
            PlaySE(SEQ_SE_DP_SELECT);
            screen->menuSelection = input;
            screen->subState++;
            break;
        }
        break;
    default:
        ListMenuItems_Delete(screen->menuItems);
        DestroyListMenu(screen->listMenu, NULL, NULL);
        EraseFrameAndWindow1(&screen->menuWindow, TRUE);
        ClearWindowTilemapAndCopyToVram(&screen->menuWindow);
        RemoveWindow(&screen->menuWindow);
        screen->state = screen->menuSelection;
        break;
    }
    return FALSE;
}

#pragma force_active reset

BOOL WiiMessageSettingsScreen_StartCheckWiiNumber(WiiMessageSettingsScreen *screen) {
    WiiMessageSettings_SetNextApp(screen->app, WII_MESSAGE_SETTINGS_APP_CHECK_PASSWORD, WII_MESSAGE_SETTINGS_APP_SET_PASSWORD);
    WiiMessageSettings_SetScreenStartState(screen->app, WMS_STATE_CHECK_PASSWORD);
    screen->state = WMS_STATE_FADE_OUT;
    return FALSE;
}

BOOL WiiMessageSettingsScreen_SetMessageReception(WiiMessageSettingsScreen *screen) {
    int input;

    if (WiiMessageSettingsScreen_IsPrinterActive(screen->textPrinterId) == TRUE) {
        return FALSE;
    }
    switch (screen->subState) {
    case 0:
        WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00025, 1, 0xF0F);
        screen->subState++;
        break;
    case 1:
        screen->yesNoPrompt = WiiMessageSettingsScreen_CreateYesNoPrompt(screen->bgConfig, 0x234, 0);
        screen->subState++;
        break;
    case 2:
        input = YesNoPrompt_HandleInput(screen->yesNoPrompt);
        if (input == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(screen->yesNoPrompt);
            WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00026, 1, 0xF0F);
            Save_WiiMessage_SetParam(screen->app->saveData, 0, TRUE);
            screen->subState++;
        } else if (input == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(screen->yesNoPrompt);
            WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00027, 1, 0xF0F);
            Save_WiiMessage_SetParam(screen->app->saveData, 0, FALSE);
            screen->subState++;
        }
        break;
    case 3:
        WiiMessageSettingsScreen_SetState(screen, WMS_STATE_SAVE_GAME, WMS_STATE_MAIN_MENU);
        break;
    }
    return FALSE;
}

BOOL WiiMessageSettingsScreen_DeleteSettings(WiiMessageSettingsScreen *screen) {
    int input;

    if (WiiMessageSettingsScreen_IsPrinterActive(screen->textPrinterId) == TRUE) {
        return FALSE;
    }
    switch (screen->subState) {
    case 0:
        WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00028, 1, 0xF0F);
        screen->subState++;
        break;
    case 1:
        screen->yesNoPrompt = WiiMessageSettingsScreen_CreateYesNoPrompt(screen->bgConfig, 0x234, 1);
        screen->subState++;
        break;
    case 2:
        input = YesNoPrompt_HandleInput(screen->yesNoPrompt);
        if (input == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(screen->yesNoPrompt);
            WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00029, 1, 0xF0F);
            Save_WiiMessage_Reset(screen->app->saveData);
            screen->subState++;
        } else if (input == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(screen->yesNoPrompt);
            screen->state = WMS_STATE_MAIN_MENU;
        }
        break;
    case 3:
        WiiMessageSettingsScreen_SetState(screen, WMS_STATE_SAVE_GAME, WMS_STATE_MAIN_MENU);
        break;
    }
    return FALSE;
}

WiiMessageSettingsChoicePosition sWiiMessageSettingsRegisterChoicePositions[2] = {
    { 4,   8 },
    { 128, 8 },
};

#pragma force_active on

void WiiMessageSettingsScreen_PrintRegisterChoice(WiiMessageSettingsScreen *screen) {
    String *string;

    FillWindowPixelBuffer(&screen->choiceWindow, 15);
    string = NewString_ReadMsgData(screen->msgData, msg_0188_00042);
    AddTextPrinterParameterized(&screen->choiceWindow, 0, string, sWiiMessageSettingsRegisterChoicePositions[0].x + 12, sWiiMessageSettingsRegisterChoicePositions[0].y, TEXT_SPEED_NOTRANSFER, NULL);
    String_Delete(string);
    string = NewString_ReadMsgData(screen->msgData, msg_0188_00043);
    AddTextPrinterParameterized(&screen->choiceWindow, 0, string, sWiiMessageSettingsRegisterChoicePositions[1].x + 12, sWiiMessageSettingsRegisterChoicePositions[1].y, TEXT_SPEED_NOTRANSFER, NULL);
    String_Delete(string);
    // the original call passed x/y without truncating them to the header's u8
    ((void (*)(struct ListMenuCursor *, Window *, u32, u32))ListMenuUpdateCursorObj)(screen->cursor, &screen->choiceWindow, sWiiMessageSettingsRegisterChoicePositions[screen->registerChoice].x, sWiiMessageSettingsRegisterChoicePositions[screen->registerChoice].y);
    CopyWindowToVram(&screen->choiceWindow);
}

#pragma force_active reset

void WiiMessageSettingsScreen_ClearRegisterChoice(WiiMessageSettingsScreen *screen) {
    FillWindowPixelBuffer(&screen->choiceWindow, 15);
    CopyWindowToVram(&screen->choiceWindow);
}

#pragma force_active on

// Blinks the scroll arrows of the explanation text on bg main 3.
void WiiMessageSettingsScreen_AnimateScrollArrows(WiiMessageSettingsScreen *screen) {
    screen->arrowTimer++;
    if (screen->arrowTimer != 8) {
        return;
    }
    screen->arrowFrame ^= 1;
    screen->arrowTimer = 0;
    if (screen->explanationScroll + 6 != screen->explanationNumLines) {
        FillBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_3, screen->arrowFrame * 20 + 1, 14, 17, 1, 1, 9);
        FillBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_3, screen->arrowFrame * 20 + 2, 15, 17, 1, 1, 9);
        FillBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_3, screen->arrowFrame * 20 + 3, 16, 17, 1, 1, 9);
        FillBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_3, screen->arrowFrame * 20 + 4, 17, 17, 1, 1, 9);
        FillBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_3, screen->arrowFrame * 20 + 11, 14, 18, 1, 1, 9);
        FillBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_3, screen->arrowFrame * 20 + 12, 15, 18, 1, 1, 9);
        FillBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_3, screen->arrowFrame * 20 + 13, 16, 18, 1, 1, 9);
        FillBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_3, screen->arrowFrame * 20 + 14, 17, 18, 1, 1, 9);
    } else {
        FillBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_3, 0, 14, 17, 4, 2, 9);
    }
    if (screen->explanationScroll != 0) {
        FillBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_3, screen->arrowFrame * 20 + 5, 14, 3, 1, 1, 9);
        FillBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_3, screen->arrowFrame * 20 + 6, 15, 3, 1, 1, 9);
        FillBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_3, screen->arrowFrame * 20 + 7, 16, 3, 1, 1, 9);
        FillBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_3, screen->arrowFrame * 20 + 8, 17, 3, 1, 1, 9);
        FillBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_3, screen->arrowFrame * 20 + 15, 14, 4, 1, 1, 9);
        FillBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_3, screen->arrowFrame * 20 + 16, 15, 4, 1, 1, 9);
        FillBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_3, screen->arrowFrame * 20 + 17, 16, 4, 1, 1, 9);
        FillBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_3, screen->arrowFrame * 20 + 18, 17, 4, 1, 1, 9);
    } else {
        FillBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_3, 0, 14, 3, 4, 2, 9);
    }
    ScheduleBgTilemapBufferTransfer(screen->bgConfig, GF_BG_LYR_MAIN_3);
}

#pragma force_active reset

// Prints the 6 lines of the "Registering a Wii Number" text from the scroll position on.
void WiiMessageSettingsScreen_PrintExplanation(WiiMessageSettingsScreen *screen) {
    String *text[1];
    String *line;
    int i;
    int y;
    Window *window;

    FillWindowPixelBuffer(&screen->explanationWindow, 15);
    text[0] = NewString_ReadMsgData(screen->msgData, msg_0188_00044);
    line = String_New(String_GetLength(text[0]), HEAP_ID_116);
    i = screen->explanationScroll;
    if (i < screen->explanationScroll + 6) {
        window = (y = 0, &screen->explanationWindow);
        do {
            String_GetLineN(line, text[0], i);
            AddTextPrinterParameterized(window, 0, line, 4, y, TEXT_SPEED_NOTRANSFER, NULL);
            i++;
            y += 16;
        } while (i < screen->explanationScroll + 6);
    }
    String_Delete(text[0]);
    String_Delete(line);
    CopyWindowToVram(&screen->explanationWindow);
}

#pragma force_active on

// The scrollable explanation with "Don't register" / "Register"; returns 1: register, 2: don't register.
int WiiMessageSettingsScreen_RunExplanation(WiiMessageSettingsScreen *screen) {
    switch (screen->explanationState) {
    case 0: {
        String *text = NewString_ReadMsgData(screen->msgData, msg_0188_00044);
        int numLines = String_CountLines(text);
        String *line;
        int i;

        screen->explanationScroll = 0;
        screen->registerChoice = 0;
        screen->explanationResult = 0;
        screen->explanationNumLines = numLines;
        AddWindowParameterized(screen->bgConfig, &screen->explanationWindow, GF_BG_LYR_MAIN_2, 1, 5, 30, 12, 13, 148);
        AddWindowParameterized(screen->bgConfig, &screen->choiceWindow, GF_BG_LYR_MAIN_2, 1, 19, 30, 4, 13, 508);
        FillWindowPixelBuffer(&screen->explanationWindow, 15);
        line = String_New(String_GetLength(text), HEAP_ID_116);
        for (i = 0; i < 6; i++) {
            String_GetLineN(line, text, i);
            AddTextPrinterParameterized(&screen->explanationWindow, 0, line, 4, i * 16, TEXT_SPEED_NOTRANSFER, NULL);
        }
        String_Delete(text);
        String_Delete(line);
        DrawFrameAndWindow1(&screen->explanationWindow, TRUE, 31, 11);
        CopyWindowToVram(&screen->explanationWindow);
        FillWindowPixelBuffer(&screen->choiceWindow, 15);
        DrawFrameAndWindow1(&screen->choiceWindow, TRUE, 31, 11);
        CopyWindowToVram(&screen->choiceWindow);
        screen->cursor = ListMenuCursorNew(HEAP_ID_116);
        FillWindowPixelBuffer(&screen->messageWindow, 15);
        ClearFrameAndWindow2(&screen->messageWindow, TRUE);
        ClearWindowTilemapAndCopyToVram(&screen->messageWindow);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        screen->arrowTimer = 0;
        screen->arrowFrame = 0;
        screen->explanationState++;
        break;
    }
    case 1: {
        int prevScroll = screen->explanationScroll;
        if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
            if (prevScroll != 0) {
                screen->explanationScroll--;
                PlaySE(SEQ_SE_DP_SELECT);
            }
        } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
            if (prevScroll + 6 < screen->explanationNumLines) {
                screen->explanationScroll++;
                PlaySE(SEQ_SE_DP_SELECT);
            }
            if (screen->explanationScroll + 6 == screen->explanationNumLines) {
                WiiMessageSettingsScreen_PrintRegisterChoice(screen);
                screen->explanationState++;
                PlaySE(SEQ_SE_DP_SELECT);
            }
        } else if (gSystem.newKeys & PAD_BUTTON_B) {
            PlaySE(SEQ_SE_DP_SELECT);
            screen->explanationResult = 2;
            screen->explanationState = 255;
        }
        if (prevScroll != screen->explanationScroll) {
            WiiMessageSettingsScreen_PrintExplanation(screen);
        }
        break;
    }
    case 2:
        if ((gSystem.newKeys & PAD_KEY_RIGHT) || (gSystem.newKeys & PAD_KEY_LEFT)) {
            screen->registerChoice ^= 1;
            PlaySE(SEQ_SE_DP_SELECT);
            WiiMessageSettingsScreen_PrintRegisterChoice(screen);
        } else if (gSystem.newKeys & PAD_BUTTON_A) {
            screen->explanationResult = 2 - screen->registerChoice;
            PlaySE(SEQ_SE_DP_SELECT);
            screen->explanationState++;
        } else if (gSystem.newKeys & PAD_BUTTON_B) {
            screen->explanationResult = 2;
            PlaySE(SEQ_SE_DP_SELECT);
            screen->explanationState++;
        } else if (gSystem.newKeys & PAD_KEY_UP) {
            WiiMessageSettingsScreen_ClearRegisterChoice(screen);
            PlaySE(SEQ_SE_DP_SELECT);
            screen->explanationState--;
            screen->explanationScroll--;
            WiiMessageSettingsScreen_PrintExplanation(screen);
        }
        break;
    default:
        EraseFrameAndWindow1(&screen->explanationWindow, FALSE);
        ClearWindowTilemapAndCopyToVram(&screen->explanationWindow);
        RemoveWindow(&screen->explanationWindow);
        EraseFrameAndWindow1(&screen->choiceWindow, FALSE);
        ClearWindowTilemapAndCopyToVram(&screen->choiceWindow);
        RemoveWindow(&screen->choiceWindow);
        DestroyListMenuCursorObj(screen->cursor);
        BgClearTilemapBufferAndCommit(screen->bgConfig, GF_BG_LYR_MAIN_3);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
        screen->explanationState = 0;
        return screen->explanationResult;
    }
    WiiMessageSettingsScreen_AnimateScrollArrows(screen);
    return 0;
}

#pragma force_active reset

BOOL WiiMessageSettingsScreen_RegisterWiiNumber(WiiMessageSettingsScreen *screen) {
    int input;

    if (WiiMessageSettingsScreen_IsPrinterActive(screen->textPrinterId) == TRUE) {
        return FALSE;
    }
    switch (screen->subState) {
    case 0:
        screen->subState = 3;
        break;
    case 1:
        screen->subState++;
        break;
    case 2:
        input = YesNoPrompt_HandleInput(screen->yesNoPrompt);
        if (input == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(screen->yesNoPrompt);
            WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00009, 1, 0xF0F);
            screen->subState = 4;
        } else if (input == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(screen->yesNoPrompt);
            screen->subState++;
        }
        break;
    case 3:
        input = WiiMessageSettingsScreen_RunExplanation(screen);
        if (input == 1) {
            WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00009, 1, 0xF0F);
            screen->subState++;
        } else if (input == 2) {
            WiiMessageSettingsScreen_SetState(screen, WMS_STATE_WAIT_MESSAGE, WMS_STATE_MAIN_MENU);
        }
        break;
    case 4:
        screen->yesNoPrompt = WiiMessageSettingsScreen_CreateYesNoPrompt(screen->bgConfig, 0x234, 0);
        screen->subState++;
        break;
    default:
        input = YesNoPrompt_HandleInput(screen->yesNoPrompt);
        if (input == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(screen->yesNoPrompt);
            screen->state = WMS_STATE_START_WII_NUMBER_ENTRY;
        } else if (input == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(screen->yesNoPrompt);
            WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00010, 1, 0xF0F);
            WiiMessageSettingsScreen_SetState(screen, WMS_STATE_WAIT_MESSAGE, WMS_STATE_MAIN_MENU);
        }
        break;
    }
    return FALSE;
}

BOOL WiiMessageSettingsScreen_StartWiiNumberEntry(WiiMessageSettingsScreen *screen) {
    WiiMessageSettings_SetNextApp(screen->app, WII_MESSAGE_SETTINGS_APP_WII_NUMBER_ENTRY, 0);
    WiiMessageSettings_SetScreenStartState(screen->app, WMS_STATE_CHECK_WII_NUMBER_ENTRY);
    screen->state = WMS_STATE_FADE_OUT;
    return FALSE;
}

BOOL WiiMessageSettingsScreen_CheckWiiNumberEntry(WiiMessageSettingsScreen *screen) {
    switch (WiiMessageSettings_GetWiiNumberEntryState(screen->app)) {
    case WII_NUMBER_ENTRY_CANCELLED:
        screen->state = WMS_STATE_MAIN_MENU;
        return FALSE;
    case WII_NUMBER_ENTRY_ENTERED_ONCE:
    case WII_NUMBER_ENTRY_MISMATCH:
        screen->state = WMS_STATE_PRINT_WII_NUMBER_REENTRY;
        return FALSE;
    default:
        if (!screen->subState) {
            screen->state = WMS_STATE_CONFIRM_CONNECT;
        }
    }
    return FALSE;
}

BOOL WiiMessageSettingsScreen_ConfirmConnect(WiiMessageSettingsScreen *screen) {
    int input;

    if (WiiMessageSettingsScreen_IsPrinterActive(screen->textPrinterId) == TRUE) {
        return FALSE;
    }

    switch (screen->subState) {
    case 0:
        screen->subState++;
        break;
    case 1:
        WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00011, 1, 0xF0F);
        screen->subState++;
        break;
    case 2:
        screen->yesNoPrompt = WiiMessageSettingsScreen_CreateYesNoPrompt(screen->bgConfig, 0x234, 0);
        screen->subState++;
        break;
    case 3:
        input = YesNoPrompt_HandleInput(screen->yesNoPrompt);
        if (input == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(screen->yesNoPrompt);
            screen->subState++;
        } else if (input == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(screen->yesNoPrompt);
            WiiMessageSettingsScreen_SetState(screen, WMS_STATE_WAIT_MESSAGE, WMS_STATE_MAIN_MENU);
        }
        break;
    case 4:
        if (Comm_HasWifiProfile(screen->app->saveData) == FALSE) {
            WiiMessageSettings_SetNextApp(screen->app, WII_MESSAGE_SETTINGS_APP_OV44, 0);
            WiiMessageSettings_SetScreenStartState(screen->app, WMS_STATE_START_CONNECT);
            screen->state = WMS_STATE_FADE_OUT;
            return FALSE;
        }
        WiiMessageSettingsScreen_SetState(screen, WMS_STATE_WAIT_MESSAGE, WMS_STATE_START_CONNECT);
        break;
    }

    return FALSE;
}

BOOL WiiMessageSettingsScreen_HandleConnectYesNo(WiiMessageSettingsScreen *screen) {
    int input = YesNoPrompt_HandleInput(screen->yesNoPrompt);

    if (input == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(screen->yesNoPrompt);
        WiiMessageSettingsScreen_PrintMessage(screen, screen->wfcMsgData, msg_0778_00001, 1, 0xF0F);
        WiiMessageSettingsScreen_SetState(screen, WMS_STATE_WAIT_MESSAGE, WMS_STATE_START_CONNECT);
        WiiMessageSettingsScreen_ShowWaitingIcon(screen);
    } else if (input == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(screen->yesNoPrompt);
        CommState_FreeWiiMessageWfcSession();
        WiiMessageSettings_SetNextApp(screen->app, WII_MESSAGE_SETTINGS_APP_QUIT, 0);
        screen->state = WMS_STATE_FADE_OUT;
    }

    return FALSE;
}

BOOL WiiMessageSettingsScreen_PrintDisconnecting(WiiMessageSettingsScreen *screen) {
    WiiMessageSettingsScreen_PrintMessage(screen, screen->wfcErrorMsgData, msg_0800_00026, 1, 0xF0F);
    WiiMessageSettingsScreen_SetState(screen, WMS_STATE_WAIT_MESSAGE, WMS_STATE_DISCONNECT);
    return FALSE;
}

BOOL WiiMessageSettingsScreen_Disconnect(WiiMessageSettingsScreen *screen) {
    CommState_FreeWiiMessageWfcSession();
    DWC_CleanupInet();
    WirelessStatusIcon_Destroy();
    Sys_ClearSleepDisableFlag(4);
    if (screen->app->serverSessionStarted == TRUE) {
        GtsServer_Exit();
        screen->app->serverSessionStarted = FALSE;
    }
    screen->state = WMS_STATE_PRINT_DISCONNECTED;
    return FALSE;
}

BOOL WiiMessageSettingsScreen_PrintDisconnected(WiiMessageSettingsScreen *screen) {
    WiiMessageSettingsScreen_PrintMessage(screen, screen->wfcErrorMsgData, msg_0800_00027, 1, 0xF0F);
    WiiMessageSettingsScreen_SetState(screen, WMS_STATE_WAIT_MESSAGE_AND_DELAY, WMS_STATE_FADE_OUT);
    return FALSE;
}

#pragma force_active on

BOOL WiiMessageSettingsScreen_StartConnect(WiiMessageSettingsScreen *screen) {
    switch (screen->subState) {
    case 0:
        CommState_InitWiiMessageWfcSession(screen->app->saveData);
        WirelessStatusIcon_Create();
        WiiMessageSettingsScreen_PrintMessage(screen, screen->wfcMsgData, msg_0778_00001, 1, 0xF0F);
        WiiMessageSettingsScreen_ShowWaitingIcon(screen);
        screen->subState++;
        break;
    case 1:
        if (WiiMessageSettingsScreen_IsPrinterActive(screen->textPrinterId) == FALSE) {
            screen->subState++;
        }
        break;
    case 2:
        Sys_SetSleepDisableFlag(4);
        DWC_InitInetEx(screen->app->unk_14, 2, 1, 20);
        DWC_SetAuthServer(2);
        DWC_ConnectInetAsync();
        screen->state = WMS_STATE_WAIT_CONNECT;
        break;
    }
    return FALSE;
}

BOOL WiiMessageSettingsScreen_WaitConnect(WiiMessageSettingsScreen *screen) {
    // the original kept these locals in one stack frame struct
    struct {
        int error;
        u32 errorType;
        int errorCode;
        u8 info[60];
    } locals;

    DWC_ProcessInet();
    if (DWC_CheckInet()) {
        switch (DWC_GetInetStatus()) {
        case 7:
            screen->connectionResult = DWC_GetLastErrorEx(&locals.errorCode, &locals.errorType);
            screen->connectionErrorCode = locals.errorCode;
            screen->connectionErrorType = locals.errorType;
            DWC_ClearError();
            DWC_CleanupInet();
            WirelessStatusIcon_Destroy();
            CommState_FreeWiiMessageWfcSession();
            Sys_ClearSleepDisableFlag(4);
            if (screen->app->serverSessionStarted == TRUE) {
                GtsServer_Exit();
                screen->app->serverSessionStarted = FALSE;
            }
            WiiMessageSettingsScreen_HideWaitingIcon(screen);
            screen->state = WMS_STATE_SHOW_CONNECTION_ERROR;
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
        case 8:
        default:
            DWC_GetLastError(&locals.error);
            WiiMessageSettingsScreen_HideWaitingIcon(screen);
            screen->state = WMS_STATE_SHOW_REGISTRATION_ERROR;
            screen->registrationError = -2;
            break;
        case 4:
            DWC_GetApInfo(locals.info);
            screen->state = WMS_STATE_14;
            break;
        }
    }
    return FALSE;
}

#pragma force_active reset

BOOL WiiMessageSettingsScreen_StartAuth(WiiMessageSettingsScreen *screen) {
    DWC_NASLoginAsync();
    screen->state = WMS_STATE_15;
    return FALSE;
}

#pragma force_active on

BOOL WiiMessageSettingsScreen_WaitAuth(WiiMessageSettingsScreen *screen) {
    int errorCode;
    u32 errorType;

    switch (DWC_NASLoginProcess()) {
    case 3:
        screen->state = WMS_STATE_START_SERVER_SESSION;
        break;
    case 0:
    case 4:
    case 5:
        WiiMessageSettingsScreen_HideWaitingIcon(screen);
        screen->connectionResult = DWC_GetLastErrorEx(&errorCode, &errorType);
        screen->connectionErrorCode = errorCode;
        DWC_ClearError();
        DWC_CleanupInet();
        WirelessStatusIcon_Destroy();
        CommState_FreeWiiMessageWfcSession();
        Sys_ClearSleepDisableFlag(4);
        if (screen->app->serverSessionStarted == TRUE) {
            GtsServer_Exit();
            screen->app->serverSessionStarted = FALSE;
        }
        screen->state = WMS_STATE_SHOW_CONNECTION_ERROR;
        switch (errorType) {
        case 1:
        case 2:
            screen->state = WMS_STATE_SHOW_CONNECTION_ERROR;
            break;
        case 4:
            DWC_ShutdownGHTTP();
            screen->state = WMS_STATE_SHOW_CONNECTION_ERROR;
            break;
        case 6:
            screen->state = WMS_STATE_SHOW_CONNECTION_ERROR;
            break;
        case 3:
            DWC_ShutdownFriendsMatch();
            screen->state = WMS_STATE_SHOW_CONNECTION_ERROR;
            break;
        case 5:
        case 7:
            CommState_ShowRestartErrorAndHalt();
            break;
        }
        if (errorCode < -20000 && errorCode >= -29999) {
            screen->state = WMS_STATE_SHOW_CONNECTION_ERROR;
        }
        break;
    }
    return FALSE;
}

BOOL WiiMessageSettingsScreen_StartServerSession(WiiMessageSettingsScreen *screen) {
    void *wifiList = Save_FriendRoster_Get(screen->app->saveData);
    SysInfo *sysInfo = Save_SysInfo_Get(screen->app->saveData);
    void *userData = FriendRoster_GetUserData(wifiList);
    u32 profileId;
    u64 friendKey;

    if (Save_SysInfo_GetDwcProfileId(sysInfo) == 0) {
        Save_SysInfo_SetDwcProfileId(sysInfo, (s32)Comm_GetOwnGsProfileId(wifiList));
    }
    profileId = Save_SysInfo_GetDwcProfileId(sysInfo);
    friendKey = DWC_CreateFriendKey(userData);
    GtsServer_Init(profileId, (u32)friendKey, (u32)(friendKey >> 32));
    screen->app->serverSessionStarted = TRUE;
    screen->state = WMS_STATE_SEND_REGISTRATION_REQUEST;
    return FALSE;
}

#pragma force_active reset

BOOL WiiMessageSettingsScreen_SendRegistrationRequest(WiiMessageSettingsScreen *screen) {
    WiiMessageSettings_InitRequest(screen->app);
    WiiMessageSettings_SetRequestWiiAddress(screen->app);
    GtsServer_RequestSetProfile(&screen->app->request, &screen->app->responseStatus);
    screen->state = WMS_STATE_WAIT_REGISTRATION_RESPONSE;
    screen->timeoutTimer = 0;
    return FALSE;
}

#pragma force_active on

BOOL WiiMessageSettingsScreen_WaitRegistrationResponse(WiiMessageSettingsScreen *screen) {
    if (GtsServer_IsRequestDone()) {
        int result = GtsServer_GetResult();
        screen->timeoutTimer = 0;
        switch (result) {
        case 0:
            switch (screen->app->responseStatus) {
            case 0:
                switch (screen->app->responseResult) {
                case 1:
                    screen->state = WMS_STATE_REGISTRATION_CODE_SENT;
                    break;
                case 2:
                    WiiMessageSettingsScreen_HideWaitingIcon(screen);
                    screen->registrationError = -5001;
                    screen->state = WMS_STATE_SHOW_REGISTRATION_ERROR;
                    break;
                case 0:
                    WiiMessageSettingsScreen_HideWaitingIcon(screen);
                    screen->registrationError = -5002;
                    screen->state = WMS_STATE_SHOW_REGISTRATION_ERROR;
                    break;
                case 3:
                    WiiMessageSettingsScreen_HideWaitingIcon(screen);
                    screen->registrationError = -5003;
                    screen->state = WMS_STATE_SHOW_REGISTRATION_ERROR;
                    break;
                default:
                    CommState_ShowRestartErrorAndHalt();
                    break;
                }
                break;
            case 1:
                WiiMessageSettingsScreen_HideWaitingIcon(screen);
                screen->registrationError = -5004;
                screen->state = WMS_STATE_SHOW_REGISTRATION_ERROR;
                break;
            case 2:
                WiiMessageSettingsScreen_HideWaitingIcon(screen);
                screen->registrationError = -5005;
                screen->state = WMS_STATE_SHOW_REGISTRATION_ERROR;
                break;
            default:
                WiiMessageSettingsScreen_HideWaitingIcon(screen);
                CommState_ShowRestartErrorAndHalt();
                break;
            }
            break;
        case 1:
            WiiMessageSettingsScreen_HideWaitingIcon(screen);
            screen->registrationError = result;
            screen->state = WMS_STATE_SHOW_REGISTRATION_ERROR;
            break;
        case -1:
        case 2:
            WiiMessageSettingsScreen_HideWaitingIcon(screen);
            screen->registrationError = result;
            screen->state = WMS_STATE_SHOW_REGISTRATION_ERROR;
            break;
        case -15:
        case -12:
            WiiMessageSettingsScreen_HideWaitingIcon(screen);
            screen->registrationError = result;
            screen->state = WMS_STATE_SHOW_REGISTRATION_ERROR;
            break;
        case -14:
        case -2:
            WiiMessageSettingsScreen_HideWaitingIcon(screen);
            screen->registrationError = result;
            screen->state = WMS_STATE_SHOW_REGISTRATION_ERROR;
            break;
        default:
            WiiMessageSettingsScreen_HideWaitingIcon(screen);
            CommState_ShowRestartErrorAndHalt();
            break;
        }
    } else {
        screen->timeoutTimer++;
        if (screen->timeoutTimer == 3600) {
            CommState_ShowRestartErrorAndHalt();
        }
    }
    return FALSE;
}

BOOL WiiMessageSettingsScreen_RegistrationCodeSent(WiiMessageSettingsScreen *screen) {
    if (WiiMessageSettingsScreen_IsPrinterActive(screen->textPrinterId) == TRUE) {
        return FALSE;
    }
    switch (screen->subState) {
    case 0:
        WiiMessageSettingsScreen_HideWaitingIcon(screen);
        WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00013, 1, 0xF0F);
        screen->subState++;
        break;
    case 1:
        screen->yesNoPrompt = WiiMessageSettingsScreen_CreateYesNoPrompt(screen->bgConfig, 0x234, 0);
        screen->subState++;
        break;
    case 2: {
        int input = YesNoPrompt_HandleInput(screen->yesNoPrompt);
        if (input == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(screen->yesNoPrompt);
            screen->subState++;
        } else if (input == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(screen->yesNoPrompt);
            screen->state = WMS_STATE_CONFIRM_CANCEL_REGISTRATION;
        }
        break;
    }
    case 3:
        WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00017, 1, 0xF0F);
        WiiMessageSettingsScreen_SetState(screen, WMS_STATE_WAIT_MESSAGE, WMS_STATE_START_REGISTRATION_CODE_ENTRY);
        break;
    }
    return FALSE;
}

#pragma force_active reset

BOOL WiiMessageSettingsScreen_StartRegistrationCodeEntry(WiiMessageSettingsScreen *screen) {
    WiiMessageSettings_SetNextApp(screen->app, WII_MESSAGE_SETTINGS_APP_REGISTRATION_CODE_ENTRY, WII_MESSAGE_SETTINGS_APP_WII_NUMBER_ENTRY);
    WiiMessageSettings_SetScreenStartState(screen->app, WMS_STATE_CONFIRM_REGISTRATION_CODE);
    screen->state = WMS_STATE_FADE_OUT_ONLINE;
    return FALSE;
}

BOOL WiiMessageSettingsScreen_ConfirmCancelRegistration(WiiMessageSettingsScreen *screen) {
    int input;

    if (WiiMessageSettingsScreen_IsPrinterActive(screen->textPrinterId) == TRUE) {
        return FALSE;
    }

    switch (screen->subState) {
    case 0:
        WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00014, 1, 0xF0F);
        screen->subState++;
        break;
    case 1:
        screen->yesNoPrompt = WiiMessageSettingsScreen_CreateYesNoPrompt(screen->bgConfig, 0x234, 0);
        screen->subState++;
        break;
    case 2:
        input = YesNoPrompt_HandleInput(screen->yesNoPrompt);
        if (input == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(screen->yesNoPrompt);
            WiiMessageSettingsScreen_SetState(screen, WMS_STATE_DISCONNECT_AND_WAIT, WMS_STATE_MAIN_MENU);
        } else if (input == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(screen->yesNoPrompt);
            screen->state = WMS_STATE_REGISTRATION_CODE_SENT;
        }
        break;
    }
    return FALSE;
}

#pragma force_active on

BOOL WiiMessageSettingsScreen_ConfirmRegistrationCode(WiiMessageSettingsScreen *screen) {
    int result;

    if (WiiMessageSettings_GetRegistrationCode(screen->app) == -1) {
        screen->state = WMS_STATE_CONFIRM_CANCEL_REGISTRATION;
        return FALSE;
    }
    switch (screen->subState) {
    case 0:
        WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00018, 0, 0xF0F);
        WiiMessageSettingsScreen_ShowWaitingIcon(screen);
        WiiMessageSettings_SetRequestRegistrationCode(screen->app);
        GtsServer_RequestSetProfile(&screen->app->request, &screen->app->responseStatus);
        screen->subState++;
        break;
    case 1:
        if (GtsServer_IsRequestDone()) {
            result = GtsServer_GetResult();
            screen->timeoutTimer = 0;
            WiiMessageSettingsScreen_HideWaitingIcon(screen);
            switch (result) {
            case 0:
                switch (screen->app->responseStatus) {
                case 0:
                    switch (screen->app->responseResult) {
                    case 0:
                        screen->subState++;
                        break;
                    case 3:
                        WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00019, 1, 0xF0F);
                        WiiMessageSettingsScreen_SetState(screen, WMS_STATE_WAIT_MESSAGE, WMS_STATE_CONFIRM_CANCEL_REGISTRATION);
                        break;
                    case 1:
                        screen->registrationError = -5006;
                        screen->state = WMS_STATE_SHOW_REGISTRATION_ERROR;
                        break;
                    case 2:
                        screen->registrationError = -5007;
                        screen->state = WMS_STATE_SHOW_REGISTRATION_ERROR;
                        break;
                    default:
                        CommState_ShowRestartErrorAndHalt();
                        break;
                    }
                    break;
                case 1:
                    screen->registrationError = -5010;
                    screen->state = WMS_STATE_SHOW_REGISTRATION_ERROR;
                    break;
                case 2:
                    screen->registrationError = -5011;
                    screen->state = WMS_STATE_SHOW_REGISTRATION_ERROR;
                    break;
                default:
                    GF_ASSERT(0);
                    CommState_ShowRestartErrorAndHalt();
                    break;
                }
                break;
            case 1:
                screen->registrationError = result;
                screen->state = WMS_STATE_SHOW_REGISTRATION_ERROR;
                break;
            case -1:
            case 2:
                screen->registrationError = result;
                screen->state = WMS_STATE_SHOW_REGISTRATION_ERROR;
                break;
            case -15:
            case -12:
                screen->registrationError = result;
                screen->state = WMS_STATE_SHOW_REGISTRATION_ERROR;
                break;
            case -14:
            case -2:
                screen->registrationError = result;
                screen->state = WMS_STATE_SHOW_REGISTRATION_ERROR;
                break;
            default:
                CommState_ShowRestartErrorAndHalt();
                break;
            }
        } else {
            screen->timeoutTimer++;
            if (screen->timeoutTimer == 3600) {
                CommState_ShowRestartErrorAndHalt();
            }
        }
        break;
    case 2:
        WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00020, 1, 0xF0F);
        screen->subState++;
        break;
    case 3:
        if (!WiiMessageSettingsScreen_IsPrinterActive(screen->textPrinterId)) {
            WiiMessageSettingsScreen_SetState(screen, WMS_STATE_DISCONNECT_AND_WAIT, WMS_STATE_START_PASSWORD_ENTRY);
        }
        break;
    }
    return FALSE;
}

#pragma force_active reset

BOOL WiiMessageSettingsScreen_StartPasswordEntry(WiiMessageSettingsScreen *screen) {
    if (WiiMessageSettingsScreen_IsPrinterActive(screen->textPrinterId) == TRUE) {
        return FALSE;
    }

    switch (screen->subState) {
    case 0:
        WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00021, 1, 0xF0F);
        screen->subState++;
        break;
    case 1:
        WiiMessageSettings_SetNextApp(screen->app, WII_MESSAGE_SETTINGS_APP_SET_PASSWORD, WII_MESSAGE_SETTINGS_APP_REGISTRATION_CODE_ENTRY);
        WiiMessageSettings_SetScreenStartState(screen->app, WMS_STATE_SAVE_SETTINGS);
        screen->state = WMS_STATE_FADE_OUT_ONLINE;
        break;
    }
    return FALSE;
}

BOOL WiiMessageSettingsScreen_SaveSettings(WiiMessageSettingsScreen *screen) {
    if (WiiMessageSettingsScreen_IsPrinterActive(screen->textPrinterId) == TRUE) {
        return FALSE;
    }

    switch (screen->subState) {
    case 0:
        WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00024, 1, 0xF0F);
        screen->subState++;
        break;
    case 1:
        WiiMessageSettings_SaveWiiAddress(screen->app);
        WiiMessageSettings_SaveRegistrationCode(screen->app);
        WiiMessageSettings_SavePassword(screen->app);
        WiiMessageSettingsScreen_SetState(screen, WMS_STATE_SAVE_GAME, WMS_STATE_MAIN_MENU);
        break;
    }
    return FALSE;
}

BOOL WiiMessageSettingsScreen_SaveGame(WiiMessageSettingsScreen *screen) {
    switch (screen->subState) {
    case 0:
        WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00032, 0, 0xF0F);
        WiiMessageSettingsScreen_ShowWaitingIcon(screen);
        screen->subState++;
        break;
    case 1:
        if (SaveGameNormal(screen->app->saveData) == WRITE_STATUS_SUCCESS) {
            PlayerProfile *profile = Save_PlayerData_GetProfile(screen->app->saveData);
            BufferPlayersName(screen->msgFormat, 0, profile);
            WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00033, 0, 0xF0F);
            PlaySE(SEQ_SE_DP_SAVE);
            Save_ClearStatusFlags(screen->app->saveData);
        } else {
            WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00034, 0, 0xF0F);
        }
        WiiMessageSettingsScreen_HideWaitingIcon(screen);
        screen->subState++;
        break;
    case 2:
        if (WiiMessageSettingsScreen_IsPrinterActive(screen->textPrinterId) == FALSE) {
            screen->subState++;
        }
        break;
    case 3:
        screen->waitTimer++;
        if (screen->waitTimer > 60) {
            screen->waitTimer = 0;
            screen->state = screen->nextState;
        }
        break;
    }
    return FALSE;
}

BOOL WiiMessageSettingsScreen_DisconnectAndWait(WiiMessageSettingsScreen *screen) {
    if (WiiMessageSettingsScreen_IsPrinterActive(screen->textPrinterId) == TRUE) {
        return FALSE;
    }

    switch (screen->subState) {
    case 0:
        WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00015, 1, 0xF0F);
        screen->subState++;
        break;
    case 1:
        DWC_CleanupInet();
        WirelessStatusIcon_Destroy();
        CommState_FreeWiiMessageWfcSession();
        Sys_ClearSleepDisableFlag(4);
        if (screen->app->serverSessionStarted == TRUE) {
            GtsServer_Exit();
            screen->app->serverSessionStarted = FALSE;
        }
        screen->subState++;
        break;
    case 2:
        if (WiiMessageSettingsScreen_IsPrinterActive(screen->textPrinterId) == FALSE) {
            WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00016, 1, 0xF0F);
            screen->subState++;
        }
        break;
    case 3:
        if (WiiMessageSettingsScreen_IsPrinterActive(screen->textPrinterId) == FALSE) {
            screen->subState++;
        }
        break;
    case 4:
        screen->timer++;
        if (screen->timer > 30) {
            screen->state = screen->nextState;
        }
        break;
    }
    return FALSE;
}

BOOL WiiMessageSettingsScreen_ShowConnectionError(WiiMessageSettingsScreen *screen) {
    s32 msgId = WifiConnection_ClassifyErrorCode(-screen->connectionErrorCode, screen->connectionErrorType);
    WiiMessageSettingsScreen_PrintErrorCode(screen, msgId, -screen->connectionErrorCode);
    screen->state = WMS_STATE_WAIT_CLOSE_ERROR;
    return FALSE;
}

BOOL WiiMessageSettingsScreen_WaitCloseError(WiiMessageSettingsScreen *screen) {
    u32 keys = gSystem.newKeys;

    if ((keys & PAD_BUTTON_A) || (keys & PAD_BUTTON_B)) {
        EraseFrameAndWindow1(&screen->errorWindow, FALSE);
        screen->state = WMS_STATE_MAIN_MENU;
    }
    return FALSE;
}

BOOL WiiMessageSettingsScreen_FadeOut(WiiMessageSettingsScreen *screen) {
    if (screen->state != WMS_STATE_FADE_OUT_ONLINE) {
        CommState_FreeWiiMessageWfcSession();
    }
    WirelessStatusIcon_Destroy();
    WiiMessageSettingsScreen_HideWaitingIcon(screen);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_116);
    screen->state = WMS_STATE_MAIN_MENU;
    return TRUE;
}

BOOL WiiMessageSettingsScreen_ShowYesNoPrompt(WiiMessageSettingsScreen *screen) {
    screen->yesNoPrompt = WiiMessageSettingsScreen_CreateYesNoPrompt(screen->bgConfig, 0x234, 0);
    screen->state = screen->nextState;
    return FALSE;
}

#pragma force_active on

BOOL WiiMessageSettingsScreen_ShowRegistrationError(WiiMessageSettingsScreen *screen) {
    int fromGtsMsgs = 0;
    int msgId;

    switch (screen->registrationError) {
    case 1:
        msgId = msg_0188_00054;
        break;
    case -1:
    case 2:
        msgId = msg_0188_00055;
        break;
    case -14:
    case -2:
        msgId = msg_0188_00057;
        break;
    case -15:
    case -12:
    case -5:
    case -3:
    default:
        msgId = msg_0188_00056;
        break;
    case -5001:
        msgId = msg_0775_00181;
        fromGtsMsgs++;
        break;
    case -5002:
        msgId = msg_0775_00182;
        fromGtsMsgs++;
        break;
    case -5003:
        msgId = msg_0775_00182;
        fromGtsMsgs++;
        break;
    case -5004:
        msgId = msg_0775_00183;
        fromGtsMsgs++;
        break;
    case -5005:
        msgId = msg_0775_00184;
        fromGtsMsgs++;
        break;
    case -5006:
        msgId = msg_0775_00182;
        fromGtsMsgs++;
        break;
    case -5007:
        msgId = msg_0775_00182;
        fromGtsMsgs++;
        break;
    case -5010:
        msgId = msg_0775_00183;
        fromGtsMsgs++;
        break;
    case -5011:
        msgId = msg_0775_00184;
        fromGtsMsgs++;
        break;
    }
    if (!fromGtsMsgs) {
        WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msgId, 1, 0xF0F);
    } else {
        WiiMessageSettingsScreen_PrintMessage(screen, screen->gtsMsgData, msgId, 1, 0xF0F);
    }
    WiiMessageSettingsScreen_SetState(screen, WMS_STATE_WAIT_MESSAGE, WMS_STATE_DISCONNECT_AND_QUIT);
    return FALSE;
}

BOOL WiiMessageSettingsScreen_DisconnectAndQuit(WiiMessageSettingsScreen *screen) {
    switch (screen->subState) {
    case 0:
        WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00015, 1, 0xF0F);
        screen->subState++;
        break;
    case 1:
        if (!WiiMessageSettingsScreen_IsPrinterActive(screen->textPrinterId)) {
            CommState_FreeWiiMessageWfcSession();
            DWC_CleanupInet();
            WirelessStatusIcon_Destroy();
            Sys_ClearSleepDisableFlag(4);
            if (screen->app->serverSessionStarted == TRUE) {
                GtsServer_Exit();
                screen->app->serverSessionStarted = FALSE;
            }
            screen->subState++;
        }
        break;
    case 2:
        WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msg_0188_00016, 1, 0xF0F);
        screen->subState++;
        break;
    case 3:
        if (!WiiMessageSettingsScreen_IsPrinterActive(screen->textPrinterId)) {
            screen->subState++;
        }
        break;
    default:
        screen->timer++;
        if (screen->timer > 30) {
            WiiMessageSettings_Quit(screen->app);
            screen->state = WMS_STATE_FADE_OUT;
        }
        break;
    }
    return FALSE;
}

#pragma force_active reset

BOOL WiiMessageSettingsScreen_WaitMessage(WiiMessageSettingsScreen *screen) {
    if (WiiMessageSettingsScreen_IsPrinterActive(screen->textPrinterId) == FALSE) {
        screen->state = screen->nextState;
    }
    return FALSE;
}

#pragma force_active on

BOOL WiiMessageSettingsScreen_WaitMessageAndDelay(WiiMessageSettingsScreen *screen) {
    if (!WiiMessageSettingsScreen_IsPrinterActive(screen->textPrinterId)) {
        if (screen->timer > 30) {
            screen->state = screen->nextState;
        }
        screen->timer++;
    }
    return FALSE;
}

#pragma force_active reset

BOOL WiiMessageSettingsScreen_WaitMessageAndShowYesNo(WiiMessageSettingsScreen *screen) {
    if (WiiMessageSettingsScreen_IsPrinterActive(screen->textPrinterId) == FALSE) {
        screen->yesNoPrompt = WiiMessageSettingsScreen_CreateYesNoPrompt(screen->bgConfig, 0x234, 0);
        screen->state = screen->nextState;
    }
    return FALSE;
}

// unused: every caller passes 0xF0F
void WiiMessageSettingsScreen_PrintMessage(WiiMessageSettingsScreen *screen, MsgData *msgData, int msgId, u32 textSpeed, u32 unused) {
    String *string = NewString_ReadMsgData(msgData, msgId);
    StringExpandPlaceholders(screen->msgFormat, screen->messageBuffer, string);
    String_Delete(string);
    FillWindowPixelBuffer(&screen->messageWindow, 15);
    DrawFrameAndWindow2(&screen->messageWindow, FALSE, 1, 10);
    screen->textPrinterId = AddTextPrinterParameterized(&screen->messageWindow, 1, screen->messageBuffer, 0, 0, textSpeed, NULL);
    screen->timer = 0;
    if (textSpeed == TEXT_SPEED_NOTRANSFER || textSpeed == TEXT_SPEED_INSTANT) {
        screen->textPrinterId = WII_MESSAGE_SETTINGS_TEXT_PRINTER_NONE;
    }
}

BOOL WiiMessageSettingsScreen_IsPrinterActive(u32 printerId) {
    if (printerId == WII_MESSAGE_SETTINGS_TEXT_PRINTER_NONE || TextPrinterCheckActive(printerId) == FALSE) {
        return FALSE;
    }
    return TRUE;
}

#pragma force_active on

BOOL WiiMessageSettingsScreen_PrintWiiNumberReentry(WiiMessageSettingsScreen *screen) {
    if (WiiMessageSettingsScreen_IsPrinterActive(screen->textPrinterId) == TRUE) {
        return FALSE;
    }
    switch (screen->subState) {
    case 0:
        WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, WiiMessageSettings_GetWiiNumberEntryState(screen->app) == WII_NUMBER_ENTRY_ENTERED_ONCE ? msg_0188_00038 : msg_0188_00039, 1, 0xF0F);
        screen->subState++;
        break;
    case 1:
        screen->state = WMS_STATE_START_WII_NUMBER_ENTRY;
        break;
    }
    return FALSE;
}

BOOL WiiMessageSettingsScreen_CheckPassword(WiiMessageSettingsScreen *screen) {
    WiiMessageSettings *app = screen->app;

    if (WiiMessageSettingsScreen_IsPrinterActive(screen->textPrinterId) == TRUE) {
        return FALSE;
    }
    switch (screen->subState) {
    case 0: {
        int msgId;
        int password = WiiMessageSettings_GetPassword(app);
        if (password == Save_WiiMessage_GetParam(app->saveData, 3)) {
            u32 groups[4];
            u32 i;
            WiiNumber_ParseDigitGroups(Save_WiiMessage_Get(app->saveData)->wiiAddress, groups, HEAP_ID_115);
            for (i = 0; i < 4; i++) {
                BufferIntegerAsString(screen->msgFormat, i, groups[i], 4, PRINTING_MODE_LEADING_ZEROS, TRUE);
            }
            msgId = msg_0188_00041;
        } else {
            msgId = msg_0188_00040;
        }
        WiiMessageSettingsScreen_PrintMessage(screen, screen->msgData, msgId, 1, 0xF0F);
        screen->subState++;
        break;
    }
    case 1:
        screen->state = WMS_STATE_MAIN_MENU;
        break;
    }
    return FALSE;
}

#pragma force_active reset

// alignment 0: left at x, 1: centered, 2: right aligned
int WiiMessageSettingsScreen_GetAlignedX(Window *window, String *string, int x, int alignment, u32 unused, u32 fontId) {
    switch (alignment) {
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

#pragma force_active on

int WiiMessageSettingsScreen_PrintAligned(Window *window, String *string, int x, int y, int alignment, u32 color) {
    int alignedX = WiiMessageSettingsScreen_GetAlignedX(window, string, x, alignment, color, 1);
    return AddTextPrinterParameterizedWithColor(window, 1, string, alignedX, y, TEXT_SPEED_INSTANT, color, NULL);
}

#pragma force_active reset

void WiiMessageSettingsScreen_PrintError(WiiMessageSettingsScreen *screen, int msgId) {
    String *string = String_New(256, HEAP_ID_116);
    ReadMsgDataIntoString(screen->wfcErrorMsgData, msgId, string);
    StringExpandPlaceholders(screen->msgFormat, screen->errorBuffer, string);
    FillWindowPixelBuffer(&screen->errorWindow, 15);
    DrawFrameAndWindow1(&screen->errorWindow, TRUE, 31, 11);
    AddTextPrinterParameterized(&screen->errorWindow, 1, screen->errorBuffer, 0, 0, TEXT_SPEED_INSTANT, NULL);
    screen->textPrinterId = WII_MESSAGE_SETTINGS_TEXT_PRINTER_NONE;
    String_Delete(string);
}

void WiiMessageSettingsScreen_PrintErrorCode(WiiMessageSettingsScreen *screen, int msgId, int errorCode) {
    if (msgId == -1) {
        msgId = msg_0800_00011;
    }
    BufferIntegerAsString(screen->msgFormat, 0, errorCode, 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
    ClearFrameAndWindow2(&screen->messageWindow, TRUE);
    WiiMessageSettingsScreen_PrintError(screen, msgId);
}

void WiiMessageSettingsScreen_RegisteredMenuMoveCursor(ListMenu *menu, s32 index, BOOL onInit) {
    if (onInit == FALSE) {
        PlaySE(SEQ_SE_DP_SELECT);
    }
}

void WiiMessageSettingsScreen_UnregisteredMenuMoveCursor(ListMenu *menu, s32 index, BOOL onInit) {
    if (onInit == FALSE) {
        PlaySE(SEQ_SE_DP_SELECT);
    }
}
