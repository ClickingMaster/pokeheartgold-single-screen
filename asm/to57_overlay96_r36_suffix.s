	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

	.text

	.public ov96_021EC70C
	.public ov96_021EC790

	thumb_func_start ov96_021EC70C
ov96_021EC70C: ; 0x021EC70C
	push {r4, r5, r6, r7, lr}
	sub sp, #0x34
	mov r7, #0
	add r5, r0, #0
	add r2, sp, #0
	add r0, r7, #0
	add r1, r7, #0
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	str r0, [r2]
	mov r4, #0x60
_021EC72A:
	add r1, r7, #0
	add r1, #0x6b
	str r1, [sp, #0x14]
	str r1, [sp, #0x18]
	mov r1, #0x67
	str r1, [sp, #0x1c]
	str r1, [sp, #0x20]
	mov r1, #1
	str r1, [sp, #0x10]
	mov r1, #0
	str r1, [sp, #0x2c]
	mov r1, #2
	str r1, [sp, #8]
	add r1, sp, #0
	add r0, r7, #0
	strh r4, [r1]
	mov r2, #0x38
	strh r2, [r1, #2]
	add r1, r5, #0
	add r1, #0xb1
	ldrb r1, [r1]
	add r0, #0x18
	cmp r1, #5
	bhs _021EC760
	mov r2, #0x80
	add r1, sp, #0
	strh r2, [r1]
_021EC760:
	lsl r0, r0, #2
	add r6, r5, r0
	mov r3, #0x83
	ldr r0, [r5, #0x18]
	ldr r1, [r5, #0x1c]
	add r2, sp, #0
	lsl r3, r3, #0xe
	bl SpriteSystem_NewSpriteWithYOffset
	str r0, [r6, #0x20]
	mov r1, #1
	bl ManagedSprite_SetAnimateFlag
	ldr r0, [r6, #0x20]
	mov r1, #0
	bl ManagedSprite_SetDrawFlag
	add r7, r7, #1
	add r4, #0x40
	cmp r7, #2
	blt _021EC72A
	add sp, #0x34
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov96_021EC70C

	thumb_func_start ov96_021EC790
ov96_021EC790: ; 0x021EC790
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x40
	str r0, [sp, #0xc]
	str r1, [sp, #0x10]
	bl PokeathlonCourse_GetHeapAllocPtr4
	add r4, r0, #0
	ldr r0, [sp, #0xc]
	ldr r1, [sp, #0x10]
	bl PokeathlonCourse_GetParticipantUnk04
	ldr r0, [sp, #0xc]
	ldr r1, [sp, #0x10]
	bl PokeathlonCourse_GetParticipantData
	mov r0, #0
	str r0, [sp, #0x14]
	add r6, r4, #0
	mov r5, #1
	add r7, sp, #0x1c
_021EC7B8:
	ldr r0, [sp, #0xc]
	ldr r1, [sp, #0x10]
	ldr r2, [sp, #0x14]
	add r3, sp, #0x30
	bl ov96_021E6168
	ldr r0, [r4]
	add r1, sp, #0x30
	str r0, [sp]
	mov r0, #0
	str r0, [sp, #4]
	ldr r0, [r6, #0x74]
	mov r2, #1
	mov r3, #0
	bl ov96_021EEBF8
	ldrb r0, [r7, #0x1a]
	mov r3, #2
	str r0, [sp]
	ldrh r0, [r7, #0x16]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #4]
	ldr r0, [sp, #0x3c]
	str r0, [sp, #8]
	ldrh r1, [r7, #0x14]
	ldrb r2, [r7, #0x1b]
	add r0, sp, #0x20
	bl GetMonSpriteCharAndPlttNarcIdsEx
	ldrh r0, [r7, #4]
	ldrh r1, [r7, #8]
	ldr r3, [r4]
	add r2, sp, #0x1c
	bl GfGfxLoader_GetPlttData
	ldr r3, [sp, #0x1c]
	str r0, [sp, #0x18]
	lsl r1, r5, #0x18
	ldr r0, [r4, #0x14]
	ldr r3, [r3, #0xc]
	lsr r1, r1, #0x18
	mov r2, #1
	bl ov96_021EAF60
	ldr r0, [sp, #0x18]
	bl Heap_Free
	ldr r0, [sp, #0x14]
	add r6, r6, #4
	add r0, r0, #1
	add r5, r5, #3
	str r0, [sp, #0x14]
	cmp r0, #3
	blt _021EC7B8
	add sp, #0x40
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov96_021EC790
