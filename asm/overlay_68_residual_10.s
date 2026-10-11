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

	thumb_func_start MoveRelearner_PrintMoveInfo
MoveRelearner_PrintMoveInfo: ; 0x021E68D4
	push {r4, r5, r6, lr}
	sub sp, #8
	add r5, r0, #0
	add r4, r1, #0
	add r0, #0x58
	mov r1, #0
	bl FillWindowPixelBuffer
	add r0, r5, #0
	add r0, #0x38
	mov r1, #0
	bl FillWindowPixelBuffer
	add r0, r5, #0
	add r0, #0x48
	mov r1, #0
	bl FillWindowPixelBuffer
	mov r0, #0x13
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	mov r1, #0
	bl ManagedSprite_SetDrawFlag
	mov r0, #1
	mvn r0, r0
	cmp r4, r0
	beq _021E69EE
	add r0, r4, #0
	mov r1, #2
	bl GetMoveAttr
	add r2, r0, #0
	cmp r2, #1
	bhi _021E692E
	mov r1, #0x21
	add r0, r5, #0
	add r2, r1, #0
	add r0, #0xf8
	add r2, #0xdf
	ldr r0, [r0]
	ldr r2, [r5, r2]
	bl ReadMsgDataIntoString
	b _021E693C
_021E692E:
	mov r0, #0
	str r0, [sp]
	add r0, r5, #0
	mov r1, #0x1d
	mov r3, #3
	bl MoveRelearner_FormatNumber
_021E693C:
	mov r0, #1
	str r0, [sp]
	mov r2, #0
	ldr r3, _021E6A24 ; =0x00010200
	add r0, r5, #0
	mov r1, #3
	str r2, [sp, #4]
	bl MoveRelearner_PrintString
	add r0, r4, #0
	mov r1, #4
	bl GetMoveAttr
	add r2, r0, #0
	bne _021E696E
	mov r1, #0x21
	add r0, r5, #0
	add r2, r1, #0
	add r0, #0xf8
	add r2, #0xdf
	ldr r0, [r0]
	ldr r2, [r5, r2]
	bl ReadMsgDataIntoString
	b _021E697C
_021E696E:
	mov r0, #0
	str r0, [sp]
	add r0, r5, #0
	mov r1, #0x1e
	mov r3, #3
	bl MoveRelearner_FormatNumber
_021E697C:
	mov r0, #1
	str r0, [sp]
	mov r2, #0
	ldr r3, _021E6A24 ; =0x00010200
	add r0, r5, #0
	mov r1, #4
	str r2, [sp, #4]
	bl MoveRelearner_PrintString
	ldr r2, _021E6A28 ; =0x000002ED
	mov r0, #1
	mov r1, #0x1b
	mov r3, #0x42
	bl NewMsgDataFromNarc
	mov r2, #1
	lsl r2, r2, #8
	ldr r2, [r5, r2]
	add r1, r4, #0
	add r6, r0, #0
	bl ReadMsgDataIntoString
	mov r2, #0
	str r2, [sp]
	ldr r3, _021E6A24 ; =0x00010200
	add r0, r5, #0
	mov r1, #5
	str r2, [sp, #4]
	bl MoveRelearner_PrintString
	add r0, r6, #0
	bl DestroyMsgData
	lsl r1, r4, #0x10
	add r0, r5, #0
	lsr r1, r1, #0x10
	bl MoveRelearner_SetCategoryIcon
	mov r0, #0x13
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	mov r1, #1
	bl ManagedSprite_SetDrawFlag
	add r0, r5, #0
	add r0, #8
	bl ScheduleWindowCopyToVram
	add r0, r5, #0
	add r0, #0x18
	bl ScheduleWindowCopyToVram
	add r0, r5, #0
	add r0, #0x28
	bl ScheduleWindowCopyToVram
	b _021E6A06
_021E69EE:
	add r0, r5, #0
	add r0, #8
	bl ClearWindowTilemapAndScheduleTransfer
	add r0, r5, #0
	add r0, #0x18
	bl ClearWindowTilemapAndScheduleTransfer
	add r0, r5, #0
	add r0, #0x28
	bl ClearWindowTilemapAndScheduleTransfer
_021E6A06:
	add r0, r5, #0
	add r0, #0x58
	bl ScheduleWindowCopyToVram
	add r0, r5, #0
	add r0, #0x38
	bl ScheduleWindowCopyToVram
	add r5, #0x48
	add r0, r5, #0
	bl ScheduleWindowCopyToVram
	add sp, #8
	pop {r4, r5, r6, pc}
	nop
_021E6A24: .word 0x00010200
_021E6A28: .word 0x000002ED
	thumb_func_end MoveRelearner_PrintMoveInfo


	thumb_func_start MoveRelearner_FormatMessage
MoveRelearner_FormatMessage: ; 0x021E6A2C
	push {r3, r4, r5, lr}
	add r5, r1, #0
	add r4, r0, #0
	cmp r5, #0xa
	bls _021E6A38
	b _021E6BB0
_021E6A38:
	add r1, r5, r5
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_021E6A44: ; jump table
	.short _021E6A5A - _021E6A44 - 2 ; case 0
	.short _021E6A72 - _021E6A44 - 2 ; case 1
	.short _021E6A86 - _021E6A44 - 2 ; case 2
	.short _021E6A9E - _021E6A44 - 2 ; case 3
	.short _021E6ACA - _021E6A44 - 2 ; case 4
	.short _021E6AF6 - _021E6A44 - 2 ; case 5
	.short _021E6B22 - _021E6A44 - 2 ; case 6
	.short _021E6B4E - _021E6A44 - 2 ; case 7
	.short _021E6B62 - _021E6A44 - 2 ; case 8
	.short _021E6B8E - _021E6A44 - 2 ; case 9
	.short _021E6B9E - _021E6A44 - 2 ; case 10
_021E6A5A:
	ldr r0, [r4]
	ldr r0, [r0]
	bl Mon_GetBoxMon
	add r2, r0, #0
	add r0, r4, #0
	add r0, #0xfc
	ldr r0, [r0]
	mov r1, #0
	bl BufferBoxMonNickname
	b _021E6BB0
_021E6A72:
	bl MoveRelearner_GetSelectedMove
	add r2, r0, #0
	add r0, r4, #0
	add r0, #0xfc
	ldr r0, [r0]
	mov r1, #1
	bl BufferMoveName
	b _021E6BB0
_021E6A86:
	ldr r0, [r4]
	ldr r0, [r0]
	bl Mon_GetBoxMon
	add r2, r0, #0
	add r0, r4, #0
	add r0, #0xfc
	ldr r0, [r0]
	mov r1, #0
	bl BufferBoxMonNickname
	b _021E6BB0
_021E6A9E:
	ldr r0, [r4]
	ldr r0, [r0]
	bl Mon_GetBoxMon
	add r2, r0, #0
	add r0, r4, #0
	add r0, #0xfc
	ldr r0, [r0]
	mov r1, #0
	bl BufferBoxMonNickname
	add r0, r4, #0
	bl MoveRelearner_GetSelectedMove
	add r2, r0, #0
	add r0, r4, #0
	add r0, #0xfc
	ldr r0, [r0]
	mov r1, #1
	bl BufferMoveName
	b _021E6BB0
_021E6ACA:
	ldr r0, [r4]
	ldr r0, [r0]
	bl Mon_GetBoxMon
	add r2, r0, #0
	add r0, r4, #0
	add r0, #0xfc
	ldr r0, [r0]
	mov r1, #0
	bl BufferBoxMonNickname
	add r0, r4, #0
	bl MoveRelearner_GetSelectedMove
	add r2, r0, #0
	add r0, r4, #0
	add r0, #0xfc
	ldr r0, [r0]
	mov r1, #1
	bl BufferMoveName
	b _021E6BB0
_021E6AF6:
	ldr r0, [r4]
	ldr r0, [r0]
	bl Mon_GetBoxMon
	add r2, r0, #0
	add r0, r4, #0
	add r0, #0xfc
	ldr r0, [r0]
	mov r1, #0
	bl BufferBoxMonNickname
	add r0, r4, #0
	bl MoveRelearner_GetMoveToReplace
	add r2, r0, #0
	add r0, r4, #0
	add r0, #0xfc
	ldr r0, [r0]
	mov r1, #1
	bl BufferMoveName
	b _021E6BB0
_021E6B22:
	ldr r0, [r4]
	ldr r0, [r0]
	bl Mon_GetBoxMon
	add r2, r0, #0
	add r0, r4, #0
	add r0, #0xfc
	ldr r0, [r0]
	mov r1, #0
	bl BufferBoxMonNickname
	add r0, r4, #0
	bl MoveRelearner_GetSelectedMove
	add r2, r0, #0
	add r0, r4, #0
	add r0, #0xfc
	ldr r0, [r0]
	mov r1, #1
	bl BufferMoveName
	b _021E6BB0
_021E6B4E:
	bl MoveRelearner_GetSelectedMove
	add r2, r0, #0
	add r0, r4, #0
	add r0, #0xfc
	ldr r0, [r0]
	mov r1, #1
	bl BufferMoveName
	b _021E6BB0
_021E6B62:
	ldr r0, [r4]
	ldr r0, [r0]
	bl Mon_GetBoxMon
	add r2, r0, #0
	add r0, r4, #0
	add r0, #0xfc
	ldr r0, [r0]
	mov r1, #0
	bl BufferBoxMonNickname
	add r0, r4, #0
	bl MoveRelearner_GetSelectedMove
	add r2, r0, #0
	add r0, r4, #0
	add r0, #0xfc
	ldr r0, [r0]
	mov r1, #1
	bl BufferMoveName
	b _021E6BB0
_021E6B8E:
	ldr r2, [r4]
	add r0, #0xfc
	ldr r0, [r0]
	ldr r2, [r2, #4]
	mov r1, #2
	bl BufferPlayersName
	b _021E6BB0
_021E6B9E:
	bl MoveRelearner_GetMoveToReplace
	add r2, r0, #0
	add r0, r4, #0
	add r0, #0xfc
	ldr r0, [r0]
	mov r1, #0
	bl BufferMoveName
_021E6BB0:
	ldr r1, [r4]
	add r0, r4, #0
	ldrb r2, [r1, #0x19]
	add r0, #0xf8
	mov r1, #0x2c
	add r3, r2, #0
	mul r3, r1
	ldr r1, _021E6BE8 ; =sMoveRelearnerMessageIds
	lsl r2, r5, #2
	add r1, r1, r3
	ldr r0, [r0]
	ldr r1, [r2, r1]
	bl NewString_ReadMsgData
	add r5, r0, #0
	add r0, r4, #0
	mov r1, #1
	add r0, #0xfc
	lsl r1, r1, #8
	ldr r0, [r0]
	ldr r1, [r4, r1]
	add r2, r5, #0
	bl StringExpandPlaceholders
	add r0, r5, #0
	bl String_Delete
	pop {r3, r4, r5, pc}
	.balign 4, 0
_021E6BE8: .word sMoveRelearnerMessageIds
	thumb_func_end MoveRelearner_FormatMessage
