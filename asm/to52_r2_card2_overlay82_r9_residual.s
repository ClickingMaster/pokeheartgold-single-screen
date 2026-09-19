	.include "asm/macros.inc"
	.include "overlay_82.inc"
	.include "global.inc"

	.text
	.public ov82_0223F558
	.public ov82_0223F570
	.public ov82_0223FCBC
	.public ov82_0223FCFC
	thumb_func_start ov82_0223F488
ov82_0223F488: ; 0x0223F488
	push {r3, r4, r5, r6, r7, lr}
	str r0, [sp]
	ldr r0, _0223F538 ; =gSystem + 0x40
	ldrh r1, [r0, #0x24]
	cmp r1, #0
	beq _0223F532
	ldrh r4, [r0, #0x20]
	ldrh r5, [r0, #0x22]
	mov r7, #0
	mov r3, #3
_0223F49C:
	add r2, r3, #0
	add r2, #0x23
	mov r1, #0
	mov r6, #1
_0223F4A4:
	add r0, r6, #0
	add r0, #0x3f
	cmp r6, r4
	bgt _0223F522
	cmp r4, r0
	bgt _0223F522
	cmp r3, r5
	bgt _0223F522
	cmp r5, r2
	bgt _0223F522
	lsl r0, r7, #2
	add r1, r1, r0
	ldr r0, [sp]
	strb r1, [r0, #0xd]
	bl ov82_0223F558
	add r4, r0, #0
	ldr r0, [sp]
	bl ov82_0223F570
	add r2, r0, #0
	mov r1, #0x81
	ldr r0, [sp]
	lsl r1, r1, #2
	ldr r0, [r0, r1]
	add r1, r4, #0
	bl ov82_0223FCBC
	ldr r0, [sp]
	ldrb r0, [r0, #0xd]
	bl ov80_02237920
	cmp r0, #0xfe
	bne _0223F508
	mov r1, #0x81
	ldr r0, [sp]
	lsl r1, r1, #2
	ldr r0, [r0, r1]
	mov r1, #2
	bl ov82_0223FCFC
	mov r1, #0x81
	ldr r0, [sp]
	lsl r1, r1, #2
	ldr r0, [r0, r1]
	mov r1, #0x80
	mov r2, #0xa8
	bl ov82_0223FCBC
	b _0223F516
_0223F508:
	mov r1, #0x81
	ldr r0, [sp]
	lsl r1, r1, #2
	ldr r0, [r0, r1]
	mov r1, #1
	bl ov82_0223FCFC
_0223F516:
	ldr r1, [sp]
	mov r0, #1
	add r1, #0x90
	strb r0, [r1]
	str r1, [sp]
	pop {r3, r4, r5, r6, r7, pc}
_0223F522:
	add r1, r1, #1
	add r6, #0x40
	cmp r1, #4
	blt _0223F4A4
	add r7, r7, #1
	add r3, #0x24
	cmp r7, #5
	blt _0223F49C
_0223F532:
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0223F538: .word gSystem + 0x40
	thumb_func_end ov82_0223F488
