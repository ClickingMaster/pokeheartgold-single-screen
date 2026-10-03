#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8*)(p)+(o)))
#define U(p,o) (*(u32 *)((u8*)(p)+(o)))
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
void ov68_021E7BC8(int);
void ov68_021E7BC8(int on){if(on==1)G2x_SetBlendBrightness_((void*)0x04001050,30,-8);else *(volatile u16*)0x04001050=0;}
