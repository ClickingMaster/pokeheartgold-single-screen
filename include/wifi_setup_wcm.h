#ifndef POKEHEARTGOLD_WIFI_SETUP_WCM_H
#define POKEHEARTGOLD_WIFI_SETUP_WCM_H

#include "global.h"

// Wireless connection manager (WCM_*) wrapper of the access-point setup client. The client carries
// two copies of it: src/wifi_setup_wcm.c (first module, state gWifiSetupWcm) and
// src/wifi_setup_link.c (second module, state inside WifiSetupWork).
//
// Wrapper states: 0 none, 1 initialised, 2 cleaning up, 3 idle, 4 starting up / stopping,
// 5 searching, 6 search starting, 7 connected, 8 connecting.

// Event passed to the WCM callback
typedef struct WifiSetupWcmEvent {
    s16 type;
    s16 result;
} WifiSetupWcmEvent;

// Start-up parameters handed to WCM_StartupAsync
typedef struct WifiSetupWcmConfig {
    u32 unk_00;     // 0x00, always 3
    void *buffer;   // 0x04, rest of the caller's allocation
    u32 bufferSize; // 0x08
    u32 unk_0C;     // 0x0C, always 0
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
