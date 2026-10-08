#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define G (*(u8*volatile*)0x0380fff4)
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define P(p,n) (*(u8**)((u8*)(p)+(n)))
#define V(n) (*(volatile u16*)(n))
void* AllocateHeapBuf(void*,u32);void WSetMacAdrs3(void*,const void*,const void*,const void*);void WUpdateCounter(void);void DMA_Write(void*,const void*,u32);u32 __VENEER_OS_DisableInterrupts(void);void __VENEER_OS_RestoreInterrupts(u32);void ReleaseHeapBuf(void*,void*);extern const u8 MP_ADRS[];extern u32 __VENEER__u32_div_f(u32,u32);typedef struct W38807Request{u16 prefix[8];u16 flags,txop,interval,bitmap,period,offset,payloadBytes,sequence;void*payload;}W38807Request;
typedef struct W38807Result{u16 command,error;}W38807Result;
int MA_MpReqCmd(W38807Request*,W38807Result*);
// @fn MA_MpReqCmd
int MA_MpReqCmd(W38807Request*a,W38807Result*o){
 u8*p=G;u8*b;u8*q=p+0x42c;u8*c=p+0x31c;u32 ctl;u8*heap;u8*tail;u8*entry;u32 count;int send=1;u32 flags=H(p,0x690);u32 key;u32 bytes;u32 irq;u32 mode;u16 bit;
 o->error=send;if(H(c,0x12)!=1)return 11;if(H(q,0x3c))return 8;b=P(q,0x44);
 if(a->flags&0x8000){if(!(a->flags&2))a->interval=H(q,0x94);if(!(a->flags&4))a->bitmap=H(q,0xa0);if(!(a->flags&8))a->period=H(q,0x96);if(!(a->flags&16)){a->payloadBytes=H(q,0x9c);send=0;}}
 if(a->payloadBytes>0x204)return 5;bit=2;count=0;for(;bit!=0;bit=(u16)(bit<<1))if(a->bitmap&bit)count++;
 H(q,0x94)=a->interval;
 if(a->interval&0x8000){a->interval&=0x7fff;mode=((u32)a->interval-208)>>2;H(b,14)=234;if(mode>0x10000)return 5;}
 else{u32 pad,offset,extra;if(flags&2){pad=1;offset=2;extra=6;}else{pad=0;offset=pad;extra=pad;}mode=a->interval+pad;if(mode>0x206+pad)return 5;a->interval=mode*4+208+offset;H(b,14)=234+extra;}
 key=(mode+9)&~1u;bytes=key*count;heap=AllocateHeapBuf(p+0x188,bytes+26);P(q,0x90)=heap;if(!heap)return 8;
 H(q,0x3c)=1;H(q,0x3e)++;H(q,0x96)=a->period;H(q,0x98)=a->bitmap;H(q,0x9a)=0;H(q,0x9c)=a->payloadBytes;H(q,0x9e)=a->txop;
 H(b,0)=0;H(b,2)=a->bitmap;H(b,4)=0;H(b,8)=20;H(b,10)=a->payloadBytes+34;H(b,12)=0x228;H(b,14)+=(a->interval+10)*count;WSetMacAdrs3(b+16,MP_ADRS,p+0x3a8,c+8);
 if((a->flags&0x8000)&&H(b,0x22)!=0xffff)ctl=0x4000;else{H(b,0x22)=0xffff;ctl=0;}
 H(b,0x24)=a->interval;H(b,0x26)=a->bitmap;H(b,0x28)=a->sequence;if(send&&a->payloadBytes){WUpdateCounter();DMA_Write(b+0x2a,a->payload,a->payloadBytes);}
 if(flags&4){u32 length=a->payloadBytes+2;u8*payload=b+0x28;tail=(u8*)(((u32)(payload+length)+3)&~3u);H(tail,0)=0xb6b8;H(tail,2)=0x1d46;}
 H(P(q,0x90),12)=0x184;H(P(q,0x90),14)=(bytes+11)>>1;H(P(q,0x90),16)=a->bitmap;H(P(q,0x90),20)=count;H(P(q,0x90),22)=key;H(P(q,0x90),24)=0;entry=P(q,0x90)+26;
 {u32 aid;u16 participant;for(aid=1,participant=2;participant!=0;participant=(u16)(participant<<1),aid++){if(a->bitmap&participant){H(entry,0)=0xffff;H(entry,2)=0;H(entry,6)=0;H(entry,4)=aid;entry+=key;}}}
 V(0x048080c4)=a->interval;V(0x048080c0)=H(b,14);send=0x10000-a->offset;
 if(a->period==0){u32 t,base=(a->payloadBytes+34)*4+96;if(V(0x04808000)!=0x1440)base+=1000;{u32 subtotal=(u32)a->interval*count+904;t=__VENEER__u32_div_f(base+subtotal+50,10);}irq=__VENEER_OS_DisableInterrupts();V(0x04808118)=t;V(0x04808090)=(((u32)b&0x3fff)>>1)|0x8000|ctl;__VENEER_OS_RestoreInterrupts(irq);}
 else{u32 t;irq=__VENEER_OS_DisableInterrupts();t=(u16)(send+V(0x048080f8));t=__VENEER__u32_div_f(t,10);if(t+3<a->period){V(0x04808118)=a->period-t-1;V(0x04808090)=(((u32)b&0x3fff)>>1)|0x8000|ctl;__VENEER_OS_RestoreInterrupts(irq);}else{__VENEER_OS_RestoreInterrupts(irq);ReleaseHeapBuf(p+0x188,P(q,0x90));H(q,0x3c)=0;H(q,0x3e)--;return 5;}}
 return 0;
}
