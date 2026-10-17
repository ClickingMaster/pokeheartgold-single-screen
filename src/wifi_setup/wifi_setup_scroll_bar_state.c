#include "global.h"

#include "wifi_setup/wifi_setup_scroll_bar.h"
#include "wifi_setup/wifi_setup_ui.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

WifiSetupScrollBar *gWifiSetupScrollBar;

void WifiSetupScrollBar_Destroy(void) {
    WifiSetupTask_Destroy(0, gWifiSetupScrollBar->task);
    WifiSetupOamBlock_Free(gWifiSetupScrollBar->thumb);
    WifiSetupHeap_FreeAndClear(&gWifiSetupScrollBar);
}
