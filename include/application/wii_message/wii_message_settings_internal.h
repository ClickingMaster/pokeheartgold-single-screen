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
    OverlayManager *child;      // 0x000
    SaveData *saveData;         // 0x004
    Options *options;           // 0x008
    void *expHeapMemory;        // 0x00C
    NNSFndHeapHandle expHeap;   // 0x010
    u8 netConnection[0x64];     // 0x014, passed to DWC_InitInetEx when connecting
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
    WifiServerProfile request;  // 0x098
    int responseStatus;         // 0x0FC, written by GtsServer_RequestSetProfile
    int responseResult;         // 0x100
    u32 password;               // 0x104
    u32 registrationCode;       // 0x108, last 4 digits; -1: entry cancelled
    u8 wiiNumberEntryState;     // 0x10C
    String *wiiNumber;          // 0x110, the first of the two Wii number entries
    String *unk_114;            // 0x114
    u32 serverSessionStarted;   // 0x118
} WiiMessageSettings;           // size: 0x11C

void WiiMessageSettings_SetNextApp(WiiMessageSettings *app, int nextApp, int unused);
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
