	.include "asm/macros.inc"
	.include "global.inc"

	.extern CARDi_EnableFlag
	.extern CARDi_OnFifoRecv
	.extern CARDi_TaskThread
	.extern OS_CreateThread
	.extern OS_WakeupThreadDirect
	.extern PXI_SetFifoRecvCallback
	.extern cardi_common
	.extern status_checked$3825

.text

	arm_func_start CARDi_InitCommon
CARDi_InitCommon: ; 0x037FF5BC
	stmdb sp!, {r4, lr}
	sub sp, sp, #8
	ldr r4, _037FF660 ; =cardi_common
	mov r2, #0
	mvn r1, #2
	str r1, [r4, #0xc]
	mov r0, #4
	str r0, [r4, #0xf0]
	str r2, [r4, #0x10]
	str r2, [r4, #0x1c]
	str r2, [r4]
	str r2, [r4, #8]
	str r2, [r4, #0x18]
	str r2, [r4, #0x14]
	str r2, [r4, #0xf8]
	str r2, [r4, #0xf4]
	mov r0, #0x400
	str r0, [sp]
	ldr ip, [r4, #0xf0]
	ldr r1, _037FF664 ; =CARDi_TaskThread
	ldr r3, _037FF668 ; =status_checked$3825
	add r0, r4, #0x48
	str ip, [sp, #4]
	bl OS_CreateThread
	add r0, r4, #0x48
	bl OS_WakeupThreadDirect
	ldr r1, _037FF66C ; =CARDi_OnFifoRecv
	mov r0, #0xb
	bl PXI_SetFifoRecvCallback
	ldr r0, _037FF670 ; =0x027FFC40
	ldrh r0, [r0]
	cmp r0, #2
	moveq r0, #1
	movne r0, #0
	cmp r0, #0
	ldreq r0, _037FF674 ; =CARDi_EnableFlag
	moveq r1, #1
	streq r1, [r0]
	add sp, sp, #8
	ldmia sp!, {r4, lr}
	bx lr
	.align 2, 0
_037FF660: .word cardi_common
_037FF664: .word CARDi_TaskThread
_037FF668: .word status_checked$3825
_037FF66C: .word CARDi_OnFifoRecv
_037FF670: .word 0x027FFC40
_037FF674: .word CARDi_EnableFlag
	arm_func_end CARDi_InitCommon
