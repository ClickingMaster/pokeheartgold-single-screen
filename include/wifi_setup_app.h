#ifndef POKEHEARTGOLD_WIFI_SETUP_APP_H
#define POKEHEARTGOLD_WIFI_SETUP_APP_H

#include "wifi_setup.h"
#include "wifi_setup_ui.h"

// Access-point setup screens (ARM code in overlay 13), application core: the entry point the game
// calls, system and display start-up and shutdown, the per-frame main loop, the setup client
// request, the text banks and the session started from the screens.

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
    s8 language;                     // 0x00, 0-6
    u8 exit;                         // 0x01, ends the main loop
    u8 unk_02[2];                    // 0x02
    void *heap;                      // 0x04, work memory handed in by the caller
    WifiSetupAppFlags flags;         // 0x08
    WifiSetupAppStateFunc stateFunc; // 0x0C, called once per frame
    int unk_10;                      // 0x10
    int unk_14;                      // 0x14
    int unk_18;                      // 0x18
    int unk_1C;                      // 0x1C
} WifiSetupApp;

typedef struct WifiSetupClient {
    u8 done;                   // 0x00, the request finished successfully
    u8 unk_01[3];              // 0x01
    WifiSetupRequest *request; // 0x04
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
    u8 share[0x1E280]; // WifiSetupShare (wifi_setup_share.h)
    u8 childUser[0x18];
    WifiSetupSessionCallback callback; // 0x1E298
    void *task;                        // 0x1E29C
    u8 missedEvent;                      // 0x1E2A0, an event arrived while there was no callback
    u8 cancelled;                      // 0x1E2A1
} WifiSetupSession;

// Parameters of the session (WifiSetupShare_Init); the first part is also the Download Play parent's
// file description (WifiSetupMbParentFile).
typedef struct WifiSetupSessionParams {
    const char *filePath;
    const u16 *title;
    const u16 *description;
    int unk_0C;
    int unk_10;
    u32 ggid;
    u8 languageCode; // the session's copy holds 2 here
} WifiSetupSessionParams;

extern WifiSetupClient gWifiSetupClient;
extern WifiSetupRequest *ov13_0224F4C8; // = gWifiSetupClient.request
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
void *WifiSetupApp_AossClientAlloc(u32 size);
void WifiSetupApp_AossClientFree(void *ptr);
BOOL WifiSetupApp_SetParams(int language, u32 flags);
void WifiSetupApp_InitSystem(void);
void WifiSetupApp_InitDisplay(void);
void WifiSetupApp_Shutdown(void);
void WifiSetupApp_SetStateFunc(WifiSetupAppStateFunc func);
void WifiSetupApp_SetTransition(int a, int b);
void WifiSetupApp_GetTransition(int *a, int *b);
void WifiSetupApp_SetScreenArgs(int a, int b);
void WifiSetupApp_GetScreenArgs(int *a, int *b);
u8 WifiSetupApp_GetLanguage(void);
u32 WifiSetupApp_GetKind(void);
BOOL WifiSetupApp_HasOption(u32 mask);
void WifiSetupApp_Exit(void);
u8 WifiSetupApp_GetLanguageCode(void);

void WifiSetupText_InitBanks(void);
void WifiSetupText_FreeBanks(void);
WifiSetupTextBank *WifiSetupText_LoadBank(void *path);
void WifiSetupText_UnloadBank(WifiSetupTextBank *bank);
u16 *WifiSetupText_GetString(WifiSetupTextBank *bank, int index);
u16 *WifiSetupText_GetStringWithDigit(WifiSetupTextBank *bank, u32 index, int pos, u32 digit);

void WifiSetupTransfer_Start(WifiSetupSessionCallback callback);
void WifiSetupTransfer_Cancel(void);
void WifiSetupTransfer_CancelTask(void *task);
BOOL WifiSetupTransfer_IsFinished(void);
void WifiSetupTransfer_SetCallback(WifiSetupSessionCallback callback);
void WifiSetupTransfer_StartDownload(void);
u8 *WifiSetupTransfer_GetChildUser(void);
void WifiSetupTransfer_Task(void *task);

#endif // POKEHEARTGOLD_WIFI_SETUP_APP_H
