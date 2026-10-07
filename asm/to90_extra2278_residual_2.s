	.extern ov08_022234A4
	.extern ov08_02223678
#include "constants/moves.h"
	.include "asm/macros.inc"
	.include "overlay_08.inc"
	.include "global.inc"
	.extern ov08_022239CC
	.extern ov08_02223A3C

.text
	.force_active on
	thumb_func_start ov08_02223AA0
ov08_02223AA0: ; 0x02223AA0
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	mov r6, #0
	add r5, r0, #0
	add r4, r6, #0
	add r7, r6, #0
_02223AAC:
	ldr r0, [r5, #0x2c]
	add r1, r7, #0
	add r0, r0, r4
	bl FillWindowPixelBuffer
	add r6, r6, #1
	add r4, #0x10
	cmp r6, #4
	blo _02223AAC
	ldr r0, _02223B14 ; =0x0000114D
	ldr r1, [r5]
	ldrb r2, [r5, r0]
	add r0, r1, r2
	add r0, #0x27
	ldrb r4, [r0]
	add r0, r1, r2
	add r0, #0x2c
	ldrb r1, [r0]
	mov r0, #6
	add r6, r1, #0
	mul r6, r0
	add r0, r5, #0
	add r1, r4, r6
	bl ov08_022239CC
	mov r2, #0
	str r2, [sp]
	ldr r0, _02223B18 ; =0x00010200
	str r2, [sp, #4]
	str r0, [sp, #8]
	add r0, r5, #0
	add r1, r4, r6
	mov r3, #1
	bl ov08_02223678
	add r0, r5, #0
	add r1, r4, r6
	bl ov08_02223A3C
	mov r0, #5
	str r0, [sp]
	ldr r0, _02223B1C ; =0x00030201
	mov r1, #3
	str r0, [sp, #4]
	add r0, r5, #0
	mov r2, #0x1f
	mov r3, #4
	bl ov08_022234A4
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	nop
_02223B14: .word 0x0000114D
_02223B18: .word 0x00010200
_02223B1C: .word 0x00030201
	thumb_func_end ov08_02223AA0
