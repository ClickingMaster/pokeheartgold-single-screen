#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Reports the failed WL command `command` (driver result `result`) of WMSP_Reset / WMSP_End.
void WmspError_06002FC0(u16 command, u16 result) {
    u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 1;
    cb[1] = WM_ERRCODE_FAILED;
    cb[2] = command;
    cb[3] = result;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

void WMSP_End(void *request) {
    u8 buf[512];
    WmspStatus *status = wmspW.status;
    u16 *cb, error;
    if (status->state != 2) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 2;
        cb[1] = WM_ERRCODE_ILLEGAL_STATE;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
    } else {
        error = WMSP_WL_DevShutdown(buf)[2];
        if (error) {
            cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            cb[0] = 2;
            cb[1] = WM_ERRCODE_FAILED;
            cb[2] = 0x301;
            cb[3] = error;
            __VENEER_WMSP_ReturnResult2Wm9(cb);
        } else {
            status->state = 1;
            __VENEER_PM_SetLEDPattern(1);
            status->state = 0;
            cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            cb[0] = 2;
            cb[1] = WM_ERRCODE_SUCCESS;
            __VENEER_WMSP_ReturnResult2Wm9(cb);
        }
    }
}

// clang-format off
asm void __VENEER_PM_SetLEDPattern(int pattern) {
    ldr pc, =PM_SetLEDPattern
}
// clang-format on

void WMSP_SetParentParam(u32 *request) {
    u16 buf[256];
    WmspStatus *status = wmspW.status;
    u16 *cb, error;
    __VENEER_MI_CpuCopy8((void *)request[1], &status->parentParam, sizeof(WmspParentParam));
    u16 channel = status->parentParam.channel;
    int mask = 1 << channel;
    if (!(mask & status->enableChannel)) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 7;
        cb[1] = WM_ERRCODE_INVALID_PARAM;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
    } else {
        error = WMSP_WL_ParamSetMaxConnectableChild(buf, status->parentParam.maxEntry)[2];
        if (error) {
            cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            cb[0] = 7;
            cb[1] = WM_ERRCODE_FAILED;
            cb[2] = 0x212;
            cb[3] = error;
            __VENEER_WMSP_ReturnResult2Wm9(cb);
        } else {
            cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            cb[0] = 7;
            cb[1] = WM_ERRCODE_SUCCESS;
            __VENEER_WMSP_ReturnResult2Wm9(cb);
        }
    }
}

#pragma dont_inline reset
