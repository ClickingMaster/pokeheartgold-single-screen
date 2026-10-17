#include "global.h"

#include "wifi_setup_scroll_bar.h"
#include "wifi_setup_ui.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

WifiSetupBgScreenState gWifiSetupBgScreen;

void WifiSetupBgScreen_Destroy(void) {
    WifiSetupTask_Destroy(1, gWifiSetupBgScreen.screen->task);
    WifiSetupHeap_FreeAndClear(&gWifiSetupBgScreen);
}
