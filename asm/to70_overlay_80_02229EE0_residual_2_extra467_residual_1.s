	.extern ov80_02229F6C
	.include "asm/macros.inc"
	.include "overlay_80_02229EE0.inc"
	.include "global.inc"
	.extern ov80_0222A4EC

.text
	thumb_func_start ov80_0222A52C
ov80_0222A52C: ; 0x0222A52C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x30]
	add r7, r1, #0
	str r0, [sp, #0x30]
	ldr r0, [sp, #0x34]
	str r2, [sp, #0x10]
	str r0, [sp, #0x34]
	ldr r0, [sp, #0x38]
	str r3, [sp, #0x14]
	str r0, [sp, #0x38]
	ldr r0, [sp, #0x3c]
	mov r4, #0
	str r0, [sp, #0x3c]
	ldr r0, [sp, #0x34]
	cmp r0, #0
	ble _0222A5A0
	ldr r5, [sp, #0x30]
	add r6, r3, #0
_0222A554:
	ldr r0, [sp, #0x10]
	cmp r0, #0
	bne _0222A55E
	mov r0, #0
	b _0222A560
_0222A55E:
	ldrb r0, [r0, r4]
_0222A560:
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	ldr r0, [sp, #0x14]
	cmp r0, #0
	bne _0222A56E
	mov r0, #0
	b _0222A570
_0222A56E:
	ldr r0, [r6]
_0222A570:
	str r0, [sp]
	ldr r0, [sp, #0x38]
	add r2, r4, #0
	str r0, [sp, #4]
	ldr r0, [sp, #0x3c]
	str r0, [sp, #8]
	ldrh r1, [r7]
	ldr r0, [sp, #0xc]
	bl ov80_0222A4EC
	ldr r1, [sp, #0x30]
	cmp r1, #0
	beq _0222A58C
	str r0, [r5]
_0222A58C:
	ldr r0, [sp, #0xc]
	add r4, r4, #1
	add r0, #0x38
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x34]
	add r6, r6, #4
	add r7, r7, #2
	add r5, r5, #4
	cmp r4, r0
	blt _0222A554
_0222A5A0:
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov80_0222A52C
