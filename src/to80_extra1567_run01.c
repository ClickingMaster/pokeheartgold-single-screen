#pragma force_active on
#pragma force_active on
#include "global.h"
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
extern int sub_0203769C(void);void ov85_021E8680(u8*,volatile u16*);
void ov85_021E8680(u8*p,volatile u16*q){int offset=q[0]*4;H(p+offset,112)=q[0];H(p+offset,114)=q[1];if(q[1]==sub_0203769C())I(p,44)=q[0];}
