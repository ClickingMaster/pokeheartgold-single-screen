	.extern ov93_02262C82
	.extern ov93_02262D54
	.include "asm/macros.inc"
	.include "overlay_93_thumb_2.inc"
	.include "global.inc"
	.public ov93_02260E1C

.text
	thumb_func_start ov93_02260E1C
ov93_02260E1C: ; 0x02260E1C
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	add r5, r0, #0
	ldr r0, [r5, #0x28]
	ldr r1, _02260F08 ; =0x00002716
	mov r2, #2
	bl SpriteManager_FindPlttResourceOffset
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #0x10]
	ldr r0, [r5]
	add r0, #0x30
	ldrb r0, [r0]
	cmp r0, #0
	ble _02260E76
	ldr r0, [sp, #4]
	ldr r7, _02260F0C ; =ov93_02262C82
	lsl r0, r0, #4
	add r4, sp, #0x14
	str r0, [sp]
_02260E46:
	ldrh r6, [r7]
	ldr r3, [sp]
	add r0, r5, #0
	add r0, #0x8c
	add r3, r6, r3
	lsl r3, r3, #0x10
	ldr r0, [r0]
	mov r1, #3
	mov r2, #1
	lsr r3, r3, #0x10
	bl PaletteData_GetBufferColorAtIndex
	strh r0, [r4]
	ldr r0, [sp, #0x10]
	add r7, r7, #2
	add r0, r0, #1
	str r0, [sp, #0x10]
	ldr r0, [r5]
	add r4, r4, #2
	add r0, #0x30
	ldrb r1, [r0]
	ldr r0, [sp, #0x10]
	cmp r0, r1
	blt _02260E46
_02260E76:
	bl sub_0203769C
	ldr r3, [r5]
	mov r2, #0
	add r1, r3, #0
	add r1, #0x30
	ldrb r4, [r1]
	cmp r4, #0
	ble _02260E9A
_02260E88:
	add r1, r3, #0
	add r1, #0x2c
	ldrb r1, [r1]
	cmp r0, r1
	beq _02260E9A
	add r2, r2, #1
	add r3, r3, #1
	cmp r2, r4
	blt _02260E88
_02260E9A:
	add r0, r5, #0
	add r0, #0x8c
	ldr r0, [r0]
	mov r1, #3
	bl PaletteData_GetUnfadedBuf
	str r0, [sp, #0xc]
	add r0, r5, #0
	add r0, #0x8c
	ldr r0, [r0]
	mov r1, #3
	bl PaletteData_GetFadedBuf
	ldr r1, [r5]
	str r0, [sp, #8]
	add r0, r1, #0
	add r0, #0x30
	ldrb r6, [r0]
	mov r4, #0
	cmp r6, #0
	ble _02260F02
	ldr r0, [sp, #4]
	lsl r7, r0, #4
_02260EC8:
	add r1, r1, r4
	add r1, #0x2c
	ldrb r1, [r1]
	add r0, r5, #0
	bl ov93_0225E3C4
	lsl r1, r0, #1
	ldr r0, _02260F10 ; =ov93_02262D54
	lsl r2, r6, #3
	add r0, r0, r2
	ldrh r0, [r1, r0]
	lsl r1, r4, #1
	add r2, sp, #0x14
	ldrh r3, [r2, r1]
	add r0, r7, r0
	ldr r2, [sp, #0xc]
	lsl r0, r0, #1
	strh r3, [r2, r0]
	add r2, sp, #0x14
	ldrh r2, [r2, r1]
	ldr r1, [sp, #8]
	add r4, r4, #1
	strh r2, [r1, r0]
	ldr r1, [r5]
	add r0, r1, #0
	add r0, #0x30
	ldrb r6, [r0]
	cmp r4, r6
	blt _02260EC8
_02260F02:
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	nop
_02260F08: .word 0x00002716
_02260F0C: .word ov93_02262C82
_02260F10: .word ov93_02262D54
	thumb_func_end ov93_02260E1C
