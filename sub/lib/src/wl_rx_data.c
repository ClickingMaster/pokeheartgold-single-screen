#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

#pragma opt_common_subs off

// Task 6: hands the next received data frame (WlHeap.bufMan[5]) to the WMSP as indication 0x180, or drops it
// (wrong state, unknown / unassociated sender: answered with a disassociation / deauthentication, duplicate).
void RxDataFrameTask(void) {
    WlWork *base = wlWorkPlain;
    WlRxBuf *buf;
    WlBss *bss;
    WlHeap *heap = &base->heap;
    buf = (WlRxBuf *)heap->bufMan[5].first;
    bss = &base->bss;
    u32 index;
    u32 drop;
    u32 mode;
    if ((u32)buf == 0xFFFFFFFFu) {
        return;
    }
    if (bss->state != 0x40) {
        ReleaseHeapBuf(&heap->bufMan[5], &buf->header);
        return;
    }
    u16 dest;
    switch (((WlFrameCtrlBits *)&buf->frame.frameCtrl)->toDs) {
    default:
        dest = buf->frame.addr3[0];
        if (dest & 1) {
            base->counter[11]++;
        } else {
            base->counter[10]++;
        }
        break;
    case 0:
        dest = buf->frame.addr1[0];
        if (dest & 1) {
            base->counter[11]++;
        } else {
            base->counter[10]++;
        }
        break;
    }
    drop = 1;
    base->counter[9] += (buf->frame.status & 0xF0) / 16 - 1;
    mode = bss->mode;
    switch (mode) {
    case 1:
        if (buf->frame.frameCtrl & 1) {
            break;
        }
        index = CAM_Search(buf->frame.addr2);
        if (index == 255 || CAM_GetStaState(index) != 0x40) {
            WlTxHeapFrame *reply;
            if (CAM_GetStaState(index) == 0x30) {
                if (IsExistManFrame(buf->frame.addr2, 0xA0)) {
                    break;
                }
                reply = MakeDisAssFrame(buf->frame.addr2, 7);
            } else {
                if (IsExistManFrame(buf->frame.addr2, 0xC0)) {
                    break;
                }
                reply = MakeDeAuthFrame(buf->frame.addr2, 7, 1);
            }
            if (reply) {
                TxManCtrlFrame(reply);
            }
            break;
        }
        CAM_SetPowerMgtMode(index, ((WlFrameCtrlBits *)&buf->frame.frameCtrl)->pwrMgt);
        if (buf->frame.seqCtrl == CAM_GetLastSeqCtrl(index)) {
            base->counter[15]++;
            break;
        }
        WSetMacAdrs1(buf->frame.addr1, buf->frame.addr3);
        drop = 0;
        break;
    case 2:
    case 3: {
        u16 frameCtrl = buf->frame.frameCtrl;
        if (frameCtrl & 1) {
            break;
        }
        if (bss->powerMgtMode && !(frameCtrl & 0x2000)) {
            if (buf->frame.addr1[0] & 1) {
                bss->parentBuffered &= ~1;
            } else if (mode != 3) {
                bss->parentBuffered &= ~2;
            }
            if (!bss->parentBuffered && !heap->bufMan[1].count && !heap->bufMan[2].count) {
                WSetPowerState(1);
            }
        }
        index = bss->parentStation;
        if (buf->frame.seqCtrl == CAM_GetLastSeqCtrl(index)) {
            base->counter[15]++;
            break;
        }
        WSetMacAdrs1(buf->frame.addr2, buf->frame.addr3);
        drop = 0;
        break;
    }
    }
    if (!drop) {
        u16 station = (u16)index;
        buf->frame.station = station;
        CAM_SetRSSI(station, buf->frame.rssi & 255);
        CAM_SetLastSeqCtrl(index, buf->frame.seqCtrl);
        CAM_UpdateLifeTime(index);
        buf->frame.length = buf->frame.frameLength - 24;
        buf->command = 0x180;
        buf->length = (buf->frame.length + 45u) >> 1;
        SendMessageToWmDirect(&heap->bufMan[5], buf);
    } else {
        ReleaseHeapBuf(&heap->bufMan[5], &buf->header);
    }
    if (heap->bufMan[5].count) {
        AddTask(2, 6);
    }
}

#pragma opt_common_subs reset

// MP frame from the parent: indication 0x182 to the WMSP (with the key data state and the time the MP
// sequence takes). Returns 0 when the frame was handed on, 1 to drop it.
int RxMpFrame(WlRxFrame *frame) {
    WlWork *w = wlWorkPlain;
    WlBss *bss = &w->bss;
    WlRxCtrl *rx = &w->rxCtrl;
    WlRxBuf *msg;
    u16 ctrl, bits;
    u32 n;
    if (bss->state != 0x40) {
        return 1;
    }
    if (!MatchMacAdrs(frame->addr2, bss->bssid) || !MatchMacAdrs(frame->addr3, bss->parentMacAdrs)) {
        return 1;
    }
    // body: interval (+0x2C), bitmap of the stations asked (+0x2E)
    if ((*(u16 *)((u8 *)frame + 0x2E) & (u16)(1 << bss->aid)) == 0) {
        rx->unk_06 = 0;
    } else {
        rx->unk_06 = 0x2000;
    }
    ctrl = WL_REG(0x098);
    if (ctrl & 0x8000) {
        if (*(u16 *)(0x04804000 + (u16)(ctrl << 1) + 4) != 0) {
            rx->unk_06 |= 0x4000;
        }
    }
    CAM_UpdateLifeTime(bss->parentStation);
    frame->length = frame->frameLength - 0x1C;
    msg = (WlRxBuf *)((u8 *)frame - 0x10);
    msg->command = 0x182;
    msg->length = (u32)(frame->length + 0x31) >> 1;
    n = 0;
    bits = *(u16 *)((u8 *)msg + 0x3E);
    while (bits) {
        if (bits & 1) {
            n++;
        }
        bits >>= 1;
    }
    msg->frame.status |= rx->unk_06 | ((int)(WL_REG(0x098) & 0x8000) >> 4) | ((int)(WL_REG(0x094) & 0x8000) >> 3);
    msg->frame.unk_0a = msg->frame.unk_0c + ((n * (*(u16 *)((u8 *)msg + 0x3C) + 10) + 0xFC) >> 4);
    SendMessageToWmDirect(&wlWorkPlain->heap.bufMan[0], msg);
    return 0;
}

// Key data frame from a child during the parent's MP sequence: copies it into the child's entry of the MP
// state (unknown / unassociated senders get a deauthentication).
void RxKeyDataFrame(WlRxFrame *frame) {
    WlMpState *mp;
    WlWork *w = wlWorkPlain;
    WlTxCtrl *txCtrl = &w->txCtrl;
    u32 aid;
    u16 bit, m;
    u8 *dst;
    mp = txCtrl->mpState;
    if (txCtrl->txq[3].active == 0) {
        return;
    }
    if (!MatchMacAdrs(frame->addr1, w->bss.bssid)) {
        return;
    }
    if (frame->frameLength - 0x18 > mp->entrySize - 8) {
        return;
    }
    aid = CAM_Search(frame->addr2);
    if (aid == 0xFF || (aid != 0 && CAM_GetStaState(aid) != 0x40)) {
        WlTxHeapFrame *deauth;
        if (IsExistManFrame(frame->addr2, 0xC0)) {
            return;
        }
        deauth = MakeDeAuthFrame(frame->addr2, 7, 0);
        if (deauth) {
            deauth->unk_00 = 2;
            TxManCtrlFrame(deauth);
        }
        return;
    }
    if (aid == 0) {
        return;
    }
    CAM_SetPowerMgtMode(aid, ((WlFrameCtrlBits *)&frame->frameCtrl)->pwrMgt);
    CAM_UpdateLifeTime(aid);
    bit = (u16)(1 << CAM_GetAID(aid));
    if (bit & txCtrl->unk_9a) {
        return;
    }
    if (!(bit & txCtrl->unk_98)) {
        return;
    }
    txCtrl->unk_9a |= bit;
    mp->unk_10 &= ~bit;
    dst = mp->entries;
    for (m = bit >> 1; m != 1; m >>= 1) {
        if (m & txCtrl->unk_98) {
            dst += mp->entrySize;
        }
    }
    ((WlMpEntry *)dst)->unk_00 = frame->frameLength - 0x18;
    WL_WriteByte(dst + 3, frame->rssi & 0xFF);
    WL_WriteByte(dst + 2, frame->unk_0e & 0xFF);
    if (((WlMpEntry *)dst)->unk_00) {
        __VENEER_MIi_CpuCopy16((u8 *)frame + 0x2C, dst + 8, ((WlMpEntry *)dst)->unk_00 + 1);
    }
}

// MP ACK frame from the parent: indication 0x185 to the WMSP.
int RxMpAckFrame(WlRxFrame *frame) {
    WlWork *w = wlWorkPlain;
    WlBss *bss = &w->bss;
    WlRxBuf *msg;
    if (w->bss.state != 0x40) {
        return 1;
    }
    if (!MatchMacAdrs(frame->addr2, bss->bssid) || !MatchMacAdrs(frame->addr3, bss->parentMacAdrs)) {
        return 1;
    }
    frame->length = frame->frameLength - 0x1C;
    msg = (WlRxBuf *)((u8 *)frame - 0x10);
    msg->command = 0x185;
    msg->length = 0x18;
    msg->frame.status |= wlWorkPlain->rxCtrl.unk_06 | ((int)(WL_REG(0x098) & 0x8000) >> 4) | ((int)(WL_REG(0x094) & 0x8000) >> 3);
    SendMessageToWmDirect(&wlWorkPlain->heap.bufMan[0], msg);
    return 0;
}

#pragma dont_inline reset
