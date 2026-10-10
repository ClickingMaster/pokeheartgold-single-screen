#pragma force_active on
#include <nitro/os/mutex.h>
#include <nitro/pxi/fifo.h>
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <nitro/wram_begin.h>
extern u8 wmspW[];
void VENEER_SVC_WaitByLoop(s32 count);
void WMSP_ReturnResult2Wm9(void *buffer);
u16 *WMSP_GetBuffer4Callback2Wm9(void);

void WMSP_ReturnResult2Wm9(void *buffer) {
    while (PXI_SendWordByFifo(10, (u32)buffer, 0) < 0) {
        VENEER_SVC_WaitByLoop(0x100);
    }
    OS_UnlockMutex((OSMutex *)(wmspW + 0x1528));
}

#include <nitro/wram_end.h>
