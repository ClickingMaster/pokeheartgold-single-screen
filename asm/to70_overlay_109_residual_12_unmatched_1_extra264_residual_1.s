	.extern _021E7884
	.extern ov109_021E7178
	.include "asm/macros.inc"
	.include "overlay_109.inc"
	.include "global.inc"

.text
	thumb_func_start ov109_021E6FD4
ov109_021E6FD4: ; 0x021E6FD4
	push {r4, r5, lr}
	sub sp, #0x1c
	cmp r2, #1
	bne _021E6FE4
	lsl r2, r3, #1
	mov r4, #3
	add r2, r2, #1
	b _021E6FEA
_021E6FE4:
	lsl r2, r3, #1
	mov r4, #0x1c
	add r2, r2, #2
_021E6FEA:
	lsl r2, r2, #0x18
	lsr r3, r2, #0x18
	add r2, r0, #0
	add r2, #0xbc
	ldr r2, [r2]
	mov r5, #1
	str r5, [sp]
	mov r5, #2
	str r5, [sp, #4]
	add r5, r2, #0
	add r5, #0xc
	lsl r3, r3, #0x18
	str r5, [sp, #8]
	lsr r3, r3, #0x18
	str r3, [sp, #0xc]
	mov r3, #0x18
	str r3, [sp, #0x10]
	ldrh r3, [r2]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	lsl r3, r3, #0x15
	lsr r3, r3, #0x18
	str r3, [sp, #0x14]
	ldrh r2, [r2, #2]
	mov r3, #0xf
	lsl r2, r2, #0x15
	lsr r2, r2, #0x18
	str r2, [sp, #0x18]
	ldr r0, [r0, #0x14]
	add r2, r4, #0
	bl CopyToBgTilemapRect
	add sp, #0x1c
	pop {r4, r5, pc}
	.balign 4, 0
	thumb_func_end ov109_021E6FD4

	thumb_func_start ov109_021E7030
ov109_021E7030: ; 0x021E7030
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	add r5, r0, #0
	mov r0, #0
	str r0, [sp, #0x18]
	str r0, [sp, #0x14]
	mov r0, #0xc
	mul r0, r2
	str r3, [sp, #8]
	str r0, [sp, #0x10]
	ldr r0, [sp, #8]
	str r1, [sp, #4]
	add r0, r0, #2
	str r0, [sp, #0xc]
_021E704C:
	ldr r4, [sp, #8]
	ldr r0, [sp, #0xc]
	add r1, r4, #0
	cmp r1, r0
	bge _021E70AA
	ldr r1, _021E70C0 ; =_021E7884
	ldr r0, [sp, #0x14]
	add r1, r1, r0
	add r0, r4, #0
	add r6, r1, r0
	ldr r0, [sp, #0x18]
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
_021E7066:
	ldrb r1, [r6]
	ldr r0, [sp, #0x10]
	add r0, r1, r0
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	add r0, r5, #0
	add r0, #0xc5
	ldrb r0, [r0]
	cmp r1, r0
	str r7, [sp]
	bhs _021E7092
	lsl r1, r1, #3
	add r1, r5, r1
	add r1, #0xd0
	lsl r3, r4, #0x18
	ldr r1, [r1]
	ldr r2, [sp, #4]
	add r0, r5, #0
	lsr r3, r3, #0x18
	bl ov109_021E7178
	b _021E70A0
_021E7092:
	lsl r3, r4, #0x18
	ldr r2, [sp, #4]
	add r0, r5, #0
	mov r1, #0
	lsr r3, r3, #0x18
	bl ov109_021E7178
_021E70A0:
	ldr r0, [sp, #0xc]
	add r4, r4, #1
	add r6, r6, #1
	cmp r4, r0
	blt _021E7066
_021E70AA:
	ldr r0, [sp, #0x14]
	add r0, r0, #4
	str r0, [sp, #0x14]
	ldr r0, [sp, #0x18]
	add r0, r0, #1
	str r0, [sp, #0x18]
	cmp r0, #3
	blt _021E704C
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	nop
_021E70C0: .word _021E7884
	thumb_func_end ov109_021E7030
