	.extern ov83_022477E4
	.include "asm/macros.inc"
	.include "overlay_83.inc"
	.include "global.inc"

.text
	thumb_func_start ov83_022477EC
ov83_022477EC: ; 0x022477EC
	push {r4, r5}
	lsl r5, r0, #2
	add r0, r1, #1
	add r4, r0, #0
	mov r0, #0xf
	add r1, r0, #0
	lsl r1, r5
	sub r0, #0x10
	ldr r3, [r2]
	eor r0, r1
	lsl r4, r5
	and r0, r3
	orr r0, r4
	str r0, [r2]
	pop {r4, r5}
	bx lr
	thumb_func_end ov83_022477EC
