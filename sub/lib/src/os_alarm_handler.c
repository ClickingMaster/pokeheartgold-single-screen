#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Timer interrupt handler of the OS alarm system (between the two parts of the alarm module, which
// tools/arm7-alarm-partition builds).

void OSi_ArrangeTimer(void);
void OSi_AlarmHandler(void *arg);

#pragma force_active on

// clang-format off
asm void OSi_AlarmHandler(void *arg)
{
    stmfd sp!, {r0, lr}
    bl OSi_ArrangeTimer
    ldmfd sp!, {r0, lr}
    bx lr
}
// clang-format on
