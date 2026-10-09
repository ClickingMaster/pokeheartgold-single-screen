#pragma force_active on
#include <nitro/types.h>
#include <nitro/code16.h>
void SVC_SoftReset(void);
void SVC_WaitByLoop(s32 count);
void SVC_WaitIntr(BOOL clear, u32 mask);
// @fn SVC_SoftReset
asm void SVC_SoftReset(void)
{
    swi 0
    bx lr
}
// @fn SVC_WaitByLoop
asm void SVC_WaitByLoop(s32 count)
{
    swi 3
    bx lr
}
// @fn SVC_WaitIntr
asm void SVC_WaitIntr(BOOL clear, u32 mask)
{
    ldr r2, [pc, #8]
    mov r12, r2
    mov r2, #0
    swi 4
    bx lr
    lsl r0, r0, #0
    dcd 0x04000000
}
