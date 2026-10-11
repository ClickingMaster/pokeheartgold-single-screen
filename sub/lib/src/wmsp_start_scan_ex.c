#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))

// Callback of WMSP_StartScanEx: header words, then the BSS descriptions found and their link levels.
typedef struct WmspScanExCallback {
    u16 header[8]; // command, result, -, -, 4 (none found) / 5, channel bits, -, count
    void *bssDesc[16];
    u16 linkLevel[16];
} WmspScanExCallback;

// TRUE if the BSS description has a non-empty SSID (length at +10, SSID bytes at +12).
static inline int BssDescHasSsid(void *p) {
    u16 len = H(p, 10);
    int i;
    if (len == 0) {
        return 0;
    }
    if (len > 32) {
        return 0;
    }
    for (i = 0; i < len; i++) {
        if (*((u8 *)p + 12 + i) != 0) {
            return 1;
        }
    }
    return 0;
}

#pragma opt_lifetimes off

// Scans the requested channels (mode 2/3: with an SSID match mask) and replies the BSS
// descriptions found, copied into the ARM9's buffer.
void WMSP_StartScanEx(WmspStartScanExRequest *req) {
    u32 bufArr[278];
    u8 ssid[32];
    u8 mask[32];
    u8 channels[16];
    u16 bssid[3];
    u32 *buffer = bufArr;
    WmspStatus *s = wmspW.status;
    int bits;

    struct {
        u16 custom, matchlen;
    } options;

    u16 mode, maxduration, capacity, ssidlen, count, j;
    u16 *r, *c;
    int i;
    u32 rssi;
    u8 *entry, *dest;
    u16 bytes;
    if (s->state != 2 && s->state != 3 && s->state != 5) {
        c = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        c[0] = 38;
        c[1] = 3;
        c[4] = 4;
        __VENEER_WMSP_ReturnResult2Wm9(c);
        return;
    }
    s->scanBuf = req->output;
    bits = req->channels;
    s->scanChannel = bits;
    maxduration = req->duration;
    __VENEER_MI_CpuCopy8(req->bssid, bssid, 6);
    mode = req->mode;
    options.matchlen = req->matchlen;
    switch (mode) {
    case 2:
        options.custom = 1;
        mode = 0;
        break;
    case 3:
        options.custom = 1;
        mode = 1;
        break;
    default:
        options.custom = 0;
        break;
    }
    ssidlen = req->length;
    __VENEER_MI_CpuCopy8(req->ssid, ssid, 32);
    capacity = req->capacity;
    if (bssid[0] != 65535 && (bssid[0] & 1)) {
        bssid[0] &= ~1;
    }
    bits = s->enableChannel & (u16)(bits << 1);
    if (bits && (!(s->unk_c8 & 1) || mode == 1) && req->output && !((u32)req->output & 3) && req->capacity >= 64) {
        goto valid_scan;
    }
    {
        c = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        c[0] = 38;
        c[1] = 6;
        c[4] = 4;
        __VENEER_WMSP_ReturnResult2Wm9(c);
        return;
    }

valid_scan: {
    u16 role = 2;
    s->unk_e6 = role;
}
    r = WMSP_WL_DevGetStationState(buffer);
    if (!r[2]) {
        goto station_ready;
    }
    WmspError_06003ED4(0x308, r[2], 1);
    return;
station_ready:;
    if (r[3] == 16) {
        if (!WMSP_SetAllParams(38, (u8 *)buffer)) {
            return;
        }
        r = WMSP_WL_DevClass1(buffer);
        if (r[2]) {
            WmspError_06003ED4(0x303, r[2], 1);
            return;
        }
        s->state = 3;
        r = WMSP_WL_MlmePowerManagement(buffer, 1, 0, 1);
        if (r[2]) {
            WmspError_06003ED4(1, r[2], 1);
            return;
        }
        s->pwrMgtMode = 1;
    }
    if (mode == 0) {
        if (s->unk_1ee == 1) {
            r = WMSP_WL_ParamSetPreambleType(buffer, 0);
            if (r[2]) {
                WmspError_06003ED4(0x20e, r[2], 1);
                return;
            }
            s->unk_1ee = 0;
        }
    } else if (s->unk_1ee == 0) {
        r = WMSP_WL_ParamSetPreambleType(buffer, 1);
        if (r[2]) {
            WmspError_06003ED4(0x20e, r[2], 1);
            return;
        }
        s->unk_1ee = 1;
    }
    if (options.custom != 1) {
        goto mask_ready;
    }
    {
        __VENEER_MI_CpuFill8(mask, 255, 32);
        if (options.matchlen <= 32) {
            __VENEER_MI_CpuFill8(mask, 0, options.matchlen);
        }
        r = WMSP_WL_ParamSetSsidMask(buffer, mask);
        if (r[2]) {
            WmspError_06003ED4(0x20d, r[2], 1);
            return;
        }
    }
mask_ready:
    count = 0;
    s->state = 5;
    __VENEER_MI_CpuFill8(channels, 0, 16);
    for (j = 1; j < 15; j++) {
        if (bits & (1u << j)) {
            channels[count++] = j;
        }
    }
    r = WMSP_WL_MlmeScan(buffer, capacity - 2 * __VENEER__u32_div_f((u32)(capacity - 64), 66) + 94, bssid, ssidlen, ssid, mode, channels, maxduration);
    if (r[2]) {
        WmspError_06003ED4(2, r[2], 1);
        return;
    }
    c = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    if (r[4] == 0) {
        c[0] = 38;
        c[1] = 0;
        c[4] = 4;
        c[7] = 0;
        c[5] = bits >> 1;
    } else {
        dest = s->scanBuf;
        entry = (u8 *)r + 10;
        __VENEER_MIi_CpuClear16(0, dest, capacity);
        for (i = 0; i < r[4]; i++) {
            bytes = (u16)(H(entry, 0) * 2);
            __VENEER_MI_CpuCopy8(entry, dest, bytes);
            if (ssidlen != 0 && !BssDescHasSsid(dest)) {
                H(dest, 10) = ssidlen;
                __VENEER_MI_CpuCopy8(ssid, dest + 12, 32);
            }
            ((WmspScanExCallback *)c)->bssDesc[i] = dest;
            rssi = WMSP_GetRssi8_06003988(H(entry, 2) & 255);
            ((WmspScanExCallback *)c)->linkLevel[i] = WMSP_GetLinkLevel(rssi);
            WMSP_AddRssiToRandomPool(rssi);
            dest += bytes;
            entry += bytes;
            if ((u32)dest & 2) {
                dest = (u8 *)(((u32)dest + 2) & ~3u);
            }
        }
        c[0] = 38;
        c[1] = 0;
        c[4] = 5;
        c[7] = r[4];
        c[5] = bits >> 1;
    }
    __VENEER_WMSP_ReturnResult2Wm9(c);
}
