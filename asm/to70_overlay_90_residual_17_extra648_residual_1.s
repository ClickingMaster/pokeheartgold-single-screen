	.extern ov90_022588CC
	.include "asm/macros.inc"
	.include "overlay_90.inc"
	.include "global.inc"

.text
	thumb_func_start ov90_0225A258
ov90_0225A258: ; 0x0225A258
	push {r3, r4, r5, r6}
	ldrb r4, [r0, #8]
	mov r3, #0
	add r2, r3, #0
	sub r4, r4, #1
	cmp r4, #0
	ble _0225A286
	mov r5, #1
_0225A268:
	add r4, r1, r2
	ldrb r6, [r4, #1]
	ldrb r4, [r1, r2]
	cmp r4, r6
	bne _0225A27A
	cmp r4, #0
	beq _0225A27A
	cmp r6, #0
	bne _0225A27C
_0225A27A:
	add r3, r5, #0
_0225A27C:
	ldrb r4, [r0, #8]
	add r2, r2, #1
	sub r4, r4, #1
	cmp r2, r4
	blt _0225A268
_0225A286:
	add r0, r3, #0
	pop {r3, r4, r5, r6}
	bx lr
	thumb_func_end ov90_0225A258
