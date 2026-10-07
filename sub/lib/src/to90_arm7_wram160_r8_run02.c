#pragma force_active on
#include <nitro/types.h>
#include <nitro/os/mutex.h>
#include <nitro/pxi/fifo.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#include <nitro/wram_begin.h>
extern u8 wmspW[];
void VENEER_SVC_WaitByLoop(s32 count);
void WMSP_ReturnResult2Wm9(void *buffer);
u16 *WMSP_GetBuffer4Callback2Wm9(void);
// @fn WMSP_GetBuffer4Callback2Wm9
u16 *WMSP_GetBuffer4Callback2Wm9(void) {
    volatile u16 *flag = (volatile u16 *)0x027fff96;
    u16 value;
    OS_LockMutex((OSMutex *)(wmspW + 0x1528));
    while (((value = *flag) & 1) != 0)
        VENEER_SVC_WaitByLoop(0x100);
    *flag = value | 1;
    return (u16 *)(*(u32 *)((u8 *)(*(u32 *)(wmspW + 0x154c)) + 8));
}
#include <nitro/wram_end.h>
