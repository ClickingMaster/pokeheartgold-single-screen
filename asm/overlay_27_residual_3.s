	.include "asm/macros.inc"
	.include "overlay_27.inc"
	.include "global.inc"

	.text
	.public _0225CEBC
	.public TouchscreenMenu_Create
	.public TouchscreenMenu_Destroy
	.public TouchscreenMenu_Task
	.public TouchscreenMenu_UpdateRunningShoesVisibility
	.public TouchscreenMenu_HandleActionButton
	.public TouchscreenMenu_GetActionType
	.public TouchscreenMenu_PrintActionLabel
	.public TouchscreenMenu_SetButtonsActive
	.public ov27_0225A714
	.public ov27_0225A7FC
	.public TouchscreenMenu_IsFieldBusy
	.public TouchscreenMenu_SetDimmed
	.public TouchscreenMenu_UpdateRegisteredItemButtons
	.public TouchscreenMenu_ShowIcons
	.public TouchscreenMenu_ShowNewIcons
	.public TouchscreenMenu_LoadBgAndWindows
	.public TouchscreenMenu_LoadSpriteResources
	.public TouchscreenMenu_LoadIconResources
	.public TouchscreenMenu_CreateSprites
	.public TouchscreenMenu_FindNeighborIcon
	.public TouchscreenMenu_HighlightIcon
	.public TouchscreenMenu_HandleDpadInput
	.public TouchscreenMenu_HandleTouchInput
	.public TouchscreenMenu_DrawWindowFrame
	.public TouchscreenMenu_LoadStrings
	.public TouchscreenMenu_FreeStrings
	.public TouchscreenMenu_CopyIconLabelsToVram
	.public TouchscreenMenu_PrintIconLabels
	.public TouchscreenMenu_GetLayout
	.public TouchscreenMenu_UpdatePhoneRing
	.public TouchscreenMenu_PrintMessageAt
	.public TouchscreenMenu_PrintItemCount
	.public TouchscreenMenu_PrintContestMonLevel
	.public TouchscreenMenu_PrintBugContestInfo
	.public TouchscreenMenu_InitIconFlags
	.public TouchscreenMenu_GetMenuIndex
	.public TouchscreenMenu_GetSlotFromMenuIndex
	.public TouchscreenMenu_ValidateSelection
	.public TouchscreenChoiceMenu_Create
	.public TouchscreenChoiceMenu_Destroy
	.public TouchscreenChoiceMenu_Task
	.public TouchscreenChoiceMenu_LoadBgGraphics
	.public TouchscreenChoiceMenu_ShowYesNo
	.public TouchscreenChoiceMenu_ShowList
	.public TouchscreenChoiceMenu_LoadSpriteResources
	.public TouchscreenChoiceMenu_CreateCursor
	.public TouchscreenChoiceMenu_State_Idle
	.public TouchscreenChoiceMenu_State_ShowList
	.public TouchscreenChoiceMenu_State_ListInput
	.public TouchscreenChoiceMenu_SetListCursorPos
	.public TouchscreenChoiceMenu_HandleYesNoInput
	.public TouchscreenChoiceMenu_State_CancelYesNo
	.public sTouchscreenMenuTextColors
	.public sTouchscreenMenuButtonHitboxes
	.public sTouchscreenMenuActionMsgIds
	.public sTouchscreenMenuBgPalettes
	.public sTouchscreenMenuBgChars
	.public sTouchscreenMenuBgScreens
	.public sTouchscreenMenuIconAnims
	.public sTouchscreenMenuCellIds
	.public sTouchscreenMenuIconHitboxes
	.public sTouchscreenMenuIcons
	.public sTouchscreenMenuLayouts
	.public sTouchscreenMenuBgTemplate_Sub0
	.public sTouchscreenMenuBgTemplate_Sub1
	.public sTouchscreenMenuIconPositions
	.public sTouchscreenMenuButtonPositions
	.public sTouchscreenMenuIconWindowPositions
	.public sTouchscreenMenuIconNeighbors
	.public sTouchscreenMenuActionTypeToString
	.public ov27_0225D110
	.public sTouchscreenChoiceMenuYesNoCursorX
	.public sTouchscreenChoiceMenuYesNoCursorY
	.public sTouchscreenChoiceMenuYesNoHitboxes
	.public ov27_0225D12C
	.public ov27_0225D138
	.public ov27_0225D144
	.public ov27_0225D154
	.public ov27_0225D164
	.public ov27_0225D174
	.public ov27_0225D188
	.public ov27_0225D19C
	.public ov27_0225D1B4
	.public ov27_0225D1CC
	.public ov27_0225D1E4
	.public ov27_0225D200
	.public ov27_0225D21C
	.public ov27_0225D23C
	.public ov27_0225D25C
	.public ov27_0225D27C
	.public ov27_0225D2A0
	.public ov27_0225D2C8
	.public ov27_0225D2F8
	.public ov27_0225D330
	.public sTouchscreenChoiceMenuBgTemplate_Sub0
	.public sTouchscreenChoiceMenuBgTemplate_Sub1
	.public sTouchscreenChoiceMenuBgTemplate_Sub2
	.public sTouchscreenChoiceMenuListCursorX
	.public sTouchscreenChoiceMenuListCursorY
	.public sTouchscreenChoiceMenuListCursorAnim
	.public sTouchscreenChoiceMenuListNeighbors
	.public sTouchscreenChoiceMenuListHitboxes
	.public sTouchscreenChoiceMenuListWindowTemplates
	.public sTouchscreenChoiceMenuStateFuncs

	.public ov27_0225A2C8
	.public TouchscreenMenu_RequestSelectedIconAnim
	.public TouchscreenMenu_RequestRegisteredItemAnim
	.public TouchscreenMenu_SetRunningShoesSprites
	.public TouchscreenMenu_HandleRunningShoesButton
	.public TouchscreenMenu_UpdateRunningShoesSprites
	.public TouchscreenMenu_UpdateActionLabel
	.public ov27_0225A7B0
	.public ov27_0225A7DC
	.public ov27_0225A86C
	.public TouchscreenMenu_SumBytes
	.public TouchscreenMenu_GetBallItem
	.public TouchscreenMenu_SetIconsTranslucent
	.public TouchscreenMenu_PrintFramedText
	.public TouchscreenMenu_IsTextboxOpen
	.public TouchscreenMenu_LayoutHasRegisteredItems
	.public TouchscreenMenu_HasRunningShoes
	.public TouchscreenMenu_InitPhoneRing
	.public TouchscreenMenu_HidePhoneCaller
	.public TouchscreenMenu_PrintBallLabel
	.public TouchscreenMenu_PrintSafariBalls
	.public TouchscreenMenu_PrintParkBalls
	.public TouchscreenMenu_PrintBallInfo
	.public ov27_0225C238
	.public ov27_0225C248
	.public ov27_0225C24C
	.public ov27_0225C418
	.public TouchscreenChoiceMenu_SetCallback
	.public TouchscreenChoiceMenu_HideYesNo
	.public TouchscreenChoiceMenu_HideList
	.public TouchscreenChoiceMenu_PrintMessage
	.public TouchscreenChoiceMenu_LoadMessages
	.public TouchscreenChoiceMenu_FreeMessages
	.public TouchscreenChoiceMenu_State_Init
	.public TouchscreenChoiceMenu_State_ShowYesNo
	.public TouchscreenChoiceMenu_State_YesNoInput
	.public TouchscreenChoiceMenu_State_YesNoEnd
	.public TouchscreenChoiceMenu_State_Done
	.public TouchscreenChoiceMenu_State_WaitReturn
	.public TouchscreenChoiceMenu_MoveListSelection
	.public TouchscreenChoiceMenu_State_ListEnd
	.public TouchscreenChoiceMenu_State_ListWriteResult
	.public TouchscreenChoiceMenu_SetYesNoCursorPos
	.public TouchscreenChoiceMenu_SetListCursorPressed

	thumb_func_start TouchscreenMenu_UpdateRunningShoesVisibility
TouchscreenMenu_UpdateRunningShoesVisibility: ; 0x0225A4D0
	push {r3, r4, r5, lr}
	add r5, r0, #0
	bl TouchscreenMenu_HasRunningShoes
	add r4, r0, #0
	ldr r0, [r5, #0x10]
	ldr r0, [r0, #0x40]
	bl PlayerAvatar_GetState
	cmp r0, #1
	beq _0225A4F2
	ldr r0, [r5, #0x10]
	add r0, #0xd2
	ldrb r0, [r0]
	lsl r0, r0, #0x1a
	lsr r0, r0, #0x1a
	beq _0225A4F4
_0225A4F2:
	mov r4, #0
_0225A4F4:
	cmp r4, #0
	beq _0225A512
	mov r0, #0xef
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #1
	bl Sprite_SetDrawFlag
	mov r0, #0xf1
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #1
	bl Sprite_SetDrawFlag
	b _0225A52A
_0225A512:
	mov r0, #0xef
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
	mov r0, #0xf1
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
_0225A52A:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end TouchscreenMenu_UpdateRunningShoesVisibility


	thumb_func_start TouchscreenMenu_HandleActionButton
TouchscreenMenu_HandleActionButton: ; 0x0225A530
	push {r4, r5, r6, lr}
	add r5, r0, #0
	mov r0, #0xf
	lsl r0, r0, #6
	ldr r0, [r5, r0]
	add r4, r1, #0
	bl Sprite_GetAnimationNumber
	add r6, r0, #0
	mov r0, #0xf
	lsl r0, r0, #6
	ldr r0, [r5, r0]
	bl Sprite_GetDrawFlag
	cmp r0, #0
	beq _0225A58C
	cmp r4, #1
	bne _0225A570
	bl System_GetTouchNew
	cmp r0, #0
	beq _0225A570
	ldr r0, _0225A590 ; =gSystem
	mov r1, #1
	str r1, [r0, #0x5c]
	mov r0, #0xf
	lsl r0, r0, #6
	ldr r0, [r5, r0]
	mov r1, #6
	bl Sprite_SetAnimCtrlSeq
	pop {r4, r5, r6, pc}
_0225A570:
	cmp r4, #1
	bne _0225A580
	cmp r6, #6
	bne _0225A580
	ldr r0, _0225A590 ; =gSystem
	mov r1, #1
	str r1, [r0, #0x5c]
	pop {r4, r5, r6, pc}
_0225A580:
	mov r0, #0xf
	lsl r0, r0, #6
	ldr r0, [r5, r0]
	mov r1, #5
	bl Sprite_SetAnimCtrlSeq
_0225A58C:
	pop {r4, r5, r6, pc}
	nop
_0225A590: .word gSystem
	thumb_func_end TouchscreenMenu_HandleActionButton


	thumb_func_start TouchscreenMenu_GetActionType
TouchscreenMenu_GetActionType: ; 0x0225A594
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	ldr r4, [r5, #0x10]
	add r0, r4, #0
	bl TouchscreenMenu_IsTextboxOpen
	cmp r0, #0
	beq _0225A5A8
	mov r0, #4
	pop {r3, r4, r5, r6, r7, pc}
_0225A5A8:
	add r0, r4, #0
	bl FieldSystem_IsPlayerMovementAllowed
	cmp r0, #0
	bne _0225A5E6
	add r0, r4, #0
	bl FieldSystem_GetPlayerAvatar
	bl PlayerAvatar_GetMapObject
	add r7, r0, #0
	bl sub_0205F330
	add r6, r0, #0
	add r0, r7, #0
	bl MapObject_GetSpriteID
	sub r0, #0xbc
	cmp r0, #1
	bhi _0225A5DC
	cmp r6, #1
	bne _0225A5D8
	mov r0, #3
	pop {r3, r4, r5, r6, r7, pc}
_0225A5D8:
	mov r0, #4
	pop {r3, r4, r5, r6, r7, pc}
_0225A5DC:
	mov r0, #0x51
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	cmp r0, #4
	bne _0225A61A
_0225A5E6:
	add r0, r4, #0
	bl ov01_021E7F54
	add r5, r0, #0
	cmp r5, #1
	bne _0225A618
	add r0, r4, #0
	add r1, sp, #0
	bl FieldSystem_GetFacingObject
	ldr r0, [sp]
	bl MapObject_GetScriptID
	bl IsScriptIdWithoutTalkAction
	cmp r0, #0
	bne _0225A616
	ldr r0, [sp]
	bl MapObject_GetSpriteID
	bl IsSpriteIdWithoutTalkAction
	cmp r0, #0
	beq _0225A618
_0225A616:
	mov r5, #0
_0225A618:
	add r0, r5, #0
_0225A61A:
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end TouchscreenMenu_GetActionType


	thumb_func_start TouchscreenMenu_PrintActionLabel
TouchscreenMenu_PrintActionLabel: ; 0x0225A61C
	push {r4, r5, r6, lr}
	sub sp, #0x10
	add r5, r0, #0
	mov r0, #0x3d
	lsl r0, r0, #4
	add r4, r1, #0
	add r0, r5, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r6, _0225A664 ; =sTouchscreenMenuActionTypeToString
	mov r3, #0
	ldrb r4, [r6, r4]
	str r3, [sp]
	mov r2, #0x3d
	lsl r4, r4, #2
	ldr r0, _0225A668 ; =0x000F0100
	str r3, [sp, #4]
	str r0, [sp, #8]
	lsl r2, r2, #4
	add r0, r5, r2
	str r3, [sp, #0xc]
	add r4, r5, r4
	add r2, #0xe0
	ldr r2, [r4, r2]
	mov r1, #4
	bl AddTextPrinterParameterizedWithColor
	mov r0, #0xf
	lsl r0, r0, #6
	ldr r0, [r5, r0]
	mov r1, #1
	bl Sprite_SetDrawFlag
	add sp, #0x10
	pop {r4, r5, r6, pc}
	.balign 4, 0
_0225A664: .word sTouchscreenMenuActionTypeToString
_0225A668: .word 0x000F0100
	thumb_func_end TouchscreenMenu_PrintActionLabel
