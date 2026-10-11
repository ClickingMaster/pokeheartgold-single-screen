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

	thumb_func_start SafariAreaCustomizer_SwapAreas
SafariAreaCustomizer_SwapAreas: ; 0x021E6090
	push {r3, r4, r5, lr}
	ldr r2, _021E61D8 ; =0x000184E2
	add r5, r0, #0
	ldrb r1, [r5, r2]
	lsl r1, r1, #0x1d
	lsr r3, r1, #0x1f
	mov r1, #6
	add r4, r3, #0
	mul r4, r1
	ldr r1, [r5, #8]
	cmp r1, #0
	beq _021E60B2
	cmp r1, #1
	beq _021E60EE
	cmp r1, #2
	beq _021E6174
	b _021E61BA
_021E60B2:
	sub r0, r2, #3
	ldrb r0, [r5, r0]
	add r0, r0, r4
	lsl r0, r0, #2
	add r1, r5, r0
	mov r0, #0xdb
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	mov r1, #0
	bl ManagedSprite_SetPriority
	ldr r0, _021E61DC ; =0x000184E0
	ldrb r0, [r5, r0]
	add r0, r0, r4
	lsl r0, r0, #2
	add r1, r5, r0
	mov r0, #0xdb
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	mov r1, #0
	bl ManagedSprite_SetPriority
	mov r0, #0x10
	mov r1, #0
	bl GfGfx_EngineBTogglePlanes
	ldr r0, [r5, #8]
	add r0, r0, #1
	str r0, [r5, #8]
	b _021E61D2
_021E60EE:
	mov r1, #0
	bl SafariAreaCustomizer_RunSwapAnim
	cmp r0, #0
	beq _021E61D2
	ldr r2, _021E61E0 ; =0x000184E8
	mov r0, #1
	add r1, r2, #0
	str r0, [r5, r2]
	sub r1, #9
	sub r2, #8
	add r0, r5, #0
	ldrb r1, [r5, r1]
	ldrb r2, [r5, r2]
	add r0, #0x1c
	bl SafariZone_SwapAreasInSet
	ldr r0, _021E61E4 ; =0x000184DF
	mov r3, #0x7a
	ldrb r2, [r5, r0]
	mov r0, #0xd2
	lsl r0, r0, #2
	add r1, r2, r4
	mul r3, r2
	add r2, r5, r3
	lsl r1, r1, #0x18
	ldrb r2, [r2, #0x1c]
	ldr r0, [r5, r0]
	lsr r1, r1, #0x18
	bl SafariAreaCustomizerSpriteRes_SetAreaPicture
	ldr r0, _021E61E4 ; =0x000184DF
	mov r2, #0x7a
	ldrb r1, [r5, r0]
	add r0, r5, #0
	mul r2, r1
	add r2, r5, r2
	ldrb r2, [r2, #0x1c]
	bl SafariAreaCustomizer_LoadAreaPicture
	ldr r0, _021E61DC ; =0x000184E0
	mov r3, #0x7a
	ldrb r2, [r5, r0]
	mov r0, #0xd2
	lsl r0, r0, #2
	add r1, r2, r4
	mul r3, r2
	add r2, r5, r3
	lsl r1, r1, #0x18
	ldrb r2, [r2, #0x1c]
	ldr r0, [r5, r0]
	lsr r1, r1, #0x18
	bl SafariAreaCustomizerSpriteRes_SetAreaPicture
	ldr r0, _021E61DC ; =0x000184E0
	mov r2, #0x7a
	ldrb r1, [r5, r0]
	add r0, r5, #0
	mul r2, r1
	add r2, r5, r2
	ldrb r2, [r2, #0x1c]
	bl SafariAreaCustomizer_LoadAreaPicture
	ldr r0, [r5, #8]
	add r0, r0, #1
	str r0, [r5, #8]
	b _021E61D2
_021E6174:
	mov r1, #1
	bl SafariAreaCustomizer_RunSwapAnim
	cmp r0, #0
	beq _021E61D2
	ldr r0, _021E61E4 ; =0x000184DF
	ldrb r0, [r5, r0]
	add r0, r0, r4
	lsl r0, r0, #2
	add r1, r5, r0
	mov r0, #0xdb
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	mov r1, #2
	bl ManagedSprite_SetPriority
	ldr r0, _021E61DC ; =0x000184E0
	ldrb r0, [r5, r0]
	add r0, r0, r4
	lsl r0, r0, #2
	add r1, r5, r0
	mov r0, #0xdb
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	mov r1, #2
	bl ManagedSprite_SetPriority
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineBTogglePlanes
	ldr r0, [r5, #8]
	add r0, r0, #1
	str r0, [r5, #8]
	b _021E61D2
_021E61BA:
	sub r1, r2, #2
	ldrb r3, [r5, r1]
	sub r1, r2, #3
	strb r3, [r5, r1]
	bl SafariAreaCustomizer_EndMove
	mov r0, #0
	str r0, [r5, #8]
	mov r0, #2
	str r0, [r5, #0xc]
	mov r0, #4
	pop {r3, r4, r5, pc}
_021E61D2:
	mov r0, #3
	pop {r3, r4, r5, pc}
	nop
_021E61D8: .word 0x000184E2
_021E61DC: .word 0x000184E0
_021E61E0: .word 0x000184E8
_021E61E4: .word 0x000184DF
	thumb_func_end SafariAreaCustomizer_SwapAreas
