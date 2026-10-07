	.include "asm/macros.inc"
	.include "global.inc"

	.extern AddTask
	.extern CAM_GetAID
	.extern CAM_GetLastSeqCtrl
	.extern CAM_GetStaState
	.extern CAM_Search
	.extern CAM_SearchAdd
	.extern CAM_SetLastSeqCtrl
	.extern CAM_SetPowerMgtMode
	.extern CAM_SetRSSI
	.extern CAM_SetSupRate
	.extern CAM_UpdateLifeTime
	.extern ClearTimeOut
	.extern ElementChecker
	.extern IsExistManFrame
	.extern MLME_IssueBeaconRecvIndication
	.extern MakeDeAuthFrame
	.extern MakeDisAssFrame
	.extern MatchMacAdrs
	.extern ReleaseHeapBuf
	.extern RxProbeResFrame
	.extern SendMessageToWmDirect
	.extern TxManCtrlFrame
	.extern TxPsPollFrame
	.extern UpdateApList
	.extern WL_ReadByte
	.extern WL_WriteByte
	.extern WSetActiveZoneTime
	.extern WSetBeaconPeriod
	.extern WSetChannel
	.extern WSetDTIMPeriod
	.extern WSetMacAdrs1
	.extern WSetPowerState
	.extern __VENEER_MIi_CpuClear32
	.extern __VENEER_MIi_CpuCopy16
	.extern __VENEER_OS_DisableInterrupts
	.extern __VENEER_OS_RestoreInterrupts
	.extern __VENEER__ll_udiv

.text

	arm_func_start RxMpFrame
RxMpFrame: ; 0x060154F8
	stmdb sp!, {r4, r5, r6, lr}
	ldr r1, _06015664 ; =0x0380FFF4
	mov r6, r0
	ldr r0, [r1]
	add r4, r0, #0x344
	ldrh r1, [r4, #8]
	add r0, r0, #0xdc
	cmp r1, #0x40
	add r5, r0, #0x400
	movne r0, #1
	bne _0601565C
	add r0, r6, #0x1e
	add r1, r4, #0x64
	bl MatchMacAdrs
	cmp r0, #0
	beq _0601554C
	add r0, r6, #0x24
	add r1, r4, #0x82
	bl MatchMacAdrs
	cmp r0, #0
	bne _06015554
_0601554C:
	mov r0, #1
	b _0601565C
_06015554:
	ldrh r0, [r4, #0x6a]
	mov r1, #1
	mov r0, r1, lsl r0
	ldrh r1, [r6, #0x2e]
	mov r0, r0, lsl #0x10
	tst r1, r0, lsr #16
	moveq r0, #0
	movne r0, #0x2000
	strh r0, [r5, #6]
	ldr r0, _06015668 ; =0x04808098
	ldrh r0, [r0]
	tst r0, #0x8000
	beq _060155AC
	mov r0, r0, lsl #0x11
	mov r0, r0, lsr #0x10
	add r0, r0, #0x4800000
	add r0, r0, #0x4000
	ldrh r0, [r0, #4]
	cmp r0, #0
	ldrneh r0, [r5, #6]
	orrne r0, r0, #0x4000
	strneh r0, [r5, #6]
_060155AC:
	ldrh r0, [r4, #0x88]
	bl CAM_UpdateLifeTime
	ldrh r1, [r6, #0x10]
	ldr r0, _0601566C ; =0x00000182
	sub r1, r1, #0x1c
	strh r1, [r6, #6]
	sub r1, r6, #0x10
	strh r0, [r1, #0xc]
	ldrh r2, [r6, #6]
	mov r0, #0
	add r2, r2, #0x31
	mov r2, r2, lsr #1
	strh r2, [r1, #0xe]
	ldrh r2, [r1, #0x3e]
	b _060155F8
_060155E8:
	tst r2, #1
	mov r2, r2, lsl #0xf
	addne r0, r0, #1
	mov r2, r2, lsr #0x10
_060155F8:
	cmp r2, #0
	bne _060155E8
	ldr r2, _06015670 ; =0x04808094
	ldrh r3, [r5, #6]
	ldrh r4, [r2]
	ldrh r2, [r2, #4]
	ldrh r5, [r1, #0x18]
	and r2, r2, #0x8000
	and r4, r4, #0x8000
	orr r2, r3, r2, asr #4
	orr r2, r2, r4, asr #3
	orr r2, r5, r2
	strh r2, [r1, #0x18]
	ldrh r2, [r1, #0x3c]
	ldrh r3, [r1, #0x1c]
	add r2, r2, #0xa
	mul r2, r0, r2
	add r0, r2, #0xfc
	add r2, r3, r0, lsr #4
	ldr r0, _06015664 ; =0x0380FFF4
	strh r2, [r1, #0x1a]
	ldr r0, [r0]
	add r0, r0, #0x188
	bl SendMessageToWmDirect
	mov r0, #0
_0601565C:
	ldmia sp!, {r4, r5, r6, lr}
	bx lr
	.align 2, 0
_06015664: .word 0x0380FFF4
_06015668: .word 0x04808098
_0601566C: .word 0x00000182
_06015670: .word 0x04808094
	arm_func_end RxMpFrame

	arm_func_start RxKeyDataFrame
RxKeyDataFrame: ; 0x06015674
	stmdb sp!, {r3, r4, r5, r6, r7, lr}
	ldr r1, _0601581C ; =0x0380FFF4
	mov r7, r0
	ldr r1, [r1]
	add r0, r1, #0x2c
	add r5, r0, #0x400
	ldrh r0, [r5, #0x3c]
	ldr r4, [r5, #0x90]
	cmp r0, #0
	beq _06015814
	add r0, r7, #0x18
	add r1, r1, #0x3a8
	bl MatchMacAdrs
	cmp r0, #0
	beq _06015814
	ldrh r1, [r7, #0x10]
	ldrh r0, [r4, #0x16]
	sub r1, r1, #0x18
	sub r0, r0, #8
	cmp r1, r0
	bgt _06015814
	add r0, r7, #0x1e
	bl CAM_Search
	mov r6, r0
	cmp r6, #0xff
	beq _060156F0
	cmp r6, #0
	beq _0601572C
	bl CAM_GetStaState
	cmp r0, #0x40
	beq _0601572C
_060156F0:
	add r0, r7, #0x1e
	mov r1, #0xc0
	bl IsExistManFrame
	cmp r0, #0
	bne _06015814
	add r0, r7, #0x1e
	mov r1, #7
	mov r2, #0
	bl MakeDeAuthFrame
	cmp r0, #0
	beq _06015814
	mov r1, #2
	strh r1, [r0]
	bl TxManCtrlFrame
	b _06015814
_0601572C:
	cmp r6, #0
	beq _06015814
	ldrh r1, [r7, #0x14]
	mov r0, r6, lsl #0x10
	mov r1, r1, lsl #0x13
	mov r0, r0, lsr #0x10
	mov r1, r1, lsr #0x1f
	bl CAM_SetPowerMgtMode
	mov r0, r6
	bl CAM_UpdateLifeTime
	mov r0, r6
	bl CAM_GetAID
	mov r1, #1
	mov r0, r1, lsl r0
	mov r0, r0, lsl #0x10
	ldrh r1, [r5, #0x9a]
	mov r2, r0, lsr #0x10
	tst r2, r1
	bne _06015814
	ldrh r0, [r5, #0x98]
	tst r2, r0
	beq _06015814
	orr r0, r1, r2
	strh r0, [r5, #0x9a]
	ldrh r1, [r4, #0x10]
	mvn r0, r2
	and r1, r1, r0
	mov r0, r2, lsl #0xf
	strh r1, [r4, #0x10]
	add r6, r4, #0x1a
	mov r1, r0, lsr #0x10
	b _060157C4
_060157AC:
	ldrh r0, [r5, #0x98]
	tst r1, r0
	ldrneh r0, [r4, #0x16]
	addne r6, r6, r0
	mov r0, r1, lsl #0xf
	mov r1, r0, lsr #0x10
_060157C4:
	cmp r1, #1
	bne _060157AC
	ldrh r1, [r7, #0x10]
	add r0, r6, #3
	sub r1, r1, #0x18
	strh r1, [r6]
	ldrh r1, [r7, #0x12]
	and r1, r1, #0xff
	bl WL_WriteByte
	ldrh r1, [r7, #0xe]
	add r0, r6, #2
	and r1, r1, #0xff
	bl WL_WriteByte
	ldrh r2, [r6]
	cmp r2, #0
	beq _06015814
	add r0, r7, #0x2c
	add r1, r6, #8
	add r2, r2, #1
	bl __VENEER_MIi_CpuCopy16
_06015814:
	ldmia sp!, {r3, r4, r5, r6, r7, lr}
	bx lr
	.align 2, 0
_0601581C: .word 0x0380FFF4
	arm_func_end RxKeyDataFrame

	arm_func_start RxMpAckFrame
RxMpAckFrame: ; 0x06015820
	stmdb sp!, {r3, r4, r5, lr}
	ldr r1, _060158E8 ; =0x0380FFF4
	mov r4, r0
	ldr r1, [r1]
	add r0, r1, #0x300
	ldrh r0, [r0, #0x4c]
	add r5, r1, #0x344
	cmp r0, #0x40
	movne r0, #1
	bne _060158E0
	add r0, r4, #0x1e
	add r1, r5, #0x64
	bl MatchMacAdrs
	cmp r0, #0
	beq _06015870
	add r0, r4, #0x24
	add r1, r5, #0x82
	bl MatchMacAdrs
	cmp r0, #0
	bne _06015878
_06015870:
	mov r0, #1
	b _060158E0
_06015878:
	ldrh r1, [r4, #0x10]
	ldr r0, _060158EC ; =0x00000185
	sub r1, r1, #0x1c
	strh r1, [r4, #6]
	sub r1, r4, #0x10
	strh r0, [r1, #0xc]
	mov r0, #0x18
	strh r0, [r1, #0xe]
	ldr r4, _060158E8 ; =0x0380FFF4
	ldr r2, _060158F0 ; =0x04808094
	ldr r0, [r4]
	ldrh ip, [r2]
	add r0, r0, #0x400
	ldrh r2, [r2, #4]
	ldrh r3, [r0, #0xe2]
	and r0, r2, #0x8000
	ldrh lr, [r1, #0x18]
	and r2, ip, #0x8000
	orr r0, r3, r0, asr #4
	orr r0, r0, r2, asr #3
	orr r0, lr, r0
	strh r0, [r1, #0x18]
	ldr r0, [r4]
	add r0, r0, #0x188
	bl SendMessageToWmDirect
	mov r0, #0
_060158E0:
	ldmia sp!, {r3, r4, r5, lr}
	bx lr
	.align 2, 0
_060158E8: .word 0x0380FFF4
_060158EC: .word 0x00000185
_060158F0: .word 0x04808094
	arm_func_end RxMpAckFrame

	arm_func_start RxBeaconFrame
RxBeaconFrame: ; 0x060158F4
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0x44
	ldr r1, _06015F1C ; =0x0380FFF4
	mov sl, r0
	ldr r3, [r1]
	add r0, sl, #0x1e
	ldr r2, [r3, #0x570]
	add r1, r3, #4
	add r2, r2, #1
	str r2, [r3, #0x570]
	add r4, r3, #0x344
	add r5, r1, #0x400
	add r6, r3, #0x31c
	add r7, r3, #0x17c
	bl CAM_SearchAdd
	mov r8, r0
	mov r0, r8, lsl #0x10
	mov r0, r0, lsr #0x10
	strh r0, [sl, #2]
	cmp r8, #0xff
	beq _06015F10
	ldrh r1, [sl, #0x12]
	bl CAM_SetRSSI
	ldrh sb, [sl, #6]
	cmp sb, #0xc
	bls _06015F10
	add r1, sp, #0x18
	mov r0, #0
	mov r2, #0x2c
	bl __VENEER_MIi_CpuClear32
	add r2, sl, #0x38
	sub r1, sb, #0xc
	mov r0, #2
	str r2, [sp, #0x18]
	strh r1, [sp, #0x20]
	strh r0, [sp, #0x22]
	ldrh r0, [r4, #0x1e]
	cmp r0, #0
	ldreqh r0, [sp, #0x22]
	orreq r0, r0, #1
	streqh r0, [sp, #0x22]
	mov r0, #0x38
	strh r0, [sp, #0x24]
	ldrh r1, [sl, #8]
	add r0, sp, #0x18
	strh r1, [sp, #0x1c]
	ldrh r1, [sl, #0x36]
	strh r1, [sp, #0x1e]
	bl ElementChecker
	ldr sb, [sp, #0x38]
	cmp sb, #0
	beq _060159F0
	ldrh r0, [sl, #0x16]
	tst r0, #0x8000
	beq _060159F0
	add r0, sb, #6
	bl WL_ReadByte
	mov fp, r0
	add r0, sb, #7
	bl WL_ReadByte
	ldr r1, _06015F20 ; =0x0480810C
	add r0, fp, r0, lsl #8
	strh r0, [r1]
_060159F0:
	ldrh r1, [r5]
	cmp r1, #0x13
	ldreq r0, [r5, #0x18]
	ldreqh r0, [r0, #0x38]
	cmpeq r0, #1
	bne _06015A28
	ldrh r0, [sp, #0x22]
	and r0, r0, #9
	cmp r0, #9
	bne _06015EF8
	add r1, sp, #0x18
	mov r0, sl
	bl RxProbeResFrame
	b _06015EF8
_06015A28:
	ldrh r0, [sp, #0x22]
	tst r0, #8
	beq _06015EF8
	cmp r1, #0x21
	bne _06015BB0
	bl ClearTimeOut
	ldrh r0, [sp, #0x22]
	and r0, r0, #0x30
	cmp r0, #0x30
	movne r0, #0xc
	strneh r0, [r5, #4]
	movne r0, #0xa
	strneh r0, [r5, #6]
	bne _06015B8C
	ldrh r0, [sl, #0x34]
	cmp r0, #0x3e8
	movhi r0, #0xc
	strhih r0, [r5, #4]
	movhi r0, #1
	strhih r0, [r5, #6]
	bhi _06015B8C
	mov r1, #0
	strh r1, [r5, #4]
	ldrh r0, [sp, #0x24]
	tst r0, #2
	beq _06015AA4
	ldrh r0, [sp, #0x22]
	tst r0, #2
	bne _06015AA4
	ldrh r0, [sp, #0x2a]
	bl WSetChannel
_06015AA4:
	ldrh r1, [sp, #0x2e]
	mov r0, r8
	bl CAM_SetSupRate
	ldrh r0, [r4, #0xc]
	cmp r0, #2
	bne _06015B34
	ldr sb, [sp, #0x40]
	cmp sb, #0
	beq _06015B1C
	add r0, sb, #6
	bl WL_ReadByte
	mov fp, r0
	add r0, sb, #7
	bl WL_ReadByte
	add r0, fp, r0, lsl #8
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	mov r1, #1
	bl WSetActiveZoneTime
	ldr r0, [sp, #0x40]
	add r0, r0, #8
	bl WL_ReadByte
	ldr r1, [sp, #0x40]
	mov sb, r0
	add r0, r1, #9
	bl WL_ReadByte
	add r1, sb, r0, lsl #8
	ldr r0, _06015F24 ; =0x0380FFF0
	strh r1, [r0]
	b _06015B34
_06015B1C:
	ldr r0, _06015F28 ; =0x0000FFFF
	mov r1, #1
	bl WSetActiveZoneTime
	ldr r0, _06015F24 ; =0x0380FFF0
	mov r1, #0
	strh r1, [r0]
_06015B34:
	ldr r0, [sp, #0x3c]
	add r0, r0, #3
	bl WL_ReadByte
	bl WSetDTIMPeriod
	ldr r0, [sp, #0x3c]
	add r0, r0, #2
	bl WL_ReadByte
	strh r0, [r4, #0x76]
	ldrh r0, [sl, #0x34]
	bl WSetBeaconPeriod
	mov r0, #1
	strh r0, [r4, #0x12]
	strh r0, [r4, #0x1a]
	ldrh r0, [r4, #0xc]
	cmp r0, #2
	ldreq r0, _06015F2C ; =0x04808048
	moveq r1, #3
	streqh r1, [r0]
	ldr r1, _06015F30 ; =0x04808038
	ldrh r0, [r1]
	orr r0, r0, #1
	strh r0, [r1]
_06015B8C:
	ldr r0, [r5, #0x1c]
	add r1, sl, #0x1e
	add r0, r0, #8
	bl WSetMacAdrs1
	mov r2, #0x25
	mov r0, #2
	mov r1, #1
	strh r2, [r5]
	bl AddTask
_06015BB0:
	ldrh r0, [r4, #0xc]
	cmp r0, #2
	beq _06015BC8
	cmp r0, #3
	beq _06015C80
	b _06015EE0
_06015BC8:
	ldr sb, [sp, #0x40]
	cmp sb, #0
	beq _06015C80
	add r0, sb, #6
	bl WL_ReadByte
	mov r5, r0
	add r0, sb, #7
	bl WL_ReadByte
	add r0, r5, r0, lsl #8
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	mov r1, #0
	bl WSetActiveZoneTime
	ldr r0, [sp, #0x40]
	add r0, r0, #8
	bl WL_ReadByte
	ldr r1, [sp, #0x40]
	mov r5, r0
	add r0, r1, #9
	bl WL_ReadByte
	ldr r1, _06015F24 ; =0x0380FFF0
	add r0, r5, r0, lsl #8
	strh r0, [r1]
	ldr r0, [sp, #0x40]
	add r0, r0, #1
	bl WL_ReadByte
	sub r0, r0, #8
	strh r0, [r4, #0xa0]
	ldrh r2, [r4, #0xa0]
	cmp r2, #0
	beq _06015C80
	ldr r0, [sp, #0x40]
	ldr r1, [r4, #0x9c]
	tst r0, #1
	beq _06015C6C
	add r0, r0, #9
	add r2, r2, #2
	bl __VENEER_MIi_CpuCopy16
	mov r0, #1
	strh r0, [r4, #0xa2]
	b _06015C80
_06015C6C:
	add r0, r0, #0xa
	add r2, r2, #1
	bl __VENEER_MIi_CpuCopy16
	mov r0, #0
	strh r0, [r4, #0xa2]
_06015C80:
	mov r1, #0
	mov r0, r8
	strh r1, [r4, #0x80]
	bl CAM_UpdateLifeTime
	add r1, sl, #0x2c
	add r0, sp, #0x10
	ldmia r1, {r2, r3}
	stmia r0, {r2, r3}
	ldrh r0, [r4, #0x6e]
	mov r5, #0
	mov r8, r0, lsl #0xa
	ldr r0, [sp, #0x10]
	ldr r1, [sp, #0x14]
	mov r2, r8
	mov r3, r5
	bl __VENEER__ll_udiv
	adds fp, r0, #1
	umull r3, r0, fp, r8
	adc sb, r1, #0
	mla r0, fp, r5, r0
	mla r0, sb, r8, r0
	add r2, sp, #0x10
	str r0, [sp, #0x14]
	ldr r1, _06015F34 ; =0x048080F6
	ldrh r0, [r2, #6]
	str r3, [sp, #0x10]
	strh r0, [r1]
	ldrh r0, [r2, #4]
	strh r0, [r1, #-2]
	ldrh r0, [r2, #2]
	strh r0, [r1, #-4]
	ldrh r0, [r2]
	orr r0, r0, #1
	strh r0, [r1, #-6]
	ldrh r0, [r4, #0xc]
	cmp r0, #2
	bne _06015DE8
	ldrh r0, [r4, #0x1a]
	cmp r0, #0
	beq _06015DE8
	ldr r1, [sp, #0x10]
	ldr r0, [sp, #0x14]
	subs r1, r1, r8
	sbc r0, r0, r5
	str r1, [sp, #0x10]
	str r0, [sp, #0x14]
	bl __VENEER_OS_DisableInterrupts
	ldr r1, _06015F38 ; =0x048080F8
	ldrh ip, [r1]
	ldrh fp, [r1, #2]
	ldrh sb, [r1, #4]
	ldrh r8, [r1, #6]
	ldrh r5, [r1]
	ldrh r3, [r1, #2]
	ldrh r2, [r1, #4]
	ldrh r1, [r1, #6]
	strh ip, [sp, #8]
	strh r5, [sp]
	strh fp, [sp, #0xa]
	strh sb, [sp, #0xc]
	strh r8, [sp, #0xe]
	strh r3, [sp, #2]
	strh r2, [sp, #4]
	strh r1, [sp, #6]
	bl __VENEER_OS_RestoreInterrupts
	ldrh r1, [sp, #8]
	ldrh r0, [sp]
	cmp r1, r0
	bhs _06015DAC
	ldr r2, [sp, #8]
	ldr r0, [sp, #0x10]
	ldr r1, [sp, #0xc]
	subs r2, r2, r0
	ldr r0, [sp, #0x14]
	b _06015DC0
_06015DAC:
	ldr r2, [sp]
	ldr r0, [sp, #0x10]
	ldr r1, [sp, #4]
	subs r2, r2, r0
	ldr r0, [sp, #0x14]
_06015DC0:
	sbc r0, r1, r0
	mov r2, r2, lsr #0xa
	orr r2, r2, r0, lsl #22
	ldrh r1, [r6, #0x20]
	ldr r0, _06015F3C ; =0x04808134
	cmp r2, r1
	sublo r1, r1, r2
	strloh r1, [r0]
	movhs r1, #0
	strhsh r1, [r0]
_06015DE8:
	ldrh r0, [r4, #8]
	cmp r0, #0x40
	bne _06015EE0
	ldr r1, [sp, #0x3c]
	cmp r1, #0
	beq _06015EE0
	ldrh r0, [r4, #0xe]
	cmp r0, #1
	bne _06015EE0
	add r0, r1, #2
	bl WL_ReadByte
	ldrh r1, [r4, #0x76]
	cmp r1, r0
	strneh r0, [r4, #0x76]
	mov r1, #0
	strh r1, [r4, #0x8e]
	cmp r0, #0
	bne _06015E4C
	ldr r0, [sp, #0x3c]
	add r0, r0, #4
	bl WL_ReadByte
	tst r0, #1
	ldrneh r0, [r4, #0x8e]
	orrne r0, r0, #1
	strneh r0, [r4, #0x8e]
_06015E4C:
	ldr r0, [sp, #0x3c]
	add r0, r0, #4
	bl WL_ReadByte
	ldr r1, [sp, #0x3c]
	and r8, r0, #0xfe
	add r0, r1, #1
	mov r5, r8, lsl #3
	bl WL_ReadByte
	ldrh r1, [r4, #0x6a]
	add r0, r8, r0
	cmp r1, r8, lsl #3
	sub r0, r0, #3
	blo _06015EBC
	cmp r1, r0, lsl #3
	bhi _06015EBC
	ldr r0, [sp, #0x3c]
	sub r5, r1, r5
	add r0, r0, #5
	add r0, r0, r5, lsr #3
	bl WL_ReadByte
	and r1, r5, #7
	mov r2, #1
	tst r0, r2, lsl r1
	beq _06015EBC
	ldrh r0, [r4, #0x8e]
	orr r0, r0, #2
	strh r0, [r4, #0x8e]
	bl TxPsPollFrame
_06015EBC:
	ldrh r0, [r7, #0x20]
	cmp r0, #0
	ldreqh r0, [r7, #0x2c]
	cmpeq r0, #0
	ldreqh r0, [r4, #0x8e]
	cmpeq r0, #0
	bne _06015EE0
	mov r0, #1
	bl WSetPowerState
_06015EE0:
	ldrh r0, [r6, #0x1e]
	mov r0, r0, lsl #0x19
	movs r0, r0, lsr #0x1f
	beq _06015EF8
	mov r0, sl
	bl MLME_IssueBeaconRecvIndication
_06015EF8:
	ldr r2, [sp, #0x34]
	cmp r2, #0
	beq _06015F10
	ldrh r0, [sp, #0x2a]
	mov r1, sl
	bl UpdateApList
_06015F10:
	add sp, sp, #0x44
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_06015F1C: .word 0x0380FFF4
_06015F20: .word 0x0480810C
_06015F24: .word 0x0380FFF0
_06015F28: .word 0x0000FFFF
_06015F2C: .word 0x04808048
_06015F30: .word 0x04808038
_06015F34: .word 0x048080F6
_06015F38: .word 0x048080F8
_06015F3C: .word 0x04808134
	arm_func_end RxBeaconFrame
