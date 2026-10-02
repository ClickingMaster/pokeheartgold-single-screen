	.extern ov89_0225A958
	.extern ov89_0225AB64
	.extern ov89_0225ADA4
	.extern ov89_0225AEA8
	.include "asm/macros.inc"
	.include "overlay_89.inc"
	.include "global.inc"

.text
	thumb_func_start ov89_0225B2E8
ov89_0225B2E8: ; 0x0225B2E8
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r5, r1, #0
	add r7, r0, #0
	add r0, r5, #0
	add r0, #0x94
	add r4, r5, #0
	ldr r0, [r0]
	add r4, #0x94
	cmp r0, #3
	bls _0225B300
	b _0225B42E
_0225B300:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0225B30C: ; jump table
	.short _0225B314 - _0225B30C - 2 ; case 0
	.short _0225B372 - _0225B30C - 2 ; case 1
	.short _0225B3AC - _0225B30C - 2 ; case 2
	.short _0225B3C0 - _0225B30C - 2 ; case 3
_0225B314:
	add r0, r5, #0
	add r0, #0x1c
	mov r1, #2
	bl sub_020182EC
	ldr r1, _0225B444 ; =0x00000AAA
	sub r0, r0, r1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	mov r0, #0xe
	lsl r0, r0, #0xc
	cmp r6, r0
	bhi _0225B364
	mov r1, #9
	lsl r1, r1, #6
	ldr r1, [r5, r1]
	ldr r0, [r5, #0x18]
	lsl r1, r1, #0x10
	lsr r1, r1, #0x18
	bl ov89_0225AB64
	ldr r1, [r5, #0x18]
	add r0, r7, #0
	bl ov89_0225A958
	mov r1, #9
	lsl r1, r1, #6
	ldr r2, [r5, r1]
	mov r0, #0xf0
	bic r2, r0
	mov r0, #0x10
	orr r2, r0
	lsl r0, r0, #0xa
	add r0, r6, r0
	lsl r0, r0, #0x10
	str r2, [r5, r1]
	lsr r6, r0, #0x10
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
_0225B364:
	add r0, r5, #0
	add r0, #0x1c
	add r1, r6, #0
	mov r2, #2
	bl sub_020182E0
	b _0225B42E
_0225B372:
	mov r1, #9
	lsl r1, r1, #6
	ldr r2, [r5, r1]
	mov r0, #0xf0
	bic r2, r0
	add r0, r5, #0
	str r2, [r5, r1]
	add r0, #0x1c
	mov r1, #2
	bl sub_020182EC
	ldr r1, _0225B444 ; =0x00000AAA
	sub r0, r0, r1
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	mov r0, #3
	lsl r0, r0, #0xe
	cmp r1, r0
	blo _0225B3A0
	ldr r0, [r4]
	mov r1, #0
	add r0, r0, #1
	str r0, [r4]
_0225B3A0:
	add r0, r5, #0
	add r0, #0x1c
	mov r2, #2
	bl sub_020182E0
	b _0225B42E
_0225B3AC:
	ldr r0, [r4, #4]
	add r0, r0, #1
	str r0, [r4, #4]
	cmp r0, #4
	blt _0225B42E
	mov r0, #0
	str r0, [r4, #4]
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
_0225B3C0:
	add r0, r5, #0
	add r0, #0x1c
	add r1, sp, #8
	add r2, sp, #4
	add r3, sp, #0
	bl sub_020182B0
	ldr r1, [sp, #8]
	ldr r0, _0225B448 ; =0xFFFB0000
	cmp r1, r0
	blt _0225B3EE
	mov r0, #5
	lsl r0, r0, #0x10
	cmp r1, r0
	bgt _0225B3EE
	mov r2, #3
	ldr r3, [sp, #4]
	lsl r2, r2, #0x10
	cmp r3, r2
	bgt _0225B3EE
	ldr r0, _0225B44C ; =0xFFFD0000
	cmp r3, r0
	bge _0225B3F4
_0225B3EE:
	add sp, #0xc
	mov r0, #1
	pop {r4, r5, r6, r7, pc}
_0225B3F4:
	ldr r0, [r4, #8]
	cmp r0, #5
	bge _0225B40E
	lsr r6, r2, #4
	lsr r2, r2, #4
	add r0, r5, #0
	sub r2, r3, r2
	ldr r3, [sp]
	add r0, #0x1c
	add r1, r1, r6
	bl sub_020182A8
	b _0225B420
_0225B40E:
	lsr r6, r2, #4
	lsr r2, r2, #4
	add r0, r5, #0
	add r2, r3, r2
	ldr r3, [sp]
	add r0, #0x1c
	add r1, r1, r6
	bl sub_020182A8
_0225B420:
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
	cmp r0, #0xa
	blt _0225B42E
	mov r0, #0
	str r0, [r4, #8]
_0225B42E:
	add r0, r5, #0
	add r4, #0xc
	add r0, #0x1c
	add r1, r5, #0
	add r2, r4, #0
	mov r3, #0
	bl ov89_0225ADA4
	mov r0, #0
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0225B444: .word 0x00000AAA
_0225B448: .word 0xFFFB0000
_0225B44C: .word 0xFFFD0000
	thumb_func_end ov89_0225B2E8
