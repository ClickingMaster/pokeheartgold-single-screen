#ifndef POKEHEARTGOLD_WIFI_SETUP_ACCESS_POINTS_H
#define POKEHEARTGOLD_WIFI_SETUP_ACCESS_POINTS_H

#include "global.h"

// Access-point data of the access-point setup (ARM code in overlay 13): the scan for nearby access
// points, the saved connection settings (four 0x100-byte backup pages, the first three editable)
// with an edit copy of one of them, and the sound archive.

#define WIFI_SETUP_NUM_SCAN_ENTRIES 20
#define WIFI_SETUP_NUM_CONNECTIONS  4

// how the connection was set up (WifiSetupConnection.setupMethod)
#define WIFI_SETUP_METHOD_MANUAL 0
#define WIFI_SETUP_METHOD_COPIED 1 // from WifiSetupConnections_SetFromInfo
#define WIFI_SETUP_METHOD_AUTO   2 // from the setup client's result
#define WIFI_SETUP_METHOD_ERASED 0xFF

// One access point found by the scan.
typedef struct WifiSetupScanEntry {
    u8 ssid[0x20];
    u8 bssid[6];
    u16 linkLevel;
    u8 security; // 0 open, 1 WEP, 2 WPA
    u8 pad29;
} WifiSetupScanEntry;

// Connection settings as stored in a backup page (0xF0 bytes, followed by the page's tail).
typedef struct WifiSetupConnectionPage {
    u8 unk_00[0x40];
    u8 ssid[0x20]; // 0x40
    u8 unk_60[0x20];
    u8 wepKeys[4][0x10]; // 0x80
    u8 ip[4];            // 0xC0
    u8 gateway[4];       // 0xC4
    u8 dns1[4];          // 0xC8
    u8 dns2[4];          // 0xCC
    u8 cidr;             // 0xD0, subnet mask as a prefix length
    u8 unk_D1[0x15];
    u8 keyType : 2;   // 0xE6, 0 none, 1 WEP 40, 2 WEP 104, 3 WEP 128
    u8 keyFormat : 6; // 0 hex digits, 1 text
    u8 setupMethod;   // 0xE7, WIFI_SETUP_METHOD_*
    u8 unk_E8[7];
    u8 usedMask; // 0xEF, pages 0 and 1: bit per page in use

    union {
        struct {
            u8 mask[4];
            u8 index;
            u8 autoAddress;
            u8 autoDns;
            u8 unk_F7;
        } work;

        struct {
            u8 info[0xE];
            u16 crc;
        } page;
    } tail; // 0xF0
} WifiSetupConnectionPage;

// Edit copy of one page: the same layout up to 0xF0, then the editing state.
typedef struct WifiSetupConnection {
    u8 unk_00[0x40];
    u8 ssid[0x20]; // 0x40
    u8 unk_60[0x20];
    u8 wepKeys[4][0x10]; // 0x80

    union {
        struct {
            u8 ip[4];      // 0xC0
            u8 gateway[4]; // 0xC4
            u8 dns1[4];    // 0xC8
            u8 dns2[4];    // 0xCC
        };

        u8 addresses[4][4];
    };

    u8 cidr; // 0xD0
    u8 unk_D1[0x15];
    u8 keyType : 2; // 0xE6
    u8 keyFormat : 6;
    u8 setupMethod; // 0xE7
    u8 unk_E8[7];
    u8 usedMask;    // 0xEF
    u8 mask[4];     // 0xF0, subnet mask
    u8 index;       // 0xF4, page being edited
    u8 autoAddress; // 0xF5
    u8 autoDns;     // 0xF6
    u8 unk_F7;
} WifiSetupConnection;

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
