#pragma force_active on
#include "global.h"

#include "wifi_setup_access_points.h"
#include "wifi_setup_address_entry.h"
#include "wifi_setup_app.h"
#include "wifi_setup_ui.h"
#include "wifi_setup_ui_parts.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Start of the address entry screen (compiled without -ipa file, so it stays its own TU): allocates
// gWifiSetupAddressEntry, fills its digits with the stored address of the kind given by the screen
// arguments (the cursor then starts on the last field; an unset address is cleared and the cursor
// starts on the first one) and runs WifiSetupAddressEntry_FadeIn next.

typedef void (*WifiSetupAddressLoadFunc)(char *buf);

// stored-address formatters per address kind (IP address, subnet mask, gateway, primary/secondary DNS)
typedef struct WifiSetupAddressLoadFuncs {
    WifiSetupAddressLoadFunc funcs[5];
} WifiSetupAddressLoadFuncs;

typedef struct WifiSetupAddressMessages {
    u8 messages[5];
} WifiSetupAddressMessages;

// screen layout read by src/wifi_setup_address_entry.c as well: field limit "255", digit positions,
// and the message shown per address kind at 0x08
typedef struct WifiSetupAddressLayout {
    u8 unk_00[8];
    WifiSetupAddressMessages messages; // 0x08
    u8 unk_0D[11];
} WifiSetupAddressLayout;

extern const u8 gWifiSetupAddressEntryUnsetAddress[]; // stored address of an unset entry
extern const WifiSetupAddressLayout ov13_02245480;
extern const WifiSetupAddressLoadFuncs ov13_02245498;

void WifiSetupAddressEntry_Init(void);

void WifiSetupAddressEntry_Init(void) {
    WifiSetupAddressLoadFuncs funcs = ov13_02245498;
    WifiSetupAddressMessages messages = ov13_02245480.messages;
    int kind;
    WifiSetupAddressEntry *work;
    gWifiSetupAddressEntry = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupAddressEntry), 4);
    WifiSetupApp_GetScreenArgs(&kind, 0);
    funcs.funcs[kind]((char *)gWifiSetupAddressEntry->digits);
    work = gWifiSetupAddressEntry;
    if (memcmp(work->digits, gWifiSetupAddressEntryUnsetAddress, 3) != 0) {
        work->field = 3;
    } else {
        MI_CpuFill8(work->digits, 0, 12);
        gWifiSetupAddressEntry->field = 0;
    }
    WifiSetupAddressEntry_LoadGraphics();
    WifiSetupBackground_LoadHeader(kind + 11);
    WifiSetupMessage_Show(messages.messages[kind], -1, 0);
    WifiSetupBanner_Show(2);
    gWifiSetupAddressEntry->canvas = WifiSetupText_InitBgCanvas(0, 0);
    gWifiSetupAddressEntry->cursor = WifiSetupOamTemplate_AllocOam(0, 0x3f);
    gWifiSetupAddressEntry->cursor->attr2 = (gWifiSetupAddressEntry->cursor->attr2 & ~0xc00) | 0xc00;
    WifiSetupAddressEntry_UpdateCursor();
    WifiSetupAddressEntry_DrawDigits();
    WifiSetupApp_SetStateFunc(WifiSetupAddressEntry_FadeIn);
}

const WifiSetupAddressLayout ov13_02245480 = {
    { 0x32, 0x35, 0x35, 0, 0xb, 0, 0x10, 0 },
    { { 0x3F, 0x40, 0x41, 0x42, 0x43 } },
    { 0, 0, 0, 0x29 }
};
const WifiSetupAddressLoadFuncs ov13_02245498 = {
    { WifiSetupConnections_FormatIpAddress, WifiSetupConnections_FormatSubnetMask, WifiSetupConnections_FormatGateway, WifiSetupConnections_FormatPrimaryDns, WifiSetupConnections_FormatSecondaryDns }
};
