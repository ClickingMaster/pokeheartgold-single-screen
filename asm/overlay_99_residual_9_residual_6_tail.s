	.include "asm/macros.inc"
	.include "overlay_99.inc"
	.include "global.inc"

	.text
	.public ov99_021E86D4

	thumb_func_start ov99_021E86D4
ov99_021E86D4: ; 0x021E86D4
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x28
	add r7, r0, #0
	ldr r0, [sp, #0x44]
	str r2, [sp, #0xc]
	str r3, [sp, #0x10]
	cmp r0, #0
	beq _021E86E8
	mov r0, #6
	b _021E86EA
_021E86E8:
	mov r0, #2
_021E86EA:
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x1c]
	mov r0, #6
	mul r0, r1
	str r0, [sp, #0x18]
	mov r0, #0x5a
	mov r5, #0
	ldr r1, [sp, #0x40]
	lsl r0, r0, #2
	mul r0, r1
	str r5, [sp, #0x20]
	str r0, [sp, #0x14]
_021E8704:
	ldr r0, [sp, #0x10]
	mov r4, #0
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
_021E870C:
	ldr r0, [sp, #0x18]
	add r1, r4, #1
	add r0, r1, r0
	add r1, r0, r5
	ldr r0, [sp, #0x14]
	add r2, sp, #0x24
	add r1, r0, r1
	add r0, sp, #0x24
	strh r1, [r0]
	str r6, [sp]
	mov r0, #1
	str r0, [sp, #4]
	str r0, [sp, #8]
	ldr r3, [sp, #0xc]
	ldr r0, [r7, #4]
	add r3, r3, r4
	lsl r3, r3, #0x18
	ldr r1, [sp, #0x1c]
	lsr r3, r3, #0x18
	bl LoadRectToBgTilemapRect
	add r4, r4, #1
	cmp r4, #6
	blt _021E870C
	ldr r0, [sp, #0x10]
	add r5, #0x3c
	add r0, r0, #1
	str r0, [sp, #0x10]
	ldr r0, [sp, #0x20]
	add r0, r0, #1
	str r0, [sp, #0x20]
	cmp r0, #6
	blt _021E8704
	ldr r0, [r7, #4]
	ldr r1, [sp, #0x1c]
	bl BgCommitTilemapBufferToVram
	add sp, #0x28
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov99_021E86D4
