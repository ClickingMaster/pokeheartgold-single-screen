	.extern ov73_021E71E4
#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_73.inc"
	.include "global.inc"

.text
	thumb_func_start ov73_021E7230
ov73_021E7230: ; 0x021E7230
	push {r4, r5, r6, lr}
	sub sp, #0x10
	add r4, r0, #0
	bl sub_0203769C
	cmp r0, #0
	bne _021E7286
	mov r1, #0
	add r0, r4, #0
	add r2, r1, #0
	bl ov73_021E71E4
	ldr r0, [r4, #0x28]
	mov r1, #0x15
	bl NewString_ReadMsgData
	add r5, r0, #0
	mov r0, #4
	add r1, r5, #0
	mov r2, #0
	bl FontID_String_GetWidth
	mov r1, #0x40
	sub r3, r1, r0
	mov r1, #0
	str r1, [sp]
	lsr r6, r3, #0x1f
	add r6, r3, r6
	ldr r0, _021E72EC ; =0x00040100
	str r1, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0xae
	lsl r0, r0, #2
	str r1, [sp, #0xc]
	add r0, r4, r0
	mov r1, #4
	add r2, r5, #0
	asr r3, r6, #1
	bl AddTextPrinterParameterizedWithColor
	add r0, r5, #0
	bl String_Delete
_021E7286:
	mov r1, #0
	ldr r0, _021E72F0 ; =0x00004A38
	mvn r1, r1
	str r1, [r4, r0]
	add r0, r4, #0
	mov r1, #1
	mov r2, #0
	bl ov73_021E71E4
	ldr r0, [r4, #0x28]
	mov r1, #0x16
	bl NewString_ReadMsgData
	add r5, r0, #0
	mov r0, #4
	add r1, r5, #0
	mov r2, #0
	bl FontID_String_GetWidth
	mov r1, #0x40
	sub r3, r1, r0
	mov r1, #0
	str r1, [sp]
	lsr r6, r3, #0x1f
	add r6, r3, r6
	ldr r0, _021E72EC ; =0x00040100
	str r1, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0xb2
	lsl r0, r0, #2
	str r1, [sp, #0xc]
	add r0, r4, r0
	mov r1, #4
	add r2, r5, #0
	asr r3, r6, #1
	bl AddTextPrinterParameterizedWithColor
	add r0, r5, #0
	bl String_Delete
	ldr r0, [r4]
	mov r1, #1
	bl ScheduleBgTilemapBufferTransfer
	ldr r0, [r4]
	mov r1, #0
	bl ScheduleBgTilemapBufferTransfer
	add sp, #0x10
	pop {r4, r5, r6, pc}
	nop
_021E72EC: .word 0x00040100
_021E72F0: .word 0x00004A38
	thumb_func_end ov73_021E7230
