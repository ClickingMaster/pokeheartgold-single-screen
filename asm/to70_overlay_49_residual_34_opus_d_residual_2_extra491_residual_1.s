	.include "asm/macros.inc"
	.include "overlay_49.inc"
	.include "global.inc"

.text
	thumb_func_start ov49_0225C368
ov49_0225C368: ; 0x0225C368
	push {r4, r5, r6, r7}
	ldr r1, [r0, #8]
	ldr r4, [r0, #0xc]
	lsr r3, r1, #1
	mov r2, #0
	cmp r3, #0
	ble _0225C3BC
	mov r5, #0xc8
_0225C378:
	ldrh r6, [r4]
	mov r0, #0x1f
	add r2, r2, #1
	add r1, r6, #0
	asr r7, r6, #5
	and r1, r0
	and r0, r7
	asr r7, r6, #0xa
	mov r6, #0x1f
	and r6, r7
	mov r7, #0x1d
	mul r7, r6
	mov r6, #0x4c
	mul r6, r1
	mov r1, #0x97
	mul r1, r0
	add r0, r6, r1
	add r0, r7, r0
	asr r6, r0, #8
	lsl r0, r6, #8
	add r7, r6, #0
	asr r1, r0, #8
	lsl r0, r6, #7
	mul r7, r5
	asr r0, r0, #8
	asr r6, r7, #8
	lsl r0, r0, #0xa
	lsl r6, r6, #5
	orr r0, r6
	orr r0, r1
	strh r0, [r4]
	add r4, r4, #2
	cmp r2, r3
	blt _0225C378
_0225C3BC:
	pop {r4, r5, r6, r7}
	bx lr
	thumb_func_end ov49_0225C368
