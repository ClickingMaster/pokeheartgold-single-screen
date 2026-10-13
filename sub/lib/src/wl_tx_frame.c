#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Management frames (MakeDisAssFrame .. MakeProbeReqFrame) are built in a driver heap buffer shaped like an MA data
// request: command 0xFFFF (no confirm), the TX frame and its body (from WlTxFrame.body0).

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

void TxManCtrlFrame(WlTxHeapFrame *frame) {
    SetManCtrlFrame(frame);
    TxqPri(1);
}

// Queues a management frame (WlHeap.bufMan[2]) for the station it is addressed to (0: none in the table).
void SetManCtrlFrame(WlTxHeapFrame *frame) {
    VOLATILE_U16(frame->station) = (u16)CAM_Search(frame->frame.addr1);
    if (VOLATILE_U16(frame->station) == 255) {
        VOLATILE_U16(frame->station) = 0;
    }
    VOLATILE_U16(frame->time) = (u16)wlWork->bss.intervalCount;
    if (VOLATILE_U16(frame->frame.frameCtrl) & 0x4000) {
        VOLATILE_U16(frame->frame.length) += 8;
    }
    CAM_IncFrameCount(frame);
    {
        WlWork *w = wlWork;
        MoveHeapBuf(&w->heap.bufMan[0], &w->heap.bufMan[2], (WlHeapBuf *)((u8 *)frame - 16));
    }
}

// Child modes: sends the PS-Poll frame of txq[2] (again).
void TxPsPollFrame(void) {
    WlWork *w = wlWork;
    WlTxDesc *desc = &w->txCtrl.txq[2];
    ++VOLATILE_U16(desc->pendingCount);
    if (VOLATILE_U16(w->txCtrl.txq[2].active)) {
        VOLATILE_U16(((WlTxFrame *)desc->frame)->retry) = 0;
    } else {
        VOLATILE_U16(desc->active) = 1;
        VOLATILE_U16(((WlTxFrame *)desc->frame)->status) = 0;
        VOLATILE_U16(((WlTxFrame *)desc->frame)->retry) = 0;
        VOLATILE_U16(((WlTxFrame *)desc->frame)->rate) = (u16)WCalcManRate();
        WL_REG(0x0A8) = ((u16)(((u32)desc->frame & 0x3FFF) >> 1) | 0x8000);
    }
}

#pragma opt_propagation off

void StartBeaconFrame(void) {
    WlWork *w = wlWork;
    WlTxDesc *beacon = &w->txCtrl.beacon;
    VOLATILE_U16(w->txCtrl.beacon.active) = 1;
    WL_REG(0x080) = (((u32)beacon->frame & 0x3FFF) >> 1) | 0x8000;
}

#pragma opt_propagation reset

void StopBeaconFrame(void) {
    WlWork *w = wlWork;
    WL_REG(0x080) = 0;
    VOLATILE_U16(w->txCtrl.beacon.active) = 0;
}

// Builds the beacon: header, timestamp, interval, capability, SSID (unless hidden), rates, DS parameter,
// TIM, and the game information element (0xDD: active zone time, frame count, WlBss.gameInfo).
void MakeBeaconFrame(void) {
    u8 *source;
    WlParam *param;
    WlBss *bss;
    u32 i;
    WlTxFrame *frame;
    u8 *pos, *body;
    WlWork *w = wlWorkPlain;
    frame = (WlTxFrame *)w->txCtrl.beacon.frame;
    param = &w->param;
    bss = &w->bss;
    frame->status = 0;
    frame->clientBits = 0;
    frame->retry = 0;
    frame->reqRate = 0;
    frame->rate = WCalcManRate();
    frame->frameCtrl = 0x80;
    frame->duration = 0;
    WSetMacAdrs3(frame->addr1, (const u16 *)BC_ADRS, param->macAddress, param->macAddress);
    frame->seqCtrl = 0;
    body = (u8 *)&frame->body0;
    ((u32 *)body)[0] = 0;
    ((u32 *)body)[1] = 0;
    ((u16 *)body)[4] = bss->beaconPeriod;
    ((u16 *)body)[5] = bss->capability;
    pos = body + 12;
    if (!param->flags.bits.beaconType) {
        bss->ssidOffset = (u16)(pos - body);
        WL_WriteByte(pos, 0);
        {
            u8 *next = pos + 1;
            u32 length = bss->ssidLength & 255;
            pos += 2;
            WL_WriteByte(next, length);
        }
        for (i = 0; i < bss->ssidLength; i++) {
            WL_WriteByte(pos, WL_ReadByte(bss->ssid + i));
            pos++;
        }
        {
            u32 backwards = 0xFFFFFFFFU - i;
            WL_WriteByte((void *)((u32)pos + backwards), i & 255);
        }
    } else {
        bss->ssidOffset = 0;
    }
    pos += SetSupRateSet(pos);
    WL_WriteByte(pos, 3);
    WL_WriteByte(pos + 1, 1);
    WL_WriteByte(pos + 2, bss->channel & 255);
    bss->timOffset = (u16)(pos + 3 - body);
    WL_REG(0x084) = bss->timOffset + 2;
    WL_WriteByte(pos + 3, 5);
    WL_WriteByte(pos + 4, 5);
    WL_WriteByte(pos + 5, 0);
    WL_WriteByte(pos + 6, bss->dtimPeriod & 255);
    WL_WriteByte(pos + 7, 0);
    WL_WriteByte(pos + 8, 0);
    WL_WriteByte(pos + 9, 0);
    bss->gameInfoOffset = (u16)(pos + 10 - body);
    bss->gameInfoOdd = bss->gameInfoOffset & 1;
    WL_WriteByte(pos + 10, 0xDD);
    WL_WriteByte(pos + 11, (bss->gameInfoLength + 8) & 255);
    WL_WriteByte(pos + 12, 0);
    WL_WriteByte(pos + 13, 9);
    WL_WriteByte(pos + 14, 0xBF);
    WL_WriteByte(pos + 15, 0);
    if (bss->powerMgtMode == 1) {
        WL_WriteByte(pos + 16, param->activeZoneTime & 255);
        {
            u8 *next = pos + 17;
            u32 value = (s32)param->activeZoneTime >> 8;
            pos += 18;
            WL_WriteByte(next, value & 255);
        }
    } else {
        WL_WriteByte(pos + 16, 255);
        {
            u8 *next = pos + 17;
            pos += 18;
            WL_WriteByte(next, 255);
        }
    }
    {
        u32 value = WL_VSYNC_TIME;
        WL_WriteByte(pos, value & 255);
        {
            u8 *next = pos + 1;
            pos += 2;
            WL_WriteByte(next, (value >> 8) & 255);
        }
    }
    source = bss->gameInfo;
    for (i = 0; i < bss->gameInfoLength; i++) {
        WL_WriteByte(pos, WL_ReadByte(source));
        pos++;
        source++;
    }
    if (bss->gameInfoOdd) {
        u32 length = bss->gameInfoLength;
        source = (u8 *)((u32)bss->gameInfo + length - 1U);
        i = 0;
        if (i < length) {
            for (; i < bss->gameInfoLength;) {
                WL_WriteByte(source + 1, WL_ReadByte(source));
                i++;
                source--;
            }
        }
    }
    if (wlWorkPlain->hwWorkarounds & 4) {
        u32 aligned = ((u32)pos + 3) & ~3U;
        ((u16 *)aligned)[0] = 0xB6B8;
        ((u16 *)aligned)[1] = 0x1D46;
    }
    bss->gameInfoUpdated = 0;
    frame->length = (u16)(pos + 0x1C - body);
}

// Copies the game information into the beacon's game information element.
void UpdateGameInfoElement(void) {
    WlWork *w = wlWork;
    WlBss *bss = &w->bss;
    u8 *beacon = *(u8 *volatile *)&w->txCtrl.beacon.frame;
    u32 length = bss->gameInfoLength;
    u8 *out = beacon + 0x24 + bss->gameInfoOffset;
    if (length) {
        u16 odd = bss->gameInfoOdd;
        u8 *source = *(u8 *volatile *)&bss->gameInfo, *destination = out + 10;
        if (odd & 1) {
            DMA_Write(destination - 1, source, length + 2);
            WL_WriteByte(out + 9, (WL_VSYNC_TIME >> 8) & 255);
        } else {
            DMA_Write(destination, source, length + 1);
        }
    }
    ((WlTxFrame *)*(u8 *volatile *)&wlWork->txCtrl.beacon.frame)->length = (u16)(bss->gameInfoOffset + 0x26 + bss->gameInfoLength);
    WL_WriteByte(out + 1, (bss->gameInfoLength + 8) & 255);
    if (wlWork->hwWorkarounds & 4) {
        u8 *end = (u8 *)((u32)(out + 13 + bss->gameInfoLength) & ~3u);
        ((u16 *)end)[0] = 0xB6B8;
        ((u16 *)end)[1] = 0x1D46;
    }
    bss->gameInfoUpdated = 0;
}

// Room for another management frame: fewer than 24 queued, counting one per AID handed out.
int IsEnableManagement(void) {
    WlWork *w = wlWork;
    u32 count = w->cam.aidCount, pending = w->heap.bufMan[2].count;
    return (int)pending < 24 - (int)count;
}

WlTxHeapFrame *MakeDisAssFrame(const u16 *macAdrs, u32 reason) {
    WlMaDataReq *buf = AllocateHeapBuf(&wlWork->heap.bufMan[0], 0x36);
    if (!buf) {
        SetFatalErr(2);
        return (WlTxHeapFrame *)buf;
    }
    buf->header.command = 0xFFFF;
    InitManHeader(&buf->frame, macAdrs);
    buf->frame.frame.body0 = (u16)reason;
    buf->frame.bodyLength = 2;
    buf->frame.frame.length = (u16)(buf->frame.bodyLength + 28);
    buf->frame.frame.frameCtrl = 0xA0;
    return &buf->frame;
}

// Body: capability, listen interval, SSID, supported rates.
WlTxHeapFrame *MakeAssReqFrame(const u16 *macAdrs) {
    WlWork *w = wlWork;
    WlBss *bss = &w->bss;
    WlMaDataReq *buf = AllocateHeapBuf(&w->heap.bufMan[0], 0x5E);
    u32 length;
    if (!buf) {
        SetFatalErr(2);
        return (WlTxHeapFrame *)buf;
    }
    VOLATILE_U16(buf->header.command) = 0xFFFF;
    InitManHeader(&buf->frame, macAdrs);
    VOLATILE_U16(buf->frame.frame.body0) = VOLATILE_U16(bss->capability);
    VOLATILE_U16(buf->frame.frame.body1) = VOLATILE_U16(bss->listenInterval);
    length = SetSSIDElement((u8 *)&buf->frame.frame.body2);
    length += SetSupRateSet((u8 *)&buf->frame.frame.body2 + length);
    VOLATILE_U16(buf->frame.bodyLength) = (u16)(length + 4);
    VOLATILE_U16(buf->frame.frame.length) = (u16)(VOLATILE_U16(buf->frame.bodyLength) + 28);
    VOLATILE_U16(buf->frame.frame.frameCtrl) = 0;
    return &buf->frame;
}

// Body: capability, listen interval, current AP, SSID, supported rates.
WlTxHeapFrame *MakeReAssReqFrame(const u16 *macAdrs) {
    WlWork *w = wlWork;
    WlBss *bss = &w->bss;
    WlMaDataReq *buf = AllocateHeapBuf(&w->heap.bufMan[0], 0x64);
    u32 length;
    if (!buf) {
        SetFatalErr(2);
        return (WlTxHeapFrame *)buf;
    }
    VOLATILE_U16(buf->header.command) = 0xFFFF;
    InitManHeader(&buf->frame, macAdrs);
    VOLATILE_U16(buf->frame.frame.body0) = VOLATILE_U16(bss->capability);
    VOLATILE_U16(buf->frame.frame.body1) = VOLATILE_U16(bss->listenInterval);
    WSetMacAdrs1(&buf->frame.frame.body2, bss->parentMacAdrs);
    length = SetSSIDElement((u8 *)&buf->frame.frame.body2 + 6);
    length += SetSupRateSet((u8 *)&buf->frame.frame.body2 + 6 + length);
    VOLATILE_U16(buf->frame.bodyLength) = (u16)(length + 10);
    VOLATILE_U16(buf->frame.frame.length) = (u16)(VOLATILE_U16(buf->frame.bodyLength) + 28);
    VOLATILE_U16(buf->frame.frame.frameCtrl) = 0x20;
    return &buf->frame;
}

// Body: capability, status (19 when no AID is left), AID, supported rates; the station's request
// element `element` (id, length, data) follows the frame.
WlTxHeapFrame *MakeAssResFrame(u32 station, u32 status, const u8 *element) {
    const u8 *extra = element;
    WlMaDataReq *frame = AllocateHeapBuf(&wlWorkPlain->heap.bufMan[0], 96);
    u8 *p = (u8 *)frame;
    u32 aid;
    if (!p) {
        SetFatalErr(2);
        return (WlTxHeapFrame *)p;
    }
    frame->header.command = 65535;
    if (status == 0) {
        aid = CAM_AllocateAID(station);
        if (aid == 0) {
            status = 19;
        }
    } else {
        aid = 0;
    }
    InitManHeader(&frame->frame, CAM_GetMacAdrs(station));
    frame->frame.frame.body0 = wlWorkPlain->bss.capability;
    frame->frame.frame.body1 = status;
    frame->frame.frame.body2 = aid;
    if (aid) {
        frame->frame.frame.body2 |= 0xC000;
    }
    frame->frame.bodyLength = SetSupRateSet(p + 0x42) + 6;
    frame->frame.frame.length = frame->frame.bodyLength + 28;
    frame->frame.frame.frameCtrl = 0x10;
    {
        const u8 *source;
        u32 i;
        u8 *dest;
        u32 length;
        dest = p + 0x24 + frame->frame.frame.length;
        if (extra) {
            length = WL_ReadByte(extra + 1);
            WL_WriteByte(dest++, WL_ReadByte(extra));
            WL_WriteByte(dest++, length & 255u);
            i = 0;
            if (i < length) {
                source = extra + 2;
                while (i < length) {
                    WL_WriteByte(dest, WL_ReadByte(source + i));
                    i++;
                    dest++;
                }
            }
        } else {
            WL_WriteByte(dest, 0);
            WL_WriteByte(dest + 1, 0);
        }
    }
    return (WlTxHeapFrame *)(p + 16);
}

WlTxHeapFrame *MakeReAssResFrame(u32 station, u32 status, const u8 *extra) {
    WlMaDataReq *frame = AllocateHeapBuf(&wlWorkPlain->heap.bufMan[0], 96);
    u8 *p = (u8 *)frame;
    u32 aid;
    if (!p) {
        SetFatalErr(2);
        return (WlTxHeapFrame *)p;
    }
    frame->header.command = 65535;
    if (status == 0) {
        aid = CAM_AllocateAID(station);
        if (aid == 0) {
            status = 19;
        }
    } else {
        aid = 0;
    }
    InitManHeader(&frame->frame, CAM_GetMacAdrs(station));
    frame->frame.frame.body0 = wlWorkPlain->bss.capability;
    frame->frame.frame.body1 = status;
    frame->frame.frame.body2 = aid | 0xC000;
    frame->frame.bodyLength = SetSupRateSet(p + 0x42) + 6;
    frame->frame.frame.length = frame->frame.bodyLength + 28;
    frame->frame.frame.frameCtrl = 0x30;
    {
        u32 i;
        u8 *dest;
        u32 length;
        dest = p + 0x24 + frame->frame.frame.length;
        if (extra) {
            length = WL_ReadByte(extra + 1);
            WL_WriteByte(dest, WL_ReadByte(extra));
            WL_WriteByte(dest + 1, length & 255u);
            dest += 2;
            extra += 2;
            for (i = 0; i < length; i++) {
                WL_WriteByte(dest, WL_ReadByte(extra + i));
                dest++;
            }
        } else {
            WL_WriteByte(dest, 0);
            WL_WriteByte(dest + 1, 0);
        }
    }
    return (WlTxHeapFrame *)(p + 16);
}

// Body: SSID, supported rates.
WlTxHeapFrame *MakeProbeReqFrame(const u16 *bssid) {
    WlMaDataReq *p = AllocateHeapBuf(&wlWorkPlain->heap.bufMan[0], 90);
    u32 length;
    if (!p) {
        SetFatalErr(2);
        return (WlTxHeapFrame *)p;
    }
    p->header.command = 65535;
    InitManHeader(&p->frame, bssid);
    length = SetSSIDElement((u8 *)&p->frame.frame.body0);
    length += SetSupRateSet((u8 *)&p->frame.frame.body0 + length);
    p->frame.bodyLength = length;
    p->frame.frame.length = length + 28;
    p->frame.frame.frameCtrl = 0x40;
    return &p->frame;
}

#pragma dont_inline reset
