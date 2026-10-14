#include <nitro/types.h>

#include <nitro/pxi/fifo_shared.h>

#include "wmsp_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Returns a WM result buffer to the ARM9 and releases it (placed in WRAM).

#include <nitro/wram_begin.h>

s32 PXI_SendWordByFifo(s32 tag, u32 data, BOOL error);
void VENEER_SVC_WaitByLoop(s32 count);

#pragma force_active on

void WMSP_ReturnResult2Wm9(void *result) {
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_WM, (u32)result, FALSE) < 0) {
        VENEER_SVC_WaitByLoop(0x100);
    }
    OS_UnlockMutex(&wmspW.mutex);
}

#include <nitro/wram_end.h>
