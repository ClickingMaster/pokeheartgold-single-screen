	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

	.text

	thumb_func_start ov96_021F80A8
ov96_021F80A8:
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r7, r0, #0
	str r2, [sp, #4]
	str r1, [sp]
	add r1, r7, #0
	ldr r0, [sp, #4]
	add r1, #0x59
	strb r0, [r1]
	ldr r0, [r7]
	add r1, r3, #0
	bl ov96_021F87E0
	mov r5, #0
	str r5, [sp, #0xc]
	add r6, r5, #0
_h11_loop:
	mov r2, #1
	mov r3, #0x9c
	add r4, r5, #0
	ldr r1, [sp]
	add r0, r7, #0
	lsl r2, r2, #8
	sub r3, r3, r6
	add r4, #0xa
	bl ov96_021F8378
	str r0, [sp, #8]
	bl ov96_021F8978
	cmp r0, #0
	bne _h11_after_extra
	add r4, #0xa
_h11_after_extra:
	ldr r1, [sp, #8]
	add r0, r7, #0
	add r2, r4, #0
	bl ov96_021F82E4
	ldr r0, [sp, #0xc]
	add r5, #0xa
	add r0, r0, #1
	add r6, #0x28
	str r0, [sp, #0xc]
	cmp r0, #4
	blt _h11_loop
	ldr r0, [sp, #4]
	cmp r0, #4
	bls _h11_done_extra
	mov r2, #1
	ldr r1, [sp]
	add r0, r7, #0
	lsl r2, r2, #8
	mov r3, #0x28
	bl ov96_021F8378
	add r1, r0, #0
	add r0, r7, #0
	mov r2, #0
	bl ov96_021F8334
_h11_done_extra:
	mov r0, #1
	add r7, #0x58
	strb r0, [r7]
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov96_021F80A8
