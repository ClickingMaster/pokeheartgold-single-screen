#pragma force_active on
#include <nitro/fx/fx.h>
void MTX_Identity43_(register MtxFx43 *pDst);
void MTX_Copy43To44_(register const MtxFx43 *pSrc, register MtxFx44 *pDst);

#include <nitro/code32.h>

// @fn MTX_Identity43_
asm void MTX_Identity43_(register MtxFx43 *pDst) {
    mov r2, #0x1000
    mov r3, #0
    stmia r0!, {r2, r3}
    mov r1, #0
    stmia r0!, {r1, r3}
    stmia r0!, {r2, r3}
    stmia r0!, {r1, r3}
    stmia r0!, {r2, r3}
    stmia r0!, {r1, r3}
    bx lr
}

// @fn MTX_Copy43To44_
asm void MTX_Copy43To44_(register const MtxFx43 *pSrc, register MtxFx44 *pDst) {
    stmdb sp!, {r4}
    mov r12, #0
    ldmia r0!, {r2, r3, r4}
    stmia r1!, {r2, r3, r4, r12}
    ldmia r0!, {r2, r3, r4}
    stmia r1!, {r2, r3, r4, r12}
    ldmia r0!, {r2, r3, r4}
    stmia r1!, {r2, r3, r4, r12}
    mov r12, #0x1000
    ldmia r0!, {r2, r3, r4}
    stmia r1!, {r2, r3, r4, r12}
    ldmia sp!, {r4}
    bx lr
}
