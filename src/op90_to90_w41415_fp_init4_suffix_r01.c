#pragma force_active on
#include <nitro/types.h>
void sys_writec(int c);
int sys_readc(void);
void sys_exit(void);

#include <nitro/code32.h>

// @fn sys_writec
asm void sys_writec(int c) {
    str lr, [sp, #-4]!
    mov r1, r0
    mov r0, #3
    swi 0x123456
    ldr pc, [sp], #4
}

// @fn sys_readc
asm int sys_readc(void) {
    str lr, [sp, #-4]!
    mov r1, #0
    mov r0, #7
    swi 0x123456
    ldr pc, [sp], #4
}

// @fn sys_exit
asm void sys_exit(void) {
    mov r1, #0
    mov r0, #0x18
    swi 0x123456
    mov pc, lr
}
