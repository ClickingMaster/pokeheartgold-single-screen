#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define ROOT (*(u8**)(0x0380fff4))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(u8**)((u8*)(p)+(o)))
#define IO(a) (*(volatile u16*)(a))
extern void AddTask(int,u32);
extern void IntrCarrierSuppresionSignal(void);
extern void SetParentTbttTxq(void);
extern void WlIntrMpEnd(int);
void WlIntrTxEnd(void);
typedef struct {u8 reserved[0x3c];u16 active,pad;u32 reserved40;u8 *packet;u8 reserved48[0x44];u32 flags;u8 *entries;u32 reserved94;u16 mask,pad9a,pad9c,threshold;u8 reserveda0[8];u16 counta8,countaa,countac;} Queue40754;
typedef struct{u16 reserved:4;u16 toggle:1;u16 rest:11;}W37020Flags;
void WlIntrTxEnd(void){
u8*base=ROOT;Queue40754*q;u32 status;
IO(0x04808010)=2;
q=(Queue40754 *)(base+0x42c);
if(H(ROOT,0x34c)==0x12){IntrCarrierSuppresionSignal();return;}
status=IO(0x048080b8)&0xf00;
switch(status){
case 0x300:{u8*p;if(q->flags&2)SetParentTbttTxq();p=ROOT;W(p,0x558)++;AddTask(0,8);break;}
case 0x800:{u16 value=H(q->packet,4)&255;u8*p;
if(q->threshold<=value){IO(0x048080ac)=2;q->counta8++;}
p=q->entries;H(p,0x18)++;break;}
case 0xb00:{u16 mask,bits;u8*entry;
IO(0x04805f70)=0xffff;IO(0x04805f72)=0xffff;IO(0x0480824c)=0xffff;IO(0x0480824e)=0xffff;
if(q->active&&H(q->packet,0x22)==0xffff){if(H(q->packet,4)){H(q->packet,4)=0;H(q->packet,2)=q->mask;}}
bits=H(q->packet,2);mask=q->mask;entry=q->entries+26;
if(bits>1&&(((W37020Flags*)(ROOT+0x33a))->toggle)){if(!(IO(0x0480819c)&1))IO(0x04808290)^=1;}
if(H(ROOT,0x690)&64){while(bits>1){bits=(u16)(bits>>1);if(bits&1)H(entry,6)++;mask=(u16)(mask>>1);if(mask&1)entry+=H(q->entries,0x16);}}
break;}}
if(status==0x800)goto scheduled;
if(IO(0x048080b0)&2)goto scheduled;
{
if(q->active){IO(0x048080b4)=2;IO(0x04808048)=0;if(IO(0x04808010)&0x1000)q->countac++;else WlIntrMpEnd(2);q->countaa++;}
IO(0x048080ae)=2;
}
scheduled: AddTask(0,14);
}
