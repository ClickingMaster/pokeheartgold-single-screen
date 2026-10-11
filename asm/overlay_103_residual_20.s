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

	thumb_func_start MailboxApp_PrintPageNumber
MailboxApp_PrintPageNumber: ; 0x021EE644
	push {r3, r4, r5, r6, lr}
	sub sp, #0xc
	add r5, r0, #0
	mov r0, #0x6e
	ldr r1, [r5, #0xc]
	lsl r0, r0, #2
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	mov r2, #0x8a
	ldr r3, [r5, #0xc]
	lsl r2, r2, #2
	ldr r0, [r3, r2]
	add r2, #8
	ldr r2, [r3, r2]
	mov r1, #2
	bl ReadMsgDataIntoString
	mov r1, #0x6e
	ldr r2, [r5, #0xc]
	mov r3, #0
	ldr r0, _021EE780 ; =0x00010200
	str r3, [sp]
	str r0, [sp, #4]
	mov r0, #2
	lsl r1, r1, #2
	str r0, [sp, #8]
	add r0, r2, r1
	add r1, #0x78
	ldr r1, [r2, r1]
	mov r2, #0x14
	bl MailboxApp_PrintString
	mov r1, #0x23
	ldr r2, [r5, #0xc]
	lsl r1, r1, #4
	mov r0, #0
	ldr r1, [r2, r1]
	add r2, r0, #0
	bl FontID_String_GetWidth
	mov r1, #0
	lsl r4, r0, #0x10
	mov r0, #0x8b
	str r1, [sp]
	mov r3, #1
	str r3, [sp, #4]
	ldr r2, [r5, #0xc]
	lsl r0, r0, #2
	ldr r0, [r2, r0]
	ldrh r2, [r5, #0x1c]
	add r2, r2, #1
	bl BufferIntegerAsString
	mov r0, #0x8a
	ldr r1, [r5, #0xc]
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	mov r1, #1
	bl NewString_ReadMsgData
	mov r1, #0x8b
	ldr r2, [r5, #0xc]
	lsl r1, r1, #2
	add r6, r0, #0
	ldr r0, [r2, r1]
	add r1, r1, #4
	ldr r1, [r2, r1]
	add r2, r6, #0
	bl StringExpandPlaceholders
	add r0, r6, #0
	bl String_Delete
	mov r1, #0x23
	ldr r2, [r5, #0xc]
	lsl r1, r1, #4
	mov r0, #0
	ldr r1, [r2, r1]
	add r2, r0, #0
	bl FontID_String_GetWidth
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	mov r6, #0x6e
	ldr r1, [r5, #0xc]
	mov r3, #0
	ldr r0, _021EE780 ; =0x00010200
	str r3, [sp]
	str r0, [sp, #4]
	lsl r6, r6, #2
	add r0, r1, r6
	str r3, [sp, #8]
	add r6, #0x78
	ldr r1, [r1, r6]
	lsr r4, r4, #0x11
	mov r6, #0x14
	sub r6, r6, r4
	sub r2, r6, r2
	bl MailboxApp_PrintString
	ldr r6, [r5, #0xc]
	mov r1, #0
	mov r2, #0x8b
	str r1, [sp]
	mov r3, #1
	str r3, [sp, #4]
	lsl r2, r2, #2
	ldr r0, [r6, r2]
	add r2, #0xb6
	ldrh r2, [r6, r2]
	add r2, r2, #1
	bl BufferIntegerAsString
	mov r0, #0x8a
	ldr r1, [r5, #0xc]
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	mov r1, #1
	bl NewString_ReadMsgData
	mov r1, #0x8b
	ldr r2, [r5, #0xc]
	lsl r1, r1, #2
	add r6, r0, #0
	ldr r0, [r2, r1]
	add r1, r1, #4
	ldr r1, [r2, r1]
	add r2, r6, #0
	bl StringExpandPlaceholders
	add r0, r6, #0
	bl String_Delete
	mov r1, #0x6e
	ldr r2, [r5, #0xc]
	mov r3, #0
	ldr r0, _021EE780 ; =0x00010200
	str r3, [sp]
	str r0, [sp, #4]
	lsl r1, r1, #2
	add r0, r2, r1
	str r3, [sp, #8]
	add r1, #0x78
	ldr r1, [r2, r1]
	add r4, #0x14
	add r2, r4, #0
	bl MailboxApp_PrintString
	mov r0, #0x6e
	ldr r1, [r5, #0xc]
	lsl r0, r0, #2
	add r0, r1, r0
	bl ScheduleWindowCopyToVram
	add sp, #0xc
	pop {r3, r4, r5, r6, pc}
	.balign 4, 0
_021EE780: .word 0x00010200
	thumb_func_end MailboxApp_PrintPageNumber
