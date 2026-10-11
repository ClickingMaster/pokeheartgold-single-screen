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

	thumb_func_start MailboxApp_DrawNameplate
MailboxApp_DrawNameplate: ; 0x021EE468
	push {r4, r5, r6, r7, lr}
	sub sp, #0x24
	add r5, r0, #0
	str r1, [sp, #0x18]
	ldr r1, [r5, #0xc]
	lsl r0, r3, #2
	add r6, r1, #0
	add r1, r1, r0
	mov r0, #0x9f
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	add r6, #0x48
	lsl r7, r2, #4
	str r0, [sp, #0x20]
	bl Mail_GetAuthorNamePtr
	add r1, r0, #0
	mov r0, #0x23
	ldr r2, [r5, #0xc]
	lsl r0, r0, #4
	ldr r0, [r2, r0]
	bl CopyU16ArrayToString
	mov r4, #0
	str r4, [sp, #0x1c]
_021EE49A:
	mov r0, #8
	str r0, [sp]
	str r0, [sp, #4]
	lsl r0, r4, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	mov r0, #8
	str r0, [sp, #0x10]
	str r0, [sp, #0x14]
	ldr r1, [r5, #0xc]
	mov r2, #0
	add r0, r6, r7
	add r1, #8
	add r3, r2, #0
	bl BlitBitmapRectToWindow
	mov r0, #8
	str r0, [sp]
	str r0, [sp, #4]
	lsl r0, r4, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #8]
	mov r0, #8
	str r0, [sp, #0xc]
	str r0, [sp, #0x10]
	str r0, [sp, #0x14]
	ldr r1, [r5, #0xc]
	mov r2, #0
	add r0, r6, r7
	add r1, #0x28
	add r3, r2, #0
	bl BlitBitmapRectToWindow
	ldr r0, [sp, #0x1c]
	add r4, #8
	add r0, r0, #1
	str r0, [sp, #0x1c]
	cmp r0, #8
	blo _021EE49A
	ldr r0, [sp, #0x20]
	bl Mail_GetAuthorGender
	cmp r0, #0
	bne _021EE514
	mov r0, #4
	str r0, [sp]
	ldr r0, _021EE548 ; =0x00030200
	mov r2, #0
	str r0, [sp, #4]
	str r2, [sp, #8]
	mov r1, #0x23
	ldr r3, [r5, #0xc]
	lsl r1, r1, #4
	ldr r1, [r3, r1]
	add r0, r6, r7
	add r3, r2, #0
	bl MailboxApp_PrintString
	b _021EE530
_021EE514:
	mov r0, #4
	str r0, [sp]
	ldr r0, _021EE54C ; =0x00050400
	mov r2, #0
	str r0, [sp, #4]
	str r2, [sp, #8]
	mov r1, #0x23
	ldr r3, [r5, #0xc]
	lsl r1, r1, #4
	ldr r1, [r3, r1]
	add r0, r6, r7
	add r3, r2, #0
	bl MailboxApp_PrintString
_021EE530:
	add r0, r6, r7
	bl CopyWindowPixelsToVram_TextMode
	ldr r0, [r5, #0xc]
	ldr r1, [sp, #0x18]
	ldr r0, [r0, #4]
	add r2, r6, r7
	bl BgCellManager_PutWindowTiles
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
	nop
_021EE548: .word 0x00030200
_021EE54C: .word 0x00050400
	thumb_func_end MailboxApp_DrawNameplate


	thumb_func_start MailboxApp_PrintTitleAndButtons
MailboxApp_PrintTitleAndButtons: ; 0x021EE550
	push {r3, r4, lr}
	sub sp, #0x14
	add r4, r0, #0
	mov r0, #0x76
	ldr r1, [r4, #0xc]
	lsl r0, r0, #2
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, [r4, #0xc]
	mov r1, #0x2c
	str r1, [sp]
	mov r3, #0
	str r3, [sp, #4]
	ldr r1, _021EE604 ; =0x00010200
	str r3, [sp, #8]
	str r1, [sp, #0xc]
	mov r1, #2
	str r1, [sp, #0x10]
	mov r1, #0x8a
	lsl r1, r1, #2
	ldr r1, [r0, r1]
	mov r2, #0x19
	bl MailboxAppData_PrintMsg
	mov r0, #0x76
	ldr r1, [r4, #0xc]
	lsl r0, r0, #2
	add r0, r1, r0
	bl ScheduleWindowCopyToVram
	mov r0, #0x66
	ldr r1, [r4, #0xc]
	lsl r0, r0, #2
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, [r4, #0xc]
	mov r1, #0x18
	str r1, [sp]
	mov r1, #0
	str r1, [sp, #4]
	mov r1, #4
	str r1, [sp, #8]
	ldr r1, _021EE608 ; =0x000F0100
	mov r2, #0x15
	str r1, [sp, #0xc]
	mov r1, #2
	str r1, [sp, #0x10]
	mov r1, #0x8a
	lsl r1, r1, #2
	ldr r1, [r0, r1]
	mov r3, #3
	bl MailboxAppData_PrintMsg
	mov r0, #0x66
	ldr r1, [r4, #0xc]
	lsl r0, r0, #2
	add r0, r1, r0
	bl ScheduleWindowCopyToVram
	mov r0, #0x6a
	ldr r1, [r4, #0xc]
	lsl r0, r0, #2
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, [r4, #0xc]
	mov r1, #0x18
	str r1, [sp]
	mov r1, #0
	str r1, [sp, #4]
	mov r3, #4
	ldr r1, _021EE608 ; =0x000F0100
	str r3, [sp, #8]
	str r1, [sp, #0xc]
	mov r1, #2
	str r1, [sp, #0x10]
	mov r1, #0x8a
	lsl r1, r1, #2
	ldr r1, [r0, r1]
	mov r2, #0x16
	bl MailboxAppData_PrintMsg
	add sp, #0x14
	pop {r3, r4, pc}
	nop
_021EE604: .word 0x00010200
_021EE608: .word 0x000F0100
	thumb_func_end MailboxApp_PrintTitleAndButtons
