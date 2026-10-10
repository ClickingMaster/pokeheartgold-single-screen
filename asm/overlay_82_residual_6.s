	.include "asm/macros.inc"
	.include "overlay_82.inc"
	.include "global.inc"

	.text
	.public _0223FE20
	.public ov82_0223DD60
	.public ov82_0223DE20
	.public ov82_0223DF74
	.public BattleHallApp_FadeIn
	.public BattleHallApp_PrintTypeGrid
	.public BattleHallApp_PrintTitle
	.public BattleHallApp_DrawFrame
	.public BattleHallApp_RunSummary
	.public BattleHallApp_SelectType
	.public BattleHallApp_LinkDecide
	.public BattleHallApp_FadeOut
	.public BattleHallApp_ClosePrompt
	.public BattleHallApp_FreeGraphics
	.public BattleHallApp_ResumeAfterSummary
	.public BattleHallApp_InitGraphics
	.public BattleHallApp_InitBgGraphics
	.public BattleHallApp_FreeBgLayers
	.public BattleHallApp_VBlankCallback
	.public BattleHallApp_InitBgLayers
	.public BattleHallApp_InitLinkWindow
	.public BattleHallApp_LoadTypeGridGraphics
	.public BattleHallApp_LoadSubBgGraphics
	.public BattleHallApp_LoadTitleFrameGraphics
	.public BattleHallApp_PrintMessage
	.public BattleHallApp_PrintMonName
	.public BattleHallApp_PrintTypeNames
	.public BattleHallApp_PrintTypeRanks
	.public BattleHallApp_PrintSelectPrompt
	.public BattleHallApp_CreateSummaryArgs
	.public BattleHallApp_HandleDpad
	.public BattleHallApp_HandleTouch
	.public BattleHallApp_SetCellPalette
	.public BattleHallApp_ShowConfirmPrompt
	.public BattleHallSpriteGfx_Init
	.public BattleHallSpriteGfx_CreateSprite
	.public BattleHallSpriteGfx_Free
	.public BattleHallSprite_Create
	.public BattleHallApp_InitYesNo
	.public ov82_0223FE28
	.public ov82_0223FE38
	.public ov82_0223FE54
	.public ov82_0223FE70
	.public ov82_0223FE8C
	.public ov82_0223FEA8
	.public ov82_0223FEC4
	.public ov82_0223FEEC
	.public ov82_0223FEF0
	.public ov82_0223FF00
	.public BattleHallApp_LinkSync
	.public BattleHallApp_DisableDisplay
	.public BattleHallApp_InitSprites
	.public BattleHallApp_SetGraphicsBanks
	.public BattleHallApp_LoadBgPalette
	.public BattleHallApp_ShowMessage
	.public BattleHallApp_BufferNumber
	.public BattleHallApp_PrintNumber
	.public BattleHallApp_SetMainState
	.public BattleHallApp_HandleConfirm
	.public BattleHallApp_GetCursorX
	.public BattleHallApp_GetCursorY
	.public BattleHallApp_GrayOutTypes
	.public BattleHallApp_GetRecordIndex
	.public BattleHallApp_IsMatronBattle
	.public BattleHallApp_IsMatronMode
	.public BattleHallApp_SendLinkCommand
	.public BattleHallApp_PrepareLinkPing
	.public ov82_0223F764
	.public BattleHallApp_PrepareLinkChoice
	.public ov82_0223F7B4
	.public BattleHallApp_PrepareLinkAnswer
	.public ov82_0223F814
	.public BattleHallApp_ResetYesNo
	.public BattleHallApp_CancelConfirm
	.public BattleHallApp_ClearMessage
	.public BattleHallApp_SetBrightness
	.public BattleHallSpriteGfx_InitTransfers
	.public BattleHallSprite_Delete
	.public BattleHallSprite_SetVisible
	.public BattleHallSprite_SetPosition
	.public BattleHallSprite_SetAnim
	.public BattleHallSprite_SetIconPalette
	.public BattleHallApp_AddWindows
	.public BattleHallApp_RemoveWindows
	.public BattleHallApp_DrawMessageFrame
	.public BattleHallApp_CreateYesNo
	.public BattleHallApp_DestroyYesNo
	.public BattleHallApp_HandleYesNo

	thumb_func_start BattleHallApp_LoadSubBgGraphics
BattleHallApp_LoadSubBgGraphics: ; 0x0223EE6C
	push {r3, r4, r5, lr}
	sub sp, #0x10
	add r5, r0, #0
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	add r4, r1, #0
	mov r0, #0x69
	str r0, [sp, #0xc]
	mov r0, #0x22
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	ldr r2, [r5, #0x48]
	mov r1, #0x81
	add r3, r4, #0
	bl GfGfxLoader_LoadCharDataFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	mov r0, #0x69
	str r0, [sp, #0xc]
	mov r0, #0x22
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	ldr r2, [r5, #0x48]
	mov r1, #0x82
	add r3, r4, #0
	bl GfGfxLoader_LoadScrnDataFromOpenNarc
	mov r0, #0x20
	str r0, [sp]
	mov r0, #0x69
	str r0, [sp, #4]
	mov r0, #0x22
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	mov r1, #0xbe
	mov r2, #4
	mov r3, #0
	bl GfGfxLoader_GXLoadPalFromOpenNarc
	add sp, #0x10
	pop {r3, r4, r5, pc}
	thumb_func_end BattleHallApp_LoadSubBgGraphics


	thumb_func_start BattleHallApp_LoadTitleFrameGraphics
BattleHallApp_LoadTitleFrameGraphics: ; 0x0223EECC
	push {r3, r4, r5, lr}
	sub sp, #0x10
	add r4, r1, #0
	add r5, r0, #0
	lsl r0, r4, #0x18
	lsr r0, r0, #0x18
	mov r1, #0x20
	mov r2, #0
	mov r3, #0x69
	bl BG_ClearCharDataRange
	mov r1, #1
	str r1, [sp]
	mov r0, #0
	str r0, [sp, #4]
	str r1, [sp, #8]
	mov r0, #0x69
	str r0, [sp, #0xc]
	mov r0, #0x22
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	ldr r2, [r5, #0x48]
	mov r1, #0x85
	add r3, r4, #0
	bl GfGfxLoader_LoadCharDataFromOpenNarc
	mov r3, #0x20
	str r3, [sp]
	mov r0, #0x69
	str r0, [sp, #4]
	mov r0, #0x22
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	mov r1, #0xc0
	mov r2, #4
	bl GfGfxLoader_GXLoadPalFromOpenNarc
	add sp, #0x10
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end BattleHallApp_LoadTitleFrameGraphics


	thumb_func_start BattleHallApp_PrintMessage
BattleHallApp_PrintMessage: ; 0x0223EF1C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r4, r1, #0
	add r1, sp, #0x38
	ldrb r1, [r1]
	add r5, r0, #0
	add r0, r4, #0
	add r6, r2, #0
	add r7, r3, #0
	bl FillWindowPixelBuffer
	ldr r0, [r5, #0x20]
	ldr r2, [r5, #0x2c]
	add r1, r6, #0
	bl ReadMsgDataIntoString
	ldr r0, [r5, #0x24]
	ldr r1, [r5, #0x28]
	ldr r2, [r5, #0x2c]
	bl StringExpandPlaceholders
	ldr r0, [sp, #0x28]
	add r2, sp, #0x18
	str r0, [sp]
	ldr r0, [sp, #0x2c]
	add r3, r7, #0
	str r0, [sp, #4]
	add r0, sp, #0x38
	ldrb r1, [r0]
	ldrb r0, [r2, #0x18]
	ldrb r2, [r2, #0x1c]
	lsl r0, r0, #0x18
	lsl r2, r2, #0x18
	lsr r0, r0, #8
	lsr r2, r2, #0x10
	orr r0, r2
	orr r0, r1
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	add r1, sp, #0x3c
	ldrb r1, [r1]
	ldr r2, [r5, #0x28]
	add r0, r4, #0
	bl AddTextPrinterParameterizedWithColor
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end BattleHallApp_PrintMessage
