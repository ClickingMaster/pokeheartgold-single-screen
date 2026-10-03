#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8*)(p)+(o)))
#define U(p,o) (*(u32 *)((u8*)(p)+(o)))
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
void ov45_0222AB0C(void*,void*);
typedef struct{u32 a,b;}Pair;
void ov45_0222AB0C(void*w,void*out){*(Pair*)out=*(Pair*)((u8*)w+0x8c);}
