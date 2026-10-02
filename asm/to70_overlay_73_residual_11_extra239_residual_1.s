	.extern ov73_021E7818
#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_73.inc"
	.include "global.inc"

.text
	thumb_func_start ov73_021E7490
ov73_021E7490: ; 0x021E7490
	push {r3, r4, r5, r6, r7, lr}
	add r7, r0, #0
	mov r4, #0
	str r4, [sp]
	add r6, r7, #0
	add r5, r7, #0
_021E749C:
	mov r0, #0xce
	lsl r0, r0, #2
	ldr r1, [r6, r0]
	add r0, r0, #4
	str r1, [r6, r0]
	add r0, r4, #0
	bl sub_02034818
	mov r1, #0xce
	lsl r1, r1, #2
	str r0, [r6, r1]
	ldr r0, _021E7560 ; =0x00004A3C
	ldr r2, _021E7564 ; =0x00004A44
	ldr r1, [r5, r0]
	add r0, r0, #4
	ldr r0, [r5, r0]
	str r1, [r5, r2]
	add r1, r2, #4
	str r0, [r5, r1]
	mov r0, #0xce
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	cmp r0, #0
	beq _021E74DA
	bl PlayerProfile_GetTrainerID
	ldr r1, _021E7560 ; =0x00004A3C
	str r0, [r5, r1]
	mov r1, #1
	ldr r0, _021E7568 ; =0x00004A40
	b _021E74E4
_021E74DA:
	add r0, r2, #0
	mov r1, #0
	sub r0, #8
	str r1, [r5, r0]
	sub r0, r2, #4
_021E74E4:
	add r4, r4, #1
	str r1, [r5, r0]
	add r6, #8
	add r5, #0x10
	cmp r4, #5
	blt _021E749C
	mov r4, #0
	add r2, r7, #0
	add r3, r7, #0
_021E74F6:
	mov r0, #0xce
	mov r1, #0xcf
	lsl r0, r0, #2
	lsl r1, r1, #2
	ldr r0, [r7, r0]
	ldr r1, [r7, r1]
	cmp r1, r0
	beq _021E7520
	mov r1, #1
	str r1, [sp]
	cmp r0, #0
	bne _021E7518
	mov r0, #0x36
	mov r1, #3
	lsl r0, r0, #4
	str r1, [r2, r0]
	b _021E7550
_021E7518:
	mov r0, #0x36
	lsl r0, r0, #4
	str r1, [r2, r0]
	b _021E7550
_021E7520:
	ldr r0, _021E7560 ; =0x00004A3C
	ldr r5, _021E7564 ; =0x00004A44
	ldr r1, [r3, r0]
	ldr r6, [r3, r5]
	add r0, r0, #4
	add r5, r5, #4
	ldr r0, [r3, r0]
	ldr r5, [r3, r5]
	eor r1, r6
	eor r5, r0
	orr r1, r5
	beq _021E7550
	mov r1, #1
	str r1, [sp]
	cmp r0, #0
	bne _021E754A
	mov r0, #0x36
	mov r1, #3
	lsl r0, r0, #4
	str r1, [r2, r0]
	b _021E7550
_021E754A:
	mov r0, #0x36
	lsl r0, r0, #4
	str r1, [r2, r0]
_021E7550:
	add r4, r4, #1
	add r7, #8
	add r2, r2, #4
	add r3, #0x10
	cmp r4, #5
	blt _021E74F6
	ldr r0, [sp]
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_021E7560: .word 0x00004A3C
_021E7564: .word 0x00004A44
_021E7568: .word 0x00004A40
	thumb_func_end ov73_021E7490
