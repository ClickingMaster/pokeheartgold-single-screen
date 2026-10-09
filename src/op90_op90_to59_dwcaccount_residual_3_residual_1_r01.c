#pragma force_active on
#include <nitro/types.h>
void CRYPTO_RC4FastEncrypt(void *context, const void *in, u32 length, void *out);

#include <nitro/code32.h>

// @fn CRYPTO_RC4FastEncrypt
asm void CRYPTO_RC4FastEncrypt(void *context, const void *in, u32 length, void *out) {
    stmdb sp!, {r4, r5, r6, r7, r8, r9, lr}
    ldr r7, [r0]
    ldr r6, [r0, #4]
    add r0, r0, #8
    mov lr, #0x1000000
    add r7, r7, lr
    subs r2, r2, #8
    ldr r9, [r0, r7, lsr #22]
    bmi @L020A07B4
@L020A064C:
    add r6, r6, r9
    ldr r8, [r0, r6, lsr #22]
    ldrb r5, [r1], #1
    str r8, [r0, r7, lsr #22]
    str r9, [r0, r6, lsr #22]
    add r4, r9, r8
    add r7, r7, lr
    ldr r4, [r0, r4, lsr #22]
    ldr r9, [r0, r7, lsr #22]
    eor r5, r5, r4, lsr #24
    strb r5, [r3], #1
    add r6, r6, r9
    ldr r8, [r0, r6, lsr #22]
    ldrb r5, [r1], #1
    str r8, [r0, r7, lsr #22]
    str r9, [r0, r6, lsr #22]
    add r4, r9, r8
    add r7, r7, lr
    ldr r4, [r0, r4, lsr #22]
    ldr r9, [r0, r7, lsr #22]
    eor r5, r5, r4, lsr #24
    strb r5, [r3], #1
    add r6, r6, r9
    ldr r8, [r0, r6, lsr #22]
    ldrb r5, [r1], #1
    str r8, [r0, r7, lsr #22]
    str r9, [r0, r6, lsr #22]
    add r4, r9, r8
    add r7, r7, lr
    ldr r4, [r0, r4, lsr #22]
    ldr r9, [r0, r7, lsr #22]
    eor r5, r5, r4, lsr #24
    strb r5, [r3], #1
    add r6, r6, r9
    ldr r8, [r0, r6, lsr #22]
    ldrb r5, [r1], #1
    str r8, [r0, r7, lsr #22]
    str r9, [r0, r6, lsr #22]
    add r4, r9, r8
    add r7, r7, lr
    ldr r4, [r0, r4, lsr #22]
    ldr r9, [r0, r7, lsr #22]
    eor r5, r5, r4, lsr #24
    strb r5, [r3], #1
    add r6, r6, r9
    ldr r8, [r0, r6, lsr #22]
    ldrb r5, [r1], #1
    str r8, [r0, r7, lsr #22]
    str r9, [r0, r6, lsr #22]
    add r4, r9, r8
    add r7, r7, lr
    ldr r4, [r0, r4, lsr #22]
    ldr r9, [r0, r7, lsr #22]
    eor r5, r5, r4, lsr #24
    strb r5, [r3], #1
    add r6, r6, r9
    ldr r8, [r0, r6, lsr #22]
    ldrb r5, [r1], #1
    str r8, [r0, r7, lsr #22]
    str r9, [r0, r6, lsr #22]
    add r4, r9, r8
    add r7, r7, lr
    ldr r4, [r0, r4, lsr #22]
    ldr r9, [r0, r7, lsr #22]
    eor r5, r5, r4, lsr #24
    strb r5, [r3], #1
    add r6, r6, r9
    ldr r8, [r0, r6, lsr #22]
    ldrb r5, [r1], #1
    str r8, [r0, r7, lsr #22]
    str r9, [r0, r6, lsr #22]
    add r4, r9, r8
    add r7, r7, lr
    ldr r4, [r0, r4, lsr #22]
    ldr r9, [r0, r7, lsr #22]
    eor r5, r5, r4, lsr #24
    strb r5, [r3], #1
    add r6, r6, r9
    subs r2, r2, #8
    ldr r8, [r0, r6, lsr #22]
    ldrb r5, [r1], #1
    str r8, [r0, r7, lsr #22]
    str r9, [r0, r6, lsr #22]
    add r4, r9, r8
    add r7, r7, lr
    ldr r4, [r0, r4, lsr #22]
    ldr r9, [r0, r7, lsr #22]
    eor r5, r5, r4, lsr #24
    strb r5, [r3], #1
    bpl @L020A064C
@L020A07B4:
    adds r2, r2, #8
    ble @L020A07F0
@L020A07BC:
    add r6, r6, r9
    subs r2, r2, #1
    ldr r8, [r0, r6, lsr #22]
    ldrb r5, [r1], #1
    str r8, [r0, r7, lsr #22]
    str r9, [r0, r6, lsr #22]
    add r4, r9, r8
    add r7, r7, lr
    ldr r4, [r0, r4, lsr #22]
    ldr r9, [r0, r7, lsr #22]
    eor r5, r5, r4, lsr #24
    strb r5, [r3], #1
    bgt @L020A07BC
@L020A07F0:
    sub r7, r7, lr
    and r7, r7, #0xff000000
    str r6, [r0, #-4]
    str r7, [r0, #-8]
    ldmia sp!, {r4, r5, r6, r7, r8, r9, pc}
}
