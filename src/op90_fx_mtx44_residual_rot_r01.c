#pragma force_active on
#include <nitro/fx/fx.h>
void MTX_RotX44_(register MtxFx44 *pDst, register fx32 sinVal, register fx32 cosVal);
void MTX_RotY44_(register MtxFx44 *pDst, register fx32 sinVal, register fx32 cosVal);
void MTX_RotZ44_(register MtxFx44 *pDst, register fx32 sinVal, register fx32 cosVal);

#include <nitro/code16.h>

// @fn MTX_RotX44_
asm void MTX_RotX44_(register MtxFx44 *pDst, register fx32 sinVal, register fx32 cosVal) {
    str r2, [r0, #0x14]
    str r2, [r0, #0x28]
    str r1, [r0, #0x18]
    neg r1, r1
    str r1, [r0, #0x24]
    mov r1, #1
    mov r2, #0
    lsl r1, r1, #0xc
    mov r3, #0
    stmia r0!, {r1, r2, r3}
    stmia r0!, {r2, r3}
    add r0, #8
    stmia r0!, {r2, r3}
    add r0, #8
    stmia r0!, {r2, r3}
    stmia r0!, {r2, r3}
    str r1, [r0]
    bx lr
}

// @fn MTX_RotY44_
asm void MTX_RotY44_(register MtxFx44 *pDst, register fx32 sinVal, register fx32 cosVal) {
    str r2, [r0]
    str r2, [r0, #0x28]
    str r1, [r0, #0x20]
    neg r1, r1
    str r1, [r0, #8]
    mov r3, #1
    mov r1, #0
    lsl r3, r3, #0xc
    mov r2, #0
    str r2, [r0, #4]
    add r0, #0xc
    stmia r0!, {r1, r2, r3}
    stmia r0!, {r1, r2}
    str r2, [r0, #4]
    add r0, #0xc
    stmia r0!, {r1, r2}
    stmia r0!, {r1, r2, r3}
    bx lr
}

// @fn MTX_RotZ44_
asm void MTX_RotZ44_(register MtxFx44 *pDst, register fx32 sinVal, register fx32 cosVal) {
    str r2, [r0]
    str r2, [r0, #0x14]
    str r1, [r0, #4]
    neg r1, r1
    str r1, [r0, #0x10]
    mov r3, #1
    mov r1, #0
    lsl r3, r3, #0xc
    mov r2, #0
    add r0, #8
    stmia r0!, {r1, r2}
    add r0, #8
    stmia r0!, {r1, r2}
    stmia r0!, {r1, r2, r3}
    stmia r0!, {r1, r2}
    stmia r0!, {r1, r2, r3}
    bx lr
}
