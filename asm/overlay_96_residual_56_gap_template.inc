	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

	.text

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
	.public ov96_021FCD74
	.public ov96_021FDE6C
	.public ov96_021FE538
	.public ov96_02200BC8
	.public ov96_02200E78
	.public ov96_02200EEC
	.public ov96_02203BC0
	.public ov96_02203CC4
	.public ov96_02203CD4
	.public ov96_022043AC
	.public ov96_022043B8
	.public ov96_02204DE8
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
	.public ov96_02219FDC
	.public ov96_0221A670
	.public ov96_0221A680
	.public ov96_0221A690
	.public ov96_0221A69C
	.public ov96_0221A720

	.if 0
	thumb_func_start PokeathlonCirclePushAi_Update
PokeathlonCirclePushAi_Update: ; 0x021F342C
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	ldr r0, _021F34FC ; =0x000004EA
	add r7, r1, #0
	ldrb r1, [r5, r0]
	mov r4, #0xc
	cmp r1, #4
	bne _021F3440
	add r0, r4, #0
	pop {r3, r4, r5, r6, r7, pc}
_021F3440:
	sub r1, r0, #4
	ldrb r1, [r5, r1]
	add r2, r1, #1
	sub r1, r0, #4
	strb r2, [r5, r1]
	ldrb r2, [r5, r1]
	sub r1, r0, #3
	ldrb r1, [r5, r1]
	cmp r2, r1
	blo _021F3484
	add r1, r0, #0
	mov r2, #1
	sub r1, #0x3a
	str r2, [r5, r1]
	sub r2, r0, #4
	mov r1, #0
	strb r1, [r5, r2]
	sub r0, r0, #6
	ldrb r0, [r5, r0]
	cmp r0, #0
	bls _021F3484
	ldr r2, _021F3500 ; =0x000004B4
	add r0, r1, #0
	add r3, r2, #0
	add r3, #0x30
_021F3472:
	lsl r6, r1, #2
	add r6, r5, r6
	str r0, [r6, r2]
	add r1, r1, #1
	lsl r1, r1, #0x18
	ldrb r6, [r5, r3]
	lsr r1, r1, #0x18
	cmp r1, r6
	blo _021F3472
_021F3484:
	mov r0, #0x4b
	lsl r0, r0, #4
	ldr r1, [r5, r0]
	cmp r1, #0
	beq _021F34F6
	add r1, r0, #0
	add r1, #0x39
	ldrb r4, [r5, r1]
	add r1, r0, #0
	add r1, #0x38
	add r0, #0x35
	ldrb r2, [r5, r1]
	mov r1, #2
	ldrb r0, [r5, r0]
	sub r1, r1, r2
	add r2, r4, r0
	lsl r0, r2, #1
	add r0, r2, r0
	add r6, r1, r0
	add r0, r6, #0
	mov r1, #3
	bl _s32_div_f
	cmp r0, r4
	bge _021F34BA
	bl GF_AssertFail
_021F34BA:
	lsl r1, r6, #0x18
	add r0, r5, #0
	lsr r1, r1, #0x18
	add r2, r7, #0
	bl PokeathlonCirclePushAi_ChooseTarget
	lsl r0, r6, #0x18
	lsr r4, r0, #0x18
	ldr r0, _021F3504 ; =0x000004E5
	ldrb r1, [r5, r0]
	add r1, r1, #1
	strb r1, [r5, r0]
	sub r1, r0, #1
	ldrb r2, [r5, r0]
	ldrb r1, [r5, r1]
	cmp r2, r1
	blo _021F34F6
	mov r1, #0
	strb r1, [r5, r0]
	add r0, r0, #3
	ldrb r0, [r5, r0]
	mov r1, #3
	add r0, r0, #1
	bl _s32_div_f
	ldr r0, _021F3508 ; =0x000004E8
	strb r1, [r5, r0]
	mov r1, #0
	sub r0, #0x38
	str r1, [r5, r0]
_021F34F6:
	add r0, r4, #0
	pop {r3, r4, r5, r6, r7, pc}
	nop
_021F34FC: .word 0x000004EA
_021F3500: .word 0x000004B4
_021F3504: .word 0x000004E5
_021F3508: .word 0x000004E8
	thumb_func_end PokeathlonCirclePushAi_Update
	.endif


	.if R56_GAP == 1
	thumb_func_start PokeathlonCirclePushAi_ChooseTarget
PokeathlonCirclePushAi_ChooseTarget: ; 0x021F350C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x198
	lsl r1, r1, #5
	str r0, [sp, #8]
	add r0, r0, r1
	str r2, [sp, #0xc]
	str r0, [sp, #0x20]
	ldr r0, [r0, #0x10]
	ldr r0, [r0]
	cmp r0, #0
	bne _021F3524
	b _021F3800
_021F3524:
	ldr r0, [sp, #0x20]
	ldr r0, [r0, #0x18]
	cmp r0, #0
	bne _021F3534
	bl GF_AssertFail
	add sp, #0x198
	pop {r3, r4, r5, r6, r7, pc}
_021F3534:
	ldr r1, _021F3804 ; =0x000004E8
	ldr r0, [sp, #8]
	ldrb r0, [r0, r1]
	sub r1, #0x34
	lsl r2, r0, #2
	ldr r0, [sp, #8]
	add r0, r0, r2
	ldr r0, [r0, r1]
	cmp r0, #3
	bge _021F356C
	bl LCRandom
	mov r1, #0x64
	bl _s32_div_f
	cmp r1, #0x1e
	bge _021F356C
	ldr r0, [sp, #8]
	ldr r1, _021F3808 ; =0x000004B4
	add sp, #0x198
	add r2, r0, r1
	add r1, #0x34
	ldrb r0, [r0, r1]
	lsl r1, r0, #2
	ldr r0, [r2, r1]
	add r0, r0, #1
	str r0, [r2, r1]
	pop {r3, r4, r5, r6, r7, pc}
_021F356C:
	ldr r5, [sp, #8]
	mov r4, #0
	add r6, sp, #0x3c
	add r7, sp, #0x38
_021F3574:
	ldr r0, [r5, #0x10]
	ldr r0, [r0]
	cmp r0, #0
	beq _021F35A6
	ldr r2, [r5, #0xc]
	add r3, sp, #0x40
	str r6, [sp]
	str r7, [sp, #4]
	ldr r0, [r5, #0x1c]
	ldmia r2!, {r1, r2}
	bl PokeathlonCourseObject_GetCollisionCircle
	ldr r1, [sp, #0x40]
	asr r0, r1, #0xb
	lsr r0, r0, #0x14
	add r0, r1, r0
	ldr r1, [sp, #0x3c]
	asr r0, r0, #0xc
	asr r2, r1, #0xb
	lsr r2, r2, #0x14
	add r2, r1, r2
	asr r1, r2, #0xc
	bl PokeathlonCirclePushAi_GetCell
	b _021F35AA
_021F35A6:
	mov r0, #0
	mvn r0, r0
_021F35AA:
	add r4, r4, #1
	str r0, [r5]
	add r5, #0x20
	cmp r4, #0xc
	blt _021F3574
	ldr r0, [sp, #0x20]
	ldr r2, [r0, #0xc]
	add r0, sp, #0x44
	str r0, [sp]
	ldr r3, [r2]
	ldr r0, [sp, #0x20]
	asr r1, r3, #0xb
	lsr r1, r1, #0x14
	add r1, r3, r1
	ldr r3, [r2, #4]
	ldr r0, [r0, #0x1c]
	asr r2, r3, #0xb
	lsr r2, r2, #0x14
	add r2, r3, r2
	asr r1, r1, #0xc
	asr r2, r2, #0xc
	add r3, sp, #0x48
	bl PokeathlonCourseObject_CenterToGround
	ldr r0, [sp, #0x48]
	ldr r1, [sp, #0x44]
	bl PokeathlonCirclePushAi_GetCell
	ldr r1, [sp, #0x20]
	str r0, [r1]
	ldr r0, [sp, #0xc]
	add r1, sp, #0xb0
	bl PokeathlonCirclePush_GetZoneCircles
	mov r6, #0
	ldr r5, [sp, #8]
	add r4, sp, #0xb0
	sub r7, r6, #1
_021F35F6:
	ldr r0, [r4]
	cmp r0, #0
	ble _021F360C
	ldrh r0, [r4, #4]
	ldrh r1, [r4, #6]
	bl PokeathlonCirclePushAi_GetCell
	mov r1, #0x4a
	lsl r1, r1, #4
	str r0, [r5, r1]
	b _021F3612
_021F360C:
	mov r0, #0x4a
	lsl r0, r0, #4
	str r7, [r5, r0]
_021F3612:
	add r6, r6, #1
	add r4, #8
	add r5, r5, #4
	cmp r6, #4
	blt _021F35F6
	ldr r0, [sp, #0x48]
	ldr r1, [sp, #0x44]
	lsl r0, r0, #0x10
	lsl r1, r1, #0x10
	add r2, sp, #0x24
	lsr r0, r0, #0x10
	lsr r1, r1, #0x10
	add r2, #2
	add r3, sp, #0x24
	bl PokeathlonCirclePushAi_GetColumnRow
	cmp r0, #0
	bne _021F363A
	bl GF_AssertFail
_021F363A:
	ldr r4, [sp, #8]
	mov r0, #0
	mov r1, #6
	str r0, [sp, #0x1c]
	str r0, [sp, #0x18]
	add r6, r0, #0
	lsl r1, r1, #6
	add r0, r4, #0
	add r5, r0, r1
	add r0, sp, #0x24
	mov r1, #2
	ldrsh r1, [r0, r1]
	add r7, sp, #0x134
	str r1, [sp, #0x14]
	ldr r1, [sp, #0x1c]
	ldrsh r0, [r0, r1]
	str r0, [sp, #0x10]
_021F365C:
	mov r0, #0x67
	lsl r0, r0, #2
	str r6, [r4, r0]
	add r0, r6, #0
	mov r1, #5
	bl _s32_div_f
	ldr r0, [sp, #0x14]
	sub r1, r1, #2
	add r1, r1, r0
	mov r0, #6
	lsl r0, r0, #6
	str r1, [r4, r0]
	add r0, r6, #0
	mov r1, #5
	bl _s32_div_f
	sub r1, r0, #2
	ldr r0, [sp, #0x10]
	add r1, r1, r0
	mov r0, #0x61
	lsl r0, r0, #2
	str r1, [r4, r0]
	sub r0, r0, #4
	ldr r1, [r4, r0]
	cmp r1, #0
	blt _021F36A4
	mov r0, #0x61
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	cmp r0, #0
	blt _021F36A4
	cmp r1, #7
	bge _021F36A4
	cmp r0, #5
	blt _021F36AE
_021F36A4:
	mov r0, #0x19
	mov r1, #0
	lsl r0, r0, #4
	str r1, [r4, r0]
	b _021F36B6
_021F36AE:
	mov r0, #0x19
	mov r1, #1
	lsl r0, r0, #4
	str r1, [r4, r0]
_021F36B6:
	mov r0, #6
	lsl r0, r0, #6
	ldr r1, [r4, r0]
	ldr r0, [sp, #0x14]
	sub r1, r1, r0
	mov r0, #0x61
	lsl r0, r0, #2
	ldr r2, [r4, r0]
	ldr r0, [sp, #0x10]
	str r5, [r7]
	sub r0, r2, r0
	mov r2, #0
	mvn r2, r2
	cmp r1, r2
	blt _021F36F2
	cmp r1, #1
	bgt _021F36F2
	add r1, r2, #0
	cmp r0, r1
	blt _021F36F2
	cmp r0, #1
	bgt _021F36F2
	ldr r1, [sp, #0x1c]
	ldr r0, [sp, #0x1c]
	add r1, r1, #1
	str r1, [sp, #0x1c]
	lsl r0, r0, #2
	add r1, sp, #0x8c
	str r5, [r1, r0]
	b _021F3700
_021F36F2:
	ldr r1, [sp, #0x18]
	ldr r0, [sp, #0x18]
	add r1, r1, #1
	str r1, [sp, #0x18]
	lsl r0, r0, #2
	add r1, sp, #0x4c
	str r5, [r1, r0]
_021F3700:
	add r6, r6, #1
	add r4, #0x20
	add r5, #0x20
	add r7, r7, #4
	cmp r6, #0x19
	blt _021F365C
	ldr r0, [sp, #0x1c]
	cmp r0, #9
	beq _021F3716
	bl GF_AssertFail
_021F3716:
	ldr r0, [sp, #0x18]
	cmp r0, #0x10
	beq _021F3720
	bl GF_AssertFail
_021F3720:
	mov r0, #9
	add r1, sp, #0x8c
	bl PokeathlonCirclePushAi_PlaceInCells
	ldr r1, [sp, #0x48]
	ldr r0, [sp, #0x9c]
	ldr r3, [sp, #0x20]
	str r1, [r0, #8]
	ldr r1, [sp, #0x44]
	ldr r0, [sp, #0x9c]
	add r2, sp, #0xb0
	str r1, [r0, #0xc]
	add r0, sp, #0x8c
	str r0, [sp]
	ldr r0, [sp, #8]
	mov r1, #9
	bl PokeathlonCirclePushAi_ScoreCells
	mov r0, #9
	add r1, sp, #0x8c
	add r2, sp, #0xd0
	bl PokeathlonCirclePushAi_SortCells
	add r4, r0, #0
	ldr r0, [sp, #0xd0]
	ldr r0, [r0, #0x18]
	cmp r0, #3
	blt _021F379C
	bl LCRandom
	add r1, r4, #0
	bl _s32_div_f
	lsl r0, r1, #0x18
	lsr r1, r0, #0x16
	add r0, sp, #0xd0
	ldr r2, [r0, r1]
	add r0, sp, #0x30
	str r0, [sp]
	ldr r0, [sp, #0x20]
	ldr r1, [r2, #8]
	ldr r0, [r0, #0x1c]
	ldr r2, [r2, #0xc]
	add r3, sp, #0x34
	bl PokeathlonCourseObject_GroundToCenter
	ldr r0, [sp, #0x34]
	lsl r1, r0, #0xc
	ldr r0, [sp, #0x20]
	ldr r0, [r0, #0x18]
	str r1, [r0]
	ldr r0, [sp, #0x30]
	lsl r1, r0, #0xc
	ldr r0, [sp, #0x20]
	ldr r0, [r0, #0x18]
	str r1, [r0, #4]
	ldr r0, [sp, #0x20]
	mov r1, #0
	ldr r0, [r0, #0x18]
	add sp, #0x198
	str r1, [r0, #8]
	pop {r3, r4, r5, r6, r7, pc}
_021F379C:
	mov r0, #0x10
	add r1, sp, #0x4c
	bl PokeathlonCirclePushAi_PlaceInCells
	add r0, sp, #0x134
	str r0, [sp]
	ldr r0, [sp, #8]
	ldr r3, [sp, #0x20]
	mov r1, #0x19
	add r2, sp, #0xb0
	bl PokeathlonCirclePushAi_ScoreCells
	mov r0, #0x19
	add r1, sp, #0x134
	add r2, sp, #0xd0
	bl PokeathlonCirclePushAi_SortCells
	add r4, r0, #0
	bl LCRandom
	add r1, r4, #0
	bl _s32_div_f
	lsl r0, r1, #0x18
	lsr r1, r0, #0x16
	add r0, sp, #0xd0
	ldr r2, [r0, r1]
	add r0, sp, #0x28
	str r0, [sp]
	ldr r0, [sp, #0x20]
	ldr r1, [r2, #8]
	ldr r0, [r0, #0x1c]
	ldr r2, [r2, #0xc]
	add r3, sp, #0x2c
	bl PokeathlonCourseObject_GroundToCenter
	ldr r0, [sp, #0x2c]
	lsl r1, r0, #0xc
	ldr r0, [sp, #0x20]
	ldr r0, [r0, #0x18]
	str r1, [r0]
	ldr r0, [sp, #0x28]
	lsl r1, r0, #0xc
	ldr r0, [sp, #0x20]
	ldr r0, [r0, #0x18]
	str r1, [r0, #4]
	ldr r0, [sp, #0x20]
	mov r1, #0
	ldr r0, [r0, #0x18]
	str r1, [r0, #8]
_021F3800:
	add sp, #0x198
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_021F3804: .word 0x000004E8
_021F3808: .word 0x000004B4
	thumb_func_end PokeathlonCirclePushAi_ChooseTarget
	.endif


	.if 0
	thumb_func_start PokeathlonCirclePushAi_PlaceInCells
PokeathlonCirclePushAi_PlaceInCells: ; 0x021F380C
	push {r3, r4, r5, r6, r7, lr}
	str r0, [sp]
	ldr r0, [sp]
	add r5, r1, #0
	mov r7, #0
	cmp r0, #0
	bls _021F3884
_021F381A:
	lsl r4, r7, #2
	ldr r6, [r5, r4]
	ldr r0, [r6, #0x10]
	cmp r0, #0
	beq _021F386A
	bl LCRandom
	ldr r1, [r6]
	lsl r3, r0, #0x1b
	lsl r2, r1, #5
	lsr r1, r0, #0x1f
	sub r3, r3, r1
	mov r0, #0x1b
	ror r3, r0
	add r2, #0x10
	add r0, r1, r3
	add r0, r2, r0
	str r0, [r6, #8]
	ldr r6, [r5, r4]
	bl LCRandom
	ldr r1, [r6, #4]
	lsl r3, r0, #0x1b
	lsl r2, r1, #5
	lsr r1, r0, #0x1f
	sub r3, r3, r1
	mov r0, #0x1b
	ror r3, r0
	add r0, r1, r3
	add r2, #0x10
	add r0, r2, r0
	str r0, [r6, #0xc]
	ldr r0, [r5, r4]
	mov r1, #7
	ldr r2, [r0, #4]
	ldr r3, [r0]
	mul r1, r2
	add r1, r3, r1
	str r1, [r0, #0x14]
	b _021F3878
_021F386A:
	mov r0, #0
	mvn r0, r0
	str r0, [r6, #8]
	ldr r1, [r5, r4]
	str r0, [r1, #0xc]
	ldr r1, [r5, r4]
	str r0, [r1, #0x14]
_021F3878:
	add r0, r7, #1
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	ldr r0, [sp]
	cmp r7, r0
	blo _021F381A
_021F3884:
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end PokeathlonCirclePushAi_PlaceInCells


	thumb_func_start PokeathlonCirclePushAi_ScoreCells
PokeathlonCirclePushAi_ScoreCells: ; 0x021F3888
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	str r1, [sp, #0xc]
	add r7, r0, #0
	ldr r0, [sp, #0xc]
	mov r4, #0
	str r2, [sp, #0x10]
	str r3, [sp, #0x14]
	ldr r5, [sp, #0x30]
	cmp r0, #0
	bls _021F38F6
_021F389E:
	lsl r6, r4, #2
	ldr r2, [r5, r6]
	ldr r0, [r2, #0x10]
	cmp r0, #0
	bne _021F38B0
	mov r0, #0x63
	mvn r0, r0
	str r0, [r2, #0x18]
	b _021F38EA
_021F38B0:
	ldr r0, [r2, #8]
	ldr r1, [sp, #0x10]
	str r0, [sp]
	ldr r0, [r2, #0xc]
	ldr r3, [sp, #0x14]
	str r0, [sp, #4]
	add r0, sp, #0x18
	str r0, [sp, #8]
	ldr r2, [r2, #0x14]
	add r0, r7, #0
	bl PokeathlonCirclePushAi_ScoreCell
	ldr r1, [r5, r6]
	str r0, [r1, #0x18]
	ldr r0, [sp, #0x18]
	cmp r0, #0
	beq _021F38EA
	ldr r3, [r5, r6]
	ldr r1, [sp, #0x10]
	add r0, r3, #0
	add r0, #0xc
	str r0, [sp]
	ldr r2, [r3, #0x14]
	add r0, r7, #0
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	add r3, #8
	bl PokeathlonCirclePushAi_SnapToZone
_021F38EA:
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	ldr r0, [sp, #0xc]
	cmp r4, r0
	blo _021F389E
_021F38F6:
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end PokeathlonCirclePushAi_ScoreCells


	thumb_func_start PokeathlonCirclePushAi_GetCell
PokeathlonCirclePushAi_GetCell: ; 0x021F38FC
	sub r0, #0x10
	asr r2, r0, #4
	lsr r2, r2, #0x1b
	add r2, r0, r2
	sub r1, #0x10
	asr r0, r1, #4
	lsr r0, r0, #0x1b
	asr r3, r2, #5
	add r0, r1, r0
	asr r2, r0, #5
	cmp r3, #0
	blt _021F3918
	cmp r2, #0
	bge _021F391E
_021F3918:
	mov r0, #0
	mvn r0, r0
	bx lr
_021F391E:
	mov r1, #7
	add r0, r2, #0
	mul r0, r1
	add r0, r3, r0
	cmp r0, #0x22
	ble _021F392E
	add r0, r1, #0
	sub r0, #8
_021F392E:
	bx lr
	thumb_func_end PokeathlonCirclePushAi_GetCell
	.endif


	.if R56_GAP == 2
	thumb_func_start PokeathlonCirclePushAi_ScoreCell
PokeathlonCirclePushAi_ScoreCell: ; 0x021F3930
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r5, r2, #0
	str r0, [sp]
	str r1, [sp, #4]
	add r6, r3, #0
	mov r4, #0
	ldr r7, [sp, #0x28]
	cmp r5, #0
	bge _021F394A
	add sp, #8
	add r0, r4, #0
	pop {r3, r4, r5, r6, r7, pc}
_021F394A:
	str r4, [r7]
	ldr r0, [r6, #8]
	ldrh r0, [r0]
	cmp r0, #0
	beq _021F3992
	ldr r0, [r6, #0x10]
	ldr r0, [r0]
	cmp r0, #1
	bne _021F3992
	add r0, r1, #0
	ldr r1, [sp, #0x20]
	ldr r2, [sp, #0x24]
	lsl r1, r1, #0xc
	lsl r2, r2, #0xc
	bl PokeathlonCirclePushAi_IsInZone
	cmp r0, #0
	beq _021F3970
	add r4, r4, #5
_021F3970:
	lsl r1, r5, #0x18
	ldr r0, [sp]
	lsr r1, r1, #0x18
	bl PokeathlonCirclePushAi_IsZoneCell
	cmp r0, #0
	beq _021F3984
	mov r0, #1
	add r4, r4, #4
	str r0, [r7]
_021F3984:
	ldr r0, [sp]
	add r1, r6, #0
	add r2, r5, #0
	bl PokeathlonCirclePushAi_CountRivals
	add r4, r4, r0
	b _021F39E8
_021F3992:
	ldr r1, [sp, #0x20]
	ldr r2, [sp, #0x24]
	ldr r0, [sp, #4]
	lsl r1, r1, #0xc
	lsl r2, r2, #0xc
	bl PokeathlonCirclePushAi_IsInZone
	cmp r0, #0
	beq _021F39A6
	add r4, r4, #5
_021F39A6:
	lsl r1, r5, #0x18
	ldr r0, [sp]
	lsr r1, r1, #0x18
	bl PokeathlonCirclePushAi_IsZoneCell
	cmp r0, #0
	beq _021F39BA
	mov r0, #1
	add r4, r4, #4
	str r0, [r7]
_021F39BA:
	ldr r0, [sp]
	add r1, r6, #0
	add r2, r5, #0
	bl PokeathlonCirclePushAi_CountRivals
	cmp r0, #0
	bne _021F39CA
	add r4, r4, #2
_021F39CA:
	ldr r1, [r6]
	cmp r1, r5
	bne _021F39D2
	add r4, r4, #2
_021F39D2:
	cmp r0, #0
	beq _021F39D8
	mov r4, #0
_021F39D8:
	ldr r2, [r6, #0xc]
	ldr r0, [sp, #4]
	ldmia r2!, {r1, r2}
	bl PokeathlonCirclePushAi_IsInZone
	cmp r0, #0
	beq _021F39E8
	add r4, r4, #4
_021F39E8:
	add r0, r4, #0
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end PokeathlonCirclePushAi_ScoreCell


	thumb_func_start PokeathlonCirclePushAi_CountRivals
PokeathlonCirclePushAi_CountRivals: ; 0x021F39F0
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r5, r0, #0
	add r0, r1, #0
	str r1, [sp]
	ldr r1, [r0]
	mov r0, #0
	mvn r0, r0
	str r2, [sp, #4]
	cmp r1, r0
	bne _021F3A0C
	add sp, #0xc
	mov r0, #0
	pop {r4, r5, r6, r7, pc}
_021F3A0C:
	ldr r0, [sp]
	mov r4, #0
	ldr r7, [r0, #0x14]
	add r6, r4, #0
_021F3A14:
	cmp r6, r7
	beq _021F3A4C
	ldr r1, [r5]
	ldr r0, [sp, #4]
	cmp r0, r1
	bne _021F3A4C
	ldr r0, [sp]
	ldr r1, [r0, #4]
	ldr r0, [r5, #4]
	cmp r1, r0
	bge _021F3A4A
	add r0, r7, #0
	mov r1, #3
	bl _s32_div_f
	str r0, [sp, #8]
	ldr r0, [r5, #0x14]
	mov r1, #3
	bl _s32_div_f
	ldr r1, [sp, #8]
	cmp r1, r0
	bne _021F3A46
	sub r4, r4, #1
	b _021F3A4C
_021F3A46:
	sub r4, r4, #2
	b _021F3A4C
_021F3A4A:
	sub r4, r4, #1
_021F3A4C:
	add r6, r6, #1
	add r5, #0x20
	cmp r6, #0xc
	blt _021F3A14
	add r0, r4, #0
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end PokeathlonCirclePushAi_CountRivals
	.endif


	.if 0
	thumb_func_start PokeathlonCirclePushAi_IsInZone
PokeathlonCirclePushAi_IsInZone: ; 0x021F3A5C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	mov r4, #0
	add r5, r0, #0
	str r1, [sp, #0xc]
	str r2, [sp, #0x10]
	str r4, [sp, #0x14]
	add r7, r4, #0
	add r6, sp, #0
_021F3A6E:
	ldr r0, [r5]
	cmp r0, #0
	ble _021F3AA8
	ldrh r0, [r5, #4]
	add r1, r6, #0
	add r2, r6, #0
	lsl r0, r0, #0xc
	str r0, [sp]
	ldrh r0, [r5, #6]
	lsl r0, r0, #0xc
	str r0, [sp, #4]
	add r0, sp, #0xc
	str r7, [sp, #8]
	bl VEC_Subtract
	add r0, r6, #0
	bl VEC_Mag
	ldr r1, [r5]
	lsl r1, r1, #0xc
	cmp r0, r1
	bge _021F3AA0
	add sp, #0x18
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_021F3AA0:
	add r4, r4, #1
	add r5, #8
	cmp r4, #4
	blt _021F3A6E
_021F3AA8:
	mov r0, #0
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end PokeathlonCirclePushAi_IsInZone


	thumb_func_start PokeathlonCirclePushAi_IsZoneCell
PokeathlonCirclePushAi_IsZoneCell: ; 0x021F3AB0
	push {r4, r5}
	mov r5, #0
	mov r2, #0x4a
	lsl r2, r2, #4
	sub r3, r5, #1
_021F3ABA:
	ldr r4, [r0, r2]
	cmp r4, r3
	beq _021F3AD2
	cmp r1, r4
	bne _021F3ACA
	mov r0, #1
	pop {r4, r5}
	bx lr
_021F3ACA:
	add r5, r5, #1
	add r0, r0, #4
	cmp r5, #4
	blt _021F3ABA
_021F3AD2:
	mov r0, #0
	pop {r4, r5}
	bx lr
	thumb_func_end PokeathlonCirclePushAi_IsZoneCell


	thumb_func_start PokeathlonCirclePushAi_SnapToZone
PokeathlonCirclePushAi_SnapToZone: ; 0x021F3AD8
	push {r3, r4, r5, r6}
	mov r4, #0x4a
	mov r6, #0
	lsl r4, r4, #4
_021F3AE0:
	ldr r5, [r0, r4]
	cmp r2, r5
	bne _021F3AF8
	lsl r0, r6, #3
	add r1, r1, r0
	ldrh r0, [r1, #4]
	str r0, [r3]
	ldrh r1, [r1, #6]
	ldr r0, [sp, #0x10]
	str r1, [r0]
	pop {r3, r4, r5, r6}
	bx lr
_021F3AF8:
	add r6, r6, #1
	add r0, r0, #4
	cmp r6, #4
	blt _021F3AE0
	pop {r3, r4, r5, r6}
	bx lr
	thumb_func_end PokeathlonCirclePushAi_SnapToZone


	thumb_func_start PokeathlonCirclePushAi_GetColumnRow
PokeathlonCirclePushAi_GetColumnRow: ; 0x021F3B04
	push {r3, r4}
	sub r0, #0x10
	asr r4, r0, #4
	lsr r4, r4, #0x1b
	add r4, r0, r4
	asr r0, r4, #5
	sub r1, #0x10
	strh r0, [r2]
	asr r0, r1, #4
	lsr r0, r0, #0x1b
	add r0, r1, r0
	asr r0, r0, #5
	strh r0, [r3]
	mov r0, #0
	ldrsh r1, [r2, r0]
	cmp r1, #0
	blt _021F3B2C
	ldrsh r0, [r3, r0]
	cmp r0, #0
	bge _021F3B32
_021F3B2C:
	mov r0, #0
	pop {r3, r4}
	bx lr
_021F3B32:
	mov r0, #1
	pop {r3, r4}
	bx lr
	thumb_func_end PokeathlonCirclePushAi_GetColumnRow
	.endif


	.if R56_GAP == 3
	thumb_func_start PokeathlonCirclePushAi_SortCells
PokeathlonCirclePushAi_SortCells: ; 0x021F3B38
	push {r4, r5, r6, r7, lr}
	sub sp, #0x13c
	add r6, r0, #0
	add r0, sp, #0x10
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp]
	add r7, r2, #0
	str r0, [sp, #0xc]
	mov ip, r1
	ldr r0, [sp]
	cmp r6, #0
	bls _021F3BAE
_021F3B54:
	add r2, r0, #1
	mov r1, #0xc
	mul r1, r2
	add r2, sp, #4
	lsl r4, r0, #2
	mov r3, ip
	ldr r3, [r3, r4]
	add r2, r2, r1
	str r3, [r2, #8]
	mov r3, #0
	str r3, [r2, #4]
	add r4, r3, #0
	add r3, sp, #4
	str r4, [r3, r1]
	add r1, r4, #0
	cmp r0, #0
	bls _021F3B9C
	ldr r4, [r2, #8]
	ldr r4, [r4, #0x18]
_021F3B7A:
	ldr r3, [r3, #4]
	ldr r5, [r3, #8]
	ldr r5, [r5, #0x18]
	cmp r5, r4
	bge _021F3B92
	ldr r4, [r3]
	str r2, [r4, #4]
	ldr r4, [r3]
	str r4, [r2]
	str r3, [r2, #4]
	str r2, [r3]
	b _021F3B9C
_021F3B92:
	add r1, r1, #1
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	cmp r1, r0
	blo _021F3B7A
_021F3B9C:
	cmp r1, r0
	bne _021F3BA4
	str r2, [r3, #4]
	str r3, [r2]
_021F3BA4:
	add r0, r0, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, r6
	blo _021F3B54
_021F3BAE:
	ldr r0, [sp, #8]
	mov r2, #0
	ldr r0, [r0, #8]
	cmp r6, #0
	ldr r1, [r0, #0x18]
	add r0, sp, #4
	bls _021F3BE0
_021F3BBC:
	ldr r0, [r0, #4]
	lsl r4, r2, #2
	ldr r3, [r0, #8]
	str r3, [r7, r4]
	ldr r3, [r7, r4]
	ldr r3, [r3, #0x18]
	cmp r1, r3
	bne _021F3BD6
	ldr r3, [sp]
	add r3, r3, #1
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	str r3, [sp]
_021F3BD6:
	add r2, r2, #1
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	cmp r2, r6
	blo _021F3BBC
_021F3BE0:
	ldr r0, [sp]
	cmp r0, #0
	bne _021F3BEA
	bl GF_AssertFail
_021F3BEA:
	ldr r0, [sp]
	add sp, #0x13c
	pop {r4, r5, r6, r7, pc}
	thumb_func_end PokeathlonCirclePushAi_SortCells
	.endif


	.if 0
	thumb_func_start PokeathlonCirclePushDisplay_New
PokeathlonCirclePushDisplay_New: ; 0x021F3BF0
	push {r3, r4, r5, r6, r7, lr}
	add r6, r1, #0
	mov r1, #0x5d
	lsl r1, r1, #2
	add r5, r0, #0
	add r7, r2, #0
	bl Heap_Alloc
	mov r2, #0x5d
	add r4, r0, #0
	mov r1, #0
	lsl r2, r2, #2
	bl MI_CpuFill8
	str r5, [r4]
	str r6, [r4, #8]
	add r0, r4, #0
	str r7, [r4, #4]
	bl PokeathlonCirclePushDisplay_CreateWindows
	ldr r2, _021F3C34 ; =0x00000135
	mov r0, #1
	mov r1, #0x1b
	add r3, r5, #0
	bl NewMsgDataFromNarc
	str r0, [r4, #0x5c]
	add r0, r5, #0
	bl MessageFormat_New
	str r0, [r4, #0x60]
	add r0, r4, #0
	pop {r3, r4, r5, r6, r7, pc}
	nop
_021F3C34: .word 0x00000135
	thumb_func_end PokeathlonCirclePushDisplay_New


	thumb_func_start PokeathlonCirclePushDisplay_Free
PokeathlonCirclePushDisplay_Free: ; 0x021F3C38
	push {r3, r4, r5, r6, r7, lr}
	add r7, r0, #0
	mov r4, #0
	add r5, r7, #0
_021F3C40:
	add r0, r5, #0
	add r0, #0x98
	ldr r0, [r0]
	bl Heap_Free
	add r0, r5, #0
	add r0, #0x9c
	ldr r0, [r0]
	bl Heap_Free
	add r4, r4, #1
	add r5, #8
	cmp r4, #0xc
	blt _021F3C40
	mov r6, #0x55
	mov r5, #0
	add r4, r7, #0
	lsl r6, r6, #2
_021F3C64:
	ldr r0, [r4, r6]
	bl Heap_Free
	add r5, r5, #1
	add r4, r4, #4
	cmp r5, #4
	blt _021F3C64
	mov r0, #0x59
	lsl r0, r0, #2
	ldr r0, [r7, r0]
	bl Heap_Free
	mov r6, #0x5a
	mov r5, #0
	add r4, r7, #0
	lsl r6, r6, #2
_021F3C84:
	ldr r0, [r4, r6]
	bl Heap_Free
	add r5, r5, #1
	add r4, r4, #4
	cmp r5, #2
	blt _021F3C84
	ldr r0, [r7, #0x60]
	bl MessageFormat_Delete
	ldr r0, [r7, #0x5c]
	bl DestroyMsgData
	add r4, r7, #0
	mov r5, #0
	add r4, #0xc
_021F3CA4:
	add r0, r4, #0
	bl RemoveWindow
	add r5, r5, #1
	add r4, #0x10
	cmp r5, #5
	blt _021F3CA4
	add r0, r7, #0
	bl Heap_Free
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end PokeathlonCirclePushDisplay_Free
	.endif


	.if R56_GAP == 4
	thumb_func_start PokeathlonCirclePushDisplay_LoadScreens
PokeathlonCirclePushDisplay_LoadScreens: ; 0x021F3CBC
	push {r3, r4, r5, lr}
	sub sp, #0x10
	add r5, r0, #0
	ldr r0, [r5]
	mov r1, #0xd
	str r0, [sp]
	mov r0, #0xa7
	mov r2, #0
	add r3, sp, #0xc
	bl GfGfxLoader_GetScrnData
	ldr r3, [sp, #0xc]
	add r4, r0, #0
	add r2, r3, #0
	ldr r0, [r5, #8]
	ldr r3, [r3, #8]
	mov r1, #5
	add r2, #0xc
	bl BG_LoadScreenTilemapData
	mov r0, #0x20
	str r0, [sp]
	mov r0, #0x18
	str r0, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	mov r2, #0
	ldr r0, [r5, #8]
	mov r1, #5
	add r3, r2, #0
	bl BgTilemapRectChangePalette
	ldr r0, [r5, #8]
	mov r1, #5
	bl ScheduleBgTilemapBufferTransfer
	add r0, r4, #0
	bl Heap_Free
	mov r1, #0x17
	ldr r0, [r5]
	add r3, r1, #0
	add r3, #0xe9
	str r0, [sp]
	mov r0, #0xa7
	mov r2, #0
	add r3, r5, r3
	bl GfGfxLoader_GetScrnData
	mov r1, #0x5a
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r1, #0x18
	ldr r0, [r5]
	add r3, r1, #0
	add r3, #0xec
	str r0, [sp]
	mov r0, #0xa7
	mov r2, #0
	add r3, r5, r3
	bl GfGfxLoader_GetScrnData
	mov r1, #0x5b
	lsl r1, r1, #2
	str r0, [r5, r1]
	sub r1, #0x6c
	ldr r3, [r5, r1]
	ldr r0, [r5, #8]
	add r2, r3, #0
	ldr r3, [r3, #8]
	mov r1, #7
	add r2, #0xc
	bl BG_LoadScreenTilemapData
	mov r0, #8
	mov r1, #0
	bl GfGfx_EngineBTogglePlanes
	mov r1, #0x12
	ldr r0, [r5]
	add r3, r1, #0
	add r3, #0xfa
	str r0, [sp]
	mov r0, #0xa7
	mov r2, #0
	add r3, r5, r3
	bl GfGfxLoader_GetScrnData
	mov r1, #0x55
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r1, #0x13
	ldr r0, [r5]
	add r3, r1, #0
	add r3, #0xfd
	str r0, [sp]
	mov r0, #0xa7
	mov r2, #0
	add r3, r5, r3
	bl GfGfxLoader_GetScrnData
	mov r1, #0x56
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r1, #0x11
	ldr r0, [r5]
	add r3, r1, #0
	add r3, #0xf7
	str r0, [sp]
	mov r0, #0xa7
	mov r2, #0
	add r3, r5, r3
	bl GfGfxLoader_GetScrnData
	mov r1, #0x57
	lsl r1, r1, #2
	str r0, [r5, r1]
	ldr r0, [r5]
	mov r1, #0x14
	str r0, [sp]
	mov r0, #0xa7
	add r3, r0, #0
	add r3, #0x6d
	mov r2, #0
	add r3, r5, r3
	bl GfGfxLoader_GetScrnData
	mov r1, #0x16
	lsl r1, r1, #4
	str r0, [r5, r1]
	ldr r0, [r5]
	mov r1, #0x10
	str r0, [sp]
	mov r0, #0xa7
	add r3, r0, #0
	add r3, #0x71
	mov r2, #0
	add r3, r5, r3
	bl GfGfxLoader_GetScrnData
	mov r1, #0x59
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r0, #0x80
	add r1, r0, #0
	add r1, #0x8c
	ldr r1, [r5, r1]
	bl PokeathlonCirclePushDisplay_OffsetScreenTiles
	mov r0, #0x80
	add r1, r0, #0
	add r1, #0x90
	ldr r1, [r5, r1]
	bl PokeathlonCirclePushDisplay_OffsetScreenTiles
	mov r0, #0x80
	add r1, r0, #0
	add r1, #0x94
	ldr r1, [r5, r1]
	bl PokeathlonCirclePushDisplay_OffsetScreenTiles
	mov r0, #0x80
	add r1, r0, #0
	add r1, #0x88
	ldr r1, [r5, r1]
	bl PokeathlonCirclePushDisplay_OffsetScreenTiles
	mov r0, #0x80
	add r1, r0, #0
	add r1, #0x98
	ldr r1, [r5, r1]
	bl PokeathlonCirclePushDisplay_OffsetScreenTiles
	mov r0, #0x43
	lsl r0, r0, #2
	ldr r3, [r5, r0]
	ldr r0, [r5, #8]
	add r2, r3, #0
	ldr r3, [r3, #8]
	mov r1, #6
	add r2, #0xc
	bl BG_LoadScreenTilemapData
	add r0, r5, #0
	ldr r1, [r5, #8]
	add r0, #0x68
	bl PokeathlonCirclePushDisplayAnim_PrintScore
	mov r0, #0x20
	str r0, [sp]
	mov r0, #0x18
	str r0, [sp, #4]
	mov r0, #3
	str r0, [sp, #8]
	mov r2, #0
	ldr r0, [r5, #8]
	mov r1, #6
	add r3, r2, #0
	bl BgTilemapRectChangePalette
	ldr r0, [r5, #8]
	mov r1, #6
	bl ScheduleBgTilemapBufferTransfer
	add sp, #0x10
	pop {r3, r4, r5, pc}
	thumb_func_end PokeathlonCirclePushDisplay_LoadScreens
	.endif
