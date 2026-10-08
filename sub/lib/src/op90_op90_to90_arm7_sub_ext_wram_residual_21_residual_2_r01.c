#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct {
    u16 next, port, destBitmap, restBitmap, sentBitmap, x0a, x0c, size, x10, seqNo;
    u16 *data;
    void *callback;
    void *arg;
} SendEntry;
typedef struct { u16 head, tail; } SendQueue;
typedef struct {
    u8 pad0[0x30];
    u16 x30, x32;
    u8 pad34[0x182 - 0x34];
    u16 childBitmap;   /* 0x182 */
    u8 pad184[0x188 - 0x184];
    u16 aid;           /* 0x188 */
    u8 pad18a[0x2f8 - 0x18a];
    SendEntry entry[32];     /* 0x2f8 */
    SendQueue freeQueue;     /* 0x6f8 */
    u8 pad6fc[0x70c - 0x6fc];
    SendQueue queue[4];      /* 0x70c */
    u8 mutex[0x18];          /* 0x71c */
} WmspStatus;
typedef struct { u8 pad[0x1550]; WmspStatus *status; } WmspWork;
extern WmspWork wmspW;
typedef struct {
    u16 apiid, errcode, wlCmdID, wlResult, state, port, destBitmap, restBitmap, sentBitmap, x12;
    u16 *data;
    u16 size, seqNo;
    void *callback;
    void *arg;
    u16 myAid, x26;
} PortSendCb;
extern PortSendCb *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern void __VENEER_OS_LockMutex(void *);
extern void __VENEER_OS_UnlockMutex(void *);
void WMSP_CleanSendQueue(u32 aidBitmap);

// @fn WMSP_CleanSendQueue
void WMSP_CleanSendQueue(u32 aidBitmap){/*D*/SendEntry*entry=wmspW.status->entry;WmspStatus*st;u32 mask;int pass;SendQueue*queues;int prio;SendQueue*q;SendEntry*e;u32 idx;u16*prevNext;u32 prevIdx;PortSendCb*cb;/*E*/st=wmspW.status;mask=~aidBitmap&st->childBitmap;__VENEER_OS_LockMutex(st->mutex);queues=st->queue;for(pass=0;pass<2;pass++){for(prio=0;prio<4;prio++){q=&queues[prio];idx=q->head;prevNext=&q->head;prevIdx=0xffff;while(idx!=0xffff){e=&entry[idx];e->restBitmap&=mask;e->x0a&=mask;if(e->restBitmap==0){cb=__VENEER_WMSP_GetBuffer4Callback2Wm9();cb->apiid=0x81;cb->errcode=0;cb->state=0x14;cb->port=e->port;cb->destBitmap=e->destBitmap;cb->restBitmap=e->restBitmap;cb->sentBitmap=e->sentBitmap;cb->size=e->size;cb->data=e->data;cb->callback=e->callback;cb->arg=e->arg;cb->seqNo=e->x10;{u16 a=st->x30;u16 b=st->x32;cb->myAid=st->aid==0?a:b;if(st->aid!=0)b=a;cb->x26=b;}__VENEER_WMSP_ReturnResult2Wm9(cb);if(e->next==0xffff)q->tail=prevIdx;*prevNext=e->next;e->next=0xffff;if(st->freeQueue.tail==0xffff)st->freeQueue.head=idx;else entry[st->freeQueue.tail].next=idx;st->freeQueue.tail=idx;idx=prevIdx;}prevIdx=idx;if(idx!=0xffff)prevNext=&entry[idx].next;else prevNext=&q->head;idx=(idx!=0xffff)?entry[idx].next:q->head;}}}__VENEER_OS_UnlockMutex(st->mutex);}
