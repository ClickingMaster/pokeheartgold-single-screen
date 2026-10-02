	.extern ov41_0224AD0C
	.extern ov41_0224AE24
	.extern ov41_0224AFF8
	.extern ov41_0224B298
	.include "asm/macros.inc"
	.include "overlay_41.inc"
	.include "global.inc"
	.extern ov41_0224B0B8

.text
	thumb_func_start ov41_0224B118
ov41_0224B118: ; 0x0224B118
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x60
	add r4, r1, #0
	mov r1, #0xe5
	str r1, [sp]
	mov r1, #0xe6
	str r1, [sp, #4]
	mov r1, #0xe4
	str r1, [sp, #8]
	mov r1, #0xe3
	str r1, [sp, #0xc]
	mov r1, #2
	str r1, [sp, #0x10]
	ldr r1, _0224B214 ; =0x00000BB8
	add r5, r2, #0
	str r3, [sp, #0x18]
	str r1, [sp, #0x14]
	ldr r3, [sp, #0x80]
	add r7, r0, #0
	add r1, r5, #0
	mov r2, #0xe
	bl ov41_0224AFF8
	add r0, r7, #0
	add r1, r5, #0
	add r2, sp, #0x3c
	mov r3, #0
	bl ov41_0224B0B8
	add r0, sp, #0x3c
	str r0, [sp, #0x20]
	mov r2, #2
	mov r0, #0xe
	mov r1, #0x3a
	str r0, [sp, #0x38]
	lsl r1, r1, #0xc
	lsl r0, r2, #0x14
	mov r6, #0
	str r4, [sp, #0x1c]
	str r1, [sp, #0x28]
	add r0, r1, r0
	str r2, [sp, #0x34]
	str r6, [sp, #0x30]
	str r0, [sp, #0x28]
	mov r4, #0x67
	add r5, r7, #0
_0224B174:
	str r4, [sp, #0x24]
	lsl r0, r4, #0xc
	str r0, [sp, #0x24]
	add r0, sp, #0x1c
	bl Sprite_Create
	str r0, [r5, #0x10]
	add r6, r6, #1
	add r4, #0x18
	add r5, r5, #4
	cmp r6, #2
	blt _0224B174
	ldr r0, [sp, #0x18]
	mov r1, #0x1e
	str r0, [r7, #0x1c]
	mul r1, r0
	str r1, [r7, #0x20]
	ldr r1, [sp, #0x7c]
	str r1, [r7, #0x2c]
	str r0, [r1]
	ldr r1, [r7, #0x2c]
	str r0, [r1, #8]
	add r0, r7, #0
	mov r1, #0
	add r0, #0x90
	str r1, [r0]
	add r0, r7, #0
	bl ov41_0224B298
	mov r0, #0xe
	str r0, [sp]
	mov r0, #4
	str r0, [sp, #4]
	mov r0, #0xc1
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	add r0, r7, #0
	ldr r1, [sp, #0x78]
	add r0, #0x18
	mov r2, #0xa
	mov r3, #8
	bl ov41_0224AD0C
	ldr r0, [r7, #0x18]
	mov r1, #0
	bl FillWindowPixelBuffer
	mov r0, #0
	str r0, [sp]
	mov r3, #4
	ldr r0, _0224B218 ; =0x00010200
	str r3, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0xff
	str r0, [sp, #0xc]
	ldr r0, [r7, #0x18]
	mov r1, #0x1b
	mov r2, #0xd7
	bl ov41_0224AE24
	mov r0, #0x48
	str r0, [sp]
	mov r0, #4
	str r0, [sp, #4]
	ldr r0, _0224B218 ; =0x00010200
	mov r1, #0x1b
	str r0, [sp, #8]
	mov r0, #0xff
	str r0, [sp, #0xc]
	ldr r0, [r7, #0x18]
	mov r2, #0xd7
	mov r3, #5
	bl ov41_0224AE24
	ldr r0, [r7, #0x18]
	bl CopyWindowToVram
	add sp, #0x60
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0224B214: .word 0x00000BB8
_0224B218: .word 0x00010200
	thumb_func_end ov41_0224B118
