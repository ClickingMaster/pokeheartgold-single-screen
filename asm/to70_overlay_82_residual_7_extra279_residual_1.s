	.include "asm/macros.inc"
	.include "overlay_82.inc"
	.include "global.inc"

.text
	thumb_func_start ov82_0223F040
ov82_0223F040: ; 0x0223F040
	push {r4, r5, r6, r7, lr}
	sub sp, #0x2c
	str r1, [sp, #0x10]
	add r0, r1, #0
	add r1, sp, #0x30
	ldrb r1, [r1, #0x10]
	add r4, r2, #0
	add r6, r3, #0
	bl FillWindowPixelBuffer
	ldr r2, _0223F108 ; =0x000002DF
	mov r0, #1
	mov r1, #0x1b
	mov r3, #0x69
	bl NewMsgDataFromNarc
	str r0, [sp, #0x24]
	mov r0, #0xb
	mov r1, #0x69
	bl String_New
	add r5, r0, #0
	mov r0, #0
	lsl r2, r4, #0x18
	lsr r3, r2, #8
	lsl r2, r6, #0x18
	lsr r2, r2, #0x10
	str r0, [sp, #0x28]
	str r0, [sp, #0x20]
	mov r0, #0x10
	add r1, sp, #0x30
	str r0, [sp, #0x14]
	ldrb r0, [r1, #0x10]
	orr r2, r3
	orr r0, r2
	str r0, [sp, #0x1c]
	ldrb r0, [r1, #0x14]
	str r0, [sp, #0x18]
_0223F08C:
	mov r4, #0
	mov r6, #1
_0223F090:
	ldr r0, [sp, #0x20]
	add r7, r4, r0
	lsl r0, r7, #0x18
	lsr r0, r0, #0x18
	bl ov80_02237920
	cmp r0, #0xfe
	beq _0223F0D4
	add r0, r5, #0
	bl String_SetEmpty
	lsl r0, r7, #0x18
	lsr r0, r0, #0x18
	bl ov80_02237920
	add r1, r0, #0
	ldr r0, [sp, #0x24]
	add r2, r5, #0
	bl ReadMsgDataIntoString
	ldr r0, [sp, #0x14]
	ldr r1, [sp, #0x18]
	str r0, [sp]
	mov r0, #0
	str r0, [sp, #4]
	ldr r0, [sp, #0x1c]
	add r2, r5, #0
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x10]
	add r3, r6, #0
	bl AddTextPrinterParameterizedWithColor
_0223F0D4:
	add r4, r4, #1
	add r6, #0x40
	cmp r4, #4
	blt _0223F090
	ldr r0, [sp, #0x20]
	add r0, r0, #4
	str r0, [sp, #0x20]
	ldr r0, [sp, #0x14]
	add r0, #0x24
	str r0, [sp, #0x14]
	ldr r0, [sp, #0x28]
	add r0, r0, #1
	str r0, [sp, #0x28]
	cmp r0, #5
	blt _0223F08C
	add r0, r5, #0
	bl String_Delete
	ldr r0, [sp, #0x24]
	bl DestroyMsgData
	ldr r0, [sp, #0x10]
	bl CopyWindowToVram
	add sp, #0x2c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0223F108: .word 0x000002DF
	thumb_func_end ov82_0223F040
