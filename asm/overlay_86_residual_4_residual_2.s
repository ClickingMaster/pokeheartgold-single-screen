	.include "asm/macros.inc"
	.public _021E7E98
	.public FrontierRecordsApp_Init
	.public FrontierRecordsApp_Main
	.public FrontierRecordsApp_Exit
	.public FrontierRecordsApp_FadeIn
	.public FrontierRecordsApp_Run
	.public FrontierRecordsApp_FadeOut
	.public FrontierRecordsApp_VBlankCallback
	.public FrontierRecordsApp_SetGraphicsBanks
	.public FrontierRecordsApp_InitBgLayers
	.public FrontierRecordsApp_InitGraphics
	.public FrontierRecordsApp_FreeBgLayers
	.public FrontierRecordsApp_SetMainState
	.public FrontierRecordsApp_GetWindowSet
	.public FrontierRecordsApp_LoadMainBgGraphics
	.public FrontierRecordsApp_LoadSubBgGraphics
	.public FrontierRecordsApp_BufferNumber
	.public FrontierRecordsApp_PrintStringAligned
	.public FrontierRecordsApp_PrintMessage
	.public FrontierRecordsApp_PrintFormattedMessage
	.public FrontierRecordsApp_PrintRecords
	.public FrontierRecordsApp_PrintFactoryLevel50Records
	.public FrontierRecordsApp_PrintFactoryOpenLevelRecords
	.public FrontierRecordsApp_GetFactoryStreakLabel
	.public FrontierRecordsApp_PrintHallRecords
	.public FrontierRecordsApp_BufferSpeciesName
	.public FrontierRecordsApp_PrintCastleRecords
	.public FrontierRecordsApp_GetCastleStreakLabel
	.public FrontierRecordsApp_PrintArcadeRecords
	.public FrontierRecordsApp_GetArcadeStreakLabel
	.public FrontierRecordsApp_PrintTowerRecords
	.public FrontierRecordsApp_PrintTowerMultiRecords
	.public FrontierRecordsApp_GetTowerStreakLabel
	.public FrontierRecordsApp_PrintReturnButton
	.public FrontierRecordsApp_CreateSprites
	.public FrontierRecordsApp_FreeSprites
	.public FrontierRecordsApp_TickSprites
	.public FrontierRecordsApp_SetSpriteAnim
	.public FrontierRecordsApp_SetSpritePosition
	.public FrontierRecordsApp_RunButtonFlash
	.public FrontierRecordsApp_StartReturnButtonFlash
	.public FrontierRecordsApp_IsHall
	.public FrontierRecordsApp_InitHallPicker
	.public FrontierRecordsApp_FreeHallPicker
	.public FrontierRecordsApp_ShowHallPage
	.public FrontierRecordsApp_DrawPageArrows
	.public FrontierRecordsApp_FreeGridInput
	.public FrontierRecordsApp_GridCallback_Nop
	.public FrontierRecordsApp_LoadSortedSpecies
	.public FrontierRecordsApp_InitSpeciesGroups
	.public FrontierRecordsApp_FreeSpeciesGroups
	.public FrontierRecordsApp_LoadWindowFrame
	.public FrontierRecordsApp_ShowPrompt
	.public FrontierRecordsApp_ClearPrompt
	.public FrontierRecordsApp_PrintLetters
	.public FrontierRecordsApp_MoveLetterCursor
	.public FrontierRecordsApp_CreateLetterGrid
	.public FrontierRecordsApp_LetterGrid_OnKeyMove
	.public FrontierRecordsApp_LetterGrid_OnTouch
	.public FrontierRecordsApp_StartLetterFlash
	.public FrontierRecordsApp_HandleLetterInput
	.public FrontierRecordsApp_GrayOutEmptyLetters
	.public FrontierRecordsApp_MoveSpeciesCursor
	.public FrontierRecordsApp_CreateSpeciesGrid
	.public FrontierRecordsApp_SpeciesGrid_OnKeyMove
	.public FrontierRecordsApp_SpeciesGrid_OnTouch
	.public FrontierRecordsApp_StartSpeciesFlash
	.public FrontierRecordsApp_StartArrowFlash
	.public FrontierRecordsApp_HandleSpeciesInput
	.public FrontierRecordsApp_FlipSpeciesPage
	.public FrontierRecordsApp_PrintSpeciesPage
	.public FrontierRecordsApp_ClearSpeciesWindows
	.public FrontierRecordsApp_PrintLetterTitle
	.public FrontierRecordsApp_PrintPageNumber
	.public FrontierRecordsApp_ResetCursor
	.public FrontierRecordsApp_SelectSpecies
	.public FrontierRecordsApp_ClearRecordWindows
	.public FrontierRecordsApp_AddWindows
	.public FrontierRecordsApp_RemoveWindows
	.public FrontierRecordsApp_AddReturnButtonWindow
	.public FrontierRecordsApp_RemoveReturnButtonWindow
	.public ov86_021E7E9C
	.public ov86_021E7EA8
	.public ov86_021E7EB8
	.public ov86_021E7EC8
	.public ov86_021E7ED8
	.public ov86_021E7EEC
	.public ov86_021E7F04
	.public ov86_021E7F20
	.public ov86_021E7F3C
	.public ov86_021E7F58
	.public ov86_021E7F74
	.public ov86_021E7F94
	.public ov86_021E7FBC
	.public ov86_021E7FE4
	.public ov86_021E802C
	.public ov86_021E8094
	.public ov86_021E8104
	.public ov86_021E81DC
	.public ov86_021E81F8
	.public ov86_021E8210
	.public ov86_021E8230
	.public ov86_021E8258
	.public ov86_021E8290
	.public ov86_021E82D8
	.include "overlay_86.inc"
	.include "global.inc"

	.text
	.public FrontierRecordsApp_SetGraphicsBanks
	.public FrontierRecordsApp_FreeBgLayers
	.public FrontierRecordsApp_SetMainState
	.public FrontierRecordsApp_BufferNumber
	.public FrontierRecordsApp_BufferSpeciesName
	.public FrontierRecordsApp_TickSprites
	.public FrontierRecordsApp_SetSpriteAnim
	.public FrontierRecordsApp_SetSpritePosition
	.public FrontierRecordsApp_IsHall
	.public FrontierRecordsApp_InitHallPicker
	.public FrontierRecordsApp_FreeHallPicker
	.public FrontierRecordsApp_FreeGridInput
	.public FrontierRecordsApp_GridCallback_Nop
	.public FrontierRecordsApp_FreeSpeciesGroups
	.public FrontierRecordsApp_LoadWindowFrame
	.public FrontierRecordsApp_ClearPrompt
	.public FrontierRecordsApp_MoveLetterCursor
	.public FrontierRecordsApp_LetterGrid_OnKeyMove
	.public FrontierRecordsApp_LetterGrid_OnTouch
	.public FrontierRecordsApp_MoveSpeciesCursor
	.public FrontierRecordsApp_SpeciesGrid_OnKeyMove
	.public FrontierRecordsApp_SpeciesGrid_OnTouch
	.public FrontierRecordsApp_FlipSpeciesPage
	.public FrontierRecordsApp_ClearSpeciesWindows
	.public FrontierRecordsApp_ResetCursor
	.public FrontierRecordsApp_ClearRecordWindows
	.public FrontierRecordsApp_RemoveWindows
	.public FrontierRecordsApp_AddReturnButtonWindow
	.public FrontierRecordsApp_RemoveReturnButtonWindow
	.public FrontierRecordsApp_PrintStringAligned
	.public FrontierRecordsApp_PrintMessage
	.public FrontierRecordsApp_PrintFormattedMessage
	.public FrontierRecordsApp_PrintRecords
	.public FrontierRecordsApp_GetFactoryStreakLabel


	thumb_func_start FrontierRecordsApp_PrintHallRecords
FrontierRecordsApp_PrintHallRecords: ; 0x021E64E0
	push {r3, r4, r5, r6, lr}
	sub sp, #0x14
	add r5, r0, #0
	add r0, #0x10
	mov r1, #0
	bl FillWindowPixelBuffer
	add r0, r5, #0
	add r0, #0x20
	mov r1, #0
	bl FillWindowPixelBuffer
	add r0, r5, #0
	add r0, #0x30
	mov r1, #0
	bl FillWindowPixelBuffer
	add r0, r5, #0
	add r0, #0x40
	mov r1, #0
	bl FillWindowPixelBuffer
	mov r1, #0
	str r1, [sp]
	ldr r0, _021E6684 ; =0x000F0200
	str r1, [sp, #4]
	str r0, [sp, #8]
	str r1, [sp, #0xc]
	add r0, r5, #0
	mov r2, #0x18
	add r3, r1, #0
	bl FrontierRecordsApp_PrintMessage
	ldrb r0, [r5, #6]
	cmp r0, #0
	bne _021E652C
	mov r2, #0x1a
	b _021E6536
_021E652C:
	cmp r0, #1
	bne _021E6534
	mov r2, #0x1b
	b _021E6536
_021E6534:
	mov r2, #0x1c
_021E6536:
	mov r1, #0
	str r1, [sp]
	ldr r0, _021E6684 ; =0x000F0200
	str r1, [sp, #4]
	str r0, [sp, #8]
	mov r0, #1
	str r0, [sp, #0xc]
	add r0, r5, #0
	mov r3, #0xe0
	bl FrontierRecordsApp_PrintMessage
	ldrh r1, [r5, #8]
	add r0, r5, #0
	bl FrontierRecordsApp_BufferSpeciesName
	mov r3, #0
	str r3, [sp]
	ldr r0, _021E6688 ; =0x00010200
	str r3, [sp, #4]
	str r0, [sp, #8]
	add r0, r5, #0
	mov r1, #1
	mov r2, #0x33
	str r3, [sp, #0xc]
	bl FrontierRecordsApp_PrintFormattedMessage
	mov r3, #0
	str r3, [sp]
	ldr r0, _021E6688 ; =0x00010200
	str r3, [sp, #4]
	str r0, [sp, #8]
	add r0, r5, #0
	mov r1, #2
	mov r2, #0x1d
	str r3, [sp, #0xc]
	bl FrontierRecordsApp_PrintMessage
	mov r0, #0x89
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl Save_Frontier_GetStatic
	add r4, r0, #0
	ldrb r0, [r5, #6]
	bl sub_0205C11C
	add r6, r0, #0
	ldrb r0, [r5, #6]
	bl sub_0205C11C
	bl sub_0205C268
	add r2, r0, #0
	add r0, r4, #0
	add r1, r6, #0
	bl FrontierSave_GetStat
	ldrh r1, [r5, #8]
	cmp r1, r0
	beq _021E65B2
	mov r2, #0
	b _021E65C8
_021E65B2:
	ldrb r0, [r5, #6]
	bl sub_0205C0CC
	add r1, r0, #0
	mov r0, #0x8a
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r2, #0xff
	bl FrontierSave_GetStat
	add r2, r0, #0
_021E65C8:
	add r0, r5, #0
	mov r1, #0
	bl FrontierRecordsApp_BufferNumber
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _021E6688 ; =0x00010200
	mov r1, #2
	str r0, [sp, #8]
	add r0, r5, #0
	mov r2, #0x32
	mov r3, #0x70
	str r1, [sp, #0xc]
	bl FrontierRecordsApp_PrintFormattedMessage
	mov r3, #0
	str r3, [sp]
	ldr r0, _021E6688 ; =0x00010200
	str r3, [sp, #4]
	str r0, [sp, #8]
	add r0, r5, #0
	mov r1, #3
	mov r2, #0x2b
	str r3, [sp, #0xc]
	bl FrontierRecordsApp_PrintMessage
	mov r0, #0x89
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0xb
	add r2, sp, #0x10
	bl sub_020312C4
	add r4, r0, #0
	ldr r0, [sp, #0x10]
	cmp r0, #1
	beq _021E6618
	mov r6, #0
	b _021E6630
_021E6618:
	ldrb r0, [r5, #6]
	bl sub_0205C144
	add r2, r0, #0
	mov r0, #0x89
	lsl r0, r0, #2
	ldrh r3, [r5, #8]
	ldr r0, [r5, r0]
	add r1, r4, #0
	bl sub_020312E0
	add r6, r0, #0
_021E6630:
	cmp r4, #0
	beq _021E663A
	add r0, r4, #0
	bl Heap_Free
_021E663A:
	add r0, r5, #0
	mov r1, #0
	add r2, r6, #0
	bl FrontierRecordsApp_BufferNumber
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _021E6688 ; =0x00010200
	mov r1, #3
	str r0, [sp, #8]
	mov r0, #2
	str r0, [sp, #0xc]
	add r0, r5, #0
	mov r2, #0x32
	mov r3, #0x70
	bl FrontierRecordsApp_PrintFormattedMessage
	add r0, r5, #0
	add r0, #0x10
	bl ScheduleWindowCopyToVram
	add r0, r5, #0
	add r0, #0x20
	bl ScheduleWindowCopyToVram
	add r0, r5, #0
	add r0, #0x30
	bl ScheduleWindowCopyToVram
	add r5, #0x40
	add r0, r5, #0
	bl ScheduleWindowCopyToVram
	add sp, #0x14
	pop {r3, r4, r5, r6, pc}
	nop
_021E6684: .word 0x000F0200
_021E6688: .word 0x00010200
	thumb_func_end FrontierRecordsApp_PrintHallRecords
