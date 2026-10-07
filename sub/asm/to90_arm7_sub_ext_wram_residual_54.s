	.include "asm/macros.inc"
	.include "global.inc"

	.extern AllocateHeapBuf
	.extern DMA_Write
	.extern MP_ADRS
	.extern ReleaseHeapBuf
	.extern WSetMacAdrs3
	.extern WUpdateCounter
	.extern __VENEER_OS_DisableInterrupts
	.extern __VENEER_OS_RestoreInterrupts
	.extern __VENEER__u32_div_f

.text

	arm_func_start MA_MpReqCmd
MA_MpReqCmd: ; 0x060126F0
	stmdb sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #8
	ldr r2, _06012B7C ; =0x0380FFF4
	mov sl, r0
	ldr r4, [r2]
	mov sb, #1
	add r0, r4, #0x600
	ldrh r0, [r0, #0x90]
	add r7, r4, #0x31c
	strh sb, [r1, #2]
	ldrh r1, [r7, #0x12]
	str r0, [sp, #4]
	add r0, r4, #0x2c
	add r6, r0, #0x400
	cmp r1, #1
	movne r0, #0xb
	bne _06012B70
	ldrh r0, [r6, #0x3c]
	cmp r0, #0
	movne r0, #8
	bne _06012B70
	ldrh r0, [sl, #0x10]
	ldr r5, [r6, #0x44]
	tst r0, #0x8000
	beq _06012794
	tst r0, #2
	ldreqh r0, [r6, #0x94]
	streqh r0, [sl, #0x14]
	ldrh r0, [sl, #0x10]
	tst r0, #4
	ldreqh r0, [r6, #0xa0]
	streqh r0, [sl, #0x16]
	ldrh r0, [sl, #0x10]
	tst r0, #8
	ldreqh r0, [r6, #0x96]
	streqh r0, [sl, #0x18]
	ldrh r0, [sl, #0x10]
	tst r0, #0x10
	ldreqh r0, [r6, #0x9c]
	moveq sb, #0
	streqh r0, [sl, #0x1c]
_06012794:
	ldrh r0, [sl, #0x1c]
	cmp r0, #0x204
	movhi r0, #5
	bhi _06012B70
	mov r1, #2
	mov r8, #0
	b _060127C4
_060127B0:
	ldrh r0, [sl, #0x16]
	tst r0, r1
	mov r0, r1, lsl #0x11
	addne r8, r8, #1
	mov r1, r0, lsr #0x10
_060127C4:
	cmp r1, #0
	bne _060127B0
	ldrh r0, [sl, #0x14]
	strh r0, [r6, #0x94]
	ldrh r0, [sl, #0x14]
	tst r0, #0x8000
	beq _06012810
	ldr r1, _06012B80 ; =0x00007FFF
	mov r2, #0xea
	and r0, r0, r1
	strh r0, [sl, #0x14]
	ldrh r0, [sl, #0x14]
	sub r0, r0, #0xd0
	mov r0, r0, lsr #2
	strh r2, [r5, #0xe]
	cmp r0, #0x10000
	bls _06012860
	mov r0, #5
	b _06012B70
_06012810:
	ldr r1, [sp, #4]
	tst r1, #2
	movne fp, #1
	moveq fp, #0
	add r3, fp, #6
	movne r1, #2
	movne r2, #6
	add r0, r0, fp
	add r3, r3, #0x200
	moveq r1, fp
	moveq r2, fp
	cmp r0, r3
	movhi r0, #5
	bhi _06012B70
	mov r3, r0, lsl #2
	add r3, r3, #0xd0
	add r1, r1, r3
	strh r1, [sl, #0x14]
	add r1, r2, #0xea
	strh r1, [r5, #0xe]
_06012860:
	add r0, r0, #9
	bic fp, r0, #1
	mul r0, fp, r8
	str r0, [sp]
	ldr r1, [sp]
	add r0, r4, #0x188
	add r1, r1, #0x1a
	bl AllocateHeapBuf
	str r0, [r6, #0x90]
	cmp r0, #0
	moveq r0, #8
	beq _06012B70
	mov r0, #1
	strh r0, [r6, #0x3c]
	ldrh r0, [r6, #0x3e]
	mov r2, #0
	add r0, r0, #1
	strh r0, [r6, #0x3e]
	ldrh r0, [sl, #0x18]
	add r3, r7, #8
	strh r0, [r6, #0x96]
	ldrh r0, [sl, #0x16]
	mov ip, #0x14
	strh r0, [r6, #0x98]
	strh r2, [r6, #0x9a]
	ldrh r0, [sl, #0x1c]
	mov lr, #0x228
	strh r0, [r6, #0x9c]
	ldrh r1, [sl, #0x12]
	add r0, r5, #0x10
	strh r1, [r6, #0x9e]
	strh r2, [r5]
	ldrh r7, [sl, #0x16]
	ldr r1, _06012B84 ; =MP_ADRS
	strh r7, [r5, #2]
	strh r2, [r5, #4]
	strh ip, [r5, #8]
	ldrh r7, [sl, #0x1c]
	add r2, r4, #0x3a8
	add r7, r7, #0x22
	strh r7, [r5, #0xa]
	strh lr, [r5, #0xc]
	ldrh ip, [sl, #0x14]
	ldrh r7, [r5, #0xe]
	add ip, ip, #0xa
	mla r7, ip, r8, r7
	strh r7, [r5, #0xe]
	bl WSetMacAdrs3
	ldrh r0, [sl, #0x10]
	tst r0, #0x8000
	ldrneh r1, [r5, #0x22]
	ldrne r0, _06012B88 ; =0x0000FFFF
	cmpne r1, r0
	ldreq r0, _06012B88 ; =0x0000FFFF
	movne r7, #0x4000
	streqh r0, [r5, #0x22]
	ldrh r0, [sl, #0x14]
	moveq r7, #0
	strh r0, [r5, #0x24]
	ldrh r0, [sl, #0x16]
	cmp sb, #0
	strh r0, [r5, #0x26]
	ldrh r0, [sl, #0x1e]
	strh r0, [r5, #0x28]
	ldrneh r0, [sl, #0x1c]
	cmpne r0, #0
	beq _06012980
	bl WUpdateCounter
	ldr r1, [sl, #0x20]
	ldrh r2, [sl, #0x1c]
	add r0, r5, #0x2a
	bl DMA_Write
_06012980:
	ldr r0, [sp, #4]
	tst r0, #4
	beq _060129B4
	ldrh r0, [sl, #0x1c]
	add r1, r5, #0x28
	add r0, r0, #2
	add r0, r1, r0
	add r0, r0, #3
	ldr r1, _06012B8C ; =0x0000B6B8
	bic r2, r0, #3
	ldr r0, _06012B90 ; =0x00001D46
	strh r1, [r2]
	strh r0, [r2, #2]
_060129B4:
	ldr r0, [r6, #0x90]
	mov r1, #0x184
	strh r1, [r0, #0xc]
	ldr r0, [sp]
	ldr r1, [r6, #0x90]
	add r0, r0, #0xb
	mov r0, r0, lsr #1
	strh r0, [r1, #0xe]
	ldrh r1, [sl, #0x16]
	ldr r0, [r6, #0x90]
	mov r3, #0
	strh r1, [r0, #0x10]
	ldr r0, [r6, #0x90]
	ldr sb, _06012B88 ; =0x0000FFFF
	strh r8, [r0, #0x14]
	ldr r0, [r6, #0x90]
	mov r1, #1
	strh fp, [r0, #0x16]
	ldr r0, [r6, #0x90]
	mov r2, #2
	strh r3, [r0, #0x18]
	ldr r0, [r6, #0x90]
	add r0, r0, #0x1a
	b _06012A3C
_06012A14:
	ldrh ip, [sl, #0x16]
	tst ip, r2
	strneh sb, [r0]
	strneh r3, [r0, #2]
	strneh r3, [r0, #6]
	strneh r1, [r0, #4]
	mov r2, r2, lsl #0x11
	addne r0, r0, fp
	mov r2, r2, lsr #0x10
	add r1, r1, #1
_06012A3C:
	cmp r2, #0
	bne _06012A14
	ldrh r0, [sl, #0x14]
	ldr r2, _06012B94 ; =0x048080C4
	strh r0, [r2]
	ldrh r0, [r5, #0xe]
	strh r0, [r2, #-4]
	ldrh r1, [sl, #0x1a]
	ldrh r0, [sl, #0x18]
	rsb sb, r1, #0x10000
	cmp r0, #0
	bne _06012AD4
	ldrh r0, [r2, #-0xc4]
	ldrh r1, [sl, #0x1c]
	cmp r0, #0x1440
	ldrh r0, [sl, #0x14]
	add r1, r1, #0x22
	mul r2, r0, r8
	mov r1, r1, lsl #2
	add r3, r1, #0x60
	addne r3, r3, #0x3e8
	add r0, r2, #0x388
	add r0, r3, r0
	mov r1, #0xa
	add r0, r0, #0x32
	bl __VENEER__u32_div_f
	mov r4, r0
	bl __VENEER_OS_DisableInterrupts
	ldr r1, _06012B98 ; =0x00003FFF
	ldr r2, _06012B9C ; =0x04808118
	and r1, r5, r1
	mov r1, r1, lsr #1
	orr r1, r1, #0x8000
	strh r4, [r2]
	orr r1, r1, r7
	strh r1, [r2, #-0x88]
	bl __VENEER_OS_RestoreInterrupts
	b _06012B6C
_06012AD4:
	bl __VENEER_OS_DisableInterrupts
	ldr r1, _06012BA0 ; =0x048080F8
	mov r8, r0
	ldrh r0, [r1]
	mov r1, #0xa
	add r0, sb, r0
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	bl __VENEER__u32_div_f
	ldrh r3, [sl, #0x18]
	add r1, r0, #3
	cmp r1, r3
	bhs _06012B3C
	ldr r1, _06012B98 ; =0x00003FFF
	ldr r2, _06012B9C ; =0x04808118
	sub r3, r3, r0
	and r0, r5, r1
	sub r1, r3, #1
	mov r0, r0, lsr #1
	orr r0, r0, #0x8000
	strh r1, [r2]
	orr r1, r0, r7
	mov r0, r8
	strh r1, [r2, #-0x88]
	bl __VENEER_OS_RestoreInterrupts
	b _06012B6C
_06012B3C:
	mov r0, r8
	bl __VENEER_OS_RestoreInterrupts
	ldr r1, [r6, #0x90]
	add r0, r4, #0x188
	bl ReleaseHeapBuf
	mov r0, #0
	strh r0, [r6, #0x3c]
	ldrh r1, [r6, #0x3e]
	mov r0, #5
	sub r1, r1, #1
	strh r1, [r6, #0x3e]
	b _06012B70
_06012B6C:
	mov r0, #0
_06012B70:
	add sp, sp, #8
	ldmia sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_06012B7C: .word 0x0380FFF4
_06012B80: .word 0x00007FFF
_06012B84: .word MP_ADRS
_06012B88: .word 0x0000FFFF
_06012B8C: .word 0x0000B6B8
_06012B90: .word 0x00001D46
_06012B94: .word 0x048080C4
_06012B98: .word 0x00003FFF
_06012B9C: .word 0x04808118
_06012BA0: .word 0x048080F8
	arm_func_end MA_MpReqCmd
