	.extern ov39_02228AA8
#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_39_thumb.inc"
	.include "global.inc"

.text
	thumb_func_start ov39_02228DA0
ov39_02228DA0: ; 0x02228DA0
	push {r4, lr}
	sub sp, #8
	add r4, r0, #0
	bl ov00_021ECDC8
	cmp r0, #5
	bhi _02228E44
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02228DBA: ; jump table
	.short _02228DCC - _02228DBA - 2 ; case 0
	.short _02228E44 - _02228DBA - 2 ; case 1
	.short _02228E44 - _02228DBA - 2 ; case 2
	.short _02228DC6 - _02228DBA - 2 ; case 3
	.short _02228DCC - _02228DBA - 2 ; case 4
	.short _02228DCC - _02228DBA - 2 ; case 5
_02228DC6:
	mov r0, #5
	str r0, [r4, #8]
	b _02228E44
_02228DCC:
	add r0, r4, #0
	bl ov39_02228AA8
	add r0, sp, #4
	add r1, sp, #0
	bl ov00_021EC11C
	str r0, [r4, #0x14]
	ldr r0, [sp, #4]
	str r0, [r4, #0x18]
	ldr r0, [sp]
	str r0, [r4, #0x1c]
	bl ov00_021EC210
	bl ov00_021EC8D8
	mov r0, #6
	str r0, [r4, #8]
	ldr r1, [sp]
	cmp r1, #7
	bhi _02228E32
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_02228E02: ; jump table
	.short _02228E32 - _02228E02 - 2 ; case 0
	.short _02228E12 - _02228E02 - 2 ; case 1
	.short _02228E12 - _02228E02 - 2 ; case 2
	.short _02228E24 - _02228E02 - 2 ; case 3
	.short _02228E16 - _02228E02 - 2 ; case 4
	.short _02228E2E - _02228E02 - 2 ; case 5
	.short _02228E20 - _02228E02 - 2 ; case 6
	.short _02228E2E - _02228E02 - 2 ; case 7
_02228E12:
	str r0, [r4, #8]
	b _02228E32
_02228E16:
	bl ov00_021FA0D8
	mov r0, #6
	str r0, [r4, #8]
	b _02228E32
_02228E20:
	str r0, [r4, #8]
	b _02228E32
_02228E24:
	bl ov00_021ED9B4
	mov r0, #6
	str r0, [r4, #8]
	b _02228E32
_02228E2E:
	bl sub_020399EC
_02228E32:
	ldr r1, [sp, #4]
	ldr r0, _02228E4C ; =0xFFFFB1E0
	cmp r1, r0
	bge _02228E44
	ldr r0, _02228E50 ; =0xFFFF8AD1
	cmp r1, r0
	blt _02228E44
	mov r0, #6
	str r0, [r4, #8]
_02228E44:
	mov r0, #0
	add sp, #8
	pop {r4, pc}
	nop
_02228E4C: .word 0xFFFFB1E0
_02228E50: .word 0xFFFF8AD1
	thumb_func_end ov39_02228DA0
