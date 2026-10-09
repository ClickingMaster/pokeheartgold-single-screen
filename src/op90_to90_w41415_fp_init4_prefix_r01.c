// ARM9 runtime floating-point / 64-bit integer support (_dadd.._ddiv, 0x020F068C, 11352 B).
// Hand-written ARM assembly (shared entry points, fall-through between routines, inline literal
// words and reciprocal tables in .text, cpsr flag tricks), kept as one MWCC asm function.
// The other 83 entry points are added as aliases by tools/object_aliases.py
// (op90_to90_w41415_fp_init4_prefix_r01.alias-map.json).
#include <nitro/types.h>
extern int errno;
void _dadd(void);
#include <nitro/code32.h>
asm void _dadd(void)
{
@dadd:
@d_add:
    stmdb sp!, {r4, lr}
    eors r12, r1, r3
    eormi r3, r3, #0x80000000
    bmi @dsub_start
@dadd_start:
    subs r12, r0, r2
    sbcs lr, r1, r3
    bhs @020F06B8
    adds r2, r2, r12
    adc r3, r3, lr
    subs r0, r0, r12
    sbc r1, r1, lr
@020F06B8:
    mov lr, #0x80000000
    mov r12, r1, lsr #0x14
    orr r1, lr, r1, lsl #11
    orr r1, r1, r0, lsr #21
    mov r0, r0, lsl #0xb
    movs r4, r12, lsl #0x15
    cmnne r4, #0x200000
    beq @020F07B4
    mov r4, r3, lsr #0x14
    orr r3, lr, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs lr, r4, lsl #0x15
    beq @020F07FC
@020F06F0:
    subs r4, r12, r4
    beq @020F0748
    cmp r4, #0x20
    ble @020F072C
    cmp r4, #0x38
    movge r4, #0x3f
    sub r4, r4, #0x20
    rsb lr, r4, #0x20
    orrs lr, r2, r3, lsl lr
    mov r2, r3, lsr r4
    orrne r2, r2, #1
    adds r0, r0, r2
    adcs r1, r1, #0
    blo @020F0770
    b @020F0754
@020F072C:
    rsb lr, r4, #0x20
    movs lr, r2, lsl lr
    rsb lr, r4, #0x20
    mov r2, r2, lsr r4
    orr r2, r2, r3, lsl lr
    mov r3, r3, lsr r4
    orrne r2, r2, #1
@020F0748:
    adds r0, r0, r2
    adcs r1, r1, r3
    blo @020F0770
@020F0754:
    add r12, r12, #1
    and r4, r0, #1
    movs r1, r1, rrx
    orr r0, r4, r0, rrx
    mov lr, r12, lsl #0x15
    cmn lr, #0x200000
    beq @020F0980
@020F0770:
    movs r2, r0, lsl #0x15
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    mov r1, r1, lsr #0xc
    orr r1, r1, r12, lsl #20
    tst r2, #0x80000000
    ldmeqia sp!, {r4, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    ldmeqia sp!, {r4, lr}
    bxeq lr
    adds r0, r0, #1
    adc r1, r1, #0
    ldmia sp!, {r4, lr}
    bx lr
@020F07B4:
    cmp r12, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bics r12, r12, #0x800
    beq @020F0820
    orrs r4, r0, r1, lsl #1
    bne @020F095C
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs r4, r4, lsl #0x15
    beq @020F0948
    cmn r4, #0x200000
    bne @020F0948
    orrs r4, r2, r3, lsl #1
    beq @020F0948
    b @020F095C
@020F07FC:
    cmp r4, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bic r12, r12, #0x800
    bics r4, r4, #0x800
    beq @020F088C
    orrs r4, r2, r3, lsl #1
    bne @020F095C
    b @020F0948
@020F0820:
    orrs r4, r0, r1, lsl #1
    beq @020F0860
    mov r12, #1
    bic r1, r1, #0x80000000
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs r4, r4, lsl #0x15
    cmnne r4, #0x200000
    mov r4, r4, lsr #0x15
    orr r4, r4, lr, lsr #20
    beq @020F07FC
    orr r3, r3, #0x80000000
    orr r12, r12, lr, lsr #20
    b @020F06F0
@020F0860:
    mov r12, r3, lsr #0x14
    mov r1, r3, lsl #0xb
    orr r1, r1, r2, lsr #21
    mov r0, r2, lsl #0xb
    movs r4, r12, lsl #0x15
    beq @020F0914
    cmn r4, #0x200000
    bne @020F0914
    orrs r4, r0, r1, lsl #1
    beq @020F0948
    b @020F0960
@020F088C:
    orrs r4, r2, r3, lsl #1
    beq @020F0924
    mov r4, #1
    bic r3, r3, #0x80000000
    cmp r1, #0
    bpl @020F08B0
    orr r12, r12, lr, lsr #20
    orr r4, r4, lr, lsr #20
    b @020F06F0
@020F08B0:
    adds r0, r0, r2
    adcs r1, r1, r3
    blo @020F08D0
    add r12, r12, #1
    and r4, r0, #1
    movs r1, r1, rrx
    mov r0, r0, rrx
    orr r0, r0, r4
@020F08D0:
    cmp r1, #0
    subges r12, r12, #1
    movs r2, r0, lsl #0x15
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, r12, lsl #20
    ldmeqia sp!, {r4, lr}
    bxeq lr
    tst r2, #0x80000000
    ldmeqia sp!, {r4, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    ldmeqia sp!, {r4, lr}
    bxeq lr
@020F0914:
    mov r1, r3
    mov r0, r2
    ldmia sp!, {r4, lr}
    bx lr
@020F0924:
    cmp r1, #0
    subges r12, r12, #1
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, r12, lsl #20
    ldmia sp!, {r4, lr}
    bx lr
@020F0948:
    ldr r1, [pc, #0x50] // _020F09A0
    orr r1, lr, r1
    mov r0, #0
    ldmia sp!, {r4, lr}
    bx lr
@020F095C:
    mov r1, r3
@020F0960:
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmia sp!, {r4, lr}
    bx lr
@020F0970:
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmia sp!, {r4, lr}
    bx lr
@020F0980:
    cmp r12, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    ldr r1, [pc, #0xC] // _020F09A0
    orr r1, lr, r1
    mov r0, #0
    ldmia sp!, {r4, lr}
    bx lr
@020F09A0:
    dcd 0x7FF00000
@d2f:
@d_dtof:
@f_qtof:
    and r2, r1, #0x80000000
    mov r12, r1, lsr #0x14
    bics r12, r12, #0x800
    beq @020F0A1C
    mov r3, r12, lsl #0x15
    cmn r3, #0x200000
    bhs @020F0A00
    subs r12, r12, #0x380
    bls @020F0A2C
    cmp r12, #0xff
    bge @020F0A9C
    mov r1, r1, lsl #0xc
    orr r3, r2, r1, lsr #9
    orr r3, r3, r0, lsr #29
    movs r1, r0, lsl #3
    orr r0, r3, r12, lsl #23
    bxeq lr
    tst r1, #0x80000000
    bxeq lr
    movs r1, r1, lsl #1
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@020F0A00:
    orrs r3, r0, r1, lsl #12
    bne @020F0A14
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@020F0A14:
    mvn r0, #0x80000000
    bx lr
@020F0A1C:
    orrs r3, r0, r1, lsl #12
    bne @f_underflow
@f_result_zero:
    mov r0, r2
    bx lr
@020F0A2C:
    cmn r12, #0x17
    beq @020F0A80
    bmi @f_underflow
    mov r1, r1, lsl #0xb
    orr r1, r1, #0x80000000
    mov r3, r1, lsr #8
    orr r3, r3, r0, lsr #29
    rsb r12, r12, #1
    movs r1, r0, lsl #3
    orr r0, r2, r3, lsr r12
    rsb r12, r12, #0x20
    mov r3, r3, lsl r12
    orrne r3, r3, #1
    movs r1, r3
    bxeq lr
    tst r1, #0x80000000
    bxeq lr
    movs r1, r1, lsl #1
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@020F0A80:
    orr r0, r0, r1, lsl #12
@f_very_tiny_result:
    movs r1, r0
    mov r0, r2
    addne r0, r0, #1
    bx lr
@f_underflow:
    mov r0, r2
    bx lr
@020F0A9C:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@dfix:
@d_dtoi:
    bic r3, r1, #0x80000000
    ldr r2, [pc, #0x3C] // _020F0AF0
    subs r2, r2, r3, lsr #20
    ble @020F0AE4
    cmp r2, #0x20
    bge @020F0ADC
    mov r3, r1, lsl #0xb
    orr r3, r3, #0x80000000
    orr r3, r3, r0, lsr #21
    cmp r1, #0
    mov r0, r3, lsr r2
    rsbmi r0, r0, #0
    bx lr
@020F0ADC:
    mov r0, #0
    bx lr
@020F0AE4:
    mvn r0, r1, asr #31
    add r0, r0, #0x80000000
    bx lr
@020F0AF0:
    dcd 0x0000041E
@dfixu:
@d_dtou:
    tst r1, #0x80000000
    bne @020F0B2C
    ldr r2, [pc, #0x44] // _020F0B48
    subs r2, r2, r1, lsr #20
    blt @020F0B40
    cmp r2, #0x20
    bge @020F0B24
    mov r3, r1, lsl #0xb
    orr r3, r3, #0x80000000
    orr r3, r3, r0, lsr #21
    mov r0, r3, lsr r2
    bx lr
@020F0B24:
    mov r0, #0
    bx lr
@020F0B2C:
    cmn r1, #0x100000
    cmpeq r0, #0
    movls r0, #0
    mvnhi r0, #0
    bx lr
@020F0B40:
    mvn r0, #0
    bx lr
@020F0B48:
    dcd 0x0000041E
@ll_ufrom_d:
@d_dtoull:
    tst r1, #0x80000000
    bne @020F0BB0
    ldr r2, [pc, #0x78] // _020F0BD4
    subs r2, r2, r1, lsr #20
    blt @020F0BC8
    cmp r2, #0x40
    bge @020F0BA4
    mov r12, r1, lsl #0xb
    orr r12, r12, #0x80000000
    orr r12, r12, r0, lsr #21
    cmp r2, #0x20
    ble @020F0B8C
    sub r2, r2, #0x20
    mov r1, #0
    mov r0, r12, lsr r2
    bx lr
@020F0B8C:
    mov r3, r0, lsl #0xb
    mov r1, r12, lsr r2
    mov r0, r3, lsr r2
    rsb r2, r2, #0x20
    orr r0, r0, r12, lsl r2
    bx lr
@020F0BA4:
    mov r1, #0
    mov r0, #0
    bx lr
@020F0BB0:
    cmn r1, #0x100000
    cmpeq r0, #0
    bhi @020F0BC8
    mov r1, #0
    mov r0, #0
    bx lr
@020F0BC8:
    mvn r1, #0
    mvn r0, #0
    bx lr
@020F0BD4:
    dcd 0x0000043E
@dflt:
@d_itod:
    ands r2, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r0, #0
@d_itod_common:
    mov r1, #0
    bxeq lr
    mov r3, #0x400
    add r3, r3, #0x1e
    clz r12, r0
    movs r0, r0, lsl r12
    sub r3, r3, r12
    movs r1, r0
    mov r0, r1, lsl #0x15
    add r1, r1, r1
    orr r1, r2, r1, lsr #12
    orr r1, r1, r3, lsl #20
    bx lr
@dfltu:
@d_utod:
    cmp r0, #0
@d_utod_common:
    mov r1, #0
    bxeq lr
    mov r3, #0x400
    add r3, r3, #0x1e
    bmi @020F0C3C
    clz r12, r0
    movs r0, r0, lsl r12
    sub r3, r3, r12
@020F0C3C:
    mov r1, r0
    mov r0, r1, lsl #0x15
    add r1, r1, r1
    mov r1, r1, lsr #0xc
    orr r1, r1, r3, lsl #20
    bx lr
@dmul:
@d_mul:
    stmdb sp!, {r4, r5, r6, r7, lr}
    eor lr, r1, r3
    and lr, lr, #0x80000000
    mov r12, r1, lsr #0x14
    mov r1, r1, lsl #0xb
    orr r1, r1, r0, lsr #21
    mov r0, r0, lsl #0xb
    movs r6, r12, lsl #0x15
    cmnne r6, #0x200000
    beq @020F0D5C
    orr r1, r1, #0x80000000
    bic r12, r12, #0x800
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs r5, r4, lsl #0x15
    cmnne r5, #0x200000
    beq @020F0DA4
    orr r3, r3, #0x80000000
    bic r4, r4, #0x800
@020F0CA8:
    add r12, r4, r12
    umull r5, r4, r0, r2
    umull r7, r6, r0, r3
    adds r4, r7, r4
    adc r6, r6, #0
    umull r7, r0, r1, r2
    adds r4, r7, r4
    adcs r0, r0, r6
    umull r7, r2, r1, r3
    adc r1, r2, #0
    adds r0, r0, r7
    adc r1, r1, #0
    orrs r4, r4, r5
    orrne r0, r0, #1
    cmp r1, #0
    blt @020F0CF4
    sub r12, r12, #1
    adds r0, r0, r0
    adc r1, r1, r1
@020F0CF4:
    add r12, r12, #2
    subs r12, r12, #0x400
    bmi @020F0E90
    beq @020F0E90
    mov r6, r12, lsl #0x14
    cmn r6, #0x100000
    bmi @020F0F90
    movs r2, r0, lsl #0x15
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, r12, lsl #20
    ldmeqia sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    tst r2, #0x80000000
    ldmeqia sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    ldmeqia sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    adds r0, r0, #1
    adc r1, r1, #0
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
@020F0D5C:
    bics r12, r12, #0x800
    beq @020F0DB8
    orrs r6, r0, r1, lsl #1
    bne @020F0F44
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs r5, r4, lsl #0x15
    beq @020F0D98
    cmn r5, #0x200000
    bne @020F0F30
    orrs r5, r2, r3, lsl #1
    beq @020F0F30
    b @020F0F44
@020F0D98:
    orrs r5, r3, r2
    beq @020F0F58
    b @020F0F30
@020F0DA4:
    bics r4, r4, #0x800
    beq @020F0E4C
    orrs r6, r2, r3, lsl #1
    bne @020F0F44
    b @020F0F30
@020F0DB8:
    orrs r6, r0, r1, lsl #1
    beq @020F0E20
    mov r12, #1
    cmp r1, #0
    bne @020F0DDC
    sub r12, r12, #0x20
    movs r1, r0
    mov r0, #0
    bmi @020F0DF8
@020F0DDC:
    clz r6, r1
    movs r1, r1, lsl r6
    rsb r6, r6, #0x20
    orr r1, r1, r0, lsr r6
    rsb r6, r6, #0x20
    mov r0, r0, lsl r6
    sub r12, r12, r6
@020F0DF8:
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs r5, r4, lsl #0x15
    cmnne r5, #0x200000
    beq @020F0DA4
    orr r3, r3, #0x80000000
    bic r4, r4, #0x800
    b @020F0CA8
@020F0E20:
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs r5, r4, lsl #0x15
    beq @020F0FA4
    cmn r5, #0x200000
    bne @020F0FA4
    orrs r6, r2, r3, lsl #1
    beq @020F0F58
    b @020F0F44
@020F0E4C:
    orrs r5, r2, r3, lsl #1
    beq @020F0FA4
    mov r4, #1
    cmp r3, #0
    bne @020F0E70
    sub r4, r4, #0x20
    movs r3, r2
    mov r2, #0
    bmi @020F0CA8
@020F0E70:
    clz r6, r3
    movs r3, r3, lsl r6
    rsb r6, r6, #0x20
    orr r3, r3, r2, lsr r6
    rsb r6, r6, #0x20
    mov r2, r2, lsl r6
    sub r4, r4, r6
    b @020F0CA8
@020F0E90:
    cmn r12, #0x34
    beq @020F0F28
    bmi @020F0F80
    mov r2, r1
    mov r3, r0
    add r4, r12, #0x34
    cmp r4, #0x20
    movge r2, r3
    movge r3, #0
    subge r4, r4, #0x20
    rsb r5, r4, #0x20
    mov r2, r2, lsl r4
    orr r2, r2, r3, lsr r5
    movs r3, r3, lsl r4
    orrne r2, r2, #1
    rsb r12, r12, #0xc
    cmp r12, #0x20
    movge r0, r1
    movge r1, #0
    subge r12, r12, #0x20
    rsb r4, r12, #0x20
    mov r0, r0, lsr r12
    orr r0, r0, r1, lsl r4
    orr r1, lr, r1, lsr r12
    cmp r2, #0
    ldmeqia sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    tst r2, #0x80000000
    ldmeqia sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    ldmeqia sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    adds r0, r0, #1
    adc r1, r1, #0
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
@020F0F28:
    orr r0, r0, r1, lsl #1
    b @020F0F68
@020F0F30:
    ldr r1, [pc, #0x7C] // _020F0FB4
    orr r1, lr, r1
    mov r0, #0
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
@020F0F44:
    mov r1, r3
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
@020F0F58:
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
@020F0F68:
    movs r2, r0
    mov r1, lr
    mov r0, #0
    addne r0, r0, #1
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
@020F0F80:
    mov r1, lr
    mov r0, #0
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
@020F0F90:
    ldr r1, [pc, #0x1C] // _020F0FB4
    orr r1, lr, r1
    mov r0, #0
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
@020F0FA4:
    mov r1, lr
    mov r0, #0
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
@020F0FB4:
    dcd 0x7FF00000
@dsqrt:
    stmdb sp!, {r4, r5, r6, lr}
    ldr r2, [pc, #0x184] // _020F1148
    cmp r1, r2
    bhs @020F1104
    movs r12, r1, lsr #0x14
    beq @020F10B0
    bic r1, r1, r2
    orr r1, r1, #0x100000
@020F0FD8:
    movs r12, r12, asr #1
    bhs @020F0FEC
    sub r12, r12, #1
    movs r0, r0, lsl #1
    adc r1, r1, r1
@020F0FEC:
    movs r3, r0, lsl #1
    adc r1, r1, r1
    mov r2, #0
    mov r4, #0
    mov lr, #0x200000
@020F1000:
    add r6, r4, lr
    cmp r6, r1
    addle r4, r6, lr
    suble r1, r1, r6
    addle r2, r2, lr
    movs r3, r3, lsl #1
    adc r1, r1, r1
    movs lr, lr, lsr #1
    bne @020F1000
    mov r0, #0
    mov r5, #0
    cmp r1, r4
    cmpeq r3, #0x80000000
    blo @020F1048
    subs r3, r3, #0x80000000
    sbc r1, r1, r4
    add r4, r4, #1
    mov r0, #0x80000000
@020F1048:
    movs r3, r3, lsl #1
    adc r1, r1, r1
    mov lr, #0x40000000
@020F1054:
    add r6, r5, lr
    cmp r4, r1
    cmpeq r6, r3
    bhi @020F1074
    add r5, r6, lr
    subs r3, r3, r6
    sbc r1, r1, r4
    add r0, r0, lr
@020F1074:
    movs r3, r3, lsl #1
    adc r1, r1, r1
    movs lr, lr, lsr #1
    bne @020F1054
    orrs r1, r1, r3
    biceq r0, r0, #1
    movs r1, r2, lsr #1
    movs r0, r0, rrx
    adcs r0, r0, #0
    adc r1, r1, #0
    add r1, r1, #0x20000000
    sub r1, r1, #0x100000
    add r1, r1, r12, lsl #20
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@020F10B0:
    cmp r1, #0
    bne @020F10E0
    cmp r0, #0
    ldmeqia sp!, {r4, r5, r6, lr}
    bxeq lr
    mvn r12, #0x13
    clz r5, r0
    movs r0, r0, lsl r5
    sub r12, r12, r5
    mov r1, r0, lsr #0xb
    mov r0, r0, lsl #0x15
    b @020F0FD8
@020F10E0:
    clz r2, r1
    movs r1, r1, lsl r2
    rsb r2, r2, #0x2b
    mov r1, r1, lsr #0xb
    orr r1, r1, r0, lsr r2
    rsb r2, r2, #0x20
    mov r0, r0, lsl r2
    rsb r12, r2, #1
    b @020F0FD8
@020F1104:
    tst r1, #0x80000000
    beq @020F1120
    bics r3, r1, #0x80000000
    cmpeq r0, #0
    ldmeqia sp!, {r4, r5, r6, lr}
    bxeq lr
    b @020F112C
@020F1120:
    orrs r2, r0, r1, lsl #12
    ldmeqia sp!, {r4, r5, r6, lr}
    bxeq lr
@020F112C:
    ldr r2, [pc, #0x18] // _020F114C
    orr r1, r1, r2
    ldr r3, [pc, #0x14] // _020F1150
    mov r4, #0x21
    str r4, [r3]
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@020F1148:
    dcd 0x7FF00000
@020F114C:
    dcd 0x7FF80000
@020F1150:
    dcd errno
@drsb:
    eor r1, r1, r3
    eor r3, r1, r3
    eor r1, r1, r3
    eor r0, r0, r2
    eor r2, r0, r2
    eor r0, r0, r2
@dsub:
@d_sub:
    stmdb sp!, {r4, lr}
    eors r12, r1, r3
    eormi r3, r3, #0x80000000
    bmi @dadd_start
@dsub_start:
    subs r12, r0, r2
    sbcs lr, r1, r3
    bhs @020F119C
    eor lr, lr, #0x80000000
    adds r2, r2, r12
    adc r3, r3, lr
    subs r0, r0, r12
    sbc r1, r1, lr
@020F119C:
    mov lr, #0x80000000
    mov r12, r1, lsr #0x14
    orr r1, lr, r1, lsl #11
    orr r1, r1, r0, lsr #21
    mov r0, r0, lsl #0xb
    movs r4, r12, lsl #0x15
    cmnne r4, #0x200000
    beq @020F13A0
    mov r4, r3, lsr #0x14
    orr r3, lr, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs lr, r4, lsl #0x15
    beq @020F13E8
@020F11D4:
    subs r4, r12, r4
    beq @020F127C
    cmp r4, #0x20
    ble @020F1210
    cmp r4, #0x38
    movge r4, #0x3f
    sub r4, r4, #0x20
    rsb lr, r4, #0x20
    orrs lr, r2, r3, lsl lr
    mov r2, r3, lsr r4
    orrne r2, r2, #1
    subs r0, r0, r2
    sbcs r1, r1, #0
    bmi @020F1238
    b @020F1328
@020F1210:
    rsb lr, r4, #0x20
    movs lr, r2, lsl lr
    rsb lr, r4, #0x20
    mov r2, r2, lsr r4
    orr r2, r2, r3, lsl lr
    mov r3, r3, lsr r4
    orrne r2, r2, #1
    subs r0, r0, r2
    sbcs r1, r1, r3
    bpl @020F1328
@020F1238:
    movs r2, r0, lsl #0x15
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    mov r1, r1, lsr #0xc
    orr r1, r1, r12, lsl #20
    tst r2, #0x80000000
    ldmeqia sp!, {r4, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    ldmeqia sp!, {r4, lr}
    bxeq lr
    adds r0, r0, #1
    adc r1, r1, #0
    ldmia sp!, {r4, lr}
    bx lr
@020F127C:
    subs r0, r0, r2
    sbc r1, r1, r3
    orrs lr, r1, r0
    beq @020F150C
    mov lr, r12, lsl #0x14
    and lr, lr, #0x80000000
    bic r12, r12, #0x800
    cmp r1, #0
    bmi @020F1304
    bne @020F12B4
    sub r12, r12, #0x20
    movs r1, r0
    mov r0, #0
    bmi @020F12D0
@020F12B4:
    clz r4, r1
    movs r1, r1, lsl r4
    rsb r4, r4, #0x20
    orr r1, r1, r0, lsr r4
    rsb r4, r4, #0x20
    mov r0, r0, lsl r4
    sub r12, r12, r4
@020F12D0:
    cmp r12, #0
    bgt @020F130C
    rsb r12, r12, #0xc
    cmp r12, #0x20
    movge r0, r1
    movge r1, #0
    subge r12, r12, #0x20
    rsb r4, r12, #0x20
    mov r0, r0, lsr r12
    orr r0, r0, r1, lsl r4
    orr r1, lr, r1, lsr r12
    ldmia sp!, {r4, lr}
    bx lr
@020F1304:
    cmp r1, #0
    subges r12, r12, #1
@020F130C:
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, r12, lsl #20
    ldmia sp!, {r4, lr}
    bx lr
@020F1328:
    mov lr, r12, lsl #0x14
    and lr, lr, #0x80000000
    bic r12, r12, #0x800
    cmp r1, #0
    bne @020F134C
    sub r12, r12, #0x20
    movs r1, r0
    mov r0, #0
    bmi @020F1368
@020F134C:
    clz r4, r1
    movs r1, r1, lsl r4
    rsb r4, r4, #0x20
    orr r1, r1, r0, lsr r4
    rsb r4, r4, #0x20
    mov r0, r0, lsl r4
    sub r12, r12, r4
@020F1368:
    cmp r12, #0
    orrgt r12, r12, lr, lsr #20
    bgt @020F1238
    rsb r12, r12, #0xc
    cmp r12, #0x20
    movge r0, r1
    movge r1, #0
    subge r12, r12, #0x20
    rsb r4, r12, #0x20
    mov r0, r0, lsr r12
    orr r0, r0, r1, lsl r4
    orr r1, lr, r1, lsr r12
    ldmia sp!, {r4, lr}
    bx lr
@020F13A0:
    cmp r12, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bics r12, r12, #0x800
    beq @020F140C
    orrs r4, r0, r1, lsl #1
    bne @020F14E8
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs r4, r4, lsl #0x15
    beq @020F14D4
    cmn r4, #0x200000
    bne @020F14D4
    orrs r4, r2, r3, lsl #1
    beq @020F14FC
    b @020F14E8
@020F13E8:
    cmp r4, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bic r12, r12, #0x800
    bics r4, r4, #0x800
    beq @020F1484
    orrs r4, r2, r3, lsl #1
    bne @020F14E8
    b @020F14D4
@020F140C:
    orrs r4, r0, r1, lsl #1
    beq @020F144C
    mov r12, #1
    bic r1, r1, #0x80000000
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs r4, r4, lsl #0x15
    cmnne r4, #0x200000
    mov r4, r4, lsr #0x15
    orr r4, r4, lr, lsr #20
    beq @020F13E8
    orr r3, r3, #0x80000000
    orr r12, r12, lr, lsr #20
    b @020F11D4
@020F144C:
    mov r12, r3, lsr #0x14
    mov r1, r3, lsl #0xb
    orr r1, r1, r2, lsr #21
    mov r0, r2, lsl #0xb
    movs r4, r12, lsl #0x15
    beq @020F1478
    cmn r4, #0x200000
    bne @020F14A0
    orrs r4, r0, r1, lsl #1
    bne @020F14EC
    b @020F14D4
@020F1478:
    orrs r4, r0, r1, lsl #1
    beq @020F150C
    b @020F14A0
@020F1484:
    orrs r4, r2, r3, lsl #1
    beq @020F14B0
    mov r4, #1
    bic r3, r3, #0x80000000
    orr r12, r12, lr, lsr #20
    orr r4, r4, lr, lsr #20
    b @020F11D4
@020F14A0:
    mov r1, r3
    mov r0, r2
    ldmia sp!, {r4, lr}
    bx lr
@020F14B0:
    cmp r1, #0
    subges r12, r12, #1
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, r12, lsl #20
    ldmia sp!, {r4, lr}
    bx lr
@020F14D4:
    ldr r1, [pc, #0x40] // _020F151C
    orr r1, lr, r1
    mov r0, #0
    ldmia sp!, {r4, lr}
    bx lr
@020F14E8:
    mov r1, r3
@020F14EC:
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmia sp!, {r4, lr}
    bx lr
@020F14FC:
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmia sp!, {r4, lr}
    bx lr
@020F150C:
    mov r1, #0
    mov r0, #0
    ldmia sp!, {r4, lr}
    bx lr
@020F151C:
    dcd 0x7FF00000
@fadd:
@f_add:
    eors r2, r0, r1
    eormi r1, r1, #0x80000000
    bmi @fsub_start
@fadd_start:
    subs r12, r0, r1
    sublo r0, r0, r12
    addlo r1, r1, r12
    mov r2, #0x80000000
    mov r3, r0, lsr #0x17
    orr r0, r2, r0, lsl #8
    ands r12, r3, #0xff
    cmpne r12, #0xff
    beq @020F15C0
    mov r12, r1, lsr #0x17
    orr r1, r2, r1, lsl #8
    ands r2, r12, #0xff
    beq @020F1600
@020F1560:
    subs r12, r3, r12
    beq @020F1578
    rsb r2, r12, #0x20
    movs r2, r1, lsl r2
    mov r1, r1, lsr r12
    orrne r1, r1, #1
@020F1578:
    adds r0, r0, r1
    blo @020F1598
    and r1, r0, #1
    orr r0, r1, r0, rrx
    add r3, r3, #1
    and r2, r3, #0xff
    cmp r2, #0xff
    beq @020F1708
@020F1598:
    ands r1, r0, #0xff
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    tst r1, #0x80
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@020F15C0:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    ands r3, r3, #0xff
    beq @020F1624
    movs r0, r0, lsl #1
    bne @020F1734
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #9
    ands r12, r12, #0xff
    beq @020F1728
    cmp r12, #0xff
    blt @020F1728
    cmp r1, #0
    beq @020F1728
    b @020F1734
@020F1600:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    and r3, r3, #0xff
    ands r12, r12, #0xff
    beq @020F1680
@020F1618:
    movs r1, r1, lsl #1
    bne @020F1734
    b @020F1728
@020F1624:
    movs r0, r0, lsl #1
    beq @020F165C
    mov r3, #1
    mov r0, r0, lsr #1
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #8
    ands r12, r12, #0xff
    beq @020F1680
    cmp r12, #0xff
    beq @020F1618
    orr r1, r1, #0x80000000
    orr r3, r3, r2, lsr #23
    orr r12, r12, r2, lsr #23
    b @020F1560
@020F165C:
    mov r3, r1, lsr #0x17
    mov r0, r1, lsl #9
    ands r3, r3, #0xff
    beq @020F16E8
    cmp r3, #0xff
    blt @020F16E8
    cmp r0, #0
    beq @020F1728
    b @020F1720
@020F1680:
    movs r1, r1, lsl #1
    beq @020F16F0
    mov r1, r1, lsr #1
    mov r12, #1
    orr r3, r3, r2, lsr #23
    orr r12, r12, r2, lsr #23
    cmp r0, #0
    bmi @020F1560
    adds r0, r0, r1
    blo @020F16B4
    and r1, r0, #1
    orr r0, r1, r0, rrx
    add r12, r12, #1
@020F16B4:
    cmp r0, #0
    subge r12, r12, #1
    ands r1, r0, #0xff
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r12, lsl #23
    bxeq lr
    tst r1, #0x80
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@020F16E8:
    mov r0, r1
    bx lr
@020F16F0:
    cmp r0, #0
    subges r3, r3, #1
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
@020F1708:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@020F1720:
    mvn r0, #0x80000000
    bx lr
@020F1728:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@020F1734:
    mvn r0, #0x80000000
    bx lr
@020F173C:
    mvn r0, #0x80000000
    bx lr
@dgeq:
@d_fge:
    mov r12, #0x200000
    cmn r12, r1, lsl #1
    bhs @020F17B8
    cmn r12, r3, lsl #1
    bhs @020F17CC
@020F1758:
    orrs r12, r3, r1
    bmi @020F1788
    cmp r1, r3
    cmpeq r0, r2
    movhs r0, #1
    movlo r0, #0
    bx lr
@020F1774:
    mov r0, #0
    mrs r12, cpsr
    bic r12, r12, #0x20000000
    msr cpsr_f, r12
    bx lr
@020F1788:
    orr r12, r0, r12, lsl #1
    orrs r12, r12, r2
    moveq r0, #1
    mrs r12, cpsr
    orr r12, r12, #0x20000000
    msr cpsr_f, r12
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    movhs r0, #1
    movlo r0, #0
    bx lr
@020F17B8:
    bne @020F1774
    cmp r0, #0
    bhi @020F1774
    cmn r12, r3, lsl #1
    blo @020F1758
@020F17CC:
    bne @020F1774
    cmp r2, #0
    bhi @020F1774
    b @020F1758
@dgr:
@d_fgt:
    mov r12, #0x200000
    cmn r12, r1, lsl #1
    bhs @020F1850
    cmn r12, r3, lsl #1
    bhs @020F1864
@020F17F0:
    orrs r12, r3, r1
    bmi @020F1820
    cmp r1, r3
    cmpeq r0, r2
    movhi r0, #1
    movls r0, #0
    bx lr
@020F180C:
    mov r0, #0
    mrs r12, cpsr
    bic r12, r12, #0x20000000
    msr cpsr_f, r12
    bx lr
@020F1820:
    orr r12, r0, r12, lsl #1
    orrs r12, r12, r2
    moveq r0, #0
    mrs r12, cpsr
    bic r12, r12, #0x20000000
    msr cpsr_f, r12
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    movhi r0, #1
    movls r0, #0
    bx lr
@020F1850:
    bne @020F180C
    cmp r0, #0
    bhi @020F180C
    cmn r12, r3, lsl #1
    blo @020F17F0
@020F1864:
    bne @020F180C
    cmp r2, #0
    bhi @020F180C
    b @020F17F0
@dleq:
@d_fle:
    mov r12, #0x200000
    cmn r12, r1, lsl #1
    bhs @020F18F4
    cmn r12, r3, lsl #1
    bhs @020F1908
@020F1888:
    orrs r12, r3, r1
    bmi @020F18BC
    cmp r1, r3
    cmpeq r0, r2
    movls r0, #1
    movhi r0, #0
    bx lr
@020F18A4:
    mov r0, #0
    mrs r12, cpsr
    bic r12, r12, #0x40000000
    orr r12, r12, #0x20000000
    msr cpsr_f, r12
    bx lr
@020F18BC:
    orr r12, r0, r12, lsl #1
    orrs r12, r12, r2
    moveq r0, #1
    bne @020F18E0
    mrs r12, cpsr
    bic r12, r12, #0x20000000
    orr r12, r12, #0x40000000
    msr cpsr_f, r12
    bxeq lr
@020F18E0:
    cmp r3, r1
    cmpeq r2, r0
    movls r0, #1
    movhi r0, #0
    bx lr
@020F18F4:
    bne @020F18A4
    cmp r0, #0
    bhi @020F18A4
    cmn r12, r3, lsl #1
    blo @020F1888
@020F1908:
    bne @020F18A4
    cmp r2, #0
    bhi @020F18A4
    b @020F1888
@dls:
@d_flt:
    mov r12, #0x200000
    cmn r12, r1, lsl #1
    bhs @020F1990
    cmn r12, r3, lsl #1
    bhs @020F19A4
@020F192C:
    orrs r12, r3, r1
    bmi @020F195C
    cmp r1, r3
    cmpeq r0, r2
    movlo r0, #1
    movhs r0, #0
    bx lr
@020F1948:
    mov r0, #0
    mrs r12, cpsr
    orr r12, r12, #0x20000000
    msr cpsr_f, r12
    bx lr
@020F195C:
    orr r12, r0, r12, lsl #1
    orrs r12, r12, r2
    moveq r0, #0
    bne @020F197C
    mrs r12, cpsr
    orr r12, r12, #0x20000000
    msr cpsr_f, r12
    bxeq lr
@020F197C:
    cmp r3, r1
    cmpeq r2, r0
    movlo r0, #1
    movhs r0, #0
    bx lr
@020F1990:
    bne @020F1948
    cmp r0, #0
    bhi @020F1948
    cmn r12, r3, lsl #1
    blo @020F192C
@020F19A4:
    bne @020F1948
    cmp r2, #0
    bhi @020F1948
    b @020F192C
@deq:
@d_feq:
    mov r12, #0x200000
    cmn r12, r1, lsl #1
    bhs @020F1A1C
    cmn r12, r3, lsl #1
    bhs @020F1A30
@020F19C8:
    orrs r12, r3, r1
    bmi @020F19F8
    cmp r1, r3
    cmpeq r0, r2
    moveq r0, #1
    movne r0, #0
    bx lr
@020F19E4:
    mov r0, #0
    mrs r12, cpsr
    bic r12, r12, #0x40000000
    msr cpsr_f, r12
    bx lr
@020F19F8:
    orr r12, r0, r12, lsl #1
    orrs r12, r12, r2
    moveq r0, #1
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    moveq r0, #1
    movne r0, #0
    bx lr
@020F1A1C:
    bne @020F19E4
    cmp r0, #0
    bhi @020F19E4
    cmn r12, r3, lsl #1
    blo @020F19C8
@020F1A30:
    bne @020F19E4
    cmp r2, #0
    bhi @020F19E4
    b @020F19C8
@dneq:
@d_fne:
    mov r12, #0x200000
    cmn r12, r1, lsl #1
    bhs @020F1AA8
    cmn r12, r3, lsl #1
    bhs @020F1ABC
@020F1A54:
    orrs r12, r3, r1
    bmi @020F1A84
    cmp r1, r3
    cmpeq r0, r2
    movne r0, #1
    moveq r0, #0
    bx lr
@020F1A70:
    mov r0, #1
    mrs r12, cpsr
    bic r12, r12, #0x40000000
    msr cpsr_f, r12
    bx lr
@020F1A84:
    orr r12, r0, r12, lsl #1
    orrs r12, r12, r2
    moveq r0, #0
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    movne r0, #1
    moveq r0, #0
    bx lr
@020F1AA8:
    bne @020F1A70
    cmp r0, #0
    bhi @020F1A70
    cmn r12, r3, lsl #1
    blo @020F1A54
@020F1ABC:
    bne @020F1A70
    cmp r2, #0
    bhi @020F1A70
    b @020F1A54
@fgr:
@f_fgt:
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    cmphs r3, r1, lsl #1
    blo @020F1B14
    cmp r0, #0
    bicmi r0, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r1, #0
    bicmi r1, r1, #0x80000000
    rsbmi r1, r1, #0
    cmp r0, r1
    movgt r0, #1
    movle r0, #0
    mrs r12, cpsr
    bicle r12, r12, #0x20000000
    orrgt r12, r12, #0x20000000
    msr cpsr_f, r12
    bx lr
@020F1B14:
    mov r0, #0
    mrs r12, cpsr
    bic r12, r12, #0x20000000
    msr cpsr_f, r12
    bx lr
@fleq:
@f_fle:
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    cmphs r3, r1, lsl #1
    blo @020F1B78
    cmp r0, #0
    bicmi r0, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r1, #0
    bicmi r1, r1, #0x80000000
    rsbmi r1, r1, #0
    cmp r0, r1
    movle r0, #1
    movgt r0, #0
    mrs r12, cpsr
    orrgt r12, r12, #0x20000000
    bicgt r12, r12, #0x40000000
    bicle r12, r12, #0x20000000
    orrle r12, r12, #0x40000000
    msr cpsr_f, r12
    bx lr
@020F1B78:
    mov r0, #0
    mrs r12, cpsr
    bic r12, r12, #0x40000000
    orr r12, r12, #0x20000000
    msr cpsr_f, r12
    bx lr
@fls:
@f_flt:
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    cmphs r3, r1, lsl #1
    blo @020F1BD8
    cmp r0, #0
    bicmi r0, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r1, #0
    bicmi r1, r1, #0x80000000
    rsbmi r1, r1, #0
    cmp r0, r1
    movlt r0, #1
    movge r0, #0
    mrs r12, cpsr
    orrge r12, r12, #0x20000000
    biclt r12, r12, #0x20000000
    msr cpsr_f, r12
    bx lr
@020F1BD8:
    mov r0, #0
    mrs r12, cpsr
    orr r12, r12, #0x20000000
    msr cpsr_f, r12
    bx lr
@feq:
@f_feq:
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    blo @020F1C40
    cmp r3, r1, lsl #1
    blo @020F1C40
    orr r3, r0, r1
    movs r3, r3, lsl #1
    moveq r0, #1
    bne @020F1C20
    mrs r12, cpsr
    orr r12, r12, #0x40000000
    msr cpsr_f, r12
    bx lr
@020F1C20:
    cmp r0, r1
    moveq r0, #1
    movne r0, #0
    mrs r12, cpsr
    orreq r12, r12, #0x40000000
    bicne r12, r12, #0x40000000
    msr cpsr_f, r12
    bx lr
@020F1C40:
    mov r0, #0
    mrs r12, cpsr
    bic r12, r12, #0x40000000
    msr cpsr_f, r12
    bx lr
@fneq:
@f_fne:
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    blo @020F1CA8
    cmp r3, r1, lsl #1
    blo @020F1CA8
    orr r3, r0, r1
    movs r3, r3, lsl #1
    moveq r0, #0
    bne @020F1C88
    mrs r12, cpsr
    orr r12, r12, #0x40000000
    msr cpsr_f, r12
    bx lr
@020F1C88:
    cmp r0, r1
    movne r0, #1
    moveq r0, #0
    mrs r12, cpsr
    bicne r12, r12, #0x40000000
    orreq r12, r12, #0x40000000
    msr cpsr_f, r12
    bx lr
@020F1CA8:
    mov r0, #1
    mrs r12, cpsr
    bic r12, r12, #0x40000000
    msr cpsr_f, r12
    bx lr
@frdiv:
    eor r0, r0, r1
    eor r1, r0, r1
    eor r0, r0, r1
@fdiv:
@f_div:
    stmdb sp!, {lr}
    mov r12, #0xff
    ands r3, r12, r0, lsr #23
    cmpne r3, #0xff
    beq @020F1E9C
    ands r12, r12, r1, lsr #23
    cmpne r12, #0xff
    beq @020F1ED8
    orr r1, r1, #0x800000
    orr r0, r0, #0x800000
    bic r2, r0, #0xff000000
    bic lr, r1, #0xff000000
@020F1CF8:
    cmp r2, lr
    movlo r2, r2, lsl #1
    sublo r3, r3, #1
    teq r0, r1
    sub r0, pc, #0x94
    ldrb r1, [r0, lr, lsr #15]
    rsb lr, lr, #0
    mov r0, lr, asr #1
    mul r0, r1, r0
    add r0, r0, #0x80000000
    mov r0, r0, lsr #6
    mul r0, r1, r0
    mov r0, r0, lsr #0xe
    mul r1, lr, r0
    sub r12, r3, r12
    mov r1, r1, lsr #0xc
    mul r1, r0, r1
    mov r0, r0, lsl #0xe
    add r0, r0, r1, lsr #15
    umull r1, r0, r2, r0
    mov r3, r0
    orrmi r0, r0, #0x80000000
    adds r12, r12, #0x7e
    bmi @020F1FA0
    cmp r12, #0xfe
    bge @020F2054
    add r0, r0, r12, lsl #23
    mov r12, r1, lsr #0x1c
    cmp r12, #7
    beq @020F1E7C
    add r0, r0, r1, lsr #31
    ldmia sp!, {lr}
    bx lr
@UNK_020F1D7C:
    dcd 0xFDFEFFFF
    dcd 0xF9FAFBFC
    dcd 0xF5F6F7F8
    dcd 0xF1F2F3F4
    dcd 0xEEEFF0F0
    dcd 0xEAEBECED
    dcd 0xE7E8E9EA
    dcd 0xE4E5E6E6
    dcd 0xE1E2E2E3
    dcd 0xDEDFDFE0
    dcd 0xDBDCDCDD
    dcd 0xD8D9D9DA
    dcd 0xD5D6D7D7
    dcd 0xD2D3D4D4
    dcd 0xD0D0D1D2
    dcd 0xCDCECECF
    dcd 0xCBCBCCCC
    dcd 0xC8C9C9CA
    dcd 0xC6C6C7C8
    dcd 0xC3C4C5C5
    dcd 0xC1C2C2C3
    dcd 0xBFBFC0C0
    dcd 0xBDBDBEBE
    dcd 0xBABBBCBC
    dcd 0xB8B9B9BA
    dcd 0xB6B7B7B8
    dcd 0xB4B5B5B6
    dcd 0xB2B3B3B4
    dcd 0xB0B1B1B2
    dcd 0xAFAFAFB0
    dcd 0xADADAEAE
    dcd 0xABABACAC
    dcd 0xA9AAAAAA
    dcd 0xA7A8A8A9
    dcd 0xA6A6A7A7
    dcd 0xA4A4A5A5
    dcd 0xA2A3A3A4
    dcd 0xA1A1A2A2
    dcd 0x9FA0A0A0
    dcd 0x9E9E9E9F
    dcd 0x9C9D9D9D
    dcd 0x9B9B9B9C
    dcd 0x999A9A9A
    dcd 0x98989999
    dcd 0x96979798
    dcd 0x95959696
    dcd 0x94949495
    dcd 0x92939393
    dcd 0x91919292
    dcd 0x90909191
    dcd 0x8F8F8F90
    dcd 0x8D8E8E8E
    dcd 0x8C8C8D8D
    dcd 0x8B8B8C8C
    dcd 0x8A8A8A8B
    dcd 0x8989898A
    dcd 0x88888888
    dcd 0x86878787
    dcd 0x85868686
    dcd 0x84858585
    dcd 0x83838484
    dcd 0x82828383
    dcd 0x81818282
    dcd 0x80808181
@020F1E7C:
    mov r1, r3, lsl #1
    add r1, r1, #1
    rsb lr, lr, #0
    mul r1, lr, r1
    cmp r1, r2, lsl #24
    addmi r0, r0, #1
    ldmia sp!, {lr}
    bx lr
@020F1E9C:
    eor lr, r0, r1
    and lr, lr, #0x80000000
    cmp r3, #0
    beq @020F1EF4
    movs r0, r0, lsl #9
    bne @020F203C
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #9
    ands r12, r12, #0xff
    beq @020F202C
    cmp r12, #0xff
    blt @020F202C
    cmp r1, #0
    beq @020F2048
    b @020F2024
@020F1ED8:
    eor lr, r0, r1
    and lr, lr, #0x80000000
    cmp r12, #0
    beq @020F1F58
@020F1EE8:
    movs r1, r1, lsl #9
    bne @020F2024
    b @020F2074
@020F1EF4:
    movs r2, r0, lsl #9
    beq @020F1F28
    clz r3, r2
    movs r2, r2, lsl r3
    rsb r3, r3, #0
    mov r2, r2, lsr #8
    ands r12, r12, r1, lsr #23
    beq @020F1F80
    cmp r12, #0xff
    beq @020F1EE8
    orr r1, r1, #0x800000
    bic lr, r1, #0xff000000
    b @020F1CF8
@020F1F28:
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #9
    ands r12, r12, #0xff
    beq @020F1F4C
    cmp r12, #0xff
    blt @020F2074
    cmp r1, #0
    beq @020F2074
    b @020F2024
@020F1F4C:
    cmp r1, #0
    beq @020F2048
    b @020F2074
@020F1F58:
    movs r12, r1, lsl #9
    beq @020F202C
    mov lr, r12
    clz r12, lr
    movs lr, lr, lsl r12
    rsb r12, r12, #0
    mov lr, lr, lsr #8
    orr r0, r0, #0x800000
    bic r2, r0, #0xff000000
    b @020F1CF8
@020F1F80:
    movs r12, r1, lsl #9
    beq @020F202C
    mov lr, r12
    clz r12, lr
    movs lr, lr, lsl r12
    rsb r12, r12, #0
    mov lr, lr, lsr #8
    b @020F1CF8
@020F1FA0:
    and r0, r0, #0x80000000
    cmn r12, #0x18
    beq @020F2014
    bmi @020F206C
    add r1, r12, #0x17
    mov r2, r2, lsl r1
    rsb r12, r12, #0
    mov r3, r3, lsr r12
    orr r0, r0, r3
    rsb lr, lr, #0
    mul r1, lr, r3
    cmp r1, r2
    ldmeqia sp!, {lr}
    bxeq lr
    add r1, r1, lr
    cmp r1, r2
    beq @020F2008
    addmi r0, r0, #1
    subpl r1, r1, lr
    add r1, lr, r1, lsl #1
    cmp r1, r2, lsl #1
    and r3, r0, #1
    addmi r0, r0, #1
    addeq r0, r0, r3
    ldmia sp!, {lr}
    bx lr
@020F2008:
    add r0, r0, #1
    ldmia sp!, {lr}
    bx lr
@020F2014:
    cmn r2, lr
    addne r0, r0, #1
    ldmia sp!, {lr}
    bx lr
@020F2024:
    mov r0, r1
    b @020F203C
@020F202C:
    mov r0, #0xff000000
    orr r0, lr, r0, lsr #1
    ldmia sp!, {lr}
    bx lr
@020F203C:
    mvn r0, #0x80000000
    ldmia sp!, {lr}
    bx lr
@020F2048:
    mvn r0, #0x80000000
    ldmia sp!, {lr}
    bx lr
@020F2054:
    tst r0, #0x80000000
    mov r0, #0xff000000
    movne r0, r0, asr #1
    moveq r0, r0, lsr #1
    ldmia sp!, {lr}
    bx lr
@020F206C:
    ldmia sp!, {lr}
    bx lr
@020F2074:
    mov r0, lr
    ldmia sp!, {lr}
    bx lr
@f2d:
@f_ftod:
    and r2, r0, #0x80000000
    mov r12, r0, lsr #0x17
    mov r3, r0, lsl #9
    ands r12, r12, #0xff
    beq @020F20B0
    cmp r12, #0xff
    beq @020F20DC
@020F209C:
    add r12, r12, #0x380
    mov r0, r3, lsl #0x14
    orr r1, r2, r3, lsr #12
    orr r1, r1, r12, lsl #20
    bx lr
@020F20B0:
    cmp r3, #0
    bne @020F20C4
    mov r1, r2
    mov r0, #0
    bx lr
@020F20C4:
    mov r3, r3, lsr #1
    clz r12, r3
    movs r3, r3, lsl r12
    rsb r12, r12, #1
    add r3, r3, r3
    b @020F209C
@020F20DC:
    cmp r3, #0
    bhi @020F20F4
    ldr r1, [pc, #0x14] // _020F2100
    orr r1, r1, r2
    mov r0, #0
    bx lr
@020F20F4:
    mvn r0, #0
    bic r1, r0, #0x80000000
    bx lr
@020F2100:
    dcd 0x7FF00000
@f_ftoi:
@ffix:
    bic r1, r0, #0x80000000
    mov r2, #0x9e
    subs r2, r2, r1, lsr #23
    ble @020F212C
    mov r1, r1, lsl #8
    orr r1, r1, #0x80000000
    cmp r0, #0
    mov r0, r1, lsr r2
    rsbmi r0, r0, #0
    bx lr
@020F212C:
    mvn r0, r0, asr #31
    add r0, r0, #0x80000000
    bx lr
@ffixu:
@f_ftou:
    tst r0, #0x80000000
    bne @020F215C
    mov r1, #0x9e
    subs r1, r1, r0, lsr #23
    blt @020F2170
    mov r2, r0, lsl #8
    orr r0, r2, #0x80000000
    mov r0, r0, lsr r1
    bx lr
@020F215C:
    mov r2, #0xff000000
    cmp r2, r0, lsl #1
    movhs r0, #0
    mvnlo r0, #0
    bx lr
@020F2170:
    mvn r0, #0
    bx lr
@fflt:
@f_itof:
    ands r2, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r0, #0
@f_itof_common:
    bxeq lr
    clz r3, r0
    movs r0, r0, lsl r3
    rsb r3, r3, #0x9e
    ands r1, r0, #0xff
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bxeq lr
    tst r1, #0x80
    bxeq lr
    ands r3, r1, #0x7f
    andeqs r3, r0, #1
    addne r0, r0, #1
    bx lr
@ffltu:
@f_utof:
    cmp r0, #0
@f_utof_common:
    bxeq lr
    mov r3, #0x9e
    bmi @020F21DC
    clz r12, r0
    movs r0, r0, lsl r12
    sub r3, r3, r12
@020F21DC:
    ands r2, r0, #0xff
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bxeq lr
    tst r2, #0x80
    bxeq lr
    ands r1, r2, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@f_lltof:
@ll_sto_f:
    ands r2, r1, #0x80000000
    beq @020F2218
    rsbs r0, r0, #0
    rsc r1, r1, #0
@020F2218:
    cmp r1, #0
    bne @020F2228
    movs r0, r0
    b @f_itof_common
@020F2228:
    clz r3, r1
    movs r1, r1, lsl r3
    rsb r3, r3, #0x20
    orr r1, r1, r0, lsr r3
    rsb r12, r3, #0x20
    movs r0, r0, lsl r12
    orrne r1, r1, #1
    add r3, r3, #0x9e
    ands r12, r1, #0xff
    add r0, r1, r1
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bxeq lr
    tst r12, #0x80
    bxeq lr
    ands r3, r12, #0x7f
    andeqs r3, r0, #1
    addne r0, r0, #1
    bx lr
@f_ulltof:
@ll_uto_f:
    cmp r1, #0
    bne @020F2284
    movs r0, r0
    b @f_utof_common
@020F2284:
    mov r3, #0x20
    bmi @020F22A4
    clz r12, r1
    movs r1, r1, lsl r12
    sub r3, r3, r12
    orr r1, r1, r0, lsr r3
    rsb r2, r3, #0x20
    mov r0, r0, lsl r2
@020F22A4:
    cmp r0, #0
    orrne r1, r1, #1
    add r3, r3, #0x9e
    ands r2, r1, #0xff
    add r0, r1, r1
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bxeq lr
    tst r2, #0x80
    bxeq lr
    ands r1, r2, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@fmul:
@f_mul:
    eor r2, r0, r1
    and r2, r2, #0x80000000
    mov r12, #0xff
    ands r3, r12, r0, lsr #23
    mov r0, r0, lsl #8
    cmpne r3, #0xff
    beq @020F2358
    orr r0, r0, #0x80000000
    ands r12, r12, r1, lsr #23
    mov r1, r1, lsl #8
    cmpne r12, #0xff
    beq @020F2398
    orr r1, r1, #0x80000000
@020F2310:
    add r12, r3, r12
    umull r1, r3, r0, r1
    movs r0, r3
    addpl r0, r0, r0
    subpl r12, r12, #1
    subs r12, r12, #0x7f
    bmi @020F2424
    cmp r12, #0xfe
    bge @020F2490
    ands r3, r0, #0xff
    orr r0, r2, r0, lsr #8
    add r0, r0, r12, lsl #23
    tst r3, #0x80
    bxeq lr
    orrs r1, r1, r3, lsl #25
    andeqs r3, r0, #1
    addne r0, r0, #1
    bx lr
@020F2358:
    cmp r3, #0
    beq @020F23AC
    movs r0, r0, lsl #1
    bne @f_result_x_NaN
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #9
    ands r12, r12, #0xff
    beq @020F238C
    cmp r12, #0xff
    blt @f_result_INF
    cmp r1, #0
    beq @f_result_INF
    b @f_result_x_NaN
@020F238C:
    cmp r1, #0
    beq @f_result_invalid
    b @f_result_INF
@020F2398:
    cmp r12, #0
    beq @020F2408
@020F23A0:
    movs r1, r1, lsl #1
    bne @f_result_x_NaN
    b @f_result_INF
@020F23AC:
    movs r0, r0, lsl #1
    beq @020F23E4
    mov r0, r0, lsr #1
    clz r3, r0
    movs r0, r0, lsl r3
    rsb r3, r3, #1
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #8
    ands r12, r12, #0xff
    beq @020F2408
    cmp r12, #0xff
    beq @020F23A0
    orr r1, r1, #0x80000000
    b @020F2310
@020F23E4:
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #9
    ands r12, r12, #0xff
    beq @020F24B4
    cmp r12, #0xff
    blt @020F24B4
    cmp r1, #0
    beq @f_result_invalid
    b @f_result_x_NaN
@020F2408:
    movs r1, r1, lsl #1
    beq @020F24B4
    mov r1, r1, lsr #1
    clz r12, r1
    movs r1, r1, lsl r12
    rsb r12, r12, #1
    b @020F2310
@020F2424:
    cmn r12, #0x18
    beq @020F246C
    bmi @020F24AC
    cmp r1, #0
    orrne r0, r0, #1
    mov r3, r0
    mov r0, r0, lsr #8
    rsb r12, r12, #0
    orr r0, r2, r0, lsr r12
    rsb r12, r12, #0x18
    movs r1, r3, lsl r12
    bxeq lr
    tst r1, #0x80000000
    bxeq lr
    movs r1, r1, lsl #1
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@020F246C:
    mov r0, r0, lsl #1
    b @020F249C
@f_result_INF:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@f_result_x_NaN:
    mvn r0, #0x80000000
    bx lr
@f_result_invalid:
    mvn r0, #0x80000000
    bx lr
@020F2490:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@020F249C:
    movs r1, r0
    mov r0, r2
    addne r0, r0, #1
    bx lr
@020F24AC:
    mov r0, r2
    bx lr
@020F24B4:
    mov r0, r2
    bx lr
@frsb:
    eor r0, r0, r1
    eor r1, r0, r1
    eor r0, r0, r1
@fsub:
@f_sub:
    eors r2, r0, r1
    eormi r1, r1, #0x80000000
    bmi @fadd_start
@fsub_start:
    subs r12, r0, r1
    eorlo r12, r12, #0x80000000
    sublo r0, r0, r12
    addlo r1, r1, r12
    mov r2, #0x80000000
    mov r3, r0, lsr #0x17
    orr r0, r2, r0, lsl #8
    ands r12, r3, #0xff
    cmpne r12, #0xff
    beq @020F25F0
    mov r12, r1, lsr #0x17
    orr r1, r2, r1, lsl #8
    ands r2, r12, #0xff
    beq @020F2630
@020F250C:
    subs r12, r3, r12
    beq @020F2554
    rsb r2, r12, #0x20
    movs r2, r1, lsl r2
    mov r1, r1, lsr r12
    orrne r1, r1, #1
    subs r0, r0, r1
    bpl @020F2598
    ands r1, r0, #0xff
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    tst r1, #0x80
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@020F2554:
    subs r0, r0, r1
    beq @020F26FC
    mov r2, r3, lsl #0x17
    and r2, r2, #0x80000000
    bic r3, r3, #0x100
    clz r12, r0
    movs r0, r0, lsl r12
    sub r3, r3, r12
    cmp r3, #0
    bgt @020F2588
    rsb r3, r3, #9
    orr r0, r2, r0, lsr r3
    bx lr
@020F2588:
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
@020F2598:
    mov r2, r3, lsl #0x17
    and r2, r2, #0x80000000
    bic r3, r3, #0x100
    clz r12, r0
    movs r0, r0, lsl r12
    sub r3, r3, r12
    cmp r3, #0
    bgt @020F25C4
    rsb r3, r3, #9
    orr r0, r2, r0, lsr r3
    bx lr
@020F25C4:
    ands r1, r0, #0xff
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bxeq lr
    tst r1, #0x80
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@020F25F0:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    ands r3, r3, #0xff
    beq @020F2658
    movs r0, r0, lsl #1
    bne @020F2730
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #9
    ands r12, r12, #0xff
    beq @020F2724
    cmp r12, #0xff
    blt @020F2724
    cmp r1, #0
    beq @020F2738
    b @020F2730
@020F2630:
    cmp r12, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    and r3, r3, #0xff
    ands r12, r12, #0xff
    beq @020F26C0
@020F2648:
    eor r2, r2, #0x80000000
    movs r1, r1, lsl #1
    bne @020F2730
    b @020F2724
@020F2658:
    movs r0, r0, lsl #1
    beq @020F2690
    mov r0, r0, lsr #1
    mov r3, #1
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #8
    ands r12, r12, #0xff
    beq @020F26C0
    cmp r12, #0xff
    beq @020F2648
    orr r1, r1, #0x80000000
    orr r3, r3, r2, lsr #23
    orr r12, r12, r2, lsr #23
    b @020F250C
@020F2690:
    mov r3, r1, lsr #0x17
    mov r0, r1, lsl #9
    ands r2, r3, #0xff
    beq @020F26B4
    cmp r2, #0xff
    blt @020F26DC
    cmp r0, #0
    bne @020F271C
    b @020F2724
@020F26B4:
    cmp r0, #0
    beq @020F26FC
    b @020F26DC
@020F26C0:
    movs r1, r1, lsl #1
    beq @020F26E4
    mov r1, r1, lsr #1
    mov r12, #1
    orr r12, r12, r2, lsr #23
    orr r3, r3, r2, lsr #23
    b @020F250C
@020F26DC:
    mov r0, r1
    bx lr
@020F26E4:
    cmp r0, #0
    subges r3, r3, #1
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
@020F26FC:
    mov r0, #0
    bx lr
@020F2704:
    cmp r0, #0
    subges r3, r3, #1
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
@020F271C:
    mvn r0, #0x80000000
    bx lr
@020F2724:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@020F2730:
    mvn r0, #0x80000000
    bx lr
@020F2738:
    mvn r0, #0x80000000
    bx lr
@ll_mod:
    stmdb sp!, {r4, r5, r6, r7, r11, r12, lr}
    mov r4, r1
    orr r4, r4, #1
    b @020F2760
@ll_sdiv:
@ll_div:
    stmdb sp!, {r4, r5, r6, r7, r11, r12, lr}
    eor r4, r1, r3
    mov r4, r4, asr #1
    mov r4, r4, lsl #1
@020F2760:
    orrs r5, r3, r2
    bne @020F2770
    ldmia sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
@020F2770:
    mov r5, r0, lsr #0x1f
    add r5, r5, r1
    mov r6, r2, lsr #0x1f
    add r6, r6, r3
    orrs r6, r5, r6
    bne @020F27A4
    mov r1, r2
    bl @s32_div_f
    ands r4, r4, #1
    movne r0, r1
    mov r1, r0, asr #0x1f
    ldmia sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
@020F27A4:
    cmp r1, #0
    bge @020F27B4
    rsbs r0, r0, #0
    rsc r1, r1, #0
@020F27B4:
    cmp r3, #0
    bge @020F27C4
    rsbs r2, r2, #0
    rsc r3, r3, #0
@020F27C4:
    orrs r5, r1, r0
    beq @020F28E8
    mov r5, #0
    mov r6, #1
    cmp r3, #0
    bmi @020F27F0
@020F27DC:
    add r5, r5, #1
    adds r2, r2, r2
    adcs r3, r3, r3
    bpl @020F27DC
    add r6, r6, r5
@020F27F0:
    cmp r1, #0
    blt @020F2810
@020F27F8:
    cmp r6, #1
    beq @020F2810
    sub r6, r6, #1
    adds r0, r0, r0
    adcs r1, r1, r1
    bpl @020F27F8
@020F2810:
    mov r7, #0
    mov r12, #0
    mov r11, #0
    b @020F2838
@020F2820:
    orr r12, r12, #1
    subs r6, r6, #1
    beq @020F2890
    adds r0, r0, r0
    adcs r1, r1, r1
    adcs r7, r7, r7
@020F2838:
    subs r0, r0, r2
    sbcs r1, r1, r3
    sbcs r7, r7, #0
    adds r12, r12, r12
    adc r11, r11, r11
    cmp r7, #0
    bge @020F2820
@020F2854:
    subs r6, r6, #1
    beq @020F2888
    adds r0, r0, r0
    adcs r1, r1, r1
    adc r7, r7, r7
    adds r0, r0, r2
    adcs r1, r1, r3
    adc r7, r7, #0
    adds r12, r12, r12
    adc r11, r11, r11
    cmp r7, #0
    bge @020F2820
    b @020F2854
@020F2888:
    adds r0, r0, r2
    adc r1, r1, r3
@020F2890:
    ands r7, r4, #1
    moveq r0, r12
    moveq r1, r11
    beq @020F28C8
    subs r7, r5, #0x20
    movge r0, r1, lsr r7
    bge @020F28EC
    rsb r7, r5, #0x20
    mov r0, r0, lsr r5
    orr r0, r0, r1, lsl r7
    mov r1, r1, lsr r5
    b @020F28C8
@020F28C0:
    mov r0, r1, lsr r7
    mov r1, #0
@020F28C8:
    cmp r4, #0
    blt @020F28D8
    ldmia sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
@020F28D8:
    rsbs r0, r0, #0
    rsc r1, r1, #0
    ldmia sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
@020F28E8:
    mov r0, #0
@020F28EC:
    mov r1, #0
    cmp r4, #0
    blt @020F28D8
    ldmia sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
@ll_udiv:
@ull_div:
    stmdb sp!, {r4, r5, r6, r7, r11, r12, lr}
    mov r4, #0
    b @020F2914
@ull_mod:
    stmdb sp!, {r4, r5, r6, r7, r11, r12, lr}
    mov r4, #1
@020F2914:
    orrs r5, r3, r2
    bne @020F2924
    ldmia sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
@020F2924:
    orrs r5, r1, r3
    bne @020F27C4
    mov r1, r2
    bl @u32_div_not_0_f
    cmp r4, #0
    movne r0, r1
    mov r1, #0
    ldmia sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
@ll_mul:
@ull_mul:
    stmdb sp!, {r4, r5, lr}
    umull r5, r4, r0, r2
    mla r4, r0, r3, r4
    mla r4, r2, r1, r4
    mov r1, r4
    mov r0, r5
    ldmia sp!, {r4, r5, lr}
    bx lr
@ll_shl:
@ll_sll:
@ull_sll:
    ands r2, r2, #0x3f
    bxeq lr
    subs r3, r2, #0x20
    bge @020F298C
    rsb r3, r2, #0x20
    mov r1, r1, lsl r2
    orr r1, r1, r0, lsr r3
    mov r0, r0, lsl r2
    bx lr
@020F298C:
    mov r1, r0, lsl r3
    mov r0, #0
    bx lr
@s32_div_f:
    eor r12, r0, r1
    and r12, r12, #0x80000000
    cmp r0, #0
    rsblt r0, r0, #0
    addlt r12, r12, #1
    cmp r1, #0
    rsblt r1, r1, #0
    beq @020F2B90
    cmp r0, r1
    movlo r1, r0
    movlo r0, #0
    blo @020F2B90
    mov r2, #0x1c
    mov r3, r0, lsr #4
    cmp r1, r3, lsr #12
    suble r2, r2, #0x10
    movle r3, r3, lsr #0x10
    cmp r1, r3, lsr #4
    suble r2, r2, #8
    movle r3, r3, lsr #8
    cmp r1, r3
    suble r2, r2, #4
    movle r3, r3, lsr #4
    mov r0, r0, lsl r2
    rsb r1, r1, #0
    adds r0, r0, r0
    add r2, r2, r2, lsl #1
    add pc, pc, r2, lsl #2
    nop
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    mov r1, r3
@020F2B90:
    ands r3, r12, #0x80000000
    rsbne r0, r0, #0
    ands r3, r12, #1
    rsbne r1, r1, #0
    bx lr
@u32_div_f:
    cmp r1, #0
    bxeq lr
@u32_div_not_0_f:
    cmp r0, r1
    movlo r1, r0
    movlo r0, #0
    bxlo lr
    mov r2, #0x1c
    mov r3, r0, lsr #4
    cmp r1, r3, lsr #12
    suble r2, r2, #0x10
    movle r3, r3, lsr #0x10
    cmp r1, r3, lsr #4
    suble r2, r2, #8
    movle r3, r3, lsr #8
    cmp r1, r3
    suble r2, r2, #4
    movle r3, r3, lsr #4
    mov r0, r0, lsl r2
    rsb r1, r1, #0
    adds r0, r0, r0
    add r2, r2, r2, lsl #1
    add pc, pc, r2, lsl #2
    nop
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    mov r1, r3
    bx lr
@drdiv:
    eor r1, r1, r3
    eor r3, r1, r3
    eor r1, r1, r3
    eor r0, r0, r2
    eor r2, r0, r2
    eor r0, r0, r2
@ddiv:
@d_div:
    stmdb sp!, {r4, r5, r6, lr}
    ldr lr, [pc, #0x534] // _020F32E0
    eor r4, r1, r3
    ands r12, lr, r1, lsr #19
    cmpne r12, lr
    beq @020F314C
    bic r1, r1, lr, lsl #20
    orr r1, r1, #0x100000
    add r12, r12, r4, lsr #31
@020F2DC4:
    ands r4, lr, r3, lsr #19
    cmpne r4, lr
    beq @020F31E4
    bic r3, r3, lr, lsl #20
    orr r3, r3, #0x100000
@020F2DD8:
    sub r12, r12, r4
    cmp r1, r3
    cmpeq r0, r2
    bhs @020F2DF4
    adds r0, r0, r0
    adc r1, r1, r1
    sub r12, r12, #2
@020F2DF4:
    sub r4, pc, #0x24
    ldrb lr, [r4, r3, lsr #12]
    rsbs r2, r2, #0
    rsc r3, r3, #0
    mov r4, #0x20000000
    mla r5, lr, r3, r4
    mov r6, r3, lsl #0xa
    mov r5, r5, lsr #7
    mul lr, r5, lr
    orr r6, r6, r2, lsr #22
    mov lr, lr, lsr #0xd
    mul r5, lr, r6
    mov r6, r1, lsl #0xa
    orr r6, r6, r0, lsr #22
    mov r5, r5, lsr #0x10
    mul r5, lr, r5
    mov lr, lr, lsl #0xe
    add lr, lr, r5, lsr #16
    umull r5, r6, lr, r6
    umull r4, r5, r6, r2
    mla r5, r3, r6, r5
    mov r4, r4, lsr #0x1a
    orr r4, r4, r5, lsl #6
    add r4, r4, r0, lsl #2
    umull lr, r5, r4, lr
    mov r4, #0
    adds r5, r5, r6, lsl #24
    adc r4, r4, r6, lsr #8
    cmp r12, #0x800
    bge @020F2FD8
    add r12, r12, #0x7f0
    adds r12, r12, #0xc
    bmi @020F2FF0
    orr r1, r4, r12, lsl #31
    bic r12, r12, #1
    add r1, r1, r12, lsl #19
    tst lr, #0x80000000
    bne @020F2EC8
    rsbs r2, r2, #0
    mov r4, r4, lsl #1
    add r4, r4, r5, lsr #31
    mul lr, r2, r4
    mov r6, #0
    mov r4, r5, lsl #1
    orr r4, r4, #1
    umlal r6, lr, r4, r2
    rsc r3, r3, #0
    mla lr, r4, r3, lr
    cmp lr, r0, lsl #21
    bmi @020F2EC8
    mov r0, r5
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@020F2EC8:
    adds r0, r5, #1
    adc r1, r1, #0
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@UNK_020F2ED8:
    dcd 0xFDFEFFFF
    dcd 0xF9FAFBFC
    dcd 0xF5F6F7F8
    dcd 0xF1F2F3F4
    dcd 0xEEEFF0F0
    dcd 0xEAEBECED
    dcd 0xE7E8E9EA
    dcd 0xE4E5E6E6
    dcd 0xE1E2E2E3
    dcd 0xDEDFDFE0
    dcd 0xDBDCDCDD
    dcd 0xD8D9D9DA
    dcd 0xD5D6D7D7
    dcd 0xD2D3D4D4
    dcd 0xD0D0D1D2
    dcd 0xCDCECECF
    dcd 0xCBCBCCCC
    dcd 0xC8C9C9CA
    dcd 0xC6C6C7C8
    dcd 0xC3C4C5C5
    dcd 0xC1C2C2C3
    dcd 0xBFBFC0C0
    dcd 0xBDBDBEBE
    dcd 0xBABBBCBC
    dcd 0xB8B9B9BA
    dcd 0xB6B7B7B8
    dcd 0xB4B5B5B6
    dcd 0xB2B3B3B4
    dcd 0xB0B1B1B2
    dcd 0xAFAFAFB0
    dcd 0xADADAEAE
    dcd 0xABABACAC
    dcd 0xA9AAAAAA
    dcd 0xA7A8A8A9
    dcd 0xA6A6A7A7
    dcd 0xA4A4A5A5
    dcd 0xA2A3A3A4
    dcd 0xA1A1A2A2
    dcd 0x9FA0A0A0
    dcd 0x9E9E9E9F
    dcd 0x9C9D9D9D
    dcd 0x9B9B9B9C
    dcd 0x999A9A9A
    dcd 0x98989999
    dcd 0x96979798
    dcd 0x95959696
    dcd 0x94949495
    dcd 0x92939393
    dcd 0x91919292
    dcd 0x90909191
    dcd 0x8F8F8F90
    dcd 0x8D8E8E8E
    dcd 0x8C8C8D8D
    dcd 0x8B8B8C8C
    dcd 0x8A8A8A8B
    dcd 0x8989898A
    dcd 0x88888888
    dcd 0x86878787
    dcd 0x85868686
    dcd 0x84858585
    dcd 0x83838484
    dcd 0x82828383
    dcd 0x81818282
    dcd 0x80808181
@020F2FD8:
    movs r1, r12, lsl #0x1f
    orr r1, r1, #0x7f000000
    orr r1, r1, #0xf00000
    mov r0, #0
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@020F2FF0:
    mvn r6, r12, asr #1
    cmp r6, #0x34
    bgt @020F313C
    beq @020F3118
    cmp r6, #0x14
    bge @020F3038
    rsb r6, r6, #0x13
    mov lr, r0, lsl r6
    rsb r6, r6, #0x14
    mov r0, r5, lsr r6
    rsb r6, r6, #0x20
    orr r0, r0, r4, lsl r6
    rsb r6, r6, #0x20
    mov r4, r4, lsr r6
    orr r1, r4, r12, lsl #31
    mov r12, lr
    mov lr, #0
    b @020F3068
@020F3038:
    rsb r6, r6, #0x33
    mov lr, r1, lsl r6
    mov r1, r12, lsl #0x1f
    rsb r6, r6, #0x20
    orr r12, lr, r0, lsr r6
    rsb r6, r6, #0x20
    mov lr, r0, lsl r6
    mov r5, r5, lsr #0x15
    orr r5, r5, r4, lsl #11
    rsb r6, r6, #0x1f
    mov r0, r5, lsr r6
    mov r4, #0
@020F3068:
    rsbs r2, r2, #0
    mul r4, r2, r4
    mov r5, #0
    umlal r5, r4, r2, r0
    rsc r3, r3, #0
    mla r4, r0, r3, r4
    cmp r4, r12
    cmpeq r5, lr
    ldmeqia sp!, {r4, r5, r6, lr}
    bxeq lr
    adds r5, r5, r2
    adc r4, r4, r3
    cmp r4, r12
    bmi @020F310C
    bne @020F30B0
    cmp r5, lr
    beq @020F30FC
    blo @020F310C
@020F30B0:
    subs r5, r5, r2
    sbc r4, r4, r3
@020F30B8:
    adds r5, r5, r5
    adc r4, r4, r4
    adds r5, r5, r2
    adc r4, r4, r3
    adds lr, lr, lr
    adc r12, r12, r12
    cmp r4, r12
    bmi @020F30FC
    ldmneia sp!, {r4, r5, r6, lr}
    bxne lr
    cmp r5, lr
    blo @020F30FC
    ldmneia sp!, {r4, r5, r6, lr}
    bxne lr
    tst r0, #1
    ldmeqia sp!, {r4, r5, r6, lr}
    bxeq lr
@020F30FC:
    adds r0, r0, #1
    adc r1, r1, #0
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@020F310C:
    adds r0, r0, #1
    adc r1, r1, #0
    b @020F30B8
@020F3118:
    rsbs r2, r2, #0
    rsc r3, r3, #0
    cmp r1, r3
    cmpeq r0, r2
    mov r1, r12, lsl #0x1f
    mov r0, #0
    movne r0, #1
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@020F313C:
    mov r1, r12, lsl #0x1f
    mov r0, #0
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@020F314C:
    orrs r5, r0, r1, lsl #1
    beq @020F3270
    cmp r12, lr
    beq @020F31B4
    movs r1, r1, lsl #0xc
    beq @020F3190
    clz r5, r1
    movs r1, r1, lsl r5
    sub r12, r12, r5
    add r5, r12, #0x1f
    mov r1, r1, lsr #0xb
    orr r1, r1, r0, lsr r5
    rsb r5, r5, #0x20
    mov r0, r0, lsl r5
    mov r12, r12, lsl #1
    orr r12, r12, r4, lsr #31
    b @020F2DC4
@020F3190:
    mvn r12, #0x13
    clz r5, r0
    movs r0, r0, lsl r5
    sub r12, r12, r5
    mov r1, r0, lsr #0xb
    mov r0, r0, lsl #0x15
    mov r12, r12, lsl #1
    orr r12, r12, r4, lsr #31
    b @020F2DC4
@020F31B4:
    orrs r5, r0, r1, lsl #12
    bne @020F3298
    bic r5, r3, #0x80000000
    cmp r5, lr, lsl #19
    bhs @020F31D8
    and r5, r3, #0x80000000
    eor r1, r5, r1
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@020F31D8:
    orrs r5, r2, r3, lsl #12
    bne @020F32B8
    b @020F32D0
@020F31E4:
    orrs r5, r2, r3, lsl #1
    beq @020F325C
    cmp r4, lr
    beq @020F3244
    movs r3, r3, lsl #0xc
    beq @020F3224
    clz r5, r3
    movs r3, r3, lsl r5
    sub r4, r4, r5
    add r5, r4, #0x1f
    mov r3, r3, lsr #0xb
    orr r3, r3, r2, lsr r5
    rsb r5, r5, #0x20
    mov r2, r2, lsl r5
    mov r4, r4, lsl #1
    b @020F2DD8
@020F3224:
    mvn r4, #0x13
    clz r5, r2
    movs r2, r2, lsl r5
    sub r4, r4, r5
    mov r3, r2, lsr #0xb
    mov r2, r2, lsl #0x15
    mov r4, r4, lsl #1
    b @020F2DD8
@020F3244:
    orrs r5, r2, r3, lsl #12
    bne @020F32B8
    mov r1, r12, lsl #0x1f
    mov r0, #0
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@020F325C:
    mov r1, r12, lsl #0x1f
    orr r1, r1, lr, lsl #19
    mov r0, #0
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@020F3270:
    orrs r5, r2, r3, lsl #1
    beq @020F32D0
    bic r5, r3, #0x80000000
    cmp r5, lr, lsl #19
    cmpeq r2, #0
    bhi @020F32B8
    eor r1, r1, r3
    and r1, r1, #0x80000000
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@020F3298:
    tst r1, #0x80000
    beq @020F32D0
    bic r5, r3, #0x80000000
    cmp r5, lr, lsl #19
    cmpeq r2, #0
    bhi @020F32B8
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@020F32B8:
    tst r3, #0x80000
    beq @020F32D0
    mov r1, r3
    mov r0, r2
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@020F32D0:
    orr r1, r1, #0x7f000000
    orr r1, r1, #0xf80000
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@020F32E0:
    dcd 0x00000FFE
}
#include <nitro/codereset.h>
