#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))
extern void (*OS_IRQTable[25])(void);
void MIC_SetIrqFunction(u32 mask,void (*handler)(void));
// @fn MIC_SetIrqFunction
void MIC_SetIrqFunction(u32 mask,void (*handler)(void)){int i;for(i=0;i<25;i++){if(mask&1)OS_IRQTable[i]=handler;mask>>=1;}}
