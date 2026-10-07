#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))
extern u8 PMi_GetRegister(u16);extern void PMi_SetRegister(u16,u32);
void PMi_SetControl(u32 mask);
// @fn PMi_SetControl
void PMi_SetControl(u32 mask){u32 value=PMi_GetRegister(0);value |= mask;PMi_SetRegister(0,value);}
