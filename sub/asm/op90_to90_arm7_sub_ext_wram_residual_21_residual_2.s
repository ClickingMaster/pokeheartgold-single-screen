	.extern OS_Sleep
	.extern __VENEER_MATH_CountPopulation
	.extern __VENEER_OS_DisableInterrupts
	.extern __VENEER_OS_LockMutex
	.extern __VENEER_OS_RestoreInterrupts
	.extern __VENEER_OS_UnlockMutex
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern wmspW
	.include "asm/macros.inc"
	.include "global.inc"

.text
	arm_func_start WMSP_FlushSendQueue
WMSP_FlushSendQueue: ; 0x0600767C
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0x3c
	ldr r2, _060079A4 ; =wmspW + 0x1000
	str r0, [sp]
	ldr r8, [r2, #0x550]
	mov r5, r1
	ldrh r1, [r8]
	mov r0, #0
	cmp r1, #9
	add r4, r8, #0x2f8
	str r0, [sp, #0xc]
	moveq r6, #1
	beq _060076BC
	cmp r1, #0xa
	moveq r6, r0
	bne _06007998
_060076BC:
	add r0, r8, #0x31c
	add r0, r0, #0x400
	bl __VENEER_OS_LockMutex
	ldr r0, [r8, #0x734]
	cmp r0, #0
	bne _060076E8
	add r0, r8, #0x31c
	add r0, r0, #0x400
	bl __VENEER_OS_UnlockMutex
	mov r0, #0
	b _06007998
_060076E8:
	cmp r6, #0
	moveq r0, #1
	streq r0, [sp, #0x10]
	beq _0600770C
	bl __VENEER_OS_DisableInterrupts
	add r1, r8, #0x100
	ldrh r1, [r1, #0x82]
	str r1, [sp, #0x10]
	bl __VENEER_OS_RestoreInterrupts
_0600770C:
	mvn r0, r5
	str r0, [sp, #4]
	mov sl, #0
_06007718:
	add r0, r8, #0x2fc
	add r1, r0, #0x400
	ldr sb, _060079A8 ; =0x0000FFFF
	mov r0, sl, lsl #2
	ldrh r5, [r1, r0]
	add r0, r8, #0x100
	str sb, [sp, #8]
	add r6, r1, sl, lsl #2
	str sb, [sp, #0x14]
	str sb, [sp, #0x18]
	str sb, [sp, #0x1c]
	str sb, [sp, #0x20]
	str sb, [sp, #0x24]
	str sb, [sp, #0x28]
	str r0, [sp, #0x2c]
	str sb, [sp, #0x30]
	add fp, r8, #0x600
	str sb, [sp, #0x34]
	str sb, [sp, #0x38]
	b _0600792C
_06007768:
	ldr r0, [sp]
	add r7, r4, r5, lsl #5
	cmp r0, #0
	bne _060077A4
	ldrh r1, [r7, #0xa]
	ldr r0, [sp, #4]
	ldrh r2, [r7, #8]
	and r0, r1, r0
	orr r0, r2, r0
	strh r0, [r7, #8]
	ldrh r0, [r7, #8]
	ldrh r1, [r7, #6]
	mvn r0, r0
	and r0, r1, r0
	strh r0, [r7, #6]
_060077A4:
	ldrh r1, [r7, #6]
	ldr r0, [sp, #0x10]
	and r0, r1, r0
	strh r0, [r7, #6]
	mov r0, #0
	strh r0, [r7, #0xa]
	ldrh r0, [r7, #6]
	cmp r0, #0
	beq _06007840
	ldrh r0, [r7, #2]
	tst r0, #8
	ldreqh r0, [r7, #0x12]
	cmpeq r0, #0
	beq _06007840
	mov r0, #1
	str r0, [sp, #0xc]
	ldrh r0, [r7, #0x12]
	cmp r0, #0
	subne r0, r0, #1
	strneh r0, [r7, #0x12]
	ldrh r1, [r7]
	ldr r0, [sp, #0x24]
	cmp r1, r0
	ldreq r0, [sp, #0x14]
	streqh r0, [r6, #2]
	ldr r0, [sp, #0x28]
	cmp sb, r0
	ldrh r0, [r7]
	strh r0, [r6]
	ldr r0, [sp, #0x18]
	strh r0, [r7]
	moveq r0, r5, lsl #0x10
	moveq r0, r0, lsr #0x10
	streq r0, [sp, #8]
	movne r0, sb, lsl #5
	strneh r5, [r4, r0]
	mov r0, r5, lsl #0x10
	mov sb, r0, lsr #0x10
	b _06007928
_06007840:
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0x81
	strh r1, [r0]
	ldrh r1, [r7, #6]
	ldr r3, [sp, #0x2c]
	cmp r1, #0
	moveq r1, #0
	movne r1, #0xf
	strh r1, [r0, #2]
	mov r1, #0x14
	strh r1, [r0, #8]
	ldrh r1, [r7, #2]
	strh r1, [r0, #0xa]
	ldrh r1, [r7, #4]
	strh r1, [r0, #0xc]
	ldrh r1, [r7, #6]
	strh r1, [r0, #0xe]
	ldrh r1, [r7, #8]
	strh r1, [r0, #0x10]
	ldrh r1, [r7, #0xe]
	strh r1, [r0, #0x18]
	ldr r1, [r7, #0x14]
	str r1, [r0, #0x14]
	ldr r1, [r7, #0x18]
	str r1, [r0, #0x1c]
	ldr r1, [r7, #0x1c]
	str r1, [r0, #0x20]
	ldrh r1, [r7, #0x10]
	strh r1, [r0, #0x1a]
	ldrh r3, [r3, #0x88]
	ldrh r1, [r8, #0x30]
	cmp r3, #0
	ldrh r2, [r8, #0x32]
	moveq r3, r1
	movne r3, r2
	strh r3, [r0, #0x24]
	ldr r3, [sp, #0x2c]
	ldrh r3, [r3, #0x88]
	cmp r3, #0
	movne r2, r1
	strh r2, [r0, #0x26]
	bl __VENEER_WMSP_ReturnResult2Wm9
	ldrh r1, [r7]
	ldr r0, [sp, #0x30]
	cmp r1, r0
	ldreq r0, [sp, #0x1c]
	streqh r0, [r6, #2]
	ldrh r0, [r7]
	strh r0, [r6]
	ldr r0, [sp, #0x20]
	strh r0, [r7]
	ldrh r1, [fp, #0xfa]
	ldr r0, [sp, #0x34]
	cmp r1, r0
	streqh r5, [fp, #0xf8]
	movne r0, r1, lsl #5
	strneh r5, [r4, r0]
	strh r5, [fp, #0xfa]
_06007928:
	ldrh r5, [r6]
_0600792C:
	ldr r0, [sp, #0x38]
	cmp r5, r0
	bne _06007768
	ldr r1, _060079A8 ; =0x0000FFFF
	cmp sb, r1
	beq _06007974
	add r0, r8, sl, lsl #2
	add r0, r0, #0x700
	ldrh r3, [r0, #0xc]
	mov r2, sb, lsl #5
	strh r3, [r4, r2]
	ldrh r2, [r0, #0xe]
	cmp r2, r1
	streqh sb, [r0, #0xe]
	add r0, r8, sl, lsl #2
	add r1, r0, #0x700
	ldr r0, [sp, #8]
	strh r0, [r1, #0xc]
_06007974:
	add sl, sl, #1
	cmp sl, #4
	blt _06007718
	add r0, r8, #0x31c
	mov r1, #0
	add r0, r0, #0x400
	str r1, [r8, #0x734]
	bl __VENEER_OS_UnlockMutex
	ldr r0, [sp, #0xc]
_06007998:
	add sp, sp, #0x3c
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_060079A4: .word wmspW + 0x1000
_060079A8: .word 0x0000FFFF
	arm_func_end WMSP_FlushSendQueue

	arm_func_start WMSP_CleanSendQueue
WMSP_CleanSendQueue: ; 0x060079AC
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0x14
	ldr r1, _06007B98 ; =wmspW + 0x1000
	mvn r2, r0
	ldr sb, [r1, #0x550]
	add r0, sb, #0x100
	ldrh r1, [r0, #0x82]
	add r0, sb, #0x31c
	add r0, r0, #0x400
	add r5, sb, #0x2f8
	and fp, r2, r1
	bl __VENEER_OS_LockMutex
	add r0, sb, #0x30c
	add r0, r0, #0x400
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
_060079F0:
	add r0, sb, #0x100
	str r0, [sp, #0x10]
	mov r6, #0
	add r4, sb, #0x600
_06007A00:
	ldr r0, [sp, #8]
	mov r1, r6, lsl #2
	add r7, r0, r6, lsl #2
	ldrh sl, [r0, r1]
	ldr r0, _06007B9C ; =0x0000FFFF
	str r7, [sp, #4]
	str r0, [sp]
	b _06007B54
_06007A20:
	add r8, r5, sl, lsl #5
	ldrh r0, [r8, #6]
	and r0, r0, fp
	strh r0, [r8, #6]
	ldrh r0, [r8, #0xa]
	and r0, r0, fp
	strh r0, [r8, #0xa]
	ldrh r0, [r8, #6]
	cmp r0, #0
	bne _06007B28
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0x81
	strh r1, [r0]
	mov r1, #0
	strh r1, [r0, #2]
	mov r1, #0x14
	strh r1, [r0, #8]
	ldrh r1, [r8, #2]
	ldr r3, [sp, #0x10]
	strh r1, [r0, #0xa]
	ldrh r1, [r8, #4]
	strh r1, [r0, #0xc]
	ldrh r1, [r8, #6]
	strh r1, [r0, #0xe]
	ldrh r1, [r8, #8]
	strh r1, [r0, #0x10]
	ldrh r1, [r8, #0xe]
	strh r1, [r0, #0x18]
	ldr r1, [r8, #0x14]
	str r1, [r0, #0x14]
	ldr r1, [r8, #0x18]
	str r1, [r0, #0x1c]
	ldr r1, [r8, #0x1c]
	str r1, [r0, #0x20]
	ldrh r1, [r8, #0x10]
	strh r1, [r0, #0x1a]
	ldrh r3, [r3, #0x88]
	ldrh r1, [sb, #0x30]
	cmp r3, #0
	ldrh r2, [sb, #0x32]
	moveq r3, r1
	movne r3, r2
	strh r3, [r0, #0x24]
	ldr r3, [sp, #0x10]
	ldrh r3, [r3, #0x88]
	cmp r3, #0
	movne r2, r1
	strh r2, [r0, #0x26]
	bl __VENEER_WMSP_ReturnResult2Wm9
	ldrh r1, [r8]
	ldr r0, _06007B9C ; =0x0000FFFF
	cmp r1, r0
	ldreq r0, [sp]
	streqh r0, [r7, #2]
	ldrh r1, [r8]
	ldr r0, [sp, #4]
	strh r1, [r0]
	ldr r0, _06007B9C ; =0x0000FFFF
	strh r0, [r8]
	ldrh r1, [r4, #0xfa]
	cmp r1, r0
	streqh sl, [r4, #0xf8]
	movne r0, r1, lsl #5
	strneh sl, [r5, r0]
	strh sl, [r4, #0xfa]
	ldr sl, [sp]
_06007B28:
	ldr r0, _06007B9C ; =0x0000FFFF
	str sl, [sp]
	cmp sl, r0
	addne r0, r5, sl, lsl #5
	strne r0, [sp, #4]
	ldr r0, _06007B9C ; =0x0000FFFF
	streq r7, [sp, #4]
	cmp sl, r0
	movne r0, sl, lsl #5
	ldrneh sl, [r5, r0]
	ldreqh sl, [r7]
_06007B54:
	ldr r0, _06007B9C ; =0x0000FFFF
	cmp sl, r0
	bne _06007A20
	add r6, r6, #1
	cmp r6, #4
	blt _06007A00
	ldr r0, [sp, #0xc]
	add r0, r0, #1
	str r0, [sp, #0xc]
	cmp r0, #2
	blt _060079F0
	add r0, sb, #0x31c
	add r0, r0, #0x400
	bl __VENEER_OS_UnlockMutex
	add sp, sp, #0x14
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_06007B98: .word wmspW + 0x1000
_06007B9C: .word 0x0000FFFF
	arm_func_end WMSP_CleanSendQueue

	arm_func_start WMSP_ParsePortPacket
WMSP_ParsePortPacket: ; 0x06007BA0
	stmdb sp!, {r0, r1, r2, r3}
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0xc
	ldr r4, _06007D9C ; =wmspW + 0x1000
	ldr r1, [sp, #0x40]
	ldr r5, [r4, #0x550]
	movs sb, r3
	str r1, [sp, #0x40]
	mov sl, r0
	str r2, [sp]
	beq _06007D8C
	cmp sb, #0x204
	bhi _06007D8C
	add r4, r5, #0x100
	ldrh r0, [r4, #0x88]
	cmp sl, r0
	beq _06007D8C
	cmp sl, #0x10
	bhs _06007D8C
	tst sb, #1
	bne _06007D8C
	add r0, r5, #0x1f8
	add r8, r0, sl, lsl #4
	mov r0, #1
	str r0, [sp, #8]
	b _06007D84
_06007C08:
	ldr r0, [sp, #8]
	ldr r7, _06007DA0 ; =0x0000FFFF
	cmp r0, #1
	addeq r1, sp, #0x34
	moveq r0, #0
	ldrne r1, [sp]
	streq r0, [sp, #8]
	movne r0, r1
	addne r0, r0, #2
	ldrh r1, [r1]
	strne r0, [sp]
	mov r0, r1, lsl #0x18
	subne sb, sb, #2
	movs r6, r0, lsr #0x17
	moveq r6, #0x200
	tst r1, #0x1000
	movne r2, #1
	moveq r2, #0
	tst r1, #0x800
	movne r3, #1
	moveq r3, #0
	cmp r2, #0
	movne fp, #2
	moveq fp, #0
	cmp r3, #0
	movne r0, #2
	moveq r0, #0
	add r0, r6, r0
	add r0, r0, fp
	subs sb, sb, r0
	bmi _06007D8C
	and r1, r1, #0xf00
	cmp r3, #1
	mov r1, r1, lsl #8
	ldr r3, [sp]
	mov fp, r1, lsr #0x10
	ldr r1, [sp]
	add r0, r3, r0
	str r1, [sp, #4]
	add r1, r1, r6
	str r0, [sp]
	bne _06007CEC
	mov r0, fp, lsl #0x1d
	mov r0, r0, lsr #0x1c
	ldrh r3, [r8, r0]
	ldrh r7, [r1], #2
	tst r3, #1
	movne r3, r7, lsl #1
	strneh r3, [r8, r0]
	bne _06007CEC
	mov r7, r7, lsl #1
	sub r3, r3, r7
	mov r3, r3, lsl #0x10
	mov r3, r3, lsr #0x10
	cmp r3, #0x100
	blo _06007D84
	strh r7, [r8, r0]
_06007CEC:
	cmp r2, #1
	bne _06007D08
	ldrh r2, [r1]
	ldrh r1, [r4, #0x88]
	mov r0, #1
	tst r2, r0, lsl r1
	beq _06007D84
_06007D08:
	cmp r6, #0
	ble _06007D84
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0x82
	strh r1, [r0]
	mov r1, #0
	strh r1, [r0, #2]
	mov r1, #0x15
	strh r1, [r0, #4]
	ldr r1, [sp, #0x40]
	strh fp, [r0, #6]
	str r1, [r0, #8]
	ldr r1, [sp, #4]
	str r1, [r0, #0xc]
	strh r6, [r0, #0x10]
	strh sl, [r0, #0x12]
	ldrh r1, [r4, #0x88]
	strh r1, [r0, #0x20]
	strh r7, [r0, #0x1a]
	ldrh r1, [r4, #0x88]
	ldrh r2, [r5, #0x30]
	cmp r1, #0
	ldrh r3, [r5, #0x32]
	moveq r1, r2
	movne r1, r3
	strh r1, [r0, #0x40]
	ldrh r1, [r4, #0x88]
	cmp r1, #0
	movne r3, r2
	strh r3, [r0, #0x42]
	bl __VENEER_WMSP_ReturnResult2Wm9
_06007D84:
	cmp sb, #0
	bgt _06007C08
_06007D8C:
	add sp, sp, #0xc
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	add sp, sp, #0x10
	bx lr
	.align 2, 0
_06007D9C: .word wmspW + 0x1000
_06007DA0: .word 0x0000FFFF
	arm_func_end WMSP_ParsePortPacket

	arm_func_start WmspGetTmptt
WmspGetTmptt: ; 0x06007DA4
	stmdb sp!, {r4, r5, r6, lr}
	mov r6, r1
	mov r5, r0
	tst r6, #0x8000
	ldrne r0, _06007E40 ; =0x00007FFF
	mov r4, r3
	andne r6, r6, r0
	addeq r0, r6, #0x1c
	moveq r0, r0, lsl #2
	addeq r6, r0, #0x66
	mov r0, r2
	bl __VENEER_MATH_CountPopulation
	mul r2, r6, r0
	ldr r1, _06007E44 ; =0x04000006
	add r0, r5, #0x22
	mov r3, r0, lsl #2
	ldrh r1, [r1]
	sub r0, r4, #2
	add r3, r3, #0x60
	add r2, r2, #0x388
	add r4, r3, r2
	subs r3, r0, r1
	bpl _06007E0C
_06007E00:
	add r0, r3, #7
	adds r3, r0, #0x100
	bmi _06007E00
_06007E0C:
	ldr r2, _06007E48 ; =0x66666667
	rsb r1, r3, r3, lsl #7
	mov r0, r1, lsr #0x1f
	smull r1, r3, r2, r1
	add r3, r0, r3, asr #3
	mov r0, #0xa
	mul r0, r3, r0
	cmp r0, r4
	movlo r3, #0
	mov r0, r3, lsl #0x10
	mov r0, r0, lsr #0x10
	ldmia sp!, {r4, r5, r6, lr}
	bx lr
	.align 2, 0
_06007E40: .word 0x00007FFF
_06007E44: .word 0x04000006
_06007E48: .word 0x66666667
	arm_func_end WmspGetTmptt
