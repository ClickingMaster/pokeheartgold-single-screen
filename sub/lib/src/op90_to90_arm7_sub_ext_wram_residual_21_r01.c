#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct {
    u16 next;      /* 0x00 */
    u16 port;      /* 0x02 */
    u16 destBitmap;/* 0x04 */
    u16 restBitmap;/* 0x06 */
    u16 sentBitmap;/* 0x08 */
    u16 x0a;
    u16 x0c;
    u16 size;      /* 0x0e */
    u16 x10;
    u16 seqNo;     /* 0x12 */
    u16 *data;     /* 0x14 */
    void *callback;/* 0x18 */
    void *arg;     /* 0x1c */
} SendEntry;
typedef struct { u16 head, tail; } SendQueue;
typedef struct {
    u8 pad[0x2f8];
    SendEntry entry[32];     /* 0x2f8 */
    SendQueue freeQueue;     /* 0x6f8 */
    u8 pad6fc[0x70c - 0x6fc];
    SendQueue queue[4];      /* 0x70c */
    u8 mutex[0x18];          /* 0x71c */
    u8 pad734[0x7ca - 0x734];
    u16 seqNo;               /* 0x7ca */
} WmspStatus;
typedef struct { u8 pad[0x1550]; WmspStatus *status; } WmspWork;
extern WmspWork wmspW;
extern void __VENEER_OS_LockMutex(void *);
extern void __VENEER_OS_UnlockMutex(void *);
u32 WMSP_PutSendQueue(u32 childBitmap, u32 priority, u32 port, u32 destBitmap, u16 *data, u16 size, void *callback, void *arg);
// @fn WMSP_PutSendQueue
u32 WMSP_PutSendQueue(u32 childBitmap, u32 priority, u32 port, u32 destBitmap, u16 *data, u16 size, void *callback, void *arg) {
    SendEntry *entry = wmspW.status->entry;
    SendQueue *queue = &wmspW.status->queue[priority];
    WmspStatus *status = wmspW.status;
    u32 index;
    SendEntry *e;

    if (size == 0) {
        return 6;
    }
    if (size + ((port & 8) ? 2 : 0) > 0x204) {
        return 6;
    }
    __VENEER_OS_LockMutex(status->mutex);
    index = status->freeQueue.head;
    if (index == 0xffff) {
        __VENEER_OS_UnlockMutex(status->mutex);
        return 10;
    }
    e = &entry[index];
    status->freeQueue.head = entry[index].next;
    if (status->freeQueue.tail == index) {
        status->freeQueue.tail = 0xffff;
    }
    e->port = port;
    e->destBitmap = destBitmap;
    e->restBitmap = destBitmap & childBitmap;
    e->sentBitmap = 0;
    e->x0a = 0;
    e->data = data;
    e->size = size;
    e->callback = callback;
    e->arg = arg;
    e->next = 0xffff;
    e->x10 = 0xffff;
    e->seqNo = status->seqNo;
    if (queue->tail == 0xffff) {
        queue->head = index;
    } else {
        entry[queue->tail].next = index;
    }
    queue->tail = index;
    __VENEER_OS_UnlockMutex(status->mutex);
    return 2;
}
