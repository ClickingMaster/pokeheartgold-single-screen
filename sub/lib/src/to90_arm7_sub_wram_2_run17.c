#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on

void MI_SetCartridgeRomCycle1st(u32 cycle);
// @fn MI_SetCartridgeRomCycle1st
void MI_SetCartridgeRomCycle1st(u32 cycle){*(volatile u16*)0x4000204=(*(volatile u16*)0x4000204&~12)|(cycle<<2);}
