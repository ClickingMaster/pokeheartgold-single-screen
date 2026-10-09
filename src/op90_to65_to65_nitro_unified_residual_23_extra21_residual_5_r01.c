#pragma force_active on
#include <nitro/types.h>
extern u32 _021E36F0;
void MicWaitBusy(void);

#include <nitro/code32.h>

// @fn MicWaitBusy
asm void MicWaitBusy(void) {
    ldr r12, =_021E36F0
@L020DAD10:
    ldr r0, [r12]
    cmp r0, #1
    beq @L020DAD10
    bx lr
}
