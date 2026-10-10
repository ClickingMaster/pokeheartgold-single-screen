#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))
extern int PXI_SendWordByFifo(int, u32, int);
void PMi_SendPxiCommand(u32 a, u32 b, u32 c);

void PMi_SendPxiCommand(u32 a, u32 b, u32 c) {
    u32 word = ((a & 0x3c00000u) << 22) | ((b & 0x3f0000u) << 16) | (u16)c;
    while (PXI_SendWordByFifo(8, word, 0) != 0) {}
}
