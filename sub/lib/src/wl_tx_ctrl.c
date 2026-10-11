#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// TX bits of the queues in register 0x0B4 (stop) by priority.
extern const u16 Pri2QBit[];

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Copies a queued TX frame (header and body; the body is inline or behind a pointer) into the MAC memory.
// WEP frames get their IV, a zero ICV (hwWorkarounds bit 3); hwWorkarounds bit 2 appends a fixed tail.
void CopyTxFrmToMacBuf(void *dest, void *source) {
    WlMaDataReq *src = source;
    u8 *d = dest, *tail;
    WlWork *w;
    u32 y, x;
    WlWork **root = (WlWork **)WL_WORK_ADDR;
    if (src->frame.frame.frameCtrl & 0x4000) {
        if (wlWorkPlain->bss.mode == 3) {
            WUpdateCounter();
        }
        if (src->header.command == 0xFFFF) {
            DMA_WepWriteHeaderData(d, &src->frame.frame, &src->frame.frame.unk_24, src->frame.bodyLength);
        } else {
            DMA_WepWriteHeaderData(d, &src->frame.frame, *(u8 **)&src->frame.frame.unk_24, src->frame.bodyLength);
        }
        x = WL_REG(0x044);
        y = WL_REG(0x044);
        ((WlTxFrame *)d)->unk_24 = (u16)(x + (y << 8));
        w = *root;
        x = WL_REG(0x044) & 255;
        ((WlTxFrame *)d)->unk_26 = (w->param.wepKeyId << 14) | x;
        if (wlWorkPlain->hwWorkarounds & 8) {
            tail = (u8 *)(((u32)(d + 12 + src->frame.frame.length) - 7) & ~1u);
            ((u16 *)tail)[0] = 0;
            ((u16 *)tail)[1] = 0;
        }
    } else {
        if (src->header.command == 0xFFFF) {
            DMA_Write(d, &src->frame.frame, src->frame.bodyLength + 0x24);
        } else {
            DMA_WriteHeaderData(d, &src->frame.frame, *(u8 **)&src->frame.frame.unk_24, src->frame.bodyLength);
        }
    }
    if (wlWorkPlain->hwWorkarounds & 4) {
        tail = (u8 *)(((u32)(d + 12 + src->frame.frame.length) - 1) & ~3u);
        ((u16 *)tail)[0] = 0xB6B8;
        ((u16 *)tail)[1] = 0x1D46;
    }
}

// The frame life time (WlBss.unk_8c, 1/8 for data frames and the parent's responses) has run out.
int CheckFrameTimeout(WlTxHeapFrame *frame) {
    WlBss *bss = &wlWork->bss;
    WlFrameCtrlBits *frameCtrl = (WlFrameCtrlBits *)&frame->frame.frameCtrl;
    u16 timeout = (u16)(bss->unk_8c << 3);
    if (frameCtrl->type == 0) {
        if (bss->mode == 1) {
            u32 subtype = frameCtrl->subtype;
            if (subtype == 1 || subtype == 3 || subtype == 11) {
                timeout >>= 3;
            }
        }
    } else {
        timeout >>= 3;
    }
    return timeout < (u16)(bss->intervalCount - frame->time);
}

// TX end callback of the data queue (txq[0]): statistics, confirm, power save of the station / own.
void TxqEndData(void *arg, u32 next) {
    WlTxHeapFrame *frame = arg;
    WlWork *w = wlWork;
    WlHeapBufMan *queue = &w->heap.bufMan[1];
    WlBss *bss = &w->bss;
    WlHeapBuf *buf = (WlHeapBuf *)((u8 *)frame - 16);
    CAM_DecFrameCount(frame);
    if (!(VOLATILE_U16(frame->frame.status) & 2)) {
        w->counter[0]++;
        if (((volatile WlFrameCtrlBits *)&frame->frame.frameCtrl)->toDs) {
            if (VOLATILE_U16(frame->frame.addr3[0]) & 1) {
                w->counter[5]++;
            } else {
                w->counter[4]++;
            }
        } else {
            if (VOLATILE_U16(frame->frame.addr1[0]) & 1) {
                w->counter[5]++;
            } else {
                w->counter[4]++;
            }
        }
    } else {
        w->counter[1]++;
    }
    if (((volatile WlFrameCtrlBits *)&frame->frame.frameCtrl)->wep) {
        w->counter[6]++;
    }
    IssueMaDataConfirm(queue, (WlMaDataReq *)buf);
    VOLATILE_U16(wlWork->txCtrl.txq[0].active) = 0;
    if (CAM_GetPowerMgtMode(VOLATILE_U16(frame->station)) != 0 && !(VOLATILE_U16(frame->frame.frameCtrl) & 0x2000)) {
        CAM_SetDoze(VOLATILE_U16(frame->station));
    }
    if (next) {
        if (queue->count) {
            TxqPri(0);
        } else if ((u16)(VOLATILE_U16(bss->mode) + 0xFFFE) <= 1 && VOLATILE_U16(bss->state) == 0x40 && VOLATILE_U16(bss->powerMgtMode) && CAM_GetFrameCount(VOLATILE_U16(bss->parentStation)) == 0 && VOLATILE_U16(bss->parentBuffered) == 0) {
            WSetPowerState(1);
        }
    }
}

// TX end callback of the management queue (txq[1]): finishes authentication, (re)association,
// disassociation and deauthentication on the station table and reports them to the WMSP.
void TxqEndManCtrl(void *arg, u32 schedule) {
    WlTxHeapFrame *frame = arg;
    WlWork *w = wlWorkPlain;
    WlBss *bss = &w->bss;
    WlMlme *mlme = &w->mlme;
    WlHeapBufMan *queue = &w->heap.bufMan[2];
    u32 index = frame->station, kind;
    if (!(frame->frame.status & 2)) {
        w->counter[0]++;
        if (frame->frame.addr1[0] & 1) {
            w->counter[5]++;
        } else {
            w->counter[4]++;
        }
        if (CAM_GetPowerMgtMode(index) && !(frame->frame.frameCtrl & 0x2000)) {
            CAM_SetDoze(index);
        }
    } else {
        w->counter[1]++;
    }
    if (((WlFrameCtrlBits *)&frame->frame.frameCtrl)->wep) {
        w->counter[6]++;
    }
    w->counter[2] += frame->frame.retry & 255;
    kind = frame->frame.frameCtrl & 0xFC;
    switch (kind) {
    case 0xB0: // authentication: body = algorithm, transaction sequence number, status code
        if (index && !(frame->frame.status & 2)) {
            if (frame->frame.unk_24 == 0 && frame->frame.unk_26 == 2 && frame->frame.unk_28 == 0) {
                CAM_SetStaState((u16)index, 0x30);
                MLME_IssueAuthIndication(frame->frame.addr1, frame->frame.unk_24);
            } else if (frame->frame.unk_24 == 1 && frame->frame.unk_26 == 4 && frame->frame.unk_28 == 0) {
                CAM_SetStaState((u16)index, 0x30);
                MLME_IssueAuthIndication(frame->frame.addr1, frame->frame.unk_24);
            }
        }
        break;
    case 0x10: // association response: body = capability, status code, AID
    case 0x30: // reassociation response
        if (!index) {
            break;
        }
        if (!(frame->frame.status & 2)) {
            if (frame->frame.unk_26 == 0 && CAM_GetStaState(index) == 0x30) {
                CAM_SetStaState((u16)index, 0x40);
                if (kind == 0x10) {
                    MLME_IssueAssIndication(frame->frame.addr1, frame->frame.unk_28, (u8 *)&frame->frame.frameCtrl + frame->frame.length);
                } else {
                    MLME_IssueReAssIndication(frame->frame.addr1, frame->frame.unk_28, (u8 *)&frame->frame.frameCtrl + frame->frame.length);
                }
            }
        } else {
            WlTxHeapFrame *deauth;
            CAM_ReleaseAID((u16)index);
            deauth = MakeDeAuthFrame(frame->frame.addr1, 1, 0);
            if (deauth) {
                deauth->unk_00 = 2;
                if (schedule) {
                    SetManCtrlFrame(deauth);
                    TxqPri(1);
                } else {
                    SetManCtrlFrame(deauth);
                }
            }
        }
        break;
    case 0xA0: // disassociation
        if (bss->mode == 1) {
            if (index) {
                if (CAM_GetStaState(index) > 0x30) {
                    CAM_SetStaState((u16)index, 0x30);
                }
            } else if (frame->frame.addr1[0] & 1) {
                u32 station;
                for (station = 1; station < wlWorkPlain->param.maxSta; station++) {
                    if (CAM_GetStaState(station) > 0x30) {
                        CAM_SetStaState((u16)station, 0x30);
                    }
                }
            }
        } else if (bss->state > 0x30) {
            WSetStaState(0x30);
            WClearAids();
        }
        if (mlme->state == 0x71 && (u8 *)frame == mlme->work.frame) {
            if (!(frame->frame.status & 2)) {
                mlme->cfm.header->result = 0;
            } else {
                mlme->cfm.header->result = 12;
            }
            mlme->state = 0;
            IssueMlmeConfirm();
        }
        break;
    case 0xC0: // deauthentication
        if (bss->mode == 1) {
            if (index) {
                if (CAM_GetStaState(index) > 0x20) {
                    CAM_SetStaState((u16)index, 0x20);
                }
            } else if (frame->frame.addr1[0] & 1) {
                u32 station;
                for (station = 1; station < wlWorkPlain->param.maxSta; station++) {
                    if (CAM_GetStaState(station) > 0x20) {
                        CAM_SetStaState((u16)station, 0x20);
                    }
                }
            }
        } else if (bss->state > 0x20) {
            WSetStaState(0x20);
            WClearAids();
        }
        if (mlme->state == 0x41 && (u8 *)frame == mlme->work.frame) {
            if (!(frame->frame.status & 2)) {
                mlme->cfm.header->result = 0;
            } else {
                mlme->cfm.header->result = 12;
            }
            mlme->state = 0;
            IssueMlmeConfirm();
        }
        if (frame->unk_00 == 1) {
            if (index) {
                WlWork *current = wlWorkPlain;
                u32 bit = 1U << index;
                current->cam.unk_08 &= ~bit;
                CAM_Delete((u16)index);
            }
            MLME_IssueDeAuthIndication(frame->frame.addr1, 1);
        } else if (frame->unk_00 == 2) {
            MLME_IssueDeAuthIndication(frame->frame.addr1, frame->frame.unk_24);
        }
        break;
    }
    CAM_DecFrameCount(frame);
    ReleaseHeapBuf(queue, (WlHeapBuf *)((u8 *)frame - 0x10));
    wlWorkPlain->txCtrl.txq[1].active = 0;
    if (schedule && queue->count) {
        TxqPri(1);
    }
}

// TX end callback of the PS-Poll slot (txq[2] in child modes); `arg` is the frame in the MAC memory.
void TxqEndPsPoll(void *arg, u32 next) {
    WlTxFrame *frame = arg;
    WlWork *w = wlWork;
    w->counter[2] += frame->retry & 255;
    if (frame->status & 2) {
        w->counter[1]++;
    } else {
        w->counter[0]++;
        w->counter[4]++;
    }
    wlWork->txCtrl.txq[2].active = 0;
}

// TX end callback of the broadcast queue (txq[2] in parent mode): management frames go on through
// TxqEndManCtrl; after the last one (no more data) the unicast queues restart.
void TxqEndBroadCast(void *arg, u32 next) {
    WlTxHeapFrame *frame = arg;
    WlWork *w = wlWork;
    WlHeap *heap = &w->heap;
    w->counter[5]++;
    if (((volatile WlFrameCtrlBits *)&frame->frame.frameCtrl)->type == 0) {
        CAM_IncFrameCount(frame);
        MoveHeapBuf(&heap->bufMan[3], &heap->bufMan[2], (WlHeapBuf *)((u8 *)frame - 16));
        TxqEndManCtrl(frame, 0);
    } else {
        IssueMaDataConfirm(&heap->bufMan[3], (WlMaDataReq *)((u8 *)frame - 16));
    }
    VOLATILE_U16(wlWork->txCtrl.txq[2].active) = 0;
    if (!(((volatile WlFrameCtrlBits *)&((WlTxFrame *)wlWork->txCtrl.txq[2].frame)->frameCtrl)->moreData)) {
        WL_REG(0x0AC) = 8;
        WL_REG(0x0AE) = 5;
        if (next) {
            if (heap->bufMan[2].count) {
                TxqPri(1);
            }
            if (heap->bufMan[1].count) {
                TxqPri(0);
            }
        }
    }
    if (heap->bufMan[3].count) {
        if (next) {
            TxqPri(2);
        }
    } else {
        CAM_ClrTIMElementBitmap(0);
    }
}

// Key data frame (txq[4] / txq[5]) sent: counts the retries.
void TxEndKeyData(WlTxDesc *desc) {
    u32 retry = VOLATILE_U16(((WlTxFrame *)desc->frame)->retry) & 255;
    WlWork *w = wlWork;
    if (retry == 0) {
        w->counter[27]++;
    } else {
        w->counter[26] += retry;
    }
    ++VOLATILE_U16(desc->count);
    VOLATILE_U16(desc->active) = 0;
}

void ClearTxKeyData(void) {
    WlTxCtrl *txCtrl = &wlWork->txCtrl;
    u32 irq = __VENEER_OS_DisableIrqMask(0x1000000);
    if (!((volatile WlParamFlags *)&wlWork->param.flags)->bits.nullKeyMode) {
        WClearKSID(0);
    }
    WL_REG(0x0B4) = 0xC0;
    VOLATILE_U16(txCtrl->txq[4].active) = 0;
    VOLATILE_U16(txCtrl->txq[5].active) = 0;
    __VENEER_OS_EnableIrqMask(irq);
}

void ClearTxMp(void) {
    WlTxCtrl *txCtrl = &wlWork->txCtrl;
    u32 irq = __VENEER_OS_DisableIrqMask(0x1000000);
    WL_REG(0x0B4) = 2;
    if (VOLATILE_U16(txCtrl->txq[3].active)) {
        WlIntrMpEndTask();
    }
    __VENEER_OS_EnableIrqMask(irq);
}

void ClearTxData(void) {
    WlTxCtrl *txCtrl = &wlWork->txCtrl;
    u32 irq = __VENEER_OS_DisableIrqMask(0x1000000);
    if (VOLATILE_U16(wlWork->bss.mode) == 1) {
        WL_REG(0x0B4) = 9;
        if (VOLATILE_U16(txCtrl->txq[2].active)) {
            ClearQueuedPri(2);
        }
        MessageDeleteTx(2, 1);
    } else {
        WL_REG(0x0B4) = 1;
    }
    if (VOLATILE_U16(txCtrl->txq[0].active)) {
        ClearQueuedPri(0);
    }
    MessageDeleteTx(0, 1);
    __VENEER_OS_EnableIrqMask(irq);
}

// Completes the frame of queue `pri` that is in the MAC (status 2 when it has none).
void ClearQueuedPri(u32 pri) {
    WlTxDesc *desc = &wlWorkPlain->txCtrl.txq[0] + pri;
    if (desc->active) {
        u16 status = *(u16 *)desc->frame;
        if (status == 0) {
            desc->heapFrame->frame.status = 2;
        } else {
            desc->heapFrame->frame.status = status;
        }
        desc->callback(desc->heapFrame, 0);
    }
}

// Stops the TX of queue `pri` and copies the retry count and sequence number of its frame back.
void ResetTxqPri(u32 pri) {
    WlTxDesc *txq = wlWork->txCtrl.txq, *desc = txq + pri;
    u32 irq = __VENEER_OS_DisableIrqMask(0x1000000);
    WL_REG(0x0B4) = Pri2QBit[pri];
    if (VOLATILE_U16(desc->active)) {
        WlTxHeapFrame *frame = desc->heapFrame;
        if (!(VOLATILE_U16(frame->frame.frameCtrl) & 0x4000)) {
            VOLATILE_U16(frame->frame.retry) = VOLATILE_U16(((WlTxFrame *)desc->frame)->retry);
        }
        VOLATILE_U16(desc->heapFrame->frame.seqCtrl) = VOLATILE_U16(((WlTxFrame *)desc->frame)->seqCtrl);
    }
    __VENEER_OS_EnableIrqMask(irq);
}

// Drops the queued frames of a station; frames in the MAC and management frames are moved to station 0.
void DeleteTxFrames(u32 station) {
    WlHeapBuf *buf = 0;
    WlTxHeapFrame *frame = 0;
    u32 deleted = 0, i;
    if (CAM_GetFrameCount(station)) {
        for (i = 0; i < 3; i++) {
            buf = (&wlWork->heap.bufMan[1])[i].first;
            if (buf != (void *)~0u) {
                do {
                    WlHeapBuf *next = GetHeapBufNextAdrs(buf);
                    frame = (WlTxHeapFrame *)((u8 *)buf + 16);
                    if (VOLATILE_U16(frame->station) == station) {
                        if (i == 1 || frame == wlWork->txCtrl.txq[i].heapFrame) {
                            CAM_DecFrameCount(frame);
                            VOLATILE_U16(frame->station) = 0;
                            CAM_IncFrameCount(frame);
                        } else {
                            VOLATILE_U16(frame->frame.status) = 2;
                            CAM_DecFrameCount(frame);
                            IssueMaDataConfirm(&(&wlWork->heap.bufMan[1])[i], (WlMaDataReq *)buf);
                            if (!deleted) {
                                deleted = 1;
                            }
                        }
                    }
                    buf = next;
                } while (buf != (void *)~0u);
            }
        }
    }
}

// Broadcast address: every station's frames.
void DeleteTxFrameByAdrs(const u16 *macAdrs) {
    WlWork *w = wlWork;
    if (macAdrs[0] & 1) {
        u32 station = 1;
        if (w->param.maxSta > 1) {
            while (station < wlWork->param.maxSta) {
                DeleteTxFrames(station);
                station++;
            }
        }
    } else {
        u32 station = CAM_Search(macAdrs);
        if (station != 255) {
            DeleteTxFrames(station);
        }
        if (wlWork->param.mode == 1 && CAM_GetStaState(station) == 0x40) {
            CAM_SetStaState((u16)station, 0x20);
            ClearTxKeyData();
        }
    }
}

void DeleteAllTxFrames(void) {
    WlWork *w = wlWork;
    WlTxCtrl *txCtrl = &w->txCtrl;
    switch (VOLATILE_U16(w->bss.mode)) {
    case 1:
        MessageDeleteTx(0, 1);
        MessageDeleteTx(1, 0);
        MessageDeleteTx(2, 1);
        if (VOLATILE_U16(txCtrl->txq[3].active)) {
            VOLATILE_U16(txCtrl->txq[3].active) = 0;
            --VOLATILE_U16(txCtrl->txq[3].unk_02);
            ReleaseHeapBuf(&wlWork->heap.bufMan[0], (WlHeapBuf *)txCtrl->mpState);
        }
        break;
    case 2:
    case 3:
        MessageDeleteTx(0, 1);
        MessageDeleteTx(1, 0);
        MessageDeleteTx(2, 0);
        break;
    default:
        MessageDeleteTx(0, 0);
        MessageDeleteTx(1, 0);
        MessageDeleteTx(2, 0);
        break;
    }
}

// Drops every frame of queue `pri` (with a confirm to the WMSP when `confirm` is set).
void MessageDeleteTx(u32 pri, u32 confirm) {
    WlHeapBuf *buf = (&wlWork->heap.bufMan[1])[pri].first;
    if (buf != (void *)~0u) {
        do {
            WlHeapBuf *next = GetHeapBufNextAdrs(buf);
            if (pri != 2) {
                CAM_DecFrameCount((u8 *)buf + 16);
            }
            VOLATILE_U16(((WlTxHeapFrame *)((u8 *)buf + 16))->frame.status) = 2;
            if (confirm) {
                IssueMaDataConfirm(&(&wlWork->heap.bufMan[1])[pri], (WlMaDataReq *)buf);
            }
            buf = next;
        } while (buf != (void *)~0u);
    }
}

#pragma dont_inline reset
