#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#pragma force_active off

// Unlink the head entry `e` of `queue`. Two copies of the same helper: one called twice is not inlined.
static inline void PopResendEntry(WmspSendQueue *queue, WmspSendEntry *e) {
    u32 empty = 0xffff;
    if (e->next == empty) {
        queue->tail = empty;
    }
    queue->head = e->next;
    e->next = empty;
}

static inline void PopDoneEntry(WmspSendQueue *queue, WmspSendEntry *e) {
    u32 empty = 0xffff;
    if (e->next == empty) {
        queue->tail = empty;
    }
    queue->head = e->next;
    e->next = empty;
}

// Appends pool[index] to `queue`.
static inline void AppendQueueEntry(WmspSendQueue *queue, WmspSendEntry *pool, u32 index) {
    u32 empty = 0xffff;
    if (queue->tail == empty) {
        queue->head = index;
    } else {
        pool[queue->tail].next = index;
    }
    queue->tail = index;
}

#pragma force_active on

// Ends the MP frame in flight: unless `retransmit`, its destinations outside `failedBitmap` count as
// reached. Entries with destinations left and resends left (port bit 3: always) go back to the front
// of their send queue, the others are reported to the ARM9 (result 0, or 15 when not all destinations
// were reached) and freed. Returns whether any entry is to be sent again.
int WMSP_FlushSendQueue(int retransmit, u16 inputFailedBitmap) {
    WmspSendEntry *entries;
    u16 failedBitmap = inputFailedBitmap;
    u32 index;
    int parent;
    WmspSendQueue *queue;
    WmspSendEntry *e;
    WmspStatus *s = wmspW.status;
    u16 connected;
    int more = 0;
    int priority;
    u16 head;
    u32 keep;
    entries = s->sendEntry;

    if (s->state == 9) {
        parent = 1;
    } else if (s->state == 10) {
        parent = 0;
    } else {
        return 0;
    }
    __VENEER_OS_LockMutex(&s->sendQueueMutex);
    if (!s->sendInFlight) {
        __VENEER_OS_UnlockMutex(&s->sendQueueMutex);
        return 0;
    }
    if (!parent) {
        connected = 1;
    } else {
        OSIntrMode irq = __VENEER_OS_DisableInterrupts();
        connected = s->childBitmap;
        __VENEER_OS_RestoreInterrupts(irq);
    }
    keep = ~failedBitmap;
    for (priority = 0; priority < 4; priority++) {
        u16 tail;
        queue = &s->sentQueue[priority];
        index = queue->head;
        tail = 0xffff;
        head = 0xffff;
        while (index != 0xffff) {
            e = &entries[index];
            if (!retransmit) {
                e->sentBitmap |= e->inFlightBitmap & keep;
                e->restBitmap &= ~e->sentBitmap;
            }
            e->restBitmap &= connected;
            e->inFlightBitmap = 0;
            if (e->restBitmap && ((e->port & 8) || e->retryCount)) {
                more = 1;
                if (e->retryCount) {
                    e->retryCount--;
                }
                PopResendEntry(queue, e);
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
                PopDoneEntry(queue, e);
                AppendQueueEntry(&s->sendFreeQueue, entries, index);
            }
            index = queue->head;
        }
        // the entries to send again go in front of the waiting ones
        if (tail != 0xffff) {
            entries[tail].next = s->sendQueue[priority].head;
            if (s->sendQueue[priority].tail == 0xffff) {
                s->sendQueue[priority].tail = tail;
            }
            s->sendQueue[priority].head = head;
        }
    }
    s->sendInFlight = 0;
    __VENEER_OS_UnlockMutex(&s->sendQueueMutex);
    return more;
}

// Drops the destinations in `aidBitmap` (and the disconnected ones) from every queued entry; entries
// left without destinations are reported to the ARM9 as done and freed. The send queues are walked
// twice.
void WMSP_CleanSendQueue(u32 aidBitmap) {
    WmspSendEntry *entry = wmspW.status->sendEntry;
    WmspStatus *st;
    u32 mask;
    int pass;
    WmspSendQueue *queues;
    int prio;
    WmspSendQueue *q;
    WmspSendEntry *e;
    u32 idx;
    u16 *prevNext;
    u32 prevIdx;
    WmspPortSendCallback *cb;

    st = wmspW.status;
    mask = ~aidBitmap & st->childBitmap;
    __VENEER_OS_LockMutex(&st->sendQueueMutex);
    queues = st->sendQueue;
    for (pass = 0; pass < 2; pass++) {
        for (prio = 0; prio < 4; prio++) {
            q = &queues[prio];
            idx = q->head;
            prevNext = &q->head;
            prevIdx = 0xffff;
            while (idx != 0xffff) {
                e = &entry[idx];
                e->restBitmap &= mask;
                e->inFlightBitmap &= mask;
                if (e->restBitmap == 0) {
                    cb = (WmspPortSendCallback *)__VENEER_WMSP_GetBuffer4Callback2Wm9();
                    cb->command = 0x81;
                    cb->result = 0;
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
                        u16 parentSize = st->parentSize;
                        u16 childSize = st->childSize;
                        cb->sendSize = st->aid == 0 ? parentSize : childSize;
                        if (st->aid != 0) {
                            childSize = parentSize;
                        }
                        cb->recvSize = childSize;
                    }
                    __VENEER_WMSP_ReturnResult2Wm9(cb);
                    if (e->next == 0xffff) {
                        q->tail = prevIdx;
                    }
                    *prevNext = e->next;
                    e->next = 0xffff;
                    if (st->sendFreeQueue.tail == 0xffff) {
                        st->sendFreeQueue.head = idx;
                    } else {
                        entry[st->sendFreeQueue.tail].next = idx;
                    }
                    st->sendFreeQueue.tail = idx;
                    idx = prevIdx;
                }
                prevIdx = idx;
                if (idx != 0xffff) {
                    prevNext = &entry[idx].next;
                } else {
                    prevNext = &q->head;
                }
                idx = (idx != 0xffff) ? entry[idx].next : q->head;
            }
        }
    }
    __VENEER_OS_UnlockMutex(&st->sendQueueMutex);
}

// s->portSeqNo[aid][port], with the row computed as a byte offset (the indexed form compiles differently).
#define PORT_SEQ_NO(s, aid, port) (((u16 *)((u8 *)(s)->portSeqNo + (aid) * 16))[port])

// Splits a received MP frame of `srcAid` into its port packets (layout: see wmsp_send_ma.c; `header`
// is the first packet's header word) and reports each one addressed to this station to the ARM9.
// Packets of a sequenced port (header bit 11) older than the last one received are dropped.
void WMSP_ParsePortPacket(u16 srcAid, u16 header, void *data, s32 size, void *context) {
    WmspStatus *s = wmspW.status;
    u16 *input;
    int initial;
    u16 *payloadp;

    if (size == 0 || (u32)size > 0x204 || srcAid == s->aid || srcAid >= 16 || (size & 1)) {
        return;
    }
    input = data;
    initial = 1;
    while (size > 0) {
        u16 bits, *extra;
        int payload;
        u32 seqNo = 0xffff;
        int hasDest, hasSeq, len;
        u16 port;
        {
            u16 *borrowed;
            if (initial == 1) {
                borrowed = &header;
                initial = 0;
            } else {
                borrowed = input++;
                size -= 2;
            }
            bits = *borrowed;
        }
        payload = (bits & 255) * 2;
        if (payload == 0) {
            payload = 512;
        }
        hasDest = (bits & 0x1000) ? 1 : 0;
        hasSeq = (bits & 0x800) ? 1 : 0;
        len = payload + (hasSeq ? 2 : 0) + (hasDest ? 2 : 0);
        size -= len;
        if (size < 0) {
            return;
        }
        port = (bits & 0xf00) >> 8;
        payloadp = input;
        extra = (u16 *)((u8 *)payloadp + payload);
        input = (u16 *)((u8 *)input + len);
        if (hasSeq == 1) {
            u32 slot = port & 7;
            u16 old = PORT_SEQ_NO(s, srcAid, slot);
            seqNo = *extra;
            extra++;
            if (old & 1) {
                PORT_SEQ_NO(s, srcAid, slot) = seqNo * 2;
            } else {
                seqNo = seqNo * 2;
                if ((u16)(old - seqNo) < 0x100) {
                    continue;
                }
                PORT_SEQ_NO(s, srcAid, slot) = seqNo;
            }
        }
        if (hasDest == 1 && !(*extra & (1U << s->aid))) {
            continue;
        }
        if (payload > 0) {
            WmspPortRecvCallback *cb = (WmspPortRecvCallback *)__VENEER_WMSP_GetBuffer4Callback2Wm9();
            cb->command = 0x82;
            cb->result = 0;
            cb->api = 0x15;
            cb->port = port;
            cb->context = context;
            cb->data = payloadp;
            cb->size = payload;
            cb->srcAid = srcAid;
            cb->destAid = s->aid;
            cb->seqNo = seqNo;
            {
                u16 parentSize = s->parentSize, childSize = s->childSize;
                cb->sendSize = s->aid == 0 ? parentSize : childSize;
                cb->recvSize = s->aid == 0 ? childSize : parentSize;
            }
            __VENEER_WMSP_ReturnResult2Wm9(cb);
        }
    }
}
