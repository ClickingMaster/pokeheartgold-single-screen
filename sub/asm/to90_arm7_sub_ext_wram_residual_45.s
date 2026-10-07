	.include "asm/macros.inc"
	.include "global.inc"

	.extern AddTask
	.extern AdjustRingPointer
	.extern MoveHeapBuf
	.extern ReleaseHeapBuf
	.extern RxBeaconFrame
	.extern RxKeyDataFrame
	.extern RxMpAckFrame
	.extern RxMpFrame
	.extern SetFatalErr
	.extern TakeoutRxFrame
	.extern WUpdateCounter

.text

	arm_func_start WlIntrRxEndTask
WlIntrRxEndTask: ; 0x0600DC34
	stmdb sp!, {r4, r5, r6, r7, r8, lr}
	ldr r0, _0600DF84 ; =0x0380FFF4
	ldr r4, [r0]
	add r0, r4, #0xdc
	add r5, r0, #0x400
_0600DC48:
	ldr r1, _0600DF88 ; =0x0480805A
	ldrh r0, [r5, #4]
	ldrh r6, [r1]
	cmp r6, r0
	beq _0600DF7C
	ldr r0, _0600DF8C ; =0x000008C6
	cmp r6, r0
	blo _0600DC6C
	bl WUpdateCounter
_0600DC6C:
	mov r6, r6, lsl #1
	add r0, r6, #0x4000
	add r7, r0, #0x4800000
	add r0, r7, #2
	bl AdjustRingPointer
	add r1, r6, #0x4800000
	add r1, r1, #0x4000
	ldrh r2, [r1]
	ldr r1, _0600DF90 ; =0x0000FFFF
	ldrh r6, [r0]
	cmp r2, r1
	ldreq r0, _0600DF88 ; =0x0480805A
	streqh r6, [r0]
	beq _0600DC48
	add r0, r7, #8
	bl AdjustRingPointer
	ldrh r1, [r0]
	mov r0, r7
	bl TakeoutRxFrame
	ldr r1, _0600DF88 ; =0x0480805A
	movs r8, r0
	strh r6, [r1]
	bne _0600DCF0
	ldrh r0, [r7]
	and r0, r0, #0xf
	cmp r0, #0xc
	bne _0600DCE4
	mov r0, #0x10
	bl SetFatalErr
	b _0600DC48
_0600DCE4:
	mov r0, #8
	bl SetFatalErr
	b _0600DC48
_0600DCF0:
	ldr r0, _0600DF84 ; =0x0380FFF4
	ldr r0, [r0]
	add r0, r0, #0x600
	ldrh r0, [r0, #0x90]
	tst r0, #8
	beq _0600DD18
	ldrh r0, [r8, #0x14]
	tst r0, #0x4000
	movne r0, #0
	strneh r0, [r5, #2]
_0600DD18:
	ldrh r0, [r8, #8]
	mov r6, #1
	tst r0, #0x200
	beq _0600DD70
	ldrh r0, [r8, #0x14]
	mov r0, r0, lsl #0x15
	mov r0, r0, lsr #0x1f
	cmp r0, #1
	beq _0600DD4C
	ldrh r0, [r8, #0x2a]
	mov r0, r0, lsl #0x1c
	movs r0, r0, lsr #0x1c
	beq _0600DF1C
_0600DD4C:
	add r0, r4, #0x188
	add r1, r4, #0x1e8
	sub r2, r8, #0x10
	mov r6, #0
	bl MoveHeapBuf
	mov r0, #2
	mov r1, #9
	bl AddTask
	b _0600DF1C
_0600DD70:
	and r0, r0, #0xf
	cmp r0, #0xf
	addls pc, pc, r0, lsl #2
	b _0600DF1C
_0600DD80: ; jump table
	b _0600DE0C ; case 0
	b _0600DDF4 ; case 1
	b _0600DF1C ; case 2
	b _0600DF1C ; case 3
	b _0600DF1C ; case 4
	b _0600DE3C ; case 5
	b _0600DF1C ; case 6
	b _0600DF1C ; case 7
	b _0600DDC0 ; case 8
	b _0600DF1C ; case 9
	b _0600DF1C ; case 10
	b _0600DF1C ; case 11
	b _0600DE94 ; case 12
	b _0600DEE8 ; case 13
	b _0600DE74 ; case 14
	b _0600DE74 ; case 15
_0600DDC0:
	ldrh r0, [r8, #0x14]
	and r0, r0, #0xf
	cmp r0, #8
	bne _0600DF1C
	add r0, r4, #0x188
	add r1, r4, #0x1c4
	sub r2, r8, #0x10
	mov r6, #0
	bl MoveHeapBuf
	mov r0, #2
	mov r1, #6
	bl AddTask
	b _0600DF1C
_0600DDF4:
	ldrh r0, [r8, #0x14]
	cmp r0, #0x80
	bne _0600DF1C
	mov r0, r8
	bl RxBeaconFrame
	b _0600DF1C
_0600DE0C:
	ldrh r0, [r8, #0x14]
	tst r0, #0xf
	bne _0600DF1C
	add r0, r4, #0x188
	add r1, r4, #0x1dc
	sub r2, r8, #0x10
	mov r6, #0
	bl MoveHeapBuf
	mov r0, #1
	mov r1, #7
	bl AddTask
	b _0600DF1C
_0600DE3C:
	ldrh r1, [r8, #0x14]
	rsb r0, r6, #0xe800
	and r0, r1, r0
	cmp r0, #0xa4
	bne _0600DF1C
	add r0, r4, #0x188
	add r1, r4, #0x1dc
	sub r2, r8, #0x10
	mov r6, #0
	bl MoveHeapBuf
	mov r0, #1
	mov r1, #7
	bl AddTask
	b _0600DF1C
_0600DE74:
	ldrh r1, [r8, #0x14]
	ldr r0, _0600DF94 ; =0x0000E7BF
	and r0, r1, r0
	cmp r0, #0x118
	bne _0600DF1C
	mov r0, r8
	bl RxKeyDataFrame
	b _0600DF1C
_0600DE94:
	ldrh r1, [r8, #0x14]
	rsb r0, r6, #0xe800
	and r0, r1, r0
	cmp r0, #0x228
	bne _0600DF1C
	ldr r0, _0600DF84 ; =0x0380FFF4
	ldr r1, _0600DF84 ; =0x0380FFF4
	ldr r0, [r0]
	add r0, r0, #0x300
	ldrh r0, [r0, #0x54]
	cmp r0, #0
	ldreq r0, _0600DF98 ; =0x0480803C
	streqh r6, [r0]
	ldr r2, [r1]
	mov r0, r8
	ldr r1, [r2, #0x5ac]
	add r1, r1, #1
	str r1, [r2, #0x5ac]
	bl RxMpFrame
	mov r6, r0
	b _0600DF1C
_0600DEE8:
	ldrh r1, [r8, #0x14]
	rsb r0, r6, #0xe800
	and r0, r1, r0
	cmp r0, #0x218
	bne _0600DF1C
	ldr r1, _0600DF84 ; =0x0380FFF4
	mov r0, r8
	ldr r2, [r1]
	ldr r1, [r2, #0x5b0]
	add r1, r1, #1
	str r1, [r2, #0x5b0]
	bl RxMpAckFrame
	mov r6, r0
_0600DF1C:
	cmp r6, #0
	beq _0600DF30
	add r0, r4, #0x188
	sub r1, r8, #0x10
	bl ReleaseHeapBuf
_0600DF30:
	ldr r3, _0600DF84 ; =0x0380FFF4
	ldr r0, [r3]
	add r0, r0, #0x600
	ldrh r0, [r0, #0x90]
	tst r0, #1
	beq _0600DC48
	ldr r2, _0600DF90 ; =0x0000FFFF
	ldr r1, _0600DF9C ; =0x04805F60
	mov r6, #0
_0600DF54:
	cmp r7, r1
	ldrhs r0, [r3]
	add r6, r6, #1
	addhs r0, r0, #0x300
	ldrhsh r0, [r0, #0xde]
	subhs r7, r7, r0
	cmp r6, #7
	strh r2, [r7], #2
	blo _0600DF54
	b _0600DC48
_0600DF7C:
	ldmia sp!, {r4, r5, r6, r7, r8, lr}
	bx lr
	.align 2, 0
_0600DF84: .word 0x0380FFF4
_0600DF88: .word 0x0480805A
_0600DF8C: .word 0x000008C6
_0600DF90: .word 0x0000FFFF
_0600DF94: .word 0x0000E7BF
_0600DF98: .word 0x0480803C
_0600DF9C: .word 0x04805F60
	arm_func_end WlIntrRxEndTask
