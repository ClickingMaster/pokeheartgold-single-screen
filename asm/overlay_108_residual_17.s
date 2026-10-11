	.include "asm/macros.inc"
	.include "overlay_108.inc"
	.include "global.inc"

	.text
	.public _021EA720
	.public SafariAreaCustomizer_Setup
	.public SafariAreaCustomizer_Teardown
	.public SafariAreaCustomizer_HandleSlotMenu
	.public SafariAreaCustomizer_ScrollPages
	.public SafariAreaCustomizer_HandleConfirmArea
	.public SafariAreaCustomizer_SwapAreas
	.public SafariAreaCustomizer_HandleSlotButtons
	.public SafariAreaCustomizer_HandleSlotTouch
	.public SafariAreaCustomizer_ChooseArea
	.public SafariAreaCustomizer_HandleAreaButtons
	.public SafariAreaCustomizer_HandleAreaTouch
	.public SafariAreaCustomizer_HandleMoveButtons
	.public SafariAreaCustomizer_HandleMoveTouch
	.public SafariAreaCustomizer_OpenSlotMenu
	.public SafariAreaCustomizer_AskConfirmArea
	.public SafariAreaCustomizer_VBlankCB
	.public SafariAreaCustomizer_InitBgLayers
	.public SafariAreaCustomizer_FreeBgLayers
	.public SafariAreaCustomizer_LoadBgGraphics
	.public SafariAreaCustomizer_CreateStrings
	.public SafariAreaCustomizer_DeleteStrings
	.public SafariAreaCustomizer_AddWindows
	.public SafariAreaCustomizer_CreateMenuItems
	.public SafariAreaCustomizer_CreateSprites
	.public SafariAreaCustomizer_CreateCancelTextObj
	.public ov108_021E767C
	.public SafariAreaCustomizer_PrintWindowText
	.public SafariAreaCustomizer_PrintPageNumber
	.public SafariAreaCustomizer_MoveCursor
	.public SafariAreaCustomizer_DrawSelectFrame
	.public SafariAreaCustomizer_InitSlotTilemaps
	.public SafariAreaCustomizer_LoadAreaPictureFromNarc
	.public SafariAreaCustomizer_LoadAreaPicture
	.public SafariAreaCustomizer_PrintObjectTypeLabels
	.public SafariAreaCustomizer_PrintSlotInfo
	.public SafariAreaCustomizer_PrintAreaNames
	.public SafariAreaCustomizer_StartPageScroll
	.public SafariAreaCustomizer_RunPageScroll
	.public SafariAreaCustomizer_StartPageChange
	.public SafariAreaCustomizer_RunPageChange
	.public SafariAreaCustomizer_RunSwapAnim
	.public SafariAreaCustomizerSpriteRes_SetAreaPicture
	.public SafariAreaCustomizerSpriteRes_CreateAreaSprite
	.public SafariAreaCustomizerSpriteRes_Init
	.public SafariAreaCustomizerSpriteRes_LoadResources
	.public ov108_021EA724
	.public ov108_021EA738
	.public ov108_021EA748
	.public ov108_021EA74A
	.public ov108_021EA760
	.public ov108_021EA780
	.public ov108_021EA7A8
	.public ov108_021EA7D0
	.public ov108_021EA7F8
	.public ov108_021EA820
	.public ov108_021EA848
	.public ov108_021EA870
	.public ov108_021EA898
	.public ov108_021EA978
	.public ov108_021EA97C
	.public ov108_021EA98C
	.public ov108_021EA9A0

	.public SafariAreaCustomizer_Exit
	.public SafariAreaCustomizer_Init
	.public SafariAreaCustomizer_Main
	.public SafariAreaCustomizer_InitFromArgs
	.public SafariAreaCustomizer_SaveResults
	.public SafariAreaCustomizer_State_Setup
	.public SafariAreaCustomizer_State_Teardown
	.public SafariAreaCustomizer_State_SelectSlot
	.public SafariAreaCustomizer_State_SelectArea
	.public SafariAreaCustomizer_State_MoveArea
	.public SafariAreaCustomizer_State_FadeIn
	.public SafariAreaCustomizer_State_FadeOut
	.public SafariAreaCustomizer_HandleSlotInput
	.public SafariAreaCustomizer_HandleAreaInput
	.public SafariAreaCustomizer_ChangePage
	.public SafariAreaCustomizer_HandleMoveInput
	.public SafariAreaCustomizer_ShowCursors
	.public ov108_021E6238
	.public SafariAreaCustomizer_CheckInputModeSwitch
	.public SafariAreaCustomizer_StartMove
	.public SafariAreaCustomizer_EndMove
	.public SafariAreaCustomizer_SetGfxBanks
	.public SafariAreaCustomizer_FreeScreenData
	.public SafariAreaCustomizer_RemoveWindows
	.public SafariAreaCustomizer_DeleteMenuItems
	.public SafariAreaCustomizer_InitSprites
	.public SafariAreaCustomizer_FreeSprites
	.public SafariAreaCustomizer_DeleteSprites
	.public SafariAreaCustomizer_DeleteCancelTextObj
	.public SafariAreaCustomizer_SetCursorAnim
	.public SafariAreaCustomizer_InitScreen
	.public SafariAreaCustomizer_InitSpriteSystem
	.public SafariAreaCustomizer_FreeSpriteSystem
	.public SafariAreaCustomizer_CreateSpriteManager
	.public SafariAreaCustomizer_DestroySpriteManager
	.public SafariAreaCustomizer_DrawSprites
	.public SafariAreaCustomizer_TransferOam
	.public SafariAreaCustomizerSpriteRes_New
	.public SafariAreaCustomizerSpriteRes_Delete
	.public SafariAreaCustomizerSpriteRes_DrawSprites
	.public SafariAreaCustomizerSpriteRes_SetSpriteList
	.public SafariAreaCustomizer_DeleteAreaSprite
	.public SafariAreaCustomizerSpriteRes_Free

	thumb_func_start SafariAreaCustomizerSpriteRes_Init
SafariAreaCustomizerSpriteRes_Init: ; 0x021E867C
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	ldr r1, _021E8714 ; =ov108_021EA978
	str r0, [sp]
	ldrb r2, [r1]
	add r0, sp, #8
	strb r2, [r0]
	ldrb r2, [r1, #1]
	strb r2, [r0, #1]
	ldrb r2, [r1, #2]
	ldrb r1, [r1, #3]
	strb r2, [r0, #2]
	strb r1, [r0, #3]
	ldr r0, [sp]
	ldr r2, [sp]
	ldr r1, [sp]
	ldrh r0, [r0, #0xc]
	ldr r2, [r2]
	add r1, #0x18
	bl G2dRenderer_Init
	ldr r1, [sp]
	mov r6, #0x55
	str r0, [r1, #0x10]
	mov r0, #0
	str r0, [sp, #4]
	add r7, sp, #8
	add r5, r1, #0
	add r4, r0, #0
	lsl r6, r6, #2
_021E86B8:
	ldr r2, [sp]
	ldrb r0, [r7]
	ldr r1, [sp, #4]
	ldr r2, [r2]
	bl Create2DGfxResObjMan
	mov r1, #0x51
	lsl r1, r1, #2
	str r0, [r5, r1]
	ldr r1, [sp]
	ldrb r0, [r7]
	ldr r1, [r1]
	bl Create2DGfxResObjList
	mov r1, #0x55
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r0, #0x55
	lsl r0, r0, #2
	ldr r3, [r5, r0]
	mov r1, #0
	ldr r0, [r3, #4]
	cmp r0, #0
	bls _021E86FA
	add r2, r1, #0
_021E86EA:
	ldr r0, [r3]
	add r1, r1, #1
	str r4, [r0, r2]
	ldr r3, [r5, r6]
	add r2, r2, #4
	ldr r0, [r3, #4]
	cmp r1, r0
	blo _021E86EA
_021E86FA:
	ldr r0, [sp, #4]
	add r7, r7, #1
	add r0, r0, #1
	add r5, r5, #4
	str r0, [sp, #4]
	cmp r0, #4
	blo _021E86B8
	ldr r0, [sp]
	bl SafariAreaCustomizerSpriteRes_LoadResources
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	nop
_021E8714: .word ov108_021EA978
	thumb_func_end SafariAreaCustomizerSpriteRes_Init
