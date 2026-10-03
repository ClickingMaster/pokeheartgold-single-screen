	.extern ov67_021E6164
	.extern ov67_021E61A0
	.extern ov67_021E61E8
	.include "asm/macros.inc"
	.include "overlay_67.inc"
	.include "global.inc"

.text
	thumb_func_start ov67_021E63E8
ov67_021E63E8: ; 0x021E63E8
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r7, r0, #0
	ldr r0, [r7, #0x24]
	ldr r1, [r7]
	bl sub_0202D8E4
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp]
	mov r0, #0xa7
	ldr r5, [sp, #4]
	lsl r0, r0, #2
	add r4, r7, r0
	add r6, r5, #0
_021E6406:
	add r0, r5, #0
	add r0, #0x20
	ldrb r0, [r0]
	add r1, r4, #0
	add r1, #8
	lsl r0, r0, #0x1e
	lsr r0, r0, #0x1f
	strb r0, [r4, #4]
	ldrb r0, [r5, #0x12]
	mov r2, #8
	strb r0, [r4, #5]
	ldrb r0, [r5, #0x13]
	strb r0, [r4, #6]
	add r0, r5, #0
	add r0, #0x20
	ldrb r0, [r0]
	lsl r0, r0, #0x1f
	lsr r0, r0, #0x1f
	strb r0, [r4, #7]
	add r0, r5, #0
	add r0, #0x18
	bl MI_CpuCopy8
	ldr r1, [r7]
	mov r0, #8
	bl String_New
	str r0, [r4]
	ldrb r0, [r4, #7]
	cmp r0, #0
	ldr r0, [r4]
	beq _021E6454
	ldrb r1, [r4, #4]
	lsl r1, r1, #2
	add r1, r7, r1
	ldr r1, [r1, #0x64]
	bl String_Copy
	b _021E645A
_021E6454:
	add r1, r6, #0
	bl CopyU16ArrayToString
_021E645A:
	ldr r0, [sp]
	add r4, #0x10
	add r0, r0, #1
	add r5, #0x22
	add r6, #0x22
	str r0, [sp]
	cmp r0, #0x1e
	blt _021E6406
	ldr r0, [sp, #4]
	bl Heap_Free
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov67_021E63E8
