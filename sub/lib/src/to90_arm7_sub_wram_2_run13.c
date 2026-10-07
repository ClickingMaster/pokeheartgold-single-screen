#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))

u8 SPI_DummyWaitReceive(void);
// @fn SPI_DummyWaitReceive
u8 SPI_DummyWaitReceive(void){H(0x040001c2)=0;while(H(0x040001c0)&128){}return (u8)H(0x040001c2);}
