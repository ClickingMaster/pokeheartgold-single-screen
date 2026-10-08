#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct {u16 next,port,dest,remaining,sent,retry,pad,len,sequence,retryNumber;u8 *data;void *callback,*arg;} SQEntry;
typedef struct {u16 head,tail;} SQPair;
typedef struct {
 u16 mode;u8 g02[0x1c-2];u32 x1c;u8 g20[0x30-0x20];u16 mpSize,mpRecvSize;u8 g34[0x38-0x34];u16 sendSize,txop;u8 g3c[0x40-0x3c];u16 x40;
 u8 g42[0x62-0x42];s16 x62,x64;u8 g66[0x68-0x66];u16 x68,x6a,x6c,x6e;u8 g70[0x72-0x70];u16 x72;u8 g74[0x7c-0x74];u8 *sendBuf;u16 sendBufSize;u8 g82[0x86-0x82];u16 x86,x88,sending;
 u8 g8c[0x9e - 0x8c];u16 x9e;u8 ga0[0x182-0xa0];u16 allowed;u8 g184[0x186-0x184];u16 children;u16 aid;u8 g18a[0x1f8-0x18a];u16 seqNo[16][8];
 SQEntry entries[32];SQPair free,active[4],ready[4];u8 mutex[24];u32 busy;u8 g738[0x7c6-0x738];u16 x7c6,x7c8;
} WmspState;
typedef char WmspStateCheck[(sizeof(WmspState)==0x7cc)?1:-1];
typedef struct {u8 pad[0x154c];u8 *config;WmspState *state;} WmspWork;
extern WmspWork wmspW;
#define G (&wmspW)
typedef struct {u16 type,result,pad0[2],event,port,dest,remaining,sent,pad1;void *data;u16 length,sequence;void *callback,*arg;u16 send,receive;} WmspResult;
extern void __VENEER_OS_LockMutex(void *),__VENEER_OS_UnlockMutex(void *);
extern void __VENEER_MIi_CpuCopy16(const void *,void *,u32);
extern u32 __VENEER_OS_DisableInterrupts(void);
extern void __VENEER_OS_RestoreInterrupts(u32);
extern u32 __VENEER_MATH_CountPopulation(u32);
extern u16 WmspGetTmptt(u32,u32,u32,u32);
extern void *WMSP_WL_MaMp(void *,u32,u32,u32,u16,u16,u16,u16,u16,u32);
extern WmspResult *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
void WMSP_SendMaMP(u16);
static inline void WmspFlushSendQueue(u32 bitmap) {
 u32 keep;
 u16 fhead;
 WmspState *fs;
 SQEntry *fe;
 int fpri;
 int fparent;
 u16 tail;
 u32 fallowed;
   fs=G->state;fe=fs->entries;
   if(fs->mode==9)fparent=1;else if(fs->mode==10)fparent=0;else return;
   __VENEER_OS_LockMutex(fs->mutex);
   if(!fs->busy){__VENEER_OS_UnlockMutex(fs->mutex);return;}
   if(!fparent)fallowed=1;else{u32 irq=__VENEER_OS_DisableInterrupts();fallowed=fs->allowed;__VENEER_OS_RestoreInterrupts(irq);}
   keep=~(u16)bitmap;
   for(fpri=0;fpri<4;fpri++){
    SQPair *queue=&fs->active[fpri];u32 fi=queue->head;tail=0xffff;fhead=0xffff;
    while(fi!=0xffff){SQEntry *x=&fe[fi];
     x->sent|=x->retry&keep;x->remaining&=~x->sent;
     x->remaining&=fallowed;x->retry=0;
     if(x->remaining&&((x->port&8)||x->retryNumber)){
      if(x->retryNumber)x->retryNumber--;
      if(x->next==0xffff)queue->tail=0xffff;queue->head=x->next;x->next=0xffff;
      if(tail==0xffff)fhead=fi;else fe[tail].next=fi;tail=fi;
     }else{
      WmspResult *out=__VENEER_WMSP_GetBuffer4Callback2Wm9();out->type=0x81;out->result=x->remaining==0?0:15;out->event=0x14;out->port=x->port;out->dest=x->dest;out->remaining=x->remaining;out->sent=x->sent;out->length=x->len;out->data=x->data;out->callback=x->callback;out->arg=x->arg;out->sequence=x->sequence;
      {u16 a=fs->mpSize,b=fs->mpRecvSize;out->send=fs->aid==0?a:b;out->receive=fs->aid==0?b:a;}
      __VENEER_WMSP_ReturnResult2Wm9(out);
      if(x->next==0xffff)queue->tail=0xffff;queue->head=x->next;x->next=0xffff;
      if(fs->free.tail==0xffff)fs->free.head=fi;else fe[fs->free.tail].next=fi;fs->free.tail=fi;
     }
     fi=queue->head;
    }
    if(tail!=0xffff){fe[tail].next=fs->ready[fpri].head;if(fs->ready[fpri].tail==0xffff)fs->ready[fpri].tail=tail;fs->ready[fpri].head=fhead;}
   }
   fs->busy=0;__VENEER_OS_UnlockMutex(fs->mutex);
}
// @fn WMSP_SendMaMP
void WMSP_SendMaMP(u16 bitmap) {
 u32 size;
 u32 time;
 u16 tmptt;
 int error;
 u16 txop;
 u32 children;
 u32 allowed;
 u16 *head;
 int extra;
 int seq;
 u16 port;
 SQEntry *e;
 u32 destBits;
 u32 index;
 u32 dest;
 u32 prev;
 u16 *prevNext;
 SQPair *active;
 SQPair *ready;
 u32 pri;
 SQEntry *entries;
 u32 mask;
 int skip;
 int parent;
 int first;
 u32 ports;
 int len;
 u8 *output;
 WmspState *status;
 WmspState *s;
 int left;
 int left2;
 u16 header;
 u32 buf[128];
 status=G->state;
 {u32 irq=__VENEER_OS_DisableInterrupts();allowed=status->allowed;children=status->x86;__VENEER_OS_RestoreInterrupts(irq);}
 header=0;
 if(status->x88==1){bitmap=0xffff;status->x88=0;}
 if(status->x9e==0){
  txop=status->txop;
  s=G->state;
  left=s->sendSize;
  head=&header;
  output=s->sendBuf;
  if(s->sendBufSize<((left+31)&~31)){error=1;goto finish;}
  if(s->aid>=16){error=1;goto finish;}
  if(s->mode==9)parent=1;
  else if(s->mode==10)parent=0;
  else{error=1;goto finish;}
  *head=0;destBits=0;size=0;len=0;
  if(left<0){error=1;goto finish;}
  mask=children;
  skip=1;
  if(parent){if(s->x7c6){skip=0;mask=allowed;}}else skip=0;
  children&=bitmap;
  ports=0;first=1;
  __VENEER_OS_LockMutex(s->mutex);
  if(s->busy==1){__VENEER_OS_UnlockMutex(s->mutex);error=1;goto finish;}
  s->busy=1;
  entries=s->entries;
  for(pri=0;pri<4&&left>2;pri++){
   ready=&s->ready[pri];active=&s->active[pri];
   index=ready->head;prevNext=&ready->head;prev=0xffff;
   while(index!=0xffff&&left>2){
    e=&entries[index];
    port=e->port;
    if(ports&(1<<port))goto next;
    ports|=1<<port;
    dest=e->remaining&mask;
    if(skip&&(dest&~children))goto next;
    seq=(port&8)?1:0;
    extra=(parent&&(e->remaining|1)!=0xffff)?1:0;
    if((e->len&1)==1)e->len++;
    left2=e->len+(first?0:2)+(seq?2:0)+(extra?2:0);
    if(left2>left)goto next;
    if(!first){head=(u16*)output;*(u16*)output=0;output+=2;len+=2;left-=2;}
    *head|=((e->port<<8)&0xf00)|(((u32)e->len>>1)&0xff);
    __VENEER_MIi_CpuCopy16(e->data,output,e->len);
    len+=e->len;output+=e->len&~1;left-=e->len;
    if(seq==1){
     int sv=e->sequence;
     if(!(sv&1))sv>>=1;
     else{sv=s->seqNo[s->aid][e->port&7];s->seqNo[s->aid][e->port&7]=sv+1;e->sequence=sv<<1;}
     len+=2;*(u16*)output=sv&0x7fff;output+=2;left-=2;
    }
    if(extra==1){len+=2;left-=2;*head|=0x1000;*(u16*)output=e->remaining;output+=2;}
    if(first==1)first=0;
    if(ready->tail==index)ready->tail=prev;
    *prevNext=e->next;
    e->next=0xffff;
    if(active->tail==0xffff)active->head=index;else entries[active->tail].next=index;
    ports&=~(1<<port);
    active->tail=index;
    e->retry=dest;
    destBits|=e->dest;
    index=prev;
    if(s->x7c8==1)goto unlock;
   next:
    prev=index;
    if(index!=0xffff)prevNext=&entries[index].next;else prevNext=&ready->head;
    index=(index!=0xffff)?entries[index].next:ready->head;
   }
  }
 unlock:
  size=len;
  __VENEER_OS_UnlockMutex(s->mutex);
  error=0;
 finish:
  if(error==1){status->x62=0;status->x64=0;return;}
  time=txop+2;
  if(status->x7c6!=1)destBits=bitmap;
  destBits&=allowed;
  if((s32)status->x72<(s32)(((txop+12)*__VENEER_MATH_CountPopulation(destBits)+0x29)&~31U)){
   WmspFlushSendQueue(destBits);
  clear:
   status->x62=0;status->x64=0;return;
  }
  if(status->x62!=1&&status->x64!=1)tmptt=0;
  else{tmptt=WmspGetTmptt(size,time,destBits,status->x40);header|=0x8000;}
 }else{
  tmptt=0;status->x9e=0;header&=~0x8000;destBits=allowed;size=0;time=0x80d6;
  {u32 irq=__VENEER_OS_DisableInterrupts();status->x62++;status->x64++;__VENEER_OS_RestoreInterrupts(irq);}
 }
 WMSP_WL_MaMp(buf,0,0,(u16)time,(u16)destBits,tmptt,*(vu16*)0x048080F8,(u16)size,header,(u32)status->sendBuf);
 status->x68=destBits;status->x6a=header;status->x6c=time;status->x6e=size;
}
