	.include "asm/macros.inc"
	.include "overlay_68.inc"
	.include "global.inc"

	.text
	.public MoveRelearner_Exit
	.public MoveRelearner_Init
	.public MoveRelearner_Main
	.public sMoveRelearnerSummaryPages
	.public MoveRelearner_Setup
	.public MoveRelearner_SetGfxBanks
	.public MoveRelearner_InitBgs
	.public MoveRelearner_LoadBgGraphics
	.public MoveRelearner_AddWindows
	.public MoveRelearner_HandleInput
	.public MoveRelearner_ShowYesNo
	.public MoveRelearner_HandleYesNo
	.public MoveRelearner_TeachMove
	.public MoveRelearner_AskAfterSummary
	.public MoveRelearner_PrintString
	.public MoveRelearner_FormatNumber
	.public MoveRelearner_PrintStaticTexts
	.public MoveRelearner_CopyButtonTiles
	.public MoveRelearner_PrintListRow
	.public MoveRelearner_PrintMoveList
	.public MoveRelearner_InitMoveList
	.public MoveRelearner_PrintMoveInfo
	.public MoveRelearner_FormatMessage
	.public MoveRelearner_PrintMessage
	.public MoveRelearner_InitSpriteSystem
	.public MoveRelearner_LoadSpriteResources
	.public MoveRelearner_SetTypeIcon
	.public MoveRelearner_UpdateListTypeIcons
	.public MoveRelearner_SetCategoryIcon
	.public MoveRelearner_InitSprites
	.public MoveRelearner_CreateCamera
	.public MoveRelearner_CreatePokepic
	.public MoveRelearner_Init3D
	.public MoveRelearner_Draw3D
	.public MoveRelearner_SetCursorPos
	.public MoveRelearner_UpdateScrollArrows
	.public MoveRelearner_OpenSummary
	.public MoveRelearner_RunSummary
	.public MoveRelearner_CreateGridInput
	.public MoveRelearner_GridOnKeyMove
	.public MoveRelearner_GridOnTouch
	.public MoveRelearner_SetLearnButtonActive
	.public MoveRelearner_UpdateScrollButtons
	.public MoveRelearner_AnimateScrollArrows
	.public MoveRelearner_DrawListRows
	.public MoveRelearner_UpdateButtonFlash
	.public MoveRelearner_DimSubScreen
	.public sMoveRelearnerCameraPos
	.public sMoveRelearnerGraphicsModes
	.public sMoveRelearnerGridCallbacks
	.public sMoveRelearnerOamCharTransferParam
	.public sMoveRelearnerSpriteResourceCounts
	.public sMoveRelearnerBgTemplate_Main3
	.public sMoveRelearnerBgTemplate_Main2
	.public sMoveRelearnerBgTemplate_Sub0
	.public sMoveRelearnerBgTemplate_Sub2
	.public sMoveRelearnerBgTemplate_Sub3
	.public sMoveRelearnerOamManagerParam
	.public sMoveRelearnerGridHitboxes
	.public sMoveRelearnerGraphicsBanks
	.public sMoveRelearnerYesActions
	.public sMoveRelearnerNoActions
	.public sMoveRelearnerGridDpadBoxes
	.public sMoveRelearnerMessageIds
	.public sMoveRelearnerWindowTemplates
	.public sMoveRelearnerSpriteTemplates
	.public MoveRelearner_Teardown
	.public MoveRelearner_VBlankCB
	.public MoveRelearner_FreeBgs
	.public MoveRelearner_FreeScreenFile
	.public MoveRelearner_RemoveWindows
	.public MoveRelearner_InitStrings
	.public MoveRelearner_FreeStrings
	.public MoveRelearner_WaitFade
	.public MoveRelearner_WaitMessage
	.public MoveRelearner_PrintLearned
	.public MoveRelearner_FadeOut
	.public MoveRelearner_PrintSelectedMove
	.public MoveRelearner_CountEligibleMoves
	.public MoveRelearner_FreeMoveList
	.public MoveRelearner_GetSelectedMove
	.public MoveRelearner_GetMoveToReplace
	.public MoveRelearner_ClearMessage
	.public MoveRelearner_TextPrinterCallback
	.public MoveRelearner_CountKnownMoves
	.public MoveRelearner_OnTeachYes
	.public MoveRelearner_OnTeachNo
	.public MoveRelearner_OnGiveUpYes
	.public MoveRelearner_OnGiveUpNo
	.public MoveRelearner_OnDeleteMoveYes
	.public MoveRelearner_OnDeleteMoveNo
	.public MoveRelearner_OnStopTeachingYes
	.public MoveRelearner_OnStopTeachingNo
	.public MoveRelearner_OnForgetMoveYes
	.public MoveRelearner_OnForgetMoveNo
	.public MoveRelearner_FreeSprites
	.public MoveRelearner_Free3D
	.public MoveRelearner_FadeOutToSummary
	.public MoveRelearner_FreeGridInput
	.public MoveRelearner_GridNoop
	.public MoveRelearner_StartReturnButtonFlash
	.public MoveRelearner_StartLearnButtonFlash
	.public MoveRelearner_AskGiveUp
	.public MoveRelearner_AskTeachAfterFlash
	.public MoveRelearner_AskTeach

	thumb_func_start MoveRelearner_OpenSummary
MoveRelearner_OpenSummary: ; 0x021E74D8
	push {r3, r4, lr}
	sub sp, #4
	ldr r2, _021E7560 ; =sMoveRelearnerSummaryPages
	add r1, sp, #0
	ldrb r3, [r2]
	ldrb r2, [r2, #1]
	add r4, r0, #0
	strb r3, [r1]
	strb r2, [r1, #1]
	bl MoveRelearner_Teardown
	ldr r0, [r4]
	mov r1, #0x17
	ldr r0, [r0]
	lsl r1, r1, #4
	str r0, [r4, r1]
	ldr r0, [r4]
	ldr r2, [r0, #8]
	add r0, r1, #4
	str r2, [r4, r0]
	add r0, r1, #0
	mov r2, #0
	add r0, #0x11
	strb r2, [r4, r0]
	add r0, r1, #0
	add r0, #0x14
	strb r2, [r4, r0]
	mov r0, #1
	add r1, #0x13
	strb r0, [r4, r1]
	add r0, r4, #0
	bl MoveRelearner_GetSelectedMove
	mov r1, #0x62
	lsl r1, r1, #2
	strh r0, [r4, r1]
	mov r2, #2
	sub r0, r1, #6
	strb r2, [r4, r0]
	add r0, r1, #0
	mov r2, #1
	add r0, #0x14
	str r2, [r4, r0]
	add r0, r1, #0
	mov r2, #0
	add r0, #0x10
	str r2, [r4, r0]
	add r0, r1, #0
	add r0, #0x18
	sub r1, #0x18
	str r2, [r4, r0]
	add r0, r4, r1
	add r1, sp, #0
	bl sub_02089D40
	mov r1, #0x17
	lsl r1, r1, #4
	ldr r0, _021E7564 ; =gOverlayTemplate_PokemonSummary
	add r1, r4, r1
	mov r2, #0x42
	bl OverlayManager_New
	mov r1, #0x6b
	lsl r1, r1, #2
	str r0, [r4, r1]
	mov r0, #0xc
	add sp, #4
	pop {r3, r4, pc}
	.balign 4, 0
_021E7560: .word sMoveRelearnerSummaryPages
_021E7564: .word gOverlayTemplate_PokemonSummary
	thumb_func_end MoveRelearner_OpenSummary


	thumb_func_start MoveRelearner_RunSummary
MoveRelearner_RunSummary: ; 0x021E7568
	push {r4, lr}
	add r4, r0, #0
	mov r0, #0x6b
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl OverlayManager_Run
	cmp r0, #0
	beq _021E75B6
	mov r0, #0x6b
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl OverlayManager_Delete
	add r0, r4, #0
	bl MoveRelearner_Setup
	ldr r1, [r4]
	add r0, r4, #0
	ldrh r1, [r1, #0x14]
	bl MoveRelearner_DrawListRows
	ldr r1, [r4]
	add r0, r4, #0
	ldrh r1, [r1, #0x14]
	mov r2, #3
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl MoveRelearner_SetCursorPos
	ldr r0, _021E75BC ; =0x00000186
	ldr r1, [r4]
	ldrb r2, [r4, r0]
	add r0, #0x2a
	strb r2, [r1, #0x1b]
	mov r1, #7
	str r1, [r4, r0]
	mov r0, #0
	pop {r4, pc}
_021E75B6:
	mov r0, #0xc
	pop {r4, pc}
	nop
_021E75BC: .word 0x00000186
	thumb_func_end MoveRelearner_RunSummary


	thumb_func_start MoveRelearner_CreateGridInput
MoveRelearner_CreateGridInput: ; 0x021E75C0
	push {r3, r4, lr}
	sub sp, #0xc
	add r4, r0, #0
	mov r0, #1
	str r0, [sp]
	mov r0, #0
	str r0, [sp, #4]
	mov r0, #0x42
	str r0, [sp, #8]
	ldr r0, _021E75F8 ; =sMoveRelearnerGridHitboxes
	ldr r1, _021E75FC ; =sMoveRelearnerGridDpadBoxes
	ldr r2, _021E7600 ; =sMoveRelearnerGridCallbacks
	add r3, r4, #0
	bl GridInputHandler_Create
	mov r1, #0x72
	lsl r1, r1, #2
	str r0, [r4, r1]
	add r0, r4, #0
	mov r1, #0
	bl MoveRelearner_SetLearnButtonActive
	add r0, r4, #0
	bl MoveRelearner_UpdateScrollButtons
	add sp, #0xc
	pop {r3, r4, pc}
	nop
_021E75F8: .word sMoveRelearnerGridHitboxes
_021E75FC: .word sMoveRelearnerGridDpadBoxes
_021E7600: .word sMoveRelearnerGridCallbacks
	thumb_func_end MoveRelearner_CreateGridInput
