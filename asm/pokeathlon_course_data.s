	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"
	.extern gPokeathlonCirclePushEvent
	.extern gPokeathlonGoalRollEvent
	.extern gPokeathlonBlockSmashEvent
	.extern gPokeathlonSnowThrowEvent
	.extern gPokeathlonRingDropEvent
	.extern gPokeathlonDiscCatchEvent
	.extern gPokeathlonPennantCaptureEvent
	.extern gPokeathlonRelayRunEvent
	.extern gPokeathlonHurdleDashEvent
	.extern gPokeathlonLampJumpEvent
	.extern gPokeathlonCirclePushZoneKind3
	.extern gPokeathlonCirclePushZoneKind4
	.extern gPokeathlonCirclePushZoneKind2
	.extern gPokeathlonCirclePushZoneKind1
	.extern gPokeathlonCirclePushZones_Layout9
	.extern gPokeathlonCirclePushZones_Layout10
	.extern gPokeathlonCirclePushZones_Layout13
	.extern gPokeathlonCirclePushZones_Layout5
	.extern gPokeathlonCirclePushZones_Layout6
	.extern gPokeathlonCirclePushZones_Layout4
	.extern gPokeathlonCirclePushZones_Layout3
	.extern gPokeathlonCirclePushZones_Layout2
	.extern gPokeathlonCirclePushZones_Layout1
	.extern gPokeathlonCirclePushZones_Layout11
	.extern gPokeathlonCirclePushZones_Layout7
	.extern gPokeathlonCirclePushZones_Layout0
	.extern gPokeathlonCirclePushZones_Layout8
	.extern gPokeathlonCirclePushZones_Layout12
	.extern gPokeathlonCirclePushZones_Layout14
	.extern PokeathlonCourse_AllocPtr4FromHeap
	.extern PokeathlonCourse_FreePtr4HeapAlloc
	.extern PokeathlonCourse_GetCurrentParticipantIndex
	.extern PokeathlonCourse_GetDataCopyArea
	.extern PokeathlonCourse_GetField1ED
	.extern PokeathlonCourse_GetField1EF
	.extern PokeathlonCourse_GetField3A4
	.extern PokeathlonCourse_GetField3D8_AtIndex
	.extern PokeathlonCourse_GetField3D8_ForCurrentParticipant
	.extern PokeathlonCourse_GetField5F0_AtIndex
	.extern PokeathlonCourse_GetField974_AtIndex
	.extern PokeathlonCourse_GetFieldBA4
	.extern PokeathlonCourse_GetFieldData
	.extern PokeathlonCourse_GetFieldData_AtIndex
	.extern PokeathlonCourse_GetGraphicsSystem
	.extern PokeathlonCourse_GetHeapAllocPtr4
	.extern PokeathlonCourse_GetHeapID
	.extern PokeathlonCourse_GetMode
	.extern PokeathlonCourse_GetParticipantCount
	.extern PokeathlonCourse_GetParticipantData
	.extern PokeathlonCourse_GetParticipantMons
	.extern PokeathlonCourse_GetPlayerProfile
	.extern PokeathlonCourse_GetPlayerProfileFromData
	.extern PokeathlonCourse_GetSaveData
	.extern PokeathlonCourse_GetSystem
	.extern PokeathlonCourse_GetEventResultSize
	.extern PokeathlonCourse_IncrementField1ED
	.extern PokeathlonCourse_IncrementField1EF
	.extern PokeathlonCourse_InitPlayerProfiles
	.extern PokeathlonCourse_InitStateInfo
	.extern PokeathlonCourse_ResetDataCopyArea
	.extern PokeathlonCourse_ResetField1EF
	.extern PokeathlonCourse_ResetField3A4
	.extern PokeathlonCourse_RunSubStateLoop
	.extern PokeathlonCourse_SetField1ED
	.extern PokeathlonCourse_SetField1F4
	.extern PokeathlonCourse_SetField3A4
	.extern PokeathlonCourse_SetField5E0_AtIndex
	.extern PokeathlonCourse_SetStateField07
	.extern PokeathlonCourse_SetStateField07_IfDifferent
	.extern PokeathlonCourse_SetStateTransitionType
	.extern PokeathlonCourse_SetVBlankIntrCB
	.extern PokeathlonCourse_SetEvents
	.extern PokeathlonCourse_FindCourseId
	.extern PokeathlonCourse_GetNetId
	.extern PokeathlonCourse_State_ResetDataCopyArea
	.extern PokeathlonCourse_State_WaitForCommand
	.extern PokeathlonCourseCmd_EventsSize
	.extern PokeathlonCourseCmd_ParticipantSize
	.extern PokeathlonCourseCmd_AllParticipantsSize
	.extern PokeathlonCourseCmd_ProfilesSize
	.extern PokeathlonCourseCmd_RecordSize
	.extern PokeathlonCourseCmd_PacketSize
	.extern PokeathlonCourseCmd_MonCountersSize
	.extern PokeathlonCourseCmd_ResultsSize
	.extern PokeathlonCourseCmd_JuiceRecordSize
	.extern PokeathlonCourseCmd_GetEventDataBuffer
	.extern PokeathlonCourseCmd_GetRecordBuffer
	.extern PokeathlonCourseCmd_GetResultsBuffer
	.extern PokeathlonCourseCmd_GetMonCountersBuffer
	.extern PokeathlonCourseCmd_GetJuiceRecordBuffer
	.extern PokeathlonCourseOpening_Init
	.extern PokeathlonCourseOpening_Main
	.extern PokeathlonCourseOpening_Exit
	.extern PokeathlonCourseClosing_Init
	.extern PokeathlonCourseClosing_Main
	.extern PokeathlonCourseClosing_Exit
	.extern PokeathlonCourseWait_Main
	.extern PokeathlonCourseWait_Exit
	.extern PokeathlonEventIntro_Main
	.extern PokeathlonCirclePush_Main
	.extern PokeathlonLampJump_Draw
	.extern PokeathlonHurdleDash_Start
	.extern PokeathlonRelayRun_Draw
	.extern PokeathlonPennantCapture_Draw
	.extern PokeathlonBlockSmash_Draw
	.extern PokeathlonGoalRoll_Draw
	.extern PokeathlonEvent_PennantCapturePoints
	.extern PokeathlonEvent_CirclePushPoints
	.extern PokeathlonEvent_BlockSmashPoints
	.extern PokeathlonEvent_DiscCatchPoints
	.extern PokeathlonEvent_SnowThrowPoints
	.extern gPokeathlonGoalRollBounceOffsets
	.extern gPokeathlonGoalRollStates
	.extern gPokeathlonRingDropStates
	.extern gPokeathlonLampJumpTrampolineRed
	.extern gPokeathlonBlockSmashFramesSinceHit
	.public gPokeathlonCourseEventSteps
	.public gPokeathlonCourseEvents
	.public gPokeathlonCourseOpeningSteps
	.public gPokeathlonCourseClosingSteps
	.public gPokeathlonCourseWaitSteps
	.public gPokeathlonEventIntroSteps
	.public gPokeathlonEventIntroSpriteTemplates
	.public gPokeathlonCirclePushStates
	.public gPokeathlonCirclePushBounceOffsets
	.public gPokeathlonCirclePushShakeOffsets
	.public gPokeathlonCirclePushZoneKinds
	.public gPokeathlonCirclePushLayoutZones
	.public gPokeathlonCirclePushDisplayIdleAnims
	.data
gPokeathlonCourseEventSteps:
	.word gPokeathlonHurdleDashStates
	.word gPokeathlonPennantCaptureStates
	.word gPokeathlonCirclePushStates
	.word gPokeathlonBlockSmashStates
	.word gPokeathlonDiscCatchStates
	.word gPokeathlonLampJumpStates
	.word gPokeathlonRelayRunStates
	.word gPokeathlonRingDropStates
	.word gPokeathlonSnowThrowStates
	.word gPokeathlonGoalRollStates

gPokeathlonCourseEvents: ; 0x0221DA28
	.word gPokeathlonHurdleDashEvent
	.word gPokeathlonPennantCaptureEvent
	.word gPokeathlonCirclePushEvent
	.word gPokeathlonBlockSmashEvent
	.word gPokeathlonDiscCatchEvent
	.word gPokeathlonLampJumpEvent
	.word gPokeathlonRelayRunEvent
	.word gPokeathlonRingDropEvent
	.word gPokeathlonSnowThrowEvent
	.word gPokeathlonGoalRollEvent

gPokeathlonCourseOpeningSteps: ; 0x0221DA50
	.word PokeathlonCourseOpening_FadeIn
	.word PokeathlonCourseOpening_Introduce
	.word PokeathlonCourseOpening_FadeOut

gPokeathlonCourseClosingSteps: ; 0x0221DA5C
	.word PokeathlonCourseClosing_FadeIn
	.word PokeathlonCourseClosing_Awards
	.word PokeathlonCourseClosing_FadeOut

gPokeathlonCourseWaitSteps: ; 0x0221DA68
	.word PokeathlonCourseWait_FadeOut

gPokeathlonEventIntroSteps: ; 0x0221DA6C
	.word PokeathlonEventIntro_Appear
	.word PokeathlonEventIntro_DetailsPage
	.word PokeathlonEventIntro_RulesPage
	.word PokeathlonEventIntro_ControlsPage
	.word PokeathlonEventIntro_FadeOut

gPokeathlonEventIntroSpriteTemplates: ; 0x0221DA80
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0xFF, 0xFF, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

gPokeathlonCirclePushStates:
	.word PokeathlonCirclePush_FadeIn
	.word PokeathlonCirclePush_Play
	.word PokeathlonCirclePush_End

gPokeathlonCirclePushBounceOffsets: ; 0x0221DB5C
	.byte 0xFC, 0xFF, 0xFF, 0xFF
	.byte 0xFE, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00

gPokeathlonCirclePushShakeOffsets: ; 0x0221DB70
	.byte 0xFD, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xFD, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF
	.byte 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0xFD, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00

gPokeathlonCirclePushZoneKinds: ; 0x0221DBB8
	.word gPokeathlonCirclePushZoneKind1
	.word gPokeathlonCirclePushZoneKind2
	.word gPokeathlonCirclePushZoneKind3
	.word gPokeathlonCirclePushZoneKind4

gPokeathlonCirclePushLayoutZones: ; 0x0221DBC8
	.word gPokeathlonCirclePushZones_Layout0
	.word gPokeathlonCirclePushZones_Layout1
	.word gPokeathlonCirclePushZones_Layout2
	.word gPokeathlonCirclePushZones_Layout3
	.word gPokeathlonCirclePushZones_Layout4
	.word gPokeathlonCirclePushZones_Layout5
	.word gPokeathlonCirclePushZones_Layout6
	.word gPokeathlonCirclePushZones_Layout7
	.word gPokeathlonCirclePushZones_Layout8
	.word gPokeathlonCirclePushZones_Layout9
	.word gPokeathlonCirclePushZones_Layout10
	.word gPokeathlonCirclePushZones_Layout11
	.word gPokeathlonCirclePushZones_Layout12
	.word gPokeathlonCirclePushZones_Layout13
	.word gPokeathlonCirclePushZones_Layout14

gPokeathlonCirclePushDisplayIdleAnims: ; 0x0221DC04
	.byte 0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00

