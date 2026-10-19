#ifndef POKEHEARTGOLD_WIFI_SETUP_CONNECTION_H
#define POKEHEARTGOLD_WIFI_SETUP_CONNECTION_H

#include "global.h"

// Connection settings of the access-point setup: one saved backup page and the edit copy of one page.

// how the connection was set up (WifiSetupConnection.setupMethod)
#define WIFI_SETUP_METHOD_MANUAL 0
#define WIFI_SETUP_METHOD_COPIED 1 // from WifiSetupConnections_SetFromInfo
#define WIFI_SETUP_METHOD_AUTO   2 // from the setup client's result
#define WIFI_SETUP_METHOD_ERASED 0xFF

// Connection settings as stored in a backup page, followed by the page's tail.
typedef struct WifiSetupConnectionPage {
    u8 unk_00[0x40];
    u8 ssid[0x20];
    u8 unk_60[0x20];
    u8 wepKeys[4][0x10];
    u8 ip[4];
    u8 gateway[4];
    u8 dns1[4];
    u8 dns2[4];
    u8 cidr; // subnet mask as a prefix length
    u8 wep40Keys[0x15];
    u8 keyType : 2;   // WM_WEPMODE_*
    u8 keyFormat : 6; // 0 hex digits, 1 text
    u8 setupMethod;   // WIFI_SETUP_METHOD_*
    u8 unk_E8[7];
    u8 usedMask; // pages 0 and 1: bit per page in use

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
    } tail;
} WifiSetupConnectionPage;

// Edit copy of one page: the same layout, then the editing state.
typedef struct WifiSetupConnection {
    u8 unk_00[0x40];
    u8 ssid[0x20];
    u8 unk_60[0x20];
    u8 wepKeys[4][0x10];

    union {
        struct {
            u8 ip[4];
            u8 gateway[4];
            u8 dns1[4];
            u8 dns2[4];
        };

        u8 addresses[4][4];
    };

    u8 cidr;
    u8 wep40Keys[0x15];
    u8 keyType : 2;
    u8 keyFormat : 6;
    u8 setupMethod;
    u8 unk_E8[7];
    u8 usedMask;
    u8 mask[4]; // subnet mask
    u8 index;   // page being edited
    u8 autoAddress;
    u8 autoDns;
    u8 unk_F7;
} WifiSetupConnection;

#endif // POKEHEARTGOLD_WIFI_SETUP_CONNECTION_H
