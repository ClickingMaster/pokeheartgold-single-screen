#pragma force_active on
#pragma force_active on
#include "global.h"
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
extern int ov75_02249534(void*);int ov75_02249478(u8*);
int ov75_02249478(u8*p){if(!ov75_02249534(V(p,68))){if(I(p,144)>30)I(p,8)=I(p,12);I(p,144)++;}return 0;}
