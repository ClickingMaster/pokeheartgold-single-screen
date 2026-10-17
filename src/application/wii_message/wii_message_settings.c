#include "global.h"

#include "constants/sndseq.h"

#include "application/wii_message/wii_message_settings_internal.h"
#include "application/wii_message/wii_number.h"
#include "msgdata/msg/msg_0038.h"

#include "code_entry.h"
#include "comm_state.h"
#include "comm_wireless.h"
#include "network_overlay_loader.h"
#include "poke_overlay.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sys_flags.h"
#include "title_screen.h"

FS_EXTERN_OVERLAY(OVY_44);
FS_EXTERN_OVERLAY(OVY_70);

typedef struct WiiMessageSettingsAppEntry {
    void *(*createArgs)(WiiMessageSettings *app);
    void (*destroyArgs)(WiiMessageSettings *app);
    const OverlayManagerTemplate *template;
    u32 unloadNetwork;
} WiiMessageSettingsAppEntry;

BOOL LoadOVY38(void);
void UnloadOVY38(void);
void CommWireless_Terminate(void);
void WirelessStatusIcon_SetLevel(int level);
void DWC_SetMemFunc(void *(*alloc)(int, u32, int), void (*free)(int, void *));
int DWC_GetLinkLevel(void);
void DWC_UpdateConnection(void);
void GtsServer_Update(void);

static NNSFndHeapHandle sWiiMessageSettingsHeap;

static void WiiMessageSettings_LoadNetworkOverlays(WiiMessageSettings *app);
static void WiiMessageSettings_UnloadNetworkOverlays(WiiMessageSettings *app);
static void *WiiMessageSettings_Alloc(int unused, u32 size, int alignment);
static void WiiMessageSettings_Free(int unused, void *ptr);
static void WiiMessageSettings_SetRegistrationCodePrefix(WiiMessageSettings *app, int prefix);
static void WiiMessageSettings_SetWiiNumberEntryState(WiiMessageSettings *app, u8 entryState);
void *WiiMessageSettings_CreateScreenArgs(WiiMessageSettings *app);
void WiiMessageSettings_DestroyScreenArgs(WiiMessageSettings *app);
void *WiiMessageSettings_CreateWiiNumberEntryArgs(WiiMessageSettings *app);
void WiiMessageSettings_FinishWiiNumberEntry(WiiMessageSettings *app);
void *WiiMessageSettings_CreateRegistrationCodeEntryArgs(WiiMessageSettings *app);
void WiiMessageSettings_FinishRegistrationCodeEntry(WiiMessageSettings *app);
void *WiiMessageSettings_CreatePasswordEntryArgs(WiiMessageSettings *app);
void WiiMessageSettings_FinishPasswordEntry(WiiMessageSettings *app);
void *WiiMessageSettings_CreateWifiClubArgs(WiiMessageSettings *app);
void WiiMessageSettings_FinishWifiClub(WiiMessageSettings *app);

static const OverlayManagerTemplate sWiiMessageSettingsScreenTemplate = { WiiMessageSettingsScreen_Init, WiiMessageSettingsScreen_Main, WiiMessageSettingsScreen_Exit, FS_OVERLAY_ID_NONE };
static const OverlayManagerTemplate sWiiMessageSettingsWifiClubTemplate = { WifiClub_Init, WifiClub_Main, WifiClub_Exit, FS_OVERLAY_ID(OVY_44) };

static const WiiMessageSettingsAppEntry sWiiMessageSettingsApps[] = {
    [WII_MESSAGE_SETTINGS_APP_SCREEN] = { WiiMessageSettings_CreateScreenArgs,                WiiMessageSettings_DestroyScreenArgs,           &sWiiMessageSettingsScreenTemplate,   FALSE },
    [WII_MESSAGE_SETTINGS_APP_WII_NUMBER_ENTRY] = { WiiMessageSettings_CreateWiiNumberEntryArgs,        WiiMessageSettings_FinishWiiNumberEntry,        &gOverlayTemplate_CodeEntry,          FALSE },
    [WII_MESSAGE_SETTINGS_APP_REGISTRATION_CODE_ENTRY] = { WiiMessageSettings_CreateRegistrationCodeEntryArgs, WiiMessageSettings_FinishRegistrationCodeEntry, &gOverlayTemplate_CodeEntry,          FALSE },
    [WII_MESSAGE_SETTINGS_APP_SET_PASSWORD] = { WiiMessageSettings_CreatePasswordEntryArgs,         WiiMessageSettings_FinishPasswordEntry,         &gOverlayTemplate_CodeEntry,          FALSE },
    [WII_MESSAGE_SETTINGS_APP_CHECK_PASSWORD] = { WiiMessageSettings_CreatePasswordEntryArgs,         WiiMessageSettings_FinishPasswordEntry,         &gOverlayTemplate_CodeEntry,          FALSE },
    [WII_MESSAGE_SETTINGS_APP_OV44] = { WiiMessageSettings_CreateWifiClubArgs,              WiiMessageSettings_FinishWifiClub,              &sWiiMessageSettingsWifiClubTemplate, TRUE  },
};

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

static void WiiMessageSettings_LoadNetworkOverlays(WiiMessageSettings *app) {
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

static void WiiMessageSettings_UnloadNetworkOverlays(WiiMessageSettings *app) {
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

static void *WiiMessageSettings_Alloc(int unused, u32 size, int alignment) {
    OSIntrMode intrMode = OS_DisableInterrupts();
    void *ptr = NNS_FndAllocFromExpHeapEx(sWiiMessageSettingsHeap, size, alignment);
    OS_RestoreInterrupts(intrMode);
    return ptr;
}

static void WiiMessageSettings_Free(int unused, void *ptr) {
    if (ptr) {
        OSIntrMode intrMode = OS_DisableInterrupts();
        NNS_FndFreeToExpHeap(sWiiMessageSettingsHeap, ptr);
        OS_RestoreInterrupts(intrMode);
    }
}

void WiiMessageSettings_SaveWiiAddress(WiiMessageSettings *app) {
    char *address = Heap_Alloc(HEAP_ID_115, 100);
    WiiNumber_ToWiiAddress(app->wiiNumber, address, HEAP_ID_115);
    Save_WiiMessage_SetWiiAddress(app->saveData, address);
    Heap_Free(address);
}

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

static void WiiMessageSettings_SetRegistrationCodePrefix(WiiMessageSettings *app, int prefix) {
    app->registrationCodePrefix = prefix;
}

u32 WiiMessageSettings_GetRegistrationCode(WiiMessageSettings *app) {
    return app->registrationCode;
}

u32 WiiMessageSettings_GetPassword(WiiMessageSettings *app) {
    return app->password;
}

static void WiiMessageSettings_SetWiiNumberEntryState(WiiMessageSettings *app, u8 entryState) {
    app->wiiNumberEntryState = entryState;
}

u8 WiiMessageSettings_GetWiiNumberEntryState(WiiMessageSettings *app) {
    return app->wiiNumberEntryState;
}

void *WiiMessageSettings_CreateScreenArgs(WiiMessageSettings *app) {
    return app;
}

void WiiMessageSettings_DestroyScreenArgs(WiiMessageSettings *app) {
}

// The Wii number: 16 digits in groups of 4.
void *WiiMessageSettings_CreateWiiNumberEntryArgs(WiiMessageSettings *app) {
    int groups[4];
    groups[0] = 4;
    groups[1] = 4;
    groups[2] = 4;
    groups[3] = 4;
    app->childArgs = CodeEntryArgs_New(HEAP_ID_115, 16, groups, Save_PlayerData_GetOptionsAddr(app->saveData), msg_0038_00004, 0);
    return app->childArgs;
}

void WiiMessageSettings_FinishWiiNumberEntry(WiiMessageSettings *app) {
    CodeEntryArgs *args = app->childArgs;
    if (WiiNumber_IsZero(args->text, HEAP_ID_115)) {
        WiiMessageSettings_SetWiiNumberEntryState(app, WII_NUMBER_ENTRY_CANCELLED);
    } else if (WiiMessageSettings_GetWiiNumberEntryState(app) == WII_NUMBER_ENTRY_ENTERED_ONCE) {
        if (String_Compare(app->wiiNumber, args->text)) {
            WiiMessageSettings_SetWiiNumberEntryState(app, WII_NUMBER_ENTRY_MISMATCH);
        } else {
            WiiMessageSettings_SetWiiNumberEntryState(app, WII_NUMBER_ENTRY_CONFIRMED);
        }
    } else {
        String_Copy(app->wiiNumber, args->text);
        WiiMessageSettings_SetWiiNumberEntryState(app, WII_NUMBER_ENTRY_ENTERED_ONCE);
    }
    CodeEntryArgs_Delete(args);
    WiiMessageSettings_SetNextApp(app, WII_MESSAGE_SETTINGS_APP_SCREEN, 0);
}

// The registration code: the 3-digit prefix sent with the request, then the 4 digits the player enters.
void *WiiMessageSettings_CreateRegistrationCodeEntryArgs(WiiMessageSettings *app) {
    int groups[4];
    groups[0] = 3;
    groups[1] = 4;
    groups[2] = 0;
    groups[3] = 0;
    app->childArgs = CodeEntryArgs_NewWithFixedDigits(HEAP_ID_115, 7, groups, Save_PlayerData_GetOptionsAddr(app->saveData), msg_0038_00005, 1, 1, app->registrationCodePrefix);
    return app->childArgs;
}

void WiiMessageSettings_FinishRegistrationCodeEntry(WiiMessageSettings *app) {
    BOOL success;
    CodeEntryArgs *args = app->childArgs;
    app->registrationCode = (u64)String_atoi(args->text, &success) % 10000;
    if (!success) {
        GF_AssertFail();
    }
    CodeEntryArgs_Delete(args);
    WiiMessageSettings_SetNextApp(app, WII_MESSAGE_SETTINGS_APP_SCREEN, 0);
}

void *WiiMessageSettings_CreatePasswordEntryArgs(WiiMessageSettings *app) {
    int groups[4];
    groups[0] = 4;
    groups[1] = 0;
    groups[2] = 0;
    groups[3] = 0;
    app->childArgs = CodeEntryArgs_New(HEAP_ID_115, 4, groups, Save_PlayerData_GetOptionsAddr(app->saveData), msg_0038_00006, 0);
    return app->childArgs;
}

void WiiMessageSettings_FinishPasswordEntry(WiiMessageSettings *app) {
    BOOL success;
    CodeEntryArgs *args = app->childArgs;
    app->password = String_atoi(args->text, &success);
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
