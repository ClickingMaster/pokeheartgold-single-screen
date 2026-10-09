#pragma force_active on
#include <nitro/fx/fx.h>
void MTX_RotY33_(register MtxFx33 *pDst, register fx32 sinVal, register fx32 cosVal);
void MTX_RotZ33_(register MtxFx33 *pDst, register fx32 sinVal, register fx32 cosVal);

#include <nitro/code16.h>

// @fn MTX_RotY33_
asm void MTX_RotY33_(register MtxFx33 *pDst, register fx32 sinVal, register fx32 cosVal) {
    str r2, [r0]
    str r2, [r0, #0x20]
    mov r3, #0
    str r3, [r0, #4]
    str r3, [r0, #0xc]
    str r3, [r0, #0x14]
    str r3, [r0, #0x1c]
    neg r2, r1
    mov r3, #1
    lsl r3, r3, #0xc
    str r1, [r0, #0x18]
    str r2, [r0, #8]
    str r3, [r0, #0x10]
    bx lr
}

// @fn MTX_RotZ33_
asm void MTX_RotZ33_(register MtxFx33 *pDst, register fx32 sinVal, register fx32 cosVal) {
    stmia r0!, {r2}
    mov r3, #0
    stmia r0!, {r1, r3}
    neg r1, r1
    stmia r0!, {r1, r2}
    mov r1, #1
    lsl r1, r1, #0xc
    str r3, [r0]
    str r3, [r0, #4]
    str r3, [r0, #8]
    str r1, [r0, #0xc]
    bx lr
}
