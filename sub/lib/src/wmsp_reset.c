#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Ends MP, reports every connection as disconnected, then brings the WL driver back to idle
// (deauthenticate and reset as its station state requires).
void WMSP_Reset(void) {
    struct {
        u8 broadcast[6], mac[6], buffer[512];
    } local;

    int index;
    u8 *buffer = local.buffer;
    WmspStatus *status = wmspW.status;
    u32 cancel = 0, irq, connections;
    int parent;
    int stationState, retry;
    u32 mode;
    u16 *result, *cb;
    irq = __VENEER_OS_DisableInterrupts();
    if (status->mpActive == 1) {
        status->mpActive = 0;
        cancel = 1;
        WMSP_CancelVAlarm();
        WMSP_SetThreadPriorityLow();
        {
            u16 state = status->state;
            if (state == 10) {
                status->state = 8;
            } else if (state == 9) {
                status->state = 7;
            }
        }
    }
    {
        u16 state = status->state;
        // 7: parent, 8: child
        if ((u16)(state + 0xfff9) <= 1) {
            connections = status->childBitmap;
            parent = (state == 7);
        } else {
            connections = 0;
        }
    }
    status->childBitmap = 0;
    status->readyBitmap = 0;
    status->unk_14 = 0;
    status->dcfActive = 0;
    status->vsyncFlag = 0;
    status->beaconIndicateFlag = 0;
    __VENEER_OS_RestoreInterrupts((OSIntrMode)irq);
    if (cancel) {
        WMSP_CleanSendQueue(0xffff);
    }
    if (parent) {
        status->parentParam.entryFlag = 0;
    }
    if (connections) {
        for (index = 0; index < 16; index++) {
            if (connections & (1U << index)) {
                WMSP_IndicateDisconnectionFromMyself(parent, (u16)index, index == 0 ? status->bssid : status->childMacAddress[index - 1]);
            }
        }
    }
    __VENEER_MI_CpuFill8(status->childMacAddress, 0, sizeof(status->childMacAddress));
    result = WMSP_WL_DevGetStationState(buffer);
    if (result[2]) {
        WmspError_06002FC0(0x308, result[2]);
        return;
    }
    stationState = result[3];
    result = WMSP_WL_ParamGetMode(buffer);
    if (result[2]) {
        WmspError_06002FC0(0x284, result[2]);
        return;
    }
    mode = result[3];
    switch (stationState) {
    case 48:
    case 64:
        if ((u16)(mode + 0xfffe) <= 1) {
            __VENEER_MI_CpuCopy8(status->bssid, local.mac, 6);
            for (retry = 0; retry < 2; retry++) {
                result = WMSP_WL_MlmeDeAuthenticate((u16 *)buffer, local.mac, 3);
                switch (result[2]) {
                case 0:
                    status->state = 3;
                    goto reset;
                case 7:
                case 12:
                    break;
                default:
                    goto reset;
                }
            }
        } else if (mode == 1) {
            __VENEER_MI_CpuFill8(local.broadcast, 0xff, 6);
            result = WMSP_WL_MlmeDeAuthenticate((u16 *)buffer, local.broadcast, 3);
            if (result[2] == 0) {
                status->state = 3;
            }
        }
    case 32:
reset:
        result = WMSP_WL_MlmeReset((u16 *)buffer, 1);
        if (result[2]) {
            WmspError_06002FC0(0, result[2]);
            return;
        }
    case 0:
        result = WMSP_WL_DevIdle(buffer);
        if (result[2]) {
            WmspError_06002FC0(0x302, result[2]);
            return;
        }
    case 16:
        if (status->preamble == 0) {
            result = WMSP_WL_ParamSetPreambleType(buffer, 1);
            if (result[2]) {
                WmspError_06002FC0(0x20e, result[2]);
                return;
            }
            status->preamble = 1;
        }
        status->state = 2;
        status->wepEnabled = 0;
        WMSP_ResetSizeVars();
        break;
    case 17:
    case 18:
        if (mode == 0) {
            result = WMSP_WL_DevTestSignal(buffer, 0, 0, 20, 1);
            if (result[2]) {
                WmspError_06002FC0(0x309, result[2]);
                return;
            }
        }
        result = WMSP_WL_DevIdle(buffer);
        if (result[2]) {
            WmspError_06002FC0(0x302, result[2]);
            return;
        }
        status->state = 2;
        break;
    default:
        WmspError_06002FC0(0x308, 0);
        return;
    }
    cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 1;
    cb[1] = 0;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}
