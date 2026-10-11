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

	thumb_func_start SafariAreaCustomizer_CreateMenuItems
SafariAreaCustomizer_CreateMenuItems: ; 0x021E7224
	push {r4, lr}
	add r4, r0, #0
	ldr r1, [r4]
	mov r0, #3
	bl ListMenuItems_New
	ldr r1, _021E729C ; =0x000004B4
	str r0, [r4, r1]
	ldr r1, [r4]
	mov r0, #2
	bl ListMenuItems_New
	ldr r1, _021E72A0 ; =0x000004B8
	mov r2, #7
	str r0, [r4, r1]
	sub r0, r1, #4
	mov r1, #0xc1
	lsl r1, r1, #2
	ldr r0, [r4, r0]
	ldr r1, [r4, r1]
	mov r3, #0
	bl ListMenuItems_AppendFromMsgData
	ldr r0, _021E729C ; =0x000004B4
	mov r1, #0xc1
	lsl r1, r1, #2
	ldr r0, [r4, r0]
	ldr r1, [r4, r1]
	mov r2, #8
	mov r3, #1
	bl ListMenuItems_AppendFromMsgData
	ldr r0, _021E729C ; =0x000004B4
	mov r1, #0xc1
	lsl r1, r1, #2
	ldr r0, [r4, r0]
	ldr r1, [r4, r1]
	mov r2, #6
	mov r3, #2
	bl ListMenuItems_AppendFromMsgData
	ldr r0, _021E72A0 ; =0x000004B8
	mov r1, #0xc1
	lsl r1, r1, #2
	ldr r0, [r4, r0]
	ldr r1, [r4, r1]
	mov r2, #5
	mov r3, #0
	bl ListMenuItems_AppendFromMsgData
	ldr r0, _021E72A0 ; =0x000004B8
	mov r1, #0xc1
	lsl r1, r1, #2
	ldr r0, [r4, r0]
	ldr r1, [r4, r1]
	mov r2, #6
	mov r3, #1
	bl ListMenuItems_AppendFromMsgData
	pop {r4, pc}
	.balign 4, 0
_021E729C: .word 0x000004B4
_021E72A0: .word 0x000004B8
	thumb_func_end SafariAreaCustomizer_CreateMenuItems
