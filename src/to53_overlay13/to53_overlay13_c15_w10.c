#include "private_w10.h"

void ov13_022230F8(s32 ticks) {
    OS_Sleep(ticks);
}
s32 ov13_02223100(s32 arg) {
    s32 (*callback)(s32) = (s32 (*)(s32))ov13_0224DD80[1];
    if (callback != NULL) {
        callback(arg);
    }
    return 0;
}
