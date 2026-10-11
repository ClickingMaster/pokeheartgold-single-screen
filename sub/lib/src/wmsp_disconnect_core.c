#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W40889State {
    u16 state;
    u8 reserved02[10];
    u32 active, field10, field14;
    u8 reserved18[4];
    u32 field1c;
    u8 reserved20[16];
    u16 field30, field32;
    u8 reserved34[0x52];
    u16 field86;
    u8 reserved88[0x3a];
    u16 fieldc2;
    u8 reservedc4[0x64];
    u8 children[15][6];
    u16 connected;
    u8 reserved184[4];
    u16 parentAid;
    u8 parent[6];
    u8 reserved190[6];
    u16 field196;
    u32 field198;
    u8 area[80];
    u8 reserved1ec[0x738 - 0x1ec];

    struct {
        u32 low, high;
    } ticks[16];
} W40889State;

typedef struct W40889Owner {
    u8 pad[0x550];
    W40889State *status;
} W40889Owner;

typedef struct W40889Request {
    u32 command, mask, reason;
} W40889Request;

typedef struct W40889ParentCallback {
    u16 api, error, reserved[2], event, aid, reason;
    u8 pad[2], mac[6];
    u16 size30, size32;
} W40889ParentCallback;

typedef struct W40889ChildCallback {
    u16 api, error, reserved[2], event;
    u8 mac[6];
    u16 aid, reason;
    u8 pad[24];
    u16 size30, size32;
} W40889ChildCallback;

extern u8 wmspW[];
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern void __VENEER_MI_CpuCopy8(const void *, void *, u32);
extern void __VENEER_MI_CpuFill8(void *, u8, u32);
extern u32 __VENEER_OS_DisableInterrupts(void);
extern u32 __VENEER_OS_RestoreInterrupts(u32);
extern void WMSP_CancelVAlarm(void);
extern void WMSP_SetThreadPriorityLow(void);
extern void WMSP_ResetSizeVars(void);
extern void WMSP_CleanSendQueue(u16);
extern void WMSP_IndicateDisconnectionFromMyself(u16, u16, const void *);
extern void WmspIndError(u16, u16, u16, u16);
extern void WmspError_06004CB4(u16, u16, u16, u16);
extern u16 *WMSP_WL_MlmeDeAuthenticate(u16 *, const void *, u16);
extern u16 *WMSP_WL_MlmeReset(u16 *, u16);
extern u16 *WMSP_WL_DevIdle(void *);
int WMSP_DisconnectCore(W40889Request *, int, u16 *);

int WMSP_DisconnectCore(W40889Request *req, int indication, u16 *disconnected) {
    struct W40889Frame {
        u8 childMac[6], parentMac[6];
        u16 output[256];
    } frame;

#define parentMac frame.parentMac
#define childMac  frame.childMac
    int cleanup, child;
    u16 *output = frame.output;
    W40889State *status = ((W40889Owner *)(wmspW + 0x1000))->status;
    u16 mask = req->mask, reason = indication ? req->reason : 0;
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
        if (status->active == 1) {
            cleanup = 1;
        }
    } else if (status->state == 10 || status->state == 8) {
        irq = __VENEER_OS_DisableInterrupts();
        if (!status->connected) {
            __VENEER_OS_RestoreInterrupts(irq);
            if (!indication) {
                callback = __VENEER_WMSP_GetBuffer4Callback2Wm9();
                callback[0] = 13;
                callback[1] = 3;
                callback[2] = 0;
                callback[3] = 0;
                callback[4] = mask;
                callback[5] = 0;
                __VENEER_WMSP_ReturnResult2Wm9(callback);
            }
            return 0;
        }
        if (status->active == 1) {
            status->active = 0;
            cleanup = 1;
            WMSP_CancelVAlarm();
            WMSP_SetThreadPriorityLow();
            if (status->state == 10) {
                status->state = 8;
            }
        }
        status->connected = 0;
        status->field86 = 0;
        status->field14 = 0;
        status->field10 = 0;
        status->field1c = 0;
        __VENEER_OS_RestoreInterrupts(irq);
    } else {
        if (!indication) {
            callback = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            callback[0] = 13;
            callback[1] = 3;
            callback[2] = 0;
            callback[3] = 0;
            callback[4] = mask;
            callback[5] = 0;
            __VENEER_WMSP_ReturnResult2Wm9(callback);
        }
        return 0;
    }
    if (status->state == 10 || status->state == 8) {

        __VENEER_MI_CpuCopy8(status->parent, parentMac, 6);
        for (bit = 0; bit < 2;) {
            result = WMSP_WL_MlmeDeAuthenticate(output, parentMac, 3);
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
                return 0;
            }
        }
deauthenticated:
        status->fieldc2 = 0;
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
            return 0;
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
            return 0;
        }
        status->state = 2;
        status->field198 = 0;
        status->field196 = 0;
        __VENEER_MI_CpuFill8(status->area, 0, 80);
        WMSP_ResetSizeVars();
        if (indication == 1) {
            W40889ParentCallback *reply = (W40889ParentCallback *)__VENEER_WMSP_GetBuffer4Callback2Wm9();
            reply->api = 12;
            reply->error = 0;
            reply->event = 9;
            reply->reason = reason;
            reply->aid = status->parentAid;
            __VENEER_MI_CpuCopy8(parentMac, reply->mac, 6);
            reply->size30 = status->field30;
            reply->size32 = status->field32;
            __VENEER_WMSP_ReturnResult2Wm9(reply);
        } else {
            WMSP_IndicateDisconnectionFromMyself(0, 0, parentMac);
        }
        if (cleanup) {
            WMSP_CleanSendQueue(1);
        }
    } else {
        for (child = 1; child < 16; child++) {
            bit = 1u << child;
            if (!(bit & (status->connected & mask))) {
                continue;
            }
            offset = (child - 1) * 6;
            mac = &status->children[0][0];
            aid = child;
            __VENEER_MI_CpuCopy8(mac + offset, childMac, 6);
            for (retries = 0; retries < 2;) {
                result = WMSP_WL_MlmeDeAuthenticate(output, childMac, 3);
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
                    return 0;
                }
            }
deauthed:
            irq = __VENEER_OS_DisableInterrupts();
            if (status->connected & bit) {
                done |= 1u << aid;
                status->connected &= ~bit;
                status->field86 &= ~bit;
                status->ticks[aid].low = 0;
                status->ticks[aid].high = 0;
                __VENEER_MI_CpuFill8(mac + offset, 0, 6);
                __VENEER_OS_RestoreInterrupts(irq);
                if (indication == 1) {
                    W40889ChildCallback *reply = (W40889ChildCallback *)__VENEER_WMSP_GetBuffer4Callback2Wm9();
                    reply->api = 8;
                    reply->error = 0;
                    reply->event = 9;
                    reply->reason = reason;
                    reply->aid = aid;
                    __VENEER_MI_CpuCopy8(childMac, reply->mac, 6);
                    reply->size30 = status->field30;
                    reply->size32 = status->field32;
                    __VENEER_WMSP_ReturnResult2Wm9(reply);
                } else {
                    WMSP_IndicateDisconnectionFromMyself(1, child, childMac);
                }
                if (cleanup) {
                    WMSP_CleanSendQueue(bit);
                }
            } else {
                __VENEER_OS_RestoreInterrupts(irq);
            }
        }
    }
    if (disconnected) {
        *disconnected = done;
    }
    return 1;
}
