#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))
extern void SPI_SendWait_03801D48(u16);
void PMi_SetRegister(u16 reg, u32 value);

void PMi_SetRegister(u16 reg, u32 value) {
    while (H(0x040001c0) & 128) {}
    H(0x040001c0) = 0x8202;
    H(0x040001c0) = 0x8802;
    SPI_SendWait_03801D48((u8)reg);
    H(0x040001c0) = 0x8002;
    H(0x040001c2) = (u8)value;
}
