#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on

void NVRAM_WriteEnable(void);
// @fn NVRAM_WriteEnable
void NVRAM_WriteEnable(void){while(*(volatile u16*)0x40001c0&128){}*(volatile u16*)0x40001c0=0x8100;*(volatile u16*)0x40001c2=6;while(*(volatile u16*)0x40001c0&128){}}
