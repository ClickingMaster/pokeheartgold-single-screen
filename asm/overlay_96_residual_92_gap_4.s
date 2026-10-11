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
	.public ov96_021FA340
	.public ov96_021FBE3C
	.public ov96_021FBE44
	.public ov96_021FBE4C
	.public ov96_021FBE9C
	.public ov96_021FBEA0
	.public ov96_021FC0D0
	.public ov96_021FC0E4
	.public ov96_021FC0E8
	.public ov96_021FC0F4
	.public ov96_021FC314
	.public ov96_021FC690
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
	.public ov96_0220D408
	.public ov96_0220D420
	.public ov96_0220E850
	.public ov96_0220E888
	.public ov96_0220E960
	.public ov96_0220F3B4
	.public ov96_0220FF64
	.public ov96_0221022C
	.public ov96_0221031C
	.public ov96_022104C4
	.public ov96_02210850
	.public PokeathlonGoalRoll_Draw
	.public PokeathlonGoalRoll_SetTouchTarget
	.public PokeathlonGoalRollDisplay_GetScore
	.public PokeathlonGoalRollCpu_SetBalls
	.public PokeathlonGoalRollCpu_Free
	.public ov96_02215864
	.public ov96_0221587C
	.public ov96_022158D4
	.public ov96_02215944
	.public ov96_02215DBC
	.public ov96_02215FA0
	.public ov96_02217890
	.public ov96_02217DBC
	.public ov96_022186B8
	.public ov96_02218A50
	.public ov96_022193E4
	.public ov96_0221978C
	.public ov96_02219FDC
	.public ov96_0221A670
	.public ov96_0221A680
	.public ov96_0221A690
	.public ov96_0221A69C
	.public ov96_0221A720


	thumb_func_start PokeathlonBlockSmash_Finish
PokeathlonBlockSmash_Finish: ; 0x022093D0
	push {r4, r5, r6, lr}
	add r6, r0, #0
	bl PokeathlonCourse_GetHeapAllocPtr4
	add r5, r0, #0
	bl IsPaletteFadeFinished
	cmp r0, #0
	beq _02209442
	add r0, r6, #0
	bl ov96_021E5F24
	add r4, r0, #0
	ldr r0, [r5, #0x40]
	bl PokeathlonBlockSmashBlocks_GetScore
	lsl r1, r4, #0x18
	add r2, r0, #0
	add r0, r6, #0
	lsr r1, r1, #0x18
	bl PokeathlonCourse_SetField5E0_AtIndex
	cmp r4, #0
	bne _0220943E
	add r0, r6, #0
	bl PokeathlonCourse_GetParticipantCount
	add r4, r0, #0
	cmp r4, #4
	bge _0220943E
	mov r0, #0x28
	add r5, r4, #0
	mul r5, r0
_02209412:
	add r0, r6, #0
	bl PokeathlonCourse_GetDataCopyArea
	add r0, #0x50
	add r0, r0, r5
	bl ov96_021E8A20
	add r2, r0, #0
	ldr r2, [r2]
	lsl r1, r4, #0x18
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	lsl r2, r2, #0x10
	add r0, r6, #0
	lsr r1, r1, #0x18
	lsr r2, r2, #0x10
	bl PokeathlonCourse_SetField5E0_AtIndex
	add r4, r4, #1
	add r5, #0x28
	cmp r4, #4
	blt _02209412
_0220943E:
	mov r0, #1
	pop {r4, r5, r6, pc}
_02209442:
	mov r0, #0
	pop {r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end PokeathlonBlockSmash_Finish


	thumb_func_start PokeathlonBlockSmash_Init
PokeathlonBlockSmash_Init: ; 0x02209448
	push {r4, r5, r6, r7, lr}
	sub sp, #0xec
	str r0, [sp, #0xc]
	bl PokeathlonCourse_GetHeapAllocPtr4
	str r0, [sp, #0x14]
	ldr r0, [sp, #0xc]
	bl PokeathlonCourse_GetField1ED
	cmp r0, #7
	bls _02209460
	b _0220971E
_02209460:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0220946C: ; jump table
	.short _0220947C - _0220946C - 2 ; case 0
	.short _022094EE - _0220946C - 2 ; case 1
	.short _02209546 - _0220946C - 2 ; case 2
	.short _022095D2 - _0220946C - 2 ; case 3
	.short _02209618 - _0220946C - 2 ; case 4
	.short _0220963C - _0220946C - 2 ; case 5
	.short _022096F2 - _0220946C - 2 ; case 6
	.short _02209710 - _0220946C - 2 ; case 7
_0220947C:
	mov r2, #6
	mov r0, #0x5c
	mov r1, #0x8d
	lsl r2, r2, #0x10
	bl Heap_Create
	mov r0, #0
	add r1, r0, #0
	bl Main_SetVBlankIntrCB
	mov r0, #0
	add r1, r0, #0
	bl Main_SetHBlankIntrCB
	bl GfGfx_DisableEngineAPlanes
	bl GfGfx_DisableEngineBPlanes
	mov r2, #1
	lsl r2, r2, #0x1a
	ldr r1, [r2]
	ldr r0, _02209728 ; =0xFFFFE0FF
	and r1, r0
	str r1, [r2]
	ldr r2, _0220972C ; =0x04001000
	ldr r1, [r2]
	and r0, r1
	str r0, [r2]
	bl PokeathlonBlockSmash_SetVramBanks
	ldr r0, _02209730 ; =gSystem + 0x60
	mov r1, #1
	strb r1, [r0, #9]
	bl GfGfx_SwapDisplay
	mov r1, #0x27
	ldr r0, [sp, #0xc]
	lsl r1, r1, #4
	bl PokeathlonCourse_AllocPtr4FromHeap
	mov r2, #0x27
	mov r1, #0
	lsl r2, r2, #4
	add r4, r0, #0
	bl MI_CpuFill8
	mov r0, #0x8d
	str r0, [r4]
	mov r1, #0xe1
	mov r0, #0x26
	lsl r1, r1, #2
	lsl r0, r0, #4
	str r1, [r4, r0]
	ldr r0, [sp, #0xc]
	bl PokeathlonCourse_IncrementField1ED
	b _02209722
_022094EE:
	ldr r0, [sp, #0x14]
	ldr r0, [r0]
	bl BgConfig_Alloc
	ldr r1, [sp, #0x14]
	str r0, [r1, #4]
	ldr r0, [sp, #0xc]
	mov r1, #4
	bl PokeathlonCourse_SetHudPlanes
	ldr r0, [sp, #0x14]
	ldr r1, [sp, #0x14]
	ldr r0, [r0, #4]
	ldr r1, [r1]
	bl PokeathlonBlockSmash_InitBgs
	ldr r0, [sp, #0xc]
	bl PokeathlonBlockSmash_InitText
	ldr r0, [sp, #0x14]
	bl PokeathlonBlockSmash_LoadBgGraphics
	ldr r0, [sp, #0x14]
	bl PokeathlonBlockSmash_InitSpriteSystem
	ldr r0, [sp, #0x14]
	ldr r0, [r0, #0xc]
	bl SpriteManager_GetSpriteList
	str r0, [sp]
	ldr r0, [sp, #0x14]
	ldr r3, [sp, #0x14]
	ldr r0, [r0]
	ldr r3, [r3, #0x14]
	mov r1, #9
	mov r2, #0x20
	bl PokeathlonCourseMonObjects_Create
	ldr r1, [sp, #0x14]
	str r0, [r1, #0x10]
	ldr r0, [sp, #0xc]
	bl PokeathlonCourse_IncrementField1ED
	b _02209722
_02209546:
	mov r5, #0
	add r6, sp, #0x18
	add r2, r6, #0
	add r0, r5, #0
	add r1, r5, #0
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	str r0, [r2]
	str r5, [sp, #0x10]
	add r7, sp, #0x5c
_02209566:
	ldr r0, [sp, #0xc]
	bl ov96_021E5F24
	ldr r1, [sp, #0x10]
	cmp r1, r0
	beq _022095A4
	mov r4, #0
_02209574:
	cmp r5, #9
	blt _0220957C
	bl GF_AssertFail
_0220957C:
	ldr r0, [sp, #0xc]
	ldr r1, [sp, #0x10]
	add r2, r4, #0
	add r3, r7, #0
	bl PokeathlonCourse_GetMonInfo
	ldr r0, [sp, #0xc]
	ldr r1, [sp, #0x10]
	add r2, r4, #0
	bl PokeathlonCourse_GetPerformance
	bl ov96_021E6108
	str r0, [r6, #0x14]
	add r4, r4, #1
	add r7, #0x10
	add r6, r6, #4
	add r5, r5, #1
	cmp r4, #3
	blt _02209574
_022095A4:
	ldr r0, [sp, #0x10]
	add r0, r0, #1
	str r0, [sp, #0x10]
	cmp r0, #4
	blt _02209566
	mov r0, #1
	str r0, [sp, #0x1c]
	str r0, [sp, #0x24]
	str r0, [sp, #0x28]
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, [sp, #0x14]
	mov r1, #9
	ldr r0, [r0, #0x10]
	add r2, sp, #0x5c
	add r3, sp, #0x18
	bl PokeathlonCourseMonObjects_Load
	ldr r0, [sp, #0xc]
	bl PokeathlonCourse_IncrementField1ED
	b _02209722
_022095D2:
	ldr r0, [sp, #0x14]
	ldr r0, [r0, #0x10]
	bl PokeathlonCourseMonObjects_IsLoaded
	cmp r0, #0
	bne _022095E0
	b _02209722
_022095E0:
	ldr r0, [sp, #0x14]
	bl PokeathlonBlockSmash_LoadSpriteResources
	ldr r0, [sp, #0x14]
	ldr r1, [sp, #0x14]
	ldr r0, [r0, #8]
	ldr r1, [r1, #0xc]
	bl ov96_02209B04
	ldr r0, [sp, #0x14]
	ldr r0, [r0, #0xc]
	bl SpriteManager_GetSpriteList
	ldr r2, [sp, #0x14]
	add r3, r0, #0
	ldr r0, [sp, #0xc]
	ldr r2, [r2, #0x14]
	mov r1, #0
	bl PokeathlonCourse_CreateHudSprites
	ldr r0, [r0]
	mov r1, #0
	bl Sprite_SetDrawPriority
	ldr r0, [sp, #0xc]
	bl PokeathlonCourse_IncrementField1ED
	b _02209722
_02209618:
	ldr r0, [sp, #0x14]
	bl PokeathlonBlockSmash_CreateHudSprites
	ldr r0, [sp, #0x14]
	ldr r1, [sp, #0x14]
	ldr r2, [sp, #0x14]
	ldr r0, [r0]
	ldr r1, [r1, #8]
	ldr r2, [r2, #0xc]
	ldr r3, [sp, #0xc]
	bl PokeathlonBlockSmashBlocks_New
	ldr r1, [sp, #0x14]
	str r0, [r1, #0x40]
	ldr r0, [sp, #0xc]
	bl PokeathlonCourse_IncrementField1ED
	b _02209722
_0220963C:
	ldr r0, [sp, #0x14]
	ldr r1, [sp, #0x14]
	ldr r2, [sp, #0x14]
	ldr r0, [r0]
	ldr r1, [r1, #8]
	ldr r2, [r2, #0xc]
	ldr r3, [sp, #0xc]
	bl PokeathlonBlockSmashMon_New
	ldr r1, [sp, #0x14]
	ldr r2, [sp, #0x14]
	str r0, [r1, #0x4c]
	add r0, r1, #0
	ldr r0, [r0, #0x10]
	ldr r3, [sp, #0xc]
	str r0, [sp]
	add r0, r1, #0
	ldr r0, [r0]
	ldr r1, [r1, #8]
	ldr r2, [r2, #0xc]
	bl PokeathlonBlockSmashRivals_New
	ldr r1, [sp, #0x14]
	str r0, [r1, #0x44]
	add r0, r1, #0
	ldr r0, [r0, #0x10]
	mov r1, #1
	bl PokeathlonCourseObjects_PauseAnims
	ldr r0, [sp, #0xc]
	bl ov96_021E5F24
	cmp r0, #0
	bne _0220968E
	ldr r1, [sp, #0x14]
	ldr r0, [sp, #0xc]
	ldr r1, [r1]
	bl PokeathlonBlockSmashCpu_New
	ldr r1, [sp, #0x14]
	str r0, [r1, #0x48]
_0220968E:
	ldr r0, [sp, #0x14]
	bl PokeathlonBlockSmash_PrintInitialText
	ldr r0, [sp, #0xc]
	bl PokeathlonBlockSmash_CreateTeamRows
	ldr r0, [sp, #0x14]
	ldr r0, [r0, #4]
	bl PokeathlonCourse_SetVBlankIntrCB
	mov r0, #1
	bl WirelessStatusIcon_CreateOnScreenIfConnected
	ldr r0, [sp, #0xc]
	mov r1, #1
	bl PokeathlonCourse_SetField1F4
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineBTogglePlanes
	ldr r0, [sp, #0xc]
	bl ov96_021E5F24
	cmp r0, #0
	bne _022096D2
	ldr r0, [sp, #0x14]
	ldr r0, [r0, #0x48]
	bl PokeathlonBlockSmashCpu_WriteReports
_022096D2:
	mov r0, #6
	str r0, [sp]
	mov r1, #1
	ldr r0, [sp, #0x14]
	str r1, [sp, #4]
	ldr r0, [r0]
	ldr r3, _02209734 ; =0x00007FFF
	str r0, [sp, #8]
	mov r0, #0
	add r2, r1, #0
	bl BeginNormalPaletteFade
	ldr r0, [sp, #0xc]
	bl PokeathlonCourse_IncrementField1ED
	b _02209722
_022096F2:
	ldr r0, [sp, #0xc]
	bl ov96_021E5F24
	cmp r0, #0
	bne _02209702
	ldr r0, [sp, #0xc]
	bl PokeathlonBlockSmash_WriteReport
_02209702:
	ldr r0, [sp, #0xc]
	bl PokeathlonBlockSmash_UpdateLinkData
	ldr r0, [sp, #0xc]
	bl PokeathlonCourse_IncrementField1ED
	b _02209722
_02209710:
	bl IsPaletteFadeFinished
	cmp r0, #0
	beq _02209722
	add sp, #0xec
	mov r0, #1
	pop {r4, r5, r6, r7, pc}
_0220971E:
	bl GF_AssertFail
_02209722:
	mov r0, #0
	add sp, #0xec
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_02209728: .word 0xFFFFE0FF
_0220972C: .word 0x04001000
_02209730: .word gSystem + 0x60
_02209734: .word 0x00007FFF
	thumb_func_end PokeathlonBlockSmash_Init
