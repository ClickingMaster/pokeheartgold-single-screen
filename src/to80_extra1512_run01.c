#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define PTR(p,o) ((void*)W(p,o))
void ov14_021E7D9C(void*,int);void sub_020196E8(void*,int,int,int);
void ov14_021E7E10(void*p,int bit);
void ov14_021E7E10(void*p,int bit){void*q=PTR(p,52);W(q,0x40c0)^=1<<bit;ov14_021E7D9C(p,bit);sub_020196E8(PTR(PTR(p,52),752),2,21,6);}
