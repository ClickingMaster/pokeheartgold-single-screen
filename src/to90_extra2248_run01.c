#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
void ov18_021E613C(void*,int);void FillWindowPixelBuffer(void*,int);void PokedexApp_PrintMessage(void*,void*,int,int,int,int,u32,int);int ov18_021E7698(int);void PokedexApp_DrawSearchButton(void*,int,int);void PokedexApp_DrawSearchNameValue(void*,int);void ScheduleWindowCopyToVram(void*);
void PokedexApp_DrawSearchNamePage(u8*w);
void PokedexApp_DrawSearchNamePage(u8*w){u16 i;ov18_021E613C(w,0);FillWindowPixelBuffer(w+0x41c,0);FillWindowPixelBuffer(w+0x43c,0);PokedexApp_PrintMessage(w+0x41c,P(w,0x65c),27,24,0,0,0x20100,2);
 for(i=0;i<27;i++){u16 n=ov18_021E7698(i);int msg;if(i==26)msg=113;else msg=i+69;int rem=n%7;PokedexApp_PrintMessage(w+0x43c,P(w,0x65c),msg,rem*32+24,(n/7)*32,4,0x20100,2);}
 PokedexApp_DrawSearchButton(w,17,39);PokedexApp_DrawSearchButton(w,19,40);PokedexApp_DrawSearchNameValue(w,66);ScheduleWindowCopyToVram(w+0x41c);ScheduleWindowCopyToVram(w+0x43c);ScheduleWindowCopyToVram(w+0x42c);
}
