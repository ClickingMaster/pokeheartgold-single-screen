#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
extern u32 OS_IRQTable[];
extern u8 OSi_ThreadInfo[];
extern u32 micIntrInfo[];
extern u32 micIntrPrio[];
void OS_IrqHandler_ThreadSwitch(void);
void MIC_IrqHandler(void);
// @fn MIC_IrqHandler
asm void MIC_IrqHandler(void)
{
    mov r12, #0x4000000
    add r1, r12, #0x208
    ldrh r0, [r1]
    tst r0, r0
    bxeq lr
    ldr r3, [r12, #0x210]
    ldr r1, [r12, #0x214]
    ands r2, r1, r3
    bxeq lr
    ldr r0, =0x01DF3FFF
    tst r2, r0
    streq r2, [r12, #0x214]
    bxeq lr
    stmdb sp!, {lr}
    mrs r0, spsr
    stmdb sp!, {r0}
    stmdb sp, {sp, lr}^
    sub sp, sp, #8
    mov r0, #0x9f
    msr cpsr_c, r0
    ldr r1, =OSi_ThreadInfo
    ldrh r0, [r1, #2]
    add r0, r0, #1
    strh r0, [r1, #2]
    ldr r1, =micIntrInfo
    cmp r0, #1
    moveq r0, sp
    ldreq sp, [r1, #4]
    streq r0, [r1, #4]
    stmdb sp!, {r3}
    ldr r1, =micIntrPrio
    ldr r0, [r1]
    tst r0, r2
    strne r0, [r12, #0x214]
    ldrne r0, [r1, #4]
    ldrne r3, =OS_IRQTable
    ldrne r0, [r3, r0, lsl #2]
    bne @L03802E90
    mov r3, #1
@L03802E50:
    ldr r0, [r1, r3, lsl #3]
    tst r0, r2
    addeq r3, r3, #1
    beq @L03802E50
    str r0, [r12, #0x214]
    add r0, r1, r3, lsl #3
    ldr r2, [r0, #4]
    ldr r3, =OS_IRQTable
    ldr r0, [r3, r2, lsl #2]
    ldr r2, =OSi_ThreadInfo
    ldrh r3, [r2, #2]
    cmp r3, #1
    ldreq r2, [r1]
    streq r2, [r12, #0x210]
    moveq r2, #0x1f
    msreq cpsr_c, r2
@L03802E90:
    ldr r1, [r12, #0x210]
    stmdb sp!, {r1}
    add lr, pc, #0
    bx r0
    mov r0, #0x9f
    msr cpsr_c, r0
    mov r12, #0x4000000
    ldmia sp!, {r0}
    ldr r1, [r12, #0x210]
    eor r2, r0, r1
    and r1, r2, r1
    and r0, r2, r0
    ldmia sp!, {r3}
    orr r3, r3, r1
    bic r3, r3, r0
    str r3, [r12, #0x210]
    ldr r2, =OSi_ThreadInfo
    ldr r3, =micIntrInfo
    ldrh r0, [r2, #2]
    subs r1, r0, #1
    strh r1, [r2, #2]
    moveq r0, sp
    ldreq sp, [r3, #4]
    streq r0, [r3, #4]
    mov r0, #0x92
    msr cpsr_c, r0
    ldmia sp, {sp, lr}^
    mov r0, r0
    add sp, sp, #8
    ldmia sp!, {r0}
    msr spsr_fc, r0
    tst r1, r1
    ldreq r0, =OS_IrqHandler_ThreadSwitch
    addeq lr, pc, #0
    bxeq r0
    ldmia sp!, {pc}
}
