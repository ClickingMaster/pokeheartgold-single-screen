	.extern ov102_021E8F6C
	.extern ov102_021E8F90
	.extern ov102_021EA268
	.extern ov102_021EA270
	.extern ov102_021EA2B4
	.extern ov102_021EA874
	.extern ov102_021EAB30
	.extern ov102_021EAC20
	.extern ov102_021EAD5C
	.extern ov102_021EAD70
	.extern ov102_021EAD98
	.extern ov102_021EB088
	.include "asm/macros.inc"
	.include "overlay_102.inc"
	.include "global.inc"

.text
	thumb_func_start ov102_021EA920
ov102_021EA920: ; 0x021EA920
	push {r4, r5, lr}
	sub sp, #0x54
	add r4, r0, #0
	add r0, #0x8c
	ldr r0, [r0]
	cmp r0, #0
	beq _021EA93A
	add r0, r4, #0
	add r0, #0x84
	add r1, sp, #8
	bl ov102_021EAD5C
	b _021EA944
_021EA93A:
	mov r1, #0x80
	add r0, sp, #8
	strh r1, [r0]
	mov r1, #0x18
	strh r1, [r0, #2]
_021EA944:
	mov r2, #0
	ldr r0, [r4]
	add r1, sp, #0x30
	add r3, r2, #0
	bl ov102_021EA270
	ldr r0, [r4]
	add r1, sp, #0xc
	mov r2, #1
	mov r3, #0
	bl ov102_021EA270
	mov r2, #0
	str r2, [sp]
	mov r0, #1
	str r0, [sp, #4]
	add r5, sp, #8
	mov r3, #2
	ldrsh r2, [r5, r2]
	ldrsh r3, [r5, r3]
	ldr r0, [r4]
	add r1, sp, #0x30
	bl ov102_021EA2B4
	str r0, [r4, #0x60]
	mov r2, #0
	str r2, [sp]
	mov r5, #2
	str r5, [sp, #4]
	add r3, sp, #8
	ldrsh r2, [r3, r2]
	ldrsh r5, [r3, r5]
	ldr r3, _021EAA38 ; =0x000001E2
	ldr r0, [r4]
	ldrsh r3, [r4, r3]
	add r1, sp, #0xc
	sub r3, r5, r3
	bl ov102_021EA2B4
	mov r1, #1
	str r0, [r4, #0x64]
	bl Sprite_SetAnimCtrlSeq
	add r0, r4, #0
	mov r1, #1
	bl ov102_021EB088
	mov r0, #1
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, [r4]
	add r1, sp, #0x30
	mov r2, #8
	mov r3, #0x40
	bl ov102_021EA2B4
	mov r1, #0x13
	str r0, [r4, #0x68]
	bl Sprite_SetAnimCtrlSeq
	mov r0, #1
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, [r4]
	add r1, sp, #0x30
	mov r2, #0xf8
	mov r3, #0x40
	bl ov102_021EA2B4
	mov r1, #0x15
	str r0, [r4, #0x6c]
	bl Sprite_SetAnimCtrlSeq
	mov r0, #1
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, [r4]
	add r1, sp, #0x30
	mov r2, #0x1c
	mov r3, #0x40
	bl ov102_021EA2B4
	mov r1, #0x12
	str r0, [r4, #0x74]
	bl Sprite_SetAnimCtrlSeq
	mov r0, #1
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, [r4]
	add r1, sp, #0x30
	mov r2, #0x80
	mov r3, #0x40
	bl ov102_021EA2B4
	str r0, [r4, #0x70]
	mov r1, #0x17
	bl Sprite_SetAnimCtrlSeq
	ldr r0, [r4, #4]
	bl ov102_021E8F6C
	cmp r0, #2
	beq _021EAA34
	ldr r0, [r4, #0x6c]
	mov r1, #0
	bl Sprite_SetDrawFlag
	ldr r0, [r4, #0x68]
	mov r1, #0
	bl Sprite_SetDrawFlag
	ldr r0, [r4, #0x70]
	mov r1, #0
	bl Sprite_SetDrawFlag
	ldr r0, [r4, #0x74]
	mov r1, #0
	bl Sprite_SetDrawFlag
_021EAA34:
	add sp, #0x54
	pop {r4, r5, pc}
	.balign 4, 0
_021EAA38: .word 0x000001E2
	thumb_func_end ov102_021EA920
