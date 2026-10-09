#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void detect_main_memory_size(void);
// @fn detect_main_memory_size
asm void detect_main_memory_size(void)
{
    mov r0, #1
    mov r1, #0
    ldr r2, =0x027FFFFA
    sub r3, r2, #0x400000
@loop:
    strh r1, [r2]
    ldrh r12, [r3]
    cmp r1, r12
    movne r0, #2
    bne @done
    add r1, r1, #1
    cmp r1, #2
    bne @loop
@done:
    strh r0, [r2]
    bx lr
}
