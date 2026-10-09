#pragma force_active on
#include <nitro/fx/fx.h>
void MTX_Identity44_(register MtxFx44 *pDst);
void MTX_Copy44To43_(register const MtxFx44 *pSrc, register MtxFx43 *pDst);

#include <nitro/code32.h>

// @fn MTX_Identity44_
asm void MTX_Identity44_(register MtxFx44 *pDst) {
    mov r2, #0x1000
    mov r3, #0
    stmia r0!, {r2, r3}
    mov r1, #0
    stmia r0!, {r1, r3}
    stmia r0!, {r1, r2, r3}
    stmia r0!, {r1, r3}
    stmia r0!, {r1, r2, r3}
    stmia r0!, {r1, r3}
    stmia r0!, {r1, r2}
    bx lr
}

// @fn MTX_Copy44To43_
asm void MTX_Copy44To43_(register const MtxFx44 *pSrc, register MtxFx43 *pDst) {
    ldmia r0!, {r2, r3, r12}
    add r0, r0, #4
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    add r0, r0, #4
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    add r0, r0, #4
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    add r0, r0, #4
    stmia r1!, {r2, r3, r12}
    bx lr
}
