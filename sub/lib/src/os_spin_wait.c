#include <nitro.h>

void OS_SpinWait(u32 cycles) {
    SVC_WaitByLoop((s32)cycles / 4);
}
