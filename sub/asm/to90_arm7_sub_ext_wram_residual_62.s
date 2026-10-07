	.include "asm/macros.inc"
	.include "global.inc"

	.extern AddTask
	.extern CAM_GetAuthSeed
	.extern CAM_GetStaState
	.extern CAM_SetAuthSeed
	.extern CAM_SetStaState
	.extern ClearTimeOut
	.extern ElementChecker
	.extern IsExistManFrame
	.extern MLME_IssueDeAuthIndication
	.extern MakeAuthFrame
	.extern MatchMacAdrs
	.extern NULL_ADRS
	.extern RND_rand
	.extern RND_seed
	.extern TxManCtrlFrame
	.extern WL_ReadByte
	.extern WL_WriteByte
	.extern WSetMacAdrs1
	.extern WSetStaState
	.extern __VENEER_MIi_CpuClear16
	.extern __VENEER_MIi_CpuClear32
	.extern __VENEER_MIi_CpuCopy16

.text

	arm_func_start RxProbeResFrame
RxProbeResFrame: ; 0x060162A4
	stmdb sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0x30
	ldr r2, _0601666C ; =0x0380FFF4
	mov r7, r0
	ldr r2, [r2]
	mov sl, r1
	add r0, r2, #0x400
	ldrh r1, [r0, #4]
	add r0, r2, #4
	cmp r1, #0x13
	add r4, r0, #0x400
	bne _06016660
	ldr r0, [r4, #0x18]
	ldr r5, [r4, #0x1c]
	add r8, r0, #0x4e
	mov r6, #0
	b _06016314
_060162E8:
	mov r0, r8
	add r1, r7, #0x24
	bl MatchMacAdrs
	cmp r0, #0
	ldrneh r1, [r5, #6]
	movne r0, #1
	orrne r0, r1, r0, lsl r6
	strneh r0, [r5, #6]
	bne _06016660
	add r8, r8, #6
	add r6, r6, #1
_06016314:
	ldr r0, [r4, #0x18]
	ldrh r0, [r0, #0x4c]
	cmp r6, r0
	blo _060162E8
	add r6, r5, #0xa
	mov r8, #0
	b _06016350
_06016330:
	add r0, r7, #0x24
	add r1, r6, #4
	bl MatchMacAdrs
	cmp r0, #0
	bne _06016660
	ldrh r0, [r6]
	add r8, r8, #1
	add r6, r6, r0, lsl #1
_06016350:
	ldrh r0, [r5, #8]
	cmp r8, r0
	blo _06016330
	mov r1, r6
	mov r0, #0
	mov r2, #0x40
	bl __VENEER_MIi_CpuClear16
	ldrh r8, [r7, #6]
	cmp r8, #0xc
	bls _06016660
	cmp sl, #0
	bne _060163CC
	add r1, sp, #4
	mov r0, #0
	mov r2, #0x2c
	bl __VENEER_MIi_CpuClear32
	add r3, r7, #0x38
	sub r2, r8, #0xc
	mov r0, #0x38
	strh r0, [sp, #0x10]
	mov r1, #3
	str r3, [sp, #4]
	strh r2, [sp, #0xc]
	strh r1, [sp, #0xe]
	ldrh r1, [r7, #8]
	add r0, sp, #4
	strh r1, [sp, #8]
	ldrh r1, [r7, #0x36]
	strh r1, [sp, #0xa]
	bl ElementChecker
	add sl, sp, #4
_060163CC:
	ldr r0, [sl, #0x28]
	cmp r0, #0
	ldreqh r0, [sl, #0x1a]
	beq _060163F0
	add r0, r0, #1
	bl WL_ReadByte
	sub r0, r0, #8
	strh r0, [r6, #0x3c]
	ldrh r0, [r6, #0x3c]
_060163F0:
	add r0, r0, #0x41
	mov r0, r0, lsr #1
	strh r0, [r6]
	ldrh r0, [sl, #0xa]
	and r0, r0, #1
	cmp r0, #1
	bne _06016654
	ldr r0, _0601666C ; =0x0380FFF4
	ldrh r1, [r6]
	ldr r0, [r0]
	add r0, r0, #0x400
	ldrh r0, [r0, #8]
	cmp r0, r1
	blo _06016654
	ldrh r1, [r7, #0x36]
	add r0, r6, #4
	strh r1, [r6, #0x2c]
	add r1, r7, #0x24
	bl WSetMacAdrs1
	ldrh r0, [r7, #0x34]
	strh r0, [r6, #0x32]
	ldrh r0, [r7, #0x12]
	and r0, r0, #0xff
	strh r0, [r6, #2]
	ldr r0, [sl, #0x28]
	cmp r0, #0
	beq _06016498
	mov r7, #0
	add r8, r6, #0x40
	b _06016488
_06016468:
	ldr r0, [sl, #0x28]
	add r0, r0, #0xa
	add r0, r0, r7
	bl WL_ReadByte
	mov r1, r0
	add r0, r8, r7
	bl WL_WriteByte
	add r7, r7, #1
_06016488:
	ldrh r0, [r6, #0x3c]
	cmp r7, r0
	blo _06016468
	b _0601653C
_06016498:
	ldrh r0, [sl, #0x18]
	strh r0, [r6, #0x3e]
	ldrh r0, [sl, #0x18]
	cmp r0, #0
	beq _0601653C
	add r7, r7, #0x38
	add fp, r6, #0x40
	mov r8, #0
	b _06016530
_060164BC:
	mov r0, r7
	bl WL_ReadByte
	mov sb, r0
	add r0, r7, #1
	bl WL_ReadByte
	cmp sb, #6
	bls _06016528
	ldr r1, [sl, #0x28]
	cmp r7, r1
	beq _06016528
	add r0, r0, #2
	mov sb, #0
	str r0, [sp]
	b _06016514
_060164F4:
	mov r0, r7
	bl WL_ReadByte
	mov r1, r0
	mov r0, fp
	bl WL_WriteByte
	add fp, fp, #1
	add r7, r7, #1
	add sb, sb, #1
_06016514:
	ldr r0, [sp]
	cmp sb, r0
	blo _060164F4
	add r8, r8, #1
	b _06016530
_06016528:
	add r0, r0, #2
	add r7, r7, r0
_06016530:
	ldrh r0, [sl, #0x18]
	cmp r8, r0
	blo _060164BC
_0601653C:
	ldr r0, [sl, #0x1c]
	cmp r0, #0
	beq _06016590
	add r0, r0, #1
	bl WL_ReadByte
	strh r0, [r6, #0xa]
	mov r7, #0
	add r8, r6, #0xc
	b _06016580
_06016560:
	ldr r0, [sl, #0x1c]
	add r0, r0, #2
	add r0, r0, r7
	bl WL_ReadByte
	mov r1, r0
	add r0, r8, r7
	bl WL_WriteByte
	add r7, r7, #1
_06016580:
	ldrh r0, [r6, #0xa]
	cmp r7, r0
	blo _06016560
	b _060165B8
_06016590:
	mov sb, #0
	strh sb, [r6, #0xa]
	add r8, r6, #0xc
	mov r7, sb
_060165A0:
	mov r1, r7
	add r0, r8, sb
	bl WL_WriteByte
	add sb, sb, #1
	cmp sb, #0x20
	blo _060165A0
_060165B8:
	ldrh r0, [sl, #0x14]
	strh r0, [r6, #0x2e]
	ldrh r0, [sl, #0x16]
	strh r0, [r6, #0x30]
	ldrh r0, [sl, #0x12]
	strh r0, [r6, #0x36]
	ldr r0, [sl, #0x20]
	cmp r0, #0
	beq _060165E8
	add r0, r0, #3
	bl WL_ReadByte
	strh r0, [r6, #0x38]
_060165E8:
	ldr r0, [sl, #0x24]
	cmp r0, #0
	beq _06016600
	add r0, r0, #3
	bl WL_ReadByte
	strh r0, [r6, #0x34]
_06016600:
	ldrh r1, [r5, #2]
	ldrh r0, [r6]
	add r0, r1, r0
	strh r0, [r5, #2]
	ldrh r0, [r5, #8]
	add r0, r0, #1
	strh r0, [r5, #8]
	ldrh r1, [r4, #4]
	ldrh r0, [r6]
	sub r0, r1, r0
	strh r0, [r4, #4]
	ldrh r0, [r4, #4]
	cmp r0, #0x20
	bhs _06016660
	bl ClearTimeOut
	mov r2, #0x15
	mov r0, #2
	mov r1, #0
	strh r2, [r4]
	bl AddTask
	b _06016660
_06016654:
	ldr r1, _06016670 ; =NULL_ADRS
	add r0, r6, #4
	bl WSetMacAdrs1
_06016660:
	add sp, sp, #0x30
	ldmia sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_0601666C: .word 0x0380FFF4
_06016670: .word NULL_ADRS
	arm_func_end RxProbeResFrame

	arm_func_start RxAuthFrame
RxAuthFrame: ; 0x06016674
	stmdb sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	ldr r1, _06016C00 ; =0x0380FFF4
	mov sl, r0
	ldr r2, [r1]
	add r0, r2, #0x600
	ldrh r1, [r0, #0x90]
	add r0, r2, #4
	tst r1, #8
	add r4, r2, #0x344
	add sb, r0, #0x400
	beq _06016714
	ldr r1, _06016C04 ; =0x048080B0
	add r0, r2, #0x2c
	ldrh r2, [r1]
	add r1, r0, #0x400
	tst r2, #1
	ldrneh r0, [r1]
	cmpne r0, #0
	bne _06016714
	tst r2, #4
	ldrneh r0, [r1, #0x14]
	cmpne r0, #0
	bne _06016714
	tst r2, #8
	ldrneh r0, [r1, #0x28]
	cmpne r0, #0
	bne _06016714
	ldr r0, _06016C08 ; =0x0480819C
	ldrh r0, [r0]
	tst r0, #1
	bne _06016714
	ldr r2, _06016C0C ; =0x04808032
	mov r3, #0
	strh r3, [r2]
	mov r1, #0x8000
	ldr r0, _06016C00 ; =0x0380FFF4
	strh r1, [r2]
	ldr r0, [r0]
	add r0, r0, #0x400
	strh r3, [r0, #0xde]
_06016714:
	add r0, sl, #0x1e
	mov r1, #0xb0
	bl IsExistManFrame
	cmp r0, #0
	bne _06016BF8
	ldrh r0, [sl, #0x2e]
	ldrh r8, [sl, #2]
	add r0, r0, #1
	mov r0, r0, lsl #0x10
	cmp r8, #0
	mov r5, #0
	mov r6, r0, lsr #0x10
	moveq r7, #0x13
	moveq r5, #1
	beq _06016BBC
	ldrh r0, [r4, #0xc]
	cmp r0, #1
	bne _060167C4
	mov r0, r8
	bl CAM_GetStaState
	cmp r0, #0x20
	bls _06016788
	mov r0, r8, lsl #0x10
	mov r0, r0, lsr #0x10
	mov r1, #0x20
	bl CAM_SetStaState
	add r0, sl, #0x1e
	mov r1, #1
	bl MLME_IssueDeAuthIndication
_06016788:
	ldrh r0, [sl, #8]
	tst r0, #0x400
	beq _060167C4
	mov r0, r8
	bl CAM_GetAuthSeed
	cmp r0, #0
	beq _060167C4
	mov r5, #1
	mov r0, r8
	mov r1, #0
	strh r5, [sl, #0x2c]
	mov r7, #0xf
	mov r6, #4
	bl CAM_SetAuthSeed
	b _06016BBC
_060167C4:
	ldrh r0, [sl, #0x2c]
	cmp r0, #0
	beq _060167DC
	cmp r0, #1
	beq _060168B8
	b _06016BAC
_060167DC:
	ldrh r1, [r4, #0xc]
	cmp r1, #1
	ldreq r0, _06016C00 ; =0x0380FFF4
	ldreq r0, [r0]
	addeq r0, r0, #0x300
	ldreqh r0, [r0, #0x32]
	cmpeq r0, #1
	moveq r7, #0xd
	moveq r5, #1
	beq _06016BBC
	cmp r1, #1
	bne _06016828
	ldrh r0, [sl, #0x2e]
	mov r5, #1
	cmp r0, #1
	moveq r7, #0
	movne r7, #0xe
	movne r6, #2
	b _06016BBC
_06016828:
	beq _06016BBC
	ldrh r0, [sl, #0x2e]
	cmp r0, #2
	ldreq r1, [sb, #0x18]
	ldreqh r0, [r1, #0x16]
	cmpeq r0, #0
	bne _06016BBC
	add r0, r1, #0x10
	add r1, sl, #0x1e
	bl MatchMacAdrs
	cmp r0, #0
	beq _06016BBC
	ldrh r0, [sb]
	cmp r0, #0x31
	bne _06016BBC
	bl ClearTimeOut
	ldrh r0, [sl, #0x30]
	cmp r0, #0
	ldrne r0, [sb, #0x1c]
	movne r1, #0xc
	strneh r1, [r0, #4]
	ldrneh r1, [sl, #0x30]
	bne _06016898
	mov r0, #0x30
	bl WSetStaState
	ldr r0, [sb, #0x1c]
	mov r1, #0
	strh r1, [r0, #4]
_06016898:
	ldr r0, [sb, #0x1c]
	mov r2, #0x35
	strh r1, [r0, #6]
	mov r0, #2
	mov r1, r0
	strh r2, [sb]
	bl AddTask
	b _06016BBC
_060168B8:
	ldrh r0, [r4, #0xc]
	cmp r0, #1
	bne _06016A58
	mov r0, r8, lsl #0x10
	mov r0, r0, lsr #0x10
	mov r1, #0x20
	bl CAM_SetStaState
	ldrh r0, [sl, #0x2e]
	cmp r0, #1
	bne _06016974
	add r0, sl, #0x1e
	mov r1, #0x80
	mov r2, #1
	bl MakeAuthFrame
	movs r4, r0
	beq _06016BBC
	ldrh r1, [sl, #0x2c]
	ldr r0, _06016C10 ; =0x04808044
	strh r1, [r4, #0x2c]
	strh r6, [r4, #0x2e]
	mov r1, #0
	strh r1, [r4, #0x30]
	ldrh r1, [r0]
	ldrh r0, [r0]
	add r0, r1, r0, lsl #8
	mov r0, r0, lsl #0x10
	movs sb, r0, lsr #0x10
	moveq sb, #1
	mov r0, sb
	bl RND_seed
	mov r0, r8
	mov r1, sb
	bl CAM_SetAuthSeed
	add r0, r4, #0x33
	add sb, r4, #0x34
	bl WL_ReadByte
	mov fp, r0
	mov r8, #0
	b _06016960
_06016954:
	bl RND_rand
	strh r0, [sb], #2
	add r8, r8, #2
_06016960:
	cmp r8, fp
	blo _06016954
	mov r0, r4
	bl TxManCtrlFrame
	b _06016BBC
_06016974:
	cmp r0, #3
	mov r0, r8
	bne _06016A40
	bl CAM_GetStaState
	cmp r0, #0x20
	bne _0601699C
	mov r0, r8
	bl CAM_GetAuthSeed
	cmp r0, #0
	bne _060169A8
_0601699C:
	mov r7, #1
	mov r5, r7
	b _06016BBC
_060169A8:
	ldrh r0, [sl, #2]
	bl CAM_GetAuthSeed
	bl RND_seed
	add r0, sl, #0x33
	add r7, sl, #0x34
	bl WL_ReadByte
	mov r4, r0
	mov r5, #0
	b _060169E4
_060169CC:
	bl RND_rand
	ldrh r1, [r7], #2
	cmp r1, r0
	movne r0, #0
	bne _06016A14
	add r5, r5, #1
_060169E4:
	cmp r5, r4, lsr #1
	blo _060169CC
	tst r4, #1
	beq _06016A10
	bl RND_rand
	ldrh r1, [r7]
	and r0, r0, #0xff
	and r1, r1, #0xff
	cmp r1, r0
	movne r0, #0
	bne _06016A14
_06016A10:
	mov r0, #1
_06016A14:
	cmp r0, #0
	mov r0, r8
	mov r1, #0
	mov r5, #1
	bne _06016A34
	mov r7, #0xf
	bl CAM_SetAuthSeed
	b _06016BBC
_06016A34:
	bl CAM_SetAuthSeed
	mov r7, #0
	b _06016BBC
_06016A40:
	mov r1, #0
	bl CAM_SetAuthSeed
	mov r7, #0xe
	mov r6, #2
	mov r5, #1
	b _06016BBC
_06016A58:
	ldr r1, [sb, #0x18]
	ldrh r0, [r1, #0x16]
	cmp r0, #1
	bne _06016BBC
	add r0, r1, #0x10
	add r1, sl, #0x1e
	bl MatchMacAdrs
	cmp r0, #0
	beq _06016BBC
	ldrh r0, [sl, #0x2e]
	cmp r0, #2
	bne _06016B48
	ldrh r0, [sb]
	cmp r0, #0x31
	bne _06016BBC
	ldrh r0, [sl, #0x30]
	cmp r0, #0
	beq _06016ADC
	bl ClearTimeOut
	mov r0, #0x35
	strh r0, [sb]
	ldr r0, [sb, #0x1c]
	mov r1, #0xc
	strh r1, [r0, #4]
	mov r0, #2
	ldrh r3, [sl, #0x30]
	ldr r2, [sb, #0x1c]
	mov r1, r0
	strh r3, [r2, #6]
	bl AddTask
	mov r0, #0x20
	bl WSetStaState
	b _06016BBC
_06016ADC:
	mov r1, #0x33
	add r0, sl, #0x33
	strh r1, [sb]
	bl WL_ReadByte
	mov r1, r0
	add r0, sl, #0x1e
	mov r2, #1
	bl MakeAuthFrame
	movs r4, r0
	beq _06016BBC
	ldrh r1, [r4, #0x14]
	add r0, sl, #0x2c
	orr r1, r1, #0x4000
	strh r1, [r4, #0x14]
	ldrh r2, [sl, #6]
	add r1, r4, #0x2c
	add r2, r2, #1
	bl __VENEER_MIi_CpuCopy16
	ldrh r1, [sl, #0x2c]
	mov r0, #3
	strh r1, [r4, #0x2c]
	strh r0, [r4, #0x2e]
	mov r1, #0
	mov r0, r4
	strh r1, [r4, #0x30]
	bl TxManCtrlFrame
	b _06016BBC
_06016B48:
	cmp r0, #4
	ldreqh r0, [sb]
	cmpeq r0, #0x33
	bne _06016BBC
	bl ClearTimeOut
	ldrh r0, [sl, #0x30]
	cmp r0, #0
	ldrne r0, [sb, #0x1c]
	movne r1, #0xc
	strneh r1, [r0, #4]
	ldrneh r1, [sl, #0x30]
	bne _06016B8C
	mov r0, #0x30
	bl WSetStaState
	ldr r0, [sb, #0x1c]
	mov r1, #0
	strh r1, [r0, #4]
_06016B8C:
	ldr r0, [sb, #0x1c]
	mov r2, #0x35
	strh r1, [r0, #6]
	mov r0, #2
	mov r1, r0
	strh r2, [sb]
	bl AddTask
	b _06016BBC
_06016BAC:
	ldrh r0, [r4, #0xc]
	cmp r0, #1
	moveq r7, #0xd
	moveq r5, #1
_06016BBC:
	cmp r5, #0
	beq _06016BF8
	cmp r7, #0
	movne r2, #1
	moveq r2, #0
	add r0, sl, #0x1e
	mov r1, #0
	bl MakeAuthFrame
	cmp r0, #0
	beq _06016BF8
	ldrh r1, [sl, #0x2c]
	strh r1, [r0, #0x2c]
	strh r6, [r0, #0x2e]
	strh r7, [r0, #0x30]
	bl TxManCtrlFrame
_06016BF8:
	ldmia sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_06016C00: .word 0x0380FFF4
_06016C04: .word 0x048080B0
_06016C08: .word 0x0480819C
_06016C0C: .word 0x04808032
_06016C10: .word 0x04808044
	arm_func_end RxAuthFrame
