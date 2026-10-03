#pragma force_active on
#pragma force_active on
#include "global.h"
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
extern u8*ov39_0222A2B4(void*);extern int ov40_02244B70(void*,int,int,void*,void*);int ov39_02227B24(u8*,u8*);
int ov39_02227B24(u8*p,u8*q){u8*r=ov39_0222A2B4(p);int result=ov40_02244B70(V(p,0),I(r,4),I(r,8),q+4,q+6);return (u32)(result-2)<=1;}
