#pragma force_active on
#include <nitro/types.h>
void MIi_CpuClear16(register u16 data, register void *destp, register u32 size);
void MIi_CpuCopy16(register const void *srcp, register void *destp, register u32 size);
void MIi_CpuClear32(register u32 data, register void *destp, register u32 size);
void MIi_CpuCopy32(register const void *srcp, register void *destp, register u32 size);
void MIi_CpuSend32(register const void *srcp, volatile void *destp, u32 size);
void MIi_CpuClearFast(register u32 data, register void *destp, register u32 size);
void MIi_CpuCopyFast(register const void *srcp, register void *destp, register u32 size);
void MI_Copy32B(register const void *pSrc, register void *pDest);
void MI_Copy36B(register const void *pSrc, register void *pDest);
void MI_Copy48B(register const void *pSrc, register void *pDest);
void MI_Copy64B(register const void *pSrc, register void *pDest);
void MI_CpuFill8(register void *dstp, register u8 data, register u32 size);
void MI_CpuCopy8(register const void *srcp, register void *dstp, register u32 size);
void MI_Zero36B(register void *pDest);

#include <nitro/code32.h>

// @fn MIi_CpuClear16
asm void MIi_CpuClear16(register u16 data, register void *destp, register u32 size) {
    mov r3, #0
@L020D4794:
    cmp r3, r2
    blt @L020D47A0
    b @L020D47A4
@L020D47A0:
    strh r0, [r1, r3]
@L020D47A4:
    blt @L020D47AC
    b @L020D47B0
@L020D47AC:
    add r3, r3, #2
@L020D47B0:
    blt @L020D4794
    bx lr
}

// @fn MIi_CpuCopy16
asm void MIi_CpuCopy16(register const void *srcp, register void *destp, register u32 size) {
    mov r12, #0
@L020D47BC:
    cmp r12, r2
    blt @L020D47C8
    b @L020D47CC
@L020D47C8:
    ldrh r3, [r0, r12]
@L020D47CC:
    blt @L020D47D4
    b @L020D47D8
@L020D47D4:
    strh r3, [r1, r12]
@L020D47D8:
    blt @L020D47E0
    b @L020D47E4
@L020D47E0:
    add r12, r12, #2
@L020D47E4:
    blt @L020D47BC
    bx lr
}

// @fn MIi_CpuClear32
asm void MIi_CpuClear32(register u32 data, register void *destp, register u32 size) {
    add r12, r1, r2
@L020D47F0:
    cmp r1, r12
    blt @L020D47FC
    b @L020D4800
@L020D47FC:
    stmia r1!, {r0}
@L020D4800:
    blt @L020D47F0
    bx lr
}

// @fn MIi_CpuCopy32
asm void MIi_CpuCopy32(register const void *srcp, register void *destp, register u32 size) {
    add r12, r1, r2
@L020D480C:
    cmp r1, r12
    blt @L020D4818
    b @L020D481C
@L020D4818:
    ldmia r0!, {r2}
@L020D481C:
    blt @L020D4824
    b @L020D4828
@L020D4824:
    stmia r1!, {r2}
@L020D4828:
    blt @L020D480C
    bx lr
}

// @fn MIi_CpuSend32
asm void MIi_CpuSend32(register const void *srcp, volatile void *destp, u32 size) {
    add r12, r0, r2
@L020D4834:
    cmp r0, r12
    blt @L020D4840
    b @L020D4844
@L020D4840:
    ldmia r0!, {r2}
@L020D4844:
    blt @L020D484C
    b @L020D4850
@L020D484C:
    str r2, [r1]
@L020D4850:
    blt @L020D4834
    bx lr
}

// @fn MIi_CpuClearFast
asm void MIi_CpuClearFast(register u32 data, register void *destp, register u32 size) {
    stmdb sp!, {r4, r5, r6, r7, r8, r9}
    add r9, r1, r2
    mov r12, r2, lsr #5
    add r12, r1, r12, lsl #5
    mov r2, r0
    mov r3, r2
    mov r4, r2
    mov r5, r2
    mov r6, r2
    mov r7, r2
    mov r8, r2
@L020D4884:
    cmp r1, r12
    blt @L020D4890
    b @L020D4894
@L020D4890:
    stmia r1!, {r0, r2, r3, r4, r5, r6, r7, r8}
@L020D4894:
    blt @L020D4884
@L020D4898:
    cmp r1, r9
    blt @L020D48A4
    b @L020D48A8
@L020D48A4:
    stmia r1!, {r0}
@L020D48A8:
    blt @L020D4898
    ldmia sp!, {r4, r5, r6, r7, r8, r9}
    bx lr
}

// @fn MIi_CpuCopyFast
asm void MIi_CpuCopyFast(register const void *srcp, register void *destp, register u32 size) {
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10}
    add r10, r1, r2
    mov r12, r2, lsr #5
    add r12, r1, r12, lsl #5
@L020D48C4:
    cmp r1, r12
    blt @L020D48D0
    b @L020D48D4
@L020D48D0:
    ldmia r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
@L020D48D4:
    blt @L020D48DC
    b @L020D48E0
@L020D48DC:
    stmia r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
@L020D48E0:
    blt @L020D48C4
@L020D48E4:
    cmp r1, r10
    blt @L020D48F0
    b @L020D48F4
@L020D48F0:
    ldmia r0!, {r2}
@L020D48F4:
    blt @L020D48FC
    b @L020D4900
@L020D48FC:
    stmia r1!, {r2}
@L020D4900:
    blt @L020D48E4
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10}
    bx lr
}

// @fn MI_Copy32B
asm void MI_Copy32B(register const void *pSrc, register void *pDest) {
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3}
    stmia r1!, {r2, r3}
    bx lr
}

// @fn MI_Copy36B
asm void MI_Copy36B(register const void *pSrc, register void *pDest) {
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    bx lr
}

// @fn MI_Copy48B
asm void MI_Copy48B(register const void *pSrc, register void *pDest) {
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    bx lr
}

// @fn MI_Copy64B
asm void MI_Copy64B(register const void *pSrc, register void *pDest) {
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0, {r0, r2, r3, r12}
    stmia r1!, {r0, r2, r3, r12}
    bx lr
}

// @fn MI_CpuFill8
asm void MI_CpuFill8(register void *dstp, register u8 data, register u32 size) {
    cmp r2, #0
    beq @L020D49A0
    b @L020D49A4
@L020D49A0:
    bx lr
@L020D49A4:
    tst r0, #1
    beq @L020D49D0
    ldrh r12, [r0, #-1]
    and r12, r12, #0xff
    orr r3, r12, r1, lsl #8
    strh r3, [r0, #-1]
    add r0, r0, #1
    subs r2, r2, #1
    beq @L020D49CC
    b @L020D49D0
@L020D49CC:
    bx lr
@L020D49D0:
    cmp r2, #2
    bcc @L020D4A28
    orr r1, r1, r1, lsl #8
    tst r0, #2
    beq @L020D49F8
    strh r1, [r0], #2
    subs r2, r2, #2
    beq @L020D49F4
    b @L020D49F8
@L020D49F4:
    bx lr
@L020D49F8:
    orr r1, r1, r1, lsl #16
    bics r3, r2, #3
    beq @L020D4A18
    sub r2, r2, r3
    add r12, r3, r0
@L020D4A0C:
    str r1, [r0], #4
    cmp r0, r12
    bcc @L020D4A0C
@L020D4A18:
    tst r2, #2
    bne @L020D4A24
    b @L020D4A28
@L020D4A24:
    strh r1, [r0], #2
@L020D4A28:
    tst r2, #1
    beq @L020D4A34
    b @L020D4A38
@L020D4A34:
    bx lr
@L020D4A38:
    ldrh r3, [r0]
    and r3, r3, #0xff00
    and r1, r1, #0xff
    orr r1, r1, r3
    strh r1, [r0]
    bx lr
}

// @fn MI_CpuCopy8
asm void MI_CpuCopy8(register const void *srcp, register void *dstp, register u32 size) {
    cmp r2, #0
    beq @L020D4A5C
    b @L020D4A60
@L020D4A5C:
    bx lr
@L020D4A60:
    tst r1, #1
    beq @L020D4AB8
    ldrh r12, [r1, #-1]
    and r12, r12, #0xff
    tst r0, #1
    bne @L020D4A7C
    b @L020D4A80
@L020D4A7C:
    ldrh r3, [r0, #-1]
@L020D4A80:
    bne @L020D4A88
    b @L020D4A8C
@L020D4A88:
    mov r3, r3, lsr #8
@L020D4A8C:
    beq @L020D4A94
    b @L020D4A98
@L020D4A94:
    ldrh r3, [r0]
@L020D4A98:
    orr r3, r12, r3, lsl #8
    strh r3, [r1, #-1]
    add r0, r0, #1
    add r1, r1, #1
    subs r2, r2, #1
    beq @L020D4AB4
    b @L020D4AB8
@L020D4AB4:
    bx lr
@L020D4AB8:
    eor r12, r1, r0
    tst r12, #1
    beq @L020D4B14
    bic r0, r0, #1
    ldrh r12, [r0], #2
    mov r3, r12, lsr #8
    subs r2, r2, #2
    bcc @L020D4AF0
@L020D4AD8:
    ldrh r12, [r0], #2
    orr r12, r3, r12, lsl #8
    strh r12, [r1], #2
    mov r3, r12, lsr #0x10
    subs r2, r2, #2
    bcs @L020D4AD8
@L020D4AF0:
    tst r2, #1
    beq @L020D4AFC
    b @L020D4B00
@L020D4AFC:
    bx lr
@L020D4B00:
    ldrh r12, [r1]
    and r12, r12, #0xff00
    orr r12, r12, r3
    strh r12, [r1]
    bx lr
@L020D4B14:
    tst r12, #2
    beq @L020D4B40
    bics r3, r2, #1
    beq @L020D4BA4
    sub r2, r2, r3
    add r12, r3, r1
@L020D4B2C:
    ldrh r3, [r0], #2
    strh r3, [r1], #2
    cmp r1, r12
    bcc @L020D4B2C
    b @L020D4BA4
@L020D4B40:
    cmp r2, #2
    bcc @L020D4BA4
    tst r1, #2
    beq @L020D4B68
    ldrh r3, [r0], #2
    strh r3, [r1], #2
    subs r2, r2, #2
    beq @L020D4B64
    b @L020D4B68
@L020D4B64:
    bx lr
@L020D4B68:
    bics r3, r2, #3
    beq @L020D4B88
    sub r2, r2, r3
    add r12, r3, r1
@L020D4B78:
    ldr r3, [r0], #4
    str r3, [r1], #4
    cmp r1, r12
    bcc @L020D4B78
@L020D4B88:
    tst r2, #2
    bne @L020D4B94
    b @L020D4B98
@L020D4B94:
    ldrh r3, [r0], #2
@L020D4B98:
    bne @L020D4BA0
    b @L020D4BA4
@L020D4BA0:
    strh r3, [r1], #2
@L020D4BA4:
    tst r2, #1
    beq @L020D4BB0
    b @L020D4BB4
@L020D4BB0:
    bx lr
@L020D4BB4:
    ldrh r2, [r1]
    ldrh r0, [r0]
    and r2, r2, #0xff00
    and r0, r0, #0xff
    orr r0, r2, r0
    strh r0, [r1]
    bx lr
}

#include <nitro/code16.h>

// @fn MI_Zero36B
asm void MI_Zero36B(register void *pDest) {
    mov r1, #0
    mov r2, #0
    mov r3, #0
    stmia r0!, {r1, r2, r3}
    stmia r0!, {r1, r2, r3}
    stmia r0!, {r1, r2, r3}
    bx lr
}
