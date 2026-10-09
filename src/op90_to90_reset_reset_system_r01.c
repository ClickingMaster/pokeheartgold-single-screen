#pragma force_active on
#include <nitro/types.h>

extern u32 SDK_AUTOLOAD_DTCM_START[];
extern u32 SDK_IRQ_STACKSIZE[];
void OS_Terminate(void);
s32 OS_GetLockID(void);
void CARD_LockRom(u16 lockID);
void MI_StopDma(u32 dmaNo);
u32 OS_SetIrqMask(u32 intr);
u32 OS_ResetRequestIrqMask(u32 intr);
void OSi_SendToPxi(u16 data);
void OSi_DoResetSystem(void);
void OS_ResetSystem(u32 parameter);

#include <nitro/code32.h>

// @fn OS_ResetSystem
asm void OS_ResetSystem(u32 parameter) {
    stmfd sp!, {r4, lr}
    ldr r1, =0x027ffc40
    mov r4, r0
    ldrh r0, [r1]
    cmp r0, #2
    moveq r0, #1
    movne r0, #0
    cmp r0, #0
    beq @1
    bl OS_Terminate
@1:
    bl OS_GetLockID
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    bl CARD_LockRom
    ldr r0, =0
    bl MI_StopDma
    ldr r0, =1
    bl MI_StopDma
    ldr r0, =2
    bl MI_StopDma
    ldr r0, =3
    bl MI_StopDma
    ldr r0, =0x00040000
    bl OS_SetIrqMask
    ldr r0, =0xfffbffff
    bl OS_ResetRequestIrqMask
    ldr r1, =0x027ffc20
    ldr r0, =0x10
    str r4, [r1]
    bl OSi_SendToPxi
    ldr r0, =SDK_AUTOLOAD_DTCM_START + 0x3f80
    ldr r1, =SDK_IRQ_STACKSIZE
    sub r0, r0, r1
    mov sp, r0
    bl OSi_DoResetSystem
    ldmfd sp!, {r4, pc}
}
