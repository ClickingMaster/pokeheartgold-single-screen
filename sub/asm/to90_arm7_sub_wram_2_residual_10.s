	.include "asm/macros.inc"
	.include "global.inc"

	.extern OS_IRQTable
	.extern OS_IrqHandler_ThreadSwitch
	.extern OSi_ThreadInfo
	.extern micIntrInfo
	.extern micIntrPrio

.text

	arm_func_start MIC_IrqHandler
MIC_IrqHandler: ; 0x03802DB4
	mov ip, #0x4000000
	add r1, ip, #0x208
	ldrh r0, [r1]
	tst r0, r0
	bxeq lr
	ldr r3, [ip, #0x210]
	ldr r1, [ip, #0x214]
	ands r2, r1, r3
	bxeq lr
	ldr r0, _03802F20 ; =0x01DF3FFF
	tst r2, r0
	streq r2, [ip, #0x214]
	bxeq lr
	stmdb sp!, {lr}
	mrs r0, spsr
	stmdb sp!, {r0}
	stmdb sp, {sp, lr} ^
	sub sp, sp, #8
	mov r0, #0x9f
	msr cpsr_c, r0
	ldr r1, _03802F24 ; =OSi_ThreadInfo
	ldrh r0, [r1, #2]
	add r0, r0, #1
	strh r0, [r1, #2]
	ldr r1, _03802F28 ; =micIntrInfo
	cmp r0, #1
	moveq r0, sp
	ldreq sp, [r1, #4]
	streq r0, [r1, #4]
	stmdb sp!, {r3}
	ldr r1, _03802F2C ; =micIntrPrio
	ldr r0, [r1]
	tst r0, r2
	strne r0, [ip, #0x214]
	ldrne r0, [r1, #4]
	ldrne r3, _03802F30 ; =OS_IRQTable
	ldrne r0, [r3, r0, lsl #2]
	bne _03802E90
	mov r3, #1
_03802E50:
	ldr r0, [r1, r3, lsl #3]
	tst r0, r2
	addeq r3, r3, #1
	beq _03802E50
	str r0, [ip, #0x214]
	add r0, r1, r3, lsl #3
	ldr r2, [r0, #4]
	ldr r3, _03802F30 ; =OS_IRQTable
	ldr r0, [r3, r2, lsl #2]
	ldr r2, _03802F24 ; =OSi_ThreadInfo
	ldrh r3, [r2, #2]
	cmp r3, #1
	ldreq r2, [r1]
	streq r2, [ip, #0x210]
	moveq r2, #0x1f
	msreq cpsr_c, r2
_03802E90:
	ldr r1, [ip, #0x210]
	stmdb sp!, {r1}
	add lr, pc, #0x0 ; =sub_03802EA0
	bx r0
	mov r0, #0x9f
	msr cpsr_c, r0
	mov ip, #0x4000000
	ldmia sp!, {r0}
	ldr r1, [ip, #0x210]
	eor r2, r0, r1
	and r1, r2, r1
	and r0, r2, r0
	ldmia sp!, {r3}
	orr r3, r3, r1
	bic r3, r3, r0
	str r3, [ip, #0x210]
	ldr r2, _03802F24 ; =OSi_ThreadInfo
	ldr r3, _03802F28 ; =micIntrInfo
	ldrh r0, [r2, #2]
	subs r1, r0, #1
	strh r1, [r2, #2]
	moveq r0, sp
	ldreq sp, [r3, #4]
	streq r0, [r3, #4]
	mov r0, #0x92
	msr cpsr_c, r0
	ldmia sp, {sp, lr} ^
	mov r0, r0
	add sp, sp, #8
	ldmia sp!, {r0}
	msr spsr_fc, r0
	tst r1, r1
	ldreq r0, _03802F34 ; =OS_IrqHandler_ThreadSwitch
	addeq lr, pc, #0x0 ; =_03802F1C
	bxeq r0
_03802F1C:
	ldmia sp!, {pc}
	.align 2, 0
_03802F20: .word 0x01DF3FFF
_03802F24: .word OSi_ThreadInfo
_03802F28: .word micIntrInfo
_03802F2C: .word micIntrPrio
_03802F30: .word OS_IRQTable
_03802F34: .word OS_IrqHandler_ThreadSwitch
	arm_func_end MIC_IrqHandler
