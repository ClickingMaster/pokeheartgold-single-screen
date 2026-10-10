#pragma force_active on
#include <nitro/types.h>

int ov00_021FAFC8(void);
u32 ov00_021FCFEC(void *p);
int SOC_SetResolver(void *dns1, void *dns2);
void ov00_021FD094(register void *ctx);

#include <nitro/code32.h>

// @fn ov00_021FD094
asm void ov00_021FD094(register void *ctx) {
    stmdb sp!, {r4, lr}
    sub sp, sp, #8
    mov r4, r0
    ldrb r0, [r4, #0xd0d]
    cmp r0, #6
    addhs sp, sp, #8
    ldmhsia sp!, {r4, pc}
    bl ov00_021FAFC8
    add r4, r4, r0, lsl #8
    ldrb r0, [r4, #0xc0]
    ldrb r2, [r4, #0xc8]
    ldrb r1, [r4, #0xc9]
    cmp r0, #0
    ldrb r3, [r4, #0xca]
    add r1, r2, r1
    ldrb r2, [r4, #0xcb]
    add r1, r3, r1
    addne sp, sp, #8
    add r0, r2, r1
    ldmneia sp!, {r4, pc}
    cmp r0, #0
    addeq sp, sp, #8
    ldmeqia sp!, {r4, pc}
    add r0, r4, #0xc8
    bl ov00_021FCFEC
    str r0, [sp, #4]
    add r0, r4, #0xcc
    bl ov00_021FCFEC
    str r0, [sp]
    add r0, sp, #4
    add r1, sp, #0
    bl SOC_SetResolver
    add sp, sp, #8
    ldmia sp!, {r4, pc}
}
