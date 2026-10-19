#ifndef POKEHEARTGOLD_WIFI_SETUP_ACCESS_POINTS_H
#define POKEHEARTGOLD_WIFI_SETUP_ACCESS_POINTS_H

#include <nitro/wm/wm_internal.h>

#include "global.h"

#include "wifi_setup/wifi_setup_connection.h"
#include "wifi_setup/wifi_setup_internal.h"

// Access-point data of the access-point setup (ARM code in overlay 13): the scan for nearby access
// points, the saved connection settings (four 0x100-byte backup pages, the first three editable)
// with an edit copy of one of them, and the sound archive.

#define WIFI_SETUP_NUM_SCAN_ENTRIES 20
#define WIFI_SETUP_NUM_CONNECTIONS  4

// One access point found by the scan.
typedef struct WifiSetupScanEntry {
    u8 ssid[0x20];
    u8 bssid[6];
    u16 linkLevel;
    u8 security; // 0 open, 1 WEP, 2 WPA
    u8 pad29;
} WifiSetupScanEntry;

// WM work of the scan, followed by the scan state.
typedef struct WifiSetupApScan {
    u8 wmWork[0xF00];
    u8 scanBuf[0x400];
    WifiSetupScanEntry entries[WIFI_SETUP_NUM_SCAN_ENTRIES];
    WmScanExParam param;
    WmStatus status;
    u8 ending;
} WifiSetupApScan;

typedef struct WifiSetupConnections {
    WifiSetupConnectionPage pages[WIFI_SETUP_NUM_CONNECTIONS];
    WifiSetupConnection edit; // 0x400
    MATHCRC16Table crcTable;  // 0x4F8
} WifiSetupConnections;

typedef struct WifiSetupSound {
    NNSSndArc arc;
    NNSSndHandle handle;
    void *data; // sound archive file
    void *task;
} WifiSetupSound;

extern WifiSetupApScan *gWifiSetupApScan;
extern WifiSetupConnections *gWifiSetupConnections;
extern WifiSetupSound *gWifiSetupSound;

int WifiSetupApScan_Start(void);
int WifiSetupApScan_End(void);
int WifiSetupApScan_GetEntries(WifiSetupScanEntry **out);
void WifiSetupConnections_Load(void);
void WifiSetupConnections_Free(void);
WifiSetupConnection *WifiSetupConnections_GetEdit(void);
void WifiSetupConnections_SetAutoAddress(u8 on);
void WifiSetupConnections_SetAutoDns(u8 on);
void WifiSetupConnections_SetSsid(const void *ssid);
void WifiSetupConnections_SetWepKey(const char *key);
void WifiSetupConnections_SetIpAddress(const char *text);
void WifiSetupConnections_SetSubnetMask(const char *text);
void WifiSetupConnections_SetGateway(const char *text);
void WifiSetupConnections_SetPrimaryDns(const char *text);
void WifiSetupConnections_SetSecondaryDns(const char *text);
void WifiSetupConnections_GetSsid(void *dst);
void WifiSetupConnections_FormatIpAddress(char *buf);
void WifiSetupConnections_FormatSubnetMask(char *buf);
void WifiSetupConnections_FormatGateway(char *buf);
void WifiSetupConnections_FormatPrimaryDns(char *buf);
void WifiSetupConnections_FormatSecondaryDns(char *buf);
u8 WifiSetupConnections_GetSetupMethod(int index);
void WifiSetupConnections_Edit(int index);
void WifiSetupConnections_SaveEdit(void);
void WifiSetupConnections_SetFromClientResult(WifiSetupResult *result);
void WifiSetupConnections_SetFromInfo(u8 *info);
WifiSetupConnections *WifiSetupConnections_Get(void);
void WifiSetupConnections_Erase(int index);
void WifiSetupConnections_Reset(void);
void WifiSetup_ParseAddress(const char *text, u8 *address);
void WifiSetupSound_Init(void);
void WifiSetupSound_Free(void);

#endif // POKEHEARTGOLD_WIFI_SETUP_ACCESS_POINTS_H
