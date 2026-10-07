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

	arm_func_start RxManCtrlTask
RxManCtrlTask: ; 0x06016FE8
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0x2c
	ldr r0, _06017690 ; =0x0380FFF4
	mvn r1, #0
	ldr r4, [r0]
	add sl, r4, #0x17c
	ldr r6, [sl, #0x60]
	add r0, r4, #0x300
	ldrh r5, [r0, #0x50]
	cmp r6, r1
	beq _06017684
	ldrh r0, [r6, #0x28]
	tst r0, #1
	ldrne r0, [r4, #0x568]
	addne r0, r0, #1
	strne r0, [r4, #0x568]
	ldreq r0, [r4, #0x564]
	addeq r0, r0, #1
	streq r0, [r4, #0x564]
	ldrh r0, [r6, #0x18]
	ldr r2, [r4, #0x560]
	and r1, r0, #0xf0
	mov r0, r1, asr #3
	add r0, r1, r0, lsr #28
	mov r0, r0, asr #4
	sub r0, r0, #1
	add r0, r2, r0
	str r0, [r4, #0x560]
	ldrh r1, [r6, #0x24]
	add r0, r6, #0x2e
	mov r2, r1, lsl #0x1c
	mov r1, r1, lsl #0x18
	mov r8, r2, lsr #0x1e
	mov sb, r1, lsr #0x1c
	bl CAM_SearchAdd
	mov r7, r0
	strh r7, [r6, #0x12]
	cmp r7, #0xff
	bne _060170D8
	mov r0, #0
	strh r0, [r6, #0x12]
	cmp r5, #1
	bne _06017660
	cmp r8, #0
	bne _06017660
	cmp sb, #0
	beq _060170CC
	cmp sb, #4
	beq _060170C0
	cmp sb, #0xb
	bne _06017660
	add r0, r6, #0x10
	bl RxAuthFrame
	b _06017660
_060170C0:
	add r0, r6, #0x10
	bl RxProbeReqFrame
	b _06017660
_060170CC:
	add r0, r6, #0x10
	bl RxAssReqFrame
	b _06017660
_060170D8:
	bl CAM_UpdateLifeTime
	ldrh r1, [r6, #0x22]
	mov r0, r7, lsl #0x10
	and r1, r1, #0xff
	mov r0, r0, lsr #0x10
	bl CAM_SetRSSI
	cmp r8, #0
	bne _06017124
	ldrh fp, [r6, #0x3a]
	mov r0, r7
	bl CAM_GetLastSeqCtrl
	cmp fp, r0
	ldreq r0, [r4, #0x578]
	addeq r0, r0, #1
	streq r0, [r4, #0x578]
	beq _06017660
	mov r0, r7
	mov r1, fp
	bl CAM_SetLastSeqCtrl
_06017124:
	cmp r5, #1
	beq _0601713C
	cmp r5, #2
	cmpne r5, #3
	beq _060173DC
	b _06017660
_0601713C:
	ldrh r1, [r6, #0x24]
	mov r0, r7, lsl #0x10
	mov r1, r1, lsl #0x13
	mov r0, r0, lsr #0x10
	mov r1, r1, lsr #0x1f
	bl CAM_SetPowerMgtMode
	cmp r8, #0
	bne _06017378
	cmp sb, #0xc
	addls pc, pc, sb, lsl #2
	b _06017660
_06017168: ; jump table
	b _060171A8 ; case 0
	b _06017660 ; case 1
	b _060171B4 ; case 2
	b _06017660 ; case 3
	b _06017338 ; case 4
	b _06017344 ; case 5
	b _06017660 ; case 6
	b _06017660 ; case 7
	b _0601719C ; case 8
	b _06017660 ; case 9
	b _06017354 ; case 10
	b _06017360 ; case 11
	b _0601736C ; case 12
_0601719C:
	add r0, r6, #0x10
	bl RxBeaconFrame
	b _06017660
_060171A8:
	add r0, r6, #0x10
	bl RxAssReqFrame
	b _06017660
_060171B4:
	ldr r0, _06017690 ; =0x0380FFF4
	ldrh r4, [r6, #0x16]
	ldr r5, [r0]
	cmp r4, #0xa
	bls _06017660
	add r0, r5, #0x300
	ldrh r0, [r0, #0x50]
	cmp r0, #1
	bne _06017660
	add r0, r6, #0x2e
	mov r1, #0x30
	bl IsExistManFrame
	cmp r0, #0
	bne _06017660
	ldrh r7, [r6, #0x12]
	mov r0, r7
	bl CAM_GetStaState
	cmp r0, #0x30
	bhs _06017234
	add r0, r6, #0x2e
	mov r1, #0xc0
	bl IsExistManFrame
	cmp r0, #0
	bne _06017660
	add r0, r6, #0x2e
	mov r1, #6
	mov r2, #1
	bl MakeDeAuthFrame
	cmp r0, #0
	beq _06017660
	bl TxManCtrlFrame
	b _06017660
_06017234:
	mov r0, r7
	bl CAM_GetStaState
	cmp r0, #0x40
	mov r0, r7
	bne _06017260
	mov r1, #0x30
	bl CAM_SetStaState
	add r0, r6, #0x2e
	mov r1, #1
	bl MLME_IssueDisAssIndication
	b _0601726C
_06017260:
	bl CAM_GetAID
	cmp r0, #0
	bne _06017660
_0601726C:
	add r1, sp, #0
	mov r0, #0
	mov r2, #0x2c
	bl __VENEER_MIi_CpuClear32
	add r3, r6, #0x46
	sub r2, r4, #0xa
	mov r1, #0x800
	str r3, [sp]
	strh r2, [sp, #8]
	strh r1, [sp, #0xc]
	add r0, sp, #0
	bl ElementChecker
	ldrh r1, [r6, #0x3c]
	ldr r0, _06017694 ; =0x0000FFC2
	tst r1, r0
	bne _060172E0
	add r0, r5, #0x300
	ldrh r2, [r0, #0x34]
	cmp r2, #0
	moveq r0, r1, lsl #0x1b
	moveq r0, r0, lsr #0x1f
	cmpeq r0, #1
	beq _060172E0
	cmp r2, #0
	beq _060172E8
	ldrh r0, [r6, #0x3c]
	mov r0, r0, lsl #0x1b
	movs r0, r0, lsr #0x1f
	bne _060172E8
_060172E0:
	mov r1, #0xa
	b _0601731C
_060172E8:
	mov r0, r7
	bl CAM_SetCapaInfo
	ldrh r0, [sp, #0xa]
	tst r0, #1
	moveq r1, #1
	beq _0601731C
	tst r0, #4
	moveq r1, #0x12
	beq _0601731C
	ldrh r1, [sp, #0x16]
	mov r0, r7
	bl CAM_SetSupRate
	mov r1, #0
_0601731C:
	ldr r2, [sp, #0x1c]
	mov r0, r7
	bl MakeReAssResFrame
	cmp r0, #0
	beq _06017660
	bl TxManCtrlFrame
	b _06017660
_06017338:
	add r0, r6, #0x10
	bl RxProbeReqFrame
	b _06017660
_06017344:
	add r0, r6, #0x10
	mov r1, #0
	bl RxProbeResFrame
	b _06017660
_06017354:
	add r0, r6, #0x10
	bl RxDisAssFrame
	b _06017660
_06017360:
	add r0, r6, #0x10
	bl RxAuthFrame
	b _06017660
_0601736C:
	add r0, r6, #0x10
	bl RxDeAuthFrame
	b _06017660
_06017378:
	cmp r8, #1
	bne _06017660
	cmp sb, #0xa
	bne _06017660
	ldr r0, _06017690 ; =0x0380FFF4
	ldrh r4, [r6, #0x12]
	ldr r1, [r0]
	mov r0, r4
	add r5, r1, #0x17c
	bl CAM_GetStaState
	cmp r0, #0x40
	bne _06017660
	mov r0, r4
	bl CAM_SetAwake
	ldrh r0, [r5, #0x2c]
	cmp r0, #0
	beq _060173C4
	mov r0, #1
	bl TxqPri
_060173C4:
	ldrh r0, [r5, #0x20]
	cmp r0, #0
	beq _06017660
	mov r0, #0
	bl TxqPri
	b _06017660
_060173DC:
	cmp r8, #0
	bne _06017660
	cmp sb, #0xc
	addls pc, pc, sb, lsl #2
	b _06017660
_060173F0: ; jump table
	b _06017660 ; case 0
	b _06017430 ; case 1
	b _06017660 ; case 2
	b _0601752C ; case 3
	b _06017660 ; case 4
	b _06017630 ; case 5
	b _06017660 ; case 6
	b _06017660 ; case 7
	b _06017424 ; case 8
	b _06017660 ; case 9
	b _06017640 ; case 10
	b _0601764C ; case 11
	b _06017658 ; case 12
_06017424:
	add r0, r6, #0x10
	bl RxBeaconFrame
	b _06017660
_06017430:
	ldr r0, _06017690 ; =0x0380FFF4
	ldr r0, [r0]
	add r5, r0, #0x344
	ldrh r1, [r5, #0xc]
	add r0, r0, #4
	cmp r1, #2
	add r4, r0, #0x400
	cmpne r1, #3
	bne _06017660
	ldrh r0, [r4]
	cmp r0, #0x51
	bne _06017660
	ldr r0, [r4, #0x18]
	add r1, r6, #0x2e
	add r0, r0, #0x10
	bl MatchMacAdrs
	cmp r0, #0
	beq _06017660
	bl ClearTimeOut
	ldrh r0, [r6, #0x3e]
	cmp r0, #0
	bne _060174CC
	ldrh r1, [r6, #0x40]
	ldr r0, _06017698 ; =0x00000FFF
	and r0, r1, r0
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	bl WSetAids
	ldrh r0, [r5, #0x6a]
	bl MakePsPollFrame
	add r0, r5, #0x82
	add r1, r6, #0x2e
	bl WSetMacAdrs1
	add r0, r6, #0x2e
	bl CAM_Search
	strh r0, [r5, #0x88]
	ldrh r0, [r5, #0x88]
	mov r1, #0x40
	bl CAM_SetStaState
_060174CC:
	ldrh r0, [r6, #0x3e]
	cmp r0, #0
	ldr r0, [r4, #0x1c]
	movne r1, #0xc
	strneh r1, [r0, #4]
	ldrneh r1, [r6, #0x3e]
	ldrne r0, [r4, #0x1c]
	strneh r1, [r0, #6]
	bne _06017508
	mov r2, #0
	strh r2, [r0, #4]
	ldr r1, [r4, #0x1c]
	mov r0, #0x40
	strh r2, [r1, #6]
	bl WSetStaState
_06017508:
	ldrh r1, [r5, #0x6a]
	ldr r0, [r4, #0x1c]
	mov r2, #0x53
	strh r1, [r0, #8]
	mov r0, #2
	mov r1, #3
	strh r2, [r4]
	bl AddTask
	b _06017660
_0601752C:
	ldr r0, _06017690 ; =0x0380FFF4
	ldr r0, [r0]
	add r5, r0, #0x344
	ldrh r1, [r5, #0xc]
	add r0, r0, #4
	cmp r1, #2
	add r4, r0, #0x400
	cmpne r1, #3
	bne _06017660
	ldrh r0, [r4]
	cmp r0, #0x61
	bne _06017660
	ldr r0, [r4, #0x18]
	add r1, r6, #0x2e
	add r0, r0, #0x10
	bl MatchMacAdrs
	cmp r0, #0
	beq _06017660
	bl ClearTimeOut
	ldrh r0, [r6, #0x3e]
	cmp r0, #0
	bne _060175D0
	ldrh r1, [r6, #0x40]
	ldr r0, _06017698 ; =0x00000FFF
	and r0, r1, r0
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	bl WSetAids
	ldrh r0, [r5, #0x6a]
	bl MakePsPollFrame
	add r0, r5, #0x82
	add r1, r6, #0x2e
	bl WSetMacAdrs1
	add r0, r6, #0x2e
	bl CAM_Search
	strh r0, [r5, #0x88]
	ldrh r0, [r5, #0x88]
	mov r1, #0x40
	bl CAM_SetStaState
	mov r0, #0x40
	bl WSetStaState
_060175D0:
	ldrh r0, [r6, #0x3e]
	cmp r0, #0
	ldr r0, [r4, #0x1c]
	movne r1, #0xc
	strneh r1, [r0, #4]
	ldrneh r1, [r6, #0x3e]
	ldrne r0, [r4, #0x1c]
	strneh r1, [r0, #6]
	bne _0601760C
	mov r2, #0
	strh r2, [r0, #4]
	ldr r1, [r4, #0x1c]
	mov r0, #0x40
	strh r2, [r1, #6]
	bl WSetStaState
_0601760C:
	ldrh r1, [r5, #0x6a]
	ldr r0, [r4, #0x1c]
	mov r2, #0x63
	strh r1, [r0, #8]
	mov r0, #2
	mov r1, #4
	strh r2, [r4]
	bl AddTask
	b _06017660
_06017630:
	add r0, r6, #0x10
	mov r1, #0
	bl RxProbeResFrame
	b _06017660
_06017640:
	add r0, r6, #0x10
	bl RxDisAssFrame
	b _06017660
_0601764C:
	add r0, r6, #0x10
	bl RxAuthFrame
	b _06017660
_06017658:
	add r0, r6, #0x10
	bl RxDeAuthFrame
_06017660:
	mov r1, r6
	add r0, sl, #0x60
	bl ReleaseHeapBuf
	ldrh r0, [sl, #0x68]
	cmp r0, #0
	beq _06017684
	mov r0, #1
	mov r1, #7
	bl AddTask
_06017684:
	add sp, sp, #0x2c
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_06017690: .word 0x0380FFF4
_06017694: .word 0x0000FFC2
_06017698: .word 0x00000FFF
	arm_func_end RxManCtrlTask

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
