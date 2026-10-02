	.extern ov57_02238958
	.include "asm/macros.inc"
	.include "overlay_57.inc"
	.include "global.inc"

.text
	thumb_func_start ov57_022387E0
ov57_022387E0: ; 0x022387E0
	push {r4, r5, r6, r7, lr}
	sub sp, #0x5c
	add r5, r0, #0
	add r4, r1, #0
	str r2, [sp, #0x10]
	add r7, r3, #0
	mov r0, #0
	mov r1, #0x1b
	mov r2, #0xb
	mov r3, #0x34
	bl NewMsgDataFromNarc
	add r1, r4, #5
	str r0, [sp, #0x14]
	bl NewString_ReadMsgData
	str r0, [sp, #0x18]
	add r0, sp, #0x1c
	bl InitWindow
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	add r0, r5, #0
	add r0, #0xe4
	ldr r0, [r0]
	add r1, sp, #0x1c
	mov r2, #0xa
	mov r3, #2
	bl AddTextWindowTopLeftCorner
	mov r3, #0
	str r3, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _022388DC ; =0x000F0D02
	ldr r2, [sp, #0x18]
	str r0, [sp, #8]
	add r0, sp, #0x1c
	mov r1, #2
	str r3, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	add r0, sp, #0x1c
	mov r1, #2
	mov r2, #0x34
	bl sub_02013688
	mov r3, #0x9a
	lsl r3, r3, #2
	mov r1, #0xc
	add r6, r4, #0
	mul r6, r1
	add r3, r5, r3
	mov r1, #1
	mov r2, #2
	add r3, r3, r6
	bl sub_02021AC8
	mov r0, #0x97
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	str r0, [sp, #0x2c]
	add r0, sp, #0x1c
	str r0, [sp, #0x30]
	add r0, r5, #0
	add r0, #0xe0
	ldr r0, [r0]
	bl SpriteManager_GetSpriteList
	str r0, [sp, #0x34]
	add r0, r5, #0
	add r0, #0xe0
	ldr r0, [r0]
	ldr r1, _022388E0 ; =0x00007530
	bl SpriteManager_FindPlttResourceProxy
	str r0, [sp, #0x38]
	mov r2, #0
	mov r0, #0x9b
	str r2, [sp, #0x3c]
	add r1, r5, r6
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	str r0, [sp, #0x40]
	ldr r1, [sp, #0x18]
	mov r0, #2
	bl FontID_String_GetWidth
	lsr r1, r0, #1
	ldr r0, [sp, #0x10]
	add r7, #0xc0
	sub r0, r0, r1
	str r0, [sp, #0x44]
	mov r0, #1
	str r0, [sp, #0x4c]
	mov r0, #0x28
	str r0, [sp, #0x50]
	mov r0, #2
	str r0, [sp, #0x54]
	mov r0, #0x34
	str r0, [sp, #0x58]
	mov r0, #0x26
	lsl r0, r0, #4
	add r5, r5, r0
	str r7, [sp, #0x48]
	lsl r4, r4, #2
	add r0, sp, #0x2c
	bl sub_020135D8
	str r0, [r5, r4]
	ldr r0, [r5, r4]
	ldr r1, [sp, #0x70]
	bl sub_020138E0
	ldr r0, [sp, #0x18]
	bl String_Delete
	ldr r0, [sp, #0x14]
	bl DestroyMsgData
	add r0, sp, #0x1c
	bl RemoveWindow
	add sp, #0x5c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_022388DC: .word 0x000F0D02
_022388E0: .word 0x00007530
	thumb_func_end ov57_022387E0
