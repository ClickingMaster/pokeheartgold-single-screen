#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Queues the ARM9's data for the next MP sequences; reports the failure right away (the success
// callback comes with the send).
void WMSP_SetMPData(WmspSetMPDataRequest *request) {
    u16 *data;
    u16 size;
    u32 destBitmap;
    void *callback;
    void *arg;
    u32 result;
    u16 connected;
    u16 port;
    u16 priority;
    WmspStatus *status;
    WmspSetMPDataRequest *r = request;
    status = wmspW.status;
    result = status->aid;
    connected = status->childBitmap;
    size = r->size;
    port = r->port;
    priority = r->priority;
    data = r->data;
    destBitmap = r->destBitmap;
    callback = r->callback;
    arg = r->arg;
    if (result) {
        // a child sends to the parent only
        destBitmap = 1;
    }
    if (!status->mpActive) {
        result = 3;
    } else if (!(destBitmap & connected)) {
        result = 0;
    } else {
        result = WMSP_PutSendQueue(connected, priority, port, destBitmap, data, size, callback, arg);
    }
    if (result != 2) {
        WmspPortSendCallback *cb = (WmspPortSendCallback *)__VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb->command = 129;
        cb->result = result;
        cb->api = 20;
        cb->port = port;
        cb->destBitmap = destBitmap;
        cb->failedBitmap = result == 10 ? (destBitmap & connected) : 0;
        cb->sentBitmap = 0;
        cb->size = size;
        cb->data = data;
        cb->callback = callback;
        cb->arg = arg;
        cb->seqNo = 0xffff;
        {
            u16 parentSize = status->parentSize, childSize = status->childSize;
            cb->sendSize = status->aid == 0 ? parentSize : childSize;
            if (status->aid != 0) {
                childSize = parentSize;
            }
            cb->recvSize = childSize;
        }
        __VENEER_WMSP_ReturnResult2Wm9(cb);
    }
}
