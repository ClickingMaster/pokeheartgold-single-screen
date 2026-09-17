	.include "asm/macros.inc"
	.include "overlay_87.inc"
	.include "global.inc"
	.extern ov87_021E6AE0
	.extern ov87_021E6AF4
	.extern ov87_021E6B28
	.extern ov87_021E7008
	.extern ov87_021E7048
	.extern ov87_021E708C
	.extern ov87_021E70D0
	.extern ov87_021E7134
	.extern ov87_021E717C
	.extern ov87_021E71B4
	.extern ov87_021E71EC
	.extern ov87_021E7324
	.extern ov87_021E7334
	.extern ov87_021E734C
	.extern ov87_021E73E0
	.extern ov87_021E7460
	.extern ov87_021E7490
	.extern ov87_021E74B8
	.extern ov87_021E74D4
	.extern ov87_021E74F4
	.extern ov87_021E7550
	.extern ov87_021E75E0
	.extern ov87_021E7698
	.extern ov87_021E7734
	.extern ov87_021E788C
	.extern ov87_021E78D8
	.extern ov87_021E7990
	.extern ov87_021E7998
	.extern ov87_021E79A0
	.extern ov87_021E7A2C
	.extern ov87_021E7FD4
	.extern ov87_021E7FE0
	.extern ov87_021E7FEC
	.extern ov87_021E8014
	.extern ov87_021E803C
	.extern ov87_021E8058
	.extern ov87_021E806C
	.extern ov87_021E8078
	.extern ov87_021E8084
	.extern ov87_021E80B4
	.extern ov87_021E8134
	.extern ov87_021E8184
	.extern ov87_021E818C
	.extern ov87_021E81A0
	.extern ov87_021E81A2
	.extern ov87_021E81C0
	.extern ov87_021E82E4
	.extern ov87_021E8308
	.public ov87_021E5B48

	.text

thumb_func_start ov87_021E5B48
ov87_021E5B48: ; 0x021E5B48
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	ldrb r0, [r6, #8]
	cmp r0, #0
	beq _021E5B5C
	cmp r0, #1
	beq _021E5BCE
	cmp r0, #2
	beq _021E5C22
	b _021E5C2A
_021E5B5C:
	mov r0, #0xff
	mvn r0, r0
	ldr r4, _021E5C30 ; =ov87_021E81A0
	strh r0, [r6, #0x10]
	mov r7, #0
	add r5, r6, #0
_021E5B68:
	mov r1, #0x10
	mov r2, #0
	ldrsh r1, [r6, r1]
	ldrsh r2, [r4, r2]
	mov r0, #0xcb
	lsl r0, r0, #2
	add r1, r1, r2
	mov r2, #2
	ldrsh r2, [r4, r2]
	ldr r0, [r5, r0]
	bl ov87_021E7FEC
	mov r0, #0xcb
	lsl r0, r0, #2
	add r1, r7, #0
	ldr r0, [r5, r0]
	add r1, #0x14
	bl ov87_021E7FE0
	add r7, r7, #1
	add r4, r4, #4
	add r5, r5, #4
	cmp r7, #4
	blt _021E5B68
	mov r0, #0x59
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	bl Options_GetFrame
	add r1, r0, #0
	mov r0, #0x53
	lsl r0, r0, #2
	add r0, r6, r0
	bl ov87_021E8134
	add r0, r6, #0
	bl ov87_021E7048
	mov r0, #1
	add r1, r0, #0
	bl GfGfx_EngineBTogglePlanes
	mov r0, #3
	strh r0, [r6, #0x12]
	mov r0, #0x56
	lsl r0, r0, #4
	bl PlaySE
	mov r0, #1
	strb r0, [r6, #8]
	b _021E5C2A
_021E5BCE:
	mov r1, #0x10
	ldrsh r0, [r6, r1]
	add r0, #0x20
	strh r0, [r6, #0x10]
	mov r0, #0x12
	ldrsh r0, [r6, r0]
	lsl r2, r0, #2
	mov r0, #0xcb
	add r3, r6, r2
	lsl r0, r0, #2
	ldr r0, [r3, r0]
	ldrsh r3, [r6, r1]
	ldr r1, _021E5C30 ; =ov87_021E81A0
	ldrsh r1, [r1, r2]
	add r1, r3, r1
	ldr r3, _021E5C34 ; =ov87_021E81A2
	ldrsh r2, [r3, r2]
	bl ov87_021E7FEC
	mov r0, #0x10
	ldrsh r0, [r6, r0]
	cmp r0, #0
	blt _021E5C2A
	mov r0, #0x12
	ldrsh r0, [r6, r0]
	cmp r0, #0
	bne _021E5C0A
	mov r0, #2
	strb r0, [r6, #8]
	b _021E5C2A
_021E5C0A:
	mov r0, #0x56
	lsl r0, r0, #4
	bl PlaySE
	mov r0, #0x12
	ldrsh r0, [r6, r0]
	sub r0, r0, #1
	strh r0, [r6, #0x12]
	mov r0, #0xff
	mvn r0, r0
	strh r0, [r6, #0x10]
	b _021E5C2A
_021E5C22:
	mov r0, #0
	strh r0, [r6, #0x10]
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_021E5C2A:
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
	nop
_021E5C30: .word ov87_021E81A0
_021E5C34: .word ov87_021E81A2
	thumb_func_end ov87_021E5B48
