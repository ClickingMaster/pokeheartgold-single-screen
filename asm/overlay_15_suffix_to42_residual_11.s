#include "constants/sndseq.h"
#include "constants/items.h"
#include "msgdata/msg/msg_0010.h"
	.include "asm/macros.inc"
	.include "overlay_15.inc"
	.include "global.inc"

	.text

	.public sDefaultItemMenuChoices
	.public _02201300
	.public _02201480
	.public BagApp_LoadItemMenuStrings
	.public BagApp_FreeItemMenuStrings
	.public BagApp_PrintItemSelectedMessage
	.public BagApp_ShowItemDescription
	.public BagApp_ClearDescriptionWindow
	.public BagApp_ShowPocketDescription
	.public BagApp_CloseItemPromptWindow
	.public BagApp_CloseItemMenuWindows
	.public BagApp_CloseItemActionWindows
	.public BagApp_PrintMoveItemPrompt
	.public BagApp_PrintQuantityDigits
	.public BagApp_PrintTrashConfirmMessage
	.public BagApp_PrintMessage
	.public BagApp_CreateYesNoPrompt
	.public BagApp_DestroyYesNoPrompt
	.public BagApp_PrintSellPrice
	.public BagApp_PrintMoney
	.public ov15_021FF1E0
	.public BagApp_PrintCancelButton
	.public BagApp_DrawItemList
	.public BagApp_DrawSelectedItemEntry
	.public BagApp_ClearSelectedItemEntry
	.public BagApp_PrintPageNumber
	.public BagApp_PrintItemMenuLabel
	.public BagApp_ClearItemMenuLabels
	.public BagApp_PrintTrashLabel
	.public BagApp_PrintSellLabel
	.public BagApp_ClearQuantityLabel
	.public BagApp_ClearCancelButtonText
	.public BagApp_InitSprites
	.public BagApp_FreeSprites
	.public BagApp_TickSprites
	.public ov15_021FF950
	.public ov15_021FF964
	.public BagApp_SetCursorPosition
	.public BagApp_HideCursor
	.public BagApp_SetMoveCursorPosition
	.public BagApp_SetQuantityCursorPosition
	.public BagApp_LoadPocketObjPalette
	.public BagApp_ShowMoveSlotSprites
	.public BagApp_DrawItemIcons
	.public BagApp_ShowMovingItemIcon
	.public BagApp_ShowItemMenuSprites
	.public BagApp_HideItemMenuSprites
	.public BagApp_ShowSelectedItemIcon
	.public BagApp_GetUnusedDigitCount
	.public BagApp_ShowQuantityButtons
	.public BagApp_HideQuantityButtons
	.public BagApp_ShowPocketTabSprites
	.public BagApp_SetCancelSpriteVisible
	.public sBagModelScale
	.public sCameraTarget
	.public sBagModelPosition
	.public sBagGraphicsModes
	.public sItemMenuNavTable
	.public ov15_02200529
	.public ov15_0220052A
	.public ov15_0220052B
	.public sCameraSetup
	.public sMoveNavTable
	.public ov15_02200585
	.public ov15_02200586
	.public ov15_02200587
	.public sBagModelRotation
	.public sBagGraphicsBanks
	.public sButtonNavTable
	.public ov15_02200641
	.public ov15_02200642
	.public ov15_02200643
	.public sBgTemplate_Main1
	.public sBgTemplate_Main2
	.public sBgTemplate_Main3
	.public sBgTemplate_Sub0
	.public sBgTemplate_Sub1
	.public sBgTemplate_Sub2
	.public sBgTemplate_Sub3
	.public sCameraTweenKeys
	.public sPocketCapacities
	.public sQuantityDigitWindowPositions
	.public sItemMenuWindowPositions
	.public sItemListWindowTemplates
	.public sBagEdgeColorTable
	.public sTouchHitboxTables
	.public sItemSlotRects
	.public sUnusedSlotRects
	.public sItemActionFuncs
	.public sSubScreenTilemapCommands
	.public sItemMenuCursorButtons
	.extern Bag_Exit
	.extern Bag_Init
	.extern Bag_Main
	.extern BagApp_LoadInfoPanelTilemap
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
	.extern BagApp_AddItemListWindows
	.extern BagApp_RemoveItemListWindows
	.extern BagApp_AddItemActionWindows
	.extern BagApp_RemoveItemActionWindows
	.extern BagApp_BufferSlotItemName
	.extern BagApp_BufferSlotItemNamePlural
	.extern BagApp_PrintItemDescription
	.extern BagApp_PrintTMHMMoveInfo
	.extern BagApp_PrintTMHMNumber
	.extern BagApp_LoadListIconChars
	.public BagApp_DrawRegisteredItemIcon
	.public BagApp_LoadItemMenuStrings
	.public BagApp_FreeItemMenuStrings
	.public BagApp_PrintItemSelectedMessage
	.public BagApp_ShowItemDescription
	.public BagApp_ClearDescriptionWindow
	.public BagApp_ShowPocketDescription
	.public BagApp_CloseItemPromptWindow
	.public BagApp_CloseItemMenuWindows
	.public BagApp_CloseItemActionWindows
	.public BagApp_PrintMoveItemPrompt
	.public BagApp_PrintQuantityDigits
	.public BagApp_PrintTrashConfirmMessage
	.public BagApp_PrintMessage
	.public BagApp_TextPrinterCallback
	.public BagApp_CreateYesNoPrompt
	.public BagApp_DestroyYesNoPrompt
	.public BagApp_PrintSellPrice
	.public BagApp_PrintMoney
	.public ov15_021FF1E0
	.public BagApp_PrintCancelButton
	.public BagApp_GetNthItemSlotIndex
	.public BagApp_DrawItemList
	.public BagApp_DrawSelectedItemEntry
	.public BagApp_ClearSelectedItemEntry
	.public BagApp_PrintItemListEntry
	.public BagApp_PrintItemQuantity
	.public BagApp_PrintPageNumber
	.public BagApp_PrintItemMenuLabel
	.public BagApp_ClearItemMenuLabels
	.public BagApp_PrintTrashLabel
	.public BagApp_PrintSellLabel
	.public BagApp_ClearQuantityLabel
	.public BagApp_ClearCancelButtonText
	.public BagApp_InitSprites
	.public BagApp_FreeSprites
	.public BagApp_TickSprites
	.public BagApp_SetItemIconSprite
	.public ov15_021FF950
	.public ov15_021FF964
	.public BagApp_ShowTMHMTypeIcons
	.public BagApp_CreateSpriteSystem
	.public BagApp_LoadSpriteResources
	.public BagApp_CreateSprites
	.public ov15_021FFEC0
	.public BagApp_SetCursorPosition
	.public BagApp_HideCursor
	.public BagApp_SetMoveCursorPosition
	.public BagApp_SetQuantityCursorPosition
	.public BagApp_LoadPocketObjPalette
	.public BagApp_ShowMoveSlotSprites
	.public BagApp_UpdatePageArrows
	.public BagApp_DrawItemIcons
	.public BagApp_ShowMovingItemIcon
	.public BagApp_ShowItemMenuSprites
	.public BagApp_HideItemMenuSprites
	.public BagApp_ShowSelectedItemIcon
	.public BagApp_GetUnusedDigitCount
	.public BagApp_ShowQuantityButtons
	.public BagApp_HideQuantityButtons
	.public BagApp_ShowPocketTabSprites
	.public BagApp_SetCancelSpriteVisible
	.extern sDefaultItemMenuChoices
	.extern sBagModelScale
	.extern sCameraTarget
	.extern sBagModelPosition
	.extern sBagGraphicsModes
	.extern sItemMenuNavTable
	.extern ov15_02200529
	.extern ov15_0220052A
	.extern ov15_0220052B
	.extern sCameraSetup
	.extern sTouchHitboxes_ItemMenu
	.extern sTouchHitboxes_SellQuantity
	.extern sMoveNavTable
	.extern ov15_02200585
	.extern ov15_02200586
	.extern ov15_02200587
	.extern sTouchHitboxes_TrashQuantity
	.extern sBagModelRotation
	.extern sTouchHitboxes_MoveItem
	.extern sBagGraphicsBanks
	.extern sButtonNavTable
	.extern ov15_02200641
	.extern ov15_02200642
	.extern ov15_02200643
	.extern sTouchHitboxes_Main
	.extern sBgTemplate_Main1
	.extern sBgTemplate_Main2
	.extern sBgTemplate_Main3
	.extern sBgTemplate_Sub0
	.extern sBgTemplate_Sub1
	.extern sBgTemplate_Sub2
	.extern sBgTemplate_Sub3
	.extern sCameraTweenKeys
	.extern sPocketCapacities
	.extern gOverlayTemplate_Bag
	.extern sListPocketCapacities
	.extern sQuantityDigitWindowPositions
	.extern sItemMenuWindowPositions
	.extern sItemListWindowTemplates
	.extern sQuantityButtonCounts
	.extern sHiddenQuantityButtonCounts
	.extern sBagOamCharTransferParam
	.extern sBagSpriteResourceCounts
	.extern sQuantityCursorPositions
	.extern ov15_022009D5
	.extern ov15_022009D6
	.extern ov15_022009D7
	.extern sBagOamManagerParam
	.extern sHiddenQuantityButtons
	.extern sMoveCursorPositions
	.extern ov15_02200A35
	.extern ov15_02200A36
	.extern ov15_02200A37
	.extern sQuantityButtonSprites
	.extern sQuantityButtonAnims
	.extern sCursorPositions
	.extern ov15_02200AB9
	.extern ov15_02200ABA
	.extern ov15_02200ABB
	.extern sBagSpriteTemplates
	.extern _02201300
	.extern sBagEdgeColorTable
	.extern sTouchHitboxTables
	.extern sItemSlotRects
	.extern sUnusedSlotRects
	.extern sItemActionFuncs
	.extern sSubScreenTilemapCommands
	.extern sItemMenuCursorButtons
	.extern _02201480

thumb_func_start BagApp_ShowTMHMTypeIcons
BagApp_ShowTMHMTypeIcons: ; 0x021FF97C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r5, r0, #0
	mov r0, #0x9b
	lsl r0, r0, #2
	add r4, r2, #0
	add r6, r1, #0
	ldr r0, [r5, r0]
	add r1, r4, #0
	bl ManagedSprite_SetDrawFlag
	mov r0, #0x27
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	add r1, r4, #0
	bl ManagedSprite_SetDrawFlag
	cmp r4, #0
	beq _021FFA34
	add r0, r6, #0
	bl TMHMGetMove
	mov r1, #3
	add r4, r0, #0
	bl GetMoveAttr
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	add r0, r4, #0
	mov r1, #1
	bl GetMoveAttr
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	bl sub_020776B4
	add r7, r0, #0
	add r0, r6, #0
	bl sub_02077678
	add r3, r0, #0
	mov r0, #1
	str r0, [sp]
	ldr r0, _021FFA38 ; =0x0000C103
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [sp, #4]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	add r2, r7, #0
	bl SpriteSystem_ReplaceCharResObj
	add r0, r6, #0
	bl sub_0207769C
	add r1, r0, #0
	mov r0, #0x9b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	add r1, r1, #4
	bl ManagedSprite_SetPaletteOverride
	bl sub_02077830
	add r6, r0, #0
	add r0, r4, #0
	bl sub_02077800
	add r3, r0, #0
	mov r0, #1
	str r0, [sp]
	ldr r0, _021FFA3C ; =0x0000C104
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [sp, #4]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	add r2, r6, #0
	bl SpriteSystem_ReplaceCharResObj
	add r0, r4, #0
	bl sub_02077818
	add r1, r0, #0
	mov r0, #0x27
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	add r1, r1, #4
	bl ManagedSprite_SetPaletteOverride
_021FFA34:
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_021FFA38: .word 0x0000C103
_021FFA3C: .word 0x0000C104
	thumb_func_end BagApp_ShowTMHMTypeIcons

thumb_func_start BagApp_CreateSpriteSystem
BagApp_CreateSpriteSystem: ; 0x021FFA40
	push {r4, r5, r6, r7, lr}
	sub sp, #0x4c
	ldr r3, _021FFAC4 ; =sBagSpriteResourceCounts
	add r2, sp, #0x34
	add r4, r0, #0
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	mov r0, #6
	bl SpriteSystem_Alloc
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [r4, r1]
	ldr r0, [r4, r1]
	bl SpriteManager_New
	mov r7, #0x93
	lsl r7, r7, #2
	add r2, sp, #0x14
	ldr r3, _021FFAC8 ; =sBagOamManagerParam
	str r0, [r4, r7]
	ldmia r3!, {r0, r1}
	add r6, r2, #0
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	ldr r5, _021FFACC ; =sBagOamCharTransferParam
	stmia r2!, {r0, r1}
	add r3, sp, #0
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	add r1, r6, #0
	str r0, [r3]
	sub r0, r7, #4
	ldr r0, [r4, r0]
	mov r3, #0x20
	bl SpriteSystem_Init
	sub r1, r7, #4
	ldr r0, [r4, r1]
	add r1, r1, #4
	ldr r1, [r4, r1]
	mov r2, #0x27
	bl SpriteSystem_InitSprites
	sub r1, r7, #4
	ldr r0, [r4, r1]
	add r1, r1, #4
	ldr r1, [r4, r1]
	add r2, sp, #0x34
	bl SpriteSystem_InitManagerWithCapacities
	add sp, #0x4c
	pop {r4, r5, r6, r7, pc}
	nop
_021FFAC4: .word sBagSpriteResourceCounts
_021FFAC8: .word sBagOamManagerParam
_021FFACC: .word sBagOamCharTransferParam
	thumb_func_end BagApp_CreateSpriteSystem

thumb_func_start BagApp_LoadSpriteResources
BagApp_LoadSpriteResources: ; 0x021FFAD0
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r5, r0, #0
	mov r0, #0
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, _021FFDAC ; =0x0000C0F9
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [sp, #8]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	mov r2, #0xf
	mov r3, #0x1a
	bl SpriteSystem_LoadCharResObj
	mov r0, #0
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, _021FFDB0 ; =0x0000C0FA
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [sp, #8]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	mov r2, #0xf
	mov r3, #6
	bl SpriteSystem_LoadCharResObj
	mov r0, #0
	str r0, [sp]
	mov r0, #2
	str r0, [sp, #4]
	ldr r0, _021FFDB4 ; =0x0000C0FB
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [sp, #8]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	mov r2, #0xf
	mov r3, #0x33
	bl SpriteSystem_LoadCharResObj
	mov r0, #0
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, _021FFDB8 ; =0x0000C102
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [sp, #8]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	mov r2, #0x3c
	mov r3, #4
	bl SpriteSystem_LoadCharResObj
	mov r4, #0
	add r6, r4, #0
	mov r7, #2
_021FFB54:
	mov r0, #0
	mov r1, #1
	bl GetItemIndexMapping
	add r3, r0, #0
	ldr r0, _021FFDBC ; =0x0000C0FC
	str r6, [sp]
	mov r1, #0x93
	str r7, [sp, #4]
	add r0, r4, r0
	str r0, [sp, #8]
	mov r0, #0x92
	lsl r0, r0, #2
	lsl r1, r1, #2
	ldr r0, [r5, r0]
	ldr r1, [r5, r1]
	mov r2, #0x12
	bl SpriteSystem_LoadCharResObj
	add r4, r4, #1
	cmp r4, #6
	blt _021FFB54
	ldr r0, _021FFDC0 ; =0x0000C103
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [sp]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	mov r2, #1
	mov r3, #0
	bl sub_020776B8
	ldr r0, _021FFDC4 ; =0x0000C104
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [sp]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	mov r2, #1
	mov r3, #0
	bl sub_02077834
	mov r0, #0
	str r0, [sp]
	mov r0, #2
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	ldr r0, _021FFDAC ; =0x0000C0F9
	mov r1, #0x92
	mov r2, #0xf
	lsl r1, r1, #2
	str r0, [sp, #0xc]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	add r3, r2, #0
	bl SpriteSystem_LoadPlttResObj
	mov r0, #0
	str r0, [sp]
	mov r0, #2
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	ldr r0, _021FFDC8 ; =0x0000C101
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [sp, #0xc]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	mov r2, #0x3c
	mov r3, #0xa
	bl SpriteSystem_LoadPlttResObj
	mov r1, #0x92
	lsl r1, r1, #2
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	ldr r3, _021FFDB8 ; =0x0000C102
	mov r2, #1
	bl sub_020776EC
	mov r0, #0
	str r0, [sp]
	mov r0, #0xa
	str r0, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	ldr r0, _021FFDB0 ; =0x0000C0FA
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [sp, #0xc]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	mov r2, #0xf
	mov r3, #0x2f
	bl SpriteSystem_LoadPlttResObj
	mov r4, #0
	add r6, r4, #0
	mov r7, #1
_021FFC2A:
	mov r0, #0
	mov r1, #2
	bl GetItemIndexMapping
	str r6, [sp]
	add r3, r0, #0
	mov r1, #0x93
	str r7, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	ldr r0, _021FFDB4 ; =0x0000C0FB
	lsl r1, r1, #2
	add r0, r4, r0
	str r0, [sp, #0xc]
	mov r0, #0x92
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	ldr r1, [r5, r1]
	mov r2, #0x12
	bl SpriteSystem_LoadPlttResObj
	add r4, r4, #1
	cmp r4, #6
	blt _021FFC2A
	mov r0, #0
	str r0, [sp]
	ldr r0, _021FFDAC ; =0x0000C0F9
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [sp, #4]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	mov r2, #0xf
	mov r3, #0x19
	bl SpriteSystem_LoadCellResObj
	mov r0, #0
	str r0, [sp]
	ldr r0, _021FFDB0 ; =0x0000C0FA
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [sp, #4]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	mov r2, #0xf
	mov r3, #5
	bl SpriteSystem_LoadCellResObj
	mov r0, #0
	str r0, [sp]
	ldr r0, _021FFDB4 ; =0x0000C0FB
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [sp, #4]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	mov r2, #0xf
	mov r3, #0x31
	bl SpriteSystem_LoadCellResObj
	bl GetItemIconCell
	add r3, r0, #0
	mov r0, #0
	str r0, [sp]
	ldr r0, _021FFDBC ; =0x0000C0FC
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [sp, #4]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	mov r2, #0x12
	bl SpriteSystem_LoadCellResObj
	mov r0, #0
	str r0, [sp]
	ldr r0, _021FFDCC ; =0x0000C0FD
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [sp, #4]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	mov r2, #0x3c
	mov r3, #5
	bl SpriteSystem_LoadCellResObj
	mov r0, #0
	str r0, [sp]
	ldr r0, _021FFDAC ; =0x0000C0F9
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [sp, #4]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	mov r2, #0xf
	mov r3, #0x15
	bl SpriteSystem_LoadAnimResObj
	mov r0, #0
	str r0, [sp]
	ldr r0, _021FFDB0 ; =0x0000C0FA
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [sp, #4]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	mov r2, #0xf
	mov r3, #0x18
	bl SpriteSystem_LoadAnimResObj
	mov r0, #0
	str r0, [sp]
	ldr r0, _021FFDB4 ; =0x0000C0FB
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [sp, #4]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	mov r2, #0xf
	mov r3, #4
	bl SpriteSystem_LoadAnimResObj
	mov r0, #0
	str r0, [sp]
	ldr r0, _021FFDBC ; =0x0000C0FC
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [sp, #4]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	mov r2, #0xf
	mov r3, #0x32
	bl SpriteSystem_LoadAnimResObj
	bl GetItemIconAnim
	add r3, r0, #0
	mov r0, #0
	str r0, [sp]
	ldr r0, _021FFDCC ; =0x0000C0FD
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [sp, #4]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	mov r2, #0x12
	bl SpriteSystem_LoadAnimResObj
	mov r0, #0
	str r0, [sp]
	ldr r0, _021FFDD0 ; =0x0000C0FE
	mov r1, #0x92
	lsl r1, r1, #2
	str r0, [sp, #4]
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	mov r2, #0x3c
	mov r3, #6
	bl SpriteSystem_LoadAnimResObj
	mov r1, #0x92
	lsl r1, r1, #2
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r2, _021FFDD0 ; =0x0000C0FE
	ldr r1, [r5, r1]
	add r3, r2, #1
	bl sub_0207775C
	mov r2, #0x6a
	lsl r2, r2, #4
	mov r0, #0xf
	mov r1, #0x30
	add r2, r5, r2
	mov r3, #6
	bl GfGfxLoader_GetPlttData
	ldr r1, _021FFDD4 ; =0x0000069C
	str r0, [r5, r1]
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	nop
_021FFDAC: .word 0x0000C0F9
_021FFDB0: .word 0x0000C0FA
_021FFDB4: .word 0x0000C0FB
_021FFDB8: .word 0x0000C102
_021FFDBC: .word 0x0000C0FC
_021FFDC0: .word 0x0000C103
_021FFDC4: .word 0x0000C104
_021FFDC8: .word 0x0000C101
_021FFDCC: .word 0x0000C0FD
_021FFDD0: .word 0x0000C0FE
_021FFDD4: .word 0x0000069C
	thumb_func_end BagApp_LoadSpriteResources

thumb_func_start BagApp_CreateSprites
BagApp_CreateSprites: ; 0x021FFDD8
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	ldr r4, _021FFEBC ; =sBagSpriteTemplates
	mov r7, #0
	add r5, r6, #0
_021FFDE2:
	mov r0, #0x92
	mov r1, #0x93
	lsl r0, r0, #2
	lsl r1, r1, #2
	mov r3, #1
	ldr r0, [r6, r0]
	ldr r1, [r6, r1]
	add r2, r4, #0
	lsl r3, r3, #0x14
	bl SpriteSystem_NewSpriteWithYOffset
	mov r1, #0x25
	lsl r1, r1, #4
	str r0, [r5, r1]
	add r7, r7, #1
	add r4, #0x34
	add r5, r5, #4
	cmp r7, #0x27
	blo _021FFDE2
	add r0, r1, #0
	add r0, #0x4c
	ldr r0, [r6, r0]
	mov r1, #1
	bl ManagedSprite_SetPriority
	mov r7, #0xb
	mov r5, #0
	add r4, r6, #0
	lsl r7, r7, #6
_021FFE1C:
	ldr r0, [r4, r7]
	mov r1, #1
	bl ManagedSprite_SetPriority
	add r5, r5, #1
	add r4, r4, #4
	cmp r5, #4
	blo _021FFE1C
	mov r7, #0x9d
	mov r5, #0
	add r4, r6, #0
	lsl r7, r7, #2
_021FFE34:
	ldr r0, [r4, r7]
	mov r1, #1
	bl ManagedSprite_SetPriority
	add r5, r5, #1
	add r4, r4, #4
	cmp r5, #8
	blo _021FFE34
	add r0, r6, #0
	mov r1, #1
	bl BagApp_ShowPocketTabSprites
	mov r0, #0x25
	lsl r0, r0, #4
	ldr r0, [r6, r0]
	mov r1, #0
	bl ManagedSprite_SetDrawFlag
	mov r0, #0x9b
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	mov r1, #0
	bl ManagedSprite_SetDrawFlag
	mov r0, #0x27
	lsl r0, r0, #4
	ldr r0, [r6, r0]
	mov r1, #0
	bl ManagedSprite_SetDrawFlag
	mov r7, #0xb
	mov r5, #0
	add r4, r6, #0
	lsl r7, r7, #6
_021FFE78:
	ldr r0, [r4, r7]
	mov r1, #0
	bl ManagedSprite_SetDrawFlag
	add r5, r5, #1
	add r4, r4, #4
	cmp r5, #4
	blo _021FFE78
	mov r7, #0x2d
	mov r5, #0
	add r4, r6, #0
	lsl r7, r7, #4
_021FFE90:
	ldr r0, [r4, r7]
	mov r1, #0
	bl ManagedSprite_SetDrawFlag
	add r5, r5, #1
	add r4, r4, #4
	cmp r5, #6
	blo _021FFE90
	mov r0, #0xba
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	mov r1, #0
	bl ManagedSprite_SetDrawFlag
	mov r0, #0xba
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	mov r1, #1
	bl ManagedSprite_SetPriority
	pop {r3, r4, r5, r6, r7, pc}
	nop
_021FFEBC: .word sBagSpriteTemplates
	thumb_func_end BagApp_CreateSprites
