#ifndef POKEHEARTGOLD_WIFI_SETUP_ACCESS_POINTS_H
#define POKEHEARTGOLD_WIFI_SETUP_ACCESS_POINTS_H

#include "global.h"

#include "wifi_setup_connection.h"

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

typedef struct WifiSetupConnections {
    WifiSetupConnectionPage pages[WIFI_SETUP_NUM_CONNECTIONS];
    WifiSetupConnection edit; // 0x400
    MATHCRC16Table crcTable;  // 0x4F8
} WifiSetupConnections;

// Result of the setup client (see WifiSetupClient_StoreResult).
typedef struct WifiSetupClientResultData {
    u8 ssid[0x20];
    int keyType;
    int unk_24;
    u8 wepKeys[4][0x20];
} WifiSetupClientResultData;

typedef struct WifiSetupSound {
    u8 arc[0x94];
    u8 handle[4]; // 0x94, NNSSndHandle
    void *data;   // 0x98, sound archive file
    void *task;   // 0x9C
} WifiSetupSound;

extern WifiSetupConnections *gWifiSetupConnections;
extern WifiSetupSound *gWifiSetupSound;

int WifiSetupApScan_Start(void);
int WifiSetupApScan_Continue(void);
int WifiSetupApScan_End(void);
int WifiSetupApScan_GetEntries(WifiSetupScanEntry **out);
void WifiSetupApScan_Callback(void *arg);
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
void WifiSetupConnections_SetFromClientResult(WifiSetupClientResultData *result);
void WifiSetupConnections_SetFromInfo(u8 *info);
WifiSetupConnections *WifiSetupConnections_Get(void);
void WifiSetupConnections_Erase(int index);
void WifiSetupConnections_Reset(void);
void WifiSetupConnections_WritePage(int index);
void WifiSetup_ParseAddress(const char *text, u8 *address);
int WifiSetup_HexDigitValue(u8 c);
void WifiSetupSound_Init(void);
void WifiSetupSound_Free(void);

#endif // POKEHEARTGOLD_WIFI_SETUP_ACCESS_POINTS_H
