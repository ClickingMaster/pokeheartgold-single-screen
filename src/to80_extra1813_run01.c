#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8*)(p)+(o)))
#define U(p,o) (*(u32 *)((u8*)(p)+(o)))
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
int ov96_021FC164(void*,int,int);
int ov96_021FC164(void*w,int row,int index){index/=8;if(!((u8(*)[512])((u8*)w+0x618))[row][index])index=-1;return index;}
