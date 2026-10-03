#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define PTR(p,o) ((void*)W(p,o))
void *ov42_02228188(void*,int);void *ov42_022283BC(void*,u32,void*);BOOL ov49_02258D70(void*,void*);
BOOL ov49_02258F40(void*p,void*q);
BOOL ov49_02258F40(void*p,void*q){void*x=ov42_02228188(PTR(q,0),6);x=ov42_022283BC(PTR(q,0),W(p,0),x);if(x==NULL){return FALSE;}else{x=ov42_02228188(x,4);return ov49_02258D70(p,x);}}
