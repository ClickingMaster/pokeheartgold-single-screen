	.extern ov08_02225BE0
	.extern ov08_02225BE4
#include "constants/moves.h"
	.include "asm/macros.inc"
	.include "overlay_08.inc"
	.include "global.inc"

.text
	.force_active on
	thumb_func_start ov08_022235D4
ov08_022235D4: ; 0x022235D4
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	add r5, r0, #0
	ldr r4, [r5, #0x2c]
	lsl r6, r3, #4
	add r7, r1, #0
	add r0, r4, r6
	mov r1, #0
	str r2, [sp, #0x10]
	bl FillWindowPixelBuffer
	ldr r0, _02223670 ; =0x0000114D
	lsl r7, r7, #2
	ldrb r1, [r5, r0]
	mov r0, #0x90
	mul r0, r1
	add r0, r5, r0
	add r0, r0, r7
	ldrh r0, [r0, #0x3c]
	cmp r0, #0
	beq _02223666
	ldr r1, [sp, #0x10]
	ldr r0, [r5, #0x10]
	lsl r2, r1, #3
	ldr r1, _02223674 ; =ov08_02225BE0
	ldr r1, [r1, r2]
	bl NewString_ReadMsgData
	ldr r2, _02223670 ; =0x0000114D
	str r0, [sp, #0x14]
	ldrb r3, [r5, r2]
	mov r2, #0x90
	ldr r0, [r5, #0x14]
	mul r2, r3
	add r2, r5, r2
	add r2, r2, r7
	ldrh r2, [r2, #0x3c]
	mov r1, #0
	bl BufferItemName
	ldr r0, [r5, #0x14]
	ldr r1, [r5, #0x18]
	ldr r2, [sp, #0x14]
	bl StringExpandPlaceholders
	ldr r0, [sp, #0x30]
	ldr r1, [r5, #0x18]
	mov r2, #0
	bl FontID_String_GetWidth
	add r7, r0, #0
	add r0, r4, r6
	bl GetWindowWidth
	lsl r0, r0, #3
	sub r0, r0, r7
	lsr r3, r0, #1
	mov r0, #7
	str r0, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, [sp, #0x34]
	ldr r1, [sp, #0x30]
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	ldr r2, [r5, #0x18]
	add r0, r4, r6
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x14]
	bl String_Delete
_02223666:
	add r0, r4, r6
	bl ScheduleWindowCopyToVram
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02223670: .word 0x0000114D
_02223674: .word ov08_02225BE0
	thumb_func_end ov08_022235D4
