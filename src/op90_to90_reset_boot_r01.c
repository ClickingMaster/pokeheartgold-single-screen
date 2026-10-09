#pragma force_active on
#include <nitro/types.h>

extern u32 SDK_AUTOLOAD_DTCM_START[];
u32 OSi_CpuClear32(register u32 value, register void *ptr, register u32 size);
void OSi_DoBoot(void);

#include <nitro/code32.h>
#include <nitro/itcm_begin.h>

// @fn OSi_DoBoot
asm void OSi_DoBoot(void) {
    mov r12, #0x04000000
    str r12, [r12, #0x208]
    ldr r1, =SDK_AUTOLOAD_DTCM_START
    add r1, r1, #0x3fc0
    add r1, r1, #0x3c
    mov r0, #0
    str r0, [r1]
    ldr r1, =0x04000180
@1:
    ldrh r0, [r1]
    and r0, r0, #0xf
    cmp r0, #1
    bne @1
    mov r0, #0x100
    strh r0, [r1]
    mov r0, #0
    ldr r3, =0x027ffd9c
    ldr r4, [r3]
    ldr r1, =0x027ffd80
    mov r2, #0x80
    bl OSi_CpuClear32
    str r4, [r3]
    ldr r1, =0x027fff80
    mov r2, #0x18
    bl OSi_CpuClear32
    ldr r1, =0x027fff98
    strh r0, [r1]
    ldr r1, =0x027fff9c
    mov r2, #0x64
    bl OSi_CpuClear32
    ldr r1, =0x04000180
@2:
    ldrh r0, [r1]
    and r0, r0, #0xf
    cmp r0, #1
    beq @2
    mov r0, #0
    strh r0, [r1]
    ldr r3, =0x027ffe00
    ldr r12, [r3, #0x24]
    mov lr, r12
    ldr r11, =0x027fff80
    ldmia r11, {r0-r10}
    mov r11, #0
    bx r12
}

#include <nitro/itcm_end.h>
