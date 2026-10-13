	.extern WiiMessageSettings_Init
	.extern WiiMessageSettings_Main
	.extern WiiMessageSettings_Exit
	.extern WiiMessageSettings_CreateScreenArgs
	.extern WiiMessageSettings_DestroyScreenArgs
	.extern WiiMessageSettings_CreateWiiNumberEntryArgs
	.extern WiiMessageSettings_FinishWiiNumberEntry
	.extern WiiMessageSettings_CreateRegistrationCodeEntryArgs
	.extern WiiMessageSettings_FinishRegistrationCodeEntry
	.extern WiiMessageSettings_CreatePasswordEntryArgs
	.extern WiiMessageSettings_FinishPasswordEntry
	.extern ov75_02246EAC
	.extern ov75_02246EDC
	.extern WiiMessageSettingsScreen_Init
	.extern WiiMessageSettingsScreen_Main
	.extern WiiMessageSettingsScreen_Exit
	.extern WiiMessageSettingsScreen_MainMenu
	.extern WiiMessageSettingsScreen_StartCheckWiiNumber
	.extern WiiMessageSettingsScreen_SetMessageReception
	.extern WiiMessageSettingsScreen_DeleteSettings
	.extern WiiMessageSettingsScreen_RegisterWiiNumber
	.extern WiiMessageSettingsScreen_StartWiiNumberEntry
	.extern WiiMessageSettingsScreen_CheckWiiNumberEntry
	.extern WiiMessageSettingsScreen_ConfirmConnect
	.extern WiiMessageSettingsScreen_HandleConnectYesNo
	.extern WiiMessageSettingsScreen_PrintDisconnecting
	.extern WiiMessageSettingsScreen_Disconnect
	.extern WiiMessageSettingsScreen_PrintDisconnected
	.extern WiiMessageSettingsScreen_StartConnect
	.extern WiiMessageSettingsScreen_WaitConnect
	.extern WiiMessageSettingsScreen_StartAuth
	.extern WiiMessageSettingsScreen_WaitAuth
	.extern WiiMessageSettingsScreen_StartServerSession
	.extern WiiMessageSettingsScreen_SendRegistrationRequest
	.extern WiiMessageSettingsScreen_WaitRegistrationResponse
	.extern WiiMessageSettingsScreen_RegistrationCodeSent
	.extern WiiMessageSettingsScreen_StartRegistrationCodeEntry
	.extern WiiMessageSettingsScreen_ConfirmCancelRegistration
	.extern WiiMessageSettingsScreen_ConfirmRegistrationCode
	.extern WiiMessageSettingsScreen_StartPasswordEntry
	.extern WiiMessageSettingsScreen_SaveSettings
	.extern WiiMessageSettingsScreen_SaveGame
	.extern WiiMessageSettingsScreen_DisconnectAndWait
	.extern WiiMessageSettingsScreen_ShowConnectionError
	.extern WiiMessageSettingsScreen_WaitCloseError
	.extern WiiMessageSettingsScreen_FadeOut
	.extern WiiMessageSettingsScreen_ShowYesNoPrompt
	.extern WiiMessageSettingsScreen_ShowRegistrationError
	.extern WiiMessageSettingsScreen_DisconnectAndQuit
	.extern WiiMessageSettingsScreen_WaitMessage
	.extern WiiMessageSettingsScreen_WaitMessageAndDelay
	.extern WiiMessageSettingsScreen_WaitMessageAndShowYesNo
	.extern WiiMessageSettingsScreen_PrintWiiNumberReentry
	.extern WiiMessageSettingsScreen_CheckPassword
	.extern WiiMessageSettingsScreen_RegisteredMenuMoveCursor
	.extern WiiMessageSettingsScreen_UnregisteredMenuMoveCursor
	.public sWiiAddressPrefix
	.public sWiiAddressDomain
	.public sWiiNumberDigitChars
	.public gApplication_TitleScreen
	.public sWiiMessageSettingsRegisterChoicePositions
	.public _02249BE0
	.public WiiMessageSettings_Init
	.public WiiMessageSettings_Main
	.public WiiMessageSettings_Exit
	.public WiiMessageSettings_LoadNetworkOverlays
	.public WiiMessageSettings_UnloadNetworkOverlays
	.public WiiMessageSettings_SaveWiiAddress
	.public WiiMessageSettings_CreateWiiNumberEntryArgs
	.public WiiMessageSettings_FinishWiiNumberEntry
	.public WiiMessageSettings_CreateRegistrationCodeEntryArgs
	.public WiiMessageSettings_FinishRegistrationCodeEntry
	.public WiiMessageSettings_CreatePasswordEntryArgs
	.public WiiMessageSettings_FinishPasswordEntry
	.public WiiMessageSettingsScreen_Init
	.public WiiMessageSettingsScreen_Main
	.public WiiMessageSettingsScreen_Exit
	.public WiiMessageSettingsScreen_InitBgs
	.public WiiMessageSettingsScreen_LoadGraphics
	.public WiiMessageSettingsScreen_AnimatePaletteTask
	.public WiiMessageSettingsScreen_AddWindows
	.public WiiMessageSettingsScreen_CreateYesNoPrompt
	.public WiiMessageSettingsScreen_MainMenu
	.public WiiMessageSettingsScreen_SetMessageReception
	.public WiiMessageSettingsScreen_DeleteSettings
	.public WiiMessageSettingsScreen_PrintRegisterChoice
	.public WiiMessageSettingsScreen_AnimateScrollArrows
	.public WiiMessageSettingsScreen_PrintExplanation
	.public WiiMessageSettingsScreen_RunExplanation
	.public WiiMessageSettingsScreen_RegisterWiiNumber
	.public WiiMessageSettingsScreen_ConfirmConnect
	.public WiiMessageSettingsScreen_HandleConnectYesNo
	.public WiiMessageSettingsScreen_PrintDisconnecting
	.public WiiMessageSettingsScreen_PrintDisconnected
	.public WiiMessageSettingsScreen_StartConnect
	.public WiiMessageSettingsScreen_WaitConnect
	.public WiiMessageSettingsScreen_WaitAuth
	.public WiiMessageSettingsScreen_StartServerSession
	.public WiiMessageSettingsScreen_WaitRegistrationResponse
	.public WiiMessageSettingsScreen_RegistrationCodeSent
	.public WiiMessageSettingsScreen_ConfirmCancelRegistration
	.public WiiMessageSettingsScreen_ConfirmRegistrationCode
	.public WiiMessageSettingsScreen_StartPasswordEntry
	.public WiiMessageSettingsScreen_SaveSettings
	.public WiiMessageSettingsScreen_SaveGame
	.public WiiMessageSettingsScreen_DisconnectAndWait
	.public WiiMessageSettingsScreen_ShowConnectionError
	.public WiiMessageSettingsScreen_WaitCloseError
	.public WiiMessageSettingsScreen_ShowRegistrationError
	.public WiiMessageSettingsScreen_DisconnectAndQuit
	.public WiiMessageSettingsScreen_WaitMessageAndDelay
	.public WiiMessageSettingsScreen_PrintMessage
	.public WiiMessageSettingsScreen_IsPrinterActive
	.public WiiMessageSettingsScreen_PrintWiiNumberReentry
	.public WiiMessageSettingsScreen_CheckPassword
	.public WiiMessageSettingsScreen_GetAlignedX
	.public WiiMessageSettingsScreen_PrintAligned
	.public WiiMessageSettingsScreen_PrintError
	.public WiiMessageSettingsScreen_PrintErrorCode
	.public WiiNumber_ToWiiAddress
	.public WiiNumber_IsZero
	.public WiiNumber_ParseDigitGroups
	.public ov75_022498E4
	.public sWiiMessageSettingsScreenTemplate
	.public sWiiMessageSettingsApps
	.public sWiiMessageSettingsRegisteredMenuWindow
	.public sWiiMessageSettingsUnregisteredMenuWindow
	.public sWiiMessageSettingsGraphicsModes
	.public sWiiMessageSettingsUnregisteredMenuItems
	.public sWiiMessageSettingsUnregisteredMenuTemplate
	.public sWiiMessageSettingsRegisteredMenuItems
	.public sWiiMessageSettingsRegisteredMenuTemplate
	.public sWiiMessageSettingsGraphicsBanks
	.public sWiiMessageSettingsSubBgTemplates
	.public sWiiMessageSettingsMainBgTemplates
	.public sWiiAddressPrefix
	.public sWiiAddressDomain
	.public sWiiNumberDigitChars
	.public ov75_02249B24
	.public sWiiMessageSettingsScreenStates
	.public WiiMessageSettings_SetNextApp
	.public WiiMessageSettings_Quit
	.public WiiMessageSettings_SetScreenStartState
	.public WiiMessageSettings_GetScreenStartState
	.public WiiMessageSettings_Alloc
	.public WiiMessageSettings_Free
	.public WiiMessageSettings_SaveRegistrationCode
	.public WiiMessageSettings_SavePassword
	.public WiiMessageSettings_InitRequest
	.public WiiMessageSettings_SetRequestWiiAddress
	.public WiiMessageSettings_SetRequestRegistrationCode
	.public WiiMessageSettings_SetRegistrationCodePrefix
	.public WiiMessageSettings_GetRegistrationCode
	.public WiiMessageSettings_GetPassword
	.public WiiMessageSettings_SetWiiNumberEntryState
	.public WiiMessageSettings_GetWiiNumberEntryState
	.public WiiMessageSettings_CreateScreenArgs
	.public WiiMessageSettings_DestroyScreenArgs
	.public ov75_02246EAC
	.public ov75_02246EDC
	.public WiiMessageSettingsScreen_VBlankCB
	.public WiiMessageSettingsScreen_FreeBgs
	.public WiiMessageSettingsScreen_RemoveWindows
	.public WiiMessageSettingsScreen_SetState
	.public WiiMessageSettingsScreen_ShowWaitingIcon
	.public WiiMessageSettingsScreen_HideWaitingIcon
	.public WiiMessageSettingsScreen_StartCheckWiiNumber
	.public WiiMessageSettingsScreen_ClearRegisterChoice
	.public WiiMessageSettingsScreen_StartWiiNumberEntry
	.public WiiMessageSettingsScreen_CheckWiiNumberEntry
	.public WiiMessageSettingsScreen_Disconnect
	.public WiiMessageSettingsScreen_StartAuth
	.public WiiMessageSettingsScreen_SendRegistrationRequest
	.public WiiMessageSettingsScreen_StartRegistrationCodeEntry
	.public WiiMessageSettingsScreen_FadeOut
	.public WiiMessageSettingsScreen_ShowYesNoPrompt
	.public WiiMessageSettingsScreen_WaitMessage
	.public WiiMessageSettingsScreen_WaitMessageAndShowYesNo
	.public WiiMessageSettingsScreen_RegisteredMenuMoveCursor
	.public WiiMessageSettingsScreen_UnregisteredMenuMoveCursor
	.public ov75_App_MainMenu_SelectOption_WiiMessageSettings
	.include "asm/macros.inc"
	.include "overlay_75.inc"
	.include "global.inc"

.text
	.rodata

ov75_022498E4:
	.word WifiClub_Init, WifiClub_Main, WifiClub_Exit, FS_OVERLAY_ID(OVY_44)
sWiiMessageSettingsScreenTemplate:
	.word WiiMessageSettingsScreen_Init, WiiMessageSettingsScreen_Main, WiiMessageSettingsScreen_Exit, 0xFFFFFFFF

sWiiMessageSettingsApps: ; 0x02249904
	.word WiiMessageSettings_CreateScreenArgs, WiiMessageSettings_DestroyScreenArgs, sWiiMessageSettingsScreenTemplate, 0
	.word WiiMessageSettings_CreateWiiNumberEntryArgs, WiiMessageSettings_FinishWiiNumberEntry, gOverlayTemplate_CodeEntry, 0
	.word WiiMessageSettings_CreateRegistrationCodeEntryArgs, WiiMessageSettings_FinishRegistrationCodeEntry, gOverlayTemplate_CodeEntry, 0
	.word WiiMessageSettings_CreatePasswordEntryArgs, WiiMessageSettings_FinishPasswordEntry, gOverlayTemplate_CodeEntry, 0
	.word WiiMessageSettings_CreatePasswordEntryArgs, WiiMessageSettings_FinishPasswordEntry, gOverlayTemplate_CodeEntry, 0
	.word ov75_02246EAC, ov75_02246EDC, ov75_022498E4, 1

ov75_App_MainMenu_SelectOption_WiiMessageSettings:
	.word WiiMessageSettings_Init, WiiMessageSettings_Main, WiiMessageSettings_Exit, 0xFFFFFFFF
sWiiMessageSettingsRegisteredMenuWindow:
	.byte 0x0B, 0x07, 0x14, 0x08

sWiiMessageSettingsUnregisteredMenuWindow: ; 0x02249978
	.byte 0x0B, 0x0D, 0x14, 0x04

sWiiMessageSettingsGraphicsModes: ; 0x0224997C
	.byte 0x01, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWiiMessageSettingsUnregisteredMenuItems: ; 0x0224998C
	.byte 0x01, 0x00, 0x00, 0x00
	.byte 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1D, 0x00, 0x00, 0x00

sWiiMessageSettingsUnregisteredMenuTemplate: ; 0x0224999C
	.byte 0x00, 0x00, 0x00, 0x00
	.word WiiMessageSettingsScreen_UnregisteredMenuMoveCursor
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x02, 0x00
	.byte 0x00, 0x08, 0x00, 0x10, 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWiiMessageSettingsRegisteredMenuItems: ; 0x022499BC
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1D, 0x00, 0x00, 0x00

sWiiMessageSettingsRegisteredMenuTemplate: ; 0x022499DC
	.byte 0x00, 0x00, 0x00, 0x00
	.word WiiMessageSettingsScreen_RegisteredMenuMoveCursor
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x04, 0x00
	.byte 0x00, 0x08, 0x00, 0x10, 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWiiMessageSettingsGraphicsBanks: ; 0x022499FC
	.byte 0x01, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
	.byte 0x60, 0x00, 0x00, 0x00

sWiiMessageSettingsSubBgTemplates: ; 0x02249A24
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x1B, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWiiMessageSettingsMainBgTemplates: ; 0x02249A5C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1F, 0x00
	.byte 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x02, 0x00, 0x03, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1D, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x1C, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWiiAddressPrefix: ; 0x02249ACC
	.byte 0x77, 0x00, 0x00, 0x00

	.balign 4, 0
sWiiAddressDomain: ; 0x02249AD0
	.asciz "@wii.com"

	.balign 4, 0
sWiiNumberDigitChars: ; 0x02249ADC
	.byte 0x21, 0x01, 0x30, 0x00
	.byte 0x22, 0x01, 0x31, 0x00, 0x23, 0x01, 0x32, 0x00, 0x24, 0x01, 0x33, 0x00, 0x25, 0x01, 0x34, 0x00
	.byte 0x26, 0x01, 0x35, 0x00, 0x27, 0x01, 0x36, 0x00, 0x28, 0x01, 0x37, 0x00, 0x29, 0x01, 0x38, 0x00
	.byte 0x2A, 0x01, 0x39, 0x00

	.data

sWiiMessageSettingsScreenStates: ; 0x02249B30
	.word WiiMessageSettingsScreen_MainMenu
	.word WiiMessageSettingsScreen_StartCheckWiiNumber
	.word WiiMessageSettingsScreen_SetMessageReception
	.word WiiMessageSettingsScreen_DeleteSettings
	.word WiiMessageSettingsScreen_RegisterWiiNumber
	.word WiiMessageSettingsScreen_StartWiiNumberEntry
	.word WiiMessageSettingsScreen_CheckWiiNumberEntry
	.word WiiMessageSettingsScreen_ConfirmConnect
	.word WiiMessageSettingsScreen_HandleConnectYesNo
	.word WiiMessageSettingsScreen_PrintDisconnecting
	.word WiiMessageSettingsScreen_Disconnect
	.word WiiMessageSettingsScreen_PrintDisconnected
	.word WiiMessageSettingsScreen_StartConnect
	.word WiiMessageSettingsScreen_WaitConnect
	.word WiiMessageSettingsScreen_StartAuth
	.word WiiMessageSettingsScreen_WaitAuth
	.word WiiMessageSettingsScreen_StartServerSession
	.word WiiMessageSettingsScreen_SendRegistrationRequest
	.word WiiMessageSettingsScreen_WaitRegistrationResponse
	.word WiiMessageSettingsScreen_RegistrationCodeSent
	.word WiiMessageSettingsScreen_StartRegistrationCodeEntry
	.word WiiMessageSettingsScreen_ConfirmCancelRegistration
	.word WiiMessageSettingsScreen_ConfirmRegistrationCode
	.word WiiMessageSettingsScreen_StartPasswordEntry
	.word WiiMessageSettingsScreen_SaveSettings
	.word WiiMessageSettingsScreen_SaveGame
	.word WiiMessageSettingsScreen_DisconnectAndWait
	.word WiiMessageSettingsScreen_ShowConnectionError
	.word WiiMessageSettingsScreen_WaitCloseError
	.word WiiMessageSettingsScreen_FadeOut
	.word WiiMessageSettingsScreen_FadeOut
	.word WiiMessageSettingsScreen_ShowYesNoPrompt
	.word WiiMessageSettingsScreen_ShowRegistrationError
	.word WiiMessageSettingsScreen_DisconnectAndQuit
	.word WiiMessageSettingsScreen_WaitMessage
	.word WiiMessageSettingsScreen_WaitMessageAndDelay
	.word WiiMessageSettingsScreen_WaitMessageAndShowYesNo
	.word WiiMessageSettingsScreen_PrintWiiNumberReentry
	.word WiiMessageSettingsScreen_CheckPassword

	.bss

_02249BE0:
	.space 0x20
