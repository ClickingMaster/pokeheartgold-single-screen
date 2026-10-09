	.include "asm/macros.inc"
	.include "global.inc"

	.extern MIC_ExecSampling12
	.extern MIC_ExecSampling8
	.extern SPI_DummyWaitReceive
	.extern SPIi_CheckEntry
	.extern SPIi_CheckException
	.extern SPIi_ReturnResult
	.extern SPIi_SetEntry
	.extern micw

.text

	arm_func_start MicTimerHandler
MicTimerHandler: ; 0x038028F8
	stmdb sp!, {r4, r5, r6, r7, lr}
	ldr r4, _03802A50 ; =micw
	ldrh r5, [r4, #0x26]
	and r0, r5, #4
	cmp r0, #4
	ldrh r6, [r4, #0x38]
	ldrneh r7, [r4, #0x3a]
	ldreq r7, _03802A54 ; =0x0000FFFF
	bl SPIi_CheckEntry
	cmp r0, #0
	bne _03802964
	mov r0, #2
	bl SPIi_CheckException
	cmp r0, #0
	beq _03802964
	and r0, r5, #1
	cmp r0, #1
	bne _03802954
	bl MIC_ExecSampling12
	tst r5, #2
	moveq r7, r0
	eorne r7, r0, #0x8000
	b _03802964
_03802954:
	bl MIC_ExecSampling8
	tst r5, #2
	moveq r7, r0
	eorne r7, r0, #0x80
_03802964:
	and r0, r5, #1
	ldr r3, _03802A58 ; =0x027FFC00
	ldr r1, [r4, #0x2c]
	cmp r0, #1
	bne _03802994
	ldr r2, [r4, #0x28]
	strh r7, [r2, r1]!
	str r2, [r3, #0x390]
	add r3, r3, #0x394
	strh r7, [r3]
	add r1, r1, #2
	b _038029CC
_03802994:
	and r7, r7, #0xff
	tst r1, #1
	bne _038029AC
	mov r6, r7
	add r1, r1, #1
	b _038029CC
_038029AC:
	orr r0, r6, r7, lsl #8
	ldr r2, [r4, #0x28]
	sub r1, r1, #1
	strh r0, [r2, r1]!
	str r2, [r3, #0x390]
	add r3, r3, #0x394
	strh r0, [r3]
	add r1, r1, #2
_038029CC:
	strh r6, [r4, #0x38]
	strh r7, [r4, #0x3a]
	ldr r0, [r4, #0x30]
	cmp r1, r0
	movhs r1, #0
	str r1, [r4, #0x2c]
	blo _03802A48
	ldrh r0, [r4, #0x24]
	and r0, r0, #0x10
	cmp r0, #0x10
	bne _03802A08
	mov r0, #0x51
	mov r1, #0
	bl SPIi_ReturnResult
	b _03802A48
_03802A08:
	mov r0, #2
	mov r1, #0x42
	mov r2, #0
	bl SPIi_SetEntry
	cmp r0, #0
	bne _03802A30
	mov r0, #0x51
	mov r1, #4
	bl SPIi_ReturnResult
	b _03802A48
_03802A30:
	mov r0, #4
	str r0, [r4, #0x20]
	ldr r1, _03802A5C ; =0x0400010E
	ldrh r0, [r1]
	bic r0, r0, #0x80
	strh r0, [r1]
_03802A48:
	ldmia sp!, {r4, r5, r6, r7, lr}
	bx lr
	.align 2, 0
_03802A50: .word micw
_03802A54: .word 0x0000FFFF
_03802A58: .word 0x027FFC00
_03802A5C: .word 0x0400010E
	arm_func_end MicTimerHandler
