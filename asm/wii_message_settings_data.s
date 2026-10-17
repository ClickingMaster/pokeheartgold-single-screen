	.extern WiiMessageSettings_Init
	.extern WiiMessageSettings_Main
	.extern WiiMessageSettings_Exit
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
	.public sWiiMessageSettingsHeap
	.public WiiMessageSettings_Init
	.public WiiMessageSettings_Main
	.public WiiMessageSettings_Exit
	.public WiiMessageSettingsScreen_MainMenu
	.public WiiMessageSettingsScreen_SetMessageReception
	.public WiiMessageSettingsScreen_DeleteSettings
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
	.public WiiMessageSettingsScreen_PrintWiiNumberReentry
	.public WiiMessageSettingsScreen_CheckPassword
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
	.public sWiiMessageSettingsScreenStates
	.public WiiMessageSettingsScreen_StartCheckWiiNumber
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

	.rodata

ov75_App_MainMenu_SelectOption_WiiMessageSettings:
	.word WiiMessageSettings_Init, WiiMessageSettings_Main, WiiMessageSettings_Exit, 0xFFFFFFFF
sWiiMessageSettingsRegisteredMenuWindow:
	.byte 0x0B, 0x07, 0x14, 0x08

sWiiMessageSettingsUnregisteredMenuWindow:
	.byte 0x0B, 0x0D, 0x14, 0x04

sWiiMessageSettingsGraphicsModes:
	.byte 0x01, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWiiMessageSettingsUnregisteredMenuItems:
	.byte 0x01, 0x00, 0x00, 0x00
	.byte 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1D, 0x00, 0x00, 0x00

sWiiMessageSettingsUnregisteredMenuTemplate:
	.byte 0x00, 0x00, 0x00, 0x00
	.word WiiMessageSettingsScreen_UnregisteredMenuMoveCursor
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x02, 0x00
	.byte 0x00, 0x08, 0x00, 0x10, 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWiiMessageSettingsRegisteredMenuItems:
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1D, 0x00, 0x00, 0x00

sWiiMessageSettingsRegisteredMenuTemplate:
	.byte 0x00, 0x00, 0x00, 0x00
	.word WiiMessageSettingsScreen_RegisteredMenuMoveCursor
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x04, 0x00
	.byte 0x00, 0x08, 0x00, 0x10, 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWiiMessageSettingsGraphicsBanks:
	.byte 0x01, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
	.byte 0x60, 0x00, 0x00, 0x00

sWiiMessageSettingsSubBgTemplates:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x1B, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWiiMessageSettingsMainBgTemplates:
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1F, 0x00
	.byte 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x02, 0x00, 0x03, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1D, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x1C, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.data

sWiiMessageSettingsScreenStates:
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

sWiiMessageSettingsHeap:
	.space 0x20
