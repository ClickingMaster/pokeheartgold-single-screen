#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8*)(p)+(o)))
#define U(p,o) (*(u32 *)((u8*)(p)+(o)))
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
extern void ov83_02247568(void*,int,int),ov83_022475D4(void*,int);void ov83_02242AC0(void*,int,int,int);
void ov83_02242AC0(void*w,int a,int b,int c){ov83_02247568(P(w,0x77c),a,b);ov83_022475D4(P(w,0x77c),c);}
