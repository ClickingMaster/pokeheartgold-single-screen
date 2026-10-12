	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

	.text

	.extern PokeathlonRingDrop_UpdateStamina
	.type PokeathlonRingDrop_UpdateStamina, @function

	.extern PokeathlonCourse_RunSubStateLoop
	.extern PokeathlonCourse_InitStateInfo
	.extern PokeathlonCourse_InitPlayerProfiles
	.extern PokeathlonCourse_GetPlayerProfile
	.extern PokeathlonCourse_GetParticipantCount
	.extern PokeathlonCourse_GetParticipantData
	.extern PokeathlonCourse_GetParticipantUnk04
	.extern PokeathlonCourse_GetSaveData
	.extern PokeathlonCourse_GetFieldData
	.extern PokeathlonCourse_GetFieldData_AtIndex
	.extern PokeathlonCourse_GetFieldBA4
	.extern PokeathlonCourse_AllocPtr4FromHeap
	.extern PokeathlonCourse_FreePtr4HeapAlloc
	.extern PokeathlonCourse_GetHeapAllocPtr4
	.extern PokeathlonCourse_GetHeapID
	.extern PokeathlonCourse_GetField1ED
	.extern PokeathlonCourse_SetField1ED
	.extern PokeathlonCourse_IncrementField1ED
	.extern PokeathlonCourse_SetField1F4
	.extern ov96_021E5E04
	.extern PokeathlonCourse_GetField3D8_ForCurrentParticipant
	.extern PokeathlonCourse_GetField3D8_AtIndex
	.extern ov96_021E5E7C
	.extern PokeathlonCourse_GetCurrentParticipantIndex
	.extern PokeathlonCourse_GetMode
	.extern PokeathlonCourse_GetField1EF
	.extern PokeathlonCourse_IncrementField1EF
	.extern PokeathlonCourse_ResetField1EF
	.extern PokeathlonCourse_GetSystem
	.extern ov96_021E5F24
	.extern PokeathlonCourse_GetPlayerProfileFromData
	.extern PokeathlonCourse_GetField974_AtIndex
	.extern PokeathlonCourse_GetDataCopyArea
	.extern PokeathlonCourse_ResetDataCopyArea
	.extern PokeathlonCourse_SetField3A4
	.extern PokeathlonCourse_ResetField3A4
	.extern PokeathlonCourse_GetField3A4
	.extern PokeathlonCourse_GetUnkConstant4
	.extern PokeathlonCourse_SetField5E0_AtIndex
	.extern PokeathlonCourse_GetField5F0_AtIndex
	.extern PokeathlonCourse_SetStateField07
	.extern PokeathlonCourse_SetStateField07_IfDifferent
	.extern PokeathlonCourse_SetStateTransitionType
	.extern PokeathlonCourse_SetVBlankIntrCB
	.extern PokeathlonCourse_GetGraphicsSystem
	.public PokeathlonCourse_GetPerformance
	.public ov96_021E6104
	.public PokeathlonCourse_SetHudPlanes
	.public PokeathlonCourse_GetPerformanceFileId
	.public ov96_021E6F18
	.public ov96_021E75B8
	.public PokeathlonCourse_AddCapped
	.public PokeathlonCourse_SetRecordScore
	.public PokeathlonCourseLink_SetCheckSize
	.public PokeathlonCourseLink_GetNetId
	.public ov96_021E8A20
	.public PokeathlonCourseCmd_GetTable
	.public PokeathlonCourseCmd_GetCount
	.public PokeathlonCourseMonGfx_AllocManager
	.public PokeathlonCourseMonGfx_IsLoaded
	.public PokeathlonCourseMonGfx_GetSprite
	.public PokeathlonCourseMonGfx_GetInfo
	.public PokeathlonCourseMonGfx_GetTopRow
	.public PokeathlonCourseMonGfx_IsShiftedUp
	.public PokeathlonCourse_FreeObjTransfer
	.public PokeathlonCourseScoreList_Init
	.public PokeathlonCourseScoreList_GetCount
	.public ov96_021E9528
	.public PokeathlonCourseCmd_EventsSize
	.public PokeathlonCourseCmd_ParticipantSize
	.public PokeathlonCourseCmd_AllParticipantsSize
	.public PokeathlonCourseCmd_ProfilesSize
	.public PokeathlonCourseCmd_RecordSize
	.public PokeathlonCourseCmd_PacketSize
	.public PokeathlonCourseCmd_MonCountersSize
	.public PokeathlonCourseCmd_ResultsSize
	.public PokeathlonCourseCmd_JuiceRecordSize
	.public PokeathlonCourseCmd_GetEventDataBuffer
	.public PokeathlonCourseCmd_GetRecordBuffer
	.public PokeathlonCourseCmd_GetResultsBuffer
	.public ov96_021E9A54
	.public PokeathlonCourseCmd_GetJuiceRecordBuffer
	.public PokeathlonCourseSprites_HasGroup
	.public PokeathlonCourseSprites_GetResMan
	.public PokeathlonCourseMonObjects_Free
	.public PokeathlonCourseMonObjects_IsLoaded
	.public PokeathlonCourseMonObject_GetGfx
	.public PokeathlonCourseMonObject_SetMonDrawFlag
	.public PokeathlonCourseMonObject_SetDrawPriorityStep
	.public PokeathlonCourseMonObject_SetAffineOverwriteMode
	.public PokeathlonCourseMonObject_SetAffineMatrix
	.public PokeathlonCourseMonObject_SetAnimStateRaw
	.public PokeathlonCourseMonObject_IsAnimated
	.public PokeathlonCourseMonObject_GetPosition
	.public PokeathlonCourseObject_SetTouchRadius
	.public PokeathlonCourseObject_SetCollision
	.public PokeathlonCourseObject_GetCollisionRadius
	.public PokeathlonCourseObject_GetCollisionOffsetY
	.public PokeathlonCourseObject_SetScale
	.public PokeathlonCourseObjects_ShiftLargeUp
	.public PokeathlonCourseSpritePoolSprite_SetAnim
	.public PokeathlonCourseSpritePoolSprite_TryChangeAnim
	.public PokeathlonCourseSpritePoolSprite_IsAnimated
	.public PokeathlonCourseSpritePoolSprite_SetPosition
	.public PokeathlonCourseSpritePoolSprite_GetPosition
	.public PokeathlonCourseSpritePoolSprite_SetScale
	.public PokeathlonCourseSpritePoolSprite_SetRotation
	.public PokeathlonCourseSpritePoolSprite_GetSprite
	.public PokeathlonCourseSpritePool_Draw
	.public PokeathlonCourseSpritePool_GetSpriteList
	.public PokeathlonCourseSpritePoolSprite_SetDrawPriority
	.public PokeathlonCourseOpening_Init
	.public PokeathlonCourseOpening_Main
	.public PokeathlonCourseOpening_Exit
	.public PokeathlonCourseClosing_Init
	.public PokeathlonCourseClosing_Main
	.public PokeathlonCourseClosing_Exit
	.public PokeathlonCourseCeremony_IsCryActive
	.public PokeathlonCourseCeremony_FreeScores
	.public PokeathlonCourseCeremony_AddPoints
	.public PokeathlonCourseCeremony_GetPoints
	.public PokeathlonCourseCeremonyScore_IsCounted
	.public PokeathlonCourseCeremonyScore_Add
	.public PokeathlonCourseCeremony_Main
	.public PokeathlonCourseConfetti_GetRandomDelay
	.public PokeathlonCourseCeremony_GetNoMissPoints
	.public PokeathlonCourseCeremony_GetPointsLeaderPoints
	.public PokeathlonCourseCeremony_GetEffortPrizePoints
	.public PokeathlonCourseCeremony_GetIntroMsgBase
	.public PokeathlonCourseMessageBox_IsBusy
	.public PokeathlonCourse_LoadSpriteChars
	.public PokeathlonCourseWait_Main
	.public PokeathlonCourseWait_Exit
	.public PokeathlonCourseWait_FreeBgs
	.public PokeathlonEventIntro_Main
	.public PokeathlonEventIntro_GetEvent
	.public PokeathlonEventIntro_HasFlag0EF
	.public PokeathlonEventIntro_ResetMascot
	.public PokeathlonEventIntro_HideZoomBgs
	.public PokeathlonEventIntro_IsFadeFinished
	.public PokeathlonCirclePush_Main
	.public PokeathlonCirclePushStack_Pop
	.public PokeathlonCirclePushZoneMarkers_Free
	.public PokeathlonCirclePushAi_Free
	.public PokeathlonCirclePushDisplay_LoadPics
	.public PokeathlonCirclePushDisplay_GetScore
	.public PokeathlonLampJump_Draw
	.public PokeathlonLampJump_FreeLamps
	.public PokeathlonLampJump_GetLampPos
	.public PokeathlonLampJump_IsLampLit
	.public PokeathlonLampJump_FreeCpuPlayers
	.public PokeathlonResultsAnnouncement_CreatePlates
	.public PokeathlonResultsAnnouncement_Wait
	.public PokeathlonResultsAnnouncement_ClearSlide
	.public PokeathlonResultsAnnouncement_RemoveSlide
	.public PokeathlonResultsAnnouncement_GetLeavingSlide
	.public PokeathlonResultsAnnouncement_DrawPlates
	.public PokeathlonResultsAnnouncement_GetPlatePos
	.public PokeathlonResultsAnnouncement_SetPlatePos
	.public PokeathlonResultsAnnouncement_GetPlateRank
	.public PokeathlonHurdleDash_Start
	.public PokeathlonHurdleDashRunner_GetHurdle
	.public PokeathlonHurdleDash_FreeHurdles
	.public PokeathlonHurdleDash_GetHurdle
	.public PokeathlonHurdle_SetSprites
	.public PokeathlonHurdle_IsVisible
	.public PokeathlonHurdle_IsStanding
	.public PokeathlonHurdleDashTrack_GetLaneHurdles
	.public PokeathlonHurdleDashTrack_GetLayoutToSend
	.public PokeathlonHurdleDashTrack_GetLayout
	.public PokeathlonHurdleDashTrack_GetLayoutSize
	.public PokeathlonHurdleDashStandings_Draw
	.public PokeathlonHurdleDashDust_Free
	.public PokeathlonRelayRun_Draw
	.public PokeathlonRelayRun_IsTouchingSwapButton
	.public PokeathlonRelayRunTeam_UpdateStun
	.public PokeathlonRelayRunHud_GetBenchMember
	.public PokeathlonRelayRunCpu_SetTeam
	.public PokeathlonRelayRunCpu_Delete
	.public PokeathlonSnowThrowDisplay_ShowTired
	.public PokeathlonSnowThrowDisplay_IsPicAnimated
	.public PokeathlonSnowThrowDisplay_Shake
	.public PokeathlonSnowThrowCpu_SetMon
	.public PokeathlonSnowThrowCpu_Delete
	.public PokeathlonPennantCapture_Draw
	.public PokeathlonPennantCapture_TickFreezeTimer
	.public PokeathlonPennantCapture_AddPoint2D
	.public PokeathlonPennantCapture_ResetPennantSpots
	.public ov96_02207BD4
	.public PokeathlonPennantCapture_GetDrawPriority
	.public PokeathlonPennantCaptureCpu_SetPlayer
	.public PokeathlonPennantCaptureCpu_SetPennants
	.public PokeathlonPennantCaptureCpu_Delete
	.public PokeathlonBlockSmash_Draw
	.public PokeathlonBlockSmashBlocks_GetScore
	.public PokeathlonBlockSmashMon_GetMode
	.public PokeathlonBlockSmashMon_IsGaugeFull
	.public PokeathlonBlockSmashMon_IsTired
	.public PokeathlonBlockSmashCpu_Delete
	.public PokeathlonDiscCatchShake_IsDone
	.public PokeathlonDiscCatchShake_Init
	.public PokeathlonDiscCatch_GetTeamScore
	.public PokeathlonDiscCatch_Square
	.public PokeathlonDiscCatch_SlowDown
	.public PokeathlonDiscCatchStats_GetJumpDuration
	.public PokeathlonDiscCatchDiscs_Init
	.public PokeathlonDiscCatch_AddScoreWindow
	.public PokeathlonDiscCatchScoreboard_Pause
	.public PokeathlonDiscCatchScoreboard_GetOwnScore
	.public PokeathlonDiscCatchCpu_Delete
	.public PokeathlonGoalRoll_Draw
	.public PokeathlonGoalRoll_SetTouchTarget
	.public PokeathlonGoalRollDisplay_GetScore
	.public PokeathlonGoalRollCpu_SetBalls
	.public PokeathlonGoalRollCpu_Free
	.public PokeathlonScreenShake_IsDone
	.public PokeathlonScreenShake_Init
	.public PokeathlonRingDrop_IsOnSwitchButton
	.public PokeathlonRingDrop_DecayVelocity
	.public PokeathlonRingDrop_GetMonState
	.public PokeathlonRingDrop_GetDirection
	.public PokeathlonRingDrop_IsAttackState
	.public PokeathlonRingDrop_FreeMonObjects
	.public PokeathlonRingDropMon_SetDestination
	.public PokeathlonRingDropMon_Respawn
	.public PokeathlonRingDrop_AddScoreWindow
	.public PokeathlonRingDropUi_GetScore
	.public PokeathlonRingDropCpu_Free
	.public PokeathlonEvent_PennantCapturePoints
	.public PokeathlonEvent_CirclePushPoints
	.public PokeathlonEvent_BlockSmashPoints
	.public PokeathlonEvent_DiscCatchPoints
	.public PokeathlonEvent_SnowThrowPoints

	.extern PokeathlonRingDropMon_ApplyPushReaction
	.type PokeathlonRingDropMon_ApplyPushReaction, @function

	.extern PokeathlonRingDropMon_GetPushReaction
	.type PokeathlonRingDropMon_GetPushReaction, @function

	thumb_func_start PokeathlonRingDrop_UpdateMons
PokeathlonRingDrop_UpdateMons: ; 0x022180CC
	push {r4, r5, r6, r7, lr}
	sub sp, #0x24
	str r0, [sp]
	mov r0, #0
	mov r7, #0x3f
	ldr r5, [sp]
	str r1, [sp, #4]
	str r0, [sp, #8]
	lsl r7, r7, #0x18
_022180DE:
	mov r1, #0
	add r0, sp, #0x18
	str r1, [r0]
	str r1, [r0, #4]
	str r1, [r0, #8]
	add r0, sp, #0xc
	str r1, [r0]
	str r1, [r0, #4]
	str r1, [r0, #8]
	add r0, r1, #0
	add r6, r5, #0
	add r4, r5, #0
	ldrsh r0, [r5, r0]
	add r6, #8
	add r4, #0xc
	cmp r0, #0
	ble _02218110
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r0, #0
	add r0, r7, #0
	bl _fadd
	b _0221811C
_02218110:
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r7, #0
	bl _fsub
_0221811C:
	bl _ffix
	str r0, [sp, #0x18]
	mov r0, #2
	ldrsh r0, [r5, r0]
	cmp r0, #0
	ble _0221813A
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r0, #0
	add r0, r7, #0
	bl _fadd
	b _02218146
_0221813A:
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r7, #0
	bl _fsub
_02218146:
	bl _ffix
	str r0, [sp, #0x1c]
	mov r0, #4
	ldrsh r0, [r5, r0]
	cmp r0, #0
	ble _02218164
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r0, #0
	add r0, r7, #0
	bl _fadd
	b _02218170
_02218164:
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r7, #0
	bl _fsub
_02218170:
	bl _ffix
	str r0, [sp, #0xc]
	mov r0, #6
	ldrsh r0, [r5, r0]
	cmp r0, #0
	ble _0221818E
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r0, #0
	add r0, r7, #0
	bl _fadd
	b _0221819A
_0221818E:
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r7, #0
	bl _fsub
_0221819A:
	bl _ffix
	str r0, [sp, #0x10]
	ldr r1, [r6]
	lsl r0, r1, #0x11
	lsr r0, r0, #0x1f
	beq _02218226
	mov r0, #0
	mov r1, #2
	ldrsh r0, [r5, r0]
	ldrsh r1, [r5, r1]
	bl PokeathlonRingDrop_IsOnSwitchButton
	cmp r0, #0
	beq _022181CC
	add r0, r4, #0
	bl PokeathlonRingDropMon_CanSwitch
	cmp r0, #0
	bne _022181C4
	b _022182FC
_022181C4:
	add r0, r4, #0
	bl PokeathlonRingDropMon_StartSwitch
	b _022182FC
_022181CC:
	bl ov96_021E6104
	cmp r0, #0
	ble _022181E8
	bl ov96_021E6104
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r0, #0
	add r0, r7, #0
	bl _fadd
	b _022181F8
_022181E8:
	bl ov96_021E6104
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r7, #0
	bl _fsub
_022181F8:
	bl _ffix
	add r2, r0, #0
	ldr r0, [r4, #0x60]
	lsl r0, r0, #5
	lsr r0, r0, #0x1f
	bne _022182FC
	ldr r0, [r4, #0x14]
	cmp r0, #3
	beq _022182FC
	add r4, #0x2c
	add r0, r4, #0
	add r1, sp, #0x18
	bl PokeathlonRingDrop_IsWithinDistance
	cmp r0, #0
	beq _022182FC
	mov r0, #2
	ldr r1, [r6]
	lsl r0, r0, #0x1c
	orr r0, r1
	str r0, [r6]
	b _022182FC
_02218226:
	lsl r0, r1, #0x10
	lsr r0, r0, #0x1f
	beq _02218274
	lsl r0, r1, #2
	lsr r0, r0, #0x1f
	cmp r0, #1
	bne _022182FC
	ldr r0, [r4, #0x2c]
	lsl r0, r0, #4
	asr r3, r0, #0x10
	ldr r0, [r4, #0x30]
	lsl r0, r0, #4
	asr r2, r0, #0x10
	ldr r0, [r4, #0x14]
	cmp r0, #3
	bne _0221824E
	ldr r0, _02218328 ; =0xDFFFFFFF
	and r0, r1
	str r0, [r6]
	b _022182FC
_0221824E:
	mov r0, #0
	ldrsh r0, [r5, r0]
	sub r0, r0, r3
	bpl _02218258
	neg r0, r0
_02218258:
	cmp r0, #1
	bgt _0221826A
	mov r0, #2
	ldrsh r0, [r5, r0]
	sub r0, r0, r2
	bpl _02218266
	neg r0, r0
_02218266:
	cmp r0, #1
	ble _022182FC
_0221826A:
	add r0, r4, #0
	add r1, r5, #0
	bl PokeathlonRingDropMon_SetDestination
	b _022182FC
_02218274:
	lsl r0, r1, #3
	lsr r0, r0, #0x13
	beq _022182FC
	lsl r0, r1, #2
	lsr r0, r0, #0x1f
	cmp r0, #1
	bne _022182F4
	ldr r0, [r4, #0x14]
	cmp r0, #1
	beq _0221828C
	cmp r0, #4
	bne _022182F4
_0221828C:
	ldr r0, [r4, #0x60]
	lsl r1, r0, #3
	lsr r1, r1, #0x1f
	bne _022182F4
	lsl r0, r0, #5
	lsr r0, r0, #0x1f
	bne _022182F4
	mov r2, #1
	add r0, sp, #0xc
	add r1, sp, #0x18
	lsl r2, r2, #0xe
	bl PokeathlonRingDrop_IsWithinDistance
	cmp r0, #0
	beq _022182C4
	ldr r0, [r6]
	lsl r0, r0, #3
	lsr r0, r0, #0x13
	cmp r0, #7
	bhi _022182F4
	ldr r1, [r4, #0x60]
	ldr r0, _0221832C ; =0xBFFFFFFF
	and r1, r0
	mov r0, #2
	lsl r0, r0, #0x1c
	orr r0, r1
	str r0, [r4, #0x60]
	b _022182F4
_022182C4:
	mov r2, #3
	add r0, sp, #0xc
	add r1, sp, #0x18
	lsl r2, r2, #0xe
	bl PokeathlonRingDrop_IsWithinDistance
	cmp r0, #0
	bne _022182F4
	ldr r0, [r6]
	lsl r0, r0, #3
	lsr r0, r0, #0x13
	cmp r0, #7
	bhi _022182F4
	mov r0, #1
	ldr r1, [r4, #0x60]
	lsl r0, r0, #0x1e
	orr r1, r0
	ldr r0, _02218328 ; =0xDFFFFFFF
	and r0, r1
	str r0, [r4, #0x60]
	ldrh r0, [r5]
	strh r0, [r4, #0x1c]
	ldrh r0, [r5, #2]
	strh r0, [r4, #0x1e]
_022182F4:
	ldr r1, [r6]
	ldr r0, _02218328 ; =0xDFFFFFFF
	and r0, r1
	str r0, [r6]
_022182FC:
	add r0, r5, #0
	ldr r1, [sp, #4]
	add r0, #0xc
	bl PokeathlonRingDropMon_Update
	ldr r0, [sp, #8]
	add r5, #0xa8
	add r0, r0, #1
	str r0, [sp, #8]
	cmp r0, #4
	bge _02218314
	b _022180DE
_02218314:
	ldr r0, [sp, #4]
	bl PokeathlonRingDrop_UpdateCollisions
	ldr r0, [sp]
	ldr r1, [sp, #4]
	bl PokeathlonRingDrop_UpdateStamina
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
	nop
_02218328: .word 0xDFFFFFFF
_0221832C: .word 0xBFFFFFFF
	thumb_func_end PokeathlonRingDrop_UpdateMons


	thumb_func_start PokeathlonRingDrop_BuildFramePacket
PokeathlonRingDrop_BuildFramePacket: ; 0x02218330
	push {r4, r5, r6, r7, lr}
	sub sp, #0x24
	str r0, [sp]
	mov r0, #0
	str r0, [sp, #4]
	add r0, r1, #0
	bl PokeathlonCourse_GetDataCopyArea
	add r0, #0x28
	bl ov96_021E8A20
	str r0, [sp, #8]
	mov r2, #0x30
	str r2, [sp, #0x14]
	mov r2, #0xc0
	ldr r0, [sp]
	str r2, [sp, #0x10]
	str r2, [sp, #0xc]
	mov r2, #0x3f
	mov ip, r2
	mov r2, #0xf
	str r2, [sp, #0x20]
	mov r2, #0x60
	str r2, [sp, #0x1c]
	mov r2, #7
	ldr r1, [sp, #8]
	mov r6, #0
	add r0, #0x1c
	mov r7, #0x18
	str r2, [sp, #0x18]
_0221836C:
	ldr r2, [r0, #0x2c]
	ldr r3, [r0, #0x30]
	lsl r2, r2, #4
	lsl r3, r3, #4
	ldr r4, [r0, #0x64]
	asr r2, r2, #0x10
	asr r3, r3, #0x10
	cmp r4, #0
	beq _02218384
	sub r4, r4, #1
	str r4, [r0, #0x64]
	b _02218392
_02218384:
	bne _02218392
	ldr r5, [r0, #0x60]
	ldr r4, _02218504 ; =0xFFFFCFFF
	and r5, r4
	ldr r4, _02218508 ; =0xFFFF3FFF
	and r4, r5
	str r4, [r0, #0x60]
_02218392:
	ldrb r4, [r1, #4]
	ldr r5, [sp, #0x14]
	bic r4, r5
	ldr r5, [r0, #0x60]
	lsl r5, r5, #0x12
	lsr r5, r5, #0x1e
	lsl r5, r5, #0x18
	lsr r5, r5, #0x18
	lsl r5, r5, #0x1e
	lsr r5, r5, #0x1a
	orr r4, r5
	strb r4, [r1, #4]
	ldrb r4, [r1, #2]
	ldr r5, [sp, #0x10]
	bic r4, r5
	ldr r5, [r0, #0x60]
	lsl r5, r5, #0x10
	lsr r5, r5, #0x1e
	lsl r5, r5, #0x18
	lsr r5, r5, #0x18
	lsl r5, r5, #0x1e
	lsr r5, r5, #0x18
	orr r4, r5
	strb r4, [r1, #2]
	ldrb r4, [r1, #4]
	ldr r5, [sp, #0xc]
	bic r4, r5
	ldr r5, [r0, #0x60]
	lsl r5, r5, #8
	lsr r5, r5, #0x1e
	lsl r5, r5, #0x18
	lsr r5, r5, #0x18
	lsl r5, r5, #0x1e
	lsr r5, r5, #0x18
	orr r4, r5
	strb r4, [r1, #4]
	ldr r5, [r0, #0x60]
	ldrb r4, [r1, #5]
	lsl r5, r5, #0xc
	lsr r5, r5, #0x1c
	sub r5, r5, #1
	lsl r5, r5, #0x18
	lsr r5, r5, #0x18
	lsl r5, r5, #0x1e
	bic r4, r7
	lsr r5, r5, #0x1b
	orr r4, r5
	strb r4, [r1, #5]
	cmp r2, #0xff
	ble _022183FA
	mov r2, #0xff
	b _02218400
_022183FA:
	cmp r2, #0
	bge _02218400
	mov r2, #0
_02218400:
	strb r2, [r1]
	cmp r3, #0xff
	ble _0221840A
	mov r3, #0xff
	b _02218410
_0221840A:
	cmp r3, #0
	bge _02218410
	mov r3, #0
_02218410:
	strb r3, [r1, #1]
	ldrb r4, [r1, #2]
	mov r2, ip
	bic r4, r2
	ldr r2, [r0, #0x50]
	asr r2, r2, #0xc
	lsl r2, r2, #0x18
	lsr r3, r2, #0x18
	mov r2, #0x3f
	and r2, r3
	orr r2, r4
	strb r2, [r1, #2]
	ldrb r4, [r1, #4]
	ldr r2, [sp, #0x20]
	bic r4, r2
	ldr r2, [r0, #0x14]
	lsl r2, r2, #0x18
	lsr r3, r2, #0x18
	mov r2, #0xf
	and r2, r3
	orr r2, r4
	strb r2, [r1, #4]
	ldr r2, [r0, #0x60]
	ldr r3, [sp, #0x1c]
	lsl r2, r2, #0x14
	lsr r2, r2, #0x14
	strb r2, [r1, #3]
	ldrb r2, [r1, #5]
	bic r2, r3
	ldr r3, [r0, #0x60]
	lsl r3, r3, #6
	lsr r3, r3, #0x1e
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	lsl r3, r3, #0x1e
	lsr r3, r3, #0x19
	orr r2, r3
	strb r2, [r1, #5]
	ldrb r4, [r1, #5]
	ldr r2, [sp, #0x18]
	bic r4, r2
	add r2, r0, #0
	add r2, #0x5f
	ldrb r3, [r2]
	mov r2, #7
	and r2, r3
	orr r2, r4
	strb r2, [r1, #5]
	ldr r2, [r0, #0x60]
	lsr r3, r2, #0x1f
	beq _02218480
	mov r3, #1
	str r3, [sp, #4]
	ldr r3, _0221850C ; =0x7FFFFFFF
	and r2, r3
	str r2, [r0, #0x60]
_02218480:
	add r6, r6, #1
	add r0, #0xa8
	add r1, r1, #6
	cmp r6, #4
	bge _0221848C
	b _0221836C
_0221848C:
	ldr r0, [sp, #8]
	ldr r6, [sp, #8]
	ldr r1, [r0, #0x1c]
	mov r0, #0x80
	bic r1, r0
	ldr r0, [sp, #4]
	mov r5, #0
	lsl r0, r0, #0x1f
	lsr r0, r0, #0x18
	orr r1, r0
	ldr r0, [sp, #8]
	add r6, #0x1c
	str r1, [r0, #0x1c]
	lsl r0, r1, #0x18
	lsr r1, r0, #0x18
	ldr r0, [sp, #8]
	add r4, r5, #0
	str r1, [r0, #0x1c]
_022184B0:
	add r0, r5, #0
	mov r1, #3
	bl _s32_div_f
	add r7, r0, #0
	add r0, r5, #0
	mov r1, #3
	bl _s32_div_f
	mov r0, #0xa8
	lsl r2, r1, #4
	ldr r1, [sp]
	mul r0, r7
	add r0, r1, r0
	add r0, r2, r0
	add r0, #0x8c
	ldr r0, [r0]
	asr r0, r0, #0xc
	bne _022184DA
	mov r1, #2
	b _022184E4
_022184DA:
	cmp r0, #0x1e
	bge _022184E2
	mov r1, #1
	b _022184E4
_022184E2:
	mov r1, #0
_022184E4:
	ldr r2, [r6]
	lsl r1, r4
	lsl r0, r2, #0x18
	lsr r2, r2, #8
	add r1, r2, r1
	lsr r0, r0, #0x18
	lsl r1, r1, #8
	orr r0, r1
	add r5, r5, #1
	add r4, r4, #2
	str r0, [r6]
	cmp r5, #0xc
	blt _022184B0
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
	nop
_02218504: .word 0xFFFFCFFF
_02218508: .word 0xFFFF3FFF
_0221850C: .word 0x7FFFFFFF
	thumb_func_end PokeathlonRingDrop_BuildFramePacket


	thumb_func_start PokeathlonRingDrop_SetMonAffineCenter
