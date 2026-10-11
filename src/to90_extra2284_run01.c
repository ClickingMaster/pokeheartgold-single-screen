#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,n) (*(void**)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define SW(p,n) (*(s32*)((u8*)(p)+(n)))
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define B(p,n) (*(u8*)((u8*)(p)+(n)))
void PrintUIntOnWindow(void*,u32,u32,u32,void*,u32,u32);void sub_0200CDAC(void*,u32,void*,u32,u32);void ScheduleWindowCopyToVram(void*);void BattlePartyMenu_PrintMonHP(void*,int,int,u32,u8);
void BattlePartyMenu_PrintMonHP(void*s,int win,int slot,u32 x,u8 y){u8*p=(u8*)s+4+slot*80;u32 off=win*16;PrintUIntOnWindow(P(s,0x1fa4),H(p,0x10),3,1,(u8*)P(s,0x2070)+off,x,y);sub_0200CDAC(P(s,0x1fa4),0,(u8*)P(s,0x2070)+off,x+24,y);x+=32;PrintUIntOnWindow(P(s,0x1fa4),H(p,0x12),3,0,(u8*)P(s,0x2070)+off,x,y);ScheduleWindowCopyToVram((u8*)P(s,0x2070)+off);}
