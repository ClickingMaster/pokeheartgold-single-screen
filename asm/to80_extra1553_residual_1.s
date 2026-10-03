	.extern ov80_0223AE14
	.extern ov80_0223AE6C
	.extern ov80_0223DBE0
	.extern ov80_0223DBEC
	.include "asm/macros.inc"
	.include "overlay_80_0223AC24.inc"
	.include "global.inc"

.text
	thumb_func_start ov80_0223AFC4
ov80_0223AFC4: ; 0x0223AFC4
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	add r5, r0, #0
	ldr r0, _0223B1C8 ; =0x0000018D
	ldrb r1, [r5, r0]
	cmp r1, #0
	bne _0223AFD8
	add sp, #0x1c
	mov r0, #1
	pop {r4, r5, r6, r7, pc}
_0223AFD8:
	add r1, r0, #0
	sub r1, #9
	ldrb r1, [r5, r1]
	cmp r1, #0x60
	bhs _0223AFF2
	sub r1, r0, #2
	ldrsb r1, [r5, r1]
	sub r2, r1, #1
	sub r1, r0, #2
	strb r2, [r5, r1]
	ldrsb r1, [r5, r1]
	cmp r1, #0
	ble _0223AFF4
_0223AFF2:
	b _0223B16E
_0223AFF4:
	sub r1, r0, #3
	ldrb r2, [r5, r1]
	sub r1, r0, #2
	ldr r3, _0223B1CC ; =ov80_0223DBEC
	strb r2, [r5, r1]
	sub r1, r0, #6
	ldrb r1, [r5, r1]
	lsl r4, r1, #4
	add r1, r0, #0
	sub r1, #8
	ldrb r2, [r5, r1]
	add r1, r3, r4
	sub r3, r0, #7
	ldrb r3, [r5, r3]
	sub r4, r0, #4
	ldrb r1, [r2, r1]
	lsl r3, r3, #4
	add r3, #8
	str r3, [sp]
	ldrb r4, [r5, r4]
	lsl r1, r1, #5
	add r2, r1, #0
	str r4, [sp, #4]
	ldr r4, [r5]
	add r2, #0x20
	str r4, [sp, #8]
	mov r4, #0x20
	str r4, [sp, #0xc]
	mov r4, #0x10
	str r4, [sp, #0x10]
	sub r4, r0, #1
	ldrb r4, [r5, r4]
	sub r0, #9
	str r4, [sp, #0x14]
	ldrb r0, [r5, r0]
	lsl r0, r0, #2
	add r0, r5, r0
	ldr r0, [r0, #4]
	bl ov80_0223AE14
	ldr r0, _0223B1D0 ; =0x00000187
	mov r1, #1
	ldrb r2, [r5, r0]
	ldr r3, _0223B1CC ; =ov80_0223DBEC
	eor r1, r2
	lsl r4, r1, #4
	sub r1, r0, #2
	ldrb r2, [r5, r1]
	add r1, r3, r4
	sub r3, r0, #1
	ldrb r4, [r5, r3]
	mov r3, #5
	ldrb r1, [r2, r1]
	sub r3, r3, r4
	lsl r3, r3, #4
	add r3, #8
	str r3, [sp]
	add r4, r0, #2
	ldrb r4, [r5, r4]
	lsl r1, r1, #5
	add r2, r1, #0
	str r4, [sp, #4]
	ldr r4, [r5]
	add r2, #0x20
	str r4, [sp, #8]
	mov r4, #0x20
	str r4, [sp, #0xc]
	mov r4, #0x10
	str r4, [sp, #0x10]
	add r4, r0, #5
	ldrb r4, [r5, r4]
	sub r0, r0, #3
	str r4, [sp, #0x14]
	ldrb r0, [r5, r0]
	add r0, r0, #1
	lsl r0, r0, #2
	add r0, r5, r0
	ldr r0, [r0, #4]
	bl ov80_0223AE14
	ldr r0, _0223B1D0 ; =0x00000187
	ldr r3, _0223B1CC ; =ov80_0223DBEC
	ldrb r1, [r5, r0]
	lsl r4, r1, #4
	sub r1, r0, #2
	ldrb r2, [r5, r1]
	add r1, r3, r4
	ldrb r1, [r2, r1]
	sub r2, r0, #1
	ldrb r2, [r5, r2]
	lsl r1, r1, #5
	add r2, r2, #6
	lsl r3, r2, #4
	add r3, #8
	str r3, [sp]
	add r2, r0, #2
	ldrb r2, [r5, r2]
	str r2, [sp, #4]
	ldr r2, [r5]
	str r2, [sp, #8]
	mov r2, #0x20
	str r2, [sp, #0xc]
	mov r2, #0x10
	str r2, [sp, #0x10]
	add r2, r0, #5
	ldrb r2, [r5, r2]
	sub r0, r0, #3
	str r2, [sp, #0x14]
	ldrb r0, [r5, r0]
	add r2, r1, #0
	add r2, #0x20
	add r0, r0, #2
	lsl r0, r0, #2
	add r0, r5, r0
	ldr r0, [r0, #4]
	bl ov80_0223AE14
	ldr r0, _0223B1D0 ; =0x00000187
	mov r1, #1
	ldrb r2, [r5, r0]
	ldr r3, _0223B1CC ; =ov80_0223DBEC
	eor r1, r2
	lsl r4, r1, #4
	sub r1, r0, #2
	ldrb r2, [r5, r1]
	add r1, r3, r4
	ldrb r1, [r2, r1]
	sub r2, r0, #1
	ldrb r3, [r5, r2]
	mov r2, #0xb
	lsl r1, r1, #5
	sub r2, r2, r3
	lsl r3, r2, #4
	add r3, #8
	str r3, [sp]
	add r2, r0, #2
	ldrb r2, [r5, r2]
	str r2, [sp, #4]
	ldr r2, [r5]
	str r2, [sp, #8]
	mov r2, #0x20
	str r2, [sp, #0xc]
	mov r2, #0x10
	str r2, [sp, #0x10]
	add r2, r0, #5
	ldrb r2, [r5, r2]
	sub r0, r0, #3
	str r2, [sp, #0x14]
	ldrb r0, [r5, r0]
	add r2, r1, #0
	add r2, #0x20
	add r0, r0, #3
	lsl r0, r0, #2
	add r0, r5, r0
	ldr r0, [r0, #4]
	bl ov80_0223AE14
	mov r1, #0x61
	lsl r1, r1, #2
	ldrb r0, [r5, r1]
	add r0, r0, #4
	strb r0, [r5, r1]
	add r0, r1, #1
	ldrb r0, [r5, r0]
	add r2, r0, #1
	add r0, r1, #1
	strb r2, [r5, r0]
	ldrb r0, [r5, r0]
	lsr r3, r0, #0x1f
	lsl r2, r0, #0x1d
	sub r2, r2, r3
	mov r0, #0x1d
	ror r2, r0
	add r0, r3, r2
	bne _0223B16E
	add r0, r1, #3
	ldrb r2, [r5, r0]
	mov r0, #1
	eor r2, r0
	add r0, r1, #3
	strb r2, [r5, r0]
	add r0, r1, #2
	ldrb r0, [r5, r0]
	add r2, r0, #1
	add r0, r1, #2
	strb r2, [r5, r0]
	mov r2, #0
	add r0, r1, #1
	strb r2, [r5, r0]
_0223B16E:
	mov r0, #0x62
	lsl r0, r0, #2
	ldrb r4, [r5, r0]
	sub r0, r0, #4
	ldrb r0, [r5, r0]
	cmp r4, r0
	bge _0223B1A4
	lsl r0, r4, #2
	mov r7, #0x62
	add r6, r5, r0
	lsl r7, r7, #2
_0223B184:
	ldr r0, [r6, #4]
	bl ov80_0223AE6C
	str r0, [sp, #0x18]
	cmp r0, #1
	bne _0223B196
	ldrb r0, [r5, r7]
	add r0, r0, #1
	strb r0, [r5, r7]
_0223B196:
	mov r0, #0x61
	lsl r0, r0, #2
	ldrb r0, [r5, r0]
	add r4, r4, #1
	add r6, r6, #4
	cmp r4, r0
	blt _0223B184
_0223B1A4:
	mov r1, #0x62
	lsl r1, r1, #2
	ldrb r0, [r5, r1]
	cmp r0, #0x60
	blo _0223B1C0
	ldr r0, [sp, #0x18]
	cmp r0, #1
	bne _0223B1C0
	mov r2, #0
	add r0, r1, #5
	strb r2, [r5, r0]
	add sp, #0x1c
	mov r0, #1
	pop {r4, r5, r6, r7, pc}
_0223B1C0:
	mov r0, #0
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	nop
_0223B1C8: .word 0x0000018D
_0223B1CC: .word ov80_0223DBEC
_0223B1D0: .word 0x00000187
	thumb_func_end ov80_0223AFC4
