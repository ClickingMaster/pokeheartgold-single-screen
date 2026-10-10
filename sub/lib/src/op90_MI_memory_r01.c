#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuCopy32(const void *src, void *dest, u32 size);
void MIi_CpuClearFast(u32 data, void *dest, u32 size);
void MIi_CpuCopyFast(const void *src, void *dest, u32 size);
void MI_CpuFill8(void *dest, u8 data, u32 size);
void MI_CpuCopy8(const void *src, void *dest, u32 size);
// @fn MIi_CpuClear16
void MIi_CpuClear16(u16 data, void *dest, u32 size)
{
    int i = 0;
    for (;;) {
        if (i >= (int)size) break;
        *(u16 *)((u8 *)dest + i) = data;
        i += 2;
    }
}
// @fn MIi_CpuCopy16
void MIi_CpuCopy16(const void *src, void *dest, u32 size)
{
    int i = 0;
    for (;;) {
        if (i >= (int)size) break;
        *(u16 *)((u8 *)dest + i) = *(const u16 *)((const u8 *)src + i);
        i += 2;
    }
}
// @fn MIi_CpuClear32
asm void MIi_CpuClear32(u32 data, void *dest, u32 size)
{
    add r12, r1, r2
@loop:
    cmp r1, r12
    stmltia r1!, {r0}
    blt @loop
    bx lr
}
// @fn MIi_CpuCopy32
asm void MIi_CpuCopy32(const void *src, void *dest, u32 size)
{
    add r12, r1, r2
@loop:
    cmp r1, r12
    ldmltia r0!, {r2}
    stmltia r1!, {r2}
    blt @loop
    bx lr
}
// @fn MIi_CpuClearFast
asm void MIi_CpuClearFast(u32 data, void *dest, u32 size)
{
    stmfd sp!, {r4-r9}
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
@loop32:
    cmp r1, r12
    stmltia r1!, {r0, r2-r8}
    blt @loop32
@loop4:
    cmp r1, r9
    stmltia r1!, {r0}
    blt @loop4
    ldmfd sp!, {r4-r9}
    bx lr
}
// @fn MIi_CpuCopyFast
asm void MIi_CpuCopyFast(const void *src, void *dest, u32 size)
{
    stmfd sp!, {r4-r10}
    add r10, r1, r2
    mov r12, r2, lsr #5
    add r12, r1, r12, lsl #5
@loop32:
    cmp r1, r12
    ldmltia r0!, {r2-r9}
    stmltia r1!, {r2-r9}
    blt @loop32
@loop4:
    cmp r1, r10
    ldmltia r0!, {r2}
    stmltia r1!, {r2}
    blt @loop4
    ldmfd sp!, {r4-r10}
    bx lr
}
// @fn MI_CpuFill8
asm void MI_CpuFill8(void *dest, u8 data, u32 size)
{
    cmp r2, #0
    bxeq lr
    tst r0, #1
    beq @aligned
    ldrh r12, [r0, #-1]
    and r12, r12, #0xff
    orr r3, r12, r1, lsl #8
    strh r3, [r0, #-1]
    add r0, r0, #1
    subs r2, r2, #1
    bxeq lr
@aligned:
    cmp r2, #2
    blo @tail
    orr r1, r1, r1, lsl #8
    tst r0, #2
    beq @word
    strh r1, [r0], #2
    subs r2, r2, #2
    bxeq lr
@word:
    orr r1, r1, r1, lsl #16
    bics r3, r2, #3
    beq @half
    sub r2, r2, r3
    add r12, r3, r0
@loop:
    str r1, [r0], #4
    cmp r0, r12
    blo @loop
@half:
    tst r2, #2
    strneh r1, [r0], #2
@tail:
    tst r2, #1
    bxeq lr
    ldrh r3, [r0]
    and r3, r3, #0xff00
    and r1, r1, #0xff
    orr r1, r1, r3
    strh r1, [r0]
    bx lr
}
// @fn MI_CpuCopy8
asm void MI_CpuCopy8(const void *src, void *dest, u32 size)
{
    cmp r2, #0
    bxeq lr
    tst r1, #1
    beq @l1
    ldrh r12, [r1, #-1]
    and r12, r12, #0xff
    tst r0, #1
    ldrneh r3, [r0, #-1]
    movne r3, r3, lsr #8
    ldreqh r3, [r0]
    orr r3, r12, r3, lsl #8
    strh r3, [r1, #-1]
    add r0, r0, #1
    add r1, r1, #1
    subs r2, r2, #1
    bxeq lr
@l1:
    eor r12, r1, r0
    tst r12, #1
    beq @l4
    bic r0, r0, #1
    ldrh r12, [r0], #2
    mov r3, r12, lsr #8
    subs r2, r2, #2
    blo @l3
@l2:
    ldrh r12, [r0], #2
    orr r12, r3, r12, lsl #8
    strh r12, [r1], #2
    mov r3, r12, lsr #16
    subs r2, r2, #2
    bhs @l2
@l3:
    tst r2, #1
    bxeq lr
    ldrh r12, [r1]
    and r12, r12, #0xff00
    orr r12, r12, r3
    strh r12, [r1]
    bx lr
@l4:
    tst r12, #2
    beq @l6
    bics r3, r2, #1
    beq @l9
    sub r2, r2, r3
    add r12, r3, r1
@l5:
    ldrh r3, [r0], #2
    strh r3, [r1], #2
    cmp r1, r12
    blo @l5
    b @l9
@l6:
    cmp r2, #2
    blo @l9
    tst r1, #2
    beq @l7
    ldrh r3, [r0], #2
    strh r3, [r1], #2
    subs r2, r2, #2
    bxeq lr
@l7:
    bics r3, r2, #3
    beq @l8
    sub r2, r2, r3
    add r12, r3, r1
@l8a:
    ldr r3, [r0], #4
    str r3, [r1], #4
    cmp r1, r12
    blo @l8a
@l8:
    tst r2, #2
    ldrneh r3, [r0], #2
    strneh r3, [r1], #2
@l9:
    tst r2, #1
    bxeq lr
    ldrh r2, [r1]
    ldrh r0, [r0]
    and r2, r2, #0xff00
    and r0, r0, #0xff
    orr r0, r2, r0
    strh r0, [r1]
    bx lr
}
