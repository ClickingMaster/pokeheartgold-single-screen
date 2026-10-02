	.extern ov41_0224ACA4
	.extern ov41_0224ACDC
	.extern ov41_0224AD0C
	.extern ov41_0224AD7C
	.extern ov41_0224AD84
	.extern ov41_0224AE24
	.extern ov41_0224AED8
	.extern ov41_0224AFD4
	.extern ov41_0224B118
	.extern ov41_0224B21C
	.include "asm/macros.inc"
	.include "overlay_41.inc"
	.include "global.inc"

.text
	thumb_func_start ov41_0224AA08
ov41_0224AA08: ; 0x0224AA08
	push {r4, r5, r6, lr}
	sub sp, #0x10
	add r4, r1, #0
	add r6, r2, #0
	mov r1, #1
	add r5, r0, #0
	tst r1, r6
	beq _0224AA22
	mov r2, #0x52
	mov r1, #0
	lsl r2, r2, #2
	bl memset
_0224AA22:
	mov r0, #2
	tst r0, r6
	beq _0224AA30
	ldr r1, [r4]
	add r0, r5, #0
	bl ov41_0224ACA4
_0224AA30:
	mov r0, #4
	add r1, r6, #0
	tst r1, r0
	beq _0224AA54
	mov r1, #0x1b
	str r1, [sp]
	str r0, [sp, #4]
	mov r0, #0x1f
	str r0, [sp, #8]
	mov r0, #1
	str r0, [sp, #0xc]
	add r0, r5, #0
	ldr r1, [r4]
	add r0, #0x2c
	mov r2, #2
	mov r3, #0x13
	bl ov41_0224AD0C
_0224AA54:
	mov r0, #8
	tst r0, r6
	beq _0224AA6C
	ldr r0, [r4, #0x24]
	str r0, [sp]
	add r0, r5, #0
	ldr r1, [r4, #4]
	ldr r2, [r4, #8]
	ldr r3, [r4, #0x10]
	add r0, #0x38
	bl ov41_0224AED8
_0224AA6C:
	mov r0, #0x10
	tst r0, r6
	beq _0224AA8C
	ldr r0, [r4]
	str r0, [sp]
	ldr r0, [r4, #0x20]
	str r0, [sp, #4]
	ldr r0, [r4, #0x24]
	str r0, [sp, #8]
	add r0, r5, #0
	ldr r1, [r4, #4]
	ldr r2, [r4, #8]
	ldr r3, [r4, #0x1c]
	add r0, #0xa0
	bl ov41_0224B118
_0224AA8C:
	mov r0, #0x20
	tst r0, r6
	beq _0224AB02
	mov r0, #0x1b
	str r0, [sp]
	mov r2, #2
	str r2, [sp, #4]
	mov r0, #0x8b
	str r0, [sp, #8]
	mov r3, #1
	str r3, [sp, #0xc]
	add r0, #0xa9
	ldr r1, [r4]
	add r0, r5, r0
	bl ov41_0224AD0C
	mov r0, #0x4d
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0xf
	bl FillWindowPixelBuffer
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _0224AB3C ; =0x0001020F
	mov r1, #0x1b
	str r0, [sp, #8]
	mov r0, #0xff
	str r0, [sp, #0xc]
	add r0, #0x35
	ldr r0, [r5, r0]
	ldr r2, [r4, #0x14]
	mov r3, #6
	bl ov41_0224AE24
	mov r0, #0x48
	str r0, [sp]
	mov r0, #0
	str r0, [sp, #4]
	ldr r0, _0224AB3C ; =0x0001020F
	mov r1, #0x1b
	str r0, [sp, #8]
	mov r0, #0xff
	str r0, [sp, #0xc]
	add r0, #0x35
	ldr r0, [r5, r0]
	ldr r2, [r4, #0x14]
	ldr r3, [r4, #0x18]
	bl ov41_0224AE24
	mov r0, #0x4d
	lsl r0, r0, #2
	mov r2, #1
	ldr r0, [r5, r0]
	mov r1, #0
	add r3, r2, #0
	bl DrawFrameAndWindow2
_0224AB02:
	ldr r0, [r4, #4]
	str r0, [r5, #0x30]
	ldr r0, [r4, #8]
	str r0, [r5, #0x34]
	ldr r0, [r4]
	ldr r1, [r4, #0xc]
	bl ov41_0224ACDC
	ldr r0, [r4, #0xc]
	bl Options_GetFrame
	mov r1, #0x4f
	lsl r1, r1, #2
	str r0, [r5, r1]
	ldr r0, [r4, #0xc]
	bl Options_GetTextFrameDelay
	mov r1, #5
	lsl r1, r1, #6
	str r0, [r5, r1]
	add r0, r1, #0
	sub r0, #8
	ldr r0, [r5, r0]
	sub r1, #8
	orr r0, r6
	str r0, [r5, r1]
	add sp, #0x10
	pop {r4, r5, r6, pc}
	nop
_0224AB3C: .word 0x0001020F
	thumb_func_end ov41_0224AA08
