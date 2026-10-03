#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8*)(p)+(o)))
#define U(p,o) (*(u32 *)((u8*)(p)+(o)))
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
extern void*ov74_02231154(void*);void ov74_02231164(void*);
void ov74_02231164(void*w){u8*p=ov74_02231154(w);MI_CpuFill8(p,0,0x1c4);H(p,0x1c0)=0x118;}
