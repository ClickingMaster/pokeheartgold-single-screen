#pragma force_active on
#include <nitro/types.h>
void GX_SendFifo48B(register const void *pSrc, register void *pDest);
void GX_SendFifo64B(register const void *pSrc, register void *pDest);

#include <nitro/code32.h>

// @fn GX_SendFifo48B
asm void GX_SendFifo48B(register const void *pSrc, register void *pDest) {
    ldmia r0!, {r2, r3, r12}
    stmia r1, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1, {r2, r3, r12}
    bx lr
}

// @fn GX_SendFifo64B
asm void GX_SendFifo64B(register const void *pSrc, register void *pDest) {
    stmdb sp!, {r4, r5, r6, r7, r8}
    ldmia r0!, {r2, r3, r4, r5, r6, r7, r8, r12}
    stmia r1, {r2, r3, r4, r5, r6, r7, r8, r12}
    ldmia r0!, {r2, r3, r4, r5, r6, r7, r8, r12}
    stmia r1, {r2, r3, r4, r5, r6, r7, r8, r12}
    ldmia sp!, {r4, r5, r6, r7, r8}
    bx lr
}
