#pragma force_active on
#include "global.h"
#pragma force_active on
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define S(p,o) (*(s16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
s32 Party_GetCount(void*);void*Party_GetMonByIndex(void*,int);u32 GetMonData(void*,int,void*);u16 ov80_02237ED8(void*);BOOL ov80_02237D8C(int);void ov80_02237ADC(u32,u32,u16*,int);int ov80_02237B58(int,int);void FrontierTrainer_GenerateMons(u32,u16,u16,u16*,void*,u8*,void**,u8),ov80_02232838(u8*);
#pragma opt_movepostops off
#pragma opt_common_subs off
void ov80_02232838(u8*d){int count=(u8)Party_GetCount(P(d,40)),i,selected;u8*cursor;void*mon;int target;u32 value;i=0;if(count>0){cursor=d;do{mon=Party_GetMonByIndex(P(d,40),i);H(cursor,0x394)=GetMonData(mon,58,0);H(cursor,0x396)=GetMonData(mon,59,0);H(cursor,0x398)=GetMonData(mon,60,0);H(cursor,0x39a)=GetMonData(mon,61,0);i++;cursor+=8;}while(i<count);}ov80_02237ADC(B(d,16),ov80_02237ED8(d),(u16*)(d+48),14);selected=B(d,17);target=ov80_02237B58(B(d,16),1);value=ov80_02237D8C(B(d,16));FrontierTrainer_GenerateMons(target,H(d+selected*2,48),H(d+(selected+7)*2,48),(u16*)(d+0x26c),d+0x288,d+0x274,(void**)(d+0x278),(u8)value);}
