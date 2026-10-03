#pragma force_active on
#pragma force_active on
#include "global.h"
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
extern u8*_0221A680;void ov00_021E6FE0(int);
void ov00_021E6FE0(int n){void(*cb)(u16,void*);I(_0221A680,0x1098)=n;cb=V(_0221A680,0xfb4);if(cb)cb((u16)n,V(_0221A680,0xfb8));}
