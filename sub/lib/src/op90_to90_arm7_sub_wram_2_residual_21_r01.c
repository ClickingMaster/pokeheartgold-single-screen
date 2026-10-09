#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void RTCi_GpioStart(void);
void RTCi_GpioEnd(void);
void RTCi_GpioSendCommand(u32 command, u32 param);
void RTCi_GpioSendData(void *data, u32 size);
void RTCi_GpioReceiveData(void *data, u32 size);
// @fn RTCi_GpioStart
asm void RTCi_GpioStart(void)
{
    mov r12, #0x4000000
    add r12, r12, #0x138
    ldrh r0, [r12]
    bic r0, r0, #0x77
    orr r0, r0, #0x72
    strh r0, [r12]
    mov r3, #2
@wait1:
    subs r3, r3, #1
    bne @wait1
    bic r0, r0, #4
    orr r0, r0, #4
    strh r0, [r12]
    mov r3, #2
@wait2:
    subs r3, r3, #1
    bne @wait2
    bx lr
}
// @fn RTCi_GpioEnd
asm void RTCi_GpioEnd(void)
{
    mov r12, #0x4000000
    add r12, r12, #0x138
    mov r3, #2
@wait1:
    subs r3, r3, #1
    bne @wait1
    ldrh r0, [r12]
    bic r0, r0, #4
    orr r0, r0, #0
    strh r0, [r12]
    mov r3, #2
@wait2:
    subs r3, r3, #1
    bne @wait2
    bx lr
}
// @fn RTCi_GpioSendCommand
asm void RTCi_GpioSendCommand(u32 command, u32 param)
{
    mov r12, #0x4000000
    add r12, r12, #0x138
    orr r1, r0, r1
    ldrh r0, [r12]
    bic r0, r0, #0x77
    orr r0, r0, #0x74
    mov r2, #0
@bit:
    bic r0, r0, #3
    orr r0, r0, #0
    mov r3, #1
    tst r3, r1, lsr r2
    movne r3, #1
    moveq r3, #0
    orr r0, r0, r3
    strh r0, [r12]
    mov r3, #9
@wait1:
    subs r3, r3, #1
    bne @wait1
    bic r0, r0, #2
    orr r0, r0, #2
    strh r0, [r12]
    mov r3, #9
@wait2:
    subs r3, r3, #1
    bne @wait2
    add r2, r2, #1
    cmp r2, #8
    bne @bit
    bx lr
}
// @fn RTCi_GpioSendData
asm void RTCi_GpioSendData(void *data, u32 size)
{
    mov r12, #0x4000000
    add r12, r12, #0x138
@byte:
    stmfd sp!, {r0, r1}
    tst r0, #1
    ldreqh r1, [r0]
    ldrneh r1, [r0, #-1]
    movne r1, r1, lsr #8
    ldrh r0, [r12]
    bic r0, r0, #0x77
    orr r0, r0, #0x74
    mov r2, #0
@bit:
    bic r0, r0, #3
    orr r0, r0, #0
    mov r3, #1
    tst r3, r1, lsr r2
    movne r3, #1
    moveq r3, #0
    orr r0, r0, r3
    strh r0, [r12]
    mov r3, #9
@wait1:
    subs r3, r3, #1
    bne @wait1
    bic r0, r0, #2
    orr r0, r0, #2
    strh r0, [r12]
    mov r3, #9
@wait2:
    subs r3, r3, #1
    bne @wait2
    add r2, r2, #1
    cmp r2, #8
    bne @bit
    ldmfd sp!, {r0, r1}
    add r0, r0, #1
    subs r1, r1, #1
    bne @byte
    bx lr
}
// @fn RTCi_GpioReceiveData
asm void RTCi_GpioReceiveData(void *data, u32 size)
{
    mov r12, #0x4000000
    add r12, r12, #0x138
@byte:
    stmfd sp!, {r0, r1}
    ldrh r0, [r12]
    bic r0, r0, #0x77
    orr r0, r0, #0x64
    mov r2, #0
    mov r1, #0
@bit:
    bic r0, r0, #3
    orr r0, r0, #0
    strh r0, [r12]
    mov r3, #9
@wait1:
    subs r3, r3, #1
    bne @wait1
    ldrh r0, [r12]
    and r3, r0, #1
    cmp r3, #1
    moveq r3, #0x80
    movne r3, #0
    orr r2, r3, r2, lsr #1
    bic r0, r0, #2
    orr r0, r0, #2
    strh r0, [r12]
    mov r3, #9
@wait2:
    subs r3, r3, #1
    bne @wait2
    add r1, r1, #1
    cmp r1, #8
    bne @bit
    ldmfd sp!, {r0, r1}
    tst r0, #1
    beq @even
    ldrh r3, [r0, #-1]
    bic r3, r3, #0xff00
    mov r2, r2, lsl #8
    orr r3, r2, r3
    strh r3, [r0, #-1]
    b @next
@even:
    ldrh r3, [r0]
    bic r3, r3, #0xff
    orr r3, r3, r2
    strh r3, [r0]
@next:
    add r0, r0, #1
    subs r1, r1, #1
    bne @byte
    bx lr
}
