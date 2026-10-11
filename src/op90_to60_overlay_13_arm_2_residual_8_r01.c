#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
void *gWifiSetupApScan;
u16 WM_GetAllowedChannel(void);
void WifiSetupHeap_FreeAndClear(void *p);
void ov13_02239B34(void);
// @fn ov13_02239B34
void ov13_02239B34(void) {
    if (gWifiSetupApScan == NULL) {
        return;
    }
    while (WM_GetAllowedChannel() != 0x8000) {
    }
    WifiSetupHeap_FreeAndClear(&gWifiSetupApScan);
}
