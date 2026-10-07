	.include "asm/macros.inc"
	.include "overlay_80_022324C4.inc"
	.include "global.inc"
	.public ov80_02233594

.text
	thumb_func_start ov80_02233594
ov80_02233594: ; 0x02233594
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	add r5, r0, #0
	str r3, [sp, #0x14]
	str r1, [sp, #0xc]
	ldr r1, [r5, #0x34]
	mov r0, #0x14
	str r2, [sp, #0x10]
	bl String_New
	add r4, r0, #0
	ldr r1, [r5, #0x34]
	mov r0, #0x14
	bl String_New
	add r6, r0, #0
	mov r0, #0
	mov r1, #0x1b
	mov r2, #0x1f
	mov r3, #0xb
	bl NewMsgDataFromNarc
	add r7, r0, #0
	mov r0, #1
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, [r5, #0x44]
	ldr r2, [sp, #0x14]
	mov r1, #0
	mov r3, #4
	bl BufferIntegerAsString
	add r0, r7, #0
	mov r1, #2
	add r2, r4, #0
	bl ReadMsgDataIntoString
	ldr r0, [r5, #0x44]
	add r1, r6, #0
	add r2, r4, #0
	bl StringExpandPlaceholders
	mov r3, #0x10
	str r3, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	mov r1, #0
	ldr r0, [sp, #0xc]
	str r1, [sp, #8]
	add r2, r6, #0
	bl AddTextPrinterParameterized
	ldr r0, [r5, #0x44]
	ldr r2, [sp, #0x10]
	mov r1, #0
	bl BufferPlayersName
	add r0, r7, #0
	mov r1, #0
	add r2, r4, #0
	bl ReadMsgDataIntoString
	ldr r0, [r5, #0x44]
	add r1, r6, #0
	add r2, r4, #0
	bl StringExpandPlaceholders
	mov r1, #0
	str r1, [sp]
	str r1, [sp, #4]
	ldr r0, [sp, #0xc]
	add r2, r6, #0
	add r3, r1, #0
	str r1, [sp, #8]
	bl AddTextPrinterParameterized
	add r0, r4, #0
	bl String_Delete
	add r0, r6, #0
	bl String_Delete
	add r0, r7, #0
	bl DestroyMsgData
	ldr r0, [sp, #0xc]
	bl CopyWindowToVram
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov80_02233594
