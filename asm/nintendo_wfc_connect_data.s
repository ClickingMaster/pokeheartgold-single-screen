	.include "asm/macros.inc"
	.include "overlay_72.inc"
	.include "global.inc"

	.extern NintendoWfc_SessionInit
	.extern NintendoWfc_SessionMain
	.extern NintendoWfc_SessionExit
	.extern NintendoWfc_AskConnect
	.extern NintendoWfc_HandleConnectChoice
	.extern NintendoWfc_AskLogOut
	.extern NintendoWfc_HandleLogOutChoice
	.extern NintendoWfc_StartConnect
	.extern NintendoWfc_WaitConnect
	.extern NintendoWfc_StartAuth
	.extern NintendoWfc_WaitAuth
	.extern NintendoWfc_InitHttp
	.extern NintendoWfc_RequestInfo
	.extern NintendoWfc_WaitInfo
	.extern NintendoWfc_RequestSetProfile
	.extern NintendoWfc_WaitSetProfile
	.extern NintendoWfc_RequestRoomCount
	.extern NintendoWfc_WaitRoomCount
	.extern NintendoWfc_AskChallengeRoom
	.extern NintendoWfc_StartChallengeRoomInput
	.extern NintendoWfc_HandleChallengeRoomInput
	.extern NintendoWfc_PrintReceivingTrainers
	.extern NintendoWfc_PrintRoomUnavailable
	.extern NintendoWfc_RequestTrainersDownload
	.extern NintendoWfc_WaitTrainersDownload
	.extern NintendoWfc_PrintTrainersReceived
	.extern NintendoWfc_PrintSendingRecord
	.extern NintendoWfc_RequestUpload
	.extern NintendoWfc_WaitUpload
	.extern NintendoWfc_PrintRecordSent
	.extern NintendoWfc_AskLeadersRank
	.extern NintendoWfc_StartLeadersRankInput
	.extern NintendoWfc_HandleLeadersRankInput
	.extern NintendoWfc_AskLeadersRoom
	.extern NintendoWfc_StartLeadersRoomInput
	.extern NintendoWfc_HandleLeadersRoomInput
	.extern NintendoWfc_PrintReceivingLeaders
	.extern NintendoWfc_RequestLeadersDownload
	.extern NintendoWfc_WaitLeadersDownload
	.extern NintendoWfc_PrintLeadersReceived
	.extern NintendoWfc_StartDisconnect
	.extern NintendoWfc_StartSave
	.extern NintendoWfc_Save
	.extern NintendoWfc_AskCancelChallenge
	.extern NintendoWfc_HandleCancelChallengeChoice
	.extern NintendoWfc_AskCancelDownload
	.extern NintendoWfc_HandleCancelDownloadChoice
	.extern NintendoWfc_PrintDisconnecting
	.extern NintendoWfc_CloseConnection
	.extern NintendoWfc_PrintDisconnected
	.extern NintendoWfc_UnusedState_Idle
	.extern NintendoWfc_StartExit
	.extern NintendoWfc_UnusedState_HandleYesNo
	.extern NintendoWfc_ShowHttpError
	.extern NintendoWfc_Disconnect
	.extern NintendoWfc_WaitMessage
	.extern NintendoWfc_WaitMessageAndDelay
	.extern NintendoWfc_OpenYesNoMenu
	.extern NintendoWfc_ShowDwcError
	.extern NintendoWfc_WaitErrorDismiss
	.extern NintendoWfc_NumberInput_FadeOut
	.extern NintendoWfc_NumberInput_Setup
	.extern NintendoWfc_NumberInput_WaitFadeIn
	.extern NintendoWfc_NumberInput_Main
	.extern NintendoWfc_NumberInput_UnusedState4
	.extern NintendoWfc_NumberInput_UnusedState5
	.extern NintendoWfc_NumberInput_UnusedState6
	.extern NintendoWfc_NumberInput_StartFadeOut
	.extern NintendoWfc_NumberInput_Teardown
	.extern NintendoWfc_NumberInput_WaitButtonFlash
	.extern NintendoWfc_NumberInput_WaitFadeOut
	.extern NintendoWfc_NumberInput_Finish
	.extern NintendoWfcYesNo_Start
	.extern NintendoWfcYesNo_HandleInput
	.extern NintendoWfcYesNo_UnusedState2
	.extern NintendoWfcYesNo_WaitClose
	.public gNintendoWfcYesNoCursorPositions
	.public sNintendoWfcRoomNumUrl
	.public sNintendoWfcHttp
	.public gNintendoWfcButtonSpritePositions
	.public sNintendoWfcObjCharTransferTemplate
	.public sNintendoWfcGraphicsModes
	.public gNintendoWfcDigitSpritePositions
	.public gNintendoWfcDigitSpritePositionsY
	.public sNintendoWfcGraphicsBanks
	.public gNintendoWfcPadCursorPositions
	.public gNintendoWfcPadCursorPositionsY
	.public sNintendoWfcBgTemplate_Sub1
	.public sNintendoWfcBgTemplate_Sub2
	.public sNintendoWfcBgTemplate_Main1
	.public sNintendoWfcBgTemplate_Main0
	.public sNintendoWfcBgTemplate_Sub0
	.public gNintendoWfcPadKeysUpFromLeftButton
	.public gNintendoWfcPadKeysDownFromLeftButton
	.public gNintendoWfcPadKeysUpFromRightButton
	.public gNintendoWfcPadKeysDownFromRightButton
	.public gNintendoWfcNumberInputWindowX
	.public gNintendoWfcPadKeyFlashes
	.public gNintendoWfcPadNeighbors
	.public gNintendoWfcYesNoHitboxes
	.public sNintendoWfcDownloadUrl
	.public sNintendoWfcUploadUrl
	.public sNintendoWfcInfoUrl
	.public sNintendoWfcSetProfileUrl
	.public sNintendoWfcModeInit
	.public sNintendoWfcModeMain
	.public sNintendoWfcModeExit
	.public sNintendoWfcStates
	.public sNintendoWfcNumberInputStates
	.public gNintendoWfcPadHitboxes_Max100
	.public gNintendoWfcPadHitboxes
	.public sNintendoWfcYesNoStates
	.public sNintendoWfcHttpFriendKey
	.public gNintendoWfcHttpRequestBuffer
	.public ov72_0223B900
	.public gNintendoWfcHttpUploadFriendKey
	.public gNintendoWfcHttpResponse
	.public gNintendoWfcHttpShortResponse
	.public sNintendoWfcGlobal

	.rodata

sNintendoWfcUnusedWindowTemplate:
	.byte 0x00, 0x17, 0x0D, 0x07, 0x04, 0x0D, 0x00, 0x00

gNintendoWfcButtonSpritePositions:
	.byte 0x28, 0x00, 0xB0, 0x00, 0x80, 0x00, 0xB0, 0x00
	.byte 0xD8, 0x00, 0xB0, 0x00

sNintendoWfcObjCharTransferTemplate:
	.byte 0x14, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x43, 0x00, 0x00, 0x00

sNintendoWfcGraphicsModes:
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

gNintendoWfcDigitSpritePositions:
	.byte 0x60, 0x00

gNintendoWfcDigitSpritePositionsY:
	.byte 0x1D, 0x00, 0x80, 0x00, 0x1D, 0x00, 0xA0, 0x00, 0x1D, 0x00
	.byte 0x50, 0x00, 0x1D, 0x00, 0x70, 0x00, 0x1D, 0x00, 0x90, 0x00, 0x1D, 0x00

sNintendoWfcGraphicsBanks:
	.byte 0x01, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
	.byte 0x60, 0x00, 0x00, 0x00

gNintendoWfcPadCursorPositions:
	.byte 0x20, 0x00

gNintendoWfcPadCursorPositionsY:
	.byte 0x51, 0x00, 0x50, 0x00, 0x51, 0x00, 0x80, 0x00, 0x51, 0x00
	.byte 0xAE, 0x00, 0x51, 0x00, 0xE0, 0x00, 0x51, 0x00, 0x20, 0x00, 0x80, 0x00, 0x50, 0x00, 0x80, 0x00
	.byte 0x80, 0x00, 0x80, 0x00, 0xAE, 0x00, 0x80, 0x00, 0xE0, 0x00, 0x80, 0x00, 0x28, 0x00, 0xB0, 0x00
	.byte 0x80, 0x00, 0xB0, 0x00, 0xD8, 0x00, 0xB0, 0x00

sNintendoWfcBgTemplate_Sub1:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1B, 0x02, 0x00, 0x02, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

sNintendoWfcBgTemplate_Sub2:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1F, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sNintendoWfcBgTemplate_Main1:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x1E, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sNintendoWfcBgTemplate_Main0:
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1F, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sNintendoWfcBgTemplate_Sub0:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x04, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

gNintendoWfcPadKeysUpFromLeftButton:
	.byte 0x05, 0x06

gNintendoWfcPadKeysDownFromLeftButton:
	.byte 0x00, 0x01

gNintendoWfcPadKeysUpFromRightButton:
	.byte 0x08, 0x09

gNintendoWfcPadKeysDownFromRightButton:
	.byte 0x03, 0x04

gNintendoWfcNumberInputWindowX:
	.byte 0x01, 0x00, 0x00, 0x00
	.byte 0x0C, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00

gNintendoWfcPadKeyFlashes:
	.byte 0x01, 0x07
	.byte 0x07, 0x07
	.byte 0x0D, 0x07
	.byte 0x13, 0x07
	.byte 0x19, 0x07
	.byte 0x01, 0x0D
	.byte 0x07, 0x0D
	.byte 0x0D, 0x0D
	.byte 0x13, 0x0D
	.byte 0x19, 0x0D

gNintendoWfcPadNeighbors:
	.byte 0x0A, 0x05, 0x04, 0x01
	.byte 0x0A, 0x06, 0x00, 0x02, 0x0B, 0x07, 0x01, 0x03, 0x0C, 0x08, 0x02, 0x04, 0x0C, 0x09, 0x03, 0x00
	.byte 0x00, 0x0A, 0x09, 0x06, 0x01, 0x0A, 0x05, 0x07, 0x02, 0x0B, 0x06, 0x08, 0x03, 0x0C, 0x07, 0x09
	.byte 0x04, 0x0C, 0x08, 0x05, 0x0D, 0x0E, 0x0C, 0x0B, 0x07, 0x02, 0x0A, 0x0C, 0x0F, 0x10, 0x0B, 0x0A

gNintendoWfcYesNoCursorPositions:
	.byte 0x80, 0x00, 0x48, 0x00
	.byte 0x80, 0x00, 0x78, 0x00

gNintendoWfcYesNoHitboxes:
	.byte 0x32, 0x5C, 0x03, 0xFB, 0x63, 0x8C, 0x03, 0xFB
	.byte 0xFF, 0x00, 0x00, 0x00

	.data

sNintendoWfcRoomNumUrl:
	.asciz "http://gamestats2.gs.nintendowifi.net/pokemondpds/battletower/roomnum.asp"
	.balign 4, 0

sNintendoWfcDownloadUrl:
	.asciz "http://gamestats2.gs.nintendowifi.net/pokemondpds/battletower/download.asp"
	.balign 4, 0

sNintendoWfcUploadUrl:
	.asciz "http://gamestats2.gs.nintendowifi.net/pokemondpds/battletower/upload.asp"
	.balign 4, 0

sNintendoWfcInfoUrl:
	.asciz "http://gamestats2.gs.nintendowifi.net/pokemondpds/battletower/info.asp"
	.balign 4, 0

sNintendoWfcSetProfileUrl:
	.asciz "http://gamestats2.gs.nintendowifi.net/pokemondpds/common/setProfile.asp"
	.balign 4, 0

sNintendoWfcModeInit:
	.word NintendoWfc_SessionInit

sNintendoWfcModeMain:
	.word NintendoWfc_SessionMain

sNintendoWfcModeExit:
	.word NintendoWfc_SessionExit

sNintendoWfcStates:
	.word NintendoWfc_AskConnect
	.word NintendoWfc_HandleConnectChoice
	.word NintendoWfc_StartConnect
	.word NintendoWfc_WaitConnect
	.word NintendoWfc_StartAuth
	.word NintendoWfc_WaitAuth
	.word NintendoWfc_InitHttp
	.word NintendoWfc_RequestInfo
	.word NintendoWfc_WaitInfo
	.word NintendoWfc_RequestSetProfile
	.word NintendoWfc_WaitSetProfile
	.word NintendoWfc_RequestRoomCount
	.word NintendoWfc_WaitRoomCount
	.word NintendoWfc_AskChallengeRoom
	.word NintendoWfc_StartChallengeRoomInput
	.word NintendoWfc_HandleChallengeRoomInput
	.word NintendoWfc_PrintReceivingTrainers
	.word NintendoWfc_RequestTrainersDownload
	.word NintendoWfc_WaitTrainersDownload
	.word NintendoWfc_PrintTrainersReceived
	.word NintendoWfc_PrintRoomUnavailable
	.word NintendoWfc_AskCancelChallenge
	.word NintendoWfc_HandleCancelChallengeChoice
	.word NintendoWfc_AskCancelDownload
	.word NintendoWfc_HandleCancelDownloadChoice
	.word NintendoWfc_PrintSendingRecord
	.word NintendoWfc_RequestUpload
	.word NintendoWfc_WaitUpload
	.word NintendoWfc_PrintRecordSent
	.word NintendoWfc_AskLeadersRank
	.word NintendoWfc_StartLeadersRankInput
	.word NintendoWfc_HandleLeadersRankInput
	.word NintendoWfc_AskLeadersRoom
	.word NintendoWfc_StartLeadersRoomInput
	.word NintendoWfc_HandleLeadersRoomInput
	.word NintendoWfc_PrintReceivingLeaders
	.word NintendoWfc_RequestLeadersDownload
	.word NintendoWfc_WaitLeadersDownload
	.word NintendoWfc_PrintLeadersReceived
	.word NintendoWfc_StartSave
	.word NintendoWfc_Save
	.word NintendoWfc_PrintDisconnecting
	.word NintendoWfc_CloseConnection
	.word NintendoWfc_PrintDisconnected
	.word NintendoWfc_UnusedState_Idle
	.word NintendoWfc_StartDisconnect
	.word NintendoWfc_StartExit
	.word NintendoWfc_WaitMessage
	.word NintendoWfc_OpenYesNoMenu
	.word NintendoWfc_WaitMessageAndDelay
	.word NintendoWfc_UnusedState_HandleYesNo
	.word NintendoWfc_AskLogOut
	.word NintendoWfc_HandleLogOutChoice
	.word NintendoWfc_ShowHttpError
	.word NintendoWfc_Disconnect
	.word NintendoWfc_ShowDwcError
	.word NintendoWfc_WaitErrorDismiss

sNintendoWfcNumberInputStates:
	.word NintendoWfc_NumberInput_FadeOut
	.word NintendoWfc_NumberInput_Setup
	.word NintendoWfc_NumberInput_WaitFadeIn
	.word NintendoWfc_NumberInput_Main
	.word NintendoWfc_NumberInput_UnusedState4
	.word NintendoWfc_NumberInput_UnusedState5
	.word NintendoWfc_NumberInput_UnusedState6
	.word NintendoWfc_NumberInput_StartFadeOut
	.word NintendoWfc_NumberInput_Teardown
	.word NintendoWfc_NumberInput_WaitFadeOut
	.word NintendoWfc_NumberInput_WaitButtonFlash
	.word NintendoWfc_NumberInput_Finish

gNintendoWfcPadHitboxes_Max100:
	.byte 0x0E, 0x2F, 0x58, 0x77, 0x0E, 0x2F, 0x78, 0x97, 0x0E, 0x2F, 0x98, 0xB7
	.byte 0x3C, 0x63, 0x0C, 0x33, 0x3C, 0x63, 0x3C, 0x63, 0x3C, 0x63, 0x6C, 0x93, 0x3C, 0x63, 0x9E, 0xC3
	.byte 0x3C, 0x63, 0xCC, 0xF3, 0x6C, 0x93, 0x0C, 0x33, 0x6C, 0x93, 0x3C, 0x63, 0x6C, 0x93, 0x6C, 0x93
	.byte 0x6C, 0x93, 0x9E, 0xC3, 0x6C, 0x93, 0xCC, 0xF3, 0xA4, 0xBB, 0x04, 0x4B, 0xA4, 0xBB, 0x5C, 0xA3
	.byte 0xA4, 0xBB, 0xB4, 0xFB, 0xFF, 0x00, 0x00, 0x00

gNintendoWfcPadHitboxes:
	.byte 0x0E, 0x2F, 0x48, 0x67, 0x0E, 0x2F, 0x68, 0x87
	.byte 0x0E, 0x2F, 0x88, 0xA7, 0x3C, 0x63, 0x0C, 0x33, 0x3C, 0x63, 0x3C, 0x63, 0x3C, 0x63, 0x6C, 0x93
	.byte 0x3C, 0x63, 0x9E, 0xC3, 0x3C, 0x63, 0xCC, 0xF3, 0x6C, 0x93, 0x0C, 0x33, 0x6C, 0x93, 0x3C, 0x63
	.byte 0x6C, 0x93, 0x6C, 0x93, 0x6C, 0x93, 0x9E, 0xC3, 0x6C, 0x93, 0xCC, 0xF3, 0xA4, 0xBB, 0x04, 0x4B
	.byte 0xA4, 0xBB, 0x5C, 0xA3, 0xA4, 0xBB, 0xB4, 0xFB, 0xFF, 0x00, 0x00, 0x00

sNintendoWfcYesNoStates:
	.word NintendoWfcYesNo_Start, NintendoWfcYesNo_HandleInput, NintendoWfcYesNo_UnusedState2, NintendoWfcYesNo_WaitClose

	.bss

sNintendoWfcHttp:
	.space 0xC

sNintendoWfcHttpFriendKey:
	.space 0x8

gNintendoWfcHttpRequestBuffer:
	.space 0xCC

ov72_0223B900:
	.space 0x1B

gNintendoWfcHttpUploadFriendKey:
	.space 0x5

gNintendoWfcHttpResponse:
	.space 0x3

gNintendoWfcHttpShortResponse:
	.space 0x9

sNintendoWfcGlobal:
	.space 0x14
