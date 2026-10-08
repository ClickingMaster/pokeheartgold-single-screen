#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u8 wmspW[];
typedef struct {u8 pad[0x54c];u8 *iface;u8 *state;} W40747Global;
#define G ((W40747Global *)(wmspW+0x1000))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
extern u32 __VENEER_OS_DisableInterrupts(void);
extern void __VENEER_OS_RestoreInterrupts(u32);
typedef struct {u16 next,port,dest,remaining,sent,retry,pad,len,sequence,retryNumber;void *data,*callback,*arg;} W40747Entry;
typedef struct {u16 head,tail;} W40747Pair;
typedef struct {u16 mode;u8 gap02[0x30-2];u16 sendSize,receiveSize;u8 gap34[0x182-0x34];u16 allowed;u8 gap184[4];u16 role;u8 gap18a[0x2f8-0x18a];W40747Entry entries[32];W40747Pair free,active[4],pending[4];u8 mutex[24];u32 busy;} W40747State;
typedef char W40747EntrySize[(sizeof(W40747Entry)==32)?1:-1];
typedef char W40747StateSize[(sizeof(W40747State)==0x738)?1:-1];
extern void __VENEER_OS_LockMutex(void *),__VENEER_OS_UnlockMutex(void *);
typedef struct W40747Result {u16 type,result,pad0[2],event,port,dest,remaining,sent,pad1;void *data;u16 length,sequence;void *callback,*arg;u16 send,receive;} W40747Result;
extern W40747Result *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
int WMSP_FlushSendQueue(int,u16);
#pragma force_active off
static inline void W40747RetryPop(W40747Pair *q,W40747Entry *e) {u32 empty=0xffff;if(e->next==empty)q->tail=empty;q->head=e->next;e->next=empty;}
static inline void W40747CompletePop(W40747Pair *q,W40747Entry *e) {u32 empty=0xffff;if(e->next==empty)q->tail=empty;q->head=e->next;e->next=empty;}
static inline void W40747FreeAppend(W40747Pair *q,W40747Entry *pool,u32 index) {u32 empty=0xffff;if(q->tail==empty)q->head=index;else pool[q->tail].next=index;q->tail=index;}
#pragma force_active on
// @fn WMSP_FlushSendQueue
int WMSP_FlushSendQueue(int retransmit,u16 inputfailed) {
 W40747Entry *entries;u16 failed=inputfailed;u32 index;int parent;W40747Pair *queue;W40747Entry *e;W40747State *s=(W40747State *)G->state;u16 allowed;int more=0;int priority;u16 head;u32 keep;entries=s->entries;
 if(s->mode==9)parent=1;else if(s->mode==10)parent=0;else return 0;
 __VENEER_OS_LockMutex(s->mutex);
 if(!s->busy){__VENEER_OS_UnlockMutex(s->mutex);return 0;}
 if(!parent)allowed=1;else{u32 irq=__VENEER_OS_DisableInterrupts();allowed=s->allowed;__VENEER_OS_RestoreInterrupts(irq);}
 keep=~failed;
 for(priority=0;priority<4;priority++){
 u16 tail;queue=&s->active[priority];index=queue->head;tail=0xffff;head=0xffff;
 while(index!=0xffff){e=&entries[index];
 if(!retransmit){e->sent |=e->retry&keep;e->remaining &=~e->sent;}
 e->remaining &=allowed;e->retry=0;
 if(e->remaining && ((e->port&8)||e->retryNumber)){
 more=1;if(e->retryNumber)e->retryNumber--;
 W40747RetryPop(queue,e);
 if(tail==0xffff)head=index;else entries[tail].next=index;tail=index;
 }else{
 W40747Result *out=__VENEER_WMSP_GetBuffer4Callback2Wm9();out->type=0x81;out->result=e->remaining==0?0:15;out->event=0x14;out->port=e->port;out->dest=e->dest;out->remaining=e->remaining;out->sent=e->sent;out->length=e->len;out->data=e->data;out->callback=e->callback;out->arg=e->arg;out->sequence=e->sequence;
 {u16 a=s->sendSize,b=s->receiveSize;out->send=s->role==0?a:b;out->receive=s->role==0?b:a;}
 __VENEER_WMSP_ReturnResult2Wm9(out);
 W40747CompletePop(queue,e);
 W40747FreeAppend(&s->free,entries,index);
 }
 index=queue->head;
 }
 if(tail!=0xffff){entries[tail].next=s->pending[priority].head;if(s->pending[priority].tail==0xffff)s->pending[priority].tail=tail;s->pending[priority].head=head;}
 }
 s->busy=0;__VENEER_OS_UnlockMutex(s->mutex);return more;
}
