#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(a) (*(volatile u16 *)(a))
#define WL (*(u8 **)0x0380fff4)
#define R16(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define R32(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define P(p,o) (*(u8 **)((u8 *)(p)+(o)))
extern void SetupUsTimeOut(u16,void(*)(void)),MacBugTxMp(void),AddTask(int,int);
void WlIntrMpEnd(int timeout);
// @fn WlIntrMpEnd
void WlIntrMpEnd(int timeout){u8 *slots=WL+0x42c;H(0x04808010)=0x1000;if(R16(slots,0x3c)){if((R16(WL,0x690)&16)&&timeout){u16 ack=H(0x048080b6);u16 state=H(0x04808214);if((state==3||state==5)&&ack==0){u32 count=0;u8 *frame=P(slots,0x44);u16 mask=R16(frame,2);while(mask){count+=mask&1;mask=(u16)(mask>>1);}SetupUsTimeOut((u16)(count*(R16(frame,0x24)+10)+0xc0+(R16(frame,0xa)<<2)),MacBugTxMp);R16(WL,0x400)++;return;}}AddTask(0,16);}}
