#pragma force_active on
#include <nitro/fx/fx.h>
void MTX_RotX43_(register MtxFx43 *pDst, register fx32 sinVal, register fx32 cosVal);
void MTX_RotY43_(register MtxFx43 *pDst, register fx32 sinVal, register fx32 cosVal);

#include <nitro/code16.h>

// @fn MTX_RotX43_
asm void MTX_RotX43_(register MtxFx43 *pDst, register fx32 sinVal, register fx32 cosVal) {
    str r1, [r0, #0x14]
    neg r1, r1
    str r1, [r0, #0x1c]
    mov r1, #1
    lsl r1, r1, #0xc
    stmia r0!, {r1}
    mov r3, #0
    mov r1, #0
    stmia r0!, {r1, r3}
    stmia r0!, {r1, r2}
    str r1, [r0, #4]
    add r0, #0xc
    stmia r0!, {r2, r3}
    stmia r0!, {r1, r3}
    bx lr
}

// @fn MTX_RotY43_
asm void MTX_RotY43_(register MtxFx43 *pDst, register fx32 sinVal, register fx32 cosVal) {
    str r1, [r0, #0x18]
    mov r3, #0
    stmia r0!, {r2, r3}
    neg r1, r1
    stmia r0!, {r1, r3}
    mov r1, #1
    lsl r1, r1, #0xc
    stmia r0!, {r1, r3}
    add r0, #4
    mov r1, #0
    stmia r0!, {r1, r2, r3}
    stmia r0!, {r1, r3}
    bx lr
}
