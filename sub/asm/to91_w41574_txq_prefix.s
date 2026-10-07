	.extern CopyTxFrmToMacBuf
	.include "asm/macros.inc"
	.include "global.inc"

	.extern CAM_DecFrameCount
	.extern CAM_GetFrameCount
	.extern CAM_GetStaState
	.extern CAM_IsActive
	.extern CheckFrameTimeout
	.extern DMA_WepWriteHeaderData
	.extern DMA_Write
	.extern DMA_WriteHeaderData
	.extern GetHeapBufNextAdrs
	.extern IssueMaDataConfirm
	.extern WSetPowerState
	.extern WUpdateCounter
	.extern __VENEER_OS_DisableIrqMask
	.extern __VENEER_OS_EnableIrqMask

.text

	arm_func_start TxqPri
TxqPri: ; 0x06012C88
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0xc
	ldr r1, _06012E9C ; =0x0380FFF4
	mov sl, r0
	ldr r2, [r1]
	mov r0, #0xc
	add r1, r2, #0x194
	mla fp, sl, r0, r1
	add r0, r2, #0x2c
	ldrh r1, [fp, #8]
	add r5, r0, #0x400
	mov r0, #0x14
	mla r6, sl, r0, r5
	cmp r1, #0
	add r4, r2, #0x344
	beq _06012E90
	mov r0, #0x1000000
	bl __VENEER_OS_DisableIrqMask
	ldrh r1, [r6]
	str r0, [sp]
	cmp r1, #0
	beq _06012CE8
	bl __VENEER_OS_EnableIrqMask
	b _06012E90
_06012CE8:
	ldr r7, [fp]
	mvn r0, #0
	str r0, [sp, #8]
_06012CF4:
	ldr r0, [sp, #8]
	cmp r7, r0
	bne _06012D0C
	ldr r0, [sp]
	bl __VENEER_OS_EnableIrqMask
	b _06012E90
_06012D0C:
	mov r0, r7
	str r7, [sp, #4]
	bl GetHeapBufNextAdrs
	mov r1, r7
	add r8, r1, #0x10
	mov r7, r0
	ldrh sb, [r8, #2]
	mov r0, r8
	bl CheckFrameTimeout
	cmp r0, #0
	beq _06012D70
	ldrh r1, [r5, #0xae]
	mov r0, r8
	add r1, r1, #1
	strh r1, [r5, #0xae]
	mov r1, #2
	strh r1, [r8, #8]
	ldrh r2, [r6, #4]
	mov r1, #0
	add r2, r2, #1
	strh r2, [r6, #4]
	ldr r2, [r6, #0x10]
	mov lr, pc
	bx r2
	b _06012CF4
_06012D70:
	cmp sl, #0
	beq _06012D90
	cmp sl, #1
	bne _06012DD0
	mov r0, sb
	bl CAM_GetStaState
	cmp r0, #0x40
	bne _06012DD0
_06012D90:
	mov r0, sb
	bl CAM_IsActive
	cmp r0, #0
	beq _06012CF4
	mov r0, sb
	bl CAM_GetStaState
	cmp r0, #0x40
	beq _06012DD0
	mov r2, #2
	mov r0, fp
	sub r1, r8, #0x10
	strh r2, [r8, #8]
	bl IssueMaDataConfirm
	mov r0, r8
	bl CAM_DecFrameCount
	b _06012CF4
_06012DD0:
	mov r0, #1
	strh r0, [r6]
	ldrh r0, [r6, #2]
	add r0, r0, #1
	strh r0, [r6, #2]
	str r8, [r6, #0xc]
	ldrh r0, [r4, #0x10]
	ldr r5, [r6, #8]
	cmp r0, #0
	bne _06012E00
	mov r0, #2
	bl WSetPowerState
_06012E00:
	ldr r1, [sp, #4]
	mov r0, r5
	bl CopyTxFrmToMacBuf
	ldrh r0, [r4, #0xc]
	cmp r0, #1
	bne _06012E30
	mov r0, sb
	bl CAM_GetFrameCount
	cmp r0, #1
	ldrhih r0, [r5, #0xc]
	orrhi r0, r0, #0x2000
	strhih r0, [r5, #0xc]
_06012E30:
	ldr r1, _06012EA0 ; =0x00003FFF
	ldr r0, _06012EA4 ; =0x048080A0
	ldrh r4, [r8, #0x14]
	and r2, r5, r1
	and r1, r4, #0xc
	cmp r1, #4
	mov r1, r2, lsr #1
	add r3, r0, sl, lsl #2
	moveq r0, r1, lsl #0x10
	moveq r0, r0, lsr #0x10
	orreq r0, r0, #0xa000
	streqh r0, [r3]
	beq _06012E88
	and r0, r4, #0xfc
	cmp r0, #0x50
	mov r0, r1, lsl #0x10
	moveq r0, r0, lsr #0x10
	orreq r0, r0, #0x9000
	streqh r0, [r3]
	movne r0, r0, lsr #0x10
	orrne r0, r0, #0x8000
	strneh r0, [r3]
_06012E88:
	ldr r0, [sp]
	bl __VENEER_OS_EnableIrqMask
_06012E90:
	add sp, sp, #0xc
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_06012E9C: .word 0x0380FFF4
_06012EA0: .word 0x00003FFF
_06012EA4: .word 0x048080A0
	arm_func_end TxqPri
