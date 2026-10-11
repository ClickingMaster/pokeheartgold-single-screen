	.include "asm/macros.inc"
	.include "overlay_75.inc"
	.include "global.inc"
	.public gApplication_TitleScreen

	.text
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
	.public ov75_02248684
	.public WiiMessageSettingsScreen_PrintDisconnecting
	.public WiiMessageSettingsScreen_PrintDisconnected
	.public WiiMessageSettingsScreen_StartConnect
	.public WiiMessageSettingsScreen_WaitConnect
	.public ov75_022488BC
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
	.public ov75_App_MainMenu_SelectOption_WiiMessageSettings
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
	.public ov75_022488AC
	.public WiiMessageSettingsScreen_SendRegistrationRequest
	.public WiiMessageSettingsScreen_StartRegistrationCodeEntry
	.public WiiMessageSettingsScreen_FadeOut
	.public WiiMessageSettingsScreen_ShowYesNoPrompt
	.public WiiMessageSettingsScreen_WaitMessage
	.public WiiMessageSettingsScreen_WaitMessageAndShowYesNo
	.public WiiMessageSettingsScreen_RegisteredMenuMoveCursor
	.public WiiMessageSettingsScreen_UnregisteredMenuMoveCursor

	thumb_func_start WiiMessageSettingsScreen_ConfirmConnect
WiiMessageSettingsScreen_ConfirmConnect: ; 0x02248584
	push {r3, r4, lr}
	sub sp, #4
	add r4, r0, #0
	ldr r0, [r4, #0x44]
	bl WiiMessageSettingsScreen_IsPrinterActive
	cmp r0, #1
	bne _0224859A
	add sp, #4
	mov r0, #0
	pop {r3, r4, pc}
_0224859A:
	add r0, r4, #0
	add r0, #0x94
	ldr r0, [r0]
	cmp r0, #4
	bhi _02248678
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_022485B0: ; jump table
	.short _022485BA - _022485B0 - 2 ; case 0
	.short _022485C8 - _022485B0 - 2 ; case 1
	.short _022485E6 - _022485B0 - 2 ; case 2
	.short _02248606 - _022485B0 - 2 ; case 3
	.short _02248646 - _022485B0 - 2 ; case 4
_022485BA:
	add r0, r4, #0
	add r0, #0x94
	ldr r0, [r0]
	add r4, #0x94
	add r0, r0, #1
	str r0, [r4]
	b _02248678
_022485C8:
	ldr r0, _02248680 ; =0x00000F0F
	mov r2, #0xb
	str r0, [sp]
	ldr r1, [r4, #0x34]
	add r0, r4, #0
	mov r3, #1
	bl WiiMessageSettingsScreen_PrintMessage
	add r0, r4, #0
	add r0, #0x94
	ldr r0, [r0]
	add r4, #0x94
	add r0, r0, #1
	str r0, [r4]
	b _02248678
_022485E6:
	mov r1, #0x8d
	ldr r0, [r4, #4]
	lsl r1, r1, #2
	mov r2, #0
	bl WiiMessageSettingsScreen_CreateYesNoPrompt
	add r1, r4, #0
	add r1, #0x88
	str r0, [r1]
	add r0, r4, #0
	add r0, #0x94
	ldr r0, [r0]
	add r4, #0x94
	add r0, r0, #1
	str r0, [r4]
	b _02248678
_02248606:
	add r0, r4, #0
	add r0, #0x88
	ldr r0, [r0]
	bl YesNoPrompt_HandleInput
	cmp r0, #1
	bne _0224862C
	add r0, r4, #0
	add r0, #0x88
	ldr r0, [r0]
	bl YesNoPrompt_Destroy
	add r0, r4, #0
	add r0, #0x94
	ldr r0, [r0]
	add r4, #0x94
	add r0, r0, #1
	str r0, [r4]
	b _02248678
_0224862C:
	cmp r0, #2
	bne _02248678
	add r0, r4, #0
	add r0, #0x88
	ldr r0, [r0]
	bl YesNoPrompt_Destroy
	add r0, r4, #0
	mov r1, #0x22
	mov r2, #0
	bl WiiMessageSettingsScreen_SetState
	b _02248678
_02248646:
	ldr r0, [r4]
	ldr r0, [r0, #4]
	bl sub_0203A05C
	cmp r0, #0
	bne _0224866E
	ldr r0, [r4]
	mov r1, #5
	mov r2, #0
	bl WiiMessageSettings_SetNextApp
	ldr r0, [r4]
	mov r1, #0xc
	bl WiiMessageSettings_SetScreenStartState
	mov r0, #0x1d
	str r0, [r4, #8]
	add sp, #4
	mov r0, #0
	pop {r3, r4, pc}
_0224866E:
	add r0, r4, #0
	mov r1, #0x22
	mov r2, #0xc
	bl WiiMessageSettingsScreen_SetState
_02248678:
	mov r0, #0
	add sp, #4
	pop {r3, r4, pc}
	nop
_02248680: .word 0x00000F0F
	thumb_func_end WiiMessageSettingsScreen_ConfirmConnect


	thumb_func_start ov75_02248684
ov75_02248684: ; 0x02248684
	push {r3, r4, lr}
	sub sp, #4
	add r4, r0, #0
	add r0, #0x88
	ldr r0, [r0]
	bl YesNoPrompt_HandleInput
	cmp r0, #1
	bne _022486C2
	add r0, r4, #0
	add r0, #0x88
	ldr r0, [r0]
	bl YesNoPrompt_Destroy
	ldr r0, _022486E8 ; =0x00000F0F
	mov r2, #1
	str r0, [sp]
	ldr r1, [r4, #0x2c]
	add r0, r4, #0
	add r3, r2, #0
	bl WiiMessageSettingsScreen_PrintMessage
	add r0, r4, #0
	mov r1, #0x22
	mov r2, #0xc
	bl WiiMessageSettingsScreen_SetState
	add r0, r4, #0
	bl WiiMessageSettingsScreen_ShowWaitingIcon
	b _022486E2
_022486C2:
	cmp r0, #2
	bne _022486E2
	add r0, r4, #0
	add r0, #0x88
	ldr r0, [r0]
	bl YesNoPrompt_Destroy
	bl sub_0203957C
	ldr r0, [r4]
	mov r1, #6
	mov r2, #0
	bl WiiMessageSettings_SetNextApp
	mov r0, #0x1d
	str r0, [r4, #8]
_022486E2:
	mov r0, #0
	add sp, #4
	pop {r3, r4, pc}
	.balign 4, 0
_022486E8: .word 0x00000F0F
	thumb_func_end ov75_02248684


	thumb_func_start WiiMessageSettingsScreen_PrintDisconnecting
WiiMessageSettingsScreen_PrintDisconnecting: ; 0x022486EC
	push {r3, r4, lr}
	sub sp, #4
	ldr r1, _02248710 ; =0x00000F0F
	add r4, r0, #0
	str r1, [sp]
	ldr r1, [r4, #0x30]
	mov r2, #0x1a
	mov r3, #1
	bl WiiMessageSettingsScreen_PrintMessage
	add r0, r4, #0
	mov r1, #0x22
	mov r2, #0xa
	bl WiiMessageSettingsScreen_SetState
	mov r0, #0
	add sp, #4
	pop {r3, r4, pc}
	.balign 4, 0
_02248710: .word 0x00000F0F
	thumb_func_end WiiMessageSettingsScreen_PrintDisconnecting
