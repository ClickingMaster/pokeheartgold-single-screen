#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Parent mode: starts an MP sequence. Writes the MP frame (txq[3]) for the stations in the bitmap,
// allocates the MP state message (one entry per station, sent to the WMSP when the sequence ends) and
// programs the hardware to send the frame now (period 0) or after `period`.
int MA_MpReqCmd(WlMaMpReq *req, WlCmdCfm *cfm) {
    WlWork *w = wlWork;
    WlTxFrame *frame;
    WlTxCtrl *txCtrl = &w->txCtrl;
    WlParam *param = &w->param;
    u32 ctl;
    WlMpState *mp;
    u8 *tail;
    u8 *entry;
    u32 count;
    int send = 1;
    u32 flags = w->hwWorkarounds;
    u32 entrySize;
    u32 bytes;
    u32 irq;
    u32 slot;
    u16 bit;
    cfm->length = send;
    if (param->mode != 1) {
        return 11;
    }
    if (txCtrl->txq[3].active) {
        return 8;
    }
    frame = (WlTxFrame *)txCtrl->txq[3].frame;
    if (req->flags & 0x8000) {
        if (!(req->flags & 2)) {
            req->interval = txCtrl->mpInterval;
        }
        if (!(req->flags & 4)) {
            req->bitmap = txCtrl->mpLastRemainBitmap;
        }
        if (!(req->flags & 8)) {
            req->period = txCtrl->mpPeriod;
        }
        if (!(req->flags & 16)) {
            req->payloadBytes = txCtrl->mpPayloadBytes;
            send = 0;
        }
    }
    if (req->payloadBytes > 0x204) {
        return 5;
    }
    bit = 2;
    count = 0;
    for (; bit != 0; bit = (u16)(bit << 1)) {
        if (req->bitmap & bit) {
            count++;
        }
    }
    txCtrl->mpInterval = req->interval;
    if (req->interval & 0x8000) {
        req->interval &= 0x7FFF;
        slot = ((u32)req->interval - 208) >> 2;
        frame->duration = 234;
        if (slot > 0x10000) {
            return 5;
        }
    } else {
        u32 pad, offset, extra;
        if (flags & 2) {
            pad = 1;
            offset = 2;
            extra = 6;
        } else {
            pad = 0;
            offset = pad;
            extra = pad;
        }
        slot = req->interval + pad;
        if (slot > 0x206 + pad) {
            return 5;
        }
        req->interval = slot * 4 + 208 + offset;
        frame->duration = 234 + extra;
    }
    entrySize = (slot + 9) & ~1u;
    bytes = entrySize * count;
    mp = AllocateHeapBuf(&w->heap.bufMan[0], bytes + 26);
    txCtrl->mpState = mp;
    if (!mp) {
        return 8;
    }
    txCtrl->txq[3].active = 1;
    txCtrl->txq[3].pendingCount++;
    txCtrl->mpPeriod = req->period;
    txCtrl->mpBitmap = req->bitmap;
    txCtrl->mpAnsweredBitmap = 0;
    txCtrl->mpPayloadBytes = req->payloadBytes;
    txCtrl->mpRetryLimit = req->retryLimit;
    frame->status = 0;
    frame->clientBits = req->bitmap;
    frame->retry = 0;
    frame->rate = 20;
    frame->length = req->payloadBytes + 34;
    frame->frameCtrl = 0x228;
    frame->duration += (req->interval + 10) * count;
    WSetMacAdrs3(frame->addr1, (const u16 *)MP_ADRS, w->bss.bssid, param->macAddress);
    if ((req->flags & 0x8000) && frame->seqCtrl != 0xFFFF) {
        ctl = 0x4000;
    } else {
        frame->seqCtrl = 0xFFFF;
        ctl = 0;
    }
    frame->body0 = req->interval;
    frame->body1 = req->bitmap;
    frame->body2 = req->sequence;
    if (send && req->payloadBytes) {
        WUpdateCounter();
        DMA_Write((u8 *)frame + 0x2A, req->payload, req->payloadBytes);
    }
    if (flags & 4) {
        u32 length = req->payloadBytes + 2;
        u8 *payload = (u8 *)&frame->body2;
        tail = (u8 *)(((u32)(payload + length) + 3) & ~3u);
        ((u16 *)tail)[0] = 0xB6B8;
        ((u16 *)tail)[1] = 0x1D46;
    }
    txCtrl->mpState->header.command = 0x184;
    txCtrl->mpState->header.length = (bytes + 11) >> 1;
    txCtrl->mpState->remainBitmap = req->bitmap;
    txCtrl->mpState->count = count;
    txCtrl->mpState->entrySize = entrySize;
    txCtrl->mpState->retryOverCount = 0;
    entry = txCtrl->mpState->entries;
    {
        u32 aid;
        u16 station;
        for (aid = 1, station = 2; station != 0; station = (u16)(station << 1), aid++) {
            if (req->bitmap & station) {
                ((WlMpEntry *)entry)->length = 0xFFFF;
                ((WlMpEntry *)entry)->unk_02 = 0;
                ((WlMpEntry *)entry)->missCount = 0;
                ((WlMpEntry *)entry)->aid = aid;
                entry += entrySize;
            }
        }
    }
    WL_REG(0x0C4) = req->interval;
    WL_REG(0x0C0) = frame->duration;
    send = 0x10000 - req->offset;
    if (req->period == 0) {
        u32 t, base = (req->payloadBytes + 34) * 4 + 96;
        if (WL_REG(0x000) != 0x1440) {
            base += 1000;
        }
        {
            u32 subtotal = (u32)req->interval * count + 904;
            t = __VENEER__u32_div_f(base + subtotal + 50, 10);
        }
        irq = __VENEER_OS_DisableInterrupts();
        WL_REG(0x118) = t;
        WL_REG(0x090) = (((u32)frame & 0x3FFF) >> 1) | 0x8000 | ctl;
        __VENEER_OS_RestoreInterrupts((OSIntrMode)irq);
    } else {
        u32 t;
        irq = __VENEER_OS_DisableInterrupts();
        t = (u16)(send + WL_REG(0x0F8));
        t = __VENEER__u32_div_f(t, 10);
        if (t + 3 < req->period) {
            WL_REG(0x118) = req->period - t - 1;
            WL_REG(0x090) = (((u32)frame & 0x3FFF) >> 1) | 0x8000 | ctl;
            __VENEER_OS_RestoreInterrupts((OSIntrMode)irq);
        } else {
            __VENEER_OS_RestoreInterrupts((OSIntrMode)irq);
            ReleaseHeapBuf(&w->heap.bufMan[0], (WlHeapBuf *)txCtrl->mpState);
            txCtrl->txq[3].active = 0;
            txCtrl->txq[3].pendingCount--;
            return 5;
        }
    }
    return 0;
}
