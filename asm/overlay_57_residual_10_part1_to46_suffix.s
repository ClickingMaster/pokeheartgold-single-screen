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

thumb_func_start BallCapsuleEditor_PreviewCapsule
BallCapsuleEditor_PreviewCapsule: ; 0x0223AB58
	push {r4, r5, lr}
	sub sp, #0x84
	ldr r1, _0223AE90 ; =0x00000404
	add r4, r0, #0
	ldr r2, [r4, r1]
	cmp r2, #7
	bls _0223AB68
	b _0223AE4C
_0223AB68:
	add r2, r2, r2
	add r2, pc
	ldrh r2, [r2, #6]
	lsl r2, r2, #0x10
	asr r2, r2, #0x10
	add pc, r2
_0223AB74: ; jump table
	.short _0223AB84 - _0223AB74 - 2 ; case 0
	.short _0223ABF6 - _0223AB74 - 2 ; case 1
	.short _0223AC4A - _0223AB74 - 2 ; case 2
	.short _0223ACAC - _0223AB74 - 2 ; case 3
	.short _0223ACF6 - _0223AB74 - 2 ; case 4
	.short _0223AD38 - _0223AB74 - 2 ; case 5
	.short _0223AD7A - _0223AB74 - 2 ; case 6
	.short _0223ADE2 - _0223AB74 - 2 ; case 7
_0223AB84:
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	mov r3, #0
	str r3, [sp]
	mov r0, #0xa
	str r0, [sp, #4]
	add r0, r4, #0
	str r3, [sp, #8]
	add r0, #0xe8
	ldr r0, [r0]
	ldr r2, _0223AE94 ; =0x0000080B
	mov r1, #2
	bl PaletteData_BeginPaletteFade
	mov r3, #0
	str r3, [sp]
	mov r0, #0xa
	str r0, [sp, #4]
	add r0, r4, #0
	str r3, [sp, #8]
	add r0, #0xe8
	ldr r0, [r0]
	ldr r2, _0223AE98 ; =0x0000FFFF
	mov r1, #8
	bl PaletteData_BeginPaletteFade
	add r0, r4, #0
	mov r1, #0
	bl BallCapsuleEditor_SetInputEnabled
	ldr r0, _0223AE9C ; =0x00000408
	mov r1, #0
	str r1, [r4, r0]
	add r0, r4, #0
	bl BallCapsuleEditor_CopyPreviewMon
	add r0, r4, #0
	bl BallCapsuleEditor_CreatePreviewPokepic
	add r0, r4, #0
	mov r1, #1
	bl BallCapsuleEditor_SetPreviewPokepicHidden
	add r0, r4, #0
	mov r1, #0
	add r0, #0xec
	mov r2, #0xd
	add r3, r1, #0
	bl BallCapsuleEditor_PrintMessage
	ldr r0, _0223AE90 ; =0x00000404
	ldr r1, [r4, r0]
	add r1, r1, #1
	str r1, [r4, r0]
	b _0223AE88
_0223ABF6:
	add r0, #0xe8
	ldr r0, [r0]
	bl PaletteData_GetSelectedBuffersBitmask
	cmp r0, #0
	beq _0223AC04
	b _0223AE88
_0223AC04:
	mov r0, #0x34
	str r0, [sp, #0x60]
	mov r1, #1
	mov r2, #0
	mov r0, #0xff
	str r0, [sp, #0x68]
	add r0, r4, #0
	str r1, [sp, #0x5c]
	str r1, [sp, #0x74]
	str r2, [sp, #0x64]
	str r2, [sp, #0x70]
	add r0, #0xdc
	ldr r0, [r0]
	mov r1, #0x9b
	str r0, [sp, #0x78]
	add r0, r4, #0
	add r0, #0xe8
	ldr r0, [r0]
	str r0, [sp, #0x7c]
	ldr r0, _0223AEA0 ; =0x00000458
	ldr r0, [r4, r0]
	bl GetMonData
	str r0, [sp, #0x6c]
	add r0, sp, #0x5c
	bl ov07_02233DB8
	mov r1, #0x95
	lsl r1, r1, #2
	str r0, [r4, r1]
	ldr r0, _0223AE90 ; =0x00000404
	ldr r1, [r4, r0]
	add r1, r1, #1
	str r1, [r4, r0]
	b _0223AE88
_0223AC4A:
	add r2, sp, #0x38
	mov r0, #0
	str r0, [r2]
	str r0, [r2, #4]
	str r0, [r2, #8]
	mov r0, #1
	str r0, [sp, #0x38]
	add r0, r1, #0
	add r0, #0x54
	ldr r0, [r4, r0]
	sub r1, #0x18
	str r0, [sp, #0x40]
	ldr r0, [r4, r1]
	add r1, r4, #0
	add r0, r0, #1
	str r0, [sp, #0xc]
	add r0, sp, #0x44
	bl BallCapsuleEditor_GetSealData
	ldr r0, _0223AEA0 ; =0x00000458
	mov r1, #0xa2
	ldr r0, [r4, r0]
	add r2, sp, #0xc
	bl SetMonData
	ldr r0, _0223AEA0 ; =0x00000458
	mov r1, #0xab
	ldr r0, [r4, r0]
	add r2, sp, #0x44
	bl SetMonData
	mov r0, #0x34
	add r1, sp, #0x38
	bl ov07_02232694
	mov r1, #0x25
	lsl r1, r1, #4
	str r0, [r4, r1]
	ldr r0, [r4, r1]
	bl ov07_022329B0
	ldr r0, _0223AE90 ; =0x00000404
	ldr r1, [r4, r0]
	add r1, r1, #1
	str r1, [r4, r0]
	mov r1, #0
	add r0, r0, #4
	str r1, [r4, r0]
	b _0223AE88
_0223ACAC:
	mov r0, #0x25
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	bl ov07_02232A04
	cmp r0, #1
	bne _0223AD64
	mov r0, #0x95
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl ov07_02233EA0
	cmp r0, #1
	bne _0223AD64
	add r0, r4, #0
	bl BallCapsuleEditor_GrowPreviewPokepic
	mov r0, #0x25
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	bl ov07_02232A44
	add r0, r4, #0
	mov r1, #0
	bl BallCapsuleEditor_SetPreviewPokepicHidden
	add r0, r4, #0
	bl BallCapsuleEditor_ClearPreviewPokepicScale
	ldr r0, _0223AEA4 ; =0x000006C5
	bl PlaySE
	ldr r0, _0223AE90 ; =0x00000404
	ldr r1, [r4, r0]
	add r1, r1, #1
	str r1, [r4, r0]
	b _0223AE88
_0223ACF6:
	bl BallCapsuleEditor_GrowPreviewPokepic
	add r5, r0, #0
	mov r0, #0x95
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl ov07_02233E88
	cmp r0, #0
	bne _0223AD64
	cmp r5, #0
	bne _0223AD64
	mov r0, #0x95
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl ov07_02233ECC
	mov r0, #0x10
	mov r1, #0
	bl GfGfx_EngineATogglePlanes
	add r0, r4, #0
	bl BallCapsuleEditor_StartPreviewPokepicAnim
	ldr r0, _0223AE9C ; =0x00000408
	mov r1, #0
	str r1, [r4, r0]
	sub r1, r0, #4
	ldr r1, [r4, r1]
	sub r0, r0, #4
	add r1, r1, #1
	str r1, [r4, r0]
	b _0223AE88
_0223AD38:
	mov r0, #0x25
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	bl ov07_02232A54
	cmp r0, #0
	bne _0223AD64
	mov r0, #0xa1
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #0
	bl PokepicAnimManager_GetAnimState
	cmp r0, #1
	bne _0223AD64
	mov r0, #0x72
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl Pokepic_IsAnimFinished
	cmp r0, #0
	beq _0223AD66
_0223AD64:
	b _0223AE88
_0223AD66:
	mov r0, #0x25
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	bl ov07_02232AB8
	ldr r0, _0223AE90 ; =0x00000404
	ldr r1, [r4, r0]
	add r1, r1, #1
	str r1, [r4, r0]
	b _0223AE88
_0223AD7A:
	add r0, r1, #4
	ldr r0, [r4, r0]
	add r2, r0, #1
	add r0, r1, #4
	str r2, [r4, r0]
	ldr r0, [r4, r0]
	cmp r0, #0x1e
	blt _0223AE88
	add r0, r1, #4
	mov r2, #0
	str r2, [r4, r0]
	mov r0, #0x34
	mov r3, #1
	str r0, [sp, #0x14]
	mov r0, #5
	str r0, [sp, #0x18]
	mov r0, #0xff
	str r0, [sp, #0x1c]
	add r0, r4, #0
	str r3, [sp, #0x10]
	str r2, [sp, #0x24]
	str r3, [sp, #0x28]
	add r0, #0xdc
	ldr r0, [r0]
	add r1, #0x54
	str r0, [sp, #0x2c]
	add r0, r4, #0
	add r0, #0xe8
	ldr r0, [r0]
	str r0, [sp, #0x30]
	ldr r0, [r4, r1]
	mov r1, #0x9b
	bl GetMonData
	str r0, [sp, #0x20]
	add r0, sp, #0x10
	bl ov07_02233DB8
	mov r1, #0x95
	lsl r1, r1, #2
	str r0, [r4, r1]
	bl BallCapsuleEditor_ResetBlend
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	ldr r0, _0223AE90 ; =0x00000404
	ldr r1, [r4, r0]
	add r1, r1, #1
	str r1, [r4, r0]
	b _0223AE88
_0223ADE2:
	bl BallCapsuleEditor_ShrinkPreviewPokepic
	add r5, r0, #0
	mov r0, #0x95
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl ov07_02233E88
	cmp r0, #0
	bne _0223AE88
	cmp r5, #0
	bne _0223AE88
	mov r0, #0xa
	str r0, [sp]
	mov r3, #0
	str r3, [sp, #4]
	add r0, r4, #0
	str r3, [sp, #8]
	add r0, #0xe8
	ldr r0, [r0]
	ldr r2, _0223AE94 ; =0x0000080B
	mov r1, #2
	bl PaletteData_BeginPaletteFade
	mov r0, #0xa
	str r0, [sp]
	mov r3, #0
	str r3, [sp, #4]
	add r0, r4, #0
	str r3, [sp, #8]
	add r0, #0xe8
	ldr r0, [r0]
	ldr r2, _0223AE98 ; =0x0000FFFF
	mov r1, #8
	bl PaletteData_BeginPaletteFade
	add r0, r4, #0
	mov r1, #1
	bl BallCapsuleEditor_SetPreviewPokepicHidden
	add r0, r4, #0
	bl BallCapsuleEditor_DeletePreviewPokepic
	mov r0, #0x95
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl ov07_02233ECC
	ldr r0, _0223AE90 ; =0x00000404
	ldr r1, [r4, r0]
	add r1, r1, #1
	str r1, [r4, r0]
	b _0223AE88
_0223AE4C:
	add r0, r4, #0
	add r0, #0xe8
	ldr r0, [r0]
	bl PaletteData_GetSelectedBuffersBitmask
	cmp r0, #0
	bne _0223AE88
	mov r0, #0x10
	mov r1, #0
	bl GfGfx_EngineATogglePlanes
	add r0, r4, #0
	mov r1, #1
	bl BallCapsuleEditor_SetInputEnabled
	mov r1, #0
	ldr r0, _0223AE90 ; =0x00000404
	mov r2, #4
	str r1, [r4, r0]
	sub r0, #8
	str r2, [r4, r0]
	add r4, #0xec
	ldr r2, _0223AE98 ; =0x0000FFFF
	add r0, r4, #0
	add r3, r1, #0
	bl BallCapsuleEditor_PrintMessage
	add sp, #0x84
	mov r0, #4
	pop {r4, r5, pc}
_0223AE88:
	mov r0, #5
	add sp, #0x84
	pop {r4, r5, pc}
	nop
_0223AE90: .word 0x00000404
_0223AE94: .word 0x0000080B
_0223AE98: .word 0x0000FFFF
_0223AE9C: .word 0x00000408
_0223AEA0: .word 0x00000458
_0223AEA4: .word 0x000006C5
	thumb_func_end BallCapsuleEditor_PreviewCapsule


	thumb_func_start BallCapsuleEditor_HitboxCallback
BallCapsuleEditor_HitboxCallback: ; 0x0223AEA8
	push {r4, r5, r6, lr}
	add r4, r2, #0
	add r5, r0, #0
	add r0, r4, #0
	add r0, #0xd8
	ldr r0, [r0]
	add r6, r1, #0
	cmp r0, #0
	bne _0223AEBC
	b _0223B100
_0223AEBC:
	cmp r5, #0x14
	bls _0223AEC2
	b _0223B0FC
_0223AEC2:
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0223AECE: ; jump table
	.short _0223B036 - _0223AECE - 2 ; case 0
	.short _0223B036 - _0223AECE - 2 ; case 1
	.short _0223B036 - _0223AECE - 2 ; case 2
	.short _0223B036 - _0223AECE - 2 ; case 3
	.short _0223B036 - _0223AECE - 2 ; case 4
	.short _0223B036 - _0223AECE - 2 ; case 5
	.short _0223B036 - _0223AECE - 2 ; case 6
	.short _0223B036 - _0223AECE - 2 ; case 7
	.short _0223AEF8 - _0223AECE - 2 ; case 8
	.short _0223AF42 - _0223AECE - 2 ; case 9
	.short _0223AF90 - _0223AECE - 2 ; case 10
	.short _0223AFBE - _0223AECE - 2 ; case 11
	.short _0223AFFC - _0223AECE - 2 ; case 12
	.short _0223B0CC - _0223AECE - 2 ; case 13
	.short _0223B0CC - _0223AECE - 2 ; case 14
	.short _0223B0CC - _0223AECE - 2 ; case 15
	.short _0223B0CC - _0223AECE - 2 ; case 16
	.short _0223B0CC - _0223AECE - 2 ; case 17
	.short _0223B0CC - _0223AECE - 2 ; case 18
	.short _0223B0CC - _0223AECE - 2 ; case 19
	.short _0223B0CC - _0223AECE - 2 ; case 20
_0223AEF8:
	cmp r6, #0
	bne _0223AF36
	ldr r1, _0223B104 ; =0x00000448
	ldr r0, [r4, r1]
	cmp r0, #0
	ble _0223AF06
	b _0223AF0A
_0223AF06:
	add r0, r1, #4
	ldr r0, [r4, r0]
_0223AF0A:
	sub r0, r0, #1
	str r0, [r4, r1]
	add r0, r4, #0
	bl BallCapsuleEditor_DeleteCaseSealSprites
	ldr r1, _0223B104 ; =0x00000448
	add r0, r4, #0
	ldr r1, [r4, r1]
	bl BallCapsuleEditor_FillPageSeals
	add r0, r4, #0
	bl BallCapsuleEditor_LoadCaseSealSpriteResources
	add r0, r4, #0
	bl BallCapsuleEditor_CreateCaseSealSprites
	add r0, r4, #0
	bl BallCapsuleEditor_PrintSealCounts
	ldr r0, _0223B108 ; =0x000006C4
	bl PlaySE
_0223AF36:
	ldr r0, _0223B10C ; =0x00000434
	add r1, r6, #0
	ldr r0, [r4, r0]
	bl BallCapsuleEditor_SetButtonFrame
	pop {r4, r5, r6, pc}
_0223AF42:
	cmp r6, #0
	bne _0223AF84
	ldr r1, _0223B104 ; =0x00000448
	ldr r0, [r4, r1]
	add r0, r0, #1
	str r0, [r4, r1]
	ldr r0, [r4, r1]
	add r1, r1, #4
	ldr r1, [r4, r1]
	bl _s32_div_f
	ldr r0, _0223B104 ; =0x00000448
	str r1, [r4, r0]
	add r0, r4, #0
	bl BallCapsuleEditor_DeleteCaseSealSprites
	ldr r1, _0223B104 ; =0x00000448
	add r0, r4, #0
	ldr r1, [r4, r1]
	bl BallCapsuleEditor_FillPageSeals
	add r0, r4, #0
	bl BallCapsuleEditor_LoadCaseSealSpriteResources
	add r0, r4, #0
	bl BallCapsuleEditor_CreateCaseSealSprites
	add r0, r4, #0
	bl BallCapsuleEditor_PrintSealCounts
	ldr r0, _0223B108 ; =0x000006C4
	bl PlaySE
_0223AF84:
	ldr r0, _0223B110 ; =0x00000438
	add r1, r6, #0
	ldr r0, [r4, r0]
	bl BallCapsuleEditor_SetButtonFrame
	pop {r4, r5, r6, pc}
_0223AF90:
	cmp r6, #0
	bne _0223AFB2
	mov r0, #0xff
	lsl r0, r0, #2
	ldr r1, [r4, r0]
	cmp r1, #5
	beq _0223AFA8
	mov r1, #5
	str r1, [r4, r0]
	ldr r0, _0223B114 ; =0x000005DD
	bl PlaySE
_0223AFA8:
	ldr r0, _0223B118 ; =0x0000043C
	mov r1, #0
	ldr r0, [r4, r0]
	bl BallCapsuleEditor_StartButtonPress
_0223AFB2:
	ldr r0, _0223B118 ; =0x0000043C
	add r1, r6, #0
	ldr r0, [r4, r0]
	bl BallCapsuleEditor_SetButtonFrame
	pop {r4, r5, r6, pc}
_0223AFBE:
	cmp r6, #0
	bne _0223AFEE
	mov r0, #0xff
	lsl r0, r0, #2
	ldr r1, [r4, r0]
	cmp r1, #6
	beq _0223AFDE
	mov r1, #6
	str r1, [r4, r0]
	add r0, r4, #0
	mov r1, #0
	bl BallCapsuleEditor_SetInputEnabled
	ldr r0, _0223B11C ; =0x000005E2
	bl PlaySE
_0223AFDE:
	mov r0, #0x11
	mov r1, #0x26
	lsl r0, r0, #6
	lsl r1, r1, #4
	ldr r0, [r4, r0]
	ldr r1, [r4, r1]
	bl BallCapsuleEditor_StartButtonPress
_0223AFEE:
	mov r0, #0x11
	lsl r0, r0, #6
	ldr r0, [r4, r0]
	add r1, r6, #0
	bl BallCapsuleEditor_SetButtonFrame
	pop {r4, r5, r6, pc}
_0223AFFC:
	cmp r6, #0
	bne _0223B02A
	mov r0, #0xff
	lsl r0, r0, #2
	ldr r1, [r4, r0]
	cmp r1, #7
	beq _0223B01C
	mov r1, #7
	str r1, [r4, r0]
	add r0, r4, #0
	mov r1, #0
	bl BallCapsuleEditor_SetInputEnabled
	ldr r0, _0223B114 ; =0x000005DD
	bl PlaySE
_0223B01C:
	ldr r0, _0223B120 ; =0x00000444
	mov r1, #0x99
	lsl r1, r1, #2
	ldr r0, [r4, r0]
	ldr r1, [r4, r1]
	bl BallCapsuleEditor_StartButtonPress
_0223B02A:
	ldr r0, _0223B120 ; =0x00000444
	add r1, r6, #0
	ldr r0, [r4, r0]
	bl BallCapsuleEditor_SetButtonFrame
	pop {r4, r5, r6, pc}
_0223B036:
	cmp r6, #0
	bne _0223B100
	add r0, r4, #0
	bl BallCapsuleEditor_HasFreeSealSlot
	cmp r0, #0
	bne _0223B05A
	ldr r0, _0223B124 ; =0x000005F2
	bl PlaySE
	add r4, #0xec
	mov r1, #0
	add r0, r4, #0
	mov r2, #0xf
	add r3, r1, #0
	bl BallCapsuleEditor_PrintMessage
	pop {r4, r5, r6, pc}
_0223B05A:
	mov r0, #0x45
	lsl r0, r0, #4
	add r6, r4, r0
	ldrb r1, [r6, r5]
	cmp r1, #0
	beq _0223B0B0
	ldr r0, [r4, #0x64]
	sub r1, r1, #1
	bl SealCaseInventory_GetSealQuantity
	cmp r0, #0
	beq _0223B0B0
	lsl r1, r5, #0x18
	add r0, r4, #0
	lsr r1, r1, #0x18
	bl BallCapsuleEditor_AddSeal
	add r1, r4, #0
	add r1, #0xd4
	str r0, [r1]
	ldrb r0, [r6, r5]
	bl sub_0209106C
	add r1, r0, #0
	add r0, r4, #0
	add r0, #0xec
	bl BallCapsuleEditor_PrintSealName
	ldr r0, [r4]
	mov r2, #0
	ldrb r1, [r6, r5]
	ldr r0, [r0, #0x20]
	mvn r2, r2
	bl GiveOrTakeSeal
	add r0, r4, #0
	add r1, r5, #0
	bl BallCapsuleEditor_PrintSealCount
	ldr r0, _0223B128 ; =0x000005EB
	bl PlaySE
	pop {r4, r5, r6, pc}
_0223B0B0:
	ldrb r0, [r6, r5]
	cmp r0, #0
	beq _0223B100
	ldr r0, _0223B124 ; =0x000005F2
	bl PlaySE
	add r4, #0xec
	mov r1, #0
	add r0, r4, #0
	mov r2, #0x10
	add r3, r1, #0
	bl BallCapsuleEditor_PrintMessage
	pop {r4, r5, r6, pc}
_0223B0CC:
	cmp r6, #0
	bne _0223B100
	sub r5, #0xd
	lsl r2, r5, #0x18
	add r0, r4, #0
	lsr r2, r2, #0x18
	bl BallCapsuleEditor_HandleSealTouch
	lsl r0, r5, #4
	add r1, r4, r0
	mov r0, #0x35
	lsl r0, r0, #4
	ldrb r0, [r1, r0]
	bl sub_0209106C
	add r4, #0xec
	add r1, r0, #0
	add r0, r4, #0
	bl BallCapsuleEditor_PrintSealName
	ldr r0, _0223B128 ; =0x000005EB
	bl PlaySE
	pop {r4, r5, r6, pc}
_0223B0FC:
	bl GF_AssertFail
_0223B100:
	pop {r4, r5, r6, pc}
	nop
_0223B104: .word 0x00000448
_0223B108: .word 0x000006C4
_0223B10C: .word 0x00000434
_0223B110: .word 0x00000438
_0223B114: .word 0x000005DD
_0223B118: .word 0x0000043C
_0223B11C: .word 0x000005E2
_0223B120: .word 0x00000444
_0223B124: .word 0x000005F2
_0223B128: .word 0x000005EB
	thumb_func_end BallCapsuleEditor_HitboxCallback
