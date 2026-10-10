#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
int PXI_SendWordByFifo(u32, u32, int);
void RtcReturnResult(u32 command, u32 status);

void RtcReturnResult(u32 command, u32 status) {
    u32 packet = ((command << 8) & 0x7f00) | 0x8000 | (status & 255);
    while (PXI_SendWordByFifo(5, packet, 0) < 0) {}
}
