#ifndef POKEHEARTGOLD_APPLICATION_WII_MESSAGE_WII_MESSAGE_SETTINGS_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_WII_MESSAGE_WII_MESSAGE_SETTINGS_INTERNAL_H

#include "global.h"

#include "options.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "save.h"
#include "save_wii_message.h"

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

// Data of the Wii message settings app (OverlayManager data, HEAP_ID_115).
typedef struct WiiMessageSettings {
    OverlayManager *child;
    SaveData *saveData;
    Options *options;
    void *expHeapMemory;
    NNSFndHeapHandle expHeap;
    u8 netConnection[0x64]; // passed to DWC_InitInetEx when connecting
    u16 registrationCodePrefix;
    u8 screenStartState;
    u8 unk_7B;
    u32 networkLoaded;
    u32 currentApp;
    u32 unk_84;
    u32 nextApp;
    u32 followingApp; // the app after nextApp
    u32 unk_90;
    void *childArgs;
    WifiServerProfile request;
    int responseStatus; // written by GtsServer_RequestSetProfile
    int responseResult;
    u32 password;
    u32 registrationCode; // last 4 digits; -1: entry cancelled
    u8 wiiNumberEntryState;
    String *wiiNumber; // the first of the two Wii number entries
    String *unk_114;
    u32 serverSessionStarted;
} WiiMessageSettings;

void WiiMessageSettings_SetNextApp(WiiMessageSettings *app, int nextApp, int followingApp);
void WiiMessageSettings_Quit(WiiMessageSettings *app);
void WiiMessageSettings_SetScreenStartState(WiiMessageSettings *app, int state);
u8 WiiMessageSettings_GetScreenStartState(WiiMessageSettings *app);
void WiiMessageSettings_SaveWiiAddress(WiiMessageSettings *app);
void WiiMessageSettings_SaveRegistrationCode(WiiMessageSettings *app);
void WiiMessageSettings_SavePassword(WiiMessageSettings *app);
void WiiMessageSettings_InitRequest(WiiMessageSettings *app);
void WiiMessageSettings_SetRequestWiiAddress(WiiMessageSettings *app);
void WiiMessageSettings_SetRequestRegistrationCode(WiiMessageSettings *app);
u32 WiiMessageSettings_GetRegistrationCode(WiiMessageSettings *app);
u32 WiiMessageSettings_GetPassword(WiiMessageSettings *app);
u8 WiiMessageSettings_GetWiiNumberEntryState(WiiMessageSettings *app);
BOOL WiiMessageSettingsScreen_Init(OverlayManager *man, int *state);
BOOL WiiMessageSettingsScreen_Main(OverlayManager *man, int *state);
BOOL WiiMessageSettingsScreen_Exit(OverlayManager *man, int *state);

BOOL WiiMessageSettings_Init(OverlayManager *man, int *state);
BOOL WiiMessageSettings_Main(OverlayManager *man, int *state);
BOOL WiiMessageSettings_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_APPLICATION_WII_MESSAGE_WII_MESSAGE_SETTINGS_INTERNAL_H
