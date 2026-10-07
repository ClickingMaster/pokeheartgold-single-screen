	.extern ov71_02247390
	.extern ov71_022473A8
	.include "asm/macros.inc"
	.include "overlay_71.inc"
	.include "global.inc"

.text
	thumb_func_start ov71_02247F9C
ov71_02247F9C: ; 0x02247F9C
	push {r3, r4, r5, lr}
	sub sp, #0x20
	add r5, r0, #0
	ldr r0, [r5]
	bl ov71_02247390
	add r4, r0, #0
	add r0, sp, #0x10
	add r1, r4, #0
	mov r2, #2
	mov r3, #0
	bl GetBoxmonSpriteCharAndPlttNarcIds
	ldr r0, [r5]
	bl ov71_022473A8
	add r2, r0, #0
	add r0, r5, #0
	add r0, #0x80
	add r1, r5, #0
	ldr r0, [r0]
	add r1, #0x14
	mov r3, #1
	bl NARC_ReadPokepicAnimScript
	add r0, r4, #0
	mov r1, #2
	mov r2, #0
	bl sub_02070854
	add r3, r0, #0
	mov r1, #0
	str r1, [sp]
	add r0, r5, #0
	str r1, [sp, #4]
	add r0, #0x14
	str r0, [sp, #8]
	str r1, [sp, #0xc]
	ldr r0, [r5, #0xc]
	add r3, #0x50
	add r1, sp, #0x10
	mov r2, #0x80
	bl PokepicManager_CreatePokepic
	add sp, #0x20
	pop {r3, r4, r5, pc}
	thumb_func_end ov71_02247F9C
