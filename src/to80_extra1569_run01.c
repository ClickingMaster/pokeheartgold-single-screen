#pragma force_active on
#pragma force_active on
#include "global.h"
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
extern void ov86_021E64E0(u8*);void ov86_021E7DA8(u8*);
void ov86_021E7DA8(u8*p){u16*table=V(p+H(p,600)*8,608);H(p,8)=table[H(p,602)+*(s8*)(p+604)*8];ov86_021E64E0(p);}
