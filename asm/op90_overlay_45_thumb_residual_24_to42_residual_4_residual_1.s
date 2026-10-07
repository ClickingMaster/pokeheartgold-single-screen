	.extern ov45_0222BD4C
	.extern ov45_0222BD74
	.extern ov45_0222BE54
	.extern ov45_0222EC68
#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_45_thumb.inc"
	.include "global.inc"
	.public ov45_0222C73C

.text
thumb_func_start ov45_0222C73C
ov45_0222C73C: ; 0x0222C73C
	push {r3, r4, r5, lr}
	add r5, r1, #0
	add r4, r3, #0
	bl ov45_0222EC68
	ldr r2, _0222C790 ; =0x000001CA
	add r1, r0, #0
	ldrb r0, [r4, r2]
	cmp r0, #0
	beq _0222C78E
	sub r0, r2, #6
	ldrh r0, [r4, r0]
	cmp r0, r1
	bne _0222C78E
	ldrh r0, [r5]
	cmp r0, #1
	bne _0222C77C
	sub r2, #0xa
	add r0, r4, r2
	bl ov45_0222BD74
	cmp r0, #0
	bne _0222C76E
	mov r1, #1
	b _0222C770
_0222C76E:
	mov r1, #0
_0222C770:
	ldr r0, _0222C794 ; =0x000001CB
	strb r1, [r4, r0]
	ldr r0, _0222C798 ; =0x000001C6
	mov r1, #2
	strb r1, [r4, r0]
	b _0222C784
_0222C77C:
	sub r2, #0xa
	add r0, r4, r2
	bl ov45_0222BD4C
_0222C784:
	mov r0, #7
	lsl r0, r0, #6
	add r0, r4, r0
	bl ov45_0222BE54
_0222C78E:
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0222C790: .word 0x000001CA
_0222C794: .word 0x000001CB
_0222C798: .word 0x000001C6
	thumb_func_end ov45_0222C73C
