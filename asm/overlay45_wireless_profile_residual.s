#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_45_thumb.inc"
	.include "global.inc"
	.extern memset
	.extern ov45_0222EC10
	.extern ov45_0222AA84
	.extern ov45_0222AAA8
	.extern ov45_0222D638
	.extern ov45_0222EA2C
	.extern ov45_0222EDC4
	.extern ov45_0222EDF0

	.text
	.public ov45_0222B79C
	thumb_func_start ov45_0222B79C
ov45_0222B79C: ; 0x0222B79C
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r5, r1, #0
	add r4, r0, #0
	add r0, r5, #0
	mov r1, #0
	mov r2, #0xc8
	bl memset
	add r0, sp, #4
	bl ov45_0222EC10
	add r4, #0x20
	add r0, r4, #0
	bl ov45_0222AA84
	add r6, r0, #0
	add r0, r4, #0
	bl ov45_0222AAA8
	add r2, r0, #0
	lsl r1, r6, #0x10
	lsl r2, r2, #0x18
	add r0, r5, #0
	lsr r1, r1, #0x10
	lsr r2, r2, #0x18
	mov r3, #0
	bl ov45_0222D638
	mov r6, #0
	add r4, r6, #0
_0222B7DA:
	ldr r0, [sp, #8]
	mov r1, #0
	ldr r0, [r0, r4]
	mvn r1, r1
	cmp r0, r1
	beq _0222B80A
	bl ov45_0222EA2C
	str r0, [sp]
	bl ov45_0222AA84
	add r7, r0, #0
	ldr r0, [sp]
	bl ov45_0222AAA8
	add r2, r0, #0
	lsl r1, r7, #0x10
	lsl r2, r2, #0x18
	add r0, r5, #0
	lsr r1, r1, #0x10
	lsr r2, r2, #0x18
	mov r3, #0
	bl ov45_0222D638
_0222B80A:
	add r6, r6, #1
	add r4, r4, #4
	cmp r6, #0x14
	blt _0222B7DA
	mov r4, #0
	mov r7, #1
_0222B816:
	lsl r0, r4, #0x18
	lsr r0, r0, #0x18
	bl ov45_0222EDC4
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	lsl r0, r4, #0x18
	lsr r0, r0, #0x18
	bl ov45_0222EDF0
	add r2, r0, #0
	add r0, r5, #0
	add r1, r6, #0
	add r3, r7, #0
	bl ov45_0222D638
	add r4, r4, #1
	cmp r4, #0x14
	blt _0222B816
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	thumb_func_end ov45_0222B79C
