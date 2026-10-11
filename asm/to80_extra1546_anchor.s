	.extern NintendoWfc_SessionInit
	.extern NintendoWfc_SessionMain
	.extern NintendoWfc_SessionExit
	.extern ov72_022390D8
	.extern ov72_02239108
	.extern ov72_02239168
	.extern ov72_02239198
	.extern ov72_022391E8
	.extern ov72_02239220
	.extern ov72_022392AC
	.extern ov72_022392BC
	.extern ov72_02239370
	.extern ov72_022393B0
	.extern ov72_022393CC
	.extern ov72_022394A4
	.extern ov72_022394D8
	.extern ov72_02239614
	.extern ov72_02239638
	.extern ov72_022396E8
	.extern ov72_02239760
	.extern ov72_02239784
	.extern ov72_022397E8
	.extern ov72_0223985C
	.extern ov72_0223988C
	.extern ov72_022398BC
	.extern ov72_022399CC
	.extern ov72_02239A08
	.extern ov72_02239A3C
	.extern ov72_02239A88
	.extern ov72_02239B5C
	.extern ov72_02239B98
	.extern ov72_02239BC8
	.extern ov72_02239BEC
	.extern ov72_02239C14
	.extern ov72_02239C8C
	.extern ov72_02239CB4
	.extern ov72_02239CD8
	.extern ov72_02239D3C
	.extern ov72_02239D6C
	.extern ov72_02239E58
	.extern ov72_02239E94
	.extern NintendoWfc_StartSave
	.extern NintendoWfc_Save
	.extern ov72_02239ED4
	.extern ov72_02239F04
	.extern ov72_02239F30
	.extern ov72_02239F60
	.extern ov72_02239F8C
	.extern ov72_02239FBC
	.extern ov72_02239FD8
	.extern ov72_0223A010
	.extern NintendoWfc_StartExit
	.extern ov72_0223A044
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
	.extern ov72_0223ABF4
	.extern ov72_0223ABF8
	.extern ov72_0223ABFC
	.extern NintendoWfc_NumberInput_StartFadeOut
	.extern NintendoWfc_NumberInput_Teardown
	.extern NintendoWfc_NumberInput_WaitButtonFlash
	.extern NintendoWfc_NumberInput_WaitFadeOut
	.extern NintendoWfc_NumberInput_Finish
	.extern NintendoWfcYesNo_Start
	.extern NintendoWfcYesNo_HandleInput
	.extern ov72_0223B084
	.extern NintendoWfcYesNo_WaitClose
	.public ov72_0223B4C0
	.public ov72_0223B4C2
	.public NintendoWfc_Exit
	.public NintendoWfc_Init
	.public NintendoWfc_Main
	.public _0223B330
	.public sNintendoWfcRoomNumUrl
	.public sNintendoWfcHttp
	.public NintendoWfcHttp_Update
	.public NintendoWfcHttp_IsDone
	.public NintendoWfcHttp_RequestRoomCount
	.public NintendoWfcHttp_RequestDownload
	.public NintendoWfcHttp_RequestUpload
	.public NintendoWfcHttp_RequestInfo
	.public NintendoWfcHttp_RequestSetProfile
	.public NintendoWfcHttp_StartRequest
	.public NintendoWfcHttp_ConvertError
	.public NintendoWfc_VBlankCB
	.public NintendoWfc_SetGfxBanks
	.public NintendoWfc_InitObjTransfer
	.public NintendoWfc_InitSpriteResources
	.public NintendoWfc_CreateSprites
	.public NintendoWfc_SetSpritePosition
	.public NintendoWfc_MovePadCursor
	.public NintendoWfc_MoveDigitCursor
	.public NintendoWfc_SetDigitSpritePosition
	.public NintendoWfc_Alloc
	.public NintendoWfc_InitGraphics
	.public NintendoWfc_FreeGraphics
	.public NintendoWfc_SessionInit
	.public NintendoWfc_SessionMain
	.public NintendoWfc_SessionExit
	.public NintendoWfc_InitBgs
	.public NintendoWfc_LoadSubScreenBg
	.public NintendoWfc_LoadBgGraphics
	.public NintendoWfc_PaletteCycleTask
	.public NintendoWfc_AddWindows
	.public NintendoWfc_InitStrings
	.public ov72_02239108
	.public ov72_02239198
	.public ov72_022391E8
	.public ov72_02239220
	.public ov72_022392BC
	.public ov72_02239370
	.public ov72_022393CC
	.public ov72_022394A4
	.public ov72_022394D8
	.public ov72_02239638
	.public ov72_022396E8
	.public ov72_02239760
	.public ov72_02239784
	.public ov72_022397E8
	.public ov72_022398BC
	.public ov72_02239A3C
	.public ov72_02239A88
	.public ov72_02239BEC
	.public ov72_02239C14
	.public ov72_02239CB4
	.public ov72_02239CD8
	.public ov72_02239D6C
	.public ov72_02239E58
	.public ov72_02239F04
	.public ov72_02239F60
	.public ov72_02239F8C
	.public ov72_02239FD8
	.public NintendoWfc_StartExit
	.public ov72_0223A044
	.public NintendoWfc_ShowHttpError
	.public NintendoWfc_Disconnect
	.public NintendoWfc_WaitMessage
	.public NintendoWfc_WaitMessageAndDelay
	.public NintendoWfc_OpenYesNoMenu
	.public NintendoWfc_PrintFormattedMessage
	.public NintendoWfc_ShowDwcError
	.public NintendoWfc_WaitErrorDismiss
	.public NintendoWfc_PrintMessage
	.public NintendoWfc_GetAlignedTextX
	.public NintendoWfc_PrintAlignedText
	.public NintendoWfc_ShowWaitingIcon
	.public NintendoWfc_PrintErrorMessage
	.public NintendoWfc_ShowErrorMessage
	.public NintendoWfc_StartNumberInput
	.public NintendoWfc_RunNumberInput
	.public NintendoWfc_NumberInput_FadeOut
	.public NintendoWfc_NumberInput_Setup
	.public NintendoWfc_ShowNumberInputSprites
	.public NintendoWfc_GetTouchedPadKey
	.public NintendoWfc_GetPadNeighbor
	.public NintendoWfc_GetEnteredNumber
	.public NintendoWfc_AreDigitsComplete
	.public NintendoWfc_PressPadKey
	.public NintendoWfc_HandlePadButtons
	.public NintendoWfc_NumberInput_Main
	.public NintendoWfc_UpdateDigitSprites
	.public NintendoWfc_NumberInput_StartFadeOut
	.public NintendoWfc_NumberInput_Teardown
	.public NintendoWfc_AddNumberInputWindows
	.public NintendoWfc_PrintButtonLabel
	.public NintendoWfc_RemoveNumberInputWindows
	.public NintendoWfc_HideNumberInput
	.public NintendoWfc_SetButtonFlashPltt
	.public NintendoWfc_UpdateButtonFlash
	.public NintendoWfc_StartButtonFlash
	.public NintendoWfcYesNo_Init
	.public NintendoWfcYesNo_Run
	.public NintendoWfcYesNo_HandleInput
	.public NintendoWfcYesNo_WaitClose
	.public NintendoWfcYesNo_LoadGraphics
	.public NintendoWfcYesNo_AddWindows
	.public NintendoWfcYesNo_RemoveWindows
	.public NintendoWfcYesNo_MoveCursor
	.public ov72_0223B338
	.public sNintendoWfcObjCharTransferTemplate
	.public sNintendoWfcGraphicsModes
	.public ov72_0223B364
	.public ov72_0223B366
	.public sNintendoWfcGraphicsBanks
	.public ov72_0223B3A4
	.public ov72_0223B3A6
	.public sNintendoWfcBgTemplate_Sub1
	.public sNintendoWfcBgTemplate_Sub2
	.public sNintendoWfcBgTemplate_Main1
	.public sNintendoWfcBgTemplate_Main0
	.public sNintendoWfcBgTemplate_Sub0
	.public ov72_0223B464
	.public ov72_0223B466
	.public ov72_0223B468
	.public ov72_0223B46A
	.public ov72_0223B46C
	.public ov72_0223B478
	.public ov72_0223B479
	.public ov72_0223B48C
	.public ov72_0223B4C0
	.public ov72_0223B4C2
	.public ov72_0223B4C8
	.public sNintendoWfcDownloadUrl
	.public sNintendoWfcUploadUrl
	.public sNintendoWfcInfoUrl
	.public sNintendoWfcSetProfileUrl
	.public sNintendoWfcModeInit
	.public sNintendoWfcModeMain
	.public sNintendoWfcModeExit
	.public sNintendoWfcStates
	.public sNintendoWfcNumberInputStates
	.public ov72_0223B774
	.public ov72_0223B7B8
	.public sNintendoWfcYesNoStates
	.public ov72_0223B82C
	.public ov72_0223B834
	.public ov72_0223B900
	.public ov72_0223B91B
	.public ov72_0223B920
	.public ov72_0223B923
	.public sNintendoWfcGlobal
	.public NintendoWfcHttp_Init
	.public NintendoWfcHttp_GetResult
	.public NintendoWfc_InitArgs
	.public ov72_02238160
	.public NintendoWfc_InitSpriteTemplate
	.public NintendoWfc_HandleYesNoPrompt
	.public NintendoWfc_SetState
	.public NintendoWfc_Free
	.public NintendoWfc_GetWifiIconLevel
	.public NintendoWfc_SetNextMode
	.public NintendoWfc_FreeBgs
	.public NintendoWfc_RemoveWindows
	.public NintendoWfc_FreeStrings
	.public ov72_022390D8
	.public ov72_02239168
	.public ov72_022392AC
	.public ov72_022393B0
	.public ov72_02239614
	.public ov72_0223985C
	.public ov72_0223988C
	.public ov72_022399CC
	.public ov72_02239A08
	.public ov72_02239B5C
	.public ov72_02239B98
	.public ov72_02239BC8
	.public ov72_02239C8C
	.public ov72_02239D3C
	.public ov72_02239E94
	.public NintendoWfc_StartSave
	.public NintendoWfc_Save
	.public ov72_02239ED4
	.public ov72_02239F30
	.public ov72_02239FBC
	.public ov72_0223A010
	.public ov72_0223A414
	.public ov72_0223A41C
	.public NintendoWfc_HideWaitingIcon
	.public NintendoWfc_NumberInput_WaitFadeIn
	.public NintendoWfc_GetCursorAfterDigit
	.public ov72_0223ABF4
	.public ov72_0223ABF8
	.public ov72_0223ABFC
	.public NintendoWfc_NumberInput_WaitButtonFlash
	.public NintendoWfc_NumberInput_WaitFadeOut
	.public NintendoWfc_NumberInput_Finish
	.public NintendoWfcYesNo_Start
	.public ov72_0223B084
	.public NintendoWfcYesNo_ShowCursor
	.include "asm/macros.inc"
	.include "overlay_72.inc"
	.include "global.inc"

.text
	.rodata

_0223B330:
	.byte 0x00, 0x17, 0x0D, 0x07, 0x04, 0x0D, 0x00, 0x00

ov72_0223B338: ; 0x0223B338
	.byte 0x28, 0x00, 0xB0, 0x00, 0x80, 0x00, 0xB0, 0x00
	.byte 0xD8, 0x00, 0xB0, 0x00

sNintendoWfcObjCharTransferTemplate: ; 0x0223B344
	.byte 0x14, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x43, 0x00, 0x00, 0x00

sNintendoWfcGraphicsModes: ; 0x0223B354
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

ov72_0223B364: ; 0x0223B364
	.byte 0x60, 0x00

ov72_0223B366: ; 0x0223B366
	.byte 0x1D, 0x00, 0x80, 0x00, 0x1D, 0x00, 0xA0, 0x00, 0x1D, 0x00
	.byte 0x50, 0x00, 0x1D, 0x00, 0x70, 0x00, 0x1D, 0x00, 0x90, 0x00, 0x1D, 0x00

sNintendoWfcGraphicsBanks: ; 0x0223B37C
	.byte 0x01, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
	.byte 0x60, 0x00, 0x00, 0x00

ov72_0223B3A4: ; 0x0223B3A4
	.byte 0x20, 0x00

ov72_0223B3A6: ; 0x0223B3A6
	.byte 0x51, 0x00, 0x50, 0x00, 0x51, 0x00, 0x80, 0x00, 0x51, 0x00
	.byte 0xAE, 0x00, 0x51, 0x00, 0xE0, 0x00, 0x51, 0x00, 0x20, 0x00, 0x80, 0x00, 0x50, 0x00, 0x80, 0x00
	.byte 0x80, 0x00, 0x80, 0x00, 0xAE, 0x00, 0x80, 0x00, 0xE0, 0x00, 0x80, 0x00, 0x28, 0x00, 0xB0, 0x00
	.byte 0x80, 0x00, 0xB0, 0x00, 0xD8, 0x00, 0xB0, 0x00

sNintendoWfcBgTemplate_Sub1: ; 0x0223B3D8
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1B, 0x02, 0x00, 0x02, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

sNintendoWfcBgTemplate_Sub2: ; 0x0223B3F4
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1F, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sNintendoWfcBgTemplate_Main1: ; 0x0223B410
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x1E, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sNintendoWfcBgTemplate_Main0: ; 0x0223B42C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1F, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sNintendoWfcBgTemplate_Sub0: ; 0x0223B448
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x04, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

ov72_0223B464: ; 0x0223B464
	.byte 0x05, 0x06

ov72_0223B466: ; 0x0223B466
	.byte 0x00, 0x01

ov72_0223B468: ; 0x0223B468
	.byte 0x08, 0x09

ov72_0223B46A: ; 0x0223B46A
	.byte 0x03, 0x04

ov72_0223B46C: ; 0x0223B46C
	.byte 0x01, 0x00, 0x00, 0x00
	.byte 0x0C, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00

ov72_0223B478: ; 0x0223B478
	.byte 0x01

ov72_0223B479: ; 0x0223B479
	.byte 0x07, 0x07, 0x07, 0x0D, 0x07, 0x13, 0x07
	.byte 0x19, 0x07, 0x01, 0x0D, 0x07, 0x0D, 0x0D, 0x0D, 0x13, 0x0D, 0x19, 0x0D

ov72_0223B48C: ; 0x0223B48C
	.byte 0x0A, 0x05, 0x04, 0x01
	.byte 0x0A, 0x06, 0x00, 0x02, 0x0B, 0x07, 0x01, 0x03, 0x0C, 0x08, 0x02, 0x04, 0x0C, 0x09, 0x03, 0x00
	.byte 0x00, 0x0A, 0x09, 0x06, 0x01, 0x0A, 0x05, 0x07, 0x02, 0x0B, 0x06, 0x08, 0x03, 0x0C, 0x07, 0x09
	.byte 0x04, 0x0C, 0x08, 0x05, 0x0D, 0x0E, 0x0C, 0x0B, 0x07, 0x02, 0x0A, 0x0C, 0x0F, 0x10, 0x0B, 0x0A

ov72_0223B4C0: ; 0x0223B4C0
	.byte 0x80, 0x00

ov72_0223B4C2: ; 0x0223B4C2
	.byte 0x48, 0x00, 0x80, 0x00, 0x78, 0x00

ov72_0223B4C8: ; 0x0223B4C8
	.byte 0x32, 0x5C, 0x03, 0xFB, 0x63, 0x8C, 0x03, 0xFB
	.byte 0xFF, 0x00, 0x00, 0x00

	.data

sNintendoWfcRoomNumUrl:
	.asciz "http://gamestats2.gs.nintendowifi.net/pokemondpds/battletower/roomnum.asp"
	.balign 4, 0

sNintendoWfcDownloadUrl: ; 0x0223B52C
	.asciz "http://gamestats2.gs.nintendowifi.net/pokemondpds/battletower/download.asp"
	.balign 4, 0

sNintendoWfcUploadUrl: ; 0x0223B578
	.asciz "http://gamestats2.gs.nintendowifi.net/pokemondpds/battletower/upload.asp"
	.balign 4, 0

sNintendoWfcInfoUrl: ; 0x0223B5C4
	.asciz "http://gamestats2.gs.nintendowifi.net/pokemondpds/battletower/info.asp"
	.balign 4, 0

sNintendoWfcSetProfileUrl: ; 0x0223B60C
	.asciz "http://gamestats2.gs.nintendowifi.net/pokemondpds/common/setProfile.asp"
	.balign 4, 0

sNintendoWfcModeInit: ; 0x0223B654
	.word NintendoWfc_SessionInit

sNintendoWfcModeMain: ; 0x0223B658
	.word NintendoWfc_SessionMain

sNintendoWfcModeExit: ; 0x0223B65C
	.word NintendoWfc_SessionExit

sNintendoWfcStates: ; 0x0223B660
	.word ov72_022390D8
	.word ov72_02239108
	.word ov72_022391E8
	.word ov72_02239220
	.word ov72_022392AC
	.word ov72_022392BC
	.word ov72_02239370
	.word ov72_022393B0
	.word ov72_022393CC
	.word ov72_022394A4
	.word ov72_022394D8
	.word ov72_02239614
	.word ov72_02239638
	.word ov72_022396E8
	.word ov72_02239760
	.word ov72_02239784
	.word ov72_022397E8
	.word ov72_0223988C
	.word ov72_022398BC
	.word ov72_022399CC
	.word ov72_0223985C
	.word ov72_02239ED4
	.word ov72_02239F04
	.word ov72_02239F30
	.word ov72_02239F60
	.word ov72_02239A08
	.word ov72_02239A3C
	.word ov72_02239A88
	.word ov72_02239B5C
	.word ov72_02239B98
	.word ov72_02239BC8
	.word ov72_02239BEC
	.word ov72_02239C14
	.word ov72_02239C8C
	.word ov72_02239CB4
	.word ov72_02239CD8
	.word ov72_02239D3C
	.word ov72_02239D6C
	.word ov72_02239E58
	.word NintendoWfc_StartSave
	.word NintendoWfc_Save
	.word ov72_02239F8C
	.word ov72_02239FBC
	.word ov72_02239FD8
	.word ov72_0223A010
	.word ov72_02239E94
	.word NintendoWfc_StartExit
	.word NintendoWfc_WaitMessage
	.word NintendoWfc_OpenYesNoMenu
	.word NintendoWfc_WaitMessageAndDelay
	.word ov72_0223A044
	.word ov72_02239168
	.word ov72_02239198
	.word NintendoWfc_ShowHttpError
	.word NintendoWfc_Disconnect
	.word NintendoWfc_ShowDwcError
	.word NintendoWfc_WaitErrorDismiss

sNintendoWfcNumberInputStates: ; 0x0223B744
	.word NintendoWfc_NumberInput_FadeOut
	.word NintendoWfc_NumberInput_Setup
	.word NintendoWfc_NumberInput_WaitFadeIn
	.word NintendoWfc_NumberInput_Main
	.word ov72_0223ABF4
	.word ov72_0223ABF8
	.word ov72_0223ABFC
	.word NintendoWfc_NumberInput_StartFadeOut
	.word NintendoWfc_NumberInput_Teardown
	.word NintendoWfc_NumberInput_WaitFadeOut
	.word NintendoWfc_NumberInput_WaitButtonFlash
	.word NintendoWfc_NumberInput_Finish

ov72_0223B774: ; 0x0223B774
	.byte 0x0E, 0x2F, 0x58, 0x77, 0x0E, 0x2F, 0x78, 0x97, 0x0E, 0x2F, 0x98, 0xB7
	.byte 0x3C, 0x63, 0x0C, 0x33, 0x3C, 0x63, 0x3C, 0x63, 0x3C, 0x63, 0x6C, 0x93, 0x3C, 0x63, 0x9E, 0xC3
	.byte 0x3C, 0x63, 0xCC, 0xF3, 0x6C, 0x93, 0x0C, 0x33, 0x6C, 0x93, 0x3C, 0x63, 0x6C, 0x93, 0x6C, 0x93
	.byte 0x6C, 0x93, 0x9E, 0xC3, 0x6C, 0x93, 0xCC, 0xF3, 0xA4, 0xBB, 0x04, 0x4B, 0xA4, 0xBB, 0x5C, 0xA3
	.byte 0xA4, 0xBB, 0xB4, 0xFB, 0xFF, 0x00, 0x00, 0x00

ov72_0223B7B8: ; 0x0223B7B8
	.byte 0x0E, 0x2F, 0x48, 0x67, 0x0E, 0x2F, 0x68, 0x87
	.byte 0x0E, 0x2F, 0x88, 0xA7, 0x3C, 0x63, 0x0C, 0x33, 0x3C, 0x63, 0x3C, 0x63, 0x3C, 0x63, 0x6C, 0x93
	.byte 0x3C, 0x63, 0x9E, 0xC3, 0x3C, 0x63, 0xCC, 0xF3, 0x6C, 0x93, 0x0C, 0x33, 0x6C, 0x93, 0x3C, 0x63
	.byte 0x6C, 0x93, 0x6C, 0x93, 0x6C, 0x93, 0x9E, 0xC3, 0x6C, 0x93, 0xCC, 0xF3, 0xA4, 0xBB, 0x04, 0x4B
	.byte 0xA4, 0xBB, 0x5C, 0xA3, 0xA4, 0xBB, 0xB4, 0xFB, 0xFF, 0x00, 0x00, 0x00

sNintendoWfcYesNoStates: ; 0x0223B7FC
	.word NintendoWfcYesNo_Start, NintendoWfcYesNo_HandleInput, ov72_0223B084, NintendoWfcYesNo_WaitClose

	.bss

sNintendoWfcHttp:
	.space 0xC

ov72_0223B82C: ; 0x0223B82C
	.space 0x8

ov72_0223B834: ; 0x0223B834
	.space 0xCC

ov72_0223B900: ; 0x0223B900
	.space 0x1B

ov72_0223B91B: ; 0x0223B91B
	.space 0x5

ov72_0223B920: ; 0x0223B920
	.space 0x3

ov72_0223B923: ; 0x0223B923
	.space 0x9

sNintendoWfcGlobal: ; 0x0223B92C
	.space 0x14
