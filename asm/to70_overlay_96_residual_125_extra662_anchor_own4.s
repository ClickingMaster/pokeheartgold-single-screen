	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"
	.extern ov96_0221A7F4
	.extern ov96_0221A81C
	.extern ov96_0221A830
	.extern ov96_0221A858
	.extern ov96_0221A880
	.extern ov96_0221A8A8
	.extern ov96_0221A8D0
	.extern ov96_0221A8F8
	.extern ov96_0221A90C
	.extern ov96_0221A920
	.extern ov96_0221BE04
	.extern ov96_0221BE08
	.extern ov96_0221BE0C
	.extern ov96_0221BE10
	.extern ov96_0221BE14
	.extern ov96_0221BE34
	.extern ov96_0221BE54
	.extern ov96_0221BE74
	.extern ov96_0221BE94
	.extern ov96_0221BEB4
	.extern ov96_0221BED4
	.extern ov96_0221BEF4
	.extern ov96_0221BF14
	.extern ov96_0221BF34
	.extern ov96_0221BF54
	.extern ov96_0221BF74
	.extern ov96_0221BF94
	.extern ov96_0221BFB4
	.extern ov96_0221BFD4
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
	.extern PokeathlonCourse_GetParticipantUnk04
	.extern PokeathlonCourse_GetPlayerProfile
	.extern PokeathlonCourse_GetPlayerProfileFromData
	.extern PokeathlonCourse_GetSaveData
	.extern PokeathlonCourse_GetSystem
	.extern PokeathlonCourse_GetUnkConstant4
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
	.extern ov96_021E5E04
	.extern ov96_021E5E7C
	.extern ov96_021E5F24
	.extern ov96_021E6F18
	.extern ov96_021E75B8
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
	.extern ov96_021E9A54
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
	.extern ov96_021FCD74
	.extern ov96_02204DE8
	.extern PokeathlonBlockSmash_Draw
	.extern PokeathlonGoalRoll_Draw
	.extern PokeathlonEvent_PennantCapturePoints
	.extern PokeathlonEvent_CirclePushPoints
	.extern PokeathlonEvent_BlockSmashPoints
	.extern PokeathlonEvent_DiscCatchPoints
	.extern PokeathlonEvent_Event8Points
	.extern ov96_0221DC80
	.extern ov96_0221DC88
	.extern ov96_0221DC90
	.extern ov96_0221E5A0
	.extern ov96_0221E5A4
	.public _0221DA00
	.public ov96_0221DA28
	.public ov96_0221DA50
	.public ov96_0221DA5C
	.public ov96_0221DA68
	.public ov96_0221DA6C
	.public ov96_0221DA80
	.public ov96_0221DB50
	.public ov96_0221DB5C
	.public ov96_0221DB70
	.public ov96_0221DBB8
	.public ov96_0221DBC8
	.public ov96_0221DC04
	.data
_0221DA00:
	.word ov96_0221DC30
	.word ov96_0221DC50
	.word ov96_0221DB50
	.word ov96_0221DC5C
	.word ov96_0221DC74
	.word ov96_0221DC1C
	.word ov96_0221DC3C
	.word ov96_0221DC90
	.word ov96_0221DC48
	.word ov96_0221DC88

ov96_0221DA28: ; 0x0221DA28
	.word ov96_0221A90C
	.word ov96_0221A8D0
	.word ov96_0221A7F4
	.word ov96_0221A830
	.word ov96_0221A8A8
	.word ov96_0221A920
	.word ov96_0221A8F8
	.word ov96_0221A880
	.word ov96_0221A858
	.word ov96_0221A81C

ov96_0221DA50: ; 0x0221DA50
	.word PokeathlonCourseOpening_FadeIn
	.word PokeathlonCourseOpening_Introduce
	.word PokeathlonCourseOpening_FadeOut

ov96_0221DA5C: ; 0x0221DA5C
	.word PokeathlonCourseClosing_FadeIn
	.word PokeathlonCourseClosing_Awards
	.word PokeathlonCourseClosing_FadeOut

ov96_0221DA68: ; 0x0221DA68
	.word PokeathlonCourseWait_FadeOut

ov96_0221DA6C: ; 0x0221DA6C
	.word PokeathlonEventIntro_Appear
	.word PokeathlonEventIntro_DetailsPage
	.word PokeathlonEventIntro_RulesPage
	.word PokeathlonEventIntro_ControlsPage
	.word PokeathlonEventIntro_FadeOut

ov96_0221DA80: ; 0x0221DA80
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

ov96_0221DB50:
	.word PokeathlonCirclePush_FadeIn
	.word PokeathlonCirclePush_Play
	.word PokeathlonCirclePush_End

ov96_0221DB5C: ; 0x0221DB5C
	.byte 0xFC, 0xFF, 0xFF, 0xFF
	.byte 0xFE, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00

ov96_0221DB70: ; 0x0221DB70
	.byte 0xFD, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xFD, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF
	.byte 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0xFD, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00

ov96_0221DBB8: ; 0x0221DBB8
	.word ov96_0221BE10
	.word ov96_0221BE0C
	.word ov96_0221BE04
	.word ov96_0221BE08

ov96_0221DBC8: ; 0x0221DBC8
	.word ov96_0221BF74
	.word ov96_0221BF14
	.word ov96_0221BEF4
	.word ov96_0221BED4
	.word ov96_0221BEB4
	.word ov96_0221BE74
	.word ov96_0221BE94
	.word ov96_0221BF54
	.word ov96_0221BF94
	.word ov96_0221BE14
	.word ov96_0221BE34
	.word ov96_0221BF34
	.word ov96_0221BFB4
	.word ov96_0221BE54
	.word ov96_0221BFD4

ov96_0221DC04: ; 0x0221DC04
	.byte 0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00

