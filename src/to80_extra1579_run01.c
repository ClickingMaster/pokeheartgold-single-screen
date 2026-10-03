#pragma force_active on
#pragma force_active on
#include "global.h"
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
void*ov96_021E60D8(u8*,int,int);
void*ov96_021E60D8(u8*p,int a,int b){if(a>=4)GF_AssertFail();if(b>=3)GF_AssertFail();return p+1016+a*124+b*40;}
