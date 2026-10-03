#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct{void*thread;int wait;u8 state;u8 pad[7];int tick;u8 pad2[0x10];u32 seq;u8 pad3[4];u16 window;u8 pad4[2];u32 ack,counter;u8 pad5[4];u32 capacity;u8*buffer;u32 used;} S;
typedef struct{u16 src,dst,seqhi,seqlo,ackhi,acklo;u8 offset,flags;u16 window;} T;
#define SWAP(x) ((u16)(((x)>>8)|((x)<<8)))
extern S *find_socket(void*);extern void tcp_send_rst(void*,T*,u32,int),tcp_send_ack(S*,int),tcp_send_finack(S*,int);
void dt_ack(void*,T*,u32);
void dt_ack(void*p,T*t,u32 n){S*s=find_socket(p);u32 ack,seq;u8 flags;int complete;if(!s){tcp_send_rst(p,t,n,0);return;}ack=(u32)SWAP(t->ackhi)<<16|SWAP(t->acklo);flags=t->flags;if((s32)(ack-s->ack)>0)s->ack=ack;seq=(u32)SWAP(t->seqhi)<<16|SWAP(t->seqlo);if(s->state==4&&s->seq!=seq){tcp_send_ack(s,0);return;}s->window=SWAP(t->window);switch(s->state){case 0:case 2:tcp_send_rst(p,t,n,0);break;case 3:s->state=4;if(s->wait==1){s->wait=0;OS_WakeupThreadDirect(s->thread);}if(!n)break;case 4:s->counter++;{u32 room=s->capacity-s->used;if(n>room){complete=0;n=room;}else complete=1;}if(n){OSIntrMode irq=OS_DisableInterrupts();MI_CpuCopy8((u8*)t+(int)(t->offset&0xf0)/4,s->buffer+s->used,n);s->used+=n;s->seq+=n;OS_RestoreInterrupts(irq);if(s->wait==2){s->wait=0;OS_WakeupThreadDirect(s->thread);}}if(complete&&(flags&1)){s->state=6;s->seq++;tcp_send_finack(s,0);if(n==0&&s->wait==2){s->wait=0;OS_WakeupThreadDirect(s->thread);}}else if(n)tcp_send_ack(s,0);break;case 7:case 8:if(flags&1){s->seq+=n+1;tcp_send_ack(s,0);s->state=0;if(s->wait==2){s->wait=0;OS_WakeupThreadDirect(s->thread);}}else{if(n){s->seq+=n;tcp_send_ack(s,0);}s->state=8;}break;case 6:case 9:s->state=0;if(s->wait==2){s->wait=0;OS_WakeupThreadDirect(s->thread);}break;default:if(flags&1)s->seq++;tcp_send_ack(s,0);break;}OS_YieldThread();}
