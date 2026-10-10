#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))
extern void MicTimerHandler(void);
void MIC_TimerHandler(void);

void MIC_TimerHandler(void) {
    MicTimerHandler();
    W(0x0380fff8) |= 64;
    W(0x04000214) = 64;
}
