#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Reports the failed WL command `command` (driver result `result`) of WMSP_StartParent.
void WmspError_060033F4(u16 command, u16 result) {
    u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 8;
    cb[1] = 1;
    cb[4] = 0;
    cb[2] = command;
    cb[3] = result;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

void WMSP_EndParent(void *request) {
    u8 mac[8], buf[512];
    int aid, retry;
    WmspStatus *status = wmspW.status;
    u16 *cb;
    if (status->state != 7) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 9;
        cb[1] = 3;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    status->parentParam.unk_0e = 0;
    // Deauthenticate every connected child.
    for (aid = 1; aid < 16; aid++) {
        u32 mask = 1u << aid;
        if (status->childBitmap & mask) {
            __VENEER_MI_CpuCopy8(status->childMacAddress[aid - 1], mac, 6);
            retry = 0;
            while (retry < 2) {
                u16 error = WMSP_WL_MlmeDeAuthenticate((u16 *)buf, mac, 3)[2];
                if (error == 0) {
                    break;
                }
                if (error != 7 && error != 12) {
                    break;
                }
                retry++;
            }
            OSIntrMode irq = __VENEER_OS_DisableInterrupts();
            if (status->childBitmap & mask) {
                status->childBitmap &= ~mask;
                status->unk_86 &= ~mask;
                status->lastRecvTick[aid] = 0;
                __VENEER_OS_RestoreInterrupts(irq);
                WMSP_IndicateDisconnectionFromMyself(TRUE, (u16)aid, mac);
            } else {
                __VENEER_OS_RestoreInterrupts(irq);
            }
        }
    }
    u16 error = WMSP_WL_MlmeReset((u16 *)buf, 1)[2];
    if (error) {
        WmspError_060035F4(0, error);
        return;
    }
    status->unk_c2 = 0;
    status->state = 3;
    error = WMSP_WL_DevIdle(buf)[2];
    if (error) {
        WmspError_060035F4(0x302, error);
        return;
    }
    status->state = 2;
    status->wepEnabled = 0;
    status->wepMode = 0;
    __VENEER_MI_CpuFill8(status->wepKey, 0, sizeof(status->wepKey));
    WMSP_ResetSizeVars();
    cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 9;
    cb[1] = 0;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

// Reports the failed WL command `command` (driver result `result`) of WMSP_EndParent.
void WmspError_060035F4(u16 command, u16 result) {
    u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 9;
    cb[1] = 1;
    cb[2] = command;
    cb[3] = result;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

// Scans one channel (request: channel, ARM9 result buffer, duration, BSSID filter).
void WMSP_StartScan(void *request) {
    u32 buffer[128];
    u8 ssid[32];
    u8 channels[16];
    u16 bssid[3];
    u16 *buf = (u16 *)buffer;
    u16 channel, duration;
    WmspStatus *status = wmspW.status;
    u16 *reply, *cb;
    u32 rssi;
    if (status->state != 2 && status->state != 3 && status->state != 5) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 10;
        cb[1] = 3;
        cb[4] = 4;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    status->scanBuf = *(void **)((u8 *)request + 4);
    channel = *(u16 *)((u8 *)request + 2);
    status->scanChannel = channel;
    duration = *(u16 *)((u8 *)request + 8);
    __VENEER_MI_CpuCopy8((u8 *)request + 10, bssid, 6);
    if (bssid[0] != 0xffff && (bssid[0] & 1)) {
        bssid[0] &= ~1;
    }
    if (channel == 0) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 10;
        cb[1] = 6;
        cb[4] = 4;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    if (!((1u << channel) & status->enableChannel)) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 10;
        cb[1] = 6;
        cb[4] = 4;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    status->unk_e6 = 2;
    reply = WMSP_WL_DevGetStationState(buf);
    if (reply[2]) {
        WmspError_06003ED4(0x308, reply[2], 0);
        return;
    }
    if (reply[3] == 16) {
        if (!WMSP_SetAllParams(10, (u8 *)buf)) {
            return;
        }
        reply = WMSP_WL_DevClass1(buf);
        if (reply[2]) {
            WmspError_06003ED4(0x303, reply[2], 0);
            return;
        }
        status->state = 3;
        reply = WMSP_WL_MlmePowerManagement(buf, 1, 0, 1);
        if (reply[2]) {
            WmspError_06003ED4(1, reply[2], 0);
            return;
        }
        status->pwrMgtMode = 1;
    }
    status->state = 5;
    __VENEER_MIi_CpuClear16(0xffff, ssid, 32);
    channels[0] = channel;
    __VENEER_MI_CpuFill8(channels + 1, 0, 15);
    reply = WMSP_WL_MlmeScan(buf, 0x11e, bssid, 0, ssid, 1, channels, duration);
    if (reply[2]) {
        WmspError_06003ED4(2, reply[2], 0);
        return;
    }
    cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    if (reply[4] == 0) {
        cb[0] = 10;
        cb[1] = 0;
        cb[4] = 4;
        cb[8] = channel;
        cb[9] = 0;
    } else {
        __VENEER_MIi_CpuClear16(0, (u8 *)status->scanBuf + 64, 128);
        __VENEER_MI_CpuCopy8(reply + 5, status->scanBuf, reply[5] * 2);
        cb[0] = 10;
        cb[1] = 0;
        cb[4] = 5;
        cb[8] = reply[0x20];
        rssi = WMSP_GetRssi8_06003988(reply[6] & 255);
        cb[9] = WMSP_GetLinkLevel(rssi);
        WMSP_AddRssiToRandomPool(rssi);
        cb[10] = reply[10];
        __VENEER_MI_CpuCopy8(reply + 7, cb + 5, 6);
        __VENEER_MIi_CpuCopy16(reply + 11, cb + 11, 32);
        cb[27] = reply[0x23];
        if (cb[27] > 128) {
            cb[0] = 10;
            cb[1] = 0;
            cb[4] = 4;
            cb[8] = channel;
            cb[9] = 0;
        } else {
            __VENEER_MIi_CpuClear16(0, cb + 28, 128);
            __VENEER_MIi_CpuCopy16(reply + 37, cb + 28, (cb[27] + 1) & ~1u);
        }
    }
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

// Converts the RSSI byte of a received frame (copy of WMSP_GetRssi8_06001B5C).
u32 WMSP_GetRssi8_06003988(int rssi) {
    int out = rssi >> 2;
    if (!(rssi & 2)) {
        out += 25;
    }
    return (u8)out;
}

// Mixes an RSSI value into the random seed word at 0x027FFF98 (shared with the ARM9).
void WMSP_AddRssiToRandomPool(u32 rssi) {
    volatile u16 *seed = (void *)0x027fff98;
    rssi ^= (u32)*seed << 1;
    rssi ^= rssi >> 16;
    *seed = rssi;
}

#pragma dont_inline reset
