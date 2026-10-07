	.include "asm/macros.inc"
	.include "global.inc"


.text

	arm_func_start RND_Init
RND_Init: ; 0x0600BA2C
	ldr r3, _0600BA58 ; =0x0380FFF4
	ldr r2, _0600BA5C ; =0x0000FFF8
	ldr r3, [r3]
	and r0, r0, r2
	add r2, r0, #5
	add r0, r3, #0x500
	strh r2, [r0, #0xf0]
	add r2, r3, #0x5f0
	orr r0, r1, #1
	strh r0, [r2, #2]
	bx lr
	.align 2, 0
_0600BA58: .word 0x0380FFF4
_0600BA5C: .word 0x0000FFF8
	arm_func_end RND_Init
