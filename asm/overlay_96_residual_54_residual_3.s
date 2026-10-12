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
	.public PokeathlonCirclePush_Cosine
	.public PokeathlonCirclePush_RecoverStamina
	.public PokeathlonCirclePush_ShakeTask
	.public PokeathlonCirclePush_StartBounce
	.public PokeathlonCirclePush_BounceTask
	.public PokeathlonCirclePush_CompareDepth
	.public PokeathlonCirclePushZoneMarkers_New


	thumb_func_start PokeathlonCirclePush_SetMonStats
PokeathlonCirclePush_SetMonStats: ; 0x021F2B24
	push {r3, r4, r5, lr}
	add r5, r1, #0
	add r1, r2, #0
	add r2, r3, #0
	ldr r4, [sp, #0x10]
	bl PokeathlonCourse_GetMonStats
	ldrb r1, [r0]
	lsl r1, r1, #2
	ldr r2, [r5, r1]
	add r1, r4, #0
	add r1, #0x8a
	strh r2, [r1]
	ldrb r1, [r0, #3]
	lsl r1, r1, #2
	add r1, r5, r1
	ldr r2, [r1, #0x14]
	add r1, r4, #0
	add r1, #0x8c
	strh r2, [r1]
	ldrb r1, [r0, #3]
	lsl r1, r1, #2
	add r1, r5, r1
	ldr r2, [r1, #0x14]
	add r1, r4, #0
	add r1, #0x8e
	strh r2, [r1]
	ldrb r0, [r0, #1]
	add r4, #0x89
	lsl r0, r0, #2
	add r0, r5, r0
	ldr r0, [r0, #0x28]
	strb r0, [r4]
	pop {r3, r4, r5, pc}
	thumb_func_end PokeathlonCirclePush_SetMonStats




	thumb_func_start PokeathlonCirclePush_StartLayoutChange
PokeathlonCirclePush_StartLayoutChange: ; 0x021F2B68
	push {r3, r4, r5, lr}
	add r4, r0, #0
	strh r1, [r4, #0x1c]
	strh r2, [r4, #0x1e]
	mov r2, #0
	ldr r0, _021F2BA4 ; =PokeathlonCirclePush_LayoutChangeTask
	strb r2, [r4]
	add r1, r4, #0
	add r5, r3, #0
	bl SysTask_CreateOnMainQueue
	str r0, [r4, #4]
	ldr r0, _021F2BA8 ; =PokeathlonCirclePush_LayoutFadeTask
	add r1, r4, #0
	mov r2, #1
	bl SysTask_CreateOnMainQueue
	str r0, [r4, #0xc]
	mov r1, #2
	ldr r0, _021F2BAC ; =0x04000018
	str r5, [r4, #0x30]
	lsl r1, r1, #0x16
	str r1, [r0]
	ldr r0, _021F2BB0 ; =PokeathlonCirclePush_RingSlideTask
	add r1, r4, #0
	mov r2, #2
	bl SysTask_CreateOnMainQueue
	str r0, [r4, #8]
	pop {r3, r4, r5, pc}
	.balign 4, 0
_021F2BA4: .word PokeathlonCirclePush_LayoutChangeTask
_021F2BA8: .word PokeathlonCirclePush_LayoutFadeTask
_021F2BAC: .word 0x04000018
_021F2BB0: .word PokeathlonCirclePush_RingSlideTask
	thumb_func_end PokeathlonCirclePush_StartLayoutChange




	thumb_func_start PokeathlonCirclePush_LayoutChangeTask
PokeathlonCirclePush_LayoutChangeTask: ; 0x021F2BB4
	push {r3, r4, r5, lr}
	add r4, r1, #0
	add r5, r0, #0
	ldrb r0, [r4]
	cmp r0, #0
	beq _021F2BC6
	cmp r0, #1
	beq _021F2BDE
	pop {r3, r4, r5, pc}
_021F2BC6:
	ldr r0, [r4, #8]
	cmp r0, #0
	bne _021F2BFE
	ldr r0, _021F2C00 ; =PokeathlonCirclePush_ShakeTask
	mov r2, #3
	bl SysTask_CreateOnMainQueue
	str r0, [r4, #0x10]
	ldrb r0, [r4]
	add r0, r0, #1
	strb r0, [r4]
	pop {r3, r4, r5, pc}
_021F2BDE:
	ldr r0, [r4, #0x10]
	cmp r0, #0
	bne _021F2BFE
	mov r0, #0
	str r0, [r4, #4]
	strb r0, [r4]
	ldr r0, [r4, #0x30]
	bl PokeathlonCourse_GetDataCopyArea
	bl ov96_021E8A20
	mov r1, #1
	strb r1, [r0, #8]
	add r0, r5, #0
	bl SysTask_Destroy
_021F2BFE:
	pop {r3, r4, r5, pc}
	.balign 4, 0
_021F2C00: .word PokeathlonCirclePush_ShakeTask
	thumb_func_end PokeathlonCirclePush_LayoutChangeTask




	thumb_func_start PokeathlonCirclePush_LayoutFadeTask
PokeathlonCirclePush_LayoutFadeTask: ; 0x021F2C04
	push {r3, r4, lr}
	sub sp, #4
	add r4, r1, #0
	ldrb r1, [r4, #1]
	cmp r1, #0
	beq _021F2C1C
	cmp r1, #1
	beq _021F2C5A
	cmp r1, #2
	beq _021F2C86
	add sp, #4
	pop {r3, r4, pc}
_021F2C1C:
	ldrh r0, [r4, #0x18]
	add r0, r0, #1
	strh r0, [r4, #0x18]
	ldrh r0, [r4, #0x18]
	cmp r0, #5
	bhi _021F2C50
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	mov r0, #0xc
	mul r0, r1
	mov r1, #5
	bl _s32_div_f
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r0, r4, #4
	str r0, [sp]
	mov r3, #0xc
	ldr r0, _021F2CC8 ; =0x04000050
	mov r1, #0
	mov r2, #1
	sub r3, r3, r4
	bl G2x_SetBlendAlpha_
	add sp, #4
	pop {r3, r4, pc}
_021F2C50:
	ldrb r0, [r4, #1]
	add sp, #4
	add r0, r0, #1
	strb r0, [r4, #1]
	pop {r3, r4, pc}
_021F2C5A:
	ldrh r1, [r4, #0x1e]
	ldr r0, [r4, #0x20]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl PokeathlonCirclePushZoneMarkers_Show
	ldrh r1, [r4, #0x1c]
	ldr r0, [r4, #0x20]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl PokeathlonCirclePushZoneMarkers_Hide
	mov r0, #0
	strh r0, [r4, #0x18]
	ldrb r0, [r4, #1]
	add r0, r0, #1
	strb r0, [r4, #1]
	ldr r0, _021F2CCC ; =0x0000089F
	bl PlaySE
	add sp, #4
	pop {r3, r4, pc}
_021F2C86:
	ldrh r1, [r4, #0x18]
	add r1, r1, #1
	strh r1, [r4, #0x18]
	ldrh r1, [r4, #0x18]
	cmp r1, #5
	bhi _021F2CB8
	lsl r0, r1, #0x18
	lsr r1, r0, #0x18
	mov r0, #0xc
	mul r0, r1
	mov r1, #5
	bl _s32_div_f
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	mov r0, #0x10
	sub r0, r0, r3
	str r0, [sp]
	ldr r0, _021F2CC8 ; =0x04000050
	mov r1, #0
	mov r2, #1
	bl G2x_SetBlendAlpha_
	add sp, #4
	pop {r3, r4, pc}
_021F2CB8:
	mov r1, #0
	str r1, [r4, #0xc]
	strh r1, [r4, #0x18]
	strb r1, [r4, #1]
	bl SysTask_Destroy
	add sp, #4
	pop {r3, r4, pc}
	.balign 4, 0
_021F2CC8: .word 0x04000050
_021F2CCC: .word 0x0000089F
	thumb_func_end PokeathlonCirclePush_LayoutFadeTask




	thumb_func_start PokeathlonCirclePush_RingSlideTask
PokeathlonCirclePush_RingSlideTask: ; 0x021F2CD0
	push {r3, r4, r5, lr}
	add r4, r1, #0
	add r5, r0, #0
	ldrb r0, [r4, #2]
	cmp r0, #0
	beq _021F2CE6
	cmp r0, #1
	beq _021F2D16
	cmp r0, #2
	beq _021F2D2E
	pop {r3, r4, r5, pc}
_021F2CE6:
	ldrh r0, [r4, #0x1a]
	mov r1, #5
	add r0, r0, #1
	strh r0, [r4, #0x1a]
	ldrh r0, [r4, #0x1a]
	lsl r0, r0, #7
	bl _s32_div_f
	mov r1, #0x80
	sub r0, r1, r0
	lsl r1, r0, #0x10
	ldr r0, _021F2D60 ; =0x01FF0000
	and r1, r0
	ldr r0, _021F2D64 ; =0x04000018
	str r1, [r0]
	ldrh r0, [r4, #0x1a]
	cmp r0, #5
	blo _021F2D5E
	mov r0, #0
	strh r0, [r4, #0x1a]
	ldrb r0, [r4, #2]
	add r0, r0, #1
	strb r0, [r4, #2]
	pop {r3, r4, r5, pc}
_021F2D16:
	ldrh r0, [r4, #0x1a]
	add r0, r0, #1
	strh r0, [r4, #0x1a]
	ldrh r0, [r4, #0x1a]
	cmp r0, #0x14
	blo _021F2D5E
	mov r0, #0
	strh r0, [r4, #0x1a]
	ldrb r0, [r4, #2]
	add r0, r0, #1
	strb r0, [r4, #2]
	pop {r3, r4, r5, pc}
_021F2D2E:
	ldrh r0, [r4, #0x1a]
	mov r1, #5
	add r0, r0, #1
	strh r0, [r4, #0x1a]
	ldrh r0, [r4, #0x1a]
	lsl r0, r0, #7
	neg r0, r0
	bl _s32_div_f
	lsl r1, r0, #0x10
	ldr r0, _021F2D60 ; =0x01FF0000
	and r1, r0
	ldr r0, _021F2D64 ; =0x04000018
	str r1, [r0]
	ldrh r0, [r4, #0x1a]
	cmp r0, #5
	blo _021F2D5E
	mov r0, #0
	strh r0, [r4, #0x1a]
	str r0, [r4, #8]
	strb r0, [r4, #2]
	add r0, r5, #0
	bl SysTask_Destroy
_021F2D5E:
	pop {r3, r4, r5, pc}
	.balign 4, 0
_021F2D60: .word 0x01FF0000
_021F2D64: .word 0x04000018
	thumb_func_end PokeathlonCirclePush_RingSlideTask
