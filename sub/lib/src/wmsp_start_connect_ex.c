#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// BSS description given to WMSP_WL_MlmeJoin (the parent's, with the SSID set from the game info).
typedef struct JoinBssDesc {
    u16 data[32];
    u32 unk_40;
} JoinBssDesc;

// Joins, authenticates with and associates to the parent in the request; on success this station
// is a child (state 8) with the aid the parent gave.
void WMSP_StartConnectEx(WmspStartConnectExRequest *request) {
    u16 command[256];
    u16 *output = command;
    JoinBssDesc descriptor;
    u8 authMac[6], associateMac[6];
    WmspWork *work = &wmspW;
    WmspArm9Buf *parent = work->arm9Buf;
    u32 irq;
    WmspStatus *status;
    WmspStartConnectExRequest *req = request;
    u16 *result;
    u16 *cb;
    u16 power;
    u32 inner;
    u16 lost;
    u8 rssi;
    status = work->status;
    if (status->state != 2 || (status->unk_c8 & 1)) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 12;
        cb[1] = 3;
        cb[4] = 6;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    __VENEER_MI_CpuCopy8(req->parent, &parent->bssDesc, 192);
    if (parent->bssDesc.gameInfoLength >= 16 && !(parent->attribute & 1)) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 12;
        cb[1] = 11;
        cb[4] = 6;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    {
        int bit = 1 << parent->bssDesc.channel;
        if (!(bit & status->enableChannel) || !(0x1fff & (bit >> 1))) {
            cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            cb[0] = 12;
            cb[1] = 6;
            cb[4] = 6;
            __VENEER_WMSP_ReturnResult2Wm9(cb);
            return;
        }
    }
    cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 12;
    cb[1] = 0;
    cb[4] = 6;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
    if (status->unk_1ec == 1) {
        if (parent->bssDesc.rateSet.basic & 1) {
            status->unk_1ec = 1;
        } else {
            status->unk_1ec = 2;
        }
    } else {
        if (parent->bssDesc.rateSet.basic & 2) {
            status->unk_1ec = 2;
        } else {
            status->unk_1ec = 1;
        }
    }
    if (parent->bssDesc.capaInfo & 32) {
        status->unk_1ee = 1;
    } else {
        status->unk_1ee = 0;
    }
    if (parent->bssDesc.gameInfoLength == 0) {
        status->unk_e6 = 3;
    } else {
        status->unk_e6 = 2;
    }
    if (!WMSP_SetAllParams(12, (u8 *)output)) {
        return;
    }
    result = WMSP_WL_ParamSetNullKeyResponseMode(output, 0);
    if (result[2]) {
        WmspError_060045D0(0x216, result[2], 0);
        return;
    }
    if (parent->bssDesc.gameInfoLength < 16) {
        lost = (parent->bssDesc.beaconPeriod == 0) ? 1 : __VENEER__s32_div_f(10000, parent->bssDesc.beaconPeriod) + 1;
        if (lost > 255) {
            lost = 255;
        }
        result = WMSP_WL_ParamSetBeaconLostThreshold(output, lost);
        if (result[2]) {
            WmspError_060045D0(0x20b, result[2], 0);
            return;
        }
    }
    result = WMSP_WL_DevClass1(output);
    if (result[2]) {
        WmspError_060045D0(0x303, result[2], 0);
        return;
    }
    status->state = 3;
    power = req->powerSave ? 1 : 0;
    result = WMSP_WL_MlmePowerManagement(output, power, 0, 1);
    if (result[2]) {
        WmspError_060045D0(1, result[2], 0);
        return;
    }
    status->pwrMgtMode = power;
    __VENEER_MI_CpuCopy8(&parent->bssDesc, &descriptor, 64);
    if (status->unk_e6 == 2) {
        // SSID: ggid, tgid, 0, then the request's 24 bytes
        descriptor.data[5] = 32;
        descriptor.data[6] = parent->ggid;
        descriptor.data[7] = parent->ggid >> 16;
        descriptor.data[8] = parent->tgid;
        descriptor.data[9] = 0;
        __VENEER_MI_CpuCopy8(req->ssid, descriptor.data + 10, 24);
    }
    result = WMSP_WL_MlmeJoin(output, 2000, &descriptor);
    if (result[2] || result[3]) {
        WmspError_060045D0(3, result[2], result[3]);
        return;
    }
    __VENEER_MI_CpuCopy8(result + 4, status->bssid, 6);
    __VENEER_MI_CpuCopy8(status->bssid, authMac, 6);
    result = WMSP_WL_MlmeAuthenticate(output, authMac, req->authMode, 2000);
    if (result[2] == 12 && result[3] == 19) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 12;
        cb[1] = 12;
        cb[4] = 6;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    if (result[2] || result[3]) {
        WmspError_060045D0(4, result[2], result[3]);
        return;
    }
    __VENEER_MI_CpuCopy8(status->bssid, associateMac, 6);
    result = WMSP_WL_MlmeAssociate(output, associateMac, 1, 2000);
    irq = __VENEER_OS_DisableInterrupts();
    if (result[2] == 12 && result[3] == 19) {
        __VENEER_OS_RestoreInterrupts((OSIntrMode)irq);
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 12;
        cb[1] = 12;
        cb[4] = 6;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    if (result[2] || result[3]) {
        __VENEER_OS_RestoreInterrupts((OSIntrMode)irq);
        WmspError_060045D0(6, result[2], result[3]);
        return;
    }
    status->aid = result[4];
    status->unk_ba = parent->tgid;
    __VENEER_MIi_CpuClear16(1, status->portSeqNo, 16);
    rssi = parent->bssDesc.rssi & 255;
    if (!(rssi & 2)) {
        rssi = (rssi >> 2) + 25;
    } else {
        rssi = rssi >> 2;
    }
    status->linkLevel = WMSP_GetLinkLevel(rssi);
    WMSP_FillRssiIntoList(rssi);
    inner = __VENEER_OS_DisableInterrupts();
    status->childBitmap = 1;
    status->unk_86 = 1;
    if (status->recvTimeout != 0) {
        OSTick tick = __VENEER_OS_GetTick() | 1;
        ((u32 *)&status->lastRecvTick[0])[0] = (u32)tick;
        ((u32 *)&status->lastRecvTick[0])[1] = (u32)(tick >> 32);
    }
    status->state = 8;
    WMSP_SetParentMaxSize(parent->parentMaxSize + ((parent->attribute & 4) ? 42 : 0));
    WMSP_SetChildMaxSize(parent->childMaxSize + ((parent->attribute & 4) ? 6 : 0));
    __VENEER_OS_RestoreInterrupts((OSIntrMode)inner);
    status->unk_c2 = 1;
    cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 12;
    cb[1] = 0;
    cb[4] = 7;
    cb[5] = status->aid;
    __VENEER_MI_CpuCopy8(status->bssid, cb + 8, 6);
    cb[11] = status->parentSize;
    cb[12] = status->childSize;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
    __VENEER_OS_RestoreInterrupts((OSIntrMode)irq);
}
