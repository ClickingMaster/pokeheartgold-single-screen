	.extern AddTextPrinterParameterizedWithColor
	.extern AddWindowParameterized
	.extern FillWindowPixelBuffer
	.extern InitWindow
	.extern NewString_ReadMsgData
	.extern ScheduleWindowCopyToVram
	.extern String_Delete
	.extern ov40_022306C0
	.extern ov40_02245268
	.include "asm/macros.inc"
	.include "overlay_40.inc"
	.include "global.inc"

.text
	thumb_func_start ov40_02237C9C
ov40_02237C9C: ; 0x02237C9C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x28
	mov r1, #0x86
	lsl r1, r1, #4
	str r0, [sp, #0x14]
	ldr r0, [r0, r1]
	mov r1, #0x6d
	str r0, [sp, #0x24]
	mov r0, #1
	str r0, [sp, #0x20]
	ldr r0, [sp, #0x24]
	lsl r1, r1, #2
	ldr r5, _02237D60 ; =ov40_02245268
	mov r7, #0
	add r4, r0, r1
_02237CBA:
	add r0, r4, #0
	bl InitWindow
	ldrb r0, [r5, #1]
	add r1, r4, #0
	mov r2, #2
	str r0, [sp]
	ldrb r0, [r5, #2]
	str r0, [sp, #4]
	ldrb r0, [r5, #3]
	str r0, [sp, #8]
	mov r0, #0xe
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x20]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0x10]
	ldr r0, [sp, #0x14]
	ldrb r3, [r5]
	ldr r0, [r0, #0x24]
	bl AddWindowParameterized
	add r0, r4, #0
	mov r1, #0
	bl FillWindowPixelBuffer
	cmp r7, #0
	bne _02237D16
	mov r1, #0xe
	ldr r0, [sp, #0x14]
	ldr r2, [sp, #0x24]
	lsl r1, r1, #6
	ldr r2, [r2, r1]
	ldr r1, _02237D64 ; =0x00000136
	ldr r0, [r0, #0x48]
	add r1, r2, r1
	bl NewString_ReadMsgData
	add r6, r0, #0
	add r0, r4, #0
	add r1, r6, #0
	bl ov40_022306C0
	str r0, [sp, #0x1c]
	mov r0, #0
	str r0, [sp, #0x18]
_02237D16:
	cmp r6, #0
	beq _02237D3C
	ldr r0, [sp, #0x18]
	ldr r3, [sp, #0x1c]
	str r0, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _02237D68 ; =0x000F0D00
	mov r1, #0
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	add r0, r4, #0
	add r2, r6, #0
	bl AddTextPrinterParameterizedWithColor
	add r0, r6, #0
	bl String_Delete
_02237D3C:
	add r0, r4, #0
	bl ScheduleWindowCopyToVram
	ldrb r1, [r5, #2]
	ldrb r0, [r5, #3]
	add r7, r7, #1
	add r2, r1, #0
	mul r2, r0
	ldr r0, [sp, #0x20]
	add r4, #0x10
	add r0, r0, r2
	add r5, r5, #4
	str r0, [sp, #0x20]
	cmp r7, #1
	blo _02237CBA
	add sp, #0x28
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02237D60: .word ov40_02245268
_02237D64: .word 0x00000136
_02237D68: .word 0x000F0D00
	thumb_func_end ov40_02237C9C
