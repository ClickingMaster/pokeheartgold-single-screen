#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
void *ov13_0224F5A0;
u16 WM_GetAllowedChannel(void);
void WifiSetupHeap_FreeAndClear(void *p);
void ov13_02239B34(void);
// @fn ov13_02239B34
void ov13_02239B34(void) {
    if (ov13_0224F5A0 == NULL) {
        return;
    }
    while (WM_GetAllowedChannel() != 0x8000) {
    }
    WifiSetupHeap_FreeAndClear(&ov13_0224F5A0);
}
