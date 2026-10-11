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

	thumb_func_start FrontierRecordsApp_Init
FrontierRecordsApp_Init: ; 0x021E5900
	push {r4, r5, r6, lr}
	add r4, r0, #0
	mov r0, #0
	add r6, r1, #0
	add r1, r0, #0
	bl Main_SetVBlankIntrCB
	mov r0, #0
	add r1, r0, #0
	bl Main_SetHBlankIntrCB
	bl GfGfx_DisableEngineAPlanes
	bl GfGfx_DisableEngineBPlanes
	mov r2, #1
	lsl r2, r2, #0x1a
	ldr r1, [r2]
	ldr r0, _021E5A2C ; =0xFFFFE0FF
	and r1, r0
	str r1, [r2]
	ldr r2, _021E5A30 ; =0x04001000
	ldr r1, [r2]
	and r0, r1
	str r0, [r2]
	ldr r2, _021E5A34 ; =0x04000304
	ldr r0, _021E5A38 ; =0xFFFF7FFF
	ldrh r1, [r2]
	and r0, r1
	strh r0, [r2]
	mov r0, #3
	mov r1, #0x79
	lsl r2, r0, #0x10
	bl Heap_Create
	mov r1, #0xe5
	add r0, r4, #0
	lsl r1, r1, #2
	mov r2, #0x79
	bl OverlayManager_CreateAndGetData
	mov r2, #0xe5
	add r5, r0, #0
	mov r1, #0
	lsl r2, r2, #2
	bl memset
	str r4, [r5]
	mov r0, #0x79
	bl BgConfig_Alloc
	str r0, [r5, #0xc]
	add r0, r4, #0
	bl OverlayManager_GetArgs
	add r4, r0, #0
	mov r0, #0x89
	ldr r1, [r4]
	lsl r0, r0, #2
	str r1, [r5, r0]
	ldr r0, [r5, r0]
	bl Save_Frontier_GetStatic
	mov r1, #0x8a
	lsl r1, r1, #2
	str r0, [r5, r1]
	ldrb r0, [r4, #4]
	strb r0, [r5, #6]
	ldrb r0, [r4, #5]
	strb r0, [r5, #7]
	ldrh r0, [r4, #6]
	strh r0, [r5, #8]
	sub r0, r1, #4
	ldr r0, [r5, r0]
	bl Save_PlayerData_GetOptionsAddr
	mov r1, #0x22
	lsl r1, r1, #4
	str r0, [r5, r1]
	mov r0, #4
	mov r1, #0x79
	bl FontID_Alloc
	add r0, r5, #0
	bl FrontierRecordsApp_InitGraphics
	mov r0, #1
	mov r1, #0x1b
	mov r2, #0x13
	mov r3, #0x79
	bl NewMsgDataFromNarc
	mov r1, #0x21
	lsl r1, r1, #4
	str r0, [r5, r1]
	mov r0, #0x79
	bl MessageFormat_New
	mov r1, #0x85
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r0, #0x32
	lsl r0, r0, #4
	mov r1, #0x79
	bl String_New
	mov r1, #0x86
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r0, #0
	sub r1, #0x78
	mov r2, #0x79
	bl LoadFontPal0
	mov r1, #6
	mov r0, #0
	lsl r1, r1, #6
	mov r2, #0x79
	bl LoadFontPal1
	ldrb r0, [r5, #7]
	bl FrontierRecordsApp_GetWindowSet
	add r2, r0, #0
	add r1, r5, #0
	ldr r0, [r5, #0xc]
	add r1, #0x10
	bl FrontierRecordsApp_AddWindows
	mov r1, #0x8f
	lsl r1, r1, #2
	ldr r0, [r5, #0xc]
	add r1, r5, r1
	bl FrontierRecordsApp_AddReturnButtonWindow
	add r0, r5, #0
	bl FrontierRecordsApp_PrintReturnButton
	add r0, r5, #0
	bl FrontierRecordsApp_CreateSprites
	ldr r0, _021E5A3C ; =FrontierRecordsApp_VBlankCallback
	add r1, r5, #0
	bl Main_SetVBlankIntrCB
	mov r0, #0
	str r0, [r6]
	mov r0, #1
	pop {r4, r5, r6, pc}
	nop
_021E5A2C: .word 0xFFFFE0FF
_021E5A30: .word 0x04001000
_021E5A34: .word 0x04000304
_021E5A38: .word 0xFFFF7FFF
_021E5A3C: .word FrontierRecordsApp_VBlankCallback
	thumb_func_end FrontierRecordsApp_Init


	thumb_func_start FrontierRecordsApp_Main
FrontierRecordsApp_Main: ; 0x021E5A40
	push {r3, r4, r5, lr}
	add r5, r1, #0
	bl OverlayManager_GetData
	ldr r1, [r5]
	add r4, r0, #0
	cmp r1, #0
	beq _021E5A5A
	cmp r1, #1
	beq _021E5A6E
	cmp r1, #2
	beq _021E5A82
	b _021E5A8E
_021E5A5A:
	bl FrontierRecordsApp_FadeIn
	cmp r0, #1
	bne _021E5A8E
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #1
	bl FrontierRecordsApp_SetMainState
	b _021E5A8E
_021E5A6E:
	bl FrontierRecordsApp_Run
	cmp r0, #1
	bne _021E5A8E
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #2
	bl FrontierRecordsApp_SetMainState
	b _021E5A8E
_021E5A82:
	bl FrontierRecordsApp_FadeOut
	cmp r0, #1
	bne _021E5A8E
	mov r0, #1
	pop {r3, r4, r5, pc}
_021E5A8E:
	add r0, r4, #0
	bl FrontierRecordsApp_TickSprites
	mov r0, #0x23
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	bl SpriteSystem_DrawSprites
	mov r0, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end FrontierRecordsApp_Main


	thumb_func_start FrontierRecordsApp_Exit
FrontierRecordsApp_Exit: ; 0x021E5AA4
	push {r3, r4, r5, lr}
	add r5, r0, #0
	bl OverlayManager_GetData
	add r4, r0, #0
	bl FrontierRecordsApp_FreeSprites
	mov r0, #0x87
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #2
	bl PaletteData_FreeBuffers
	mov r0, #0x87
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #0
	bl PaletteData_FreeBuffers
	mov r0, #0x87
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl PaletteData_Free
	mov r0, #0x87
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r4, r0]
	sub r0, #0xc
	ldr r0, [r4, r0]
	bl DestroyMsgData
	mov r0, #0x85
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl MessageFormat_Delete
	mov r0, #0x86
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl String_Delete
	mov r0, #0x8f
	lsl r0, r0, #2
	add r0, r4, r0
	bl FrontierRecordsApp_RemoveReturnButtonWindow
	ldrb r0, [r4, #7]
	bl FrontierRecordsApp_GetWindowSet
	add r1, r0, #0
	add r0, r4, #0
	add r0, #0x10
	bl FrontierRecordsApp_RemoveWindows
	ldr r0, [r4, #0xc]
	bl FrontierRecordsApp_FreeBgLayers
	mov r0, #4
	bl FontID_Release
	add r0, r5, #0
	bl OverlayManager_FreeData
	mov r0, #0
	add r1, r0, #0
	bl Main_SetVBlankIntrCB
	mov r0, #0x79
	bl Heap_Destroy
	mov r0, #1
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end FrontierRecordsApp_Exit


	thumb_func_start FrontierRecordsApp_FadeIn
FrontierRecordsApp_FadeIn: ; 0x021E5B38
	push {r3, r4, lr}
	sub sp, #0xc
	add r4, r0, #0
	ldrb r1, [r4, #4]
	cmp r1, #0
	beq _021E5B4E
	cmp r1, #1
	beq _021E5B6C
	cmp r1, #2
	beq _021E5B8C
	b _021E5B9A
_021E5B4E:
	bl FrontierRecordsApp_IsHall
	cmp r0, #1
	bne _021E5B5E
	add r0, r4, #0
	bl FrontierRecordsApp_InitHallPicker
	b _021E5B64
_021E5B5E:
	add r0, r4, #0
	bl FrontierRecordsApp_PrintRecords
_021E5B64:
	ldrb r0, [r4, #4]
	add r0, r0, #1
	strb r0, [r4, #4]
	b _021E5B9A
_021E5B6C:
	mov r0, #6
	str r0, [sp]
	mov r0, #3
	str r0, [sp, #4]
	mov r0, #0x79
	str r0, [sp, #8]
	mov r0, #0
	mov r1, #1
	add r2, r1, #0
	add r3, r0, #0
	bl BeginNormalPaletteFade
	ldrb r0, [r4, #4]
	add r0, r0, #1
	strb r0, [r4, #4]
	b _021E5B9A
_021E5B8C:
	bl IsPaletteFadeFinished
	cmp r0, #1
	bne _021E5B9A
	add sp, #0xc
	mov r0, #1
	pop {r3, r4, pc}
_021E5B9A:
	mov r0, #0
	add sp, #0xc
	pop {r3, r4, pc}
	thumb_func_end FrontierRecordsApp_FadeIn


	thumb_func_start FrontierRecordsApp_Run
FrontierRecordsApp_Run: ; 0x021E5BA0
	push {r4, lr}
	add r4, r0, #0
	ldrb r1, [r4, #4]
	cmp r1, #0xa
	bhi _021E5C82
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_021E5BB6: ; jump table
	.short _021E5BCC - _021E5BB6 - 2 ; case 0
	.short _021E5BE0 - _021E5BB6 - 2 ; case 1
	.short _021E5C0A - _021E5BB6 - 2 ; case 2
	.short _021E5C16 - _021E5BB6 - 2 ; case 3
	.short _021E5C1E - _021E5BB6 - 2 ; case 4
	.short _021E5C2A - _021E5BB6 - 2 ; case 5
	.short _021E5C32 - _021E5BB6 - 2 ; case 6
	.short _021E5C3A - _021E5BB6 - 2 ; case 7
	.short _021E5C46 - _021E5BB6 - 2 ; case 8
	.short _021E5C70 - _021E5BB6 - 2 ; case 9
	.short _021E5C7E - _021E5BB6 - 2 ; case 10
_021E5BCC:
	bl FrontierRecordsApp_IsHall
	cmp r0, #1
	bne _021E5BDA
	mov r0, #3
	strb r0, [r4, #4]
	b _021E5C82
_021E5BDA:
	mov r0, #1
	strb r0, [r4, #4]
	b _021E5C82
_021E5BE0:
	ldr r0, _021E5C88 ; =_021E7E98
	bl TouchscreenHitbox_TouchNewIsIn
	cmp r0, #1
	beq _021E5BF4
	ldr r0, _021E5C8C ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #3
	tst r0, r1
	beq _021E5C82
_021E5BF4:
	ldr r0, _021E5C90 ; =0x000005DD
	bl PlaySE
	add r0, r4, #0
	bl FrontierRecordsApp_StartReturnButtonFlash
	mov r0, #0xa
	strb r0, [r4, #5]
	mov r0, #9
	strb r0, [r4, #4]
	b _021E5C82
_021E5C0A:
	mov r1, #1
	bl FrontierRecordsApp_ShowHallPage
	mov r0, #3
	strb r0, [r4, #4]
	b _021E5C82
_021E5C16:
	bl FrontierRecordsApp_HandleLetterInput
	strb r0, [r4, #4]
	b _021E5C82
_021E5C1E:
	mov r1, #2
	bl FrontierRecordsApp_ShowHallPage
	mov r0, #5
	strb r0, [r4, #4]
	b _021E5C82
_021E5C2A:
	bl FrontierRecordsApp_HandleSpeciesInput
	strb r0, [r4, #4]
	b _021E5C82
_021E5C32:
	bl FrontierRecordsApp_FlipSpeciesPage
	strb r0, [r4, #4]
	b _021E5C82
_021E5C3A:
	mov r1, #0
	bl FrontierRecordsApp_ShowHallPage
	mov r0, #8
	strb r0, [r4, #4]
	b _021E5C82
_021E5C46:
	ldr r0, _021E5C88 ; =_021E7E98
	bl TouchscreenHitbox_TouchNewIsIn
	cmp r0, #1
	beq _021E5C5A
	ldr r0, _021E5C8C ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #3
	tst r0, r1
	beq _021E5C82
_021E5C5A:
	ldr r0, _021E5C90 ; =0x000005DD
	bl PlaySE
	add r0, r4, #0
	bl FrontierRecordsApp_StartReturnButtonFlash
	mov r0, #4
	strb r0, [r4, #5]
	mov r0, #9
	strb r0, [r4, #4]
	b _021E5C82
_021E5C70:
	bl FrontierRecordsApp_RunButtonFlash
	cmp r0, #0
	bne _021E5C82
	ldrb r0, [r4, #5]
	strb r0, [r4, #4]
	b _021E5C82
_021E5C7E:
	mov r0, #1
	pop {r4, pc}
_021E5C82:
	mov r0, #0
	pop {r4, pc}
	nop
_021E5C88: .word _021E7E98
_021E5C8C: .word gSystem
_021E5C90: .word 0x000005DD
	thumb_func_end FrontierRecordsApp_Run


	thumb_func_start FrontierRecordsApp_FadeOut
FrontierRecordsApp_FadeOut: ; 0x021E5C94
	push {r3, r4, lr}
	sub sp, #0xc
	add r4, r0, #0
	ldrb r0, [r4, #4]
	cmp r0, #0
	beq _021E5CA6
	cmp r0, #1
	beq _021E5CC6
	b _021E5CD4
_021E5CA6:
	mov r0, #6
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	mov r0, #0x79
	str r0, [sp, #8]
	mov r0, #0
	add r1, r0, #0
	add r2, r0, #0
	add r3, r0, #0
	bl BeginNormalPaletteFade
	ldrb r0, [r4, #4]
	add r0, r0, #1
	strb r0, [r4, #4]
	b _021E5CD4
_021E5CC6:
	bl IsPaletteFadeFinished
	cmp r0, #1
	bne _021E5CD4
	add sp, #0xc
	mov r0, #1
	pop {r3, r4, pc}
_021E5CD4:
	mov r0, #0
	add sp, #0xc
	pop {r3, r4, pc}
	.balign 4, 0
	thumb_func_end FrontierRecordsApp_FadeOut


	thumb_func_start FrontierRecordsApp_VBlankCallback
FrontierRecordsApp_VBlankCallback: ; 0x021E5CDC
	push {r4, lr}
	add r4, r0, #0
	mov r0, #0x87
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	cmp r0, #0
	beq _021E5CEE
	bl PaletteData_PushTransparentBuffers
_021E5CEE:
	ldr r0, [r4, #0xc]
	bl DoScheduledBgGpuUpdates
	bl SpriteSystem_TransferOam
	ldr r3, _021E5D08 ; =0x027E0000
	ldr r1, _021E5D0C ; =0x00003FF8
	mov r0, #1
	ldr r2, [r3, r1]
	orr r0, r2
	str r0, [r3, r1]
	pop {r4, pc}
	nop
_021E5D08: .word 0x027E0000
_021E5D0C: .word 0x00003FF8
	thumb_func_end FrontierRecordsApp_VBlankCallback
