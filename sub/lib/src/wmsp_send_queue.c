#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
// Low half-word of the wireless TSF timer.
#define REG_WL_TSF0 (*(vu16 *)0x048080F8)

#pragma dont_inline on

// clang-format off
asm u32 __VENEER_MATH_CountPopulation(u32 value) {
    ldr pc, =MATH_CountPopulation
}
// clang-format on

// Restarts the MP sequence to the children in `bitmap` (request 0x800C), or retries later through
// WMSP_RequestResumeMP while the receive buffer is too small for them.
void WMSP_ResumeMaMP(u32 bitmap) {
    u32 buf[128];
    u32 mask = bitmap;
    u16 current;
    u32 time;
    WmspStatus *status = wmspW.status;
    u16 recvSize, tmptt;
    __VENEER_OS_RestoreInterrupts(__VENEER_OS_DisableInterrupts());
    mask &= status->childBitmap;
    recvSize = status->recvSize;
    current = REG_WL_TSF0;
    time = status->prevWmHeader;
    if ((s32)status->mpBufSize < (s32)(((recvSize + 12) * __VENEER_MATH_CountPopulation(mask) + 0x29) & ~31U)) {
        __VENEER_OS_Sleep(2);
        WMSP_RequestResumeMP();
        return;
    }
    if (status->mpCount != 1 && status->mpLimitCount != 1) {
        time &= ~0x8000;
        tmptt = 0;
    } else {
        tmptt = WmspGetTmptt(status->prevDataLength, status->prevTxop, mask, (u16)status->parentVAlarmLine);
        time |= 0x8000;
    }
    WMSP_WL_MaMp(buf, 0x800c, 0, 0, (u16)mask, tmptt, current, 0, time, 0);
}

// clang-format off
asm void __VENEER_OS_Sleep(u32 msec) {
    ldr pc, =OS_Sleep
}
// clang-format on

// Appends a send of `data` to the queue of `priority`. Returns 2 (queued), 6 (bad size) or 10
// (no free entry).
u32 WMSP_PutSendQueue(u32 childBitmap, u32 priority, u32 port, u32 destBitmap, u16 *data, u16 size, void *callback, void *arg) {
    WmspSendEntry *entry = wmspW.status->sendEntry;
    WmspSendQueue *queue = &wmspW.status->sendQueue[priority];
    WmspStatus *status = wmspW.status;
    u32 index;
    WmspSendEntry *e;
    if (size == 0) {
        return 6;
    }
    // port bit 3: two more bytes (sequence number)
    if (size + ((port & 8) ? 2 : 0) > 0x204) {
        return 6;
    }
    __VENEER_OS_LockMutex(&status->sendQueueMutex);
    index = status->sendFreeQueue.head;
    if (index == 0xffff) {
        __VENEER_OS_UnlockMutex(&status->sendQueueMutex);
        return 10;
    }
    e = &entry[index];
    status->sendFreeQueue.head = entry[index].next;
    if (status->sendFreeQueue.tail == index) {
        status->sendFreeQueue.tail = 0xffff;
    }
    e->port = port;
    e->destBitmap = destBitmap;
    e->restBitmap = destBitmap & childBitmap;
    e->sentBitmap = 0;
    e->inFlightBitmap = 0;
    e->data = data;
    e->size = size;
    e->callback = callback;
    e->arg = arg;
    e->next = 0xffff;
    e->seqNo = 0xffff;
    e->retryCount = status->sendRetryCount;
    if (queue->tail == 0xffff) {
        queue->head = index;
    } else {
        entry[queue->tail].next = index;
    }
    queue->tail = index;
    __VENEER_OS_UnlockMutex(&status->sendQueueMutex);
    return 2;
}

#pragma dont_inline reset
