#pragma force_active on
#pragma force_active on
#include "global.h"
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
typedef struct{int a,b;} Pair;extern void*_022577C0;extern Pair*ov45_0222F954(void*,void*);void ov45_0222EA4C(void*,Pair*);
void ov45_0222EA4C(void*p,Pair*out){Pair*q;if(!_022577C0)GF_AssertFail();q=ov45_0222F954(_022577C0,p);*out=*q;}
