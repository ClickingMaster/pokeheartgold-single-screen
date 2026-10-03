#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8*)(p)+(o)))
#define U(p,o) (*(u32 *)((u8*)(p)+(o)))
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
u16 ov45_0222AAEC(void*,u32);
u16 ov45_0222AAEC(void*w,u32 i){u16 value;if(i>=2)GF_AssertFail();value=H((u8*)w+i*2,0x88);if(value>=18)value=0;return value;}
