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

	thumb_func_start WiiMessageSettings_Init
WiiMessageSettings_Init: ; 0x02246960
	push {r3, r4, r5, lr}
	mov r2, #0xa
	add r5, r0, #0
	mov r0, #3
	mov r1, #0x73
	lsl r2, r2, #0xe
	bl Heap_Create
	mov r2, #0x57
	mov r0, #0
	mov r1, #0x59
	lsl r2, r2, #4
	bl Heap_Create
	mov r1, #0x47
	add r0, r5, #0
	lsl r1, r1, #2
	mov r2, #0x73
	bl OverlayManager_CreateAndGetData
	mov r2, #0x47
	mov r1, #0
	lsl r2, r2, #2
	add r4, r0, #0
	bl MI_CpuFill8
	add r0, r5, #0
	bl OverlayManager_GetArgs
	ldr r0, [r0, #8]
	str r0, [r4, #4]
	bl Save_PlayerData_GetOptionsAddr
	str r0, [r4, #8]
	mov r0, #0x64
	mov r1, #0x73
	bl String_New
	mov r1, #0x11
	lsl r1, r1, #4
	str r0, [r4, r1]
	mov r0, #0x64
	mov r1, #0x73
	bl String_New
	mov r1, #0x45
	lsl r1, r1, #2
	str r0, [r4, r1]
	ldr r1, _022469D4 ; =0x0000047D
	mov r0, #0x11
	mov r2, #1
	bl Sound_SetSceneAndPlayBGM
	mov r0, #0
	add r4, #0x88
	str r0, [r4]
	mov r0, #1
	pop {r3, r4, r5, pc}
	.balign 4, 0
_022469D4: .word 0x0000047D
	thumb_func_end WiiMessageSettings_Init


	thumb_func_start WiiMessageSettings_Main
WiiMessageSettings_Main: ; 0x022469D8
	push {r3, r4, r5, lr}
	add r5, r1, #0
	bl OverlayManager_GetData
	add r4, r0, #0
	ldr r0, [r4, #0x7c]
	cmp r0, #1
	bne _022469FC
	bl ov00_021ECB40
	bl GtsServer_Update
	bl ov00_021EC9D4
	mov r1, #3
	sub r0, r1, r0
	bl WirelessStatusIcon_SetLevel
_022469FC:
	ldr r0, [r5]
	cmp r0, #4
	bhi _02246ADE
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02246A0E: ; jump table
	.short _02246A18 - _02246A0E - 2 ; case 0
	.short _02246A24 - _02246A0E - 2 ; case 1
	.short _02246A44 - _02246A0E - 2 ; case 2
	.short _02246A82 - _02246A0E - 2 ; case 3
	.short _02246ADA - _02246A0E - 2 ; case 4
_02246A18:
	add r0, r4, #0
	bl WiiMessageSettings_LoadNetworkOverlays
	mov r0, #1
	str r0, [r5]
	b _02246ADE
_02246A24:
	bl CommWireless_IsStartedUp
	cmp r0, #0
	beq _02246ADE
	ldr r1, [r4, #0x10]
	ldr r0, _02246AE4 ; =_02249BE0
	str r1, [r0]
	ldr r0, _02246AE8 ; =WiiMessageSettings_Alloc
	ldr r1, _02246AEC ; =WiiMessageSettings_Free
	bl ov00_021EC294
	mov r0, #1
	str r0, [r4, #0x7c]
	mov r0, #2
	str r0, [r5]
	b _02246ADE
_02246A44:
	add r1, r4, #0
	add r1, #0x88
	ldr r1, [r1]
	add r0, r4, #0
	lsl r2, r1, #4
	ldr r1, _02246AF0 ; =sWiiMessageSettingsApps
	ldr r1, [r1, r2]
	blx r1
	add r1, r0, #0
	add r0, r4, #0
	add r0, #0x88
	ldr r0, [r0]
	lsl r2, r0, #4
	ldr r0, _02246AF4 ; =sWiiMessageSettingsApps + 8
	ldr r0, [r0, r2]
	mov r2, #0x73
	bl OverlayManager_New
	str r0, [r4]
	add r0, r4, #0
	add r0, #0x88
	ldr r1, [r0]
	add r0, r4, #0
	add r0, #0x80
	str r1, [r0]
	mov r0, #6
	add r4, #0x88
	str r0, [r4]
	mov r0, #3
	str r0, [r5]
	b _02246ADE
_02246A82:
	ldr r0, [r4]
	bl OverlayManager_Run
	cmp r0, #1
	bne _02246ADE
	add r1, r4, #0
	add r1, #0x80
	ldr r1, [r1]
	add r0, r4, #0
	lsl r2, r1, #4
	ldr r1, _02246AF8 ; =sWiiMessageSettingsApps + 4
	ldr r1, [r1, r2]
	blx r1
	ldr r0, [r4]
	bl OverlayManager_Delete
	add r0, r4, #0
	add r0, #0x88
	ldr r0, [r0]
	cmp r0, #6
	bne _02246AB2
	mov r0, #4
	str r0, [r5]
	b _02246ADE
_02246AB2:
	lsl r1, r0, #4
	ldr r0, _02246AFC ; =sWiiMessageSettingsApps + 12
	ldr r0, [r0, r1]
	cmp r0, #1
	bne _02246AC8
	add r0, r4, #0
	bl WiiMessageSettings_UnloadNetworkOverlays
	mov r0, #2
	str r0, [r5]
	b _02246ADE
_02246AC8:
	ldr r0, [r4, #0x7c]
	cmp r0, #1
	bne _02246AD4
	mov r0, #2
	str r0, [r5]
	b _02246ADE
_02246AD4:
	mov r0, #0
	str r0, [r5]
	b _02246ADE
_02246ADA:
	mov r0, #1
	pop {r3, r4, r5, pc}
_02246ADE:
	mov r0, #0
	pop {r3, r4, r5, pc}
	nop
_02246AE4: .word _02249BE0
_02246AE8: .word WiiMessageSettings_Alloc
_02246AEC: .word WiiMessageSettings_Free
_02246AF0: .word sWiiMessageSettingsApps
_02246AF4: .word sWiiMessageSettingsApps + 8
_02246AF8: .word sWiiMessageSettingsApps + 4
_02246AFC: .word sWiiMessageSettingsApps + 12
	thumb_func_end WiiMessageSettings_Main


	thumb_func_start WiiMessageSettings_Exit
WiiMessageSettings_Exit: ; 0x02246B00
	push {r3, r4, r5, lr}
	add r5, r0, #0
	bl OverlayManager_GetData
	add r4, r0, #0
	bl WiiMessageSettings_UnloadNetworkOverlays
	mov r0, #0x45
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl String_Delete
	mov r0, #0x11
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	bl String_Delete
	add r0, r5, #0
	bl OverlayManager_FreeData
	mov r0, #0x73
	bl Heap_Destroy
	mov r0, #0x59
	bl Heap_Destroy
	ldr r0, _02246B40 ; =FS_OVERLAY_ID(intro_title)
	ldr r1, _02246B44 ; =gApplication_TitleScreen
	bl RegisterMainOverlay
	mov r0, #1
	pop {r3, r4, r5, pc}
	.balign 4, 0
_02246B40: .word FS_OVERLAY_ID(intro_title)
_02246B44: .word gApplication_TitleScreen
	thumb_func_end WiiMessageSettings_Exit


	thumb_func_start WiiMessageSettings_LoadNetworkOverlays
WiiMessageSettings_LoadNetworkOverlays: ; 0x02246B48
	push {r4, lr}
	add r4, r0, #0
	ldr r0, [r4, #0x7c]
	cmp r0, #0
	bne _02246B8E
	ldr r0, _02246B90 ; =FS_OVERLAY_ID(OVY_70)
	mov r1, #2
	bl HandleLoadOverlay
	bl LoadDwcOverlay
	bl LoadOVY38
	mov r0, #0x73
	bl sub_02039FD8
	ldr r1, _02246B94 ; =0x00020020
	mov r0, #0x73
	bl Heap_Alloc
	str r0, [r4, #0xc]
	add r0, #0x1f
	mov r1, #0x1f
	bic r0, r1
	mov r1, #2
	lsl r1, r1, #0x10
	mov r2, #0
	bl NNS_FndCreateExpHeapEx
	str r0, [r4, #0x10]
	bl CommWireless_StartUp
	mov r0, #4
	bl Sys_ClearSleepDisableFlag
_02246B8E:
	pop {r4, pc}
	.balign 4, 0
_02246B90: .word FS_OVERLAY_ID(OVY_70)
_02246B94: .word 0x00020020
	thumb_func_end WiiMessageSettings_LoadNetworkOverlays


	thumb_func_start WiiMessageSettings_UnloadNetworkOverlays
WiiMessageSettings_UnloadNetworkOverlays: ; 0x02246B98
	push {r4, lr}
	add r4, r0, #0
	ldr r0, [r4, #0x7c]
	cmp r0, #1
	bne _02246BC4
	ldr r0, [r4, #0x10]
	bl NNS_FndDestroyExpHeap
	ldr r0, [r4, #0xc]
	bl Heap_Free
	bl UnloadOVY38
	bl UnloadDwcOverlay
	bl CommWireless_Terminate
	ldr r0, _02246BC8 ; =FS_OVERLAY_ID(OVY_70)
	bl UnloadOverlayByID
	mov r0, #0
	str r0, [r4, #0x7c]
_02246BC4:
	pop {r4, pc}
	nop
_02246BC8: .word FS_OVERLAY_ID(OVY_70)
	thumb_func_end WiiMessageSettings_UnloadNetworkOverlays
