	.include "asm/macros.inc"
	.include "global.inc"

	.extern SND_InitLfoParam
	.extern SNDi_Work
	.extern channel_order$3829
	.extern sWeakLockChannel
	.extern shift$4005

.text

	arm_func_start SND_AllocExChannel
SND_AllocExChannel: ; 0x037FC8E8
	stmdb sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	ldr r4, _037FCAA0 ; =sWeakLockChannel
	mov r6, r1
	ldr r1, [r4, #4]
	mov r5, r3
	mvn r1, r1
	cmp r2, #0
	and r0, r0, r1
	ldreq r1, [r4]
	mov r4, #0
	mvneq r1, r1
	andeq r0, r0, r1
	ldr r3, _037FCAA4 ; =shift$4005
	ldr r8, _037FCAA8 ; =channel_order$3829
	mov sb, r4
	mvn r1, #0
	mov fp, #1
_037FC92C:
	ldrb ip, [r8, sb]
	tst r0, fp, lsl ip
	beq _037FC9AC
	ldr r2, _037FCAAC ; =SNDi_Work
	mov r7, #0x54
	mla sl, ip, r7, r2
	cmp r4, #0
	moveq r4, sl
	beq _037FC9AC
	ldrb r7, [r4, #0x22]
	ldrb r2, [sl, #0x22]
	cmp r2, r7
	bhi _037FC9AC
	bne _037FC9A8
	ldrh r2, [r4, #0x24]
	ldrh r7, [sl, #0x24]
	mov ip, r2, lsl #0x18
	mov ip, ip, lsr #0x14
	ldrb r2, [r3, r2, asr #8]
	mov lr, r7, lsl #0x18
	mov r2, ip, asr r2
	mov ip, lr, lsr #0x14
	ldrb r7, [r3, r7, asr #8]
	cmp r2, ip, asr r7
	beq _037FC99C
	movlt r2, fp
	movge r2, r1
	b _037FC9A0
_037FC99C:
	mov r2, #0
_037FC9A0:
	cmp r2, #0
	bge _037FC9AC
_037FC9A8:
	mov r4, sl
_037FC9AC:
	add sb, sb, #1
	cmp sb, #0x10
	blt _037FC92C
	cmp r4, #0
	moveq r0, #0
	beq _037FCA98
	ldrb r0, [r4, #0x22]
	cmp r6, r0
	movlt r0, #0
	blt _037FCA98
	ldr r3, [r4, #0x48]
	cmp r3, #0
	beq _037FC9F4
	ldr r2, [r4, #0x4c]
	mov r0, r4
	mov r1, #0
	mov lr, pc
	bx r3
_037FC9F4:
	ldrb r0, [r4, #3]
	mov r7, #0
	bic r0, r0, #0xf8
	orr r1, r0, #0x10
	and r0, r1, #0xff
	bic r0, r0, #1
	strb r0, [r4, #3]
	str r7, [r4, #0x50]
	ldr r0, [sp, #0x28]
	str r5, [r4, #0x48]
	str r0, [r4, #0x4c]
	str r7, [r4, #0x34]
	strb r6, [r4, #0x22]
	mov r3, #0x7f
	strh r3, [r4, #0x24]
	ldrb r1, [r4, #3]
	mov r0, #0x3c
	bic r2, r1, #2
	and r1, r2, #0xff
	orr r1, r1, #4
	strb r1, [r4, #3]
	strb r0, [r4, #8]
	strb r0, [r4, #5]
	strb r3, [r4, #9]
	strb r7, [r4, #0xa]
	strh r7, [r4, #0xc]
	strh r7, [r4, #6]
	strh r7, [r4, #0xe]
	strb r7, [r4, #0xb]
	strb r3, [r4, #4]
	strh r7, [r4, #0x32]
	str r7, [r4, #0x18]
	str r7, [r4, #0x14]
	ldr r1, _037FCAB0 ; =0x0000FFFF
	strb r7, [r4, #0x1c]
	strh r1, [r4, #0x1e]
	strb r3, [r4, #0x1d]
	add r0, r4, #0x28
	strh r1, [r4, #0x20]
	bl SND_InitLfoParam
	mov r0, r4
_037FCA98:
	ldmia sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_037FCAA0: .word sWeakLockChannel
_037FCAA4: .word shift$4005
_037FCAA8: .word channel_order$3829
_037FCAAC: .word SNDi_Work
_037FCAB0: .word 0x0000FFFF
	arm_func_end SND_AllocExChannel
