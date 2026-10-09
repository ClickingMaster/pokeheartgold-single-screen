#pragma force_active on
#include <nitro/types.h>
void _Z14__ThrowHandlerP12ThrowContext(void *ctx);
void __throw(char *throwtype, void *location, void *dtor);
void __rethrow(void);

#include <nitro/code32.h>

// @fn __throw
asm void __throw(char *throwtype, void *location, void *dtor) {
    mov r12, sp
    sub sp, sp, #0x70
    str r4, [sp, #0x2c]
    str r5, [sp, #0x30]
    str r6, [sp, #0x34]
    str r7, [sp, #0x38]
    str r8, [sp, #0x3c]
    str r9, [sp, #0x40]
    str r10, [sp, #0x44]
    str r11, [sp, #0x48]
    str r12, [sp, #0x14]
    str r12, [sp, #0x5c]
    str lr, [sp, #0x10]
    str r0, [sp]
    str r1, [sp, #4]
    str r2, [sp, #8]
    mov r0, sp
    b _Z14__ThrowHandlerP12ThrowContext
}

// @fn __rethrow
asm void __rethrow(void) {
    mov r12, sp
    sub sp, sp, #0x70
    str r4, [sp, #0x2c]
    str r5, [sp, #0x30]
    str r6, [sp, #0x34]
    str r7, [sp, #0x38]
    str r8, [sp, #0x3c]
    str r9, [sp, #0x40]
    str r10, [sp, #0x44]
    str r11, [sp, #0x48]
    str r12, [sp, #0x14]
    str r12, [sp, #0x5c]
    str lr, [sp, #0x10]
    mov r12, #0
    str r12, [sp]
    str r12, [sp, #4]
    str r12, [sp, #8]
    mov r0, sp
    b _Z14__ThrowHandlerP12ThrowContext
}
