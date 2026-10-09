#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
extern u8 micw[];
int SPIi_CheckEntry(void);
BOOL SPIi_CheckException(int device);
u16 MIC_ExecSampling12(void);
u16 MIC_ExecSampling8(void);
void SPIi_ReturnResult(int command, int result);
BOOL SPIi_SetEntry(int device, int command, int numArgs, ...);
void MicTimerHandler(void);
// @fn MicTimerHandler
asm void MicTimerHandler(void)
{
    stmdb sp!, {r4, r5, r6, r7, lr}
    ldr r4, =micw
    ldrh r5, [r4, #0x26]
    and r0, r5, #4
    cmp r0, #4
    ldrh r6, [r4, #0x38]
    ldrneh r7, [r4, #0x3a]
    ldreq r7, =0x0000FFFF
    bl SPIi_CheckEntry
    cmp r0, #0
    bne @L03802964
    mov r0, #2
    bl SPIi_CheckException
    cmp r0, #0
    beq @L03802964
    and r0, r5, #1
    cmp r0, #1
    bne @L03802954
    bl MIC_ExecSampling12
    tst r5, #2
    moveq r7, r0
    eorne r7, r0, #0x8000
    b @L03802964
@L03802954:
    bl MIC_ExecSampling8
    tst r5, #2
    moveq r7, r0
    eorne r7, r0, #0x80
@L03802964:
    and r0, r5, #1
    ldr r3, =0x027FFC00
    ldr r1, [r4, #0x2c]
    cmp r0, #1
    bne @L03802994
    ldr r2, [r4, #0x28]
    strh r7, [r2, r1]!
    str r2, [r3, #0x390]
    add r3, r3, #0x394
    strh r7, [r3]
    add r1, r1, #2
    b @L038029CC
@L03802994:
    and r7, r7, #0xff
    tst r1, #1
    bne @L038029AC
    mov r6, r7
    add r1, r1, #1
    b @L038029CC
@L038029AC:
    orr r0, r6, r7, lsl #8
    ldr r2, [r4, #0x28]
    sub r1, r1, #1
    strh r0, [r2, r1]!
    str r2, [r3, #0x390]
    add r3, r3, #0x394
    strh r0, [r3]
    add r1, r1, #2
@L038029CC:
    strh r6, [r4, #0x38]
    strh r7, [r4, #0x3a]
    ldr r0, [r4, #0x30]
    cmp r1, r0
    movcs r1, #0
    str r1, [r4, #0x2c]
    bcc @L03802A48
    ldrh r0, [r4, #0x24]
    and r0, r0, #0x10
    cmp r0, #0x10
    bne @L03802A08
    mov r0, #0x51
    mov r1, #0
    bl SPIi_ReturnResult
    b @L03802A48
@L03802A08:
    mov r0, #2
    mov r1, #0x42
    mov r2, #0
    bl SPIi_SetEntry
    cmp r0, #0
    bne @L03802A30
    mov r0, #0x51
    mov r1, #4
    bl SPIi_ReturnResult
    b @L03802A48
@L03802A30:
    mov r0, #4
    str r0, [r4, #0x20]
    ldr r1, =0x0400010E
    ldrh r0, [r1]
    bic r0, r0, #0x80
    strh r0, [r1]
@L03802A48:
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
}
