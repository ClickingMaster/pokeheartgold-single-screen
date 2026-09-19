	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

	.text

	thumb_func_start ov96_021F8160
ov96_021F8160:
	push {r3, r4, r5, r6, r7, lr}
	add r6, r1, #0
	add r7, r2, #0
	mov r4, #0
	add r5, r0, #4
_h11_middle_loop:
	add r0, r5, #0
	add r1, r6, #0
	add r2, r7, #0
	bl ov96_021F8128
	add r4, r4, #1
	add r5, #0x10
	cmp r4, #4
	blt _h11_middle_loop
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov96_021F8160
