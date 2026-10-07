	.include "asm/macros.inc"
	.include "global.inc"


.section .wram,4,1,5

	arm_func_start WMSP_GetAllowedChannel
WMSP_GetAllowedChannel: ; 0x03806968
	stmdb sp!, {r4, r5, r6, lr}
	ldr r1, _03806A70 ; =0x00001FFF
	and r0, r0, r1
	mov r0, r0, lsl #0x10
	movs r2, r0, lsr #0x10
	moveq r0, #0
	beq _03806A68
	mov r1, #0
	mov r0, #1
	b _0380699C
_03806990:
	tst r2, r0, lsl r1
	bne _038069A4
	add r1, r1, #1
_0380699C:
	cmp r1, #0x10
	blt _03806990
_038069A4:
	mov r0, #0xf
	mov r3, #1
	b _038069BC
_038069B0:
	tst r2, r3, lsl r0
	bne _038069C4
	sub r0, r0, #1
_038069BC:
	cmp r0, #0
	bne _038069B0
_038069C4:
	sub r6, r0, r1
	cmp r6, #5
	movlt r0, #1
	movlt r0, r0, lsl r1
	movlt r0, r0, lsl #0x10
	movlt r0, r0, lsr #0x10
	blt _03806A68
	add r3, r0, r1
	add r3, r3, r3, lsr #31
	mov r4, r3, asr #1
	mov r5, #0
	mov lr, #1
	b _03806A1C
_038069F8:
	mov ip, r5, lsr #0x1f
	rsb r3, ip, r5, lsl #31
	add r3, ip, r3, ror #31
	mov r3, r3, lsl #1
	sub r3, r3, #1
	mla r4, r5, r3, r4
	tst r2, lr, lsl r4
	bne _03806A24
	add r5, r5, #1
_03806A1C:
	cmp r5, r6
	blt _038069F8
_03806A24:
	sub r2, r0, r4
	cmp r2, #5
	subge r2, r4, r1
	cmpge r2, #5
	bge _03806A50
	mov r2, #1
	mov r0, r2, lsl r0
	orr r0, r0, r2, lsl r1
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	b _03806A68
_03806A50:
	mov r3, #1
	mov r2, r3, lsl r4
	orr r0, r2, r3, lsl r0
	orr r0, r0, r3, lsl r1
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
_03806A68:
	ldmia sp!, {r4, r5, r6, lr}
	bx lr
	.align 2, 0
_03806A70: .word 0x00001FFF
	arm_func_end WMSP_GetAllowedChannel
