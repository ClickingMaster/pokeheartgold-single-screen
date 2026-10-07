	.include "asm/macros.inc"
	.include "global.inc"

	.extern CARD_InitPulledOutCallback
	.extern CARDi_InitCommon
	.extern CARDi_ReadCard
	.extern OS_WakeupThreadDirect
	.extern cardi_common
	.extern cardi_rom_base
	.extern rom_stat

.text

	arm_func_start CARDi_OnFifoRecv
CARDi_OnFifoRecv: ; 0x038002C4
	stmdb sp!, {r3, lr}
	cmp r0, #0xb
	bne _0380039C
	cmp r2, #0
	beq _0380039C
	ldr r0, _038003A4 ; =cardi_common
	ldr r2, [r0, #8]
	cmp r2, #0
	streq r1, [r0, #4]
	ldr r2, [r0, #4]
	cmp r2, #0xf
	addls pc, pc, r2, lsl #2
	b _03800368
_038002F8: ; jump table
	b _03800338 ; case 0
	b _03800368 ; case 1
	b _0380035C ; case 2
	b _0380035C ; case 3
	b _0380035C ; case 4
	b _0380035C ; case 5
	b _0380035C ; case 6
	b _0380035C ; case 7
	b _0380035C ; case 8
	b _0380035C ; case 9
	b _0380035C ; case 10
	b _0380035C ; case 11
	b _0380035C ; case 12
	b _0380035C ; case 13
	b _0380035C ; case 14
	b _0380035C ; case 15
_03800338:
	ldr r2, [r0, #8]
	cmp r2, #0
	beq _03800368
	cmp r2, #1
	ldreq r2, [r0, #0xfc]
	streq r1, [r0]
	orreq r1, r2, #0x10
	streq r1, [r0, #0xfc]
	b _03800368
_0380035C:
	ldr r1, [r0, #0xfc]
	orr r1, r1, #0x10
	str r1, [r0, #0xfc]
_03800368:
	ldr r1, [r0, #0xfc]
	tst r1, #0x10
	ldreq r1, [r0, #8]
	addeq r1, r1, #1
	streq r1, [r0, #8]
	beq _0380039C
	ldr r1, [r0, #0xfc]
	mov r2, #0
	str r2, [r0, #8]
	tst r1, #4
	ldrne r0, [r0, #0xec]
	addeq r0, r0, #0x48
	bl OS_WakeupThreadDirect
_0380039C:
	ldmia sp!, {r3, lr}
	bx lr
	.align 2, 0
_038003A4: .word cardi_common
	arm_func_end CARDi_OnFifoRecv
