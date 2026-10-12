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
	.public PokeathlonRelayRun_Draw
	.public PokeathlonRelayRun_IsTouchingSwapButton
	.public PokeathlonRelayRunTeam_UpdateStun
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
	.public PokeathlonRingDropCpu_Free
	.public PokeathlonEvent_PennantCapturePoints
	.public PokeathlonEvent_CirclePushPoints
	.public PokeathlonEvent_BlockSmashPoints
	.public PokeathlonEvent_DiscCatchPoints
	.public PokeathlonEvent_SnowThrowPoints


	thumb_func_start PokeathlonRingDropMon_Update
PokeathlonRingDropMon_Update: ; 0x02218B1C
	push {r4, r5, lr}
	sub sp, #0xc
	add r4, r0, #0
	mov r2, #0x5d
	ldrsb r0, [r4, r2]
	cmp r0, #0
	ble _02218B3A
	sub r3, r0, #1
	add r0, r4, #0
	add r0, #0x5d
	strb r3, [r0]
	ldrsb r0, [r4, r2]
	cmp r0, #0
	ble _02218B3A
	b _02218DEA
_02218B3A:
	ldr r0, [r4, #0x14]
	cmp r0, #0xc
	bhi _02218B6E
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02218B4C: ; jump table
	.short _02218B6E - _02218B4C - 2 ; case 0
	.short _02218B6E - _02218B4C - 2 ; case 1
	.short _02218B6E - _02218B4C - 2 ; case 2
	.short _02218B6E - _02218B4C - 2 ; case 3
	.short _02218B6E - _02218B4C - 2 ; case 4
	.short _02218B74 - _02218B4C - 2 ; case 5
	.short _02218B66 - _02218B4C - 2 ; case 6
	.short _02218B74 - _02218B4C - 2 ; case 7
	.short _02218B74 - _02218B4C - 2 ; case 8
	.short _02218B6E - _02218B4C - 2 ; case 9
	.short _02218B74 - _02218B4C - 2 ; case 10
	.short _02218B74 - _02218B4C - 2 ; case 11
	.short _02218B74 - _02218B4C - 2 ; case 12
_02218B66:
	add r0, r4, #0
	bl PokeathlonRingDropMon_UpdateDash
	b _02218B74
_02218B6E:
	add r0, r4, #0
	bl PokeathlonRingDropMon_Move
_02218B74:
	ldr r0, [r4, #0x14]
	cmp r0, #0xc
	bls _02218B7C
	b _02218DE0
_02218B7C:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02218B88: ; jump table
	.short _02218DE0 - _02218B88 - 2 ; case 0
	.short _02218C4E - _02218B88 - 2 ; case 1
	.short _02218C20 - _02218B88 - 2 ; case 2
	.short _02218BFE - _02218B88 - 2 ; case 3
	.short _02218BA2 - _02218B88 - 2 ; case 4
	.short _02218D62 - _02218B88 - 2 ; case 5
	.short _02218D84 - _02218B88 - 2 ; case 6
	.short _02218DC6 - _02218B88 - 2 ; case 7
	.short _02218C88 - _02218B88 - 2 ; case 8
	.short _02218CAC - _02218B88 - 2 ; case 9
	.short _02218CEA - _02218B88 - 2 ; case 10
	.short _02218D0E - _02218B88 - 2 ; case 11
	.short _02218D46 - _02218B88 - 2 ; case 12
_02218BA2:
	ldr r0, [r4, #0x60]
	lsl r0, r0, #4
	lsr r0, r0, #0x1f
	beq _02218BAE
	bl GF_AssertFail
_02218BAE:
	add r0, r4, #0
	bl PokeathlonRingDropMon_IsOutOfRing
	cmp r0, #0
	beq _02218BC0
	add r0, r4, #0
	bl PokeathlonRingDropMon_FallOut
	b _02218DE4
_02218BC0:
	ldr r0, [r4, #0x60]
	lsl r1, r0, #2
	lsr r1, r1, #0x1f
	beq _02218BD0
	add r0, r4, #0
	bl PokeathlonRingDropMon_StartJump
	b _02218DE4
_02218BD0:
	lsl r0, r0, #1
	lsr r0, r0, #0x1f
	beq _02218BE2
	add r1, r4, #0
	add r0, r4, #0
	add r1, #0x1c
	bl PokeathlonRingDropMon_StartDash
	b _02218DE4
_02218BE2:
	mov r1, #0x5e
	ldrsb r0, [r4, r1]
	sub r2, r0, #1
	add r0, r4, #0
	add r0, #0x5e
	strb r2, [r0]
	ldrsb r0, [r4, r1]
	cmp r0, #0
	bgt _02218CD8
	add r0, r4, #0
	mov r1, #1
	bl PokeathlonRingDropMon_SetState
	b _02218DE4
_02218BFE:
	mov r1, #0x5b
	ldrsb r0, [r4, r1]
	sub r2, r0, #1
	add r0, r4, #0
	add r0, #0x5b
	strb r2, [r0]
	ldrsb r0, [r4, r1]
	cmp r0, #0
	bgt _02218CD8
	add r0, r4, #0
	bl PokeathlonRingDropMon_Respawn
	add r0, r4, #0
	mov r1, #4
	bl PokeathlonRingDropMon_SetState
	b _02218DE4
_02218C20:
	add r0, r4, #0
	bl PokeathlonRingDropMon_IsOutOfRing
	cmp r0, #0
	beq _02218C32
	add r0, r4, #0
	bl PokeathlonRingDropMon_FallOut
	b _02218DE4
_02218C32:
	mov r1, #0x58
	ldrsb r0, [r4, r1]
	sub r2, r0, #1
	add r0, r4, #0
	add r0, #0x58
	strb r2, [r0]
	ldrsb r0, [r4, r1]
	cmp r0, #0
	bgt _02218CD8
	add r0, r4, #0
	mov r1, #1
	bl PokeathlonRingDropMon_SetState
	b _02218DE4
_02218C4E:
	add r0, r4, #0
	bl PokeathlonRingDropMon_IsOutOfRing
	cmp r0, #0
	beq _02218C60
	add r0, r4, #0
	bl PokeathlonRingDropMon_FallOut
	b _02218DE4
_02218C60:
	ldr r1, [r4, #0x60]
	lsl r0, r1, #4
	lsr r0, r0, #0x1f
	bne _02218CD8
	lsl r0, r1, #2
	lsr r0, r0, #0x1f
	beq _02218C76
	add r0, r4, #0
	bl PokeathlonRingDropMon_StartJump
	b _02218DE4
_02218C76:
	lsl r0, r1, #1
	lsr r0, r0, #0x1f
	beq _02218CD8
	add r1, r4, #0
	add r0, r4, #0
	add r1, #0x1c
	bl PokeathlonRingDropMon_StartDash
	b _02218DE4
_02218C88:
	mov r1, #0x5a
	ldrsb r0, [r4, r1]
	sub r2, r0, #1
	add r0, r4, #0
	add r0, #0x5a
	strb r2, [r0]
	ldrsb r0, [r4, r1]
	cmp r0, #0
	bgt _02218CD8
	add r0, r4, #0
	mov r1, #0
	add r0, #0x5a
	strb r1, [r0]
	add r0, r4, #0
	mov r1, #9
	bl PokeathlonRingDropMon_SetState
	b _02218DE4
_02218CAC:
	ldr r0, [r4, #4]
	ldr r3, _02218DF0 ; =ov96_0221D7D0
	ldr r0, [r0, #4]
	ldrb r1, [r0, #0x18]
	mov r0, #0x14
	add r5, r1, #0
	mov r1, #0x5a
	mul r5, r0
	ldrsb r0, [r4, r1]
	lsl r2, r0, #1
	add r0, r3, r5
	ldrsh r0, [r2, r0]
	lsl r0, r0, #0xc
	str r0, [r4, #0x50]
	ldrsb r0, [r4, r1]
	add r2, r0, #1
	add r0, r4, #0
	add r0, #0x5a
	strb r2, [r0]
	ldrsb r0, [r4, r1]
	cmp r0, #0xa
	bge _02218CDA
_02218CD8:
	b _02218DE4
_02218CDA:
	add r0, r4, #0
	mov r1, #0xa
	add r0, #0x5a
	strb r1, [r0]
	add r0, r4, #0
	bl PokeathlonRingDropMon_SetState
	b _02218DE4
_02218CEA:
	mov r1, #0x5a
	ldrsb r0, [r4, r1]
	sub r2, r0, #1
	add r0, r4, #0
	add r0, #0x5a
	strb r2, [r0]
	ldrsb r0, [r4, r1]
	cmp r0, #0
	bgt _02218DE4
	add r0, r4, #0
	mov r1, #0
	add r0, #0x5a
	strb r1, [r0]
	add r0, r4, #0
	mov r1, #0xb
	bl PokeathlonRingDropMon_SetState
	b _02218DE4
_02218D0E:
	ldr r0, [r4, #0x50]
	cmp r0, #0
	bne _02218D2C
	add r0, r4, #0
	bl PokeathlonRingDropMon_Land
	add r0, r4, #0
	bl PokeathlonRingDropMon_IsOutOfRing
	cmp r0, #0
	beq _02218DE4
	add r0, r4, #0
	bl PokeathlonRingDropMon_FallOut
	b _02218DE4
_02218D2C:
	ldr r1, [r4, #4]
	ldr r1, [r1, #4]
	ldrb r2, [r1, #0x18]
	ldr r1, _02218DF4 ; =ov96_0221D69C
	ldrb r1, [r1, r2]
	lsl r1, r1, #0xc
	sub r0, r0, r1
	str r0, [r4, #0x50]
	cmp r0, #0
	bgt _02218DE4
	mov r0, #0
	str r0, [r4, #0x50]
	b _02218DE4
_02218D46:
	mov r1, #0x5a
	ldrsb r0, [r4, r1]
	sub r2, r0, #1
	add r0, r4, #0
	add r0, #0x5a
	strb r2, [r0]
	ldrsb r0, [r4, r1]
	cmp r0, #0
	bgt _02218DE4
	add r0, r4, #0
	mov r1, #1
	bl PokeathlonRingDropMon_SetState
	b _02218DE4
_02218D62:
	mov r1, #0x59
	ldrsb r0, [r4, r1]
	sub r2, r0, #1
	add r0, r4, #0
	add r0, #0x59
	strb r2, [r0]
	ldrsb r0, [r4, r1]
	cmp r0, #0
	bgt _02218DE4
	add r0, r4, #0
	mov r1, #6
	add r0, #0x59
	strb r1, [r0]
	add r0, r4, #0
	bl PokeathlonRingDropMon_SetState
	b _02218DE4
_02218D84:
	mov r1, #0x59
	ldrsb r0, [r4, r1]
	sub r2, r0, #1
	add r0, r4, #0
	add r0, #0x59
	strb r2, [r0]
	ldrsb r0, [r4, r1]
	cmp r0, #0
	bgt _02218DE4
	add r3, r4, #0
	add r3, #0x2c
	add r2, r4, #0
	ldmia r3!, {r0, r1}
	add r2, #0x20
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	add r3, sp, #0
	str r0, [r2]
	mov r0, #0
	str r0, [r3]
	str r0, [r3, #4]
	str r0, [r3, #8]
	add r2, r4, #0
	ldmia r3!, {r0, r1}
	add r2, #0x44
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	mov r1, #7
	str r0, [r2]
	add r0, r4, #0
	bl PokeathlonRingDropMon_SetState
	b _02218DE4
_02218DC6:
	mov r1, #0x59
	ldrsb r0, [r4, r1]
	sub r2, r0, #1
	add r0, r4, #0
	add r0, #0x59
	strb r2, [r0]
	ldrsb r0, [r4, r1]
	cmp r0, #0
	bgt _02218DE4
	add r0, r4, #0
	bl PokeathlonRingDropMon_EndDash
	b _02218DE4
_02218DE0:
	bl GF_AssertFail
_02218DE4:
	add r0, r4, #0
	bl PokeathlonRingDropMon_ApplyGravity
_02218DEA:
	add sp, #0xc
	pop {r4, r5, pc}
	nop
_02218DF0: .word ov96_0221D7D0
_02218DF4: .word ov96_0221D69C
	thumb_func_end PokeathlonRingDropMon_Update





	thumb_func_start PokeathlonRingDropMon_StartDash
PokeathlonRingDropMon_StartDash: ; 0x02218DF8
	push {r4, r5, lr}
	sub sp, #0xc
	add r5, r0, #0
	add r4, r1, #0
	add r0, sp, #0
	mov r1, #0
	str r1, [r0]
	str r1, [r0, #4]
	str r1, [r0, #8]
	ldr r0, [r5, #0x14]
	mov r2, #1
	cmp r0, #1
	beq _02218E18
	cmp r0, #4
	beq _02218E18
	add r2, r1, #0
_02218E18:
	cmp r2, #0
	bne _02218E20
	bl GF_AssertFail
_02218E20:
	ldr r0, [r5, #0x60]
	lsl r0, r0, #3
	lsr r0, r0, #0x1f
	beq _02218E2C
	bl GF_AssertFail
_02218E2C:
	ldr r0, [r5, #0x60]
	lsl r0, r0, #5
	lsr r0, r0, #0x1f
	beq _02218E38
	bl GF_AssertFail
_02218E38:
	ldr r1, [r5, #0x60]
	ldr r0, _02218EB0 ; =0xBFFFFFFF
	add r2, r5, #0
	and r0, r1
	str r0, [r5, #0x60]
	mov r0, #0
	ldrsh r0, [r4, r0]
	add r1, r5, #0
	add r1, #0x2c
	lsl r0, r0, #0xc
	str r0, [sp]
	mov r0, #2
	ldrsh r0, [r4, r0]
	add r2, #0x38
	lsl r0, r0, #0xc
	str r0, [sp, #4]
	add r0, sp, #0
	bl VEC_Subtract
	add r0, r5, #0
	add r0, #0x38
	add r1, r0, #0
	bl VEC_Normalize
	add r1, r5, #0
	mov r0, #1
	add r1, #0x38
	add r3, r5, #0
	lsl r0, r0, #0xe
	add r2, r1, #0
	add r3, #0x44
	bl VEC_MultAdd
	ldr r0, [r5, #0x44]
	cmp r0, #0
	bne _02218E86
	ldr r0, [r5, #0x48]
	cmp r0, #0
	beq _02218EAC
_02218E86:
	add r0, r5, #0
	add r0, #0x38
	bl PokeathlonRingDrop_VecToDirection
	lsl r0, r0, #0x1c
	ldr r2, [r5, #0x60]
	ldr r1, _02218EB4 ; =0xFFF0FFFF
	lsr r0, r0, #0xc
	and r1, r2
	orr r0, r1
	str r0, [r5, #0x60]
	add r0, r5, #0
	mov r1, #4
	add r0, #0x59
	strb r1, [r0]
	add r0, r5, #0
	mov r1, #5
	bl PokeathlonRingDropMon_SetState
_02218EAC:
	add sp, #0xc
	pop {r4, r5, pc}
	.balign 4, 0
_02218EB0: .word 0xBFFFFFFF
_02218EB4: .word 0xFFF0FFFF
	thumb_func_end PokeathlonRingDropMon_StartDash
