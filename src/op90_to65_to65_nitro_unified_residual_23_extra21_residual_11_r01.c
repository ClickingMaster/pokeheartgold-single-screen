#pragma force_active on
#include <nitro/types.h>
extern u32 _021E3784;
void RtcWaitBusy(void);

#include <nitro/code32.h>

// @fn RtcWaitBusy
asm void RtcWaitBusy(void) {
    ldr r12, =_021E3784
@L020DC208:
    ldr r0, [r12]
    cmp r0, #1
    beq @L020DC208
    bx lr
}
