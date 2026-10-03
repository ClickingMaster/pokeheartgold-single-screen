#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8*)(p)+(o)))
#define U(p,o) (*(u32 *)((u8*)(p)+(o)))
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
extern void*sub_0209686C(void*,void*);void ov80_02239914(void*,void*,u32,u32,const u16*);
typedef struct{u16 entries[15];}Entries;
void ov80_02239914(void*a,void*b,u32 x,u32 y,const u16*src){u8*w=sub_0209686C(a,b);U(w,0)=x;U(w,4)=y;*(Entries*)(w+8)=*(const Entries*)src;}
