#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// request: command, ARM9 buffer, status block, (ARM9 buffer word 2), argument of WMSPi_CommonInit
void WMSP_Initialize(u32 *request) {
    u32 *arm9Buf = (u32 *)request[1];
    u16 result, wlResult;
    u32 status;
    wmspW.arm9Buf = (WmspArm9Buf *)arm9Buf;
    status = request[2];
    wmspW.status = (WmspStatus *)status;
    arm9Buf[0] = status;
    arm9Buf[2] = request[3];
    WMSPi_CommonInit(request[4]);
    if (!WMSPi_CommonWlIdle(&result, &wlResult)) {
        u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 0;
        cb[1] = WM_ERRCODE_FAILED;
        cb[2] = result;
        cb[3] = wlResult;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
    } else {
        ((WmspStatus *)arm9Buf[0])->state = 2;
        {
            u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            cb[0] = 0;
            cb[1] = WM_ERRCODE_SUCCESS;
            __VENEER_WMSP_ReturnResult2Wm9(cb);
        }
    }
}
