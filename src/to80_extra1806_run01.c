#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8*)(p)+(o)))
#define U(p,o) (*(u32 *)((u8*)(p)+(o)))
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
void*ov96_021E8B8C(void*,int);
void*ov96_021E8B8C(void*w,int i){if(i>=H(w,4))GF_AssertFail();return (u8*)P(w,0x154)+44*i;}
