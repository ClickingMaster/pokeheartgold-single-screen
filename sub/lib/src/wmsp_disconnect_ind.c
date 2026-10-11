#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Tells the ARM9 that this station ended the connection to `aid` (MAC `mac`).
void WMSP_IndicateDisconnectionFromMyself(BOOL parent, u16 aid, void *mac) {
    u16 *cb;
    WmspStatus *status = wmspW.status;
    cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[1] = 0;
    if (parent) {
        cb[0] = 8;
        cb[4] = 26;
        cb[9] = 0xf001;
        cb[8] = aid;
        __VENEER_MI_CpuCopy8(mac, cb + 5, 6);
        cb[22] = status->parentSize;
        cb[23] = status->childSize;
    } else {
        cb[0] = 12;
        cb[4] = 26;
        cb[6] = 0xf001;
        cb[5] = status->aid;
        __VENEER_MI_CpuCopy8(mac, cb + 8, 6);
        cb[11] = status->parentSize;
        cb[12] = status->childSize;
    }
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

// Reports the failed WL command `command` (driver result `result`) of WMSP_DisconnectCore.
void WmspError_06004CB4(u16 command, u16 result, u16 aidBitmap, u16 disconnected) {
    u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 13;
    cb[1] = 1;
    cb[2] = command;
    cb[3] = result;
    cb[4] = aidBitmap;
    cb[5] = disconnected;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

// Reports a WL driver error indication.
void WmspIndError(u16 a, u16 b, u16 c, u16 d) {
    u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 37;
    cb[1] = 1;
    cb[2] = a;
    cb[3] = b;
    cb[4] = c;
    cb[5] = d;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

#pragma dont_inline reset
