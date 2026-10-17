	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"

	.extern PokeathlonCirclePush_Init
	.extern PokeathlonCirclePush_Main
	.extern PokeathlonCirclePush_CalcResults
	.extern PokeathlonCirclePush_Exit
	.extern PokeathlonCourseOpening_Init
	.extern PokeathlonCourseOpening_Main
	.extern PokeathlonCourseOpening_Exit
	.extern PokeathlonGoalRoll_Init
	.extern PokeathlonGoalRoll_Draw
	.extern PokeathlonGoalRoll_ShowResults
	.extern PokeathlonGoalRoll_Exit
	.extern PokeathlonBlockSmash_Init
	.extern PokeathlonBlockSmash_Draw
	.extern PokeathlonBlockSmash_ShowResults
	.extern PokeathlonBlockSmash_Exit
	.extern PokeathlonCourseClosing_Init
	.extern PokeathlonCourseClosing_Main
	.extern PokeathlonCourseClosing_Exit
	.extern PokeathlonSnowThrow_Init
	.extern PokeathlonSnowThrow_Draw
	.extern PokeathlonSnowThrow_ShowResults
	.extern PokeathlonSnowThrow_Exit
	.extern PokeathlonResults_Init
	.extern PokeathlonResults_Draw
	.extern PokeathlonResults_Exit
	.extern PokeathlonRingDrop_Init
	.extern PokeathlonRingDrop_Draw
	.extern PokeathlonRingDrop_ShowResults
	.extern PokeathlonRingDrop_Exit
	.extern PokeathlonDiscCatch_Init
	.extern PokeathlonDiscCatch_Draw
	.extern PokeathlonDiscCatch_ShowResults
	.extern PokeathlonDiscCatch_Exit
	.extern PokeathlonCourseWait_Init
	.extern PokeathlonCourseWait_Main
	.extern PokeathlonCourseWait_Exit
	.extern PokeathlonPennantCapture_Init
	.extern PokeathlonPennantCapture_Draw
	.extern PokeathlonPennantCapture_ShowResults
	.extern PokeathlonPennantCapture_Exit
	.extern PokeathlonEventIntro_Init
	.extern PokeathlonEventIntro_Main
	.extern PokeathlonEventIntro_Exit
	.extern PokeathlonRelayRun_Init
	.extern PokeathlonRelayRun_Draw
	.extern PokeathlonRelayRun_ShowResults
	.extern PokeathlonRelayRun_Exit
	.extern PokeathlonHurdleDash_Init
	.extern PokeathlonHurdleDash_Draw
	.extern PokeathlonHurdleDash_ShowResults
	.extern PokeathlonHurdleDash_Exit
	.extern PokeathlonLampJump_Init
	.extern PokeathlonLampJump_Draw
	.extern PokeathlonLampJump_ShowResults
	.extern PokeathlonLampJump_Exit
	.extern PokeathlonCourse_State_SendEvents
	.extern PokeathlonCourse_State_RunEventOverlay
	.extern PokeathlonCourse_State_RunWait
	.extern PokeathlonCourse_State_SendParticipant
	.extern PokeathlonCourse_State_SendParticipants
	.extern PokeathlonCourse_State_SendProfiles
	.extern PokeathlonCourse_State_SendRecord
	.extern PokeathlonCourse_State_StartOpening
	.extern PokeathlonCourse_State_WaitOpeningStart
	.extern PokeathlonCourse_State_RunOpening
	.extern PokeathlonCourse_State_EndOpening
	.extern PokeathlonCourse_State_StartEventIntro
	.extern PokeathlonCourse_State_WaitEventIntroStart
	.extern PokeathlonCourse_State_RunEventIntro
	.extern PokeathlonCourse_State_EndEventIntro
	.extern PokeathlonCourse_State_StartRound
	.extern PokeathlonCourse_State_WaitRoundStart
	.extern PokeathlonCourse_State_SendEventData
	.extern PokeathlonCourse_State_RunEvent
	.extern PokeathlonCourse_State_ResetDataCopyArea
	.extern PokeathlonCourse_State_SendEventResult
	.extern PokeathlonCourse_State_SendEventResults
	.extern PokeathlonCourse_State_EndRound
	.extern PokeathlonCourse_State_ExitEvent
	.extern PokeathlonCourse_State_NextRound
	.extern PokeathlonCourse_State_StartResults
	.extern PokeathlonCourse_State_WaitResultsStart
	.extern PokeathlonCourse_State_RunResults
	.extern PokeathlonCourse_State_EndResults
	.extern PokeathlonCourse_State_SendMonCounters
	.extern PokeathlonCourse_State_SendResults
	.extern PokeathlonCourse_State_StartClosing
	.extern PokeathlonCourse_State_WaitClosingStart
	.extern PokeathlonCourse_State_RunClosing
	.extern PokeathlonCourse_State_CreateJuiceRecords
	.extern PokeathlonCourse_State_SendJuiceRecord
	.extern PokeathlonCourse_State_StoreJuiceRecords
	.extern PokeathlonCourse_State_Exit
	.extern PokeathlonCourse_State_WaitForCommand
	.extern PokeathlonCourseCmd_RecvEvents
	.extern PokeathlonCourseCmd_EventsSize
	.extern PokeathlonCourseCmd_RecvParticipant
	.extern PokeathlonCourseCmd_ParticipantSize
	.extern PokeathlonCourseCmd_RecvAllParticipants
	.extern PokeathlonCourseCmd_AllParticipantsSize
	.extern PokeathlonCourseCmd_RecvProfiles
	.extern PokeathlonCourseCmd_ProfilesSize
	.extern PokeathlonCourseCmd_RecvRecord
	.extern PokeathlonCourseCmd_RecordSize
	.extern PokeathlonCourseCmd_GetRecordBuffer
	.extern PokeathlonCourseCmd_RecvPacket
	.extern PokeathlonCourseCmd_PacketSize
	.extern PokeathlonCourseCmd_RecvEventData
	.extern PokeathlonCourseCmd_GetEventDataBuffer
	.extern PokeathlonCourseCmd_RecvEventResult
	.extern PokeathlonCourseCmd_RecvEventResults
	.extern PokeathlonCourseCmd_RecvMonCounters
	.extern PokeathlonCourseCmd_MonCountersSize
	.extern PokeathlonCourseCmd_GetMonCountersBuffer
	.extern PokeathlonCourseCmd_RecvResults
	.extern PokeathlonCourseCmd_ResultsSize
	.extern PokeathlonCourseCmd_GetResultsBuffer
	.extern PokeathlonCourseCmd_RecvJuiceRecord
	.extern PokeathlonCourseCmd_JuiceRecordSize
	.extern PokeathlonCourseCmd_GetJuiceRecordBuffer
	.extern gPokeathlonHurdleDashStates
	.extern gPokeathlonPennantCaptureStates
	.extern gPokeathlonCirclePushStates
	.extern gPokeathlonBlockSmashStates
	.extern gPokeathlonDiscCatchStates
	.extern gPokeathlonLampJumpStates
	.extern gPokeathlonRelayRunStates
	.extern gPokeathlonRingDropStates
	.extern gPokeathlonSnowThrowStates
	.extern gPokeathlonGoalRollStates

	.public gPokeathlonEventScoreOrders
	.public subOverlayTemplate
	.public gPokeathlonCirclePushEvent
	.public gPokeathlonCourseEvent_Opening
	.public gPokeathlonGoalRollEvent
	.public gPokeathlonBlockSmashEvent
	.public gPokeathlonCourseEvent_Closing
	.public gPokeathlonSnowThrowEvent
	.public gPokeathlonCourseEvent_ResultsAnnouncement
	.public gPokeathlonRingDropEvent
	.public gPokeathlonEventTargetScores
	.public gPokeathlonDiscCatchEvent
	.public gPokeathlonCourseEvent_Wait
	.public gPokeathlonPennantCaptureEvent
	.public gPokeathlonCourseEvent_EventIntro
	.public gPokeathlonRelayRunEvent
	.public gPokeathlonHurdleDashEvent
	.public gPokeathlonLampJumpEvent
	.public gPokeathlonCourseEventIds
	.public gPokeathlonCourse10EventSets
	.public sPokeathlonStateInfoFuncTable
	.public gPokeathlonNpcTeamSpriteIds
	.public gPokeathlonPerformanceFileIds
	.public gPokeathlonCourseCmds
	.public gPokeathlonCourseEventSteps
	.public gPokeathlonCourseEvents

	.rodata
gPokeathlonEventScoreOrders:
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.word 0

subOverlayTemplate:
	.word PokeathlonBox_Init
	.word PokeathlonBox_Main
	.word PokeathlonBox_Exit
	.word FS_OVERLAY_ID(OVY_97)

gPokeathlonCirclePushEvent:
	.word PokeathlonCirclePush_Init
	.word PokeathlonCirclePush_Main
	.word PokeathlonCirclePush_CalcResults
	.word PokeathlonCirclePush_Exit
	.word 0

gPokeathlonCourseEvent_Opening:
	.word PokeathlonCourseOpening_Init
	.word PokeathlonCourseOpening_Main
	.word 0
	.word PokeathlonCourseOpening_Exit
	.word 0

gPokeathlonGoalRollEvent:
	.word PokeathlonGoalRoll_Init
	.word PokeathlonGoalRoll_Draw
	.word PokeathlonGoalRoll_ShowResults
	.word PokeathlonGoalRoll_Exit
	.word 0

gPokeathlonBlockSmashEvent:
	.word PokeathlonBlockSmash_Init
	.word PokeathlonBlockSmash_Draw
	.word PokeathlonBlockSmash_ShowResults
	.word PokeathlonBlockSmash_Exit
	.word 1

gPokeathlonCourseEvent_Closing:
	.word PokeathlonCourseClosing_Init
	.word PokeathlonCourseClosing_Main
	.word 0
	.word PokeathlonCourseClosing_Exit
	.word 0

gPokeathlonSnowThrowEvent:
	.word PokeathlonSnowThrow_Init
	.word PokeathlonSnowThrow_Draw
	.word PokeathlonSnowThrow_ShowResults
	.word PokeathlonSnowThrow_Exit
	.word 0

gPokeathlonCourseEvent_ResultsAnnouncement:
	.word PokeathlonResults_Init
	.word PokeathlonResults_Draw
	.word 0
	.word PokeathlonResults_Exit
	.word 0

gPokeathlonRingDropEvent:
	.word PokeathlonRingDrop_Init
	.word PokeathlonRingDrop_Draw
	.word PokeathlonRingDrop_ShowResults
	.word PokeathlonRingDrop_Exit
	.word 0

gPokeathlonEventTargetScores:
	.short 3000, 35, 40, 80, 40, 350, 12288, 70
	.short 30, 3

gPokeathlonDiscCatchEvent:
	.word PokeathlonDiscCatch_Init
	.word PokeathlonDiscCatch_Draw
	.word PokeathlonDiscCatch_ShowResults
	.word PokeathlonDiscCatch_Exit
	.word 0

gPokeathlonCourseEvent_Wait:
	.word PokeathlonCourseWait_Init
	.word PokeathlonCourseWait_Main
	.word 0
	.word PokeathlonCourseWait_Exit
	.word 0

gPokeathlonPennantCaptureEvent:
	.word PokeathlonPennantCapture_Init
	.word PokeathlonPennantCapture_Draw
	.word PokeathlonPennantCapture_ShowResults
	.word PokeathlonPennantCapture_Exit
	.word 0

gPokeathlonCourseEvent_EventIntro:
	.word PokeathlonEventIntro_Init
	.word PokeathlonEventIntro_Main
	.word 0
	.word PokeathlonEventIntro_Exit
	.word 0

gPokeathlonRelayRunEvent:
	.word PokeathlonRelayRun_Init
	.word PokeathlonRelayRun_Draw
	.word PokeathlonRelayRun_ShowResults
	.word PokeathlonRelayRun_Exit
	.word 0

gPokeathlonHurdleDashEvent:
	.word PokeathlonHurdleDash_Init
	.word PokeathlonHurdleDash_Draw
	.word PokeathlonHurdleDash_ShowResults
	.word PokeathlonHurdleDash_Exit
	.word 1

gPokeathlonLampJumpEvent:
	.word PokeathlonLampJump_Init
	.word PokeathlonLampJump_Draw
	.word PokeathlonLampJump_ShowResults
	.word PokeathlonLampJump_Exit
	.word 1

gPokeathlonCourseEventIds:
	.byte 0x00, 0x01, 0x06, 0x0A, 0x03, 0x02, 0x09, 0x0A, 0x08, 0x09, 0x01, 0x0A
	.byte 0x07, 0x06, 0x03, 0x0A, 0x05, 0x04, 0x00, 0x0A, 0x00, 0x05, 0x04, 0x06, 0x05, 0x04, 0x01, 0x08
	.byte 0x09, 0x08, 0x01, 0x03, 0x03, 0x02, 0x09, 0x07, 0x06, 0x07, 0x00, 0x02

gPokeathlonCourse10EventSets:
	.byte 0x04, 0x02, 0x09, 0x07
	.byte 0x07, 0x02, 0x03, 0x08, 0x05, 0x00, 0x09, 0x07, 0x03, 0x06, 0x08, 0x05, 0x08, 0x05, 0x03, 0x00
	.byte 0x01, 0x02, 0x09, 0x04, 0x02, 0x06, 0x05, 0x03, 0x00, 0x07, 0x03, 0x09, 0x09, 0x08, 0x04, 0x01
	.byte 0x06, 0x03, 0x07, 0x00

sPokeathlonStateInfoFuncTable:
	.word PokeathlonCourse_State_SendEvents
	.word PokeathlonCourse_State_RunEventOverlay
	.word PokeathlonCourse_State_RunWait
	.word PokeathlonCourse_State_SendParticipant
	.word PokeathlonCourse_State_SendParticipants
	.word PokeathlonCourse_State_SendProfiles
	.word PokeathlonCourse_State_SendRecord
	.word PokeathlonCourse_State_StartOpening
	.word PokeathlonCourse_State_WaitOpeningStart
	.word PokeathlonCourse_State_RunOpening
	.word PokeathlonCourse_State_EndOpening
	.word PokeathlonCourse_State_StartEventIntro
	.word PokeathlonCourse_State_WaitEventIntroStart
	.word PokeathlonCourse_State_RunEventIntro
	.word PokeathlonCourse_State_EndEventIntro
	.word PokeathlonCourse_State_StartRound
	.word PokeathlonCourse_State_WaitRoundStart
	.word PokeathlonCourse_State_SendEventData
	.word PokeathlonCourse_State_RunEvent
	.word PokeathlonCourse_State_ResetDataCopyArea
	.word PokeathlonCourse_State_SendEventResult
	.word PokeathlonCourse_State_SendEventResults
	.word PokeathlonCourse_State_EndRound
	.word PokeathlonCourse_State_ExitEvent
	.word PokeathlonCourse_State_NextRound
	.word PokeathlonCourse_State_StartResults
	.word PokeathlonCourse_State_WaitResultsStart
	.word PokeathlonCourse_State_RunResults
	.word PokeathlonCourse_State_EndResults
	.word PokeathlonCourse_State_SendMonCounters
	.word PokeathlonCourse_State_SendResults
	.word PokeathlonCourse_State_StartClosing
	.word PokeathlonCourse_State_WaitClosingStart
	.word PokeathlonCourse_State_RunClosing
	.word PokeathlonCourse_State_CreateJuiceRecords
	.word PokeathlonCourse_State_SendJuiceRecord
	.word PokeathlonCourse_State_StoreJuiceRecords
	.word PokeathlonCourse_State_Exit
	.word PokeathlonCourse_State_WaitForCommand

gPokeathlonNpcTeamSpriteIds:
	.short 0x140, 0x13F, 0x142, 0x143, 0x13D, 0x145, 0x147, 0x158
	.short 0x155, 0x14C, 0x141, 0x146, 0x13E, 0x14A, 0x143, 0x15A
	.short 0x147, 0x14D, 0x158, 0x179, 0x149, 0x140, 0x15C, 0x15A
	.short 0x159, 0x140, 0x13F, 0x142, 0x15A, 0x159, 0x149, 0x140
	.short 0x15C, 0x14A, 0x143, 0x141, 0x146, 0x13E, 0x155, 0x14C
	.short 0x145, 0x147, 0x158, 0x158, 0x179, 0x15A, 0x147, 0x14D
	.short 0x143, 0x13D, 0x144, 0x162, 0x140, 0x158, 0x159, 0x180
	.short 0x81, 0x145, 0x14C, 0x14D, 0x155, 0x13D, 0x146, 0xE8
	.short 0x17A, 0x165, 0x141, 0x179, 0x158, 0x13E, 0x142, 0x13F
	.short 0x160, 0x15A, 0x155, 0x144, 0x162, 0x140, 0x15A, 0x155
	.short 0x142, 0x13F, 0x160, 0xE8, 0x17A, 0x155, 0x13D, 0x146
	.short 0x14C, 0x14D, 0x180, 0x81, 0x145, 0x158, 0x13E, 0x165
	.short 0x141, 0x179, 0x159, 0x158

gPokeathlonPerformanceFileIds:
	.short 0, 0, 1, 2, 3, 4, 5, 6
	.short 7, 8, 9, 0xA, 0xB, 0xC, 0xD, 0xE
	.short 0xF, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16
	.short 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E
	.short 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26
	.short 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E
	.short 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36
	.short 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E
	.short 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46
	.short 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E
	.short 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56
	.short 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E
	.short 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66
	.short 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E
	.short 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76
	.short 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E
	.short 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86
	.short 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E
	.short 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96
	.short 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E
	.short 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6
	.short 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAD, 0xAE, 0xAF
	.short 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7
	.short 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF
	.short 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7
	.short 0xC8, 0xC9, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA
	.short 0xEB, 0xEC, 0xED, 0xEE, 0xEF, 0xF0, 0xF1, 0xF2
	.short 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA
	.short 0xFB, 0xFC, 0xFD, 0xFE, 0xFF, 0x100, 0x101, 0x102
	.short 0x103, 0x104, 0x105, 0x106, 0x107, 0x108, 0x109, 0x10A
	.short 0x10B, 0x10C, 0x10D, 0x10E, 0x10F, 0x110, 0x111, 0x112
	.short 0x113, 0x114, 0x115, 0x116, 0x117, 0x118, 0x119, 0x11A
	.short 0x11B, 0x11C, 0x11D, 0x11E, 0x11F, 0x120, 0x121, 0x122
	.short 0x123, 0x124, 0x125, 0x126, 0x127, 0x128, 0x129, 0x12A
	.short 0x12B, 0x12C, 0x12D, 0x12E, 0x12F, 0x130, 0x131, 0x132
	.short 0x133, 0x134, 0x135, 0x136, 0x137, 0x138, 0x139, 0x13A
	.short 0x13B, 0x13C, 0x13D, 0x13E, 0x13F, 0x140, 0x141, 0x142
	.short 0x143, 0x144, 0x145, 0x146, 0x147, 0x148, 0x149, 0x14A
	.short 0x14B, 0x14C, 0x14D, 0x14E, 0x14F, 0x150, 0x151, 0x152
	.short 0x153, 0x154, 0x155, 0x156, 0x157, 0x158, 0x159, 0x15A
	.short 0x15B, 0x15C, 0x15D, 0x15E, 0x15F, 0x160, 0x161, 0x162
	.short 0x163, 0x164, 0x165, 0x166, 0x167, 0x168, 0x169, 0x16A
	.short 0x16B, 0x16C, 0x16D, 0x16E, 0x16F, 0x170, 0x171, 0x172
	.short 0x173, 0x174, 0x175, 0x176, 0x177, 0x178, 0x179, 0x17A
	.short 0x17B, 0x17C, 0x17D, 0x17E, 0x17F, 0x180, 0x181, 0x182
	.short 0x183, 0x184, 0x185, 0x186, 0x187, 0x188, 0x189, 0x18A
	.short 0x18B, 0x18C, 0x18D, 0x18E, 0x18F, 0x190, 0x191, 0x192
	.short 0x193, 0x194, 0x195, 0x196, 0x197, 0x198, 0x199, 0x19A
	.short 0x19B, 0x19C, 0x19D, 0x1A1, 0x1A2, 0x1A3, 0x1A4, 0x1A5
	.short 0x1A6, 0x1A7, 0x1A8, 0x1A9, 0x1AA, 0x1AB, 0x1AC, 0x1AD
	.short 0x1AE, 0x1AF, 0x1B0, 0x1B1, 0x1B2, 0x1B3, 0x1B4, 0x1B5
	.short 0x1B6, 0x1B7, 0x1B8, 0x1B9, 0x1BA, 0x1BD, 0x1C0, 0x1C1
	.short 0x1C2, 0x1C3, 0x1C4, 0x1C5, 0x1C6, 0x1C7, 0x1C8, 0x1CA
	.short 0x1CC, 0x1CD, 0x1CE, 0x1CF, 0x1D0, 0x1D1, 0x1D2, 0x1D3
	.short 0x1D4, 0x1D5, 0x1D6, 0x1D7, 0x1D8, 0x1D9, 0x1DA, 0x1DB
	.short 0x1DC, 0x1DD, 0x1DE, 0x1DF, 0x1E0, 0x1E1, 0x1E2, 0x1E3
	.short 0x1E4, 0x1E5, 0x1E6, 0x1E7, 0x1E8, 0x1E9, 0x1EA, 0x1EB
	.short 0x1EC, 0x1ED, 0x1EE, 0x1EF, 0x1F0, 0x1F1, 0x1F2, 0x1F3
	.short 0x1F4, 0x1F5, 0x1F6, 0x1F7, 0x1F8, 0x1F9, 0x1FA, 0x1FB
	.short 0x1FC, 0x1FD, 0x1FE, 0x1FF, 0x200, 0x201, 0x202, 0x203
	.short 0x209, 0x20A, 0x20B, 0x20C, 0x20D, 0x20E, 0x20F, 0x210
	.short 0x212, 0x213, 0x214, 0x215, 0x216, 0x218

gPokeathlonCourseCmds:
	.word PokeathlonCourseCmd_RecvEvents
	.word PokeathlonCourseCmd_EventsSize
	.word 0
	.word PokeathlonCourseCmd_RecvParticipant
	.word PokeathlonCourseCmd_ParticipantSize
	.word 0
	.word PokeathlonCourseCmd_RecvAllParticipants
	.word PokeathlonCourseCmd_AllParticipantsSize
	.word 0
	.word PokeathlonCourseCmd_RecvProfiles
	.word PokeathlonCourseCmd_ProfilesSize
	.word 0
	.word PokeathlonCourseCmd_RecvRecord
	.word PokeathlonCourseCmd_RecordSize
	.word PokeathlonCourseCmd_GetRecordBuffer
	.word PokeathlonCourseCmd_RecvPacket
	.word PokeathlonCourseCmd_PacketSize
	.word 0
	.word PokeathlonCourseCmd_RecvEventData
	.word CommCmd_GetSizeVariable
	.word PokeathlonCourseCmd_GetEventDataBuffer
	.word PokeathlonCourseCmd_RecvEventResult
	.word PokeathlonCourseCmd_PacketSize
	.word 0
	.word PokeathlonCourseCmd_RecvEventResults
	.word PokeathlonCourseCmd_PacketSize
	.word 0
	.word PokeathlonCourseCmd_RecvMonCounters
	.word PokeathlonCourseCmd_MonCountersSize
	.word PokeathlonCourseCmd_GetMonCountersBuffer
	.word PokeathlonCourseCmd_RecvResults
	.word PokeathlonCourseCmd_ResultsSize
	.word PokeathlonCourseCmd_GetResultsBuffer
	.word PokeathlonCourseCmd_RecvJuiceRecord
	.word PokeathlonCourseCmd_JuiceRecordSize
	.word PokeathlonCourseCmd_GetJuiceRecordBuffer

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

gPokeathlonCourseEvents:
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
