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
extern void AddTask(u32,u32);
extern void WUpdateCounter(void);
extern u32 AdjustRingPointer(u32);
extern u32 CheckKeyTxEnd(void);
extern void SetFatalErr(u32);
extern void WClearKSID(u32);
extern void __VENEER_OS_CancelAlarm(void*);
extern void __VENEER_OS_SetAlarm(void*,u64,void(*)(void*),void*);
extern u64 __VENEER__ll_udiv(u64,u64);
void WlIntrRxEnd(void);
typedef struct{u16 reserved:7;u16 keyFlag:1;u16 rest:8;}W37021Flags;
// @fn WlIntrRxEnd
void WlIntrRxEnd(void){
u8*base=ROOT;u32 enabled;u16*nextp;u8*q;u8*rx;u16*packet;u32 current,end;q=base+0x344;rx=base+0x4dc;enabled=H(base,0x690);
IO(0x04808010)=1;
if(!H(q,12))IO(0x0480805a)=IO(0x04808054);
enabled&=1;for(;;){u32 tick1,tick2,hi1,hi2,ticks;u16*timestampp,*lengthp;u16*durationp;u32 len;u32 frame;
current=H(rx,4);end=IO(0x04808054);if(current==end)break;
tick1=IO(0x048080f8);hi1=IO(0x048080fa);tick2=IO(0x048080f8);hi2=IO(0x048080fa);
if(tick1>tick2)ticks=(tick2>>4)|(hi2<<12);else ticks=(tick1>>4)|(hi1<<12);
if(current>=0x8c6&&current<=0x8ef)WUpdateCounter();
packet=(u16*)(0x04804000+current*2);
nextp=(u16*)AdjustRingPointer((u32)(packet+1));timestampp=(u16*)AdjustRingPointer((u32)(nextp+1));lengthp=(u16*)AdjustRingPointer((u32)(timestampp+2));durationp=(u16*)AdjustRingPointer((u32)(packet+7));
{u32 part=*nextp;packet[0]|=(part<<1)&0x400;}*timestampp=(u16)ticks;len=*lengthp;
current=((len+(current<<1)+15)>>2)*2;
if(current>=0xfb0)current-=(u32)H(q,0x9a)>>1;
if(len>0x92c){current=end;packet[0]=0xffff;H(q,0xb4)++;}
else if(enabled&&current!=end){u32 next=0x04804000+current*2;u32 rate;u32 st=*(u16*)((u8*)0x04804000+current*2);
if(next<0x04805f5a)rate=*(u16*)(next+6)&255;else rate=*(u16*)(next-H(q,0x9a)+6);
if((st&0x7c00)||(rate!=10&&rate!=20)||len>0xfff){H(q,0xb4)++;packet[0]=0xffff;H(rx,4)=(u16)end;*nextp=(u16)end;break;}}
{u32 type=packet[0]&15;if(type==12){frame=*(u16*)AdjustRingPointer((u32)(packet+6));u32 id=*(u16*)AdjustRingPointer((u32)(packet+17));
if(H(rx,0)==id&&(frame&0x800)){u8*p=ROOT;W(p,0x57c)++;packet[0]=0xffff;}
else{u8*p=ROOT;if(!(((W37021Flags*)(p+0x33a))->keyFlag)&&H(p,0x34c)==0x40){if(IO(0x04808028)&& (IO(0x04808098)&0x8000)){u64 ticks;
__VENEER_OS_CancelAlarm(p+0x660);ticks=__VENEER__ll_udiv(((u64)*durationp*0x82ea)>>6,1000);__VENEER_OS_SetAlarm(ROOT+0x660,ticks,(void(*)(void*))WClearKSID,0);
}else{id=0xffff;IO(0x04805f7e)=0xffff;IO(0x0480824c)=0xffff;IO(0x0480824e)=0xffff;packet[0]=0xffff;}}}
H(rx,0)=(u16)id;if(CheckKeyTxEnd()&1){u8*p=ROOT;W(p,0x5a8)++;}
}else if(type==13){u8*p=ROOT;if(!(((W37021Flags*)(p+0x33a))->keyFlag)&&H(p,0x34c)==0x40){if(!IO(0x04808028)||!(IO(0x04808098)&0x8000))packet[0]=0xffff;}}}
H(rx,4)=(u16)current;*nextp=(u16)current;
}
if(enabled){u32 prior=IO(0x04808054);u16 key=CheckKeyTxEnd();if(key&&prior==IO(0x04808054)){if(key&2)SetFatalErr(0x80);else if(key&1)SetFatalErr(0x100);}}
if(IO(0x0480805a)!=IO(0x04808054))AddTask(0,15);
}
