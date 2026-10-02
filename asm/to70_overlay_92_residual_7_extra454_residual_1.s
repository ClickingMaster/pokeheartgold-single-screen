	.extern ov92_0225DDD8
	.extern ov92_0225DF0C
	.extern ov92_02263B54
	.include "asm/macros.inc"
	.include "overlay_92.inc"
	.include "global.inc"

.text
	thumb_func_start ov92_0225DE08
ov92_0225DE08: ; 0x0225DE08
	push {r4, r5, r6, r7, lr}
	sub sp, #0x44
	ldr r1, [r0, #0x14]
	str r0, [sp, #8]
	ldr r0, [r1, #8]
	mov r6, #0
	ldr r7, [r1, #0xc]
	ldr r5, [sp, #8]
	str r0, [sp, #0xc]
	add r4, r6, #0
_0225DE1C:
	mov r0, #3
	str r0, [sp]
	ldr r0, _0225DE6C ; =0x0000232E
	lsl r1, r4, #0x10
	str r0, [sp, #4]
	add r0, sp, #0x10
	asr r1, r1, #0x10
	mov r2, #0x14
	mov r3, #1
	bl ov92_0225DDD8
	ldr r0, [sp, #0xc]
	add r1, r7, #0
	add r2, sp, #0x10
	bl SpriteSystem_NewSprite
	mov r1, #3
	str r0, [r5, #0x40]
	bl ManagedSprite_SetPaletteOverrideOffset
	add r0, r6, #0
	mov r1, #0xb
	bl _s32_div_f
	ldr r0, [r5, #0x40]
	add r1, r1, #1
	bl ManagedSprite_SetAnim
	add r6, r6, #1
	add r4, #8
	add r5, r5, #4
	cmp r6, #0x24
	blt _0225DE1C
	mov r1, #0
	ldr r0, [sp, #8]
	add r2, r1, #0
	bl ov92_0225DF0C
	add sp, #0x44
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0225DE6C: .word 0x0000232E
	thumb_func_end ov92_0225DE08
