#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
void ov18_021E613C(void*,int);void FillWindowPixelBuffer(void*,int);void ov18_021F9648(void*,void*,int,int,int,int,u32,int);int ov18_021E7698(int);void ov18_021EF388(void*,int,int);void ov18_021EFC3C(void*,int);void ScheduleWindowCopyToVram(void*);
void ov18_021EF764(u8*w);
void ov18_021EF764(u8*w){u16 i;ov18_021E613C(w,0);FillWindowPixelBuffer(w+0x41c,0);FillWindowPixelBuffer(w+0x43c,0);ov18_021F9648(w+0x41c,P(w,0x65c),27,24,0,0,0x20100,2);
 for(i=0;i<27;i++){u16 n=ov18_021E7698(i);int msg;if(i==26)msg=113;else msg=i+69;int rem=n%7;ov18_021F9648(w+0x43c,P(w,0x65c),msg,rem*32+24,(n/7)*32,4,0x20100,2);}
 ov18_021EF388(w,17,39);ov18_021EF388(w,19,40);ov18_021EFC3C(w,66);ScheduleWindowCopyToVram(w+0x41c);ScheduleWindowCopyToVram(w+0x43c);ScheduleWindowCopyToVram(w+0x42c);
}
