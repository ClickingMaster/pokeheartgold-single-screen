#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8*)(p)+(o)))
#define U(p,o) (*(u32 *)((u8*)(p)+(o)))
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
extern void BeginNormalPaletteFade(int,int,int,int,int,int,int);void ov70_022396A8(void*);
void ov70_022396A8(void*w){BeginNormalPaletteFade(3,1,1,0,6,1,61);U(w,0x2c)=3;}
