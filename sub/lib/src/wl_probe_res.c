#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#pragma force_active off

// Writes the game information element (0xDD: active zone time, frame count, WlBss.gameInfo); returns its size.
static inline u32 SetGameInfoElement(u8 *element) {
    WlBss *bss;
    WlParam *param;
    u32 value;
    u32 i;
    u32 j;
    u8 *src;
    bss = &wlWorkPlain->bss;
    param = &wlWorkPlain->param;
    i = 0;
    WL_WriteByte(element, 0xDD);
    WL_WriteByte(element + 1, (u8)(wlWorkPlain->bss.gameInfoLength + 8));
    WL_WriteByte(element + 2, 0);
    WL_WriteByte(element + 3, 9);
    WL_WriteByte(element + 4, 0xBF);
    WL_WriteByte(element + 5, 0);
    WL_WriteByte(element + 6, (u8)param->activeZoneTime);
    WL_WriteByte(element + 7, (u8)(param->activeZoneTime >> 8));
    value = WL_VSYNC_TIME;
    WL_WriteByte(element + 8, (u8)value);
    WL_WriteByte(element + 9, (u8)(value >> 8));
    i += 10;
    if (bss->gameInfoLength) {
        src = bss->gameInfo;
        if (bss->gameInfoOdd & 1) {
            src++;
        }
        for (j = 0; j < bss->gameInfoLength; j++) {
            WL_WriteByte(element + i, WL_ReadByte(src));
            i++;
            src++;
        }
    }
    return i;
}

#pragma force_active on

// Probe response (when a management frame can be queued): timestamp, interval, capability, SSID, rates,
// DS parameter, game information.
WlTxHeapFrame *MakeProbeResFrame(const u16 *macAdrs) {
    WlBss *bss = &wlWorkPlain->bss;
    WlMaDataReq *buf;
    u8 *element;
    u32 length;

    if (!IsEnableManagement()) {
        return NULL;
    }
    buf = AllocateHeapBuf(&wlWorkPlain->heap.bufMan[0], bss->gameInfoLength + 0x78);
    if (!buf) {
        SetFatalErr(2);
        return (WlTxHeapFrame *)buf;
    }
    buf->header.command = 0xFFFF;
    InitManHeader(&buf->frame, macAdrs);
    // body: timestamp (8 bytes, set by the hardware), beacon interval, capability, elements
    *(u16 *)((u8 *)buf + 0x44) = bss->beaconPeriod;
    *(u16 *)((u8 *)buf + 0x46) = bss->capability;
    length = SetSSIDElement((u8 *)buf + 0x48);
    length += SetSupRateSet((u8 *)buf + 0x48 + length);
    element = (u8 *)buf + 0x48 + length;
    WL_WriteByte(element, 3);
    WL_WriteByte(element + 1, 1);
    WL_WriteByte(element + 2, (u8)wlWorkPlain->bss.channel);
    length += 3;
    length += SetGameInfoElement((u8 *)buf + 0x48 + length);
    buf->frame.bodyLength = length + 0xC;
    buf->frame.frame.length = buf->frame.bodyLength + 0x1C;
    buf->frame.frame.frameCtrl = 0x50;
    return &buf->frame;
}
