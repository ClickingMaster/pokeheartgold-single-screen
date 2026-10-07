	.extern ov28_0225DD58
	.extern ov28_0225DEB0
	.extern ov28_0225DF14
	.extern ov28_0225DFBC
	.extern ov28_0225E060
	.extern ov28_0225E0F4
	.extern ov28_0225EA90
	.extern ov28_0225EAAC
	.extern ov28_0225EB84
	.include "asm/macros.inc"
	.include "overlay_28.inc"
	.include "global.inc"

.text
thumb_func_start ov28_0225E1A4
ov28_0225E1A4: ; 0x0225E1A4
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r6, r0, #0
	bl ov28_0225DD58
	mov r0, #0x83
	lsl r0, r0, #2
	ldr r1, [r6, r0]
	cmp r1, #0
	bne _0225E1C8
	add r0, r6, #0
	bl ov28_0225DEB0
	mov r0, #0x21
	mov r1, #0
	lsl r0, r0, #4
	str r1, [r6, r0]
	b _0225E236
_0225E1C8:
	sub r0, r0, #4
	ldrsh r0, [r6, r0]
	bl abs
	lsl r0, r0, #0x10
	asr r4, r0, #0x10
	ldr r0, _0225E310 ; =0x0000020A
	ldrsh r0, [r6, r0]
	bl abs
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	cmp r4, #8
	bgt _0225E1F8
	cmp r0, #8
	bgt _0225E1F8
	add r0, r6, #0
	bl ov28_0225E0F4
	mov r0, #0x21
	mov r1, #4
	lsl r0, r0, #4
	str r1, [r6, r0]
	b _0225E236
_0225E1F8:
	cmp r4, #0xc
	bgt _0225E210
	cmp r0, #0xc
	bgt _0225E210
	add r0, r6, #0
	bl ov28_0225E060
	mov r0, #0x21
	mov r1, #3
	lsl r0, r0, #4
	str r1, [r6, r0]
	b _0225E236
_0225E210:
	cmp r4, #0x11
	bgt _0225E228
	cmp r0, #0x11
	bgt _0225E228
	add r0, r6, #0
	bl ov28_0225DFBC
	mov r0, #0x21
	mov r1, #2
	lsl r0, r0, #4
	str r1, [r6, r0]
	b _0225E236
_0225E228:
	add r0, r6, #0
	bl ov28_0225DF14
	mov r0, #0x21
	mov r1, #1
	lsl r0, r0, #4
	str r1, [r6, r0]
_0225E236:
	mov r1, #0x93
	lsl r1, r1, #2
	ldrh r0, [r6, r1]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x1f
	bne _0225E27E
	mov r2, #0x22
	lsl r2, r2, #4
	mov r0, #0
	add r1, r6, #0
	sub r3, r2, #4
	add r4, r2, #0
	add r5, r2, #4
_0225E250:
	ldr r7, [r1, r2]
	add r0, r0, #1
	str r7, [r1, r3]
	ldr r7, [r1, r4]
	str r7, [r1, r5]
	add r1, #0xc
	cmp r0, #4
	blo _0225E250
	mov r1, #0x93
	lsl r1, r1, #2
	ldrh r2, [r6, r1]
	mov r0, #2
	lsl r0, r0, #0xe
	orr r0, r2
	strh r0, [r6, r1]
	ldrh r2, [r6, r1]
	ldr r0, _0225E314 ; =0xFFFF8000
	add sp, #8
	and r2, r0
	mov r0, #1
	orr r0, r2
	strh r0, [r6, r1]
	pop {r3, r4, r5, r6, r7, pc}
_0225E27E:
	mov r0, #0
	sub r1, #0x30
	str r0, [sp]
	add r0, r6, r1
	add r4, r6, #0
	str r0, [sp, #4]
_0225E28A:
	mov r0, #0x22
	lsl r0, r0, #4
	mov r1, #0x5a
	ldr r0, [r4, r0]
	lsl r1, r1, #2
	bl _u32_div_f
	mov r0, #0x22
	lsl r0, r0, #4
	str r1, [r4, r0]
	sub r0, r0, #4
	mov r1, #0x5a
	ldr r0, [r4, r0]
	lsl r1, r1, #2
	bl _u32_div_f
	mov r0, #0x87
	lsl r0, r0, #2
	str r1, [r4, r0]
	ldr r5, [r4, r0]
	add r0, r0, #4
	ldr r7, [r4, r0]
	sub r0, r7, r5
	bl abs
	cmp r0, #0xb4
	blt _0225E2EA
	cmp r7, r5
	bls _0225E2D6
	mov r0, #0x87
	lsl r0, r0, #2
	ldr r1, [r4, r0]
	sub r0, #0xb4
	add r1, r1, r0
	mov r0, #0x87
	lsl r0, r0, #2
	str r1, [r4, r0]
	b _0225E2EA
_0225E2D6:
	cmp r7, r5
	bhs _0225E2EA
	mov r0, #0x22
	lsl r0, r0, #4
	ldr r1, [r4, r0]
	sub r0, #0xb8
	add r1, r1, r0
	mov r0, #0x22
	lsl r0, r0, #4
	str r1, [r4, r0]
_0225E2EA:
	mov r1, #0x21
	lsl r1, r1, #4
	ldr r1, [r6, r1]
	ldr r0, [sp, #4]
	lsl r2, r1, #2
	ldr r1, _0225E318 ; =ov28_0225EAAC
	ldr r1, [r1, r2]
	blx r1
	ldr r0, [sp, #4]
	add r4, #0xc
	add r0, #0xc
	str r0, [sp, #4]
	ldr r0, [sp]
	add r0, r0, #1
	str r0, [sp]
	cmp r0, #4
	blo _0225E28A
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0225E310: .word 0x0000020A
_0225E314: .word 0xFFFF8000
_0225E318: .word ov28_0225EAAC
	thumb_func_end ov28_0225E1A4
