#include "constants/moves.h"
	.include "asm/macros.inc"
	.include "overlay_08.inc"
	.include "global.inc"
	.public ov08_02220CF4
	.extern ov08_02220A8C
	.extern ov08_02220B90
	.extern ov08_02220C08
	.extern ov08_02220C3C
	.extern ov08_02225534
	.extern ov08_02225564

	.text
	.force_active on

	thumb_func_start ov08_02220CF4
ov08_02220CF4: ; 0x02220CF4
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r6, r0, #0
	mov r0, #0
	str r0, [sp, #4]
	add r0, r6, #0
	str r0, [sp]
	add r0, #0x1b
	ldr r4, _02220D78 ; =ov08_02225534
	ldr r7, _02220D7C ; =ov08_02225564
	add r5, r6, #0
	str r0, [sp]
_02220D0C:
	ldrh r0, [r6, #8]
	cmp r0, #0
	beq _02220D5A
	ldr r0, _02220D80 ; =0x00001FD4
	ldr r1, [r4]
	ldr r0, [r5, r0]
	ldr r2, [r4, #4]
	bl ov08_02220A8C
	ldr r0, [sp]
	ldr r1, _02220D84 ; =0x00001FEC
	ldrb r0, [r0]
	ldr r1, [r5, r1]
	ldr r2, [r7]
	lsl r0, r0, #0x19
	ldr r3, [r7, #4]
	lsr r0, r0, #0x1c
	bl ov08_02220B90
	ldr r1, _02220D88 ; =0x00001FB8
	ldr r2, [r4]
	ldr r3, [r4, #4]
	ldrh r0, [r6, #0x1e]
	ldr r1, [r5, r1]
	add r2, #8
	add r3, #8
	bl ov08_02220C08
	add r0, r6, #0
	add r0, #0x31
	ldr r1, _02220D8C ; =0x00002038
	ldr r2, [r4]
	ldr r3, [r4, #4]
	ldrb r0, [r0]
	ldr r1, [r5, r1]
	add r2, #0x10
	add r3, #8
	bl ov08_02220C3C
_02220D5A:
	ldr r0, [sp]
	add r6, #0x50
	add r0, #0x50
	str r0, [sp]
	ldr r0, [sp, #4]
	add r4, #8
	add r0, r0, #1
	add r5, r5, #4
	add r7, #8
	str r0, [sp, #4]
	cmp r0, #6
	blt _02220D0C
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02220D78: .word ov08_02225534
_02220D7C: .word ov08_02225564
_02220D80: .word 0x00001FD4
_02220D84: .word 0x00001FEC
_02220D88: .word 0x00001FB8
_02220D8C: .word 0x00002038
	thumb_func_end ov08_02220CF4
