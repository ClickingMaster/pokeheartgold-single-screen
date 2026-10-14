#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// An OSTick cleared as two words.
typedef struct TickWords {
    u32 low, high;
} TickWords;

// Disconnect callbacks to the ARM9 (indication 12 on a child, 8 on the parent).
typedef struct ParentLostCallback {
    u16 command, result, unk_04[2], state, aid, reason;
    u8 unk_0e[2], macAddress[6];
    u16 parentSize, childSize;
} ParentLostCallback;

typedef struct ChildLostCallback {
    u16 command, result, unk_04[2], state;
    u8 macAddress[6];
    u16 aid, reason;
    u8 unk_14[24];
    u16 parentSize, childSize;
} ChildLostCallback;

// request[1]: aid bitmap to disconnect (parent), request[2]: reason (indications only). A child
// disconnects from its parent. `indication`: called for a disconnect indication, not an ARM9 request.
// Returns FALSE when a WL command failed or there was nothing to disconnect.
BOOL WMSP_DisconnectCore(u32 *request, BOOL indication, u16 *disconnected) {
    struct {
        u8 childMac[6], parentMac[6];
        u16 output[256];
    } frame;

    int cleanup, child;
    u16 *output = frame.output;
    WmspStatus *status = wmspW.status;
    u16 mask = request[1], reason = indication ? request[2] : 0;
    int retries;
    u16 done = 0;
    cleanup = done;
    u16 *result, *callback;
    u32 irq;
    int bit;
    u16 aid;
    u8 *mac;
    int offset;
    if (status->state == 9 || status->state == 7) {
        if (status->mpActive == 1) {
            cleanup = 1;
        }
    } else if (status->state == 10 || status->state == 8) {
        irq = __VENEER_OS_DisableInterrupts();
        if (!status->childBitmap) {
            __VENEER_OS_RestoreInterrupts((OSIntrMode)irq);
            if (!indication) {
                callback = __VENEER_WMSP_GetBuffer4Callback2Wm9();
                callback[0] = 13;
                callback[1] = WM_ERRCODE_ILLEGAL_STATE;
                callback[2] = 0;
                callback[3] = 0;
                callback[4] = mask;
                callback[5] = 0;
                __VENEER_WMSP_ReturnResult2Wm9(callback);
            }
            return FALSE;
        }
        if (status->mpActive == 1) {
            status->mpActive = 0;
            cleanup = 1;
            WMSP_CancelVAlarm();
            WMSP_SetThreadPriorityLow();
            if (status->state == 10) {
                status->state = 8;
            }
        }
        status->childBitmap = 0;
        status->readyBitmap = 0;
        status->unk_14 = 0;
        status->dcfActive = 0;
        status->vsyncFlag = 0;
        __VENEER_OS_RestoreInterrupts((OSIntrMode)irq);
    } else {
        if (!indication) {
            callback = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            callback[0] = 13;
            callback[1] = WM_ERRCODE_ILLEGAL_STATE;
            callback[2] = 0;
            callback[3] = 0;
            callback[4] = mask;
            callback[5] = 0;
            __VENEER_WMSP_ReturnResult2Wm9(callback);
        }
        return FALSE;
    }
    if (status->state == 10 || status->state == 8) {

        __VENEER_MI_CpuCopy8(status->bssid, frame.parentMac, 6);
        for (bit = 0; bit < 2;) {
            result = WMSP_WL_MlmeDeAuthenticate(output, frame.parentMac, 3);
            switch (result[2]) {
            case 0:
            case 1:
                goto deauthenticated;
            case 7:
            case 12:
                bit++;
                break;
            default:
                if (indication) {
                    WmspIndError(5, result[2], mask, done);
                } else {
                    WmspError_06004CB4(5, result[2], mask, done);
                }
                if (cleanup) {
                    WMSP_CleanSendQueue(1);
                }
                return FALSE;
            }
        }
deauthenticated:
        status->beaconIndicateFlag = 0;
        status->state = 3;
        done = 1;
        result = WMSP_WL_MlmeReset(output, 1);
        if (result[2]) {
            if (indication) {
                WmspIndError(0, result[2], mask, 1);
            } else {
                WmspError_06004CB4(0, result[2], mask, 1);
            }
            if (cleanup) {
                WMSP_CleanSendQueue(1);
            }
            return FALSE;
        }
        result = WMSP_WL_DevIdle(output);
        if (result[2]) {
            if (indication) {
                WmspIndError(0x302, result[2], mask, 1);
            } else {
                WmspError_06004CB4(0x302, result[2], mask, 1);
            }
            if (cleanup) {
                WMSP_CleanSendQueue(1);
            }
            return FALSE;
        }
        status->state = 2;
        status->wepEnabled = 0;
        status->wepMode = 0;
        __VENEER_MI_CpuFill8(status->wepKey, 0, sizeof(status->wepKey));
        WMSP_ResetSizeVars();
        if (indication == 1) {
            ParentLostCallback *reply = (ParentLostCallback *)__VENEER_WMSP_GetBuffer4Callback2Wm9();
            reply->command = 12;
            reply->result = 0;
            reply->state = 9;
            reply->reason = reason;
            reply->aid = status->aid;
            __VENEER_MI_CpuCopy8(frame.parentMac, reply->macAddress, 6);
            reply->parentSize = status->parentSize;
            reply->childSize = status->childSize;
            __VENEER_WMSP_ReturnResult2Wm9(reply);
        } else {
            WMSP_IndicateDisconnectionFromMyself(0, 0, frame.parentMac);
        }
        if (cleanup) {
            WMSP_CleanSendQueue(1);
        }
    } else {
        for (child = 1; child < 16; child++) {
            bit = 1u << child;
            if (!(bit & (status->childBitmap & mask))) {
                continue;
            }
            offset = (child - 1) * 6;
            mac = &status->childMacAddress[0][0];
            aid = child;
            __VENEER_MI_CpuCopy8(mac + offset, frame.childMac, 6);
            for (retries = 0; retries < 2;) {
                result = WMSP_WL_MlmeDeAuthenticate(output, frame.childMac, 3);
                switch (result[2]) {
                case 0:
                    goto deauthed;
                case 7:
                case 12:
                    retries++;
                    break;
                default:
                    if (indication) {
                        WmspIndError(5, result[2], mask, done);
                    } else {
                        WmspError_06004CB4(5, result[2], mask, done);
                    }
                    if (cleanup) {
                        WMSP_CleanSendQueue(1);
                    }
                    return FALSE;
                }
            }
deauthed:
            irq = __VENEER_OS_DisableInterrupts();
            if (status->childBitmap & bit) {
                done |= 1u << aid;
                status->childBitmap &= ~bit;
                status->readyBitmap &= ~bit;
                ((TickWords *)status->lastRecvTick)[aid].low = 0;
                ((TickWords *)status->lastRecvTick)[aid].high = 0;
                __VENEER_MI_CpuFill8(mac + offset, 0, 6);
                __VENEER_OS_RestoreInterrupts((OSIntrMode)irq);
                if (indication == 1) {
                    ChildLostCallback *reply = (ChildLostCallback *)__VENEER_WMSP_GetBuffer4Callback2Wm9();
                    reply->command = 8;
                    reply->result = 0;
                    reply->state = 9;
                    reply->reason = reason;
                    reply->aid = aid;
                    __VENEER_MI_CpuCopy8(frame.childMac, reply->macAddress, 6);
                    reply->parentSize = status->parentSize;
                    reply->childSize = status->childSize;
                    __VENEER_WMSP_ReturnResult2Wm9(reply);
                } else {
                    WMSP_IndicateDisconnectionFromMyself(1, child, frame.childMac);
                }
                if (cleanup) {
                    WMSP_CleanSendQueue((u16)bit);
                }
            } else {
                __VENEER_OS_RestoreInterrupts((OSIntrMode)irq);
            }
        }
    }
    if (disconnected) {
        *disconnected = done;
    }
    return TRUE;
}
