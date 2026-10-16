#ifndef POKEHEARTGOLD_WIFI_SETUP_WCM_H
#define POKEHEARTGOLD_WIFI_SETUP_WCM_H

#include "global.h"

// Wireless connection manager (WCM_*) wrapper of the access-point setup client. The client carries
// two copies of it: src/wifi_setup_wcm.c (first module, state gWifiSetupWcm) and
// src/wifi_setup_link.c (second module, state inside WifiSetupWork).
//
// wrapper states (gWifiSetupWcm.state)
#define WIFI_SETUP_WCM_STATE_NONE            0
#define WIFI_SETUP_WCM_STATE_READY           1 // WCM_Init done
#define WIFI_SETUP_WCM_STATE_CLEANUP         2
#define WIFI_SETUP_WCM_STATE_IDLE            3
#define WIFI_SETUP_WCM_STATE_BUSY            4 // starting up, ending a search or disconnecting
#define WIFI_SETUP_WCM_STATE_SEARCHING       5
#define WIFI_SETUP_WCM_STATE_SEARCH_STARTING 6
#define WIFI_SETUP_WCM_STATE_CONNECTED       7
#define WIFI_SETUP_WCM_STATE_CONNECTING      8

// events passed to the wrapper's callback
#define WIFI_SETUP_WCM_EVENT_UNKNOWN           1
#define WIFI_SETUP_WCM_EVENT_FAILED            2 // a request could not be started
#define WIFI_SETUP_WCM_EVENT_FOUND_AP          5
#define WIFI_SETUP_WCM_EVENT_STARTED           6
#define WIFI_SETUP_WCM_EVENT_SEARCH_STARTED    8
#define WIFI_SETUP_WCM_EVENT_SEARCH_FAILED     9
#define WIFI_SETUP_WCM_EVENT_SEARCH_ENDED      10
#define WIFI_SETUP_WCM_EVENT_SEARCH_END_FAILED 11
#define WIFI_SETUP_WCM_EVENT_CONNECTED         12
#define WIFI_SETUP_WCM_EVENT_CONNECT_FAILED    13
#define WIFI_SETUP_WCM_EVENT_DISCONNECTED      14
#define WIFI_SETUP_WCM_EVENT_DISCONNECT_FAILED 15
#define WIFI_SETUP_WCM_EVENT_CLEANED_UP        20

// Event passed to the WCM callback
typedef struct WifiSetupWcmEvent {
    s16 type;
    s16 result;
} WifiSetupWcmEvent;

// Start-up parameters handed to WCM_StartupAsync
typedef struct WifiSetupWcmConfig {
    u32 dmaNo;      // 0x00, always 3
    void *buffer;   // 0x04, rest of the caller's allocation
    u32 bufferSize; // 0x08
    u32 apListMode; // 0x0C, always 0
} WifiSetupWcmConfig;

// gWifiSetupWcm (0x28 bytes); followed by its search filter buffers gWifiSetupWcmSearchBssid (BSSID) and
// gWifiSetupWcmSearchSsid (SSID)
typedef struct WifiSetupWcm {
    void *wcmWork;                        // 0x00, WCM_Init work area
    int searchOption;                     // 0x04
    WifiSetupWcmConfig *config;           // 0x08
    int connectOption;                    // 0x0C
    void *searchBssid;                    // 0x10
    void *searchSsid;                     // 0x14
    int state;                            // 0x18
    void (*callback)(int event, int arg); // 0x1C
    void *connectAp;                      // 0x20, access point description (0xC0 bytes)
    void *connectKey;                     // 0x24, key description (0x50 bytes)
} WifiSetupWcm;

void WifiSetup_Sleep(s32 ticks);

int WCM_Init(void *work, u32 size);
int WCM_StartupAsync(WifiSetupWcmConfig *config, void (*callback)(WifiSetupWcmEvent *event));
int WCM_CleanupAsync(void);
void WCM_Finish(void);
int WCM_SearchAsync(void *bssid, void *ssid, int option);
int WCM_ConnectAsync(void *ap, void *key, int option);
int WCM_DisconnectAsync(void);
int WCM_LockApList(int lock);
int WCM_CountApList(void);
void *WCM_PointApList(u16 index);

#endif // POKEHEARTGOLD_WIFI_SETUP_WCM_H
