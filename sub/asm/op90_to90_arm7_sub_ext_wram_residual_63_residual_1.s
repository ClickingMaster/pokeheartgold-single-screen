	.include "asm/macros.inc"
	.include "global.inc"
	.extern AddTask
	.extern AllocateHeapBuf
	.extern CAM_GetAID
	.extern CAM_GetLastSeqCtrl
	.extern CAM_GetStaState
	.extern CAM_Search
	.extern CAM_SearchAdd
	.extern CAM_SetAwake
	.extern CAM_SetCapaInfo
	.extern CAM_SetLastSeqCtrl
	.extern CAM_SetPowerMgtMode
	.extern CAM_SetRSSI
	.extern CAM_SetStaState
	.extern CAM_SetSupRate
	.extern CAM_UpdateLifeTime
	.extern ClearTimeOut
	.extern ElementChecker
	.extern IsExistManFrame
	.extern MI_WaitDma
	.extern MLME_IssueDisAssIndication
	.extern MakeDeAuthFrame
	.extern MakePsPollFrame
	.extern MakeReAssResFrame
	.extern MatchMacAdrs
	.extern MoveHeapBuf
	.extern ReleaseHeapBuf
	.extern RxAssReqFrame
	.extern RxAuthFrame
	.extern RxBeaconFrame
	.extern RxDeAuthFrame
	.extern RxDisAssFrame
	.extern RxProbeReqFrame
	.extern RxProbeResFrame
	.extern SetFatalErr
	.extern TxManCtrlFrame
	.extern TxqPri
	.extern WSetAids
	.extern WSetMacAdrs1
	.extern WSetStaState
	.extern __VENEER_MIi_CpuClear32
	.extern __VENEER_MIi_CpuCopy16
	.extern __VENEER__u32_div_f

.text
	arm_func_start DefragTask
DefragTask: ; 0x0601769C
	stmdb sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0x18
	ldr r0, _06017BB4 ; =0x0380FFF4
	mvn r1, #0
	ldr r2, [r0]
	add r0, r2, #0x17c
	ldr r5, [r0, #0x6c]
	str r0, [sp, #4]
	cmp r5, r1
	beq _06017BA8
	add r0, r2, #0x300
	ldrh r0, [r0, #0x4c]
	cmp r0, #0x40
	bne _06017B7C
	ldrh r1, [r5, #0x20]
	ldr r0, _06017BB8 ; =0x000005FC
	cmp r1, r0
	bhi _06017B7C
	ldrh r4, [r5, #0x24]
	add r0, sp, #8
	tst r4, #0x100
	beq _06017714
	add r1, r5, #0x34
	bl WSetMacAdrs1
	tst r4, #0x200
	bne _06017B7C
	add r0, sp, #0xe
	add r1, r5, #0x2e
	bl WSetMacAdrs1
	b _0601773C
_06017714:
	add r1, r5, #0x28
	bl WSetMacAdrs1
	tst r4, #0x200
	add r0, sp, #0xe
	beq _06017734
	add r1, r5, #0x34
	bl WSetMacAdrs1
	b _0601773C
_06017734:
	add r1, r5, #0x2e
	bl WSetMacAdrs1
_0601773C:
	ldrh r0, [r5, #0x3a]
	tst r4, #0x400
	strh r0, [sp, #0x14]
	beq _06017974
	ldrh r0, [r5, #0x3a]
	mov r0, r0, lsl #0x1c
	movs r0, r0, lsr #0x1c
	bne _06017974
	ldr r0, _06017BB4 ; =0x0380FFF4
	mvn r6, #0
	ldr r0, [r0]
	mov r7, #0
	add r0, r0, #0xe4
	add r8, r0, #0x400
	add fp, sp, #8
	mov r4, #0x18
	b _06017884
_06017780:
	mul sb, r7, r4
	ldrh r0, [r8, sb]
	add sl, r8, sb
	cmp r0, #0
	beq _0601787C
	mov r1, fp
	add r0, sl, #4
	bl MatchMacAdrs
	cmp r0, #0
	beq _06017880
	add r1, sp, #0xe
	add r0, sl, #0xa
	bl MatchMacAdrs
	cmp r0, #0
	beq _06017880
	ldrh r0, [sp, #0x14]
	ldrh r1, [sl, #0x10]
	mov r0, r0, lsl #0x10
	mov r2, r0, lsr #0x14
	mov r0, r1, lsl #0x10
	cmp r2, r0, lsr #20
	bne _06017880
	ldrh r0, [r5, #0x18]
	mov r1, r1, lsl #0x1c
	and r2, r0, #0xf0
	mov r0, r2, asr #3
	add r0, r2, r0, lsr #28
	mov r7, r0, asr #4
	subs r0, r7, r1, lsr #28
	beq _06017B7C
	tst r0, #-0x80000000
	bne _06017B7C
	mov r0, sl
	ldr r4, [r0, #0x14]
	ldrh r0, [r5, #0x20]
	ldrh r3, [r4, #0x20]
	sub r0, r0, r3
	subs r6, r0, #0x18
	beq _06017B7C
	tst r6, #-0x80000000
	bne _06017B7C
	add r0, r5, #0x3c
	add r1, r4, #0x3c
	mov r2, r6
	add r0, r0, r3
	add r1, r1, r3
	bl __VENEER_MIi_CpuCopy16
	ldrh r1, [r4, #0x20]
	mov r0, r7, lsl #0x10
	add r1, r1, r6
	strh r1, [r4, #0x20]
	ldrh r1, [sl, #0x10]
	mov r0, r0, lsr #0x10
	bic r1, r1, #0xf
	and r0, r0, #0xf
	orr r1, r1, r0
	ldr r0, _06017BB4 ; =0x0380FFF4
	strh r1, [sl, #0x10]
	ldr r1, [r0]
	ldr r0, [r1, #0x560]
	add r0, r0, r7
	str r0, [r1, #0x560]
	b _06017B7C
_0601787C:
	mov r6, r7
_06017880:
	add r7, r7, #1
_06017884:
	cmp r7, #3
	blo _06017780
	mvn r0, #0
	cmp r6, r0
	beq _06017B7C
	ldr r0, _06017BB4 ; =0x0380FFF4
	ldr r1, _06017BBC ; =0x00000622
	ldr r0, [r0]
	add r0, r0, #0x188
	bl AllocateHeapBuf
	movs r4, r0
	beq _06017968
	mov r0, #0x18
	mul r7, r6, r0
	add r6, r8, r7
	add r0, sp, #8
	add r1, r6, #4
	mov r2, #0x10
	bl __VENEER_MIi_CpuCopy16
	mov r0, #5
	strh r0, [r8, r7]
	mov r0, r6
	str r4, [r0, #0x14]
	ldrh r2, [r5, #0x20]
	add r0, r5, #0x18
	add r1, r4, #0x18
	add r2, r2, #0xc
	bl __VENEER_MIi_CpuCopy16
	ldr r0, _06017BB4 ; =0x0380FFF4
	ldr r0, [r0]
	ldr r0, [r0, #0x30c]
	bl __VENEER_MI_WaitDma
	ldrh r1, [r4, #0x18]
	ldrh r0, [r6, #0x10]
	and r1, r1, #0xf0
	bic r3, r0, #0xf
	mov r0, r1, asr #3
	add r0, r1, r0, lsr #28
	mov r1, r0, asr #4
	mov r2, r1, lsl #0x10
	mov r2, r2, lsr #0x10
	and r2, r2, #0xf
	orr r2, r3, r2
	strh r2, [r6, #0x10]
	ldr r2, _06017BB4 ; =0x0380FFF4
	ldr r3, [r2]
	ldr r2, [r3, #0x560]
	add r0, r2, r0, asr #4
	str r0, [r3, #0x560]
	ldrh r0, [r5, #0x20]
	sub r0, r0, #0x18
	strh r0, [r4, #0x20]
	ldrh r0, [r4, #0x20]
	bl __VENEER__u32_div_f
	mov r1, r6
	strh r0, [r1, #2]
	b _06017B7C
_06017968:
	mov r0, #4
	bl SetFatalErr
	b _06017B7C
_06017974:
	ldr r1, _06017BB4 ; =0x0380FFF4
	ldrh r0, [r5, #0x20]
	ldr r4, [r1]
	sub r1, r0, #0x18
	add r0, r4, #0xe4
	strh r1, [r5, #0x20]
	add r8, r0, #0x400
	mov r7, #0
	b _06017A30
_06017998:
	mov r0, #0x18
	mul sb, r7, r0
	ldrh r0, [r8, sb]
	add sl, r8, sb
	cmp r0, #0
	beq _06017A2C
	add r1, sp, #8
	add r0, sl, #4
	bl MatchMacAdrs
	cmp r0, #0
	beq _06017A2C
	add r1, sp, #0xe
	add r0, sl, #0xa
	bl MatchMacAdrs
	cmp r0, #0
	beq _06017A2C
	ldrh r2, [sp, #0x14]
	ldrh r0, [sl, #0x10]
	mov r1, r2, lsl #0x10
	mov r1, r1, lsr #0x14
	mov r0, r0, lsl #0x10
	cmp r1, r0, lsr #20
	bne _06017A2C
	mov r1, sl
	ldrh r0, [r1, #0x10]
	sub fp, r0, r2
	tst fp, #-0x80000000
	bne _06017B7C
	ldrh r0, [r1, #2]
	ldrh r1, [r5, #0x20]
	mul r0, fp, r0
	str r0, [sp]
	subs r6, r1, r0
	beq _06017B7C
	tst r6, #-0x80000000
	beq _06017A38
	b _06017B7C
_06017A2C:
	add r7, r7, #1
_06017A30:
	cmp r7, #3
	blo _06017998
_06017A38:
	cmp r7, #3
	beq _06017B7C
	mov r0, #0x18
	mul sb, r7, r0
	add r0, r8, sb
	ldr r1, [r0, #0x14]
	ldr r0, _06017BC0 ; =0x000005E4
	add r7, r1, #0x10
	ldrh r1, [r7, #0x10]
	add sl, r1, r6
	cmp sl, r0
	bls _06017A80
	add r0, r4, #0x188
	sub r1, r7, #0x10
	bl ReleaseHeapBuf
	mov r0, #0
	strh r0, [r8, sb]
	b _06017B7C
_06017A80:
	ldr r0, [sp]
	add r3, r5, #0x3c
	add r2, r7, #0x2c
	add r1, r2, r1
	add r0, r3, r0
	add r2, r6, #1
	bl __VENEER_MIi_CpuCopy16
	strh sl, [r7, #0x10]
	ldrh r1, [r5, #0x18]
	add r0, r8, sb
	ldrh r6, [r0, #0x10]
	and r2, r1, #0xf0
	mov r1, r2, asr #3
	add r1, r2, r1, lsr #28
	mov r3, r6, lsl #0x1c
	rsb r2, fp, r1, asr #4
	add r2, r2, r3, lsr #28
	mov r2, r2, lsl #0x10
	mov r2, r2, lsr #0x10
	bic r3, r6, #0xf
	and r2, r2, #0xf
	orr r3, r3, r2
	ldr r2, _06017BB4 ; =0x0380FFF4
	strh r3, [r0, #0x10]
	ldr r3, [r2]
	ldr r2, [r3, #0x560]
	add r1, r2, r1, asr #4
	str r1, [r3, #0x560]
	ldrh r1, [r5, #0x18]
	tst r1, #0x100
	bne _06017B7C
	mov r1, #0
	strh r1, [r0]
	ldrh r0, [r7, #8]
	bic r0, r0, #0xf0
	add r0, r0, #0x10
	strh r0, [r7, #8]
	ldrh r0, [r7, #0x10]
	add r0, r0, #0x18
	strh r0, [r7, #0x10]
	ldrh r0, [r7, #8]
	ands r0, r0, #0xf
	beq _06017B54
	cmp r0, #8
	add r0, r4, #0x188
	bne _06017B74
	add r1, r4, #0x1c4
	sub r2, r7, #0x10
	bl MoveHeapBuf
	mov r0, #2
	mov r1, #6
	bl AddTask
	b _06017B7C
_06017B54:
	add r0, r4, #0x188
	add r1, r4, #0x1dc
	sub r2, r7, #0x10
	bl MoveHeapBuf
	mov r0, #1
	mov r1, #7
	bl AddTask
	b _06017B7C
_06017B74:
	sub r1, r7, #0x10
	bl ReleaseHeapBuf
_06017B7C:
	ldr r0, [sp, #4]
	mov r1, r5
	add r0, r0, #0x6c
	bl ReleaseHeapBuf
	ldr r0, [sp, #4]
	ldrh r0, [r0, #0x74]
	cmp r0, #0
	beq _06017BA8
	mov r0, #2
	mov r1, #9
	bl AddTask
_06017BA8:
	add sp, sp, #0x18
	ldmia sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_06017BB4: .word 0x0380FFF4
_06017BB8: .word 0x000005FC
_06017BBC: .word 0x00000622
_06017BC0: .word 0x000005E4
	arm_func_end DefragTask

	arm_func_start __VENEER_MI_WaitDma
__VENEER_MI_WaitDma: ; 0x06017BC4
	ldr pc, _06017BC8 ; =MI_WaitDma
	.align 2, 0
_06017BC8: .word MI_WaitDma
	arm_func_end __VENEER_MI_WaitDma
