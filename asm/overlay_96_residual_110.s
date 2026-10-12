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
	.public PokeathlonRelayRunHud_GetBenchMember
	.public PokeathlonRelayRunCpu_SetTeam
	.public PokeathlonRelayRunCpu_Delete
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

	thumb_func_start PokeathlonGoalRollDisplay_UpdateTimer
PokeathlonGoalRollDisplay_UpdateTimer: ; 0x0221490C
	push {r3, r4, r5, lr}
	add r5, r0, #0
	mov r0, #0xe1
	lsl r0, r0, #2
	cmp r1, r0
	bgt _02214944
	ldr r0, [r5, #8]
	mov r1, #0
	bl Sprite_SetDrawFlag
	ldr r0, [r5, #0xc]
	mov r1, #0
	bl Sprite_SetDrawFlag
	ldr r0, [r5, #0x4c]
	mov r1, #1
	mov r2, #0
	bl PokeathlonCourseSpritePoolSprite_Show
	ldr r0, [r5, #0x48]
	cmp r0, #0
	bne _0221497A
	add r0, r5, #0
	bl PokeathlonGoalRollDisplay_PrintSecondWind
	mov r0, #1
	str r0, [r5, #0x48]
	pop {r3, r4, r5, pc}
_02214944:
	add r0, r1, #0
	mov r1, #0x1e
	bl _s32_div_f
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	add r0, r4, #0
	mov r1, #0xa
	bl _s32_div_f
	add r1, r0, #0
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	ldr r0, [r5, #8]
	add r1, r1, #1
	bl Sprite_SetAnimCtrlSeq
	add r0, r4, #0
	mov r1, #0xa
	bl _s32_div_f
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	ldr r0, [r5, #0xc]
	add r1, r1, #1
	bl Sprite_SetAnimCtrlSeq
_0221497A:
	pop {r3, r4, r5, pc}
	thumb_func_end PokeathlonGoalRollDisplay_UpdateTimer


	thumb_func_start PokeathlonGoalRollDisplay_DrawScore
PokeathlonGoalRollDisplay_DrawScore: ; 0x0221497C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x20
	add r6, r1, #0
	add r5, r0, #0
	add r0, r6, #0
	mov r1, #0xa
	bl _s32_div_f
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	add r0, r4, #0
	mov r1, #5
	bl _s32_div_f
	lsl r1, r1, #2
	add r0, sp, #0x1c
	strb r1, [r0, #2]
	add r0, r4, #0
	mov r1, #5
	bl _s32_div_f
	lsl r1, r0, #3
	add r0, sp, #0x1c
	strb r1, [r0]
	add r0, r6, #0
	mov r1, #0xa
	bl _s32_div_f
	lsl r0, r1, #0x18
	lsr r4, r0, #0x18
	add r0, r4, #0
	mov r1, #5
	bl _s32_div_f
	lsl r1, r1, #2
	add r0, sp, #0x1c
	strb r1, [r0, #3]
	add r0, r4, #0
	mov r1, #5
	bl _s32_div_f
	lsl r1, r0, #3
	add r0, sp, #0x1c
	strb r1, [r0, #1]
	mov r4, #0
	mov r6, #4
	mov r7, #8
_022149DA:
	str r6, [sp]
	str r7, [sp, #4]
	ldr r0, [r5, #0x3c]
	lsl r2, r4, #2
	add r0, #0xc
	str r0, [sp, #8]
	add r0, sp, #0x1c
	add r0, #2
	ldrb r0, [r0, r4]
	add r2, r2, #6
	lsl r2, r2, #0x18
	str r0, [sp, #0xc]
	add r0, sp, #0x1c
	ldrb r0, [r0, r4]
	mov r1, #6
	lsr r2, r2, #0x18
	str r0, [sp, #0x10]
	mov r0, #0x14
	str r0, [sp, #0x14]
	mov r0, #0x10
	str r0, [sp, #0x18]
	ldr r0, [r5, #0x10]
	mov r3, #0xa
	bl CopyRectToBgTilemapRect
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	cmp r4, #2
	blo _022149DA
	ldr r0, [r5, #0x10]
	mov r1, #6
	bl ScheduleBgTilemapBufferTransfer
	add sp, #0x20
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end PokeathlonGoalRollDisplay_DrawScore


	thumb_func_start PokeathlonGoalRollCpu_New
PokeathlonGoalRollCpu_New: ; 0x02214A24
	push {r3, r4, r5, lr}
	add r5, r1, #0
	mov r1, #0xa0
	bl Heap_Alloc
	add r4, r0, #0
	mov r1, #0
	mov r2, #0xa0
	bl MI_CpuFill8
	add r0, r4, #0
	add r0, #0x98
	strb r5, [r0]
	mov r0, #4
	sub r1, r0, r5
	add r0, r4, #0
	add r0, #0x9e
	strb r1, [r0]
	add r0, r4, #0
	add r0, #0x9e
	ldrb r1, [r0]
	add r0, r4, #0
	add r0, #0x9d
	strb r1, [r0]
	add r0, r4, #0
	mov r1, #3
	add r0, #0x9b
	strb r1, [r0]
	add r0, r4, #0
	add r0, #0x9b
	ldrb r1, [r0]
	add r0, r4, #0
	add r0, #0x9a
	strb r1, [r0]
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	thumb_func_end PokeathlonGoalRollCpu_New


	thumb_func_start PokeathlonGoalRollCpu_SetMon
PokeathlonGoalRollCpu_SetMon: ; 0x02214A6C
	push {r3, r4, r5, r6, r7, lr}
	add r4, r1, #0
	add r5, r0, #0
	add r0, r4, #0
	mov r1, #3
	add r6, r2, #0
	bl _s32_div_f
	add r7, r0, #0
	add r0, r4, #0
	mov r1, #3
	bl _s32_div_f
	lsl r0, r7, #0x18
	lsr r2, r0, #0x18
	mov r0, #0x24
	mul r0, r2
	add r2, r5, r0
	lsl r0, r1, #0x18
	lsr r0, r0, #0x16
	add r0, r2, r0
	str r6, [r0, #0x18]
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end PokeathlonGoalRollCpu_SetMon


	thumb_func_start PokeathlonGoalRollCpu_ChooseAllRoles
PokeathlonGoalRollCpu_ChooseAllRoles: ; 0x02214A9C
	push {r4, r5, r6, lr}
	add r5, r0, #0
	mov r4, #0
	mov r6, #0x24
_02214AA4:
	add r0, r4, #0
	mul r0, r6
	add r0, r5, r0
	bl PokeathlonGoalRollCpu_ChooseRoles
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	cmp r4, #4
	blo _02214AA4
	pop {r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end PokeathlonGoalRollCpu_ChooseAllRoles


	thumb_func_start PokeathlonGoalRollCpu_ChooseRoles
PokeathlonGoalRollCpu_ChooseRoles: ; 0x02214ABC
	push {r3, r4, r5, r6}
	sub sp, #8
	add r1, sp, #0
	add r5, r0, #0
	mov r4, #0
	add r1, #3
	add r0, sp, #0
_02214ACA:
	lsl r2, r4, #2
	add r2, r5, r2
	ldr r3, [r2, #0x18]
	add r2, r3, #0
	add r6, r3, #0
	add r2, #0x73
	add r6, #0x70
	ldrb r2, [r2]
	ldrb r6, [r6]
	add r3, #0x75
	add r6, r2, r6
	strb r6, [r1, r4]
	ldrb r3, [r3]
	add r2, r2, r3
	strb r2, [r0, r4]
	add r2, r4, #1
	lsl r2, r2, #0x18
	lsr r4, r2, #0x18
	cmp r4, #3
	blo _02214ACA
	add r3, sp, #0
	mov r0, #0
	mov r4, #1
	add r3, #3
_02214AFA:
	add r1, r3, r4
	sub r1, r1, #1
	ldrb r2, [r1]
	ldrb r1, [r3, r4]
	cmp r2, r1
	bhs _02214B08
	add r0, r4, #0
_02214B08:
	add r1, r4, #1
	lsl r1, r1, #0x18
	lsr r4, r1, #0x18
	cmp r4, #3
	blo _02214AFA
	cmp r0, #0
	add r1, sp, #0
	bne _02214B2C
	ldrb r2, [r1, #1]
	ldrb r1, [r1, #2]
	cmp r2, r1
	blo _02214B26
	mov r2, #1
	mov r1, #2
	b _02214B56
_02214B26:
	mov r2, #2
	mov r1, #1
	b _02214B56
_02214B2C:
	cmp r0, #1
	bne _02214B44
	ldrb r2, [r1]
	ldrb r1, [r1, #2]
	cmp r2, r1
	blo _02214B3E
	mov r2, #0
	mov r1, #2
	b _02214B56
_02214B3E:
	mov r2, #2
	mov r1, #0
	b _02214B56
_02214B44:
	ldrb r2, [r1]
	ldrb r1, [r1, #1]
	cmp r2, r1
	blo _02214B52
	mov r2, #0
	mov r1, #1
	b _02214B56
_02214B52:
	mov r2, #1
	mov r1, #0
_02214B56:
	lsl r0, r0, #2
	add r0, r5, r0
	ldr r0, [r0, #0x18]
	str r0, [r5, #4]
	lsl r0, r2, #2
	add r0, r5, r0
	ldr r0, [r0, #0x18]
	str r0, [r5, #0xc]
	lsl r0, r1, #2
	add r0, r5, r0
	ldr r0, [r0, #0x18]
	str r0, [r5, #0x14]
	add sp, #8
	pop {r3, r4, r5, r6}
	bx lr
	thumb_func_end PokeathlonGoalRollCpu_ChooseRoles
