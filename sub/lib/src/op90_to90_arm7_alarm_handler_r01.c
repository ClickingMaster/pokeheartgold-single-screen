#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void OSi_ArrangeTimer(void);
void OSi_AlarmHandler(void *arg);
// @fn OSi_AlarmHandler
asm void OSi_AlarmHandler(void *arg)
{
    stmfd sp!, {r0, lr}
    bl OSi_ArrangeTimer
    ldmfd sp!, {r0, lr}
    bx lr
}
