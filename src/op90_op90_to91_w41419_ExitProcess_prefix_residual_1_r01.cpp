#pragma force_active on
#include <nitro/types.h>
struct ThrowContext;
struct ExceptionInfo;
void __TransferControl(ThrowContext *context, ExceptionInfo *info, char *address);
#include <nitro/code32.h>
// @fn _Z17__TransferControlP12ThrowContextP13ExceptionInfoPc
asm void __TransferControl(ThrowContext *context, ExceptionInfo *info, char *address)
{
    ldr r4, [r0, #0x2c]
    ldr r5, [r0, #0x30]
    ldr r6, [r0, #0x34]
    ldr r7, [r0, #0x38]
    ldr r8, [r0, #0x3c]
    ldr r9, [r0, #0x40]
    ldr r10, [r0, #0x44]
    ldr r11, [r0, #0x48]
    ldr sp, [r0, #0x5c]
    ldr r12, [r0, #0x64]
    sub sp, sp, r12
    bx r2
}
#include <nitro/codereset.h>
