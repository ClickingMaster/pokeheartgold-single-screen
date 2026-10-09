#pragma force_active on
#include <nitro/types.h>
typedef int (*MATHQSortComparator)(const void *, const void *);
void MATH_QSort(void *base, u32 count, u32 width, MATHQSortComparator compare, void *context);
#include <nitro/code32.h>
// @fn MATH_QSort
asm void MATH_QSort(void *base, u32 count, u32 width, MATHQSortComparator compare, void *context)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    cmp r1, #1
    ble @L020E3D6C
    ldr r4, [sp, #0x24]
    mov r11, r3
    mov r8, r2
    cmp r4, #0
    bne @L020E3ABC
    clz r2, r1
    rsb r2, r2, #0x20
    mov r2, r2, lsl #3
    sub sp, sp, r2
    mov r4, sp
    str r2, [sp, #-4]!
@L020E3ABC:
    sub r1, r1, #1
    mla r1, r1, r8, r0
    mov r5, r4
    str r0, [r4], #4
    str r1, [r4], #4
    clz r2, r8
    rsb r2, r2, #0x20
    str r2, [sp, #-4]!
@L020E3ADC:
    cmp r4, r5
    beq @L020E3D3C
    ldr r7, [r4, #-4]
    ldr r6, [r4, #-8]!
    sub r2, r7, r6
    cmp r2, r8
    bne @L020E3B50
    mov r0, r6
    mov r1, r7
    blx r11
    cmp r0, #0
    ble @L020E3ADC
    mov r0, r8
    tst r0, #3
    beq @L020E3B34
@L020E3B18:
    ldrb r1, [r6]
    subs r0, r0, #1
    swpb r1, r1, [r7]
    add r7, r7, #1
    strb r1, [r6], #1
    bne @L020E3B18
    b @L020E3ADC
@L020E3B34:
    ldr r1, [r6]
    subs r0, r0, #4
    swp r1, r1, [r7]
    add r7, r7, #4
    str r1, [r6], #4
    bne @L020E3B34
    b @L020E3ADC
@L020E3B50:
    ldr r3, [sp]
    sub r2, r7, r6
    mov r2, r2, lsr r3
    mla r2, r2, r8, r6
    mov r3, r6
    mov r0, r8
    mov r2, r2
    tst r0, #3
    beq @L020E3B90
@L020E3B74:
    ldrb r1, [r2]
    subs r0, r0, #1
    swpb r1, r1, [r3]
    add r3, r3, #1
    strb r1, [r2], #1
    bne @L020E3B74
    b @L020E3BA8
@L020E3B90:
    ldr r1, [r2]
    subs r0, r0, #4
    swp r1, r1, [r3]
    add r3, r3, #4
    str r1, [r2], #4
    bne @L020E3B90
@L020E3BA8:
    mov r9, r6
    mov r10, r7
    add r9, r9, r8
@L020E3BB4:
    cmp r9, r7
    bge @L020E3BDC
    mov r1, r6
    mov r0, r9
    blx r11
    cmp r0, #0
    blt @L020E3BD4
    b @L020E3BD8
@L020E3BD4:
    add r9, r9, r8
@L020E3BD8:
    blt @L020E3BB4
@L020E3BDC:
    mov r1, r6
    mov r0, r10
    blx r11
    cmp r0, #0
    bgt @L020E3BF4
    b @L020E3BF8
@L020E3BF4:
    sub r10, r10, r8
@L020E3BF8:
    bgt @L020E3BDC
    cmp r9, r10
    bge @L020E3C5C
    mov r2, r9
    mov r3, r10
    mov r0, r8
    tst r0, #3
    beq @L020E3C34
@L020E3C18:
    ldrb r1, [r2]
    subs r0, r0, #1
    swpb r1, r1, [r3]
    add r3, r3, #1
    strb r1, [r2], #1
    bne @L020E3C18
    b @L020E3C4C
@L020E3C34:
    ldr r1, [r2]
    subs r0, r0, #4
    swp r1, r1, [r3]
    add r3, r3, #4
    str r1, [r2], #4
    bne @L020E3C34
@L020E3C4C:
    add r9, r9, r8
    sub r10, r10, r8
    cmp r9, r10
    ble @L020E3BB4
@L020E3C5C:
    mov r2, r6
    mov r3, r10
    mov r0, r8
    tst r0, #3
    beq @L020E3C8C
@L020E3C70:
    ldrb r1, [r2]
    subs r0, r0, #1
    swpb r1, r1, [r3]
    add r3, r3, #1
    strb r1, [r2], #1
    bne @L020E3C70
    b @L020E3CA4
@L020E3C8C:
    ldr r1, [r2]
    subs r0, r0, #4
    swp r1, r1, [r3]
    add r3, r3, #4
    str r1, [r2], #4
    bne @L020E3C8C
@L020E3CA4:
    sub r2, r10, r6
    sub r3, r7, r10
    cmp r2, r3
    ble @L020E3CF8
    sub r2, r10, r8
    cmp r6, r2
    blt @L020E3CC4
    b @L020E3CC8
@L020E3CC4:
    str r6, [r4], #4
@L020E3CC8:
    blt @L020E3CD0
    b @L020E3CD4
@L020E3CD0:
    str r2, [r4], #4
@L020E3CD4:
    add r2, r10, r8
    cmp r2, r7
    blt @L020E3CE4
    b @L020E3CE8
@L020E3CE4:
    str r2, [r4], #4
@L020E3CE8:
    blt @L020E3CF0
    b @L020E3CF4
@L020E3CF0:
    str r7, [r4], #4
@L020E3CF4:
    b @L020E3ADC
@L020E3CF8:
    add r2, r10, r8
    cmp r2, r7
    blt @L020E3D08
    b @L020E3D0C
@L020E3D08:
    str r2, [r4], #4
@L020E3D0C:
    blt @L020E3D14
    b @L020E3D18
@L020E3D14:
    str r7, [r4], #4
@L020E3D18:
    sub r2, r10, r8
    cmp r6, r2
    blt @L020E3D28
    b @L020E3D2C
@L020E3D28:
    str r6, [r4], #4
@L020E3D2C:
    blt @L020E3D34
    b @L020E3D38
@L020E3D34:
    str r2, [r4], #4
@L020E3D38:
    b @L020E3ADC
@L020E3D3C:
    add sp, sp, #4
    sub r4, r4, #4
    cmp r4, sp
    beq @L020E3D50
    b @L020E3D54
@L020E3D50:
    ldr r0, [sp]
@L020E3D54:
    beq @L020E3D5C
    b @L020E3D60
@L020E3D5C:
    add r0, r0, #4
@L020E3D60:
    beq @L020E3D68
    b @L020E3D6C
@L020E3D68:
    add sp, sp, r0
@L020E3D6C:
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
}
#include <nitro/codereset.h>
