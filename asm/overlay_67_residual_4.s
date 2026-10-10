	.include "asm/macros.inc"
	.include "overlay_67.inc"
	.include "global.inc"

	.public sBattleRoomResultsPageHitboxes
	.public sBattleRoomResultsGraphicsModes
	.public sBattleRoomResultsGridCallbacks
	.public sBattleRoomResultsOamCharTransferParam
	.public sBattleRoomResultsSpriteResourceCounts
	.public sBattleRoomResultsBgTemplate_Main0
	.public sBattleRoomResultsBgTemplate_Sub0
	.public sBattleRoomResultsBgTemplate_Sub1
	.public sBattleRoomResultsBgTemplate_Main1
	.public sBattleRoomResultsOamManagerParam
	.public sBattleRoomResultsGraphicsBanks
	.public sBattleRoomResultsGridHitboxes
	.public sBattleRoomResultsWindowTemplates
	.public sBattleRoomResultsGridDpadBoxes
	.public sBattleRoomResultsSpriteTemplates
	.public sBattleRoomResultsLeaderWindowTemplates

	.public _021E6D4C

	.text
	.public BattleRoomResults_Init
	.public BattleRoomResults_Exit
	.public BattleRoomResults_Main
	.public BattleRoomResults_SetupScreens
	.public BattleRoomResults_TeardownScreens
	.public BattleRoomResults_SetGfxBanks
	.public BattleRoomResults_VBlankCB
	.public BattleRoomResults_HandleResultsInput
	.public BattleRoomResults_HandleLeadersInput
	.public BattleRoomResults_InitBgs
	.public BattleRoomResults_FreeBgs
	.public BattleRoomResults_LoadBgGraphics
	.public BattleRoomResults_AddWindows
	.public BattleRoomResults_RemoveWindows
	.public BattleRoomResults_InitStrings
	.public BattleRoomResults_FreeStrings
	.public BattleRoomResults_PrintString
	.public BattleRoomResults_PrintMessage
	.public BattleRoomResults_PrintFormattedMessage
	.public BattleRoomResults_PrintTitle
	.public BattleRoomResults_PrintResults
	.public BattleRoomResults_LoadLeaders
	.public BattleRoomResults_FreeLeaderNames
	.public BattleRoomResults_PrintLeaderNames
	.public BattleRoomResults_PrintPageNumber
	.public BattleRoomResults_PrintLeadersScreen
	.public BattleRoomResults_PrintLeaderInfo
	.public ov67_021E681C
	.public BattleRoomResults_InitSprites
	.public BattleRoomResults_FreeSprites
	.public BattleRoomResults_UpdateSprites
	.public BattleRoomResults_TransferOam
	.public BattleRoomResults_SetSpriteAnim
	.public BattleRoomResults_SetSpritePosition
	.public BattleRoomResults_UpdateButtonFlash
	.public BattleRoomResults_StartSpriteFlash
	.public BattleRoomResults_StartExitButtonFlash
	.public BattleRoomResults_CreateGridInput
	.public BattleRoomResults_FreeGridInput
	.public BattleRoomResults_MoveCursor
	.public BattleRoomResults_GridNoop
	.public BattleRoomResults_GridOnKeyMove
	.public BattleRoomResults_GridOnTouch
	thumb_func_start BattleRoomResults_LoadBgGraphics
BattleRoomResults_LoadBgGraphics: ; 0x021E5EB0
	push {r3, r4, r5, lr}
	sub sp, #0x10
	add r4, r0, #0
	ldrh r0, [r4, #8]
	cmp r0, #0
	bne _021E5F68
	ldr r1, [r4]
	mov r0, #0x76
	bl NARC_New
	mov r1, #0
	str r1, [sp]
	str r1, [sp, #4]
	mov r3, #1
	str r3, [sp, #8]
	ldr r1, [r4]
	add r5, r0, #0
	str r1, [sp, #0xc]
	ldr r2, [r4, #0x10]
	mov r1, #4
	bl GfGfxLoader_LoadCharDataFromOpenNarc
	mov r0, #0x20
	str r0, [sp]
	ldr r0, [r4]
	mov r2, #0
	str r0, [sp, #4]
	add r0, r5, #0
	mov r1, #7
	add r3, r2, #0
	bl GfGfxLoader_GXLoadPalFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	mov r3, #1
	str r3, [sp, #8]
	ldr r0, [r4]
	mov r1, #6
	str r0, [sp, #0xc]
	ldr r2, [r4, #0x10]
	add r0, r5, #0
	bl GfGfxLoader_LoadScrnDataFromOpenNarc
	add r0, r5, #0
	bl NARC_Delete
	ldr r1, [r4]
	mov r0, #0xb7
	bl NARC_New
	mov r1, #0
	str r1, [sp]
	str r1, [sp, #4]
	mov r1, #1
	str r1, [sp, #8]
	ldr r1, [r4]
	mov r3, #5
	str r1, [sp, #0xc]
	ldr r2, [r4, #0x10]
	mov r1, #0x8e
	add r5, r0, #0
	bl GfGfxLoader_LoadCharDataFromOpenNarc
	mov r0, #0x20
	str r0, [sp]
	ldr r0, [r4]
	mov r1, #0xc4
	str r0, [sp, #4]
	add r0, r5, #0
	mov r2, #4
	mov r3, #0
	bl GfGfxLoader_GXLoadPalFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	ldr r0, [r4]
	mov r1, #0x8d
	str r0, [sp, #0xc]
	ldr r2, [r4, #0x10]
	add r0, r5, #0
	mov r3, #5
	bl GfGfxLoader_LoadScrnDataFromOpenNarc
	add r0, r5, #0
	bl NARC_Delete
	add sp, #0x10
	pop {r3, r4, r5, pc}
_021E5F68:
	ldr r1, [r4]
	mov r0, #0x76
	bl NARC_New
	mov r1, #0
	str r1, [sp]
	str r1, [sp, #4]
	mov r3, #1
	str r3, [sp, #8]
	ldr r1, [r4]
	add r5, r0, #0
	str r1, [sp, #0xc]
	ldr r2, [r4, #0x10]
	mov r1, #4
	bl GfGfxLoader_LoadCharDataFromOpenNarc
	mov r0, #0x20
	str r0, [sp]
	ldr r0, [r4]
	mov r2, #0
	str r0, [sp, #4]
	add r0, r5, #0
	mov r1, #7
	add r3, r2, #0
	bl GfGfxLoader_GXLoadPalFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	mov r3, #1
	str r3, [sp, #8]
	ldr r0, [r4]
	mov r1, #5
	str r0, [sp, #0xc]
	ldr r2, [r4, #0x10]
	add r0, r5, #0
	bl GfGfxLoader_LoadScrnDataFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	ldr r0, [r4]
	mov r1, #8
	str r0, [sp, #0xc]
	ldr r2, [r4, #0x10]
	add r0, r5, #0
	mov r3, #5
	bl GfGfxLoader_LoadCharDataFromOpenNarc
	mov r0, #0x20
	str r0, [sp]
	ldr r0, [r4]
	mov r1, #0xa
	str r0, [sp, #4]
	add r0, r5, #0
	mov r2, #4
	mov r3, #0
	bl GfGfxLoader_GXLoadPalFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	ldr r0, [r4]
	mov r1, #9
	str r0, [sp, #0xc]
	ldr r2, [r4, #0x10]
	add r0, r5, #0
	mov r3, #5
	bl GfGfxLoader_LoadScrnDataFromOpenNarc
	add r0, r5, #0
	bl NARC_Delete
	add sp, #0x10
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end BattleRoomResults_LoadBgGraphics


	thumb_func_start BattleRoomResults_AddWindows
BattleRoomResults_AddWindows: ; 0x021E6008
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	ldrh r0, [r6, #8]
	cmp r0, #0
	bne _021E6030
	add r5, r6, #0
	ldr r4, _021E6078 ; =sBattleRoomResultsWindowTemplates
	mov r7, #0
	add r5, #0x6c
_021E601A:
	ldr r0, [r6, #0x10]
	add r1, r5, #0
	add r2, r4, #0
	bl AddWindow
	add r7, r7, #1
	add r4, #8
	add r5, #0x10
	cmp r7, #0xa
	blt _021E601A
	b _021E6054
_021E6030:
	add r5, r6, #0
	ldr r4, _021E607C ; =sBattleRoomResultsLeaderWindowTemplates
	mov r7, #0
	add r5, #0x6c
_021E6038:
	ldr r0, [r6, #0x10]
	add r1, r5, #0
	add r2, r4, #0
	bl AddWindow
	add r0, r5, #0
	mov r1, #0
	bl FillWindowPixelBuffer
	add r7, r7, #1
	add r4, #8
	add r5, #0x10
	cmp r7, #0x23
	blt _021E6038
_021E6054:
	ldr r1, [r6]
	mov r0, #4
	bl FontID_Alloc
	mov r1, #0x1e
	ldr r2, [r6]
	mov r0, #0
	lsl r1, r1, #4
	bl LoadFontPal0
	mov r1, #0x1e
	ldr r2, [r6]
	mov r0, #4
	lsl r1, r1, #4
	bl LoadFontPal0
	pop {r3, r4, r5, r6, r7, pc}
	nop
_021E6078: .word sBattleRoomResultsWindowTemplates
_021E607C: .word sBattleRoomResultsLeaderWindowTemplates
	thumb_func_end BattleRoomResults_AddWindows
