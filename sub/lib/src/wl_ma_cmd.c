#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// MA command handlers (WlibCmdTbl_MA): data and key data frames.

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Queues a data frame (the request message itself becomes the TX frame): parent mode to the station
// addressed (station 0: broadcast, held for the stations in power save), child modes to the parent.
int MA_DataReqCmd(WlMaDataReq *req, WlCmdCfm *cfm) {
    WlWork *w = wlWork;
    WlBss *bss = &w->bss;
    WlParam *param = &w->param;
    u32 id;
    if (req->frame.bodyLength > 0x5E4) {
        return 5;
    }
    if (param->mode == 1) {
        id = CAM_Search(req->frame.frame.addr1);
        if (id == 255 || CAM_GetStaState(id) != 64) {
            return 10;
        }
    } else {
        id = bss->parentStation;
    }
    req->frame.station = id;
    req->frame.time = wlWork->bss.intervalCount;
    if (req->frame.frame.unk_06 & 255) {
        req->frame.frame.rate = req->frame.frame.unk_06;
        req->frame.frame.unk_06 = 0;
    } else {
        req->frame.frame.rate = CAM_GetTxRate(id);
    }
    if (req->frame.bodyLength == 0) {
        req->frame.frame.frameCtrl = (bss->unk_8a | 64) & ~0x4000;
        req->frame.frame.length = 28;
    } else {
        req->frame.frame.frameCtrl = bss->unk_8a;
        if (param->wepMode == 0) {
            req->frame.frame.length = req->frame.bodyLength + 28;
        } else {
            req->frame.frame.length = req->frame.bodyLength + 36;
        }
    }
    switch (param->mode) {
    case 1:
        WSetMacAdrs1(req->frame.frame.addr3, req->frame.frame.addr2);
        WSetMacAdrs1(req->frame.frame.addr2, bss->bssid);
        if (id == 0) {
            CAM_AddBcFrame(&w->heap.bufMan[10], (WlHeapBuf *)req);
            {
                WlWork *w2 = wlWork;
                if ((w2->cam.powerMgtMap & ~w2->cam.unk_06) == 0) {
                    TxqPri(2);
                }
            }
        } else {
            CAM_IncFrameCount(&req->frame);
            MoveHeapBuf(&w->heap.bufMan[10], &w->heap.bufMan[1], (WlHeapBuf *)req);
            TxqPri(0);
        }
        break;
    case 2:
    case 3:
        WSetMacAdrs1(req->frame.frame.addr3, req->frame.frame.addr1);
        WSetMacAdrs1(req->frame.frame.addr1, bss->bssid);
        CAM_IncFrameCount(&req->frame);
        MoveHeapBuf(&w->heap.bufMan[10], &w->heap.bufMan[1], (WlHeapBuf *)req);
        TxqPri(0);
        break;
    }
    return 0x81;
}

// Child mode: writes a key data frame for the parent's MP sequence into one of the two key data slots
// (txq[4], txq[5]) of the MAC memory and hands it to the hardware (register 0x094).
int MA_KeyDataReqCmd(WlMaKeyDataReq *req, WlCmdCfm *cfm) {
    WlParam *param;
    u8 *q; // &WlWork.txCtrl; the key data slot (txq[4] or txq[5]) is picked by the byte offset `offset`
    u32 flags;
    WlTxFrame *frame;
    u32 offset, i = 1;
    u16 *slots;
    u32 irq;
    WlWork *w = wlWork;
    u8 *tail;
    flags = w->hwWorkarounds;
    param = (WlParam *)&w->param;
    cfm->length = i;
    q = (u8 *)&w->txCtrl;
    if (param->mode != 2) {
        return 11;
    }
    if (req->length > 0x204) {
        return 5;
    }
    if (((WlTxCtrl *)q)->txq[4].active == 0) {
        i = 0;
    }
    slots = &((WlTxCtrl *)q)->txq[4].active;
    offset = i * sizeof(WlTxDesc);
    if (*(u16 *)((u8 *)slots + offset)) {
        return 8;
    }
    if (WL_REG(0x094) & 0x8000) {
        return 8;
    }
    frame = *(WlTxFrame **)(q + offset + 0x58); // txq[4 + i].frame
    frame->status = 0;
    frame->retry = 0;
    frame->rate = 20;
    frame->length = req->length + 30;
    frame->frameCtrl = 0x118;
    WSetMacAdrs3(frame->addr1, w->bss.bssid, param->macAddress, (const u16 *)MPKEY_ADRS);
    frame->unk_24 = req->unk_12;
    if (req->length) {
        if (i == 0) {
            WUpdateCounter();
        }
        DMA_Write((u8 *)frame + 0x26, req->data, req->length);
    }
    if (flags & 4) {
        {
            u32 length = req->length + 2;
            u8 *payload = (u8 *)&frame->unk_24;
            tail = (u8 *)(((u32)(payload + length) + 3) & ~3u);
        }
        ((u16 *)tail)[0] = 0xB6B8;
        ((u16 *)tail)[1] = 0x1D46;
    }
    irq = __VENEER_OS_DisableIrqMask(0x1000000);
    *(u16 *)((u8 *)slots + offset) = 2;
    q += 0x52; // &txq[4].unk_02
    (*(u16 *)(q + offset))++;
    WL_REG(0x094) = (((u32)frame & 0x3FFF) >> 1) | 0x8000;
    {
        u32 nullKeyMode = param->flags.bits.nullKeyMode;
        if (!nullKeyMode) {
            WSetKSID(nullKeyMode);
        }
    }
    __VENEER_OS_EnableIrqMask(irq);
    return 0;
}

#pragma dont_inline reset
