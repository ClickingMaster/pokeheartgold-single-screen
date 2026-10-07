	.include "asm/macros.inc"
	.public OS_GetLockID

.text
	.type OS_GetLockID, @function
	arm_func_start OS_GetLockID
OS_GetLockID:
	ldr	r3, [pc, #136]
	ldr	r1, [r3]
	mov	r2, #0
	mov	r0, #-2147483648
	tst	r1, r0
	bne . + 0x18
	add	r2, r2, #1
	cmp	r2, #32
	beq . + 0xC
	mov r0, r0, lsr #1
	b . - 0x18
	cmp	r2, #32
	movne	r0, #128
	bne . + 0x40
	add	r3, r3, #4
	ldr	r1, [r3]
	mov	r2, #0
	mov	r0, #-2147483648
	tst	r1, r0
	bne . + 0x18
	add	r2, r2, #1
	cmp	r2, #32
	beq . + 0xC
	mov r0, r0, lsr #1
	b . - 0x18
	cmp	r2, #32
	ldr	r0, [pc, #36]
	bxeq lr
	mov	r0, #160
	add	r0, r0, r2
	mov	r1, #-2147483648
	mov r1, r1, lsr r2
	ldr	r2, [r3]
	bic	r2, r2, r1
	str	r2, [r3]
	bx lr
	.word	0x027fffb8
	.word	0xfffffffd
	arm_func_end OS_GetLockID
	.type $d, @function
