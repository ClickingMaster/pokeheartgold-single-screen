#pragma force_active on
#include "global.h"

#include "wifi_setup_access_points.h"
#include "wifi_setup_scroll_bar.h"
#include "wifi_setup_ui.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// WifiSetupApScan_FreeWork + the definition of gWifiSetupApScan (kept in its own translation unit:
// the code of a function changes when the global it uses is defined in the same file).

typedef struct WifiSetupApScan WifiSetupApScan; // wifi_setup_access_points.c
u16 WM_GetAllowedChannel(void);

WifiSetupApScan *gWifiSetupApScan;

void WifiSetupApScan_FreeWork(void) {
    if (gWifiSetupApScan == NULL) {
        return;
    }
    while (WM_GetAllowedChannel() != 0x8000) {
    }
    WifiSetupHeap_FreeAndClear(&gWifiSetupApScan);
}
