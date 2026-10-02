	.extern ov31_0225E4BC
	.extern ov31_0225E51C
	.include "asm/macros.inc"
	.include "overlay_31.inc"
	.include "global.inc"

.text
	thumb_func_start ov31_0225E5FC
ov31_0225E5FC: ; 0x0225E5FC
	push {r4, r5, lr}
	sub sp, #0xc
	add r4, r0, #0
	mov r0, #0x51
	lsl r0, r0, #2
	add r0, r4, r0
	mov r1, #0xf
	bl FillWindowPixelBuffer
	ldr r2, [r4, #0x14]
	ldr r3, _0225E6F8 ; =0x00000283
	ldrb r0, [r2, r3]
	add r1, r0, #0
	add r1, #0xfd
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	cmp r1, #1
	bhi _0225E63E
	mov r1, #0x55
	add r3, r3, #1
	lsl r1, r1, #2
	ldrh r2, [r2, r3]
	ldr r1, [r4, r1]
	mov r3, #0
	bl ov31_0225E51C
	mov r0, #0x56
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #0x2e
	bl NewString_ReadMsgData
	b _0225E684
_0225E63E:
	mov r1, #0
	str r1, [sp]
	mov r0, #1
	str r0, [sp, #4]
	mov r0, #0x55
	add r3, r3, #3
	lsl r0, r0, #2
	ldrsh r2, [r2, r3]
	ldr r0, [r4, r0]
	mov r3, #2
	bl BufferIntegerAsString
	mov r5, #0xa3
	ldr r2, [r4, #0x14]
	mov r0, #0
	str r0, [sp]
	mov r1, #1
	mov r0, #0x55
	str r1, [sp, #4]
	lsl r5, r5, #2
	ldr r3, [r2, r5]
	sub r5, r5, #6
	ldrsh r2, [r2, r5]
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mul r2, r3
	mov r3, #6
	bl BufferIntegerAsString
	mov r0, #0x56
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #0xe
	bl NewString_ReadMsgData
_0225E684:
	mov r1, #0x55
	lsl r1, r1, #2
	add r5, r0, #0
	ldr r0, [r4, r1]
	add r1, #0x34
	ldr r1, [r4, r1]
	add r2, r5, #0
	bl StringExpandPlaceholders
	add r0, r5, #0
	bl String_Delete
	mov r0, #0x59
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl Options_GetFrame
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	mov r1, #4
	str r1, [sp, #4]
	ldr r0, [r4, #4]
	ldr r2, _0225E6FC ; =0x000001B5
	mov r3, #5
	bl LoadUserFrameGfx2
	mov r2, #0x51
	lsl r2, r2, #2
	add r0, r4, r2
	mov r1, #1
	add r2, #0x71
	mov r3, #5
	bl DrawFrameAndWindow2
	mov r0, #0x59
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl Options_GetTextFrameDelay
	mov r3, #0
	str r3, [sp]
	mov r2, #0x51
	str r0, [sp, #4]
	lsl r2, r2, #2
	add r0, r4, r2
	str r3, [sp, #8]
	add r2, #0x44
	ldr r2, [r4, r2]
	mov r1, #1
	bl AddTextPrinterParameterized
	mov r1, #0x61
	lsl r1, r1, #2
	str r0, [r4, r1]
	add sp, #0xc
	pop {r4, r5, pc}
	nop
_0225E6F8: .word 0x00000283
_0225E6FC: .word 0x000001B5
	thumb_func_end ov31_0225E5FC
