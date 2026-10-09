#pragma force_active on
#include <nitro/fx/fx.h>
void MTX_Scale43_(register MtxFx43 *pDst, register fx32 x, register fx32 y, register fx32 z);

#include <nitro/code16.h>

// @fn MTX_Scale43_
asm void MTX_Scale43_(register MtxFx43 *pDst, register fx32 x, register fx32 y, register fx32 z) {
    stmia r0!, {r1}
    mov r1, #0
    str r3, [r0, #0x1c]
    mov r3, #0
    stmia r0!, {r1, r3}
    stmia r0!, {r1, r2, r3}
    mov r2, #0
    stmia r0!, {r1, r3}
    add r0, #4
    stmia r0!, {r1, r2, r3}
    bx lr
}
