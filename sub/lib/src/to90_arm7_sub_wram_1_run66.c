#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int PXI_SendWordByFifo(int, u32, int);
void AlarmHandler(void *arg);

void AlarmHandler(void *arg) {
    while (PXI_SendWordByFifo(7, (u32)arg, 0) < 0) {}
}
