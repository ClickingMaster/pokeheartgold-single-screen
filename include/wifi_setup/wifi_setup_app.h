#ifndef POKEHEARTGOLD_WIFI_SETUP_APP_H
#define POKEHEARTGOLD_WIFI_SETUP_APP_H

#include "wifi_setup/wifi_setup.h"
#include "wifi_setup/wifi_setup_ui.h"

// Access-point setup screens, application core: the entry point the game calls, the per-frame main loop, the setup
// client request, the text banks and the session started from the screens.

// results of WifiSetupApp_PollAossClient
#define WIFI_SETUP_CLIENT_BUSY   0
#define WIFI_SETUP_CLIENT_DONE   1
#define WIFI_SETUP_CLIENT_FAILED 2

typedef void (*WifiSetupAppStateFunc)(void);

typedef union WifiSetupAppFlags {
    u32 raw;

    struct {
        u32 kind : 4;     // low bits of the caller's flags
        u32 options : 28; // the other bits
    } bits;
} WifiSetupAppFlags;

typedef struct WifiSetupApp {
    s8 language; // 0-6
    u8 exit;     // ends the main loop
    u8 unk_02[2];
    void *heap; // work memory handed in by the caller
    WifiSetupAppFlags flags;
    WifiSetupAppStateFunc stateFunc; // called once per frame
    int bannerMode;
    int buttonBarMode;
    int screenArg0;
    int screenArg1;
} WifiSetupApp;

typedef struct WifiSetupClient {
    u8 done; // the request finished successfully
    u8 unk_01[3];
    WifiSetupRequest *request;
} WifiSetupClient;

// A loaded text bank: a table of offsets into a block of u16 strings.
typedef struct WifiSetupTextBank {
    u32 *offsets;
    u8 *strings;
    void *file;
} WifiSetupTextBank;

typedef struct WifiSetupTextBanks {
    WifiSetupTextBank items[8];
    WifiSetupPool *pool;
} WifiSetupTextBanks;

typedef void (*WifiSetupSessionCallback)(int event);

typedef struct WifiSetupSession {
    u8 share[0x1E280]; // WifiSetupShare
    u8 childUser[0x18];
    WifiSetupSessionCallback callback;
    void *task;
    u8 missedEvent; // an event arrived while there was no callback
    u8 cancelled;
} WifiSetupSession;

// Parameters of the session (WifiSetupShare_Init); the first part is also the Download Play parent's
// file description (WifiSetupMbParentFile).
typedef struct WifiSetupSessionParams {
    const char *filePath;
    const u16 *title;
    const u16 *description;
    int iconCharPath;
    int iconPalettePath;
    u32 ggid;
    u8 languageCode; // the session's copy holds 2 here
} WifiSetupSessionParams;

extern WifiSetupClient gWifiSetupClient;
extern WifiSetupRequest *ov13_0224F4C8;
extern WifiSetupApp gWifiSetupApp;
extern WifiSetupTextBanks *gWifiSetupTextBanks;
extern WifiSetupSession *gWifiSetupSession;
extern WifiSetupTextBank *gWifiSetupTextBank;
extern const u8 gWifiSetupAossClientName[12];
extern const WifiSetupSessionParams gWifiSetupTransferSessionParams;

int WifiSetupApp_Run(void *heap, s32 language, s32 flags);
void WifiSetupApp_StartAossClient(void);
void WifiSetupApp_StopAossClient(BOOL apply);
int WifiSetupApp_PollAossClient(void);
void WifiSetupApp_SetStateFunc(WifiSetupAppStateFunc func);
void WifiSetupApp_SetTransition(int a, int b);
void WifiSetupApp_GetTransition(int *a, int *b);
void WifiSetupApp_SetScreenArgs(int a, int b);
void WifiSetupApp_GetScreenArgs(int *a, int *b);
u8 WifiSetupApp_GetLanguage(void);
u32 WifiSetupApp_GetKind(void);
BOOL WifiSetupApp_HasOption(u32 mask);
void WifiSetupApp_Exit(void);

void WifiSetupText_InitBanks(void);
void WifiSetupText_FreeBanks(void);
WifiSetupTextBank *WifiSetupText_LoadBank(void *path);
void WifiSetupText_UnloadBank(WifiSetupTextBank *bank);
u16 *WifiSetupText_GetString(WifiSetupTextBank *bank, int index);
u16 *WifiSetupText_GetStringWithDigit(WifiSetupTextBank *bank, u32 index, int pos, u32 digit);

void WifiSetupTransfer_Start(WifiSetupSessionCallback callback);
void WifiSetupTransfer_Cancel(void);
BOOL WifiSetupTransfer_IsFinished(void);
void WifiSetupTransfer_SetCallback(WifiSetupSessionCallback callback);
void WifiSetupTransfer_StartDownload(void);
u8 *WifiSetupTransfer_GetChildUser(void);

#endif // POKEHEARTGOLD_WIFI_SETUP_APP_H
