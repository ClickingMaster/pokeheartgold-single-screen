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
