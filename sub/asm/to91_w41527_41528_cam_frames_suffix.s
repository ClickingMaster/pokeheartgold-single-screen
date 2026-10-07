	.include "asm/macros.inc"
	.include "global.inc"

	.extern CAM_ClrTIMElementBitmap
	.extern CAM_GetAID
	.extern CAM_ReleaseAID
	.extern CAM_SetDoze
	.extern CAM_SetTIMElementBitmap
	.extern __VENEER_OS_DisableIrqMask
	.extern __VENEER_OS_EnableIrqMask

.text

	arm_func_start CAM_SetStaState
CAM_SetStaState: ; 0x0600E4F0
	stmdb sp!, {r4, r5, r6, lr}
	mov r6, r0
	mov r0, #0x1000000
	mov r5, r1
	bl __VENEER_OS_DisableIrqMask
	ldr r3, _0600E5D4 ; =0x0380FFF4
	mov r4, r0
	cmp r5, #0x40
	bhs _0600E570
	ldr r0, [r3]
	mov r2, #1
	add r0, r0, #0x500
	ldrh r1, [r0, #0x30]
	orr r1, r1, r2, lsl r6
	strh r1, [r0, #0x30]
	ldr r0, [r3]
	add r0, r0, #0x500
	ldrh r1, [r0, #0x32]
	orr r1, r1, r2, lsl r6
	strh r1, [r0, #0x32]
	ldr r0, [r3]
	add r0, r0, #0x300
	ldrh r0, [r0, #0x50]
	cmp r0, #1
	bne _0600E5AC
	mov r0, r6
	bl CAM_GetAID
	cmp r0, #0
	beq _0600E5AC
	mov r0, r6
	bl CAM_ReleaseAID
	b _0600E5AC
_0600E570:
	ldr r2, [r3]
	mov r0, #1
	mvn r1, r0, lsl r6
	add r0, r2, #0x500
	ldrh r2, [r0, #0x32]
	and r1, r2, r1
	strh r1, [r0, #0x32]
	ldr r0, [r3]
	add r0, r0, #0x500
	ldrh r0, [r0, #0x2e]
	mov r0, r0, asr r6
	tst r0, #1
	beq _0600E5AC
	mov r0, r6
	bl CAM_SetDoze
_0600E5AC:
	ldr r1, _0600E5D4 ; =0x0380FFF4
	mov r0, #0x1c
	ldr r2, [r1]
	mul r1, r6, r0
	ldr r2, [r2, #0x31c]
	mov r0, r4
	strh r5, [r2, r1]
	bl __VENEER_OS_EnableIrqMask
	ldmia sp!, {r4, r5, r6, lr}
	bx lr
	.align 2, 0
_0600E5D4: .word 0x0380FFF4
	arm_func_end CAM_SetStaState
