#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8*)(p)+(o)))
#define U(p,o) (*(u32 *)((u8*)(p)+(o)))
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
extern void*WaitingIcon_New(void*,int);void ov72_0223A420(void*);
void ov72_0223A420(void*w){if(!P(w,0xf4c))P(w,0xf4c)=WaitingIcon_New((u8*)w+0xe18,1);}
