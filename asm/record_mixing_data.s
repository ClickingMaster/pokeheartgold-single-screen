	.public sRecordMixingButtonX
	.public sRecordMixingStateFuncs
	.public _021EA940
	.public sRecordMixingAvatarVramOffsets
	.public sRecordMixingFlashDurations
	.public sRecordMixingFlashPalettes
	.public sRecordMixingButtonHitboxes
	.public sRecordMixingGraphicsModes
	.public sRecordMixingObjCharTransferTemplate
	.public sRecordMixingSpritePositions
	.public sRecordMixingBgTemplate_Main0
	.public sRecordMixingBgTemplate_Main1
	.public sRecordMixingBgTemplate_Sub0
	.public sRecordMixingBgTemplate_Sub1
	.public sRecordMixingGraphicsBanks
	.public ov73_021EA610
	.public ov73_021EA618
	.public ov73_021EA624
	.public ov73_021EA630
	.public ov73_021EA640
	.public ov73_021EA650
	.public sRecordMixingAllowedEntryKinds
	.public sNintendoWfcUnusedArrowPositions
	.public sNintendoWfcUnusedGraphicsModes
	.public sNintendoWfcUnusedObjCharTransferTemplate
	.public sNintendoWfcUnusedGraphicsBanks
	.public sNintendoWfcUnusedBgTemplate_Sub1
	.public sNintendoWfcUnusedBgTemplate_Main1
	.public sNintendoWfcUnusedBgTemplate_Main0
	.public sNintendoWfcUnusedBgTemplate_Sub0
	.public sRecordMixingHandlers
	.public sNintendoWfcUnusedModeInit
	.public sNintendoWfcUnusedModeMain
	.public sNintendoWfcUnusedModeExit
	.public sNintendoWfcUnusedStates
	.extern RecordMixing_State_PrintGroupPrompt
	.extern RecordMixing_State_HandleGroupMenu
	.extern RecordMixing_State_WaitTextThenResetMenu
	.extern RecordMixing_State_WaitText
	.extern RecordMixing_State_ResetLeaderMenuState
	.extern RecordMixing_State_ShowCancelYesNo
	.extern RecordMixing_State_HandleCancelYesNo
	.extern RecordMixing_State_WaitLeaveReply
	.extern RecordMixing_State_SendLeave
	.extern RecordMixing_State_AbortLeave
	.extern RecordMixing_State_ShowConfirmYesNo
	.extern RecordMixing_State_HandleConfirmYesNo
	.extern RecordMixing_State_SendStartMixing
	.extern RecordMixing_State_WaitRecords
	.extern RecordMixing_State_MergeRecords
	.extern RecordMixing_State_StartSave
	.extern RecordMixing_State_Save
	.extern RecordMixing_State_WaitAfterSave
	.extern RecordMixing_State_PrintCanceled
	.extern RecordMixing_State_FadeOutAfterDelay
	.extern RecordMixing_State_ShowDisbandYesNo
	.extern RecordMixing_State_HandleDisbandYesNo
	.extern RecordMixing_State_PrintDisband
	.extern RecordMixing_State_WaitDisband
	.extern RecordMixing_State_StartExitSync
	.extern RecordMixing_State_WaitExitSync
	.extern RecordMixing_State_PrintMemberUnavailable
	.extern RecordMixing_State_WaitMemberUnavailable
	.extern RecordMixing_State_PrintMemberLeft
	.extern RecordMixing_State_StartMemberLeftWait
	.extern RecordMixing_State_WaitMemberLeft
	.extern RecordMixing_Block23List2_GetSize
	.extern RecordMixing_Block23List0_GetSize
	.extern RecordMixing_Block23List1_GetSize
	.extern RecordMixing_Block23List2_Load
	.extern RecordMixing_Block23List0_Load
	.extern RecordMixing_Block23List1_Load
	.extern RecordMixing_Block23List2_Load2
	.extern RecordMixing_Block23List0_Load2
	.extern RecordMixing_Block23List1_Load2
	.extern NintendoWfcUnused_SessionInit
	.extern NintendoWfcUnused_SessionMain
	.extern NintendoWfcUnused_SessionExit
	.extern NintendoWfcUnused_AskConnect
	.extern NintendoWfcUnused_HandleConnectChoice
	.extern NintendoWfcUnused_AskLogOut
	.extern NintendoWfcUnused_HandleLogOutChoice
	.extern NintendoWfcUnused_StartConnect
	.extern NintendoWfcUnused_WaitConnect
	.extern NintendoWfcUnused_StartAuth
	.extern NintendoWfcUnused_WaitAuth
	.extern NintendoWfcUnused_InitHttp
	.extern NintendoWfcUnused_RequestInfo
	.extern NintendoWfcUnused_WaitInfo
	.extern NintendoWfcUnused_RequestSetProfile
	.extern NintendoWfcUnused_WaitSetProfile
	.extern NintendoWfcUnused_RequestRoomCount
	.extern NintendoWfcUnused_WaitRoomCount
	.extern NintendoWfcUnused_AskChallengeRoom
	.extern NintendoWfcUnused_StartChallengeRoomInput
	.extern NintendoWfcUnused_HandleChallengeRoomInput
	.extern NintendoWfcUnused_PrintReceivingTrainers
	.extern NintendoWfcUnused_PrintRoomUnavailable
	.extern NintendoWfcUnused_RequestTrainersDownload
	.extern NintendoWfcUnused_WaitTrainersDownload
	.extern NintendoWfcUnused_PrintTrainersReceived
	.extern NintendoWfcUnused_PrintSendingRecord
	.extern NintendoWfcUnused_RequestUpload
	.extern NintendoWfcUnused_WaitUpload
	.extern NintendoWfcUnused_PrintRecordSent
	.extern NintendoWfcUnused_AskLeadersRank
	.extern NintendoWfcUnused_StartLeadersRankInput
	.extern NintendoWfcUnused_HandleLeadersRankInput
	.extern NintendoWfcUnused_AskLeadersRoom
	.extern NintendoWfcUnused_StartLeadersRoomInput
	.extern NintendoWfcUnused_HandleLeadersRoomInput
	.extern NintendoWfcUnused_PrintReceivingLeaders
	.extern NintendoWfcUnused_RequestLeadersDownload
	.extern NintendoWfcUnused_WaitLeadersDownload
	.extern NintendoWfcUnused_PrintLeadersReceived
	.extern NintendoWfcUnused_StartDisconnect
	.extern NintendoWfcUnused_StartSave
	.extern NintendoWfcUnused_Save
	.extern NintendoWfcUnused_AskCancelChallenge
	.extern NintendoWfcUnused_HandleCancelChallengeChoice
	.extern NintendoWfcUnused_AskCancelDownload
	.extern NintendoWfcUnused_HandleCancelDownloadChoice
	.extern NintendoWfcUnused_PrintDisconnecting
	.extern NintendoWfcUnused_CloseConnection
	.extern NintendoWfcUnused_PrintDisconnected
	.extern NintendoWfcUnused_UnusedState_Idle
	.extern NintendoWfcUnused_StartExit
	.extern NintendoWfcUnused_UnusedState_HandleYesNo
	.extern NintendoWfcUnused_ShowHttpError
	.extern NintendoWfcUnused_Disconnect
	.extern NintendoWfcUnused_WaitMessage
	.extern NintendoWfcUnused_WaitMessageAndDelay
	.extern NintendoWfcUnused_OpenYesNoPrompt
	.extern NintendoWfcUnused_ShowDwcError
	.extern NintendoWfcUnused_WaitErrorDismiss
	.extern RecordMixing_FriendGroup_GetSize
	.extern RecordMixing_FriendGroup_Load
	.extern RecordMixing_FriendGroup_Merge
	.extern RecordMixing_Fashion_GetSize
	.extern RecordMixing_Fashion_Load
	.extern RecordMixing_Fashion_Merge
	.extern RecordMixing_Block23_Merge
	.extern RecordMixing_Rankings_Load
	.extern RecordMixing_Rankings_Merge
	.extern RecordMixing_BattleRoomTrainer_Load
	.extern RecordMixing_BattleRoomTrainer_Merge
#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_73.inc"
	.include "global.inc"

.text
	.rodata

sRecordMixingButtonX:
	.byte 0x10, 0x18, 0x00, 0x00

sRecordMixingAvatarVramOffsets: ; 0x021EA520
	.byte 0xA0, 0x01, 0xA0, 0x07, 0xA0, 0x0D, 0xA0, 0x13, 0xA0, 0x19

sRecordMixingFlashDurations: ; 0x021EA52A
	.byte 0x08

sRecordMixingFlashPalettes: ; 0x021EA52B
	.byte 0x02, 0x08, 0x03, 0x08, 0x04
	.byte 0x18, 0x00, 0x00, 0xFF

sRecordMixingButtonHitboxes: ; 0x021EA534
	.byte 0xA0, 0xC0, 0x80, 0xC0, 0xA0, 0xC0, 0xC0, 0x00, 0xFF, 0x00, 0x00, 0x00

sRecordMixingGraphicsModes: ; 0x021EA540
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sRecordMixingObjCharTransferTemplate: ; 0x021EA550
	.byte 0x14, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00

sRecordMixingSpritePositions: ; 0x021EA560
	.byte 0x54, 0x00, 0x68, 0x00, 0x55, 0x00, 0x48, 0x00, 0x72, 0x00, 0x58, 0x00, 0x67, 0x00, 0x75, 0x00
	.byte 0x42, 0x00, 0x75, 0x00, 0x37, 0x00, 0x58, 0x00

sRecordMixingBgTemplate_Main0: ; 0x021EA578
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

sRecordMixingBgTemplate_Main1: ; 0x021EA594
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sRecordMixingBgTemplate_Sub0: ; 0x021EA5B0
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x1F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sRecordMixingBgTemplate_Sub1: ; 0x021EA5CC
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x00
	.byte 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sRecordMixingGraphicsBanks: ; 0x021EA5E8
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

ov73_021EA610:
	.byte 0x01, 0x03, 0x04, 0x06, 0x08, 0x09, 0xFF, 0x00

ov73_021EA618:
	.byte 0x01, 0x03, 0x04, 0x05, 0x08, 0x09, 0x0A, 0x0B, 0xFF, 0x00, 0x00, 0x00

ov73_021EA624:
	.byte 0x02, 0x04, 0x06, 0x07, 0x09, 0x0A, 0x0B, 0x0D, 0xFF, 0x00, 0x00, 0x00

ov73_021EA630:
	.byte 0x02, 0x04, 0x06, 0x07, 0x09, 0x0A, 0x0B, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0xFF, 0x00

ov73_021EA640:
	.byte 0x01, 0x02, 0x03, 0x04, 0x06, 0x0A, 0x0D, 0x0F, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0xFF

ov73_021EA650:
	.byte 0x01, 0x02, 0x03, 0x04, 0x06, 0x07, 0x08, 0x0A, 0x0D, 0x0F, 0x10, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x19, 0x1A, 0xFF

sRecordMixingAllowedEntryKinds: ; 0x021EA664
	.word ov73_021EA630
	.word ov73_021EA650
	.word ov73_021EA618
	.word ov73_021EA624
	.word ov73_021EA640
	.word ov73_021EA610
	.byte 0x00, 0x17, 0x0D, 0x07
	.byte 0x04, 0x0D, 0x00, 0x00

sNintendoWfcUnusedArrowPositions: ; 0x021EA684
	.byte 0xE0, 0x00, 0x6F, 0x00, 0xE0, 0x00, 0x81, 0x00

sNintendoWfcUnusedGraphicsModes: ; 0x021EA68C
	.byte 0x01, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sNintendoWfcUnusedObjCharTransferTemplate: ; 0x021EA69C
	.byte 0x14, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x96, 0x00, 0x00, 0x00

sNintendoWfcUnusedGraphicsBanks: ; 0x021EA6AC
	.byte 0x01, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
	.byte 0x60, 0x00, 0x00, 0x00

sNintendoWfcUnusedBgTemplate_Sub1: ; 0x021EA6D4
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1B, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sNintendoWfcUnusedBgTemplate_Main1: ; 0x021EA6F0
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x1E, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sNintendoWfcUnusedBgTemplate_Main0: ; 0x021EA70C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1F, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sNintendoWfcUnusedBgTemplate_Sub0: ; 0x021EA728
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x04, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

sRecordMixingHandlers: ; 0x021EA744
	.word RecordMixing_FriendGroup_GetSize, RecordMixing_FriendGroup_Load, RecordMixing_FriendGroup_Merge
	.word RecordMixing_Fashion_GetSize, RecordMixing_Fashion_Load, RecordMixing_Fashion_Merge
	.word RecordMixing_Block23List0_GetSize, RecordMixing_Block23List0_Load, 0
	.word RecordMixing_Block23List1_GetSize, RecordMixing_Block23List1_Load, 0
	.word RecordMixing_Block23List2_GetSize, RecordMixing_Block23List2_Load, 0
	.word Save_Rankings_GetMixingSize, RecordMixing_Rankings_Load, RecordMixing_Rankings_Merge
	.word BattleRoomTrainer_sizeof, RecordMixing_BattleRoomTrainer_Load, RecordMixing_BattleRoomTrainer_Merge
	.word RecordMixing_Block23List0_GetSize, RecordMixing_Block23List0_Load2, 0
	.word RecordMixing_Block23List1_GetSize, RecordMixing_Block23List1_Load2, 0
	.word RecordMixing_Block23List2_GetSize, RecordMixing_Block23List2_Load2, RecordMixing_Block23_Merge

	.data

sRecordMixingStateFuncs:
	.word RecordMixing_State_PrintGroupPrompt
	.word RecordMixing_State_WaitTextThenResetMenu
	.word RecordMixing_State_ResetLeaderMenuState
	.word RecordMixing_State_HandleGroupMenu
	.word RecordMixing_State_ShowCancelYesNo
	.word RecordMixing_State_HandleCancelYesNo
	.word RecordMixing_State_WaitLeaveReply
	.word RecordMixing_State_SendLeave
	.word RecordMixing_State_AbortLeave
	.word RecordMixing_State_PrintCanceled
	.word RecordMixing_State_FadeOutAfterDelay
	.word RecordMixing_State_ShowDisbandYesNo
	.word RecordMixing_State_HandleDisbandYesNo
	.word RecordMixing_State_PrintDisband
	.word RecordMixing_State_WaitDisband
	.word RecordMixing_State_StartExitSync
	.word RecordMixing_State_WaitExitSync
	.word RecordMixing_State_PrintMemberUnavailable
	.word RecordMixing_State_WaitMemberUnavailable
	.word RecordMixing_State_PrintMemberLeft
	.word RecordMixing_State_StartMemberLeftWait
	.word RecordMixing_State_WaitMemberLeft
	.word RecordMixing_State_ShowConfirmYesNo
	.word RecordMixing_State_HandleConfirmYesNo
	.word RecordMixing_State_SendStartMixing
	.word RecordMixing_State_WaitRecords
	.word RecordMixing_State_MergeRecords
	.word RecordMixing_State_StartSave
	.word RecordMixing_State_Save
	.word RecordMixing_State_WaitAfterSave
	.word RecordMixing_State_WaitText

sNintendoWfcUnusedModeInit: ; 0x021EA83C
	.word NintendoWfcUnused_SessionInit

sNintendoWfcUnusedModeMain: ; 0x021EA840
	.word NintendoWfcUnused_SessionMain

sNintendoWfcUnusedModeExit: ; 0x021EA844
	.word NintendoWfcUnused_SessionExit

sNintendoWfcUnusedStates: ; 0x021EA848
	.word NintendoWfcUnused_AskConnect
	.word NintendoWfcUnused_HandleConnectChoice
	.word NintendoWfcUnused_StartConnect
	.word NintendoWfcUnused_WaitConnect
	.word NintendoWfcUnused_StartAuth
	.word NintendoWfcUnused_WaitAuth
	.word NintendoWfcUnused_InitHttp
	.word NintendoWfcUnused_RequestInfo
	.word NintendoWfcUnused_WaitInfo
	.word NintendoWfcUnused_RequestSetProfile
	.word NintendoWfcUnused_WaitSetProfile
	.word NintendoWfcUnused_RequestRoomCount
	.word NintendoWfcUnused_WaitRoomCount
	.word NintendoWfcUnused_AskChallengeRoom
	.word NintendoWfcUnused_StartChallengeRoomInput
	.word NintendoWfcUnused_HandleChallengeRoomInput
	.word NintendoWfcUnused_PrintReceivingTrainers
	.word NintendoWfcUnused_RequestTrainersDownload
	.word NintendoWfcUnused_WaitTrainersDownload
	.word NintendoWfcUnused_PrintTrainersReceived
	.word NintendoWfcUnused_PrintRoomUnavailable
	.word NintendoWfcUnused_AskCancelChallenge
	.word NintendoWfcUnused_HandleCancelChallengeChoice
	.word NintendoWfcUnused_AskCancelDownload
	.word NintendoWfcUnused_HandleCancelDownloadChoice
	.word NintendoWfcUnused_PrintSendingRecord
	.word NintendoWfcUnused_RequestUpload
	.word NintendoWfcUnused_WaitUpload
	.word NintendoWfcUnused_PrintRecordSent
	.word NintendoWfcUnused_AskLeadersRank
	.word NintendoWfcUnused_StartLeadersRankInput
	.word NintendoWfcUnused_HandleLeadersRankInput
	.word NintendoWfcUnused_AskLeadersRoom
	.word NintendoWfcUnused_StartLeadersRoomInput
	.word NintendoWfcUnused_HandleLeadersRoomInput
	.word NintendoWfcUnused_PrintReceivingLeaders
	.word NintendoWfcUnused_RequestLeadersDownload
	.word NintendoWfcUnused_WaitLeadersDownload
	.word NintendoWfcUnused_PrintLeadersReceived
	.word NintendoWfcUnused_StartSave
	.word NintendoWfcUnused_Save
	.word NintendoWfcUnused_PrintDisconnecting
	.word NintendoWfcUnused_CloseConnection
	.word NintendoWfcUnused_PrintDisconnected
	.word NintendoWfcUnused_UnusedState_Idle
	.word NintendoWfcUnused_StartDisconnect
	.word NintendoWfcUnused_StartExit
	.word NintendoWfcUnused_WaitMessage
	.word NintendoWfcUnused_OpenYesNoPrompt
	.word NintendoWfcUnused_WaitMessageAndDelay
	.word NintendoWfcUnused_UnusedState_HandleYesNo
	.word NintendoWfcUnused_AskLogOut
	.word NintendoWfcUnused_HandleLogOutChoice
	.word NintendoWfcUnused_ShowHttpError
	.word NintendoWfcUnused_Disconnect
	.word NintendoWfcUnused_ShowDwcError
	.word NintendoWfcUnused_WaitErrorDismiss

	.bss

_021EA940:
	.space 0x8
