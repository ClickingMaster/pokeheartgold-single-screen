#pragma force_active on
#pragma force_active on
#include "global.h"
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
u32 ov48_02259B10(u8*,int,int);
u32 ov48_02259B10(u8*p,int a,int b){int i=0;u32 count=U(p,0);for(;(u32)i<count;i++,p+=48){if(a==H(p,46)&&b==H(p,48))return i;}return count;}
