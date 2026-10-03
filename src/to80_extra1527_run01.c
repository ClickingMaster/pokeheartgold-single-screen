#pragma force_active on
#pragma force_active on
#include "global.h"
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
extern u8*_022577C0;extern int ov45_0222F7B4(void*);int ov45_0222ED7C(void);
int ov45_0222ED7C(void){if(!_022577C0)GF_AssertFail();int result=ov45_0222F7B4(_022577C0);if(result)B(_022577C0,0x130)=1;return result;}
