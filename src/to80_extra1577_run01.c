#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define PTR(p,o) ((void*)W(p,o))
void ov96_0221A61C(float);
void ov96_0221A640(u32 n);
void ov96_0221A640(u32 n){ov96_0221A61C((float)(11500.0/(double)((float)n/30.0f)));}
