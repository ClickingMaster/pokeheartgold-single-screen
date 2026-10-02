	.extern ov89_0225A5A4
	.extern ov89_0225C9CC
	.include "asm/macros.inc"
	.include "overlay_89.inc"
	.include "global.inc"

.text
	thumb_func_start ov89_02259CD0
ov89_02259CD0: ; 0x02259CD0
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	mov r2, #1
	add r6, r0, #0
	str r2, [sp]
	mov r0, #0x1b
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	mov r0, #0xd
	str r0, [sp, #0xc]
	mov r0, #0x5b
	str r0, [sp, #0x10]
	add r1, r6, #0
	ldr r0, [r6, #8]
	add r1, #0xb4
	mov r3, #2
	bl AddWindowParameterized
	add r0, r6, #0
	add r0, #0xb4
	mov r1, #0xf
	bl FillWindowPixelBuffer
	add r4, r6, #0
	mov r5, #0
	ldr r7, _02259D4C ; =ov89_0225C9CC
	add r4, #0x34
	b _02259D44
_02259D0A:
	lsl r3, r5, #1
	add r0, r7, r3
	ldrb r0, [r0, #1]
	lsl r1, r5, #4
	mov r2, #5
	str r0, [sp]
	mov r0, #8
	str r0, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	mov r0, #0xd
	str r0, [sp, #0xc]
	mov r0, #2
	lsl r0, r0, #8
	add r0, r1, r0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0x10]
	ldrb r3, [r7, r3]
	ldr r0, [r6, #8]
	add r1, r4, r1
	bl AddWindowParameterized
	lsl r0, r5, #4
	add r0, r4, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	add r5, r5, #1
_02259D44:
	cmp r5, #8
	blt _02259D0A
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_02259D4C: .word ov89_0225C9CC
	thumb_func_end ov89_02259CD0
