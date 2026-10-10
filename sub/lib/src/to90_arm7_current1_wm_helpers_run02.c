#pragma force_active on
#include <nitro/os/thread.h>
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u8 wmspRequestThread[];
typedef char W40803ThreadSize[(sizeof(OSThread) == 0xa4) ? 1 : -1];
OSThread *WMSP_GetRequestThread(void);

OSThread *WMSP_GetRequestThread(void) {
    return (OSThread *)wmspRequestThread;
}
