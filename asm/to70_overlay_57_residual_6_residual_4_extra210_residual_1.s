	.include "asm/macros.inc"
	.include "overlay_57.inc"
	.include "global.inc"

.text
	thumb_func_start ov57_02238E48
ov57_02238E48: ; 0x02238E48
	push {r4, r5, r6, r7, lr}
	sub sp, #0x4c
	add r5, r0, #0
	ldr r1, [r5]
	mov r6, #0
	ldr r0, [r1]
	cmp r0, #0
	ble _02238F36
	str r6, [sp, #0xc]
	add r4, r5, #0
_02238E5C:
	ldr r0, [sp, #0xc]
	add r0, r1, r0
	ldr r7, [r0, #4]
	add r0, r7, #0
	bl Pokemon_GetIconNaix
	add r3, r0, #0
	mov r0, #0
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, _02238F3C ; =0x00003A98
	add r1, r5, #0
	add r0, r6, r0
	str r0, [sp, #8]
	add r0, r5, #0
	add r0, #0xdc
	add r1, #0xe0
	ldr r0, [r0]
	ldr r1, [r1]
	mov r2, #0x14
	bl SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType
	mov r1, #0
	add r0, sp, #0x18
	strh r1, [r0]
	strh r1, [r0, #2]
	strh r1, [r0, #4]
	strh r1, [r0, #6]
	mov r0, #0xa
	str r0, [sp, #0x20]
	add r0, r1, #0
	str r0, [sp, #0x24]
	mov r0, #1
	str r0, [sp, #0x28]
	mov r0, #2
	str r0, [sp, #0x44]
	add r0, r1, #0
	str r0, [sp, #0x48]
	ldr r0, _02238F3C ; =0x00003A98
	add r2, sp, #0x18
	add r0, r6, r0
	str r0, [sp, #0x2c]
	mov r0, #0xfa
	lsl r0, r0, #6
	str r0, [sp, #0x30]
	ldr r0, _02238F40 ; =0x00004268
	str r0, [sp, #0x34]
	ldr r0, _02238F44 ; =0x00004650
	str r0, [sp, #0x38]
	sub r0, r1, #1
	str r0, [sp, #0x3c]
	sub r0, r1, #1
	str r0, [sp, #0x40]
	add r0, r5, #0
	add r1, r5, #0
	add r0, #0xdc
	add r1, #0xe0
	ldr r0, [r0]
	ldr r1, [r1]
	bl SpriteSystem_NewSprite
	mov r1, #0xc9
	lsl r1, r1, #2
	str r0, [r4, r1]
	add r0, r7, #0
	mov r1, #5
	mov r2, #0
	bl GetMonData
	str r0, [sp, #0x10]
	add r0, r7, #0
	mov r1, #0x4c
	mov r2, #0
	bl GetMonData
	str r0, [sp, #0x14]
	add r0, r7, #0
	mov r1, #0x70
	mov r2, #0
	bl GetMonData
	add r1, r0, #0
	ldr r0, [sp, #0x10]
	ldr r2, [sp, #0x14]
	bl GetMonIconPaletteEx
	add r1, r0, #0
	mov r0, #0xc9
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	ldr r0, [r0]
	bl Sprite_SetPalOffsetRespectVramOffset
	mov r0, #0xc9
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #1
	bl ManagedSprite_SetAnim
	ldr r0, [sp, #0xc]
	ldr r1, [r5]
	add r0, r0, #4
	str r0, [sp, #0xc]
	ldr r0, [r1]
	add r6, r6, #1
	add r4, r4, #4
	cmp r6, r0
	blt _02238E5C
_02238F36:
	add sp, #0x4c
	pop {r4, r5, r6, r7, pc}
	nop
_02238F3C: .word 0x00003A98
_02238F40: .word 0x00004268
_02238F44: .word 0x00004650
	thumb_func_end ov57_02238E48
