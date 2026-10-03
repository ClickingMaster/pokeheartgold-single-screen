#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define PTR(p,o) ((void*)W(p,o))
extern void *_022577C0;void ov45_02231EC0(void);int ov45_0222F74C(void);
int ov45_0222EB94(void);
int ov45_0222EB94(void){if(!_022577C0)GF_AssertFail();ov45_02231EC0();{int result=ov45_0222F74C();if(result==1)B(_022577C0,302)=3;return result;}}
