#pragma force_active on
#pragma force_active on
#include "global.h"
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
extern void ov74_02236070(void);void ov74_02236074(void);
void ov74_02236074(void){volatile u16*ime=(volatile u16*)0x04000208;u16 old=*ime;*ime=0;OS_SetIrqFunction(8192,ov74_02236070);OS_EnableIrqMask(8192);old=*ime;*ime=1;(void)old;}
