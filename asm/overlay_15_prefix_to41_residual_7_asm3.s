#include "constants/sndseq.h"
#include "constants/items.h"
#include "msgdata/msg/msg_0010.h"
	.include "asm/macros.inc"
	.include "overlay_15.inc"
	.include "global.inc"

	.text
	.public BagApp_PrintCursorDescription
	.public BagApp_State_HandleInput
	.public BagApp_State_OpenItemMenu
	.public BagApp_State_ChangePage
	.public BagApp_CalcPocketIconSpacing
	.public BagApp_FindPocketIndex
	.public BagApp_GetAdjacentPocket
	.public BagApp_PressButton
	.public BagApp_State_PocketSwitch
	.extern Bag_Exit
	.extern Bag_Init
	.extern Bag_Main
	.extern BagApp_LoadInfoPanelTilemap
	.extern BagApp_LoadPocketItemNames
	.extern BagApp_ClampPocketScroll
	.extern ov15_021FA070
	.extern BagApp_GetPageItemCount
	.extern BagApp_SetPocketPositionFromButton
	.extern BagApp_ShouldMoveCursorToButton
	.extern BagApp_GetTouchedButton
	.extern BagApp_GetTouchedPocket
	.extern BagApp_RedrawMovePage
	.extern BagApp_SetupItemMenu
	.extern BagApp_ItemAction_Use
	.extern ov15_021FBC6C
	.extern BagApp_ItemAction_Confirm
	.extern BagApp_ItemAction_Trash
	.extern BagApp_ItemAction_Register
	.extern BagApp_ItemAction_Deselect
	.extern BagApp_ItemAction_Move
	.extern BagApp_ItemAction_Give
	.extern BagApp_ItemAction_ChooseForBerryPatch
	.extern BagApp_IsBerryOrMulch
	.extern BagApp_LoadPocketSubBgPalette
	.extern BagApp_LoadSubScreenLayout
	.extern BagApp_SetMenuInputState
	.extern BagApp_StartButtonFlash
	.extern BagApp_StartButtonAnim
	.extern BagApp_StartCameraTween
	.extern BagApp_SetBagModelPocketAnim
	.extern BagApp_AddItemListWindows
	.extern BagApp_RemoveItemListWindows
	.extern BagApp_AddItemActionWindows
	.extern BagApp_RemoveItemActionWindows
	.extern BagApp_BufferSlotItemName
	.extern BagApp_BufferSlotItemNamePlural
	.extern BagApp_PrintItemDescription
	.extern BagApp_PrintTMHMMoveInfo

	.public Bag_Exit
	.public Bag_Init
	.public Bag_Main
	.public BagApp_LoadInfoPanelTilemap
	.public BagApp_ItemAction_Use
	.public ov15_021FBC6C
	.public BagApp_ItemAction_Confirm
	.public BagApp_ItemAction_Trash
	.public BagApp_ItemAction_Register
	.public BagApp_ItemAction_Deselect
	.public BagApp_ItemAction_Move
	.public BagApp_ItemAction_Give
	.public BagApp_ItemAction_ChooseForBerryPatch
	.public BagApp_IsBerryOrMulch
	.public BagApp_AddItemListWindows
	.public BagApp_RemoveItemListWindows
	.public BagApp_AddItemActionWindows
	.public BagApp_RemoveItemActionWindows
	.public BagApp_BufferSlotItemName
	.public BagApp_BufferSlotItemNamePlural
	.public BagApp_PrintItemDescription
	.public BagApp_PrintTMHMMoveInfo
	.extern sDefaultItemMenuChoices
	.extern sDebugSubScreenLayoutCount
	.extern sDebugSubScreenLayout
	.extern BagApp_LoadItemMenuStrings
	.extern BagApp_FreeItemMenuStrings
	.extern BagApp_PrintItemSelectedMessage
	.extern BagApp_ShowItemDescription
	.extern BagApp_ClearDescriptionWindow
	.extern BagApp_ShowPocketDescription
	.extern BagApp_CloseItemPromptWindow
	.extern BagApp_CloseItemMenuWindows
	.extern BagApp_CloseItemActionWindows
	.extern BagApp_PrintMoveItemPrompt
	.extern BagApp_PrintQuantityDigits
	.extern BagApp_PrintTrashConfirmMessage
	.extern BagApp_PrintMessage
	.extern BagApp_CreateYesNoPrompt
	.extern BagApp_DestroyYesNoPrompt
	.extern BagApp_PrintSellPrice
	.extern BagApp_PrintMoney
	.extern ov15_021FF1E0
	.extern BagApp_PrintCancelButton
	.extern BagApp_DrawItemList
	.extern BagApp_DrawSelectedItemEntry
	.extern BagApp_ClearSelectedItemEntry
	.extern BagApp_PrintPageNumber
	.extern BagApp_PrintItemMenuLabel
	.extern BagApp_ClearItemMenuLabels
	.extern BagApp_PrintTrashLabel
	.extern BagApp_PrintSellLabel
	.extern BagApp_ClearQuantityLabel
	.extern BagApp_ClearCancelButtonText
	.extern BagApp_InitSprites
	.extern BagApp_FreeSprites
	.extern BagApp_TickSprites
	.extern ov15_021FF950
	.extern ov15_021FF964
	.extern BagApp_SetCursorPosition
	.extern BagApp_HideCursor
	.extern BagApp_SetMoveCursorPosition
	.extern BagApp_SetQuantityCursorPosition
	.extern BagApp_LoadPocketObjPalette
	.extern BagApp_ShowMoveSlotSprites
	.extern BagApp_DrawItemIcons
	.extern BagApp_ShowMovingItemIcon
	.extern BagApp_ShowItemMenuSprites
	.extern BagApp_HideItemMenuSprites
	.extern BagApp_ShowSelectedItemIcon
	.extern BagApp_GetUnusedDigitCount
	.extern BagApp_ShowQuantityButtons
	.extern BagApp_HideQuantityButtons
	.extern BagApp_ShowPocketTabSprites
	.extern BagApp_SetCancelSpriteVisible
	.extern sBagModelScale
	.extern sCameraTarget
	.extern sBagModelPosition
	.extern sBagGraphicsModes
	.extern sItemMenuNavTable
	.extern sItemMenuNavTableDown
	.extern sItemMenuNavTableLeft
	.extern sItemMenuNavTableRight
	.extern sCameraSetup
	.extern sMoveNavTable
	.extern sMoveNavTableDown
	.extern sMoveNavTableLeft
	.extern sMoveNavTableRight
	.extern sBagModelRotation
	.extern sBagGraphicsBanks
	.extern sButtonNavTable
	.extern sButtonNavTableDown
	.extern sButtonNavTableLeft
	.extern sButtonNavTableRight
	.extern sBgTemplate_Main1
	.extern sBgTemplate_Main2
	.extern sBgTemplate_Main3
	.extern sBgTemplate_Sub0
	.extern sBgTemplate_Sub1
	.extern sBgTemplate_Sub2
	.extern sBgTemplate_Sub3
	.extern sCameraTweenKeys
	.extern sPocketCapacities
	.extern sQuantityDigitWindowPositions
	.extern sItemMenuWindowPositions
	.extern sItemListWindowTemplates
	.extern sBagEdgeColorTable
	.extern sTouchHitboxTables
	.extern sItemSlotRects
	.extern sUnusedSlotRects
	.extern sItemActionFuncs
	.extern sSubScreenTilemapCommands
	.extern sItemMenuCursorButtons

	thumb_func_start BagApp_PressButton
BagApp_PressButton: ; 0x021FA73C
	push {r3, r4, r5, r6, r7, lr}
	add r4, r0, #0
	add r7, r2, #0
	mov r6, #1
	cmp r1, #0x10
	bls _021FA74A
	b _021FA92A
_021FA74A:
	add r2, r1, r1
	add r2, pc
	ldrh r2, [r2, #6]
	lsl r2, r2, #0x10
	asr r2, r2, #0x10
	add pc, r2
_021FA756: ; jump table
	.short _021FA778 - _021FA756 - 2 ; case 0
	.short _021FA778 - _021FA756 - 2 ; case 1
	.short _021FA778 - _021FA756 - 2 ; case 2
	.short _021FA778 - _021FA756 - 2 ; case 3
	.short _021FA778 - _021FA756 - 2 ; case 4
	.short _021FA778 - _021FA756 - 2 ; case 5
	.short _021FA778 - _021FA756 - 2 ; case 6
	.short _021FA778 - _021FA756 - 2 ; case 7
	.short _021FA856 - _021FA756 - 2 ; case 8
	.short _021FA856 - _021FA756 - 2 ; case 9
	.short _021FA856 - _021FA756 - 2 ; case 10
	.short _021FA856 - _021FA756 - 2 ; case 11
	.short _021FA856 - _021FA756 - 2 ; case 12
	.short _021FA856 - _021FA756 - 2 ; case 13
	.short _021FA8C4 - _021FA756 - 2 ; case 14
	.short _021FA892 - _021FA756 - 2 ; case 15
	.short _021FA8F6 - _021FA756 - 2 ; case 16
_021FA778:
	bl BagApp_FindPocketIndex
	sub r1, r6, #2
	cmp r0, r1
	bne _021FA786
	add r0, r6, #0
	pop {r3, r4, r5, r6, r7, pc}
_021FA786:
	mov r1, #0x8d
	lsl r1, r1, #2
	ldr r2, [r4, r1]
	add r1, r2, #0
	add r1, #0x64
	ldrb r1, [r1]
	cmp r0, r1
	bne _021FA7A0
	ldr r1, [sp, #0x1c]
	cmp r1, #0
	bne _021FA7A0
	add r0, r6, #0
	pop {r3, r4, r5, r6, r7, pc}
_021FA7A0:
	add r2, #0x64
	strb r0, [r2]
	add r0, r4, #0
	bl BagApp_LoadPocketItemNames
	mov r0, #0x8d
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	add r2, r0, #4
	add r0, #0x64
	ldrb r1, [r0]
	mov r0, #0xc
	mul r0, r1
	add r5, r2, r0
	add r0, r4, #0
	bl BagApp_GetPageItemCount
	mov r1, #0
	add r2, r0, #0
	add r0, r4, #0
	add r3, r1, #0
	bl BagApp_LoadSubScreenLayout
	mov r2, #6
	ldrsh r1, [r5, r2]
	add r0, r4, #0
	sub r2, r2, #7
	mov r3, #0
	bl BagApp_DrawItemList
	mov r1, #0x8d
	lsl r1, r1, #2
	ldr r1, [r4, r1]
	add r0, r4, #0
	add r1, #0x64
	ldrb r1, [r1]
	bl BagApp_LoadPocketObjPalette
	mov r2, #6
	ldrb r1, [r5, #9]
	ldrsh r2, [r5, r2]
	add r0, r4, #0
	mov r3, #0
	bl BagApp_PrintPageNumber
	add r0, r4, #0
	bl BagApp_GetPageItemCount
	add r2, r0, #0
	add r0, r4, #0
	add r1, r5, #0
	mov r3, #1
	bl BagApp_DrawItemIcons
	mov r2, #0x8d
	lsl r2, r2, #2
	ldr r2, [r4, r2]
	add r0, r4, #0
	add r2, #0x64
	ldrb r2, [r2]
	mov r1, #1
	bl BagApp_LoadPocketSubBgPalette
	ldr r0, _021FA930 ; =0x000005DC
	bl PlaySE
	add r0, r4, #0
	bl BagApp_PrintCursorDescription
	ldr r0, _021FA934 ; =0x00000644
	ldr r1, [r4, r0]
	cmp r1, #8
	blt _021FA838
	add r0, r4, #0
	bl BagApp_SetPocketPositionFromButton
_021FA838:
	add r0, r4, #0
	bl BagApp_SetBagModelPocketAnim
	mov r1, #0x8d
	lsl r1, r1, #2
	ldr r1, [r4, r1]
	ldr r0, _021FA938 ; =0x00000808
	add r1, #0x64
	ldrb r1, [r1]
	add r0, r4, r0
	mov r2, #7
	add r1, r1, #1
	bl BagApp_StartCameraTween
	b _021FA92A
_021FA856:
	mov r0, #0x8d
	lsl r0, r0, #2
	ldr r2, [r4, r0]
	sub r1, #8
	add r3, r2, #0
	add r3, #0x64
	ldrb r5, [r3]
	mov r3, #0xc
	add r0, r2, #4
	mul r3, r5
	add r3, r0, r3
	mov r0, #6
	ldrsh r0, [r3, r0]
	add r0, r0, r1
	ldrb r1, [r3, #9]
	cmp r0, r1
	bge _021FA88A
	ldr r1, [r3]
	lsl r0, r0, #2
	ldrh r0, [r1, r0]
	add r2, #0x66
	strh r0, [r2]
	ldr r0, _021FA930 ; =0x000005DC
	strb r6, [r7]
	bl PlaySE
_021FA88A:
	add r0, r4, #0
	bl BagApp_PrintCursorDescription
	b _021FA92A
_021FA892:
	mov r0, #0x8d
	lsl r0, r0, #2
	ldr r2, [r4, r0]
	add r0, r2, #0
	add r0, #0x64
	ldrb r1, [r0]
	mov r0, #0xc
	mul r0, r1
	add r0, r2, r0
	ldrb r0, [r0, #0xd]
	cmp r0, #6
	bls _021FA92A
	ldr r0, _021FA930 ; =0x000005DC
	bl PlaySE
	mov r0, #0x1e
	str r0, [sp]
	add r0, r4, #0
	mov r1, #0x12
	mov r2, #9
	mov r3, #8
	bl BagApp_StartButtonFlash
	add r6, r0, #0
	b _021FA92A
_021FA8C4:
	mov r0, #0x8d
	lsl r0, r0, #2
	ldr r2, [r4, r0]
	add r0, r2, #0
	add r0, #0x64
	ldrb r1, [r0]
	mov r0, #0xc
	mul r0, r1
	add r0, r2, r0
	ldrb r0, [r0, #0xd]
	cmp r0, #6
	bls _021FA92A
	ldr r0, _021FA930 ; =0x000005DC
	bl PlaySE
	mov r0, #0x1f
	str r0, [sp]
	add r0, r4, #0
	mov r1, #0x11
	mov r2, #9
	mov r3, #8
	bl BagApp_StartButtonFlash
	add r6, r0, #0
	b _021FA92A
_021FA8F6:
	mov r1, #0x8d
	lsl r1, r1, #2
	ldr r2, [r4, r1]
	mov r3, #0
	add r2, #0x66
	strh r3, [r2]
	ldr r1, [r4, r1]
	mov r2, #5
	add r1, #0x68
	strh r2, [r1]
	ldr r1, [sp, #0x1c]
	bl BagApp_SetMenuInputState
	mov r0, #SEQ_SE_GS_GEARCANCEL>>6
	lsl r0, r0, #6
	bl PlaySE
	mov r0, #0x24
	str r0, [sp]
	add r0, r4, #0
	mov r1, #0x13
	mov r2, #9
	mov r3, #8
	bl BagApp_StartButtonFlash
	add r6, r0, #0
_021FA92A:
	add r0, r6, #0
	pop {r3, r4, r5, r6, r7, pc}
	nop
_021FA930: .word 0x000005DC
_021FA934: .word 0x00000644
_021FA938: .word 0x00000808
	thumb_func_end BagApp_PressButton


	thumb_func_start BagApp_State_PocketSwitch
BagApp_State_PocketSwitch: ; 0x021FA93C
	push {r4, r5, r6, lr}
	add r5, r0, #0
	ldr r0, _021FAA10 ; =0x00000619
	add r4, r5, r0
	bl System_GetTouchHeld
	cmp r0, #0
	bne _021FA958
	ldrb r1, [r4, #7]
	mov r0, #0x70
	bic r1, r0
	mov r0, #0x10
	orr r0, r1
	strb r0, [r4, #7]
_021FA958:
	add r0, r5, #0
	bl BagApp_GetTouchedPocket
	add r6, r0, #0
	mov r0, #0
	mvn r0, r0
	cmp r6, r0
	beq _021FA96E
	ldrb r0, [r4]
	cmp r6, r0
	beq _021FA97A
_021FA96E:
	ldrb r1, [r4, #7]
	mov r0, #0xf
	bic r1, r0
	mov r0, #1
	orr r0, r1
	strb r0, [r4, #7]
_021FA97A:
	add r0, r5, #0
	bl BagApp_HandlePocketSwitchDpad
	cmp r0, #0
	bne _021FA9FE
	ldrb r0, [r4, #7]
	lsl r0, r0, #0x19
	lsr r0, r0, #0x1d
	cmp r0, #1
	bne _021FA9FE
	mov r0, #0
	mvn r0, r0
	cmp r6, r0
	beq _021FA9FE
	mov r0, #0x8d
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	ldrb r1, [r4]
	add r0, #0x64
	strb r1, [r0]
	lsl r0, r6, #0x18
	ldrb r1, [r4]
	lsr r0, r0, #0x18
	cmp r1, r0
	bls _021FA9B4
	mov r0, #0
	strb r0, [r4, #1]
	strb r0, [r4, #2]
	b _021FA9C6
_021FA9B4:
	cmp r1, r0
	bhs _021FA9C2
	mov r0, #1
	strb r0, [r4, #1]
	mov r0, #0
	strb r0, [r4, #2]
	b _021FA9C6
_021FA9C2:
	mov r0, #2
	strb r0, [r4, #1]
_021FA9C6:
	lsl r0, r6, #0x18
	lsr r0, r0, #0x18
	strb r0, [r4]
	ldrb r2, [r4, #7]
	mov r1, #0xf
	bic r2, r1
	strb r2, [r4, #7]
	ldrb r2, [r4, #7]
	mov r1, #0x70
	bic r2, r1
	strb r2, [r4, #7]
	mov r1, #0
	strb r1, [r4, #4]
	mov r1, #0x67
	lsl r1, r1, #4
	strb r0, [r5, r1]
	add r0, r5, #0
	bl ov15_021FF950
	mov r1, #0x67
	lsl r1, r1, #4
	ldrb r1, [r5, r1]
	ldr r0, _021FAA14 ; =0x00000808
	mov r2, #7
	add r0, r5, r0
	add r1, r1, #1
	bl BagApp_StartCameraTween
_021FA9FE:
	add r0, r5, #0
	bl BagApp_RunPocketSwitch
	add r0, r5, #0
	bl ov15_021FF964
	mov r0, #0
	pop {r4, r5, r6, pc}
	nop
_021FAA10: .word 0x00000619
_021FAA14: .word 0x00000808
	thumb_func_end BagApp_State_PocketSwitch


	thumb_func_start BagApp_HandlePocketSwitchDpad
BagApp_HandlePocketSwitchDpad: ; 0x021FAA18
	push {r3, r4, r5, lr}
	ldr r1, _021FAB28 ; =gSystem
	add r5, r0, #0
	ldr r0, _021FAB2C ; =0x00000619
	ldr r2, [r1, #0x48]
	mov r1, #0x20
	add r4, r5, r0
	tst r1, r2
	beq _021FAAA6
	sub r1, r0, #5
	ldrb r1, [r5, r1]
	cmp r1, #1
	bne _021FAA36
	mov r0, #1
	pop {r3, r4, r5, pc}
_021FAA36:
	sub r0, #0x3d
	bl PlaySE
	mov r0, #0x8d
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	ldrb r1, [r4]
	add r0, #0x64
	strb r1, [r0]
	ldrb r0, [r4]
	cmp r0, #0
	beq _021FAA5C
	sub r0, r0, #1
	strb r0, [r4]
	mov r0, #0x67
	lsl r0, r0, #4
	ldrb r1, [r5, r0]
	sub r1, r1, #1
	b _021FAA6A
_021FAA5C:
	ldr r0, _021FAB30 ; =0x00000614
	ldrb r1, [r5, r0]
	sub r1, r1, #1
	strb r1, [r4]
	ldrb r1, [r5, r0]
	add r0, #0x5c
	sub r1, r1, #1
_021FAA6A:
	strb r1, [r5, r0]
	mov r0, #0
	strb r0, [r4, #2]
	strb r0, [r4, #1]
	ldrb r1, [r4, #7]
	mov r0, #0x70
	bic r1, r0
	mov r0, #0x10
	orr r0, r1
	strb r0, [r4, #7]
	ldrb r1, [r4, #7]
	mov r0, #0xf
	bic r1, r0
	mov r0, #1
	orr r0, r1
	strb r0, [r4, #7]
	ldrb r1, [r4, #7]
	mov r0, #0x80
	bic r1, r0
	strb r1, [r4, #7]
	ldrb r0, [r4, #4]
	cmp r0, #3
	beq _021FAA9C
	mov r0, #4
	strb r0, [r4, #4]
_021FAA9C:
	add r0, r5, #0
	bl ov15_021FF950
	mov r0, #1
	pop {r3, r4, r5, pc}
_021FAAA6:
	mov r1, #0x10
	tst r1, r2
	beq _021FAB24
	sub r1, r0, #5
	ldrb r1, [r5, r1]
	cmp r1, #1
	bne _021FAAB8
	mov r0, #1
	pop {r3, r4, r5, pc}
_021FAAB8:
	sub r0, #0x3d
	bl PlaySE
	mov r0, #0x8d
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	ldrb r1, [r4]
	add r0, #0x64
	strb r1, [r0]
	ldrb r0, [r4]
	ldr r1, _021FAB30 ; =0x00000614
	add r2, r0, #1
	ldrb r0, [r5, r1]
	cmp r2, r0
	bge _021FAAE2
	add r0, r1, #0
	strb r2, [r4]
	add r0, #0x5c
	ldrb r0, [r5, r0]
	add r0, r0, #1
	b _021FAAE6
_021FAAE2:
	mov r0, #0
	strb r0, [r4]
_021FAAE6:
	add r1, #0x5c
	strb r0, [r5, r1]
	mov r0, #0
	strb r0, [r4, #2]
	mov r0, #1
	strb r0, [r4, #1]
	ldrb r2, [r4, #7]
	mov r1, #0x70
	bic r2, r1
	mov r1, #0x10
	orr r1, r2
	strb r1, [r4, #7]
	ldrb r2, [r4, #7]
	mov r1, #0xf
	bic r2, r1
	orr r0, r2
	strb r0, [r4, #7]
	ldrb r1, [r4, #7]
	mov r0, #0x80
	bic r1, r0
	strb r1, [r4, #7]
	ldrb r0, [r4, #4]
	cmp r0, #3
	beq _021FAB1A
	mov r0, #4
	strb r0, [r4, #4]
_021FAB1A:
	add r0, r5, #0
	bl ov15_021FF950
	mov r0, #1
	pop {r3, r4, r5, pc}
_021FAB24:
	mov r0, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_021FAB28: .word gSystem
_021FAB2C: .word 0x00000619
_021FAB30: .word 0x00000614
	thumb_func_end BagApp_HandlePocketSwitchDpad


	thumb_func_start BagApp_RunPocketSwitch
BagApp_RunPocketSwitch: ; 0x021FAB34
	push {r3, r4, r5, lr}
	ldr r1, _021FAC28 ; =0x00000619
	add r5, r0, #0
	add r4, r5, r1
	ldrb r1, [r4, #2]
	cmp r1, #0
	beq _021FAB4C
	cmp r1, #1
	beq _021FAB58
	cmp r1, #2
	beq _021FAC20
	b _021FAC24
_021FAB4C:
	mov r0, #0
	strb r0, [r4, #3]
	ldrb r0, [r4, #2]
	add r0, r0, #1
	strb r0, [r4, #2]
	b _021FAC24
_021FAB58:
	ldrb r1, [r4, #3]
	cmp r1, #8
	bhs _021FAB64
	add r0, r1, #1
	strb r0, [r4, #3]
	b _021FAC24
_021FAB64:
	mov r1, #0x8d
	lsl r1, r1, #2
	ldr r1, [r5, r1]
	ldrb r2, [r4]
	add r1, #0x64
	strb r2, [r1]
	bl BagApp_LoadPocketItemNames
	mov r0, #0x8d
	lsl r0, r0, #2
	ldr r1, [r5, r0]
	add r0, r5, #0
	add r2, r1, #0
	add r2, #0x64
	ldrb r3, [r2]
	mov r2, #0xc
	mul r2, r3
	add r1, r1, r2
	mov r2, #0xa
	ldrsh r1, [r1, r2]
	sub r2, #0xb
	mov r3, #0
	bl BagApp_DrawItemList
	add r0, r5, #0
	bl BagApp_GetPageItemCount
	mov r1, #0
	add r2, r0, #0
	add r0, r5, #0
	add r3, r1, #0
	bl BagApp_LoadSubScreenLayout
	mov r1, #0x8d
	lsl r1, r1, #2
	ldr r1, [r5, r1]
	add r0, r5, #0
	add r1, #0x64
	ldrb r1, [r1]
	bl BagApp_LoadPocketObjPalette
	mov r2, #0x8d
	lsl r2, r2, #2
	ldr r2, [r5, r2]
	add r0, r5, #0
	add r2, #0x64
	ldrb r2, [r2]
	mov r1, #1
	bl BagApp_LoadPocketSubBgPalette
	mov r0, #0x8d
	lsl r0, r0, #2
	ldr r3, [r5, r0]
	add r0, r3, #0
	add r0, #0x64
	ldrb r1, [r0]
	mov r0, #0xc
	add r2, r1, #0
	mul r2, r0
	add r0, r3, #0
	add r1, r3, #0
	add r0, #0xa
	add r1, #8
	add r0, r0, r2
	add r1, r1, r2
	add r2, r3, r2
	ldrb r2, [r2, #0xd]
	bl BagApp_ClampPocketScroll
	mov r0, #0x8d
	lsl r0, r0, #2
	ldr r3, [r5, r0]
	add r0, r3, #0
	add r0, #0x64
	ldrb r1, [r0]
	mov r0, #0xc
	add r2, r1, #0
	mul r2, r0
	add r0, r3, #0
	add r1, r3, #0
	add r0, #0xa
	add r1, #8
	add r0, r0, r2
	add r1, r1, r2
	add r2, r3, r2
	ldrb r2, [r2, #0xd]
	mov r3, #6
	bl ov15_021FA070
	ldrb r0, [r4, #2]
	add r0, r0, #1
	strb r0, [r4, #2]
	mov r0, #1
	pop {r3, r4, r5, pc}
_021FAC20:
	mov r0, #1
	pop {r3, r4, r5, pc}
_021FAC24:
	mov r0, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_021FAC28: .word 0x00000619
	thumb_func_end BagApp_RunPocketSwitch
