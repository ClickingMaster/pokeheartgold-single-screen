	.include "asm/macros.inc"
	.include "global.inc"

	.extern MIC_DisableMultipleInterrupt
	.extern MIC_EnableMultipleInterrupt
	.extern MIC_ExecSampling12
	.extern MIC_ExecSampling8
	.extern MIC_SetIrqFunction
	.extern MIC_TimerHandler
	.extern OS_DisableInterrupts
	.extern OS_EnableIrqMask
	.extern OS_RestoreInterrupts
	.extern SPIi_CheckException
	.extern SPIi_GetException
	.extern SPIi_ReleaseException
	.extern SPIi_ReturnResult
	.extern micw

.text

	arm_func_start MIC_ExecuteProcess
MIC_ExecuteProcess: ; 0x0380269C
	stmdb sp!, {r3, r4, r5, lr}
	mov r4, r0
	ldr r2, [r4, #4]
	cmp r2, #0x40
	beq _038026C4
	cmp r2, #0x41
	beq _03802774
	cmp r2, #0x42
	beq _0380280C
	b _038028A8
_038026C4:
	bl OS_DisableInterrupts
	mov r5, r0
	mov r0, #2
	bl SPIi_CheckException
	cmp r0, #0
	bne _038026FC
	mov r0, r5
	bl OS_RestoreInterrupts
	ldr r0, [r4, #4]
	mov r1, #4
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	bl SPIi_ReturnResult
	b _038028A8
_038026FC:
	mov r0, #2
	bl SPIi_GetException
	mov r0, r5
	bl OS_RestoreInterrupts
	ldr r0, [r4, #8]
	and r0, r0, #1
	cmp r0, #1
	bne _03802730
	bl MIC_ExecSampling12
	ldr r1, [r4, #8]
	tst r1, #2
	eorne r0, r0, #0x8000
	b _03802740
_03802730:
	bl MIC_ExecSampling8
	ldr r1, [r4, #8]
	tst r1, #2
	eorne r0, r0, #0x80
_03802740:
	ldr r1, _038028B0 ; =0x027FFF94
	movne r0, r0, lsl #0x10
	movne r0, r0, lsr #0x10
	strh r0, [r1]
	str r1, [r1, #-4]
	ldr r0, [r4, #4]
	mov r1, #0
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	bl SPIi_ReturnResult
	mov r0, #2
	bl SPIi_ReleaseException
	b _038028A8
_03802774:
	ldr r0, _038028B4 ; =micw
	ldr r1, [r0, #0x20]
	cmp r1, #1
	bne _038027F8
	mov r1, #0
	strh r1, [r0, #0x3a]
	strh r1, [r0, #0x38]
	bl OS_DisableInterrupts
	mov r5, r0
	mov r0, #0x40
	bl OS_EnableIrqMask
	ldr r1, _038028B8 ; =MIC_TimerHandler
	mov r0, #0x40
	bl MIC_SetIrqFunction
	bl MIC_EnableMultipleInterrupt
	ldr r1, _038028B4 ; =micw
	ldr r2, _038028BC ; =0x0400010C
	ldrh r3, [r1, #0x34]
	mov r0, r5
	strh r3, [r2]
	ldrh r1, [r1, #0x36]
	orr r1, r1, #0xc0
	strh r1, [r2, #2]
	bl OS_RestoreInterrupts
	ldr r0, [r4, #4]
	mov r1, #0
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	bl SPIi_ReturnResult
	ldr r0, _038028B4 ; =micw
	mov r1, #2
	str r1, [r0, #0x20]
	b _038028A8
_038027F8:
	mov r0, r2, lsl #0x10
	mov r0, r0, lsr #0x10
	mov r1, #3
	bl SPIi_ReturnResult
	b _038028A8
_0380280C:
	ldr r0, _038028B4 ; =micw
	ldr r1, [r0, #0x20]
	sub r0, r1, #3
	cmp r0, #1
	bhi _03802888
	ldr r1, _038028C0 ; =0x0400010E
	ldrh r0, [r1]
	bic r0, r0, #0x80
	strh r0, [r1]
	bl OS_DisableInterrupts
	mov r4, r0
	mov r0, #0x40
	mov r1, #0
	bl MIC_SetIrqFunction
	bl MIC_DisableMultipleInterrupt
	mov r0, r4
	bl OS_RestoreInterrupts
	ldr r0, _038028B4 ; =micw
	mov r1, #0
	ldr r0, [r0, #0x20]
	cmp r0, #3
	bne _03802870
	mov r0, #0x42
	bl SPIi_ReturnResult
	b _03802878
_03802870:
	mov r0, #0x51
	bl SPIi_ReturnResult
_03802878:
	ldr r0, _038028B4 ; =micw
	mov r1, #0
	str r1, [r0, #0x20]
	b _038028A8
_03802888:
	cmp r1, #3
	mov r1, #3
	bne _038028A0
	mov r0, #0x42
	bl SPIi_ReturnResult
	b _038028A8
_038028A0:
	mov r0, #0x51
	bl SPIi_ReturnResult
_038028A8:
	ldmia sp!, {r3, r4, r5, lr}
	bx lr
	.align 2, 0
_038028B0: .word 0x027FFF94
_038028B4: .word micw
_038028B8: .word MIC_TimerHandler
_038028BC: .word 0x0400010C
_038028C0: .word 0x0400010E
	arm_func_end MIC_ExecuteProcess
