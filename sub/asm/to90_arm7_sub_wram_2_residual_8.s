	.include "asm/macros.inc"
	.include "global.inc"

	.extern MIC_ExecSampling12
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

	arm_func_start MIC_ExecSampling8
MIC_ExecSampling8: ; 0x03802A60
	stmdb sp!, {r4, lr}
	ldr r2, _03802B6C ; =0x040001C0
_03802A68:
	ldrh r0, [r2]
	tst r0, #0x80
	bne _03802A68
	ldr r1, _03802B70 ; =0x00008A01
	ldr r0, _03802B74 ; =0x040001C2
	strh r1, [r2]
	mov r1, #0xec
	strh r1, [r0]
	sub r1, r0, #2
_03802A8C:
	ldrh r0, [r1]
	tst r0, #0x80
	bne _03802A8C
	bl SPI_DummyWaitReceive
	ldr r2, _03802B78 ; =0x00008201
	ldr r1, _03802B6C ; =0x040001C0
	mov r4, r0, lsl #0x18
	strh r2, [r1]
	bl SPI_DummyWaitReceive
	ldr r2, _03802B7C ; =micw + 0x3C
	ldr r1, _03802B80 ; =0x00007F80
	orr r0, r0, r4, lsr #16
	and r1, r0, r1
	ldr r0, [r2, #0x10]
	mov r1, r1, lsl #9
	add r3, r0, r1, lsr #16
	str r3, [r2, #0x10]
	ldr r0, [r2, #0xc]
	add r0, r0, #1
	str r0, [r2, #0xc]
	cmp r0, #0x1000
	blo _03802B3C
	ldrsb r4, [r2]
	mov r0, r3, lsr #0xc
	sub r0, r0, #0x80
	mov r0, r0, lsl #0x18
	cmp r4, r0, asr #24
	mov r0, r0, asr #0x18
	cmplt r4, #0xc
	addlt r0, r4, #1
	strltb r0, [r2]
	blt _03802B2C
	cmp r0, r4
	bge _03802B2C
	mvn r0, #0xb
	cmp r4, r0
	ldrgt r0, _03802B7C ; =micw + 0x3C
	ldrgtsb r2, [r0]
	subgt r2, r2, #1
	strgtb r2, [r0]
_03802B2C:
	ldr r0, _03802B7C ; =micw + 0x3C
	mov r2, #0
	str r2, [r0, #0xc]
	str r2, [r0, #0x10]
_03802B3C:
	ldr r0, _03802B7C ; =micw + 0x3C
	ldrsb r0, [r0]
	rsb r0, r0, r1, lsr #16
	cmp r0, #0xff
	movgt r0, #0xff
	bgt _03802B5C
	cmp r0, #0
	movlt r0, #0
_03802B5C:
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	ldmia sp!, {r4, lr}
	bx lr
	.align 2, 0
_03802B6C: .word 0x040001C0
_03802B70: .word 0x00008A01
_03802B74: .word 0x040001C2
_03802B78: .word 0x00008201
_03802B7C: .word micw + 0x3C
_03802B80: .word 0x00007F80
	arm_func_end MIC_ExecSampling8
