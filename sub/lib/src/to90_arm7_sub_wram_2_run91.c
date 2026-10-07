#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u16 nvramw[];
void NVRAM_Init(void);
// @fn NVRAM_Init
void NVRAM_Init(void){int i;for(i=0;i<16;i++)nvramw[i]=0;}
