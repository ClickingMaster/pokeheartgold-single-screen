#include <nitro/types.h>

// The code below is compiled as Thumb (16-bit) code.
#include <nitro/code16.h>

// BIOS calls (SWI) of the ARM7.

void SVC_SoftReset(void);
void SVC_WaitByLoop(s32 count);
void SVC_WaitIntr(BOOL clear, u32 mask);

#pragma force_active on

// clang-format off
asm void SVC_SoftReset(void)
{
    swi 0
    bx lr
}
// clang-format on

// clang-format off
asm void SVC_WaitByLoop(s32 count)
{
    swi 3
    bx lr
}
// clang-format on

// r12 = the I/O register base (0x04000000), r2 = 0.
// clang-format off
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
// clang-format on
