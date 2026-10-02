	.extern ov68_021E6DDC
	.extern ov68_021E6EB8
	.extern ov68_021E7BEC
	.extern ov68_021E7E74
	.include "asm/macros.inc"
	.include "overlay_68.inc"
	.include "global.inc"

.text
	thumb_func_start ov68_021E7178
ov68_021E7178: ; 0x021E7178
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	add r4, r1, #0
	bl ov68_021E6DDC
	add r0, r6, #0
	add r1, r4, #0
	bl ov68_021E6EB8
	ldr r4, _021E71C0 ; =ov68_021E7E74
	mov r7, #0
	add r5, r6, #0
_021E7190:
	mov r0, #0x47
	mov r1, #0x12
	lsl r0, r0, #2
	lsl r1, r1, #4
	ldr r0, [r6, r0]
	ldr r1, [r6, r1]
	add r2, r4, #0
	bl SpriteSystem_NewSprite
	mov r1, #0x49
	lsl r1, r1, #2
	str r0, [r5, r1]
	add r0, r1, #0
	ldr r0, [r5, r0]
	mov r1, #0
	bl ManagedSprite_SetAnimateFlag
	add r7, r7, #1
	add r4, #0x34
	add r5, r5, #4
	cmp r7, #0xc
	blo _021E7190
	pop {r3, r4, r5, r6, r7, pc}
	nop
_021E71C0: .word ov68_021E7E74
	thumb_func_end ov68_021E7178
