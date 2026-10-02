	.extern ov90_02258EB4
	.extern ov90_02259F7C
	.extern ov90_0225A050
	.extern ov90_0225A134
	.extern ov90_0225C274
	.extern ov90_0225C276
	.extern ov90_0225C310
	.extern ov90_0225C314
	.extern ov90_0225C318
	.include "asm/macros.inc"
	.include "overlay_90.inc"
	.include "global.inc"

.text
	thumb_func_start ov90_02259BCC
ov90_02259BCC: ; 0x02259BCC
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x30
	add r5, r0, #0
	ldr r0, [sp, #0x48]
	add r6, r3, #0
	str r0, [sp, #0x48]
	ldr r0, [sp, #0x54]
	str r1, [sp, #0x1c]
	ldr r4, [sp, #0x4c]
	str r0, [sp, #0x54]
	cmp r2, #0
	beq _02259BEC
	cmp r2, #1
	beq _02259BFA
	cmp r2, #2
	bne _02259C10
_02259BEC:
	ldr r0, [sp, #0x1c]
	ldr r2, [sp, #0x50]
	add r1, r4, #0
	bl ov90_0225A050
	add r7, r0, #0
	b _02259C12
_02259BFA:
	ldr r0, [sp, #0x50]
	cmp r0, r4
	bls _02259C04
	add r7, r4, #1
	b _02259C12
_02259C04:
	cmp r0, r4
	bne _02259C0C
	mov r7, #0
	b _02259C12
_02259C0C:
	add r7, r4, #0
	b _02259C12
_02259C10:
	add r7, r4, #0
_02259C12:
	mov r0, #1
	mov r1, #2
	bl SetBgPriority
	mov r0, #0
	mov r1, #1
	bl SetBgPriority
	ldr r1, [sp, #0x1c]
	mov r0, #0xc
	add r2, r1, #0
	mul r2, r0
	str r2, [sp, #0x2c]
	ldr r2, _02259D38 ; =ov90_0225C310
	ldr r1, [sp, #0x2c]
	mov r3, #5
	add r1, r2, r1
	ldrb r1, [r7, r1]
	mov r2, #0
	str r1, [sp, #0x28]
	add r1, r1, #1
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	str r1, [sp]
	mov r1, #0x1a
	str r1, [sp, #4]
	mov r1, #4
	str r1, [sp, #8]
	str r0, [sp, #0xc]
	mov r0, #0x68
	add r1, r4, #0
	mul r1, r0
	ldr r0, _02259D3C ; =0x00000201
	add r0, r1, r0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0x10]
	ldr r0, [sp, #0x48]
	add r1, r5, #0
	ldr r0, [r0]
	bl AddWindowParameterized
	add r0, r5, #0
	mov r1, #0
	bl FillWindowPixelBuffer
	mov r0, #0x20
	str r0, [sp]
	mov r0, #6
	str r0, [sp, #4]
	add r0, r6, #0
	add r0, #0xc
	str r0, [sp, #8]
	mov r2, #0
	ldr r1, _02259D40 ; =ov90_0225C314
	ldr r0, [sp, #0x2c]
	str r2, [sp, #0xc]
	add r0, r1, r0
	ldrb r0, [r4, r0]
	ldr r3, [sp, #0x28]
	mov r1, #1
	str r0, [sp, #0x10]
	ldrh r0, [r6]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x18
	str r0, [sp, #0x14]
	ldrh r0, [r6, #2]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x18
	str r0, [sp, #0x18]
	ldr r0, [sp, #0x48]
	ldr r0, [r0]
	bl CopyToBgTilemapRect
	ldr r0, [sp, #0x28]
	ldr r1, _02259D44 ; =ov90_0225C318
	lsl r0, r0, #3
	str r0, [sp, #0x20]
	ldr r0, [sp, #0x2c]
	ldr r2, _02259D48 ; =ov90_0225C274
	add r0, r1, r0
	ldrb r0, [r7, r0]
	ldr r3, _02259D4C ; =ov90_0225C276
	ldr r1, [sp, #0x54]
	lsl r6, r0, #2
	ldrsh r2, [r2, r6]
	str r0, [sp, #0x24]
	mov r0, #0
	ldrsh r6, [r3, r6]
	str r0, [sp]
	ldr r0, [sp, #0x5c]
	ldr r3, [sp, #0x20]
	str r0, [sp, #4]
	add r3, r6, r3
	lsl r2, r2, #0x18
	lsl r3, r3, #0x18
	ldr r0, [sp, #0x58]
	ldr r1, [r1]
	lsr r2, r2, #0x18
	lsr r3, r3, #0x18
	bl ov90_02258EB4
	ldr r1, [sp, #0x24]
	str r0, [r5, #0x10]
	bl Sprite_SetAnimCtrlSeq
	ldr r0, [r5, #0x10]
	add r1, r4, #0
	bl Sprite_SetPalOffset
	ldr r0, [r5, #0x10]
	mov r1, #0
	bl Sprite_SetDrawFlag
	ldr r0, [r5, #0x10]
	mov r1, #1
	bl Sprite_SetAnimActiveFlag
	mov r0, #0
	str r0, [sp]
	ldr r0, [sp, #0x5c]
	ldr r3, [sp, #0x20]
	str r0, [sp, #4]
	ldr r1, [sp, #0x54]
	add r3, #0x15
	str r3, [sp, #0x20]
	lsl r3, r3, #0x18
	ldr r0, [sp, #0x58]
	ldr r1, [r1]
	mov r2, #0x18
	lsr r3, r3, #0x18
	bl ov90_02258EB4
	str r0, [r5, #0x14]
	mov r1, #0
	bl Sprite_SetAnimCtrlSeq
	ldr r0, [r5, #0x14]
	mov r1, #0
	bl Sprite_SetDrawFlag
	strb r4, [r5, #0x1a]
	ldr r0, [sp, #0x1c]
	strb r7, [r5, #0x1b]
	strh r0, [r5, #0x18]
	add sp, #0x30
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02259D38: .word ov90_0225C310
_02259D3C: .word 0x00000201
_02259D40: .word ov90_0225C314
_02259D44: .word ov90_0225C318
_02259D48: .word ov90_0225C274
_02259D4C: .word ov90_0225C276
	thumb_func_end ov90_02259BCC
