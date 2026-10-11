#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// clang-format off
asm u32 __VENEER__u32_div_f(u32 dividend, u32 divisor) {
    ldr pc, =_u32_div_f
}
// clang-format on

// Reports the failed WL command `command` (driver result `result`) of WMSP_StartScan(Ex).
void WmspError_06003ED4(u16 command, u16 result, BOOL scanEx) {
    u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = scanEx ? 38 : 10;
    cb[1] = 1;
    cb[4] = 4;
    cb[2] = command;
    cb[3] = result;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

void WMSP_EndScan(void *request) {
    u8 buf[512];
    void *b;
    WmspStatus *status;
    u16 *cb, error;
    status = wmspW.status;
    b = buf;
    if (status->state != 5) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 11;
        cb[1] = 3;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
    } else {
        error = WMSP_WL_DevIdle(b)[2];
        if (error) {
            WmspError_06003FF0(0x302, error);
            return;
        }
        status->state = 2;
        if (!status->unk_1ee) {
            error = WMSP_WL_ParamSetPreambleType(b, 1)[2];
            if (error) {
                WmspError_06003FF0(0x20e, error);
                return;
            }
            status->unk_1ee = 1;
        }
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 11;
        cb[1] = 0;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
    }
}

// Reports the failed WL command `command` (driver result `result`) of WMSP_EndScan.
void WmspError_06003FF0(u16 command, u16 result) {
    u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 11;
    cb[1] = 1;
    cb[2] = command;
    cb[3] = result;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

#pragma dont_inline reset
