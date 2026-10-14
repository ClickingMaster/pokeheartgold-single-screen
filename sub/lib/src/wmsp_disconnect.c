#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// clang-format off
asm s32 __VENEER__s32_div_f(s32 dividend, s32 divisor) {
    ldr pc, =_s32_div_f
}
// clang-format on

// Reports the failed WL command `command` (driver result `result`) of WMSP_StartConnectEx.
void WmspError_060045D0(u16 command, u16 result, u16 detail) {
    u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 12;
    cb[1] = WM_ERRCODE_FAILED;
    cb[2] = command;
    cb[3] = result;
    cb[7] = detail;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

// request[1]: aid bitmap of the stations to disconnect.
void WMSP_Disconnect(u32 *request) {
    u16 result;
    u16 aidBitmap = request[1];
    if (WMSP_DisconnectCore(request, FALSE, &result) == TRUE) {
        u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 13;
        cb[1] = WM_ERRCODE_SUCCESS;
        cb[4] = aidBitmap;
        cb[5] = result;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
    }
}

#pragma dont_inline reset
