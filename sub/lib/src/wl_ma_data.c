#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Queues a test data frame (command 0xFFFF: no confirm) for station 0.
int MA_TestDataReqCmd(WlMaDataReq *req, WlCmdCfm *cfm) {
    WlWork *w;
    cfm->length = 1;
    req->header.command = 0xFFFF;
    req->frame.station = 0;
    req->frame.frame.length = req->frame.bodyLength;
    CAM_IncFrameCount(&req->frame);
    w = wlWork;
    MoveHeapBuf(&w->heap.bufMan[10], &w->heap.bufMan[1], (WlHeapBuf *)req);
    TxqPri(0);
    return 0;
}

// param: bit 0: drop the key data frames, bit 1: the MP frame, bit 2: the data frames.
int MA_ClrDataReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    cfm->length = 1;
    if (req->param[0] & 1) {
        ClearTxKeyData();
    }
    if (req->param[0] & 2) {
        ClearTxMp();
    }
    if (req->param[0] & 4) {
        ClearTxData();
    }
    return 0;
}

// Sends the confirm of a data request (the frame's TX status) back to the WMSP.
void IssueMaDataConfirm(WlHeapBufMan *man, WlMaDataReq *req) {
    WlMaDataConfirm *confirm = (WlMaDataConfirm *)((u16 *)req + req->header.length);
    req->header.command = confirm->cfm.command;
    confirm->cfm.length = 2;
    confirm->cfm.result = 0;
    confirm->status = req->frame.frame.status;
    SendMessageToWmDirect(man, req);
}

#pragma dont_inline reset
