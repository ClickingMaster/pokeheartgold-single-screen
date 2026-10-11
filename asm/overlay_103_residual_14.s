	.include "asm/macros.inc"
	.include "overlay_103.inc"
	.include "global.inc"

	.text
	.public _021EEB34
	.public MailboxApp_VBlankCB
	.public MailboxApp_InitBgLayers
	.public MailboxApp_LoadBgGraphics
	.public MailboxApp_CreateNameplateCells
	.public MailboxApp_OpenMailMenu
	.public MailboxApp_LoadMail
	.public MailboxApp_DrawPage
	.public MailboxApp_RunPaletteFlash
	.public MailboxApp_State_YesNo
	.public MailboxApp_State_SelectMail
	.public MailboxApp_State_MailMenu
	.public ov103_021ED9AC
	.public MailboxApp_StartCellFlash
	.public MailboxApp_StartUseMail
	.public MailboxApp_FinishUseMail
	.public MailboxApp_PrintMailToBagResult
	.public MailboxApp_CreateSprites
	.public MailboxAppData_InitSpriteSystem
	.public MailboxAppData_DeleteSprite
	.public MailboxAppData_LoadMonIconResources
	.public MailboxApp_ShowMailMonIcons
	.public MailboxAppData_LoadSpriteResources
	.public MailboxApp_DrawNameplate
	.public MailboxApp_PrintTitleAndButtons
	.public MailboxApp_PrintPageNumber
	.public MailboxApp_PrintMessageBox
	.public MailboxApp_PrintMessage
	.public MailboxApp_PrintMail
	.public MailboxApp_PrintMailLine
	.public ov103_021EEB40
	.public ov103_021EEB50
	.public ov103_021EEB6C
	.public ov103_021EEB88
	.public ov103_021EEBA4
	.public ov103_021EEBC0
	.public ov103_021EEBDC
	.public ov103_021EEBF8
	.public ov103_021EEC14
	.public ov103_021EEC30
	.public ov103_021EEC58
	.public ov103_021EEC68
	.public ov103_021EEC78
	.public ov103_021EECA8
	.public ov103_021EED0C
	.public ov103_021EED20
	.public ov103_021EED38
	.public ov103_021EED58
	.public ov103_021EEEC4
	.public ov103_021EEFB4
	.public ov103_021EEFBC
	.public ov103_021EEFC8
	.public ov103_021EEFD8
	.public ov103_021EF008
	.public MailboxApp_Init
	.public MailboxApp_Main
	.public MailboxApp_Exit
	.public MailboxApp_SetGfxBanks
	.public MailboxApp_FreeBgLayers
	.public MailboxApp_LoadFrameAndFontGfx
	.public MailboxApp_DeleteNameplateCells
	.public MailboxAppData_CreateStrings
	.public MailboxAppData_DeleteStrings
	.public MailboxApp_CreateYesNoPrompt
	.public MailboxApp_DestroyYesNoPrompt
	.public MailboxApp_ShowYesNoPrompt
	.public MailboxApp_CreateMailMenuItems
	.public MailboxApp_DeleteMailMenuItems
	.public MailboxApp_CloseMailMenu
	.public MailboxApp_FreeMail
	.public MailboxApp_DrawSelectedNameplate
	.public MailboxApp_LoadStationeryGfx
	.public MailboxApp_GetMailMenuItemCount
	.public MailboxApp_RunState
	.public MailboxApp_State_Setup
	.public MailboxApp_State_Teardown
	.public MailboxApp_State_WaitFade
	.public MailboxApp_State_WaitText
	.public MailboxApp_State_PaletteFlash
	.public MailboxApp_State_StartSubApp
	.public MailboxApp_State_RunSubApp
	.public MailboxApp_State_FadeIn
	.public MailboxApp_State_Quit
	.public MailboxApp_State_ChangePage
	.public MailboxApp_State_ShowMailPrompt
	.public MailboxApp_State_OpenMailMenu
	.public MailboxApp_State_ReadMail
	.public MailboxApp_State_CloseMail
	.public MailboxApp_State_ConfirmErase
	.public MailboxApp_State_PrintHoldMailPrompt
	.public MailboxApp_State_ConfirmHoldMail
	.public ov103_021ED960
	.public ov103_021ED97C
	.public MailboxApp_State_ShowMailToBagResult
	.public MailboxApp_ReturnToMailList
	.public MailboxApp_StartFadeIn
	.public MailboxApp_StartFadeOut
	.public MailboxApp_WaitForText
	.public MailboxApp_AskYesNo
	.public MailboxApp_StartSpriteFlash
	.public MailboxAppData_SetListDimmed
	.public MailboxApp_ShowMail
	.public MailboxApp_HideMail
	.public MailboxApp_PutMailInBag
	.public MailboxApp_DeleteSelectedMail
	.public MailboxApp_CreatePartyMenuArgs
	.public MailboxApp_StartPartyMenu
	.public MailboxApp_FinishGiveMail
	.public MailboxApp_FinishSelectMonForMail
	.public MailboxApp_FinishPartyMenu
	.public MailboxApp_EraseMail
	.public MailboxApp_SelectMonToHoldMail
	.public MailboxApp_DeleteSprites
	.public MailboxAppData_TickSprites
	.public MailboxAppData_FreeSpriteSystem
	.public MailboxAppData_NewSprite
	.public MailboxAppData_DeleteAllSprites
	.public MailboxAppData_SetSpriteAnim
	.public MailboxAppData_SetSpriteDrawFlag
	.public MailboxAppData_SetSpritePosition
	.public MailboxAppData_SetSpriteTranslucent
	.public MailboxAppData_OpenMonIconNarc
	.public MailboxAppData_CloseMonIconNarc
	.public MailboxAppData_MoveCursorToCloseButton
	.public MailboxApp_AddWindows
	.public MailboxApp_RemoveWindows
	.public MailboxApp_PrintString
	.public MailboxAppData_PrintMsg
	.public MailboxAppData_ShowCloseButton
	.public MailboxAppData_ShowCancelButton
	.public MailboxApp_BufferAuthorName
	.public MailboxApp_PrintMailPrompt
	.public MailboxApp_DrawMessageBox
	.public MailboxApp_ClearMessageBox
	.public MailboxApp_GetTouchedPageArrow
	.public MailboxApp_CreateGridInput
	.public MailboxApp_FreeGridInput
	.public MailboxApp_GridOnButton
	.public MailboxApp_GridOnSwitchToTouchMode
	.public MailboxApp_GridOnKeyMove
	.public MailboxApp_GridOnTouch
	.public MailboxApp_MoveCursor
	.public MailboxApp_IsCloseMailInput

	thumb_func_start MailboxApp_StartUseMail
MailboxApp_StartUseMail: ; 0x021EDD54
	push {r3, r4, lr}
	sub sp, #4
	add r4, r0, #0
	ldrh r0, [r4, #0x22]
	bl ItemToMailId
	add r3, r0, #0
	mov r0, #0x9c
	add r2, r4, #0
	str r0, [sp]
	add r2, #0x21
	ldrb r2, [r2]
	ldr r0, [r4, #8]
	mov r1, #2
	lsl r2, r2, #0x19
	ldr r0, [r0]
	lsr r2, r2, #0x19
	bl sub_02090E68
	ldr r1, [r4, #8]
	mov r2, #0x9c
	ldr r1, [r1, #8]
	str r1, [r0, #0x1c]
	str r0, [r4, #0x14]
	ldr r0, _021EDD94 ; =ov103_021EEC58
	ldr r1, [r4, #0x14]
	bl OverlayManager_New
	str r0, [r4, #0x10]
	mov r0, #0
	add sp, #4
	pop {r3, r4, pc}
	.balign 4, 0
_021EDD94: .word ov103_021EEC58
	thumb_func_end MailboxApp_StartUseMail


	thumb_func_start MailboxApp_FinishUseMail
MailboxApp_FinishUseMail: ; 0x021EDD98
	push {r4, lr}
	add r4, r0, #0
	ldr r0, [r4, #0x14]
	bl sub_02090F6C
	str r0, [r4, #0x24]
	cmp r0, #1
	bne _021EDDD8
	ldr r0, [r4, #8]
	ldr r0, [r0]
	bl SaveArray_Party_Get
	add r1, r4, #0
	add r1, #0x21
	ldrb r1, [r1]
	lsl r1, r1, #0x19
	lsr r1, r1, #0x19
	bl Party_GetMonByIndex
	add r1, r0, #0
	ldr r0, [r4, #0x14]
	bl sub_02090F70
	ldr r0, [r4, #8]
	ldr r0, [r0]
	bl Save_Bag_Get
	ldrh r1, [r4, #0x22]
	mov r2, #1
	mov r3, #0x9c
	bl Bag_TakeItem
_021EDDD8:
	ldr r0, [r4, #0x14]
	bl sub_02090F90
	mov r0, #0
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end MailboxApp_FinishUseMail
