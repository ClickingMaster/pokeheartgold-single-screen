#include <nitro/types.h>

#include "wmsp_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Takes the WM result buffer shared with the ARM9 (placed in WRAM).

#include <nitro/wram_begin.h>

#define ARM9_RESULT_BUSY (*(volatile u16 *)0x027FFF96) // bit 0: the result buffer is in use

void VENEER_SVC_WaitByLoop(s32 count);

#pragma force_active on

u16 *WMSP_GetBuffer4Callback2Wm9(void) {
    volatile u16 *flag = &ARM9_RESULT_BUSY;
    u16 value;
    OS_LockMutex(&wmspW.mutex);
    while (((value = *flag) & 1) != 0) {
        VENEER_SVC_WaitByLoop(0x100);
    }
    *flag = value | 1;
    return (u16 *)wmspW.arm9Buf->unk_08;
}

#include <nitro/wram_end.h>
