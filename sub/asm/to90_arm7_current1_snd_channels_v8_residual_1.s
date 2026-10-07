	.include "asm/macros.inc"
	.include "global.inc"

	.extern CalcSurroundDecay
	.extern sMasterPan
	.extern sOrgPan
	.extern sOrgVolume
	.extern sSurroundDecay

.text

	arm_func_start SND_SetupChannelPsg
SND_SetupChannelPsg: ; 0x037FB864
	stmdb sp!, {r3, r4, r5, r6, r7, lr}
	ldr r4, _037FB8FC ; =sMasterPan
	mov r7, r1
	ldr ip, [r4]
	ldr r1, _037FB900 ; =sSurroundDecay
	mov r6, r3
	ldr r3, _037FB904 ; =sOrgVolume
	ldr r5, [sp, #0x1c]
	ldr r4, _037FB908 ; =sOrgPan
	ldr r1, [r1]
	strb r5, [r4, r0]
	cmp ip, #0
	movge r5, ip
	mov r4, r0, lsl #4
	strb r2, [r3, r0]
	cmp r1, #0
	ble _037FB8C8
	ldr r1, _037FB90C ; =0x0000FFF5
	mov r3, #1
	tst r1, r3, lsl r0
	beq _037FB8C8
	mov r0, r2
	mov r1, r5
	bl CalcSurroundDecay
	mov r2, r0
_037FB8C8:
	mov r0, r7, lsl #0x18
	orr r0, r0, #0x60000000
	orr r0, r0, r5, lsl #16
	ldr r1, [sp, #0x18]
	orr r3, r0, r6, lsl #8
	add r0, r4, #0x4000000
	orr r2, r2, r3
	str r2, [r0, #0x400]
	rsb r1, r1, #0x10000
	add r0, r0, #0x400
	strh r1, [r0, #8]
	ldmia sp!, {r3, r4, r5, r6, r7, lr}
	bx lr
	.align 2, 0
_037FB8FC: .word sMasterPan
_037FB900: .word sSurroundDecay
_037FB904: .word sOrgVolume
_037FB908: .word sOrgPan
_037FB90C: .word 0x0000FFF5
	arm_func_end SND_SetupChannelPsg
