#pragma force_active on
#include "global.h"

#include "wifi_setup_scroll_bar.h"
#include "wifi_setup_ui.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// WifiSetupScrollBar_Destroy + the definition of gWifiSetupScrollBar (kept in its own translation unit:
// the code of a function changes when the global it uses is defined in the same file).

WifiSetupScrollBar *gWifiSetupScrollBar;

void WifiSetupScrollBar_Destroy(void) {
    WifiSetupTask_Destroy(0, gWifiSetupScrollBar->task);
    WifiSetupOamBlock_Free(gWifiSetupScrollBar->thumb);
    WifiSetupHeap_FreeAndClear(&gWifiSetupScrollBar);
}
