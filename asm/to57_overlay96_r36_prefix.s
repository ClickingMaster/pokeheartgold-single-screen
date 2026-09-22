	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

	.text

	.public ov96_021EC5C0

	thumb_func_start ov96_021EC5C0
ov96_021EC5C0: ; 0x021EC5C0
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x40
	add r5, r0, #0
	mov r0, #0
	add r2, sp, #0xc
	add r1, r0, #0
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	str r0, [sp, #8]
	str r0, [r2]
	mov r0, #5
	str r0, [sp, #4]
	mov r0, #0x20
	str r0, [sp]
_021EC5E4:
	mov r0, #0x69
	str r0, [sp, #0x20]
	str r0, [sp, #0x24]
	mov r0, #0x65
	mov r1, #2
	str r0, [sp, #0x28]
	str r0, [sp, #0x2c]
	mov r0, #1
	str r0, [sp, #0x38]
	ldr r0, [sp]
	str r1, [sp, #0x1c]
	str r1, [sp, #0x14]
	add r1, sp, #0xc
	strh r0, [r1]
	mov r0, #0x48
	strh r0, [r1, #2]
	mov r0, #0
	strh r0, [r1, #6]
	ldr r0, [sp, #8]
	mov r3, #0x83
	add r0, r0, #1
	str r0, [sp, #0x18]
	ldr r0, [sp, #4]
	ldr r1, [r5, #0x1c]
	lsl r0, r0, #2
	add r4, r5, r0
	ldr r0, [r5, #0x18]
	add r2, sp, #0xc
	lsl r3, r3, #0xe
	bl SpriteSystem_NewSpriteWithYOffset
	mov r1, #1
	str r0, [r4, #0x20]
	bl ManagedSprite_SetAnimateFlag
	mov r2, #1
	mov r4, #0
	str r4, [sp, #0x18]
	str r2, [sp, #0x38]
	str r2, [sp, #0x14]
	add r0, sp, #0xc
	ldrsh r1, [r0, r4]
	sub r1, #0x10
	strh r1, [r0]
	strh r2, [r0, #6]
	ldr r0, [sp, #4]
	add r7, r0, #1
_021EC642:
	add r0, r7, r4
	lsl r0, r0, #2
	add r6, r5, r0
	mov r3, #0x83
	ldr r0, [r5, #0x18]
	ldr r1, [r5, #0x1c]
	add r2, sp, #0xc
	lsl r3, r3, #0xe
	bl SpriteSystem_NewSpriteWithYOffset
	mov r1, #1
	str r0, [r6, #0x20]
	bl ManagedSprite_SetAnimateFlag
	add r1, sp, #0xc
	mov r0, #0
	ldrsh r1, [r1, r0]
	add r0, sp, #0xc
	add r4, r4, #1
	add r1, #0x10
	strh r1, [r0]
	cmp r4, #3
	blt _021EC642
	ldr r0, [sp, #4]
	add r0, r0, #4
	str r0, [sp, #4]
	ldr r0, [sp]
	add r0, #0x40
	str r0, [sp]
	ldr r0, [sp, #8]
	add r0, r0, #1
	str r0, [sp, #8]
	cmp r0, #4
	blt _021EC5E4
	add sp, #0x40
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov96_021EC5C0
