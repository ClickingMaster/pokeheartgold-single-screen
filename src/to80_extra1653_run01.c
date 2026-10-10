#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern void Callback_NVRAM(PXIFifoTag, u32, BOOL);
extern int NVRAMm_ExecuteCommand(int, u32, u16, void *);
int readNvram(u32, u32, void *);

int readNvram(u32 address, u32 length, void *dst) {
    DC_InvalidateRange(dst, length);
    while (!PXI_IsCallbackReady(4, PXI_PROC_ARM7)) {}
    PXI_SetFifoRecvCallback(4, Callback_NVRAM);
    for (;;) {
        if (NVRAMm_ExecuteCommand(1, address, (u16)length, dst) == 1) {
            break;
        }
        SVC_WaitByLoop(0x40000);
    }
    DC_InvalidateRange(dst, length);
    return 1;
}
