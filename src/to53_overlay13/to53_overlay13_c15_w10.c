#include "private_w10.h"

void WifiSetup_Sleep(s32 ticks) {
    OS_Sleep(ticks);
}
s32 ov13_02223100(s32 arg) {
    s32 (*callback)(s32) = (s32 (*)(s32))gWifiSetupRuntime[1];
    if (callback != NULL) {
        callback(arg);
    }
    return 0;
}
