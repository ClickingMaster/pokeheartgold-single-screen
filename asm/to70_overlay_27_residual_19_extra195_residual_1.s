	.extern ov27_0225D118
	.extern ov27_0225D11A
	.include "asm/macros.inc"
	.include "overlay_27.inc"
	.include "global.inc"

.text
	thumb_func_start ov27_0225C80C
ov27_0225C80C: ; 0x0225C80C
	push {r4, r5, lr}
	sub sp, #0x5c
	add r4, r1, #0
	mov r1, #0xa
	add r5, r0, #0
	add r0, r1, #0
	str r1, [sp]
	sub r0, #0xb
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r2, #0
	str r2, [sp, #0xc]
	mov r0, #0xd1
	str r2, [sp, #0x10]
	lsl r0, r0, #2
	ldr r3, [r5, r0]
	str r3, [sp, #0x14]
	add r3, r0, #4
	ldr r3, [r5, r3]
	str r3, [sp, #0x18]
	add r3, r0, #0
	add r3, #8
	ldr r3, [r5, r3]
	str r3, [sp, #0x1c]
	add r3, r0, #0
	add r3, #0xc
	ldr r3, [r5, r3]
	add r0, #0x20
	str r3, [sp, #0x20]
	str r2, [sp, #0x24]
	str r2, [sp, #0x28]
	add r0, r5, r0
	add r2, r1, #0
	add r3, r1, #0
	bl CreateSpriteResourcesHeader
	mov r0, #0x86
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	ldr r1, _0225C8C8 ; =ov27_0225D118
	lsl r2, r4, #2
	ldrh r1, [r1, r2]
	str r0, [sp, #0x2c]
	mov r0, #0xd9
	lsl r1, r1, #0xc
	str r1, [sp, #0x34]
	ldr r1, _0225C8CC ; =ov27_0225D11A
	lsl r0, r0, #2
	ldrh r2, [r1, r2]
	mov r1, #1
	lsl r1, r1, #8
	add r2, r2, r1
	lsl r1, r1, #4
	add r0, r5, r0
	str r0, [sp, #0x30]
	mov r0, #0
	lsl r2, r2, #0xc
	str r0, [sp, #0x3c]
	str r2, [sp, #0x38]
	str r1, [sp, #0x40]
	str r1, [sp, #0x44]
	str r1, [sp, #0x48]
	add r1, sp, #0x2c
	strh r0, [r1, #0x20]
	mov r0, #1
	str r0, [sp, #0x50]
	mov r0, #2
	str r0, [sp, #0x54]
	mov r0, #8
	str r0, [sp, #0x58]
	add r0, sp, #0x2c
	bl Sprite_CreateAffine
	mov r1, #0xe2
	lsl r1, r1, #2
	str r0, [r5, r1]
	ldr r0, [r5, r1]
	mov r1, #1
	bl Sprite_SetAnimCtrlSeq
	mov r0, #0xe2
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #1
	bl Sprite_SetAnimActiveFlag
	mov r0, #0xe2
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
	add sp, #0x5c
	pop {r4, r5, pc}
	.balign 4, 0
_0225C8C8: .word ov27_0225D118
_0225C8CC: .word ov27_0225D11A
	thumb_func_end ov27_0225C80C
