#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))
extern u8 PMi_GetRegister(u16);
extern void PMi_SetRegister(u16, u32);
void PMi_ResetControl(u32 mask);

void PMi_ResetControl(u32 mask) {
    u32 value = PMi_GetRegister(0);
    value &= ~mask;
    PMi_SetRegister(0, (u8)value);
}
