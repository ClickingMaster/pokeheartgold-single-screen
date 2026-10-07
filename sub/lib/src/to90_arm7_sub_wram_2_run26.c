#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on

void SPI_SendWait_03803D98(u32 data);
// @fn SPI_SendWait_03803D98
void SPI_SendWait_03803D98(u32 data){*(volatile u16*)0x40001c2=data&255;while(*(volatile u16*)0x40001c0&128){}}
