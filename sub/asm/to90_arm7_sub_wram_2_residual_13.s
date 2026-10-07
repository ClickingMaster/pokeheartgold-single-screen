	.include "asm/macros.inc"
	.include "global.inc"

	.extern CTRDGi_SendtoPxi
	.extern OS_GetLockID
	.extern OS_LockCartridge
	.extern OS_ReleaseLockID
	.extern OS_SetIrqMask
	.extern OS_UnLockCartridge
	.extern VENEER_SVC_WaitByLoop
	.extern lock_id

.text

	arm_func_start CTRDGi_InitModuleInfo
CTRDGi_InitModuleInfo: ; 0x038032C8
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, lr}
	ldr r0, _03803424 ; =lock_id
	ldr r1, [r0, #8]
	cmp r1, #0
	bne _0380341C
	ldr r1, _03803428 ; =0x04000300
	mov r2, #1
	ldrh r1, [r1]
	str r2, [r0, #8]
	tst r1, #1
	beq _0380341C
	mov r0, #0x40000
	bl OS_SetIrqMask
	ldr r2, _0380342C ; =0x04000208
	mov r1, #1
	ldrh sb, [r2]
	ldr r5, _03803424 ; =lock_id
	mov r4, r0
	strh r1, [r2]
	mov r6, #0x100
	b _03803324
_0380331C:
	mov r0, r6
	bl VENEER_SVC_WaitByLoop
_03803324:
	ldr r0, [r5, #0x28]
	cmp r0, #1
	bne _0380331C
	ldr r0, _03803424 ; =lock_id
	mov r1, #0x8000000
	ldr r2, [r0, #0x18]
	ldr r0, _03803430 ; =0x01FFFFC0
	add r8, r1, #4
	and r0, r2, r0
	mov r0, r0, lsr #6
	mov r0, r0, lsl #5
	add r5, r0, #0x2000000
	mov r6, #1
	bl OS_GetLockID
	mov r0, r0, lsl #0x10
	mov r7, r0, lsr #0x10
	mov r0, r7
	bl OS_LockCartridge
	ldr ip, _03803434 ; =0x0000FFFF
	mov sl, #0
	eor r0, ip, #3
	eor r1, ip, #0x84
	mov r0, r0, lsl #0x10
	mov r1, r1, lsl #0x10
	mov r2, r0, lsr #0x10
	mov r3, r1, lsr #0x10
	b _038033CC
_03803390:
	mov lr, ip
	cmp sl, #0x4c
	moveq lr, r3
	beq _038033A8
	cmp sl, #0x4d
	moveq lr, r2
_038033A8:
	add r0, r5, sl, lsl #1
	mov r1, sl, lsl #1
	ldrh r0, [r0, #4]
	ldrh r1, [r8, r1]
	and r0, lr, r0
	cmp r0, r1
	movne r6, #0
	bne _038033D4
	add sl, sl, #1
_038033CC:
	cmp sl, #0x4e
	blt _03803390
_038033D4:
	mov r0, r7
	bl OS_UnLockCartridge
	mov r0, r7
	bl OS_ReleaseLockID
	ldr r2, _03803438 ; =0x027FFC30
	and r0, r6, #0xff
	ldrb r1, [r2, #5]
	and r0, r0, #1
	bic r1, r1, #1
	orr r1, r1, r0
	mov r0, #1
	strb r1, [r2, #5]
	bl CTRDGi_SendtoPxi
	ldr r2, _0380342C ; =0x04000208
	mov r0, r4
	ldrh r1, [r2]
	strh sb, [r2]
	bl OS_SetIrqMask
_0380341C:
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, lr}
	bx lr
	.align 2, 0
_03803424: .word lock_id
_03803428: .word 0x04000300
_0380342C: .word 0x04000208
_03803430: .word 0x01FFFFC0
_03803434: .word 0x0000FFFF
_03803438: .word 0x027FFC30
	arm_func_end CTRDGi_InitModuleInfo
