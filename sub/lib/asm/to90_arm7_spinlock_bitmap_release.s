	.include "asm/macros.inc"
	.public OS_ReleaseLockID

.text
	.type OS_ReleaseLockID, @function
	arm_func_start OS_ReleaseLockID
OS_ReleaseLockID:
	ldr	r3, [pc, #36]
	cmp	r0, #160
	addpl	r3, r3, #4
	subpl	r0, r0, #160
	submi	r0, r0, #128
	mov	r1, #-2147483648
	mov r1, r1, lsr r0
	ldr	r2, [r3]
	orr	r2, r2, r1
	str	r2, [r3]
	bx lr
	.word	0x027fffb8
	arm_func_end OS_ReleaseLockID
	.type $d, @function
