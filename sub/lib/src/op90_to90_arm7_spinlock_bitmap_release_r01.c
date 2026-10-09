#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void OS_ReleaseLockID(u16 lockID);
// @fn OS_ReleaseLockID
asm void OS_ReleaseLockID(u16 lockID)
{
    ldr r3, =0x027fffb8
    cmp r0, #160
    addpl r3, r3, #4
    subpl r0, r0, #160
    submi r0, r0, #128
    mov r1, #0x80000000
    mov r1, r1, lsr r0
    ldr r2, [r3]
    orr r2, r2, r1
    str r2, [r3]
    bx lr
}
