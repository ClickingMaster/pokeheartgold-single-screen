	.extern AddTextPrinterParameterizedWithColor
	.extern GetMonData
	.extern Party_GetCount
	.extern Party_GetMonByIndex
	.extern ReadMsgDataIntoString
	.extern SaveArray_Party_Get
	.extern ScheduleWindowCopyToVram
	.extern ov59_0223AEB0
	.extern ov59_0223BA64
	.extern ov59_0223BC88
	.extern ov59_0223C99C
	.include "asm/macros.inc"
	.include "overlay_59.inc"
	.include "global.inc"

.text
	thumb_func_start ov59_0223AE20
ov59_0223AE20: ; 0x0223AE20
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r5, r0, #0
	ldr r0, [r5, #8]
	bl SaveArray_Party_Get
	str r0, [r5, #0x10]
	bl Party_GetCount
	mov r6, #0
	add r4, r6, #0
	str r0, [sp, #4]
	cmp r0, #0
	ble _0223AE7E
	add r0, r5, #0
	str r0, [sp, #8]
	add r0, #0x7c
	str r0, [sp, #8]
_0223AE44:
	ldr r0, [r5, #0x10]
	add r1, r4, #0
	bl Party_GetMonByIndex
	mov r1, #0x4c
	mov r2, #0
	add r7, r0, #0
	bl GetMonData
	cmp r0, #0
	bne _0223AE76
	ldr r0, [r5]
	add r1, r6, #0
	str r0, [sp]
	mov r0, #0x34
	mul r1, r0
	ldr r0, [sp, #8]
	lsl r3, r4, #0x18
	add r0, r0, r1
	ldr r1, [r5, #0x10]
	add r2, r7, #0
	lsr r3, r3, #0x18
	add r6, r6, #1
	bl ov59_0223BA64
_0223AE76:
	ldr r0, [sp, #4]
	add r4, r4, #1
	cmp r4, r0
	blt _0223AE44
_0223AE7E:
	add r5, #0x49
	strb r6, [r5]
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov59_0223AE20
