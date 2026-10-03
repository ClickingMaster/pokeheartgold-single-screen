#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8*)(p)+(o)))
#define U(p,o) (*(u32 *)((u8*)(p)+(o)))
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#include "system.h"
int ov112_021EBA8C(void);
int ov112_021EBA8C(void){if(gSystem.newKeys&1||gSystem.touchNew)return 3;return 2;}
