	.include "asm/macros.inc"
	.include "global.inc"

	.extern MicSetTimerValue
	.extern OS_DisableInterrupts
	.extern OS_RestoreInterrupts
	.extern SPIi_ReturnResult
	.extern SPIi_SetEntry
	.extern micw

.text

	arm_func_start MIC_Init
MIC_Init: ; 0x038022E8
	ldr r0, _03802324 ; =micw
	mov r3, #0
	str r3, [r0, #0x20]
	ldr r0, _03802328 ; =micw
	mov r2, r3
_038022FC:
	mov r1, r3, lsl #1
	add r3, r3, #1
	strh r2, [r0, r1]
	cmp r3, #0x10
	blt _038022FC
	ldr r1, _0380232C ; =0x0400010E
	ldrh r0, [r1]
	bic r0, r0, #0x80
	strh r0, [r1]
	bx lr
	.align 2, 0
_03802324: .word micw
_03802328: .word micw
_0380232C: .word 0x0400010E
	arm_func_end MIC_Init

	arm_func_start MIC_AnalyzeCommand
MIC_AnalyzeCommand: ; 0x03802330
	stmdb sp!, {r4, lr}
	tst r0, #0x2000000
	beq _0380235C
	ldr r1, _038025E8 ; =micw
	mov r4, #0
	mov r3, r4
_03802348:
	mov r2, r4, lsl #1
	add r4, r4, #1
	strh r3, [r1, r2]
	cmp r4, #0x10
	blt _03802348
_0380235C:
	ldr r1, _038025E8 ; =micw
	and r2, r0, #0xf0000
	mov r2, r2, lsr #0x10
	mov r2, r2, lsl #1
	strh r0, [r1, r2]
	tst r0, #0x1000000
	beq _038025E0
	ldr r1, _038025EC ; =micw
	ldrh r2, [r1]
	and r0, r2, #0xff00
	mov r0, r0, lsl #8
	mov r4, r0, lsr #0x10
	sub r0, r4, #0x40
	cmp r0, #3
	addls pc, pc, r0, lsl #2
	b _038025D4
_0380239C: ; jump table
	b _038023AC ; case 0
	b _038023E8 ; case 1
	b _038024EC ; case 2
	b _03802550 ; case 3
_038023AC:
	mov r1, r4
	and r3, r2, #0xff
	mov r0, #2
	mov r2, #1
	bl SPIi_SetEntry
	cmp r0, #0
	bne _038023D4
	mov r0, r4
	mov r1, #4
	bl SPIi_ReturnResult
_038023D4:
	ldr r0, _038025F0 ; =0x027FFF94
	mov r1, #0
	strh r1, [r0]
	str r1, [r0, #-4]
	b _038025E0
_038023E8:
	ldr r0, [r1, #0x20]
	cmp r0, #0
	beq _03802404
	mov r0, r4
	mov r1, #3
	bl SPIi_ReturnResult
	b _038025E0
_03802404:
	and r0, r2, #0xff
	strh r0, [r1, #0x24]
	ldrh r2, [r1, #2]
	ldrh r0, [r1, #4]
	orr r3, r0, r2, lsl #16
	cmp r3, #0x2000000
	blo _03802428
	cmp r3, #0x2400000
	blo _03802438
_03802428:
	mov r0, r4
	mov r1, #2
	bl SPIi_ReturnResult
	b _038025E0
_03802438:
	str r3, [r1, #0x28]
	ldrh r2, [r1, #6]
	ldrh r0, [r1, #8]
	orr r2, r0, r2, lsl #16
	add r0, r3, r2
	cmp r0, #0x2400000
	bls _03802464
	mov r0, r4
	mov r1, #2
	bl SPIi_ReturnResult
	b _038025E0
_03802464:
	str r2, [r1, #0x30]
	ldrh r2, [r1, #0xa]
	ldrh r0, [r1, #0xc]
	orr r0, r0, r2, lsl #16
	bl MicSetTimerValue
	cmp r0, #0
	bne _03802490
	mov r0, r4
	mov r1, #2
	bl SPIi_ReturnResult
	b _038025E0
_03802490:
	ldr r3, _038025EC ; =micw
	mov r2, #0
	str r2, [r3, #0x2c]
	ldrh r0, [r3, #0x24]
	mov r1, r4
	and ip, r0, #7
	mov r0, #2
	strh ip, [r3, #0x26]
	bl SPIi_SetEntry
	cmp r0, #0
	bne _038024CC
	mov r0, r4
	mov r1, #4
	bl SPIi_ReturnResult
	b _038025E0
_038024CC:
	ldr r1, _038025F0 ; =0x027FFF94
	mov r2, #0
	strh r2, [r1]
	ldr r0, _038025EC ; =micw
	str r2, [r1, #-4]
	mov r1, #1
	str r1, [r0, #0x20]
	b _038025E0
_038024EC:
	ldr r0, [r1, #0x20]
	cmp r0, #2
	beq _03802508
	mov r0, r4
	mov r1, #3
	bl SPIi_ReturnResult
	b _038025E0
_03802508:
	mov r1, r4
	mov r0, #2
	mov r2, #0
	bl SPIi_SetEntry
	cmp r0, #0
	bne _03802530
	mov r0, r4
	mov r1, #4
	bl SPIi_ReturnResult
	b _038025E0
_03802530:
	ldr r0, _038025EC ; =micw
	mov r2, #3
	ldr r1, _038025F4 ; =0x0400010E
	str r2, [r0, #0x20]
	ldrh r0, [r1]
	bic r0, r0, #0x80
	strh r0, [r1]
	b _038025E0
_03802550:
	ldr r0, [r1, #0x20]
	cmp r0, #2
	beq _0380256C
	mov r0, r4
	mov r1, #3
	bl SPIi_ReturnResult
	b _038025E0
_0380256C:
	ldrh r2, [r1, #2]
	ldrh r0, [r1, #4]
	orr r0, r0, r2, lsl #16
	bl MicSetTimerValue
	cmp r0, #0
	bne _03802594
	mov r0, r4
	mov r1, #2
	bl SPIi_ReturnResult
	b _038025E0
_03802594:
	bl OS_DisableInterrupts
	ldr r3, _038025F4 ; =0x0400010E
	ldr r1, _038025EC ; =micw
	ldrh r2, [r3]
	bic r2, r2, #0x80
	strh r2, [r3]
	ldrh r2, [r1, #0x34]
	strh r2, [r3, #-2]
	ldrh r1, [r1, #0x36]
	orr r1, r1, #0xc0
	strh r1, [r3]
	bl OS_RestoreInterrupts
	mov r0, r4
	mov r1, #0
	bl SPIi_ReturnResult
	b _038025E0
_038025D4:
	mov r0, r4
	mov r1, #1
	bl SPIi_ReturnResult
_038025E0:
	ldmia sp!, {r4, lr}
	bx lr
	.align 2, 0
_038025E8: .word micw
_038025EC: .word micw
_038025F0: .word 0x027FFF94
_038025F4: .word 0x0400010E
	arm_func_end MIC_AnalyzeCommand
