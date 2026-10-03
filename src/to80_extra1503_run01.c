#pragma force_active on
#pragma force_active on
#include "global.h"
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
extern u8*sub_0205F1A0(void*);extern u8*ov01_021FC5A4(void*,int);void ov01_022057DC(void*);
void ov01_022057DC(void*p){u8*q=ov01_021FC5A4(V(sub_0205F1A0(p),252),22);int count=I(q,0),i;u8*flags=(u8*)((u16*)(q+4)+count);flags+=count;for(i=0;i<count;i++)flags[i]=1;}
