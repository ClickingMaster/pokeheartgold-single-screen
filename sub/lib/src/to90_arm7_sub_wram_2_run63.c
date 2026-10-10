#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <nitro/pxi/fifo.h>
extern void WvrPxiReceiveCallback(PXIFifoTag, u32, BOOL);
extern void WvrPxiWmSubstituteCallback(PXIFifoTag, u32, BOOL);
extern void MI_CpuFill8(void *, int, u32);
extern u8 WVR_BSS_START[];
void WVR_Init(s32 heapHandle);

void WVR_Init(s32 heapHandle) {
    *(s32 *)(WVR_BSS_START + 8) = heapHandle;
    *WVR_BSS_START = 1;
    MI_CpuFill8(WVR_BSS_START + 16, 0, 0xa4);
    PXI_SetFifoRecvCallback(15, WvrPxiReceiveCallback);
    PXI_SetFifoRecvCallback(10, WvrPxiWmSubstituteCallback);
}
