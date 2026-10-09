#include <nitro/types.h>
#include <nitro/code32.h>
extern u32 SDK_STATIC_BSS_END[];
extern u32 SDK_IRQ_STACKSIZE[];
extern u32 _start_ModuleParams[];
void OS_IrqHandler(void);
void NitroSpMain(void);
void detect_main_memory_size(void);
void _start_AutoloadDoneCallback(void);
void _start(void);
void do_autoload(void);
asm void _start(void)
{
    mov r12, #0x4000000
    str r12, [r12, #0x208]
    ldr r1, =SDK_STATIC_BSS_END
    mov r0, #0x3800000
    cmp r0, r1
    movpl r1, r0
    ldr r2, =0x0380FF00
    mov r0, #0
@clear:
    cmp r1, r2
    stmltia r1!, {r0}
    blt @clear
    mov r0, #0x13
    msr cpsr_c, r0
    ldr sp, =0x0380FFC0
    mov r0, #0x12
    msr cpsr_c, r0
    ldr r0, =0x0380FF80
    mov sp, r0
    ldr r1, =SDK_IRQ_STACKSIZE
    sub r1, r0, r1
    mov r0, #0x1f
    msr cpsr_fsxc, r0
    sub sp, r1, #4
    ldr r0, =0x023FE940
    ldr r1, =0x027FFA80
    add r2, r1, #0x160
@copy1:
    ldr r3, [r0], #4
    str r3, [r1], #4
    cmp r1, r2
    bmi @copy1
    ldr r0, =0x023FE904
    add r2, r1, #0x20
@copy2:
    ldr r3, [r0], #4
    str r3, [r1], #4
    cmp r1, r2
    bmi @copy2
    bl do_autoload
    ldr r0, =_start_ModuleParams
    ldr r1, [r0, #0xc]
    ldr r2, [r0, #0x10]
    mov r0, #0
@bss:
    cmp r1, r2
    strlo r0, [r1], #4
    blo @bss
    bl detect_main_memory_size
    ldr r1, =0x0380FFFC
    ldr r0, =OS_IrqHandler
    str r0, [r1]
    ldr r1, =NitroSpMain
    ldr lr, =0xFFFF0000
    bx r1
}
asm void do_autoload(void)
{
    ldr r0, =_start_ModuleParams
    ldr r1, [r0]
    ldr r2, [r0, #4]
    ldr r3, [r0, #8]
@next:
    cmp r1, r2
    beq @done
    ldr r4, [r1], #4
    ldr r5, [r1], #4
    add r6, r4, r5
@copy:
    cmp r4, r6
    ldrmi r7, [r3], #4
    strmi r7, [r4], #4
    bmi @copy
    ldr r5, [r1], #4
    add r6, r4, r5
    mov r7, #0
@clear:
    cmp r4, r6
    strlo r7, [r4], #4
    blo @clear
    beq @next
@done:
    b _start_AutoloadDoneCallback
}
