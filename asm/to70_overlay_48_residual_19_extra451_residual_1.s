	.extern ov48_0225A30C
	.extern ov48_0225A790
	.extern ov48_0225A95C
	.extern ov48_0225AA38
	.extern ov48_0225AA50
	.extern ov48_0225ACAC
	.extern ov48_0225ACD8
	.extern ov48_0225B038
	.include "asm/macros.inc"
	.include "overlay_48.inc"
	.include "global.inc"

.text
	thumb_func_start ov48_0225A354
ov48_0225A354: ; 0x0225A354
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x20
	add r5, r0, #0
	str r1, [sp]
	add r0, #0x40
	mov r1, #0
	mov r2, #0x48
	bl memset
	mov r6, #0
	strh r6, [r5, #0x3c]
	add r4, r5, #0
	strh r6, [r5, #0x3e]
	add r4, #0xa0
_0225A370:
	add r0, r4, #0
	bl ov48_0225AA50
	add r6, r6, #1
	add r4, #0x28
	cmp r6, #5
	blt _0225A370
	mov r0, #0x6e
	lsl r0, r0, #2
	add r0, r5, r0
	bl ov48_0225ACAC
	add r0, r5, #0
	str r0, [sp, #8]
	add r0, #0xa0
	mov r7, #0
	str r0, [sp, #8]
	add r0, r5, #0
	mvn r7, r7
	str r0, [sp, #0xc]
	add r0, #0x3e
	mov r6, #0
	str r7, [sp, #0x10]
	str r0, [sp, #0xc]
_0225A3A0:
	mov r0, #2
	sub r4, r0, r6
	add r0, r5, #0
	add r1, sp, #0x14
	add r2, r6, #0
	bl ov48_0225A30C
	cmp r0, #1
	bne _0225A406
	ldr r0, [sp, #0x14]
	bl ov48_0225B038
	mov r1, #0x28
	str r0, [sp, #4]
	mul r1, r0
	ldr r0, [sp, #8]
	mov r2, #1
	add r0, r0, r1
	add r1, r4, #0
	bl ov48_0225AA38
	ldr r3, [sp]
	add r0, r5, #0
	add r1, r4, #0
	add r2, sp, #0x14
	bl ov48_0225A790
	lsl r0, r4, #2
	add r1, r5, r0
	lsl r3, r7, #0x18
	ldr r0, [sp, #4]
	add r1, #0x88
	str r0, [r1]
	ldr r2, [sp, #0x14]
	mov r0, #0x6e
	lsl r0, r0, #2
	lsl r1, r4, #0x18
	lsl r2, r2, #0x18
	add r0, r5, r0
	lsr r1, r1, #0x18
	lsr r2, r2, #0x18
	lsr r3, r3, #0x18
	bl ov48_0225ACD8
	ldr r0, [sp, #0xc]
	ldrh r0, [r0]
	add r1, r0, #6
	ldr r0, [sp, #0xc]
	strh r1, [r0]
	ldr r7, [sp, #0x14]
	b _0225A412
_0225A406:
	lsl r0, r4, #2
	add r1, r5, r0
	add r1, #0x88
	mov r0, #5
	ldr r7, [sp, #0x10]
	str r0, [r1]
_0225A412:
	add r6, r6, #1
	cmp r6, #3
	blt _0225A3A0
	add sp, #0x20
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov48_0225A354
