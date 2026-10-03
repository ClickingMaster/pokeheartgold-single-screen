#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8*)(p)+(o)))
#define U(p,o) (*(u32 *)((u8*)(p)+(o)))
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#include "system.h"
extern void ov91_02260CE0(void*,int,int);void ov91_02260CC8(void*);
void ov91_02260CC8(void*w){if(P(w,0))ov91_02260CE0(w,gSystem.touchX,gSystem.touchY);}
