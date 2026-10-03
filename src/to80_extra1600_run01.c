#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define PTR(p,o) ((void*)W(p,o))
int Pokepic_IsActive(void*);
void *ov07_0221FA48(void*p,int i);
void *ov07_0221FA48(void*p,int i){void*q=PTR((u8*)PTR(p,192)+i*4,196);if(q==0)return 0;if(Pokepic_IsActive(q))return PTR((u8*)PTR(p,192)+i*4,196);return 0;}
