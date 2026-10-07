	.include "asm/macros.inc"
	.include "global.inc"

	.extern AddTask
	.extern AdjustRingPointer
	.extern CheckKeyTxEnd
	.extern IntrCarrierSuppresionSignal
	.extern SetFatalErr
	.extern SetParentTbttTxq
	.extern WClearKSID
	.extern WUpdateCounter
	.extern WlIntrMpEnd
	.extern __VENEER_OS_CancelAlarm
	.extern __VENEER_OS_SetAlarm
	.extern __VENEER__ll_udiv

.text

	arm_func_start WlIntrRxEnd
WlIntrRxEnd: ; 0x0600CE98
	stmdb sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0x38
	ldr r0, _0600D328 ; =0x0380FFF4
	ldr r2, _0600D32C ; =0x04808010
	ldr r3, [r0]
	mov r1, #1
	add r0, r3, #0x600
	ldrh r4, [r0, #0x90]
	add r6, r3, #0x344
	strh r1, [r2]
	ldrh r1, [r6, #0xc]
	add r0, r3, #0xdc
	cmp r1, #0
	add r7, r0, #0x400
	ldreqh r0, [r2, #0x44]
	and r4, r4, #1
	streqh r0, [r2, #0x4a]
	ldr r0, _0600D330 ; =0x04808098
	sub fp, r0, #0x70
	add r0, r0, #0x1b4
	str r0, [sp, #0x24]
	ldr r0, _0600D334 ; =0x0480824E
	sub r0, r0, #0x154
	str r0, [sp, #0x14]
	ldr r0, _0600D330 ; =0x04808098
	sub r0, r0, #0x44
	str r0, [sp, #0x10]
	ldr r0, _0600D338 ; =0x000008EF
	sub r0, r0, #0x29
	str r0, [sp, #0x28]
	ldr r0, _0600D338 ; =0x000008EF
	add r0, r0, #0x3d
	str r0, [sp, #0x2c]
	ldr r0, _0600D33C ; =0x04805F7E
	sub r0, r0, #0x24
	str r0, [sp, #0x30]
	ldr r0, _0600D338 ; =0x000008EF
	add r0, r0, #0x710
	str r0, [sp, #0x34]
_0600CF34:
	ldr r0, [sp, #0x10]
	ldrh sb, [r7, #4]
	ldrh sl, [r0]
	cmp sb, sl
	beq _0600D2A8
	ldr r0, _0600D330 ; =0x04808098
	ldr r1, [sp, #0x14]
	ldrh r0, [r0, #0x60]
	ldrh r3, [r1]
	ldr r1, _0600D330 ; =0x04808098
	ldrh r2, [r1, #0x60]
	ldr r1, [sp, #0x14]
	cmp r0, r2
	ldrh r1, [r1]
	movhi r0, r1, lsl #0xc
	orrhi r0, r0, r2, lsr #4
	strhi r0, [sp, #4]
	movls r1, r3, lsl #0xc
	orrls r0, r1, r0, lsr #4
	strls r0, [sp, #4]
	ldr r0, [sp, #0x28]
	cmp sb, r0
	blo _0600CFA0
	ldr r0, _0600D338 ; =0x000008EF
	cmp sb, r0
	bhi _0600CFA0
	bl WUpdateCounter
_0600CFA0:
	mov r0, sb, lsl #1
	str r0, [sp, #0x18]
	add r0, r0, #0x4000
	add r8, r0, #0x4800000
	add r0, r8, #2
	bl AdjustRingPointer
	mov r5, r0
	add r0, r5, #2
	bl AdjustRingPointer
	str r0, [sp, #0x1c]
	add r0, r0, #4
	bl AdjustRingPointer
	str r0, [sp, #0x20]
	add r0, r8, #0xe
	bl AdjustRingPointer
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x18]
	ldrh r1, [r5]
	add r0, r0, #0x4800000
	add r0, r0, #0x4000
	mov r1, r1, lsl #1
	ldrh r2, [r0]
	and r1, r1, #0x400
	orr r1, r2, r1
	strh r1, [r0]
	ldr r1, [sp, #4]
	ldr r0, [sp, #0x1c]
	strh r1, [r0]
	ldr r0, [sp, #0x20]
	ldrh r1, [r0]
	add r0, r1, sb, lsl #1
	add r0, r0, #0xf
	mov r0, r0, lsr #2
	mov sb, r0, lsl #1
	cmp sb, #0xfb0
	ldrhsh r0, [r6, #0x9a]
	subhs sb, sb, r0, lsr #1
	ldr r0, [sp, #0x2c]
	cmp r1, r0
	bls _0600D05C
	ldr r0, _0600D340 ; =0x0000FFFF
	mov sb, sl
	strh r0, [r8]
	ldrh r0, [r6, #0xb4]
	add r0, r0, #1
	strh r0, [r6, #0xb4]
	b _0600D0E4
_0600D05C:
	cmp r4, #0
	cmpne sb, sl
	beq _0600D0E4
	ldr r3, [sp, #0x30]
	mov r2, sb, lsl #1
	add r0, r2, #0x4000
	add r0, r0, #0x4800000
	cmp r0, r3
	ldrloh r0, [r0, #6]
	add r2, r2, #0x4800000
	add r2, r2, #0x4000
	ldrhsh r3, [r6, #0x9a]
	andlo r0, r0, #0xff
	subhs r0, r0, r3
	ldrh r2, [r2]
	ldrhsh r0, [r0, #6]
	tst r2, #0x7c00
	bne _0600D0BC
	cmp r0, #0xa
	cmpne r0, #0x14
	bne _0600D0BC
	ldr r0, [sp, #0x34]
	cmp r1, r0
	bls _0600D0E4
_0600D0BC:
	ldrh r0, [r6, #0xb4]
	ldr r1, _0600D340 ; =0x0000FFFF
	add r0, r0, #1
	strh r0, [r6, #0xb4]
	mov r0, sl, lsl #0x10
	strh r1, [r8]
	mov r0, r0, lsr #0x10
	strh r0, [r7, #4]
	strh r0, [r5]
	b _0600D2A8
_0600D0E4:
	ldrh r0, [r8]
	and r0, r0, #0xf
	cmp r0, #0xc
	bne _0600D244
	add r0, r8, #0xc
	bl AdjustRingPointer
	ldrh r0, [r0]
	str r0, [sp, #8]
	add r0, r8, #0x22
	bl AdjustRingPointer
	ldrh sl, [r0]
	ldrh r0, [r7]
	cmp r0, sl
	bne _0600D148
	ldr r0, [sp, #8]
	tst r0, #0x800
	beq _0600D148
	ldr r0, _0600D328 ; =0x0380FFF4
	ldr r1, [r0]
	ldr r0, [r1, #0x57c]
	add r0, r0, #1
	str r0, [r1, #0x57c]
	ldr r0, _0600D340 ; =0x0000FFFF
	strh r0, [r8]
	b _0600D21C
_0600D148:
	ldr r0, _0600D328 ; =0x0380FFF4
	ldr r0, [r0]
	add r1, r0, #0x300
	ldrh r2, [r1, #0x3a]
	mov r2, r2, lsl #0x18
	movs r2, r2, lsr #0x1f
	ldreqh r1, [r1, #0x4c]
	cmpeq r1, #0x40
	bne _0600D21C
	ldrh r1, [fp]
	cmp r1, #0
	beq _0600D1F4
	ldr r1, _0600D330 ; =0x04808098
	ldrh r1, [r1]
	tst r1, #0x8000
	beq _0600D1F4
	add r0, r0, #0x660
	bl __VENEER_OS_CancelAlarm
	ldr r0, [sp, #0xc]
	mov r3, #0
	ldrh r1, [r0]
	ldr r0, _0600D344 ; =0x000082EA
	mov r2, #0x3e8
	umull ip, r8, r1, r0
	mov r0, r3
	mla r8, r1, r0, r8
	mov r0, ip, lsr #6
	ldr r1, _0600D344 ; =0x000082EA
	mov ip, r3
	mla r8, ip, r1, r8
	mov r1, r8, lsr #6
	orr r0, r0, r8, lsl #26
	bl __VENEER__ll_udiv
	mov r2, #0
	str r2, [sp]
	mov r2, r1
	mov r1, r0
	ldr r0, _0600D328 ; =0x0380FFF4
	ldr r3, _0600D348 ; =WClearKSID
	ldr r0, [r0]
	add r0, r0, #0x660
	bl __VENEER_OS_SetAlarm
	b _0600D21C
_0600D1F4:
	ldr r1, _0600D340 ; =0x0000FFFF
	ldr r0, _0600D33C ; =0x04805F7E
	mov sl, r1
	strh r1, [r0]
	ldr r0, [sp, #0x24]
	strh r1, [r0]
	ldr r0, _0600D334 ; =0x0480824E
	strh r1, [r0]
	mov r0, r1
	strh r0, [r8]
_0600D21C:
	strh sl, [r7]
	bl CheckKeyTxEnd
	tst r0, #1
	beq _0600D294
	ldr r0, _0600D328 ; =0x0380FFF4
	ldr r1, [r0]
	ldr r0, [r1, #0x5a8]
	add r0, r0, #1
	str r0, [r1, #0x5a8]
	b _0600D294
_0600D244:
	cmp r0, #0xd
	bne _0600D294
	ldr r0, _0600D328 ; =0x0380FFF4
	ldr r0, [r0]
	add r0, r0, #0x300
	ldrh r1, [r0, #0x3a]
	mov r1, r1, lsl #0x18
	movs r1, r1, lsr #0x1f
	ldreqh r0, [r0, #0x4c]
	cmpeq r0, #0x40
	bne _0600D294
	ldrh r0, [fp]
	cmp r0, #0
	beq _0600D28C
	ldr r0, _0600D330 ; =0x04808098
	ldrh r0, [r0]
	tst r0, #0x8000
	bne _0600D294
_0600D28C:
	ldr r0, _0600D340 ; =0x0000FFFF
	strh r0, [r8]
_0600D294:
	mov r0, sb, lsl #0x10
	mov r0, r0, lsr #0x10
	strh r0, [r7, #4]
	strh r0, [r5]
	b _0600CF34
_0600D2A8:
	cmp r4, #0
	beq _0600D2FC
	ldr r0, _0600D34C ; =0x04808054
	ldrh r4, [r0]
	bl CheckKeyTxEnd
	mov r0, r0, lsl #0x10
	movs r1, r0, lsr #0x10
	beq _0600D2FC
	ldr r0, _0600D34C ; =0x04808054
	ldrh r0, [r0]
	cmp r4, r0
	bne _0600D2FC
	tst r1, #2
	beq _0600D2EC
	mov r0, #0x80
	bl SetFatalErr
	b _0600D2FC
_0600D2EC:
	tst r1, #1
	beq _0600D2FC
	mov r0, #0x100
	bl SetFatalErr
_0600D2FC:
	ldr r0, _0600D350 ; =0x0480805A
	ldrh r1, [r0]
	ldrh r0, [r0, #-6]
	cmp r1, r0
	beq _0600D31C
	mov r0, #0
	mov r1, #0xf
	bl AddTask
_0600D31C:
	add sp, sp, #0x38
	ldmia sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_0600D328: .word 0x0380FFF4
_0600D32C: .word 0x04808010
_0600D330: .word 0x04808098
_0600D334: .word 0x0480824E
_0600D338: .word 0x000008EF
_0600D33C: .word 0x04805F7E
_0600D340: .word 0x0000FFFF
_0600D344: .word 0x000082EA
_0600D348: .word WClearKSID
_0600D34C: .word 0x04808054
_0600D350: .word 0x0480805A
	arm_func_end WlIntrRxEnd
