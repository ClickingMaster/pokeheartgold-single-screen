	.extern ov57_0223BE18
	.include "asm/macros.inc"
	.include "overlay_57.inc"
	.include "global.inc"

.text
	thumb_func_start ov57_02239184
ov57_02239184: ; 0x02239184
	push {r4, r5, r6, r7, lr}
	sub sp, #0x64
	ldr r4, _02239208 ; =ov57_0223BE18
	add r6, r0, #0
	add r3, sp, #0
	mov r2, #6
_02239190:
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, r2, #1
	bne _02239190
	mov r7, #0
	add r4, sp, #0
	add r5, r6, #0
_0223919E:
	ldr r1, [r4]
	add r0, sp, #0
	strh r1, [r0, #0x30]
	ldr r1, [r4, #4]
	add r2, sp, #0x30
	strh r1, [r0, #0x32]
	mov r1, #0
	strh r1, [r0, #0x34]
	strh r1, [r0, #0x36]
	mov r0, #0x28
	str r0, [sp, #0x38]
	mov r0, #2
	str r0, [sp, #0x3c]
	str r0, [sp, #0x40]
	mov r0, #1
	str r0, [sp, #0x5c]
	add r0, r1, #0
	str r0, [sp, #0x60]
	ldr r0, _0223920C ; =0x0000AFC8
	str r0, [sp, #0x44]
	ldr r0, _02239210 ; =0x00006594
	str r0, [sp, #0x48]
	ldr r0, _02239214 ; =0x0000B798
	str r0, [sp, #0x4c]
	ldr r0, _02239218 ; =0x0000BB80
	str r0, [sp, #0x50]
	sub r0, r1, #1
	str r0, [sp, #0x54]
	sub r0, r1, #1
	str r0, [sp, #0x58]
	add r0, r6, #0
	add r1, r6, #0
	add r0, #0xdc
	add r1, #0xe0
	ldr r0, [r0]
	ldr r1, [r1]
	bl SpriteSystem_NewSprite
	mov r1, #0xcf
	lsl r1, r1, #2
	str r0, [r5, r1]
	add r0, r1, #0
	ldr r0, [r5, r0]
	ldr r1, [r4, #8]
	bl ManagedSprite_SetFlipMode
	add r7, r7, #1
	add r4, #0xc
	add r5, r5, #4
	cmp r7, #4
	blt _0223919E
	add sp, #0x64
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_02239208: .word ov57_0223BE18
_0223920C: .word 0x0000AFC8
_02239210: .word 0x00006594
_02239214: .word 0x0000B798
_02239218: .word 0x0000BB80
	thumb_func_end ov57_02239184
