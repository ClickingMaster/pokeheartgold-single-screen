#pragma force_active on
#include <nitro/types.h>

void DGTi_hash2_arm4_small(register void *context, register const void *data, register u32 length);

#include <nitro/code32.h>

// @fn DGTi_hash2_arm4_small
asm void DGTi_hash2_arm4_small(register void *context, register const void *data, register u32 length) {
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, r12, lr}
    ldmia r0, {r3, r9, r10, r11, r12}
    sub sp, sp, #0x84
    str r2, [sp, #0x80]
@L020D9984:
    ldr r8, [pc, #-40] // _020D9964: .word 0x5A827999 (asm data immediately before this function)
    ldr r7, [pc, #-48] // _020D9960: .word 0x00FF00FF (asm data immediately before this function)
    mov r6, sp
    mov r5, #0
@L020D9994:
    ldr r4, [r1], #4
    add r2, r8, r12
    add r2, r2, r3, ror #27
    and lr, r4, r7
    and r4, r7, r4, ror #24
    orr r4, r4, lr, ror #8
    str r4, [r6, #0x40]
    str r4, [r6], #4
    add r2, r2, r4
    eor r4, r10, r11
    and r4, r4, r9
    eor r4, r4, r11
    add r2, r2, r4
    mov r9, r9, ror #2
    mov r12, r11
    mov r11, r10
    mov r10, r9
    mov r9, r3
    mov r3, r2
    add r5, r5, #4
    cmp r5, #0x40
    blt @L020D9994
    mov r7, #0
    mov r6, sp
@L020D99F4:
    ldr r2, [r6]
    ldr r5, [r6, #8]
    ldr r4, [r6, #0x20]
    ldr lr, [r6, #0x34]
    eor r2, r2, r5
    eor r4, r4, lr
    eor r2, r2, r4
    mov r2, r2, ror #0x1f
    str r2, [r6, #0x40]
    str r2, [r6], #4
    add r2, r2, r12
    add r2, r2, r8
    add r2, r2, r3, ror #27
    eor r4, r10, r11
    and r4, r4, r9
    eor r4, r4, r11
    add r2, r2, r4
    mov r9, r9, ror #2
    mov r12, r11
    mov r11, r10
    mov r10, r9
    mov r9, r3
    mov r3, r2
    add r7, r7, #4
    cmp r7, #0x10
    blt @L020D99F4
    ldr r8, [pc, #-252] // _020D9968: .word 0x6ED9EBA1 (asm data immediately before this function)
    mov r7, #0
@L020D9A64:
    ldr r2, [r6]
    ldr r4, [r6, #8]
    ldr lr, [r6, #0x20]
    ldr r5, [r6, #0x34]
    eor r2, r2, r4
    eor lr, lr, r5
    eor r2, r2, lr
    mov r2, r2, ror #0x1f
    str r2, [r6, #0x40]
    str r2, [r6], #4
    add r2, r2, r12
    add r2, r2, r8
    add r2, r2, r3, ror #27
    eor lr, r9, r10
    eor lr, lr, r11
    add r2, r2, lr
    mov r9, r9, ror #2
    mov r12, r11
    mov r11, r10
    mov r10, r9
    mov r9, r3
    mov r3, r2
    add r7, r7, #1
    cmp r7, #0xc
    moveq r6, sp
    cmp r7, #0x14
    blt @L020D9A64
    ldr r8, [pc, #-364] // _020D996C: .word 0x8F1BBCDC (asm data immediately before this function)
    mov r7, #0
@L020D9AD8:
    ldr r2, [r6]
    ldr lr, [r6, #8]
    ldr r5, [r6, #0x20]
    ldr r4, [r6, #0x34]
    eor r2, r2, lr
    eor r5, r5, r4
    eor r2, r2, r5
    mov r2, r2, ror #0x1f
    str r2, [r6, #0x40]
    str r2, [r6], #4
    add r2, r2, r12
    add r2, r2, r8
    add r2, r2, r3, ror #27
    orr r5, r9, r10
    and r5, r5, r11
    and r4, r9, r10
    orr r5, r5, r4
    add r2, r2, r5
    mov r9, r9, ror #2
    mov r12, r11
    mov r11, r10
    mov r10, r9
    mov r9, r3
    mov r3, r2
    add r7, r7, #1
    cmp r7, #8
    moveq r6, sp
    cmp r7, #0x14
    blt @L020D9AD8
    ldr r8, [pc, #-484] // _020D9970: .word 0xCA62C1D6 (asm data immediately before this function)
    mov r7, #0
@L020D9B54:
    ldr r2, [r6]
    ldr r5, [r6, #8]
    ldr r4, [r6, #0x20]
    ldr lr, [r6, #0x34]
    eor r2, r2, r5
    eor r4, r4, lr
    eor r2, r2, r4
    mov r2, r2, ror #0x1f
    str r2, [r6, #0x40]
    str r2, [r6], #4
    add r2, r2, r12
    add r2, r2, r8
    add r2, r2, r3, ror #27
    eor r4, r9, r10
    eor r4, r4, r11
    add r2, r2, r4
    mov r9, r9, ror #2
    mov r12, r11
    mov r11, r10
    mov r10, r9
    mov r9, r3
    mov r3, r2
    add r7, r7, #1
    cmp r7, #4
    moveq r6, sp
    cmp r7, #0x14
    blt @L020D9B54
    ldmia r0, {r2, r4, r6, r7, lr}
    add r3, r3, r2
    add r9, r9, r4
    add r10, r10, r6
    add r11, r11, r7
    add r12, r12, lr
    stmia r0, {r3, r9, r10, r11, r12}
    ldr lr, [sp, #0x80]
    subs lr, lr, #0x40
    str lr, [sp, #0x80]
    bgt @L020D9984
    add sp, sp, #0x84
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, r12, pc}
}
