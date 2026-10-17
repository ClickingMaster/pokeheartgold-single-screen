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
	.public gPokeathlonLampJumpSoundPlayers
	.public gPokeathlonHurdleDashLanePans
	.public gPokeathlonHurdleDashLaneSoundPlayers
	.public gPokeathlonBlockSmashSpotHitboxes
	.extern PokeathlonCourse_AllocWork
	.extern PokeathlonCourse_FreeWork
	.extern PokeathlonCourse_GetRound
	.extern PokeathlonCourse_GetDataCopyArea
	.extern PokeathlonCourse_GetEventInitState
	.extern PokeathlonCourse_GetReceivedCount
	.extern PokeathlonCourse_GetEventRecvBuffer
	.extern PokeathlonCourse_GetRoundEvent
	.extern PokeathlonCourse_GetCurrentEvent
	.extern PokeathlonCourse_GetReceivedScore
	.extern PokeathlonCourse_GetRecord
	.extern PokeathlonCourse_GetMonCounters
	.extern PokeathlonCourse_GetFieldData
	.extern PokeathlonCourse_GetFieldData_AtIndex
	.extern PokeathlonCourse_GetGraphicsSystem
	.extern PokeathlonCourse_GetWork
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
	.extern PokeathlonCourse_AdvanceEventInitState
	.extern PokeathlonCourse_IncrementReceivedCount
	.extern PokeathlonCourse_InitPlayerProfiles
	.extern PokeathlonCourse_InitStateInfo
	.extern PokeathlonCourse_ResetDataCopyArea
	.extern PokeathlonCourse_ResetReceivedCount
	.extern PokeathlonCourse_ClearEventDataTransfer
	.extern PokeathlonCourse_RunSubStateLoop
	.extern PokeathlonCourse_SetEventInitState
	.extern PokeathlonCourse_SetEventDrawEnabled
	.extern PokeathlonCourse_SetEventDataTransfer
	.extern PokeathlonCourse_SetEventScore
	.extern PokeathlonCourse_SetNextState
	.extern PokeathlonCourse_SetNextStateIfNot
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
	.public gPokeathlonEventRankPoints
	.public gPokeathlonEventPointsCalcFuncs
	.extern PokeathlonCourse_RunSubStateLoop
	.extern PokeathlonCourse_InitStateInfo
	.extern PokeathlonCourse_InitPlayerProfiles
	.extern PokeathlonCourse_GetPlayerProfile
	.extern PokeathlonCourse_GetParticipantCount
	.extern PokeathlonCourse_GetParticipantData
	.extern PokeathlonCourse_GetParticipantMons
	.extern PokeathlonCourse_GetSaveData
	.extern PokeathlonCourse_GetFieldData
	.extern PokeathlonCourse_GetFieldData_AtIndex
	.extern PokeathlonCourse_GetMonCounters
	.extern PokeathlonCourse_AllocWork
	.extern PokeathlonCourse_FreeWork
	.extern PokeathlonCourse_GetWork
	.extern PokeathlonCourse_GetHeapID
	.extern PokeathlonCourse_GetEventInitState
	.extern PokeathlonCourse_SetEventInitState
	.extern PokeathlonCourse_AdvanceEventInitState
	.extern PokeathlonCourse_SetEventDrawEnabled
	.extern PokeathlonCourse_SetEvents
	.extern PokeathlonCourse_GetCurrentEvent
	.extern PokeathlonCourse_GetRoundEvent
	.extern PokeathlonCourse_FindCourseId
	.extern PokeathlonCourse_GetRound
	.extern PokeathlonCourse_GetMode
	.extern PokeathlonCourse_GetReceivedCount
	.extern PokeathlonCourse_IncrementReceivedCount
	.extern PokeathlonCourse_ResetReceivedCount
	.extern PokeathlonCourse_GetSystem
	.extern PokeathlonCourse_GetNetId
	.extern PokeathlonCourse_GetPlayerProfileFromData
	.extern PokeathlonCourse_GetRecord
	.extern PokeathlonCourse_GetDataCopyArea
	.extern PokeathlonCourse_ResetDataCopyArea
	.extern PokeathlonCourse_SetEventDataTransfer
	.extern PokeathlonCourse_ClearEventDataTransfer
	.extern PokeathlonCourse_GetEventRecvBuffer
	.extern PokeathlonCourse_GetEventResultSize
	.extern PokeathlonCourse_SetEventScore
	.extern PokeathlonCourse_GetReceivedScore
	.extern PokeathlonCourse_SetNextState
	.extern PokeathlonCourse_SetNextStateIfNot
	.extern PokeathlonCourse_SetStateTransitionType
	.extern PokeathlonCourse_SetVBlankIntrCB
	.extern PokeathlonCourse_GetGraphicsSystem
	.public PokeathlonCourse_State_ResetDataCopyArea
	.public PokeathlonCourse_State_WaitForCommand
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
	.public PokeathlonCourseCmd_GetMonCountersBuffer
	.public PokeathlonCourseCmd_GetJuiceRecordBuffer
	.public PokeathlonCourseOpening_Init
	.public PokeathlonCourseOpening_Main
	.public PokeathlonCourseOpening_Exit
	.public PokeathlonCourseClosing_Init
	.public PokeathlonCourseClosing_Main
	.public PokeathlonCourseClosing_Exit
	.public PokeathlonCourseWait_Main
	.public PokeathlonCourseWait_Exit
	.public PokeathlonEventIntro_Main
	.public PokeathlonCirclePush_Main
	.public PokeathlonLampJump_Draw
	.public PokeathlonHurdleDash_Start
	.public PokeathlonRelayRun_Draw
	.public PokeathlonPennantCapture_Draw
	.public PokeathlonBlockSmash_Draw
	.public PokeathlonGoalRoll_Draw
	.public PokeathlonEvent_PennantCapturePoints
	.public PokeathlonEvent_CirclePushPoints
	.public PokeathlonEvent_BlockSmashPoints
	.public PokeathlonEvent_DiscCatchPoints
	.public PokeathlonEvent_SnowThrowPoints
	.extern gPokeathlonGoalRollBounceOffsets
	.extern gPokeathlonGoalRollStates
	.extern gPokeathlonRingDropStates
	.extern gPokeathlonLampJumpTrampolineRed
	.extern gPokeathlonBlockSmashFramesSinceHit
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

.text
	.rodata

gPokeathlonEventRankPoints:
	.byte 0x64, 0x50, 0x46, 0x3C

gPokeathlonEventPointsCalcFuncs:
	.word PokeathlonEvent_CalcPointsFromScores
	.word PokeathlonEvent_CalcPointsFromScores
	.word PokeathlonEvent_CalcPointsFromScores
	.word PokeathlonEvent_CalcPointsFromScores
	.word PokeathlonEvent_CalcPointsFromScores
	.word PokeathlonEvent_CalcPointsFromScores
	.word PokeathlonEvent_CalcPointsFromScores
	.word PokeathlonEvent_CalcPointsFromScores
	.word PokeathlonEvent_CalcPointsFromScores
	.word PokeathlonEvent_CalcPointsFromRanks

gPokeathlonEventScorePointsFuncs:
	.word PokeathlonEvent_HurdleDashPoints
	.word PokeathlonEvent_PennantCapturePoints
	.word PokeathlonEvent_CirclePushPoints
	.word PokeathlonEvent_BlockSmashPoints
	.word PokeathlonEvent_DiscCatchPoints
	.word PokeathlonEvent_LampJumpPoints
	.word PokeathlonEvent_RelayRunPoints
	.word PokeathlonEvent_RingDropPoints
	.word PokeathlonEvent_SnowThrowPoints
	.word 0

	.data

gPokeathlonLampJumpSoundPlayers:
	.byte 0x03, 0x04, 0x05, 0x06

gPokeathlonLampJumpStates:
	.word PokeathlonLampJump_Play
	.word PokeathlonLampJump_CalcScores

gPokeathlonResultsAnnouncementSteps:
	.word PokeathlonResults_Run

gPokeathlonHurdleDashLanePans:
	.byte 0x94, 0x00, 0x6B, 0x00

gPokeathlonHurdleDashLaneSoundPlayers:
	.byte 0x03, 0x04, 0x05, 0x00

gPokeathlonHurdleDashStates:
	.word PokeathlonHurdleDash_Start
	.word PokeathlonHurdleDash_Play
	.word PokeathlonHurdleDash_CalcScores

gPokeathlonRelayRunStates:
	.word PokeathlonRelayRun_Start
	.word PokeathlonRelayRun_Play
	.word PokeathlonRelayRun_SaveScores

gPokeathlonSnowThrowStates:
	.word PokeathlonSnowThrow_Play
	.word PokeathlonSnowThrow_SaveScores

gPokeathlonPennantCaptureStates:
	.word PokeathlonPennantCapture_State_Setup
	.word PokeathlonPennantCapture_State_Play
	.word PokeathlonPennantCapture_State_End

gPokeathlonBlockSmashStates:
	.word PokeathlonBlockSmash_Intro
	.word PokeathlonBlockSmash_Play
	.word PokeathlonBlockSmash_Finish

gPokeathlonBlockSmashSpotHitboxes:
	.word gPokeathlonBlockSmashSpotHitboxes_Kind0
	.word gPokeathlonBlockSmashSpotHitboxes_Kind1
	.word gPokeathlonBlockSmashSpotHitboxes_Kind2

gPokeathlonDiscCatchStates:
	.word PokeathlonDiscCatch_Intro
	.word PokeathlonDiscCatch_Play
	.word PokeathlonDiscCatch_Finish

	.bss

gPokeathlonCirclePushContacts:
	.space 0x900
