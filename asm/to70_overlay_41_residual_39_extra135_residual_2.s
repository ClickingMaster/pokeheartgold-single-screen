	.extern ov41_022488D8
	.extern ov41_02248984
	.extern ov41_02248A08
	.extern ov41_02248AE0
	.extern ov41_02248AFC
	.extern ov41_02248D64
	.extern ov41_02248DA4
	.extern ov41_02248E10
	.include "asm/macros.inc"
	.include "overlay_41.inc"
	.include "global.inc"
	.public ov41_02248BFC

.text
	thumb_func_start ov41_02248BFC
ov41_02248BFC: ; 0x02248BFC
	push {r3, r4, r5, r6, lr}
	sub sp, #4
	add r4, r1, #0
	add r5, r0, #0
	ldr r0, [r4, #0x1c]
	cmp r0, #4
	bls _02248C0C
	b _02248D54
_02248C0C:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02248C18: ; jump table
	.short _02248C22 - _02248C18 - 2 ; case 0
	.short _02248C76 - _02248C18 - 2 ; case 1
	.short _02248CC2 - _02248C18 - 2 ; case 2
	.short _02248D08 - _02248C18 - 2 ; case 3
	.short _02248D20 - _02248C18 - 2 ; case 4
_02248C22:
	mov r0, #0x83
	mvn r0, r0
	str r0, [sp]
	ldr r0, [r4]
	ldr r1, [r4, #0xc]
	ldr r2, [r4, #0x10]
	mov r3, #0
	bl ov41_02248984
	ldr r0, [r4]
	ldr r1, [r4, #0xc]
	ldr r2, [r4, #0x10]
	mov r3, #1
	bl ov41_02248A08
	mov r0, #0
	str r0, [r4, #0x14]
	add r1, r4, #0
	ldr r0, [r4]
	add r1, #0x14
	str r1, [sp]
	ldr r1, [r0, #0x40]
	add r1, r1, #1
	lsr r3, r1, #0x1f
	lsl r2, r1, #0x1f
	sub r2, r2, r3
	mov r1, #0x1f
	ror r2, r1
	add r1, r3, r2
	mov r2, #2
	mov r3, #5
	bl ov41_022488D8
	ldr r0, [r4, #0x20]
	mov r1, #1
	bl ov41_02248E10
	str r0, [r4, #0x18]
	ldr r0, [r4, #0x1c]
	add r0, r0, #1
	str r0, [r4, #0x1c]
	b _02248D58
_02248C76:
	ldr r0, [r4, #0x18]
	mov r6, #0
	cmp r0, #0
	ble _02248CAA
	add r5, r4, #0
	add r5, #0x20
_02248C82:
	ldr r0, [r4, #0x20]
	sub r0, r0, #1
	bmi _02248CA2
	ldr r0, [r5]
	sub r0, r0, #1
	str r0, [r5]
	ldr r0, [r4]
	ldr r1, [r4, #4]
	ldr r2, [r4, #8]
	ldr r3, [r4, #0x20]
	bl ov41_02248AFC
	ldr r1, [r4, #0x28]
	ldr r2, [r4, #0x2c]
	bl ov41_02248D64
_02248CA2:
	ldr r0, [r4, #0x18]
	add r6, r6, #1
	cmp r6, r0
	blt _02248C82
_02248CAA:
	ldr r0, [r4, #0x20]
	cmp r0, #0
	bne _02248D58
	ldr r0, [r4, #0x24]
	mov r1, #2
	bl ov41_02248E10
	str r0, [r4, #0x18]
	ldr r0, [r4, #0x1c]
	add r0, r0, #1
	str r0, [r4, #0x1c]
	b _02248D58
_02248CC2:
	ldr r0, [r4, #0x18]
	mov r6, #0
	cmp r0, #0
	ble _02248CF6
	add r5, r4, #0
	add r5, #0x24
_02248CCE:
	ldr r0, [r4, #0x24]
	sub r0, r0, #1
	bmi _02248CEE
	ldr r0, [r5]
	sub r0, r0, #1
	str r0, [r5]
	ldr r0, [r4]
	ldr r1, [r4, #0xc]
	ldr r2, [r4, #0x10]
	ldr r3, [r4, #0x24]
	bl ov41_02248AFC
	ldr r1, [r4, #0x28]
	ldr r2, [r4, #0x2c]
	bl ov41_02248D64
_02248CEE:
	ldr r0, [r4, #0x18]
	add r6, r6, #1
	cmp r6, r0
	blt _02248CCE
_02248CF6:
	ldr r0, [r4, #0x24]
	cmp r0, #0
	bne _02248D58
	ldr r0, [r4, #0x1c]
	add r0, r0, #1
	str r0, [r4, #0x1c]
	mov r0, #0
	str r0, [r4, #0x18]
	b _02248D58
_02248D08:
	ldr r0, [r4, #0x18]
	add r0, r0, #1
	str r0, [r4, #0x18]
	cmp r0, #3
	ble _02248D58
	ldr r0, [r4, #0x14]
	cmp r0, #0
	beq _02248D58
	ldr r0, [r4, #0x1c]
	add r0, r0, #1
	str r0, [r4, #0x1c]
	b _02248D58
_02248D20:
	ldr r0, [r4]
	ldr r1, [r4, #4]
	ldr r2, [r4, #8]
	mov r3, #0
	bl ov41_02248A08
	mov r0, #0x83
	mvn r0, r0
	str r0, [sp]
	ldr r0, [r4]
	ldr r1, [r4, #4]
	ldr r2, [r4, #8]
	mov r3, #0
	bl ov41_02248984
	ldr r0, [r4]
	mov r1, #0
	str r1, [r0, #0x3c]
	ldr r0, [r4, #0x28]
	bl Heap_Free
	add r0, r5, #0
	bl DestroySysTaskAndEnvironment
	add sp, #4
	pop {r3, r4, r5, r6, pc}
_02248D54:
	bl GF_AssertFail
_02248D58:
	ldr r0, [r4, #0x28]
	ldr r1, [r4, #0x2c]
	bl ov41_02248DA4
	add sp, #4
	pop {r3, r4, r5, r6, pc}
	thumb_func_end ov41_02248BFC
