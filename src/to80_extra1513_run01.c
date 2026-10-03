#pragma force_active on
#pragma force_active on
#include "global.h"
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
extern void ov14_021F6654(void*,int),ov14_021F2A18(void*,int,int),ov14_021F3488(void*,int,int),ov14_021F1090(void*,int);void ov14_021F2734(u8*);
void ov14_021F2734(u8*p){ov14_021F6654(V(p,52),39);ov14_021F2A18(V(p,52),9,0);ov14_021F3488(p,1,1);ov14_021F1090(p,63);}
