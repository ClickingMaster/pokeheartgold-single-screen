#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define PTR(p,o) ((void*)W(p,o))
void *Gts_GetWifiIconLevel(void*);void sub_0203A930(void*);extern int (*ov70_02246780[])(void*);
int ov70_02244124(void*p);
int ov70_02244124(void*p){int old;int result;sub_0203A930(Gts_GetWifiIconLevel(p));old=W(p,44);result=ov70_02246780[old](p);if(old!=W(p,44)){H(p,5632)=0;H(p,5634)=0;}return result;}
