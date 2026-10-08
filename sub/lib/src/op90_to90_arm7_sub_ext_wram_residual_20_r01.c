#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct {u16 next,port,dest,remaining,sent,retry,pad,len,sequence,retryNumber;u8 *data;void *callback,*arg;} SQEntry;
typedef struct {u16 head,tail;} SQPair;
typedef struct {
 u16 mode;u8 g02[0x1c-2];u32 x1c;u8 g20[0x38-0x20];u16 sendSize;u8 g3a[0x7c-0x3a];u8 *sendBuf;u16 sendBufSize;u8 g82[0x8a-0x82];u16 sending;
 u8 g8c[0x182-0x8c];u16 allowed;u8 g184[0x188-0x184];u16 aid;u8 g18a[0x1f8-0x18a];u16 seqNo[16][8];
 SQEntry entries[32];SQPair free,active[4],ready[4];u8 mutex[24];u32 busy;u8 g738[0x7c6-0x738];u16 x7c6,x7c8;
} WmspState;
typedef char WmspStateCheck[(sizeof(WmspState)==0x7cc)?1:-1];
typedef struct {u8 pad[0x154c];u8 *config;WmspState *state;} WmspWork;
extern WmspWork wmspW;
#define G (&wmspW)
typedef struct {u16 a,b,result;} WlResp;
extern void __VENEER_OS_LockMutex(void *),__VENEER_OS_UnlockMutex(void *);
extern void __VENEER_MIi_CpuCopy16(const void *,void *,u32);
extern WlResp *WMSP_WL_MaKeyData(void *,u16,u16,void *);
void WMSP_SendMaKeyData(void);
// @fn WMSP_SendMaKeyData
void WMSP_SendMaKeyData(void) {

 SQEntry *e;
 u32 pri;
 u16 *head;
 u32 dest;
 int len;
 u8 *output;
 WmspState *s;
 u32 prev;
 int left;
 u16 *prevNext;
 SQPair *active;
 SQPair *ready;
 SQEntry *entries;
 int skip;
 u32 index;
 int error;
 int first;
 u32 ports;
 u32 port;
 WmspState *status;
 int seq;
 int extra;

 int left2;
 u32 mask;
 u16 header;
 u32 buf[128];
 status=G->state;
 if(status->allowed==0)return;
 status->sending=1;
 error=1;
 header=0;
 s=G->state;
 left=s->sendSize;
 head=&header;
 output=s->sendBuf;
 if(s->sendBufSize<((left+31)&~31))goto finish;
 if(s->aid>=16)goto finish;
 if(s->mode==9){}
 else if(s->mode==10)error=0;
 else{error=1;goto finish;}
 *head=0;len=0;
 if(left<0){error=1;goto finish;}
 mask=1;
 skip=1;
 if(error){if(s->x7c6){skip=0;mask=1;}}else skip=0;
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
   if(skip&&(dest&~1))goto next;
   seq=(port&8)?1:0;
   extra=(error&&(e->remaining|1)!=0xffff)?1:0;
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
   index=prev;
   if(s->x7c8==1)goto unlock;
  next:
   prev=index;
   if(index!=0xffff)prevNext=&entries[index].next;else prevNext=&ready->head;
   index=(index!=0xffff)?entries[index].next:ready->head;
  }
 }
unlock:
 __VENEER_OS_UnlockMutex(s->mutex);
 error=0;
finish:
 if(error==1){status->sending=0;return;}
 if(status->x1c==1)header|=0x8000;
 {WlResp *r=WMSP_WL_MaKeyData(buf,(u16)len,header,status->sendBuf);
 if(r->result!=0&&r->result!=8)status->sending=0;}
}
