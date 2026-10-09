#pragma force_active on
#include <nitro/types.h>
u32 WcmCountBits(u32 x);

#include <nitro/code32.h>

// @fn WcmCountBits
asm u32 WcmCountBits(u32 x) {
    mov r1, r0
    mov r0, #0
    mov r3, #1
@L020AEA24:
    clz r2, r1
    rsbs r2, r2, #0x1f
    bcc @L020AEA34
    b @L020AEA38
@L020AEA34:
    bx lr
@L020AEA38:
    bic r1, r1, r3, lsl r2
    add r0, r0, #1
    b @L020AEA24
}
