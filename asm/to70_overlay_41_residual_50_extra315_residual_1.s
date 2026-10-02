	.include "asm/macros.inc"
	.include "overlay_41.inc"
	.include "global.inc"

.text
	thumb_func_start ov41_022498E8
ov41_022498E8: ; 0x022498E8
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	str r0, [sp]
	str r1, [sp, #4]
	add r1, r3, #0
	mov r0, #0x28
	strb r0, [r1]
	strb r0, [r1, #1]
	strb r0, [r1, #2]
	strb r0, [r1, #3]
	mov r6, #0
_022498FE:
	mov r2, #0x50
	sub r2, r2, r6
	str r2, [sp, #8]
	lsl r2, r6, #0x18
	lsr r2, r2, #0x18
	mov lr, r2
	ldr r2, [sp, #8]
	mov r0, #0
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	add r5, r0, #0
	mov ip, r2
_02249916:
	add r7, r6, r5
	lsr r3, r7, #0x1f
	lsl r4, r7, #0x1f
	sub r4, r4, r3
	mov r2, #0x1f
	ror r4, r2
	add r2, r3, r4
	lsl r4, r2, #2
	mov r2, #0xf
	lsl r2, r4
	lsl r2, r2, #0x18
	lsr r4, r2, #0x18
	add r3, r7, r3
	ldr r2, [sp]
	asr r3, r3, #1
	ldrsb r2, [r2, r3]
	tst r2, r4
	beq _02249964
	ldrb r2, [r1]
	cmp r2, r6
	ble _02249944
	mov r2, lr
	strb r2, [r1]
_02249944:
	ldrb r3, [r1, #1]
	ldr r2, [sp, #8]
	cmp r3, r2
	ble _02249950
	mov r2, ip
	strb r2, [r1, #1]
_02249950:
	ldrb r2, [r1, #2]
	cmp r2, r0
	ble _02249958
	strb r0, [r1, #2]
_02249958:
	mov r2, #0x50
	sub r3, r2, r0
	ldrb r2, [r1, #3]
	cmp r2, r3
	ble _02249964
	strb r3, [r1, #3]
_02249964:
	ldr r2, [sp, #4]
	add r0, r0, #1
	add r5, r5, r2
	cmp r0, #0x50
	blt _02249916
	add r6, r6, #1
	cmp r6, #0x50
	blt _022498FE
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	thumb_func_end ov41_022498E8

	thumb_func_start ov41_02249978
ov41_02249978: ; 0x02249978
	push {r3, r4}
	ldr r4, [sp, #8]
	sub r4, r2, r4
	bmi _02249982
	b _02249984
_02249982:
	mov r4, #0
_02249984:
	strb r4, [r0]
	ldr r4, [sp, #8]
	add r2, r2, r4
	cmp r2, #0xbf
	bgt _02249990
	b _02249992
_02249990:
	mov r2, #0xbf
_02249992:
	strb r2, [r0, #1]
	sub r2, r1, r3
	bmi _0224999A
	b _0224999C
_0224999A:
	mov r2, #0
_0224999C:
	add r1, r1, r3
	strb r2, [r0, #2]
	cmp r1, #0xff
	bgt _022499AA
	strb r1, [r0, #3]
	pop {r3, r4}
	bx lr
_022499AA:
	mov r1, #0xff
	strb r1, [r0, #3]
	pop {r3, r4}
	bx lr
	.balign 4, 0
	thumb_func_end ov41_02249978
