	.extern ov41_022488D8
	.extern ov41_02248984
	.extern ov41_02248A08
	.extern ov41_02248AE0
	.extern ov41_02248AFC
	.extern ov41_02248D64
	.extern ov41_02248DA4
	.extern ov41_02248E10
	.include "asm/macros.inc"
	.include "overlay_41.inc"
	.include "global.inc"

.text
	thumb_func_start ov41_02248B48
ov41_02248B48: ; 0x02248B48
	push {r3, r4, r5, r6, r7, lr}
	add r5, r1, #0
	mov r1, #9
	add r4, r2, #0
	bl _s32_div_f
	add r7, r1, #0
	add r0, r7, #0
	mov r1, #3
	bl _s32_div_f
	add r6, r0, #0
	add r0, r7, #0
	mov r1, #3
	bl _s32_div_f
	add r0, r6, #1
	lsl r2, r0, #3
	lsl r0, r6, #5
	add r0, r2, r0
	add r0, #0x10
	str r0, [r4]
	add r0, r1, #1
	lsl r2, r0, #3
	mov r0, #0x18
	mul r0, r1
	add r0, r2, r0
	add r0, #8
	str r0, [r5]
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov41_02248B48
