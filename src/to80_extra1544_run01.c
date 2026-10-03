#pragma force_active on
#pragma force_active on
#include "global.h"
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
extern int ov72_0223AF48(void*);int ov72_02239F04(u8*);
int ov72_02239F04(u8*p){int result=ov72_0223AF48(p+0x136c);if(result){if(result==2)I(p,28)=13;else{I(p,28)=41;I(V(p,0),32)=1;}}return 3;}
