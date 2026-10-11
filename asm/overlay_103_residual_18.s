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

	thumb_func_start MailboxAppData_LoadMonIconResources
MailboxAppData_LoadMonIconResources: ; 0x021EE160
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	ldr r4, _021EE208 ; =0x000037F0
	add r5, r0, #0
	mov r6, #0
	mov r7, #2
_021EE16C:
	mov r0, #0
	add r1, r0, #0
	add r2, r0, #0
	bl GetMonIconNaixEx
	str r6, [sp]
	add r3, r0, #0
	str r7, [sp, #4]
	mov r0, #0x25
	mov r1, #0x95
	mov r2, #0x9d
	str r4, [sp, #8]
	lsl r0, r0, #4
	lsl r1, r1, #2
	lsl r2, r2, #2
	ldr r0, [r5, r0]
	ldr r1, [r5, r1]
	ldr r2, [r5, r2]
	bl SpriteSystem_LoadCharResObjFromOpenNarc
	ldr r0, _021EE20C ; =0x000037F2
	add r4, r4, #1
	cmp r4, r0
	bls _021EE16C
	bl sub_02074490
	add r3, r0, #0
	mov r0, #0
	str r0, [sp]
	mov r0, #3
	str r0, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	ldr r0, _021EE208 ; =0x000037F0
	mov r2, #0x25
	lsl r2, r2, #4
	str r0, [sp, #0xc]
	add r1, r2, #4
	ldr r0, [r5, r2]
	add r2, #0x24
	ldr r1, [r5, r1]
	ldr r2, [r5, r2]
	bl SpriteSystem_LoadPlttResObjFromOpenNarc
	bl sub_02074494
	add r3, r0, #0
	mov r0, #0
	str r0, [sp]
	ldr r0, _021EE208 ; =0x000037F0
	mov r2, #0x25
	lsl r2, r2, #4
	str r0, [sp, #4]
	add r1, r2, #4
	ldr r0, [r5, r2]
	add r2, #0x24
	ldr r1, [r5, r1]
	ldr r2, [r5, r2]
	bl SpriteSystem_LoadCellResObjFromOpenNarc
	bl sub_02074498
	add r3, r0, #0
	mov r0, #0
	str r0, [sp]
	ldr r0, _021EE208 ; =0x000037F0
	mov r2, #0x25
	lsl r2, r2, #4
	str r0, [sp, #4]
	add r1, r2, #4
	ldr r0, [r5, r2]
	add r2, #0x24
	ldr r1, [r5, r1]
	ldr r2, [r5, r2]
	bl SpriteSystem_LoadAnimResObjFromOpenNarc
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_021EE208: .word 0x000037F0
_021EE20C: .word 0x000037F2
	thumb_func_end MailboxAppData_LoadMonIconResources


	thumb_func_start MailboxApp_ShowMailMonIcons
MailboxApp_ShowMailMonIcons: ; 0x021EE210
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	add r5, r0, #0
	ldrb r0, [r5, #0x1f]
	mov r4, #0
	ldr r1, [r5, #0xc]
	lsl r0, r0, #2
	add r1, r1, r0
	mov r0, #0x9f
	lsl r0, r0, #2
	ldr r7, [r1, r0]
	add r6, r4, #0
_021EE228:
	add r0, r7, #0
	bl sub_0202B4E4
	lsl r1, r4, #0x18
	add r3, r0, #0
	add r0, r7, #0
	lsr r1, r1, #0x18
	mov r2, #2
	bl sub_0202B404
	add r1, sp, #0x10
	strh r0, [r1]
	add r0, r1, #0
	ldrh r1, [r0]
	ldr r0, _021EE2DC ; =0x0000FFFF
	cmp r1, r0
	beq _021EE254
	lsl r0, r1, #0x14
	lsr r0, r0, #0x14
	str r0, [sp, #4]
	cmp r0, #7
	bne _021EE260
_021EE254:
	ldr r0, [r5, #0xc]
	add r1, r4, #4
	mov r2, #0
	bl MailboxAppData_SetSpriteDrawFlag
	b _021EE2CE
_021EE260:
	ldr r0, [r5, #0xc]
	add r1, r0, r6
	mov r0, #0x9a
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	ldr r0, [r0]
	bl Sprite_GetImageProxy
	mov r1, #2
	bl NNS_G2dGetImageLocation
	str r0, [sp, #8]
	mov r0, #0x9d
	str r0, [sp]
	ldr r1, [r5, #0xc]
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	ldr r1, [sp, #4]
	mov r2, #0
	add r3, sp, #0x14
	bl GfGfxLoader_GetCharDataFromOpenNarc
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x14]
	mov r1, #2
	ldr r0, [r0, #0x14]
	lsl r1, r1, #8
	bl DC_FlushRange
	ldr r0, [sp, #0x14]
	mov r2, #2
	ldr r0, [r0, #0x14]
	ldr r1, [sp, #8]
	lsl r2, r2, #8
	bl GXS_LoadOBJ
	ldr r0, [sp, #0xc]
	bl Heap_Free
	ldr r0, [r5, #0xc]
	add r1, r0, r6
	mov r0, #0x9a
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	add r1, sp, #0x10
	ldrh r1, [r1]
	lsl r1, r1, #0x10
	lsr r1, r1, #0x1c
	bl ManagedSprite_SetPaletteOverride
	ldr r0, [r5, #0xc]
	add r1, r4, #4
	mov r2, #1
	bl MailboxAppData_SetSpriteDrawFlag
_021EE2CE:
	add r4, r4, #1
	add r6, r6, #4
	cmp r4, #3
	blo _021EE228
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	nop
_021EE2DC: .word 0x0000FFFF
	thumb_func_end MailboxApp_ShowMailMonIcons


	thumb_func_start MailboxAppData_LoadSpriteResources
MailboxAppData_LoadSpriteResources: ; 0x021EE2E0
	push {r3, r4, r5, lr}
	sub sp, #0x10
	add r5, r0, #0
	mov r0, #0xfc
	mov r1, #0x9d
	bl NARC_New
	add r4, r0, #0
	mov r0, #1
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _021EE36C ; =0x000037F3
	mov r1, #0x25
	lsl r1, r1, #4
	str r0, [sp, #8]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	add r2, r4, #0
	mov r3, #7
	bl SpriteSystem_LoadCharResObjFromOpenNarc
	mov r0, #0
	str r0, [sp]
	mov r0, #2
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	ldr r0, _021EE370 ; =0x000037F1
	mov r1, #0x25
	lsl r1, r1, #4
	str r0, [sp, #0xc]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	add r2, r4, #0
	mov r3, #0xa
	bl SpriteSystem_LoadPlttResObjFromOpenNarc
	mov r0, #1
	str r0, [sp]
	ldr r0, _021EE370 ; =0x000037F1
	mov r1, #0x25
	lsl r1, r1, #4
	str r0, [sp, #4]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	add r2, r4, #0
	mov r3, #8
	bl SpriteSystem_LoadCellResObjFromOpenNarc
	mov r0, #1
	str r0, [sp]
	ldr r0, _021EE370 ; =0x000037F1
	mov r1, #0x25
	lsl r1, r1, #4
	str r0, [sp, #4]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	add r2, r4, #0
	mov r3, #9
	bl SpriteSystem_LoadAnimResObjFromOpenNarc
	add r0, r4, #0
	bl NARC_Delete
	add sp, #0x10
	pop {r3, r4, r5, pc}
	.balign 4, 0
_021EE36C: .word 0x000037F3
_021EE370: .word 0x000037F1
	thumb_func_end MailboxAppData_LoadSpriteResources
