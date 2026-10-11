	.include "asm/macros.inc"
	.public sBallCapsuleEditorGraphicsModes
	.public BallCapsuleEditor_LoadBattleOverlays
	.public BallCapsuleEditor_Init
	.public BallCapsuleEditor_Main
	.public BallCapsuleEditor_Exit
	.public BallCapsuleEditor_ResetDisplay
	.public BallCapsuleEditor_DisableInterrupts
	.public BallCapsuleEditor_InitBgs
	.public BallCapsuleEditor_VBlankCB
	.public BallCapsuleEditor_GetArgsCapsuleIndex
	.public BallCapsuleEditor_SetArgsCapsuleIndex
	.public BallCapsuleEditor_SetArgsSetCapsuleOnMon
	.public BallCapsuleEditor_SetSealHitbox
	.public BallCapsuleEditor_SetDragHitbox
	.public BallCapsuleEditor_UpdateSealHitbox
	.public BallCapsuleEditor_ShiftSealsToCapsule
	.public BallCapsuleEditor_LoadSeals
	.public BallCapsuleEditor_AddSeal
	.public BallCapsuleEditor_HasFreeSealSlot
	.public BallCapsuleEditor_BringSealToFront
	.public BallCapsuleEditor_LoadSealSpriteResources
	.public BallCapsuleEditor_SortSeals
	.public BallCapsuleEditor_CreateSealSprite
	.public BallCapsuleEditor_HandleSealTouch
	.public BallCapsuleEditor_RemoveAllSeals
	.public BallCapsuleEditor_SetSealsDrawFlag
	.public BallCapsuleEditor_IsSealOnCapsule
	.public BallCapsuleEditor_RemoveSeal
	.public BallCapsuleEditor_UpdateSealOamMode
	.public BallCapsuleEditor_IsCapsuleChanged
	.public BallCapsuleEditor_StoreSealPosition
	.public BallCapsuleEditor_SaveCapsule
	.public BallCapsuleEditor_GetSealData
	.public BallCapsuleEditor_SetSpriteHitbox
	.public BallCapsuleEditor_AddMessageWindows
	.public BallCapsuleEditor_RemoveMessageWindows
	.public BallCapsuleEditor_PrintExitLabel
	.public BallCapsuleEditor_InitFonts
	.public BallCapsuleEditor_FreeFonts
	.public BallCapsuleEditor_LoadButtonLabelPalette
	.public BallCapsuleEditor_CreateButtonLabels
	.public BallCapsuleEditor_SetButtonLabelsDrawFlag
	.public BallCapsuleEditor_CreateButtonLabel
	.public BallCapsuleEditor_FillPageSeals
	.public BallCapsuleEditor_LoadCaseSealSpriteResources
	.public BallCapsuleEditor_CreateCaseSealSprites
	.public BallCapsuleEditor_DeleteCaseSealSprites
	.public BallCapsuleEditor_MoveSeals
	.public BallCapsuleEditor_Create3DVramMan
	.public BallCapsuleEditor_Init3D
	.public BallCapsuleEditor_Alloc3DVram
	.public BallCapsuleEditor_End3DFrame
	.public BallCapsuleEditor_CreateMenu
	.public BallCapsuleEditor_DestroyMenu
	.public BallCapsuleEditor_GetCapsulePosition
	.public BallCapsuleEditor_LoadMonIconResources
	.public BallCapsuleEditor_CreateMonIcons
	.public BallCapsuleEditor_PlaceMonIcons
	.public BallCapsuleEditor_TickMonIcons
	.public BallCapsuleEditor_DeleteMonIcons
	.public BallCapsuleEditor_UpdateCapsuleIconStates
	.public BallCapsuleEditor_InitSpriteSystem
	.public BallCapsuleEditor_FreeSpriteSystem
	.public BallCapsuleEditor_LoadArrowResources
	.public BallCapsuleEditor_CreateArrows
	.public BallCapsuleEditor_TickArrows
	.public BallCapsuleEditor_SetArrowsDrawFlag
	.public BallCapsuleEditor_DeleteArrows
	.public BallCapsuleEditor_LoadSpriteResources
	.public BallCapsuleEditor_CreateCapsuleIcons
	.public BallCapsuleEditor_RefreshCapsuleIcons
	.public BallCapsuleEditor_SwitchCapsules
	.public BallCapsuleEditor_TickCapsuleIcons
	.public BallCapsuleEditor_DeleteCapsuleIcons
	.public BallCapsuleEditor_LoadMainBgGraphics
	.public BallCapsuleEditor_LoadSubBgGraphics
	.public BallCapsuleEditor_PrintMessage
	.public BallCapsuleEditor_AddFramedWindow
	.public BallCapsuleEditor_AddWindow
	.public BallCapsuleEditor_AddWindowWithPalette
	.public BallCapsuleEditor_AddSealCountWindows
	.public BallCapsuleEditor_PrintSealCounts
	.public BallCapsuleEditor_PrintSealCount
	.public BallCapsuleEditor_RemoveSealCountWindows
	.public BallCapsuleEditor_PrintSealName
	.public BallCapsuleEditor_ShowScreens
	.public BallCapsuleEditor_FadeIn
	.public BallCapsuleEditor_FadeOut
	.public BallCapsuleEditor_LoadCaseBgGraphics
	.public BallCapsuleEditor_LoadSubBgCapsuleGraphics
	.public BallCapsuleEditor_LoadSubBgFrameGraphics
	.public BallCapsuleEditor_LoadCaseButtonResources
	.public BallCapsuleEditor_CreateCaseButtons
	.public BallCapsuleEditor_SetCaseSpritesDrawFlag
	.public BallCapsuleEditor_Noop
	.public BallCapsuleEditor_DeleteCaseButtons
	.public BallCapsuleEditor_CapsuleHasSeals
	.public BallCapsuleEditor_GetMenuKind
	.public BallCapsuleEditor_RunMode
	.public BallCapsuleEditor_ModeSelect
	.public BallCapsuleEditor_ModeEdit
	.public BallCapsuleEditor_ModeRemove
	.public BallCapsuleEditor_ModeSwitch
	.public BallCapsuleEditor_ModeNone
	.public BallCapsuleEditor_EnterEdit
	.public BallCapsuleEditor_LeaveEdit
	.public BallCapsuleEditor_ConfirmQuitEdit
	.public BallCapsuleEditor_PreviewCapsule
	.public BallCapsuleEditor_HitboxCallback
	.public BallCapsuleEditor_UpdateMenuInputState
	.public BallCapsuleEditor_HandleSelectInput
	.public BallCapsuleEditor_HandleSelection
	.public BallCapsuleEditor_HandleSelectKeys
	.public BallCapsuleEditor_HandleSelectTouch
	.public BallCapsuleEditor_HandleSwitchInput
	.public BallCapsuleEditor_FinishSwitch
	.public BallCapsuleEditor_HandleSwitchSelection
	.public BallCapsuleEditor_HandleSwitchKeys
	.public BallCapsuleEditor_HandleSwitchTouch
	.public BallCapsuleEditor_MoveCapsuleCursor
	.public BallCapsuleEditor_UpdateCursors
	.public BallCapsuleEditor_GetTouchedCapsule
	.public BallCapsuleEditor_HandleEditKeys
	.public BallCapsuleEditor_MenuEdit
	.public BallCapsuleEditor_MenuQuit
	.public BallCapsuleEditor_MenuSet
	.public BallCapsuleEditor_MenuRemove
	.public BallCapsuleEditor_MenuSwitch
	.public BallCapsuleEditor_SaveEditedCapsule
	.public BallCapsuleEditor_RestoreEditedCapsule
	.public BallCapsuleEditor_SaveSealCounts
	.public BallCapsuleEditor_RestoreSealCounts
	.public BallCapsuleEditor_CopyPreviewMon
	.public BallCapsuleEditor_SetMode
	.public BallCapsuleEditor_SetButtonFrame
	.public BallCapsuleEditor_MoveButtonLabel
	.public BallCapsuleEditor_Task_ButtonPress
	.public BallCapsuleEditor_StartButtonPress
	.public ov57_0223B940
	.public BallCapsuleEditor_SetInputEnabled
	.public BallCapsuleEditor_CreatePreviewPokepic
	.public BallCapsuleEditor_StartPreviewPokepicAnim
	.public BallCapsuleEditor_ClearPreviewPokepicScale
	.public BallCapsuleEditor_GrowPreviewPokepic
	.public BallCapsuleEditor_ShrinkPreviewPokepic
	.public BallCapsuleEditor_SetPreviewPokepicHidden
	.public BallCapsuleEditor_DeletePreviewPokepic
	.public BallCapsuleEditor_ResetBlend
	.public BallCapsuleEditor_InitHitboxes
	.public BallCapsuleEditor_ClearCapsuleMon
	.public sBallCapsuleEditorGraphicsBanks
	.public sBallCapsuleEditorMainBgTemplates
	.public sBallCapsuleEditorSubBgTemplates
	.public sBallCapsuleEditorMenuTemplate
	.public sBallCapsuleEditorOamCharTransferParam
	.public sBallCapsuleEditorSpriteResourceCounts
	.public sBallCapsuleEditorOamManagerParam
	.public sBallCapsuleEditorCaseSealPositions
	.public sBallCapsuleEditorWindowFrameTiles
	.public sBallCapsuleEditorArrows
	.public sBallCapsuleEditorCaseButtonPositions
	.public sBallCapsuleEditorCaseButtonHitboxSizes
	.public sBallCapsuleEditorExitButtonHitboxes
	.public sBallCapsuleEditorModes
	.public sBallCapsuleEditorMenuItems
	.public sBallCapsuleEditorHitboxes
	.include "overlay_57.inc"
	.include "global.inc"

	.text
	.public BallCapsuleEditor_LoadBattleOverlays
	.public BallCapsuleEditor_DisableInterrupts
	.public BallCapsuleEditor_GetArgsCapsuleIndex
	.public BallCapsuleEditor_SetArgsCapsuleIndex
	.public BallCapsuleEditor_SetArgsSetCapsuleOnMon
	.public BallCapsuleEditor_SetSealHitbox
	.public BallCapsuleEditor_SetDragHitbox
	.public BallCapsuleEditor_HasFreeSealSlot
	.public BallCapsuleEditor_RemoveMessageWindows
	.public BallCapsuleEditor_SetButtonLabelsDrawFlag
	.public BallCapsuleEditor_SetArrowsDrawFlag
	.public BallCapsuleEditor_DeleteArrows
	.public BallCapsuleEditor_ShowScreens
	.public BallCapsuleEditor_FadeIn
	.public BallCapsuleEditor_FadeOut
	.public BallCapsuleEditor_Noop
	.public BallCapsuleEditor_ModeNone
	.public BallCapsuleEditor_MenuEdit
	.public BallCapsuleEditor_MenuQuit
	.public BallCapsuleEditor_MenuRemove
	.public BallCapsuleEditor_MenuSwitch
	.public BallCapsuleEditor_SaveEditedCapsule
	.public BallCapsuleEditor_RestoreEditedCapsule
	.public ov57_0223B940
	.public BallCapsuleEditor_SetInputEnabled
	.public BallCapsuleEditor_SetPreviewPokepicHidden
	.public BallCapsuleEditor_DeletePreviewPokepic

	thumb_func_start BallCapsuleEditor_LoadSpriteResources
BallCapsuleEditor_LoadSpriteResources: ; 0x0223927C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	add r7, r2, #0
	mov r2, #1
	str r2, [sp]
	ldr r2, [sp, #0x3c]
	ldr r4, [sp, #0x48]
	str r2, [sp, #4]
	ldr r2, _022392F0 ; =0x00002AF8
	add r5, r0, #0
	add r2, r3, r2
	str r2, [sp, #8]
	add r2, r4, #0
	add r6, r1, #0
	bl SpriteSystem_LoadCharResObjFromOpenNarc
	ldr r0, [sp, #0x30]
	str r4, [sp]
	str r0, [sp, #4]
	mov r1, #0
	str r1, [sp, #8]
	ldr r1, [sp, #0x44]
	add r2, r5, #0
	str r1, [sp, #0xc]
	ldr r1, [sp, #0x3c]
	add r3, r6, #0
	str r1, [sp, #0x10]
	ldr r1, _022392F0 ; =0x00002AF8
	add r0, r0, r1
	str r0, [sp, #0x14]
	ldr r1, [sp, #0x40]
	add r0, r7, #0
	bl SpriteSystem_LoadPaletteBufferFromOpenNarc
	mov r0, #1
	str r0, [sp]
	ldr r3, [sp, #0x34]
	ldr r0, _022392F0 ; =0x00002AF8
	add r1, r6, #0
	add r0, r3, r0
	str r0, [sp, #4]
	add r0, r5, #0
	add r2, r4, #0
	bl SpriteSystem_LoadCellResObjFromOpenNarc
	mov r0, #1
	str r0, [sp]
	ldr r3, [sp, #0x38]
	ldr r0, _022392F0 ; =0x00002AF8
	add r1, r6, #0
	add r0, r3, r0
	str r0, [sp, #4]
	add r0, r5, #0
	add r2, r4, #0
	bl SpriteSystem_LoadAnimResObjFromOpenNarc
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_022392F0: .word 0x00002AF8
	thumb_func_end BallCapsuleEditor_LoadSpriteResources


	thumb_func_start BallCapsuleEditor_CreateCapsuleIcons
BallCapsuleEditor_CreateCapsuleIcons: ; 0x022392F4
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x58
	add r5, r0, #0
	mov r0, #3
	str r0, [sp]
	mov r0, #0x13
	str r0, [sp, #4]
	mov r0, #0x12
	add r4, r1, #0
	str r0, [sp, #8]
	mov r1, #1
	add r2, r5, #0
	str r1, [sp, #0xc]
	mov r0, #2
	str r0, [sp, #0x10]
	str r1, [sp, #0x14]
	add r0, r5, #0
	add r1, r5, #0
	str r4, [sp, #0x18]
	add r0, #0xdc
	add r1, #0xe0
	add r2, #0xe8
	ldr r0, [r0]
	ldr r1, [r1]
	ldr r2, [r2]
	mov r3, #0x11
	bl BallCapsuleEditor_LoadSpriteResources
	mov r0, #5
	str r0, [sp]
	mov r0, #0x22
	str r0, [sp, #4]
	mov r0, #0x21
	str r0, [sp, #8]
	mov r1, #1
	add r2, r5, #0
	str r1, [sp, #0xc]
	mov r0, #2
	str r0, [sp, #0x10]
	str r1, [sp, #0x14]
	add r0, r5, #0
	add r1, r5, #0
	str r4, [sp, #0x18]
	add r0, #0xdc
	add r1, #0xe0
	add r2, #0xe8
	ldr r0, [r0]
	ldr r1, [r1]
	ldr r2, [r2]
	mov r3, #0x20
	bl BallCapsuleEditor_LoadSpriteResources
	mov r6, #0
	add r4, r5, #0
	add r7, sp, #0x1c
_02239362:
	mov r0, #0
	strh r0, [r7, #8]
	strh r0, [r7, #0xa]
	strh r0, [r7, #0xc]
	mov r0, #0x29
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	add r1, r5, #0
	strh r0, [r7, #0xe]
	mov r0, #0x28
	sub r0, r0, r6
	str r0, [sp, #0x2c]
	mov r0, #0
	str r0, [sp, #0x30]
	mov r0, #1
	str r0, [sp, #0x34]
	mov r0, #2
	str r0, [sp, #0x50]
	mov r0, #0
	str r0, [sp, #0x54]
	ldr r0, _022394A0 ; =0x00002B09
	add r1, #0xe0
	str r0, [sp, #0x38]
	sub r0, #0xe
	str r0, [sp, #0x3c]
	ldr r0, _022394A4 ; =0x00002B0B
	add r2, sp, #0x24
	str r0, [sp, #0x40]
	sub r0, r0, #1
	str r0, [sp, #0x44]
	mov r0, #0
	mvn r0, r0
	str r0, [sp, #0x48]
	str r0, [sp, #0x4c]
	add r0, r5, #0
	add r0, #0xdc
	ldr r0, [r0]
	ldr r1, [r1]
	bl SpriteSystem_NewSprite
	mov r1, #0xa5
	lsl r1, r1, #2
	str r0, [r4, r1]
	add r1, sp, #0x20
	add r0, r6, #0
	add r1, #2
	add r2, sp, #0x20
	bl BallCapsuleEditor_GetCapsulePosition
	mov r0, #0xa5
	mov r1, #6
	mov r2, #4
	lsl r0, r0, #2
	ldrsh r1, [r7, r1]
	ldrsh r2, [r7, r2]
	ldr r0, [r4, r0]
	bl ManagedSprite_SetPositionXY
	add r6, r6, #1
	add r4, #0xc
	cmp r6, #0xc
	blt _02239362
	mov r1, #0
	add r0, sp, #0x1c
	strh r1, [r0, #8]
	strh r1, [r0, #0xa]
	strh r1, [r0, #0xc]
	strh r1, [r0, #0xe]
	mov r0, #0x14
	str r0, [sp, #0x2c]
	mov r0, #1
	str r1, [sp, #0x30]
	str r1, [sp, #0x54]
	ldr r1, _022394A8 ; =0x00002B18
	str r0, [sp, #0x34]
	add r0, r1, #0
	sub r0, #0x1b
	str r0, [sp, #0x3c]
	add r0, r1, #2
	mov r2, #2
	str r0, [sp, #0x40]
	add r0, r1, #1
	str r0, [sp, #0x44]
	sub r0, r2, #3
	str r1, [sp, #0x38]
	add r1, r5, #0
	str r2, [sp, #0x50]
	str r0, [sp, #0x48]
	str r0, [sp, #0x4c]
	add r0, r5, #0
	add r0, #0xdc
	add r1, #0xe0
	ldr r0, [r0]
	ldr r1, [r1]
	add r2, sp, #0x24
	bl SpriteSystem_NewSprite
	mov r1, #0xc7
	lsl r1, r1, #2
	str r0, [r5, r1]
	add r0, r5, #0
	add r1, r5, #0
	add r0, #0xdc
	add r1, #0xe0
	ldr r0, [r0]
	ldr r1, [r1]
	add r2, sp, #0x24
	bl SpriteSystem_NewSprite
	mov r1, #0x32
	lsl r1, r1, #4
	str r0, [r5, r1]
	add r1, #0xcc
	ldr r0, [r5, r1]
	add r1, sp, #0x1c
	add r1, #2
	add r2, sp, #0x1c
	bl BallCapsuleEditor_GetCapsulePosition
	mov r0, #0xc7
	lsl r0, r0, #2
	add r3, sp, #0x1c
	mov r1, #2
	mov r2, #0
	ldrsh r1, [r3, r1]
	ldrsh r2, [r3, r2]
	ldr r0, [r5, r0]
	bl ManagedSprite_SetPositionXY
	mov r0, #0x32
	lsl r0, r0, #4
	add r3, sp, #0x1c
	mov r1, #2
	mov r2, #0
	ldrsh r1, [r3, r1]
	ldrsh r2, [r3, r2]
	ldr r0, [r5, r0]
	bl ManagedSprite_SetPositionXY
	mov r0, #0x32
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	mov r1, #0x19
	bl ManagedSprite_SetDrawPriority
	mov r0, #0xc7
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0
	bl ManagedSprite_SetAnim
	mov r0, #0x32
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	mov r1, #0
	bl ManagedSprite_SetAnim
	add sp, #0x58
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_022394A0: .word 0x00002B09
_022394A4: .word 0x00002B0B
_022394A8: .word 0x00002B18
	thumb_func_end BallCapsuleEditor_CreateCapsuleIcons


	thumb_func_start BallCapsuleEditor_RefreshCapsuleIcons
BallCapsuleEditor_RefreshCapsuleIcons: ; 0x022394AC
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	bl BallCapsuleEditor_UpdateCapsuleIconStates
	add r0, r5, #0
	bl BallCapsuleEditor_PlaceMonIcons
	mov r7, #0x29
	lsl r7, r7, #4
	mov r4, #0
	add r6, r7, #4
_022394C2:
	ldr r0, [r5, r6]
	cmp r0, #0
	beq _022394CE
	ldr r1, [r5, r7]
	bl ManagedSprite_SetAnim
_022394CE:
	add r4, r4, #1
	add r5, #0xc
	cmp r4, #0xc
	blt _022394C2
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end BallCapsuleEditor_RefreshCapsuleIcons


	thumb_func_start BallCapsuleEditor_SwitchCapsules
BallCapsuleEditor_SwitchCapsules: ; 0x022394D8
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x28
	add r5, r0, #0
	lsl r6, r1, #3
	add r0, r5, r6
	ldr r0, [r0, #4]
	lsl r4, r2, #3
	str r1, [sp]
	add r1, r5, r4
	ldr r7, [r1, #4]
	cmp r0, #0xff
	beq _02239504
	add r1, r2, #1
	str r1, [sp, #0xc]
	ldr r1, [r5]
	lsl r0, r0, #2
	add r0, r1, r0
	ldr r0, [r0, #4]
	mov r1, #0xa2
	add r2, sp, #0xc
	bl SetMonData
_02239504:
	cmp r7, #0xff
	beq _0223951E
	ldr r0, [sp]
	add r2, sp, #8
	add r0, r0, #1
	str r0, [sp, #8]
	ldr r1, [r5]
	lsl r0, r7, #2
	add r0, r1, r0
	ldr r0, [r0, #4]
	mov r1, #0xa2
	bl SetMonData
_0223951E:
	add r2, r5, #4
	ldr r1, [r2, r6]
	ldr r0, [r2, r4]
	add r7, r5, #0
	str r0, [r2, r6]
	str r1, [r2, r4]
	add r7, #8
	ldr r0, [r7, r6]
	add r1, sp, #0x10
	bl CopyCapsule
	add r0, r5, #0
	str r0, [sp, #4]
	add r0, #8
	str r0, [sp, #4]
	ldr r0, [r0, r4]
	ldr r1, [r7, r6]
	bl CopyCapsule
	ldr r1, [sp, #4]
	add r0, sp, #0x10
	ldr r1, [r1, r4]
	bl CopyCapsule
	add r0, r5, #0
	bl BallCapsuleEditor_RefreshCapsuleIcons
	add sp, #0x28
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end BallCapsuleEditor_SwitchCapsules
