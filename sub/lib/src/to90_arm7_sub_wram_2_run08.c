#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))
extern void SPI_SendWait_03801D48(u16);
u8 PMi_GetRegister(u16 reg);
// @fn PMi_GetRegister
u8 PMi_GetRegister(u16 reg){while(H(0x040001c0)&128){}H(0x040001c0)=0x8202;H(0x040001c0)=0x8802;SPI_SendWait_03801D48((u8)(reg|128));H(0x040001c0)=0x8002;H(0x040001c2)=0;while(H(0x040001c0)&128){}return (u8)H(0x040001c2);}
