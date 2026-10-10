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
	thumb_func_start BattleRoomResults_PrintString
BattleRoomResults_PrintString: ; 0x021E6118
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r6, r0, #0
	ldr r0, [sp, #0x30]
	add r7, r1, #0
	add r5, r2, #0
	add r4, r3, #0
	cmp r0, #1
	bne _021E6136
	ldr r0, [sp, #0x28]
	mov r2, #0
	bl FontID_String_GetWidth
	sub r5, r5, r0
	b _021E6146
_021E6136:
	cmp r0, #2
	bne _021E6146
	ldr r0, [sp, #0x28]
	mov r2, #0
	bl FontID_String_GetWidth
	lsr r0, r0, #1
	sub r5, r5, r0
_021E6146:
	str r4, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, [sp, #0x2c]
	ldr r1, [sp, #0x28]
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	add r0, r6, #0
	add r2, r7, #0
	add r3, r5, #0
	bl AddTextPrinterParameterizedWithColor
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end BattleRoomResults_PrintString


	thumb_func_start BattleRoomResults_PrintMessage
BattleRoomResults_PrintMessage: ; 0x021E6164
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r5, r0, #0
	ldr r0, [r5, #0x28]
	add r4, r1, #0
	add r1, r2, #0
	add r7, r3, #0
	bl NewString_ReadMsgData
	add r6, r0, #0
	ldr r0, [sp, #0x24]
	add r5, #0x6c
	str r0, [sp]
	ldr r0, [sp, #0x28]
	ldr r3, [sp, #0x20]
	str r0, [sp, #4]
	ldr r0, [sp, #0x2c]
	add r1, r6, #0
	str r0, [sp, #8]
	lsl r0, r4, #4
	add r0, r5, r0
	add r2, r7, #0
	bl BattleRoomResults_PrintString
	add r0, r6, #0
	bl String_Delete
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end BattleRoomResults_PrintMessage


	thumb_func_start BattleRoomResults_PrintFormattedMessage
BattleRoomResults_PrintFormattedMessage: ; 0x021E61A0
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r5, r0, #0
	ldr r0, [r5, #0x28]
	add r4, r1, #0
	add r1, r2, #0
	add r6, r3, #0
	bl NewString_ReadMsgData
	add r7, r0, #0
	ldr r0, [r5, #0x2c]
	ldr r1, [r5, #0x30]
	add r2, r7, #0
	bl StringExpandPlaceholders
	ldr r0, [sp, #0x24]
	add r1, r5, #0
	str r0, [sp]
	ldr r0, [sp, #0x28]
	add r1, #0x6c
	str r0, [sp, #4]
	ldr r0, [sp, #0x2c]
	ldr r3, [sp, #0x20]
	str r0, [sp, #8]
	lsl r0, r4, #4
	add r0, r1, r0
	ldr r1, [r5, #0x30]
	add r2, r6, #0
	bl BattleRoomResults_PrintString
	add r0, r7, #0
	bl String_Delete
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end BattleRoomResults_PrintFormattedMessage
