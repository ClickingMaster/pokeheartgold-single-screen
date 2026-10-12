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
	.public PokeathlonEvent_Event8Points
	.public PokeathlonRingDropUi_WaitSwitch
	.public PokeathlonRingDropUi_SetSwitching
	.public PokeathlonRingDropUi_AddWindows
	.public PokeathlonRingDropUi_DrawScore
	.public PokeathlonRingDropUi_PrintNames
	.public PokeathlonRingDropUi_PrintMessage
	.public PokeathlonRingDropUi_PrintMessageAt
	.public PokeathlonRingDropUi_LoadMonPic
	.public PokeathlonRingDropUi_LoadMonIcon
	.public PokeathlonRingDropUi_SetPicPosition
	.public PokeathlonRingDropUi_ShowSwitching


	thumb_func_start PokeathlonRingDropUi_NewTimerSprite
PokeathlonRingDropUi_NewTimerSprite: ; 0x02219E00
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x38
	ldr r6, _02219E5C ; =ov96_0221D944
	add r7, r2, #0
	str r3, [sp]
	add r5, r0, #0
	add r4, r1, #0
	add r3, sp, #4
	mov r2, #6
_02219E12:
	ldmia r6!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, r2, #1
	bne _02219E12
	ldr r0, [r6]
	cmp r5, #0
	str r0, [r3]
	bne _02219E26
	bl GF_AssertFail
_02219E26:
	cmp r4, #0
	bne _02219E2E
	bl GF_AssertFail
_02219E2E:
	add r1, sp, #4
	strh r7, [r1]
	ldr r0, [sp]
	mov r3, #0x1e
	strh r0, [r1, #2]
	add r0, sp, #0x40
	ldrh r2, [r0, #0x10]
	lsl r3, r3, #0x10
	strh r2, [r1, #6]
	ldrh r0, [r0, #0x14]
	add r1, r4, #0
	add r2, sp, #4
	str r0, [sp, #0xc]
	add r0, r5, #0
	bl SpriteSystem_NewSpriteWithYOffset
	mov r1, #1
	add r4, r0, #0
	bl ManagedSprite_SetAnimateFlag
	add r0, r4, #0
	add sp, #0x38
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02219E5C: .word ov96_0221D944
	thumb_func_end PokeathlonRingDropUi_NewTimerSprite




	thumb_func_start PokeathlonRingDropUi_CreateTimerSprites
PokeathlonRingDropUi_CreateTimerSprites: ; 0x02219E60
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r6, r0, #0
	mov r0, #0x14
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, [r6, #8]
	ldr r1, [r6, #0xc]
	mov r2, #0x72
	mov r3, #0xb4
	bl PokeathlonRingDropUi_NewTimerSprite
	mov r1, #0xe
	str r0, [r6, #0x64]
	bl ManagedSprite_SetPaletteOverride
	mov r7, #0
	add r4, r6, #0
	mov r5, #0x86
_02219E88:
	mov r1, #0
	add r0, sp, #8
	str r1, [r0]
	str r1, [r0, #4]
	str r1, [r0, #8]
	ldr r0, [r6, #0xc]
	bl SpriteManager_GetSpriteList
	add r1, r0, #0
	ldr r0, [r6, #0x10]
	ldr r3, [r6, #4]
	mov r2, #1
	bl ov96_021EA374
	add r1, r4, #0
	add r1, #0xa8
	str r0, [r1]
	lsl r0, r5, #0xc
	str r0, [sp, #8]
	mov r0, #0x2d
	lsl r0, r0, #0xe
	str r0, [sp, #0xc]
	mov r0, #0
	str r0, [sp, #0x10]
	add r0, r4, #0
	add r0, #0xa8
	ldr r0, [r0]
	add r1, sp, #8
	bl Sprite_SetMatrix
	add r0, r4, #0
	add r0, #0xa8
	ldr r0, [r0]
	mov r1, #1
	bl Sprite_SetDrawFlag
	add r7, r7, #1
	add r4, r4, #4
	add r5, #0x10
	cmp r7, #2
	blt _02219E88
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end PokeathlonRingDropUi_CreateTimerSprites




	thumb_func_start PokeathlonRingDropUi_PrintNextName
PokeathlonRingDropUi_PrintNextName: ; 0x02219EE0
	push {r4, lr}
	sub sp, #8
	add r4, r0, #0
	add r0, r1, #1
	mov r1, #3
	bl _s32_div_f
	lsl r2, r1, #0x18
	mov r0, #1
	lsr r2, r2, #0x16
	add r2, r4, r2
	str r0, [sp]
	mov r3, #2
	str r3, [sp, #4]
	add r2, #0xb0
	ldr r0, [r4, #0x1c]
	ldr r2, [r2]
	mov r1, #0
	bl BufferString
	ldr r0, [r4, #4]
	mov r3, #0x9a
	str r0, [sp]
	add r0, r4, #0
	ldr r1, [r4, #0x1c]
	ldr r2, [r4, #0x18]
	add r0, #0x40
	bl PokeathlonRingDropUi_PrintMessage
	add sp, #8
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end PokeathlonRingDropUi_PrintNextName
