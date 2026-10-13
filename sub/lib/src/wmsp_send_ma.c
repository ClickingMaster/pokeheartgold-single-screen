#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Low half-word of the wireless TSF timer.
#define REG_WL_TSF0 (*(vu16 *)0x048080F8)

// Frame layout built by both senders: the entries of the send queues, highest priority first, at most
// one per port. Each entry is preceded by a header word (bits 0-7: size in half-words, bits 8-11: port,
// bit 12: destination bitmap follows), the first header goes into the WL request instead of the frame.
// Port bit 3 adds the entry's sequence number after the data.

// Child: sends the queued data to the parent as key data (MA key data request).
void WMSP_SendMaKeyData(void) {
    WmspSendEntry *e;
    u32 pri;
    u16 *head;
    u32 dest;
    int len;
    u8 *output;
    WmspStatus *s;
    u32 prev;
    int left;
    u16 *prevNext;
    WmspSendQueue *sent;
    WmspSendQueue *ready;
    WmspSendEntry *entries;
    int skip;
    u32 index;
    int error;
    int first;
    u32 ports;
    u32 port;
    WmspStatus *status;
    int seq;
    int extra;
    int needed;
    u32 mask;
    u16 header;
    u32 buf[128];

    status = wmspW.status;
    if (status->childBitmap == 0) {
        return;
    }
    status->unk_8a = 1;
    error = 1;
    header = 0;
    s = wmspW.status;
    left = s->sendSize;
    head = &header;
    output = s->sendBuf;
    if (s->sendBufSize < ((left + 31) & ~31)) {
        goto finish;
    }
    if (s->aid >= 16) {
        goto finish;
    }
    if (s->state == 9) {
    } else if (s->state == 10) {
        error = 0;
    } else {
        error = 1;
        goto finish;
    }
    *head = 0;
    len = 0;
    if (left < 0) {
        error = 1;
        goto finish;
    }
    mask = 1;
    skip = 1;
    if (error) {
        if (s->currentMinPollBmpMode) {
            skip = 0;
            mask = 1;
        }
    } else {
        skip = 0;
    }
    ports = 0;
    first = 1;
    __VENEER_OS_LockMutex(&s->sendQueueMutex);
    if (s->sendInFlight == 1) {
        __VENEER_OS_UnlockMutex(&s->sendQueueMutex);
        error = 1;
        goto finish;
    }
    s->sendInFlight = 1;
    entries = s->sendEntry;
    for (pri = 0; pri < 4 && left > 2; pri++) {
        ready = &s->sendQueue[pri];
        sent = &s->sentQueue[pri];
        index = ready->head;
        prevNext = &ready->head;
        prev = 0xffff;
        while (index != 0xffff && left > 2) {
            e = &entries[index];
            port = e->port;
            if (ports & (1 << port)) {
                goto next;
            }
            ports |= 1 << port;
            dest = e->restBitmap & mask;
            if (skip && (dest & ~1)) {
                goto next;
            }
            seq = (port & 8) ? 1 : 0;
            extra = (error && (e->restBitmap | 1) != 0xffff) ? 1 : 0;
            if ((e->size & 1) == 1) {
                e->size++;
            }
            needed = e->size + (first ? 0 : 2) + (seq ? 2 : 0) + (extra ? 2 : 0);
            if (needed > left) {
                goto next;
            }
            if (!first) {
                head = (u16 *)output;
                *(u16 *)output = 0;
                output += 2;
                len += 2;
                left -= 2;
            }
            *head |= ((e->port << 8) & 0xf00) | (((u32)e->size >> 1) & 0xff);
            __VENEER_MIi_CpuCopy16(e->data, output, e->size);
            len += e->size;
            output += e->size & ~1;
            left -= e->size;
            if (seq == 1) {
                int seqNo = e->seqNo;
                if (!(seqNo & 1)) {
                    seqNo >>= 1;
                } else {
                    seqNo = s->portSeqNo[s->aid][e->port & 7];
                    s->portSeqNo[s->aid][e->port & 7] = seqNo + 1;
                    e->seqNo = seqNo << 1;
                }
                len += 2;
                *(u16 *)output = seqNo & 0x7fff;
                output += 2;
                left -= 2;
            }
            if (extra == 1) {
                len += 2;
                left -= 2;
                *head |= 0x1000;
                *(u16 *)output = e->restBitmap;
                output += 2;
            }
            if (first == 1) {
                first = 0;
            }
            // move the entry from the send queue to the sent queue
            if (ready->tail == index) {
                ready->tail = prev;
            }
            *prevNext = e->next;
            e->next = 0xffff;
            if (sent->tail == 0xffff) {
                sent->head = index;
            } else {
                entries[sent->tail].next = index;
            }
            ports &= ~(1 << port);
            sent->tail = index;
            e->inFlightBitmap = dest;
            index = prev;
            if (s->currentSinglePacketMode == 1) {
                goto unlock;
            }
next:
            prev = index;
            if (index != 0xffff) {
                prevNext = &entries[index].next;
            } else {
                prevNext = &ready->head;
            }
            index = (index != 0xffff) ? entries[index].next : ready->head;
        }
    }
unlock:
    __VENEER_OS_UnlockMutex(&s->sendQueueMutex);
    error = 0;
finish:
    if (error == 1) {
        status->unk_8a = 0;
        return;
    }
    if (status->vsyncFlag == 1) {
        header |= 0x8000;
    }
    {
        u16 *result = WMSP_WL_MaKeyData(buf, (u16)len, header, status->sendBuf);
        if (result[2] != 0 && result[2] != 8) {
            status->unk_8a = 0;
        }
    }
}

// Returns the entries of the sent queues: destinations outside `failedBitmap` count as reached.
// Entries with destinations left go back to the front of their send queue, the others are reported
// to the ARM9 and freed. Same as WMSP_FlushSendQueue(FALSE, failedBitmap) without its resend limit.
static inline void WmspFlushSendQueue(u32 failedBitmap) {
    u32 keep;
    u16 head;
    WmspStatus *s;
    WmspSendEntry *entries;
    int pri;
    int parent;
    u16 tail;
    u32 connected;

    s = wmspW.status;
    entries = s->sendEntry;
    if (s->state == 9) {
        parent = 1;
    } else if (s->state == 10) {
        parent = 0;
    } else {
        return;
    }
    __VENEER_OS_LockMutex(&s->sendQueueMutex);
    if (!s->sendInFlight) {
        __VENEER_OS_UnlockMutex(&s->sendQueueMutex);
        return;
    }
    if (!parent) {
        connected = 1;
    } else {
        OSIntrMode irq = __VENEER_OS_DisableInterrupts();
        connected = s->childBitmap;
        __VENEER_OS_RestoreInterrupts(irq);
    }
    keep = ~(u16)failedBitmap;
    for (pri = 0; pri < 4; pri++) {
        WmspSendQueue *queue = &s->sentQueue[pri];
        u32 index = queue->head;
        tail = 0xffff;
        head = 0xffff;
        while (index != 0xffff) {
            WmspSendEntry *e = &entries[index];
            e->sentBitmap |= e->inFlightBitmap & keep;
            e->restBitmap &= ~e->sentBitmap;
            e->restBitmap &= connected;
            e->inFlightBitmap = 0;
            if (e->restBitmap && ((e->port & 8) || e->retryCount)) {
                if (e->retryCount) {
                    e->retryCount--;
                }
                if (e->next == 0xffff) {
                    queue->tail = 0xffff;
                }
                queue->head = e->next;
                e->next = 0xffff;
                if (tail == 0xffff) {
                    head = index;
                } else {
                    entries[tail].next = index;
                }
                tail = index;
            } else {
                WmspPortSendCallback *cb = (WmspPortSendCallback *)__VENEER_WMSP_GetBuffer4Callback2Wm9();
                cb->command = 0x81;
                cb->result = e->restBitmap == 0 ? 0 : 15;
                cb->api = 0x14;
                cb->port = e->port;
                cb->destBitmap = e->destBitmap;
                cb->failedBitmap = e->restBitmap;
                cb->sentBitmap = e->sentBitmap;
                cb->size = e->size;
                cb->data = e->data;
                cb->callback = e->callback;
                cb->arg = e->arg;
                cb->seqNo = e->seqNo;
                {
                    u16 parentSize = s->parentSize, childSize = s->childSize;
                    cb->sendSize = s->aid == 0 ? parentSize : childSize;
                    cb->recvSize = s->aid == 0 ? childSize : parentSize;
                }
                __VENEER_WMSP_ReturnResult2Wm9(cb);
                if (e->next == 0xffff) {
                    queue->tail = 0xffff;
                }
                queue->head = e->next;
                e->next = 0xffff;
                if (s->sendFreeQueue.tail == 0xffff) {
                    s->sendFreeQueue.head = index;
                } else {
                    entries[s->sendFreeQueue.tail].next = index;
                }
                s->sendFreeQueue.tail = index;
            }
            index = queue->head;
        }
        if (tail != 0xffff) {
            entries[tail].next = s->sendQueue[pri].head;
            if (s->sendQueue[pri].tail == 0xffff) {
                s->sendQueue[pri].tail = tail;
            }
            s->sendQueue[pri].head = head;
        }
    }
    s->sendInFlight = 0;
    __VENEER_OS_UnlockMutex(&s->sendQueueMutex);
}

// Sends the next MP frame to the children in `bitmap` (0xFFFF: all), or an empty one while unk_9e is
// set. When the receive buffer is too small for the replies, the frame is dropped and its entries are
// returned as failed.
void WMSP_SendMaMP(u16 bitmap) {
    u32 size;
    u32 time;
    u16 tmptt;
    int error;
    u16 recvSize;
    u32 children;
    u32 connected;
    u16 *head;
    int extra;
    int seq;
    u16 port;
    WmspSendEntry *e;
    u32 destBitmap;
    u32 index;
    u32 dest;
    u32 prev;
    u16 *prevNext;
    WmspSendQueue *sent;
    WmspSendQueue *ready;
    u32 pri;
    WmspSendEntry *entries;
    u32 mask;
    int skip;
    int parent;
    int first;
    u32 ports;
    int len;
    u8 *output;
    WmspStatus *status;
    WmspStatus *s;
    int left;
    int needed;
    u16 header;
    u32 buf[128];

    status = wmspW.status;
    {
        OSIntrMode irq = __VENEER_OS_DisableInterrupts();
        connected = status->childBitmap;
        children = status->readyBitmap;
        __VENEER_OS_RestoreInterrupts(irq);
    }
    header = 0;
    if (status->unk_88 == 1) {
        bitmap = 0xffff;
        status->unk_88 = 0;
    }
    if (status->unk_9e == 0) {
        recvSize = status->recvSize;
        s = wmspW.status;
        left = s->sendSize;
        head = &header;
        output = s->sendBuf;
        if (s->sendBufSize < ((left + 31) & ~31)) {
            error = 1;
            goto finish;
        }
        if (s->aid >= 16) {
            error = 1;
            goto finish;
        }
        if (s->state == 9) {
            parent = 1;
        } else if (s->state == 10) {
            parent = 0;
        } else {
            error = 1;
            goto finish;
        }
        *head = 0;
        destBitmap = 0;
        size = 0;
        len = 0;
        if (left < 0) {
            error = 1;
            goto finish;
        }
        mask = children;
        skip = 1;
        if (parent) {
            if (s->currentMinPollBmpMode) {
                skip = 0;
                mask = connected;
            }
        } else {
            skip = 0;
        }
        children &= bitmap;
        ports = 0;
        first = 1;
        __VENEER_OS_LockMutex(&s->sendQueueMutex);
        if (s->sendInFlight == 1) {
            __VENEER_OS_UnlockMutex(&s->sendQueueMutex);
            error = 1;
            goto finish;
        }
        s->sendInFlight = 1;
        entries = s->sendEntry;
        for (pri = 0; pri < 4 && left > 2; pri++) {
            ready = &s->sendQueue[pri];
            sent = &s->sentQueue[pri];
            index = ready->head;
            prevNext = &ready->head;
            prev = 0xffff;
            while (index != 0xffff && left > 2) {
                e = &entries[index];
                port = e->port;
                if (ports & (1 << port)) {
                    goto next;
                }
                ports |= 1 << port;
                dest = e->restBitmap & mask;
                if (skip && (dest & ~children)) {
                    goto next;
                }
                seq = (port & 8) ? 1 : 0;
                extra = (parent && (e->restBitmap | 1) != 0xffff) ? 1 : 0;
                if ((e->size & 1) == 1) {
                    e->size++;
                }
                needed = e->size + (first ? 0 : 2) + (seq ? 2 : 0) + (extra ? 2 : 0);
                if (needed > left) {
                    goto next;
                }
                if (!first) {
                    head = (u16 *)output;
                    *(u16 *)output = 0;
                    output += 2;
                    len += 2;
                    left -= 2;
                }
                *head |= ((e->port << 8) & 0xf00) | (((u32)e->size >> 1) & 0xff);
                __VENEER_MIi_CpuCopy16(e->data, output, e->size);
                len += e->size;
                output += e->size & ~1;
                left -= e->size;
                if (seq == 1) {
                    int seqNo = e->seqNo;
                    if (!(seqNo & 1)) {
                        seqNo >>= 1;
                    } else {
                        seqNo = s->portSeqNo[s->aid][e->port & 7];
                        s->portSeqNo[s->aid][e->port & 7] = seqNo + 1;
                        e->seqNo = seqNo << 1;
                    }
                    len += 2;
                    *(u16 *)output = seqNo & 0x7fff;
                    output += 2;
                    left -= 2;
                }
                if (extra == 1) {
                    len += 2;
                    left -= 2;
                    *head |= 0x1000;
                    *(u16 *)output = e->restBitmap;
                    output += 2;
                }
                if (first == 1) {
                    first = 0;
                }
                // move the entry from the send queue to the sent queue
                if (ready->tail == index) {
                    ready->tail = prev;
                }
                *prevNext = e->next;
                e->next = 0xffff;
                if (sent->tail == 0xffff) {
                    sent->head = index;
                } else {
                    entries[sent->tail].next = index;
                }
                ports &= ~(1 << port);
                sent->tail = index;
                e->inFlightBitmap = dest;
                destBitmap |= e->destBitmap;
                index = prev;
                if (s->currentSinglePacketMode == 1) {
                    goto unlock;
                }
next:
                prev = index;
                if (index != 0xffff) {
                    prevNext = &entries[index].next;
                } else {
                    prevNext = &ready->head;
                }
                index = (index != 0xffff) ? entries[index].next : ready->head;
            }
        }
unlock:
        size = len;
        __VENEER_OS_UnlockMutex(&s->sendQueueMutex);
        error = 0;
finish:
        if (error == 1) {
            status->mpCount = 0;
            status->mpLimitCount = 0;
            return;
        }
        time = recvSize + 2;
        if (status->currentMinPollBmpMode != 1) {
            destBitmap = bitmap;
        }
        destBitmap &= connected;
        if ((s32)status->mpBufSize < (s32)(((recvSize + 12) * __VENEER_MATH_CountPopulation(destBitmap) + 0x29) & ~31U)) {
            WmspFlushSendQueue(destBitmap);
            status->mpCount = 0;
            status->mpLimitCount = 0;
            return;
        }
        if (status->mpCount != 1 && status->mpLimitCount != 1) {
            tmptt = 0;
        } else {
            tmptt = WmspGetTmptt(size, time, destBitmap, (u16)status->parentVAlarmLine);
            header |= 0x8000;
        }
    } else {
        tmptt = 0;
        status->unk_9e = 0;
        header &= ~0x8000;
        destBitmap = connected;
        size = 0;
        time = 0x80d6;
        {
            OSIntrMode irq = __VENEER_OS_DisableInterrupts();
            status->mpCount++;
            status->mpLimitCount++;
            __VENEER_OS_RestoreInterrupts(irq);
        }
    }
    WMSP_WL_MaMp(buf, 0, 0, (u16)time, (u16)destBitmap, tmptt, REG_WL_TSF0, (u16)size, header, (u32)status->sendBuf);
    status->prevPollBitmap = destBitmap;
    status->prevWmHeader = header;
    status->prevTxop = time;
    status->prevDataLength = size;
}
