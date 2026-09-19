	.include "asm/macros.inc"
	.include "overlay_87.inc"
	.include "global.inc"
	.extern ov87_021E7EF0
	.extern ov87_021E8394
	.public ov87_021E7A44
	.public ov87_021E7AF8
	.public ov87_021E7DF8

	.extern ov87_021E7BC4
	.extern ov87_021E7C50
	.extern ov87_021E7CE0
	.extern ov87_021E7D6C

	.text

thumb_func_start ov87_021E7AB0
ov87_021E7AB0: ; 0x021E7AB0
	push {r3, r4, r5, r6}
	mov r3, #1
	mov r4, #0xff
	tst r3, r1
	bne _021E7ABE
	mov r3, #0xf
	b _021E7AC0
_021E7ABE:
	mov r3, #0xf0
_021E7AC0:
	eor r3, r4
	lsl r3, r3, #0x18
	lsr r4, r1, #3
	mov r6, #7
	lsr r5, r2, #3
	and r1, r6
	lsr r3, r3, #0x18
	lsl r4, r4, #5
	lsl r5, r5, #0xa
	lsr r1, r1, #1
	cmp r2, #8
	bhs _021E7ADC
	lsl r2, r2, #2
	b _021E7AE0
_021E7ADC:
	lsl r2, r2, #0x1d
	lsr r2, r2, #0x1b
_021E7AE0:
	mov r6, #0xe3
	lsl r6, r6, #2
	add r4, r5, r4
	add r1, r1, r4
	ldr r0, [r0, r6]
	add r2, r2, r1
	ldrb r1, [r0, r2]
	and r1, r3
	strb r1, [r0, r2]
	pop {r3, r4, r5, r6}
	bx lr
	.balign 4, 0
	thumb_func_end ov87_021E7AB0
