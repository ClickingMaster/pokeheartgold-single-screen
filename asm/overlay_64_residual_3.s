	.include "asm/macros.inc"
	.include "overlay_64.inc"
	.include "global.inc"
	.public HallOfFameShowcase_Init
	.public HallOfFameShowcase_Main
	.public _021E6E7C
	.public HallOfFameShowcase_InitBgs
	.public HallOfFameShowcase_InitSprites
	.public HallOfFameShowcase_InitStrings
	.public HallOfFameShowcase_InitWindows
	.public HallOfFameShowcase_HandleInput
	.public HallOfFameShowcase_ChangeRecord
	.public HallOfFameShowcase_ShowMonDetail
	.public HallOfFameShowcase_AnimateButtonPress
	.public HallOfFameShowcase_CreatePartyIcon
	.public sShowcaseGraphicsModes
	.public sShowcaseMonPicRect
	.public sShowcaseGridCallbacks
	.public sShowcaseOamCharTransferParam
	.public sShowcaseIconPositions
	.public sShowcaseIconPositionsY
	.public sShowcaseSpriteResourceCounts
	.public sShowcaseBgTemplate_Main1
	.public sShowcaseBgTemplate_Sub0
	.public sShowcaseBgTemplate_Sub1
	.public sShowcaseBgTemplate_Main0
	.public sShowcaseGridHitboxes
	.public sShowcaseOamManagerParam
	.public sShowcaseGraphicsBanks
	.public sShowcasePartyIconSpriteTemplate
	.public sShowcaseGridDpadBoxes
	.public sShowcaseWindowTemplates
	.public sShowcaseMonPicSpriteTemplate1
	.public sShowcaseMonPicSpriteTemplate2
	.public sShowcaseCursorSpriteTemplate
	.public sShowcasePrevPageSpriteTemplate
	.public sShowcaseNextPageSpriteTemplate
	.public sShowcaseExitSpriteTemplate


	.text
	.public HallOfFameShowcase_Exit
	.public HallOfFameShowcase_VBlankCB
	.public HallOfFameShowcase_LoadMainObjChar
	.public HallOfFameShowcase_LoadSubObjChar
	.public HallOfFameShowcase_LoadMainObjPltt
	.public HallOfFameShowcase_SetGfxBanks
	.public HallOfFameShowcase_FreeBgs
	.public HallOfFameShowcase_DeleteSprite
	.public HallOfFameShowcase_FreeSprites
	.public HallOfFameShowcase_TickSprites
	.public HallOfFameShowcase_FreeStrings
	.public HallOfFameShowcase_FreeWindows
	.public HallOfFameShowcase_CreateGridInput
	.public HallOfFameShowcase_FreeGridInput
	.public HallOfFameShowcase_GridOnButton
	.public HallOfFameShowcase_GridOnSwitchToTouchMode
	.public HallOfFameShowcase_GridOnTouch
	.public HallOfFameShowcase_GridOnKeyMove
	.public HallOfFameShowcase_BeginFadeIn
	.public HallOfFameShowcase_BeginFadeOut
	.public HallOfFameShowcase_DeletePartyIcons
	.public HallOfFameShowcase_HideMonDetail
	.public HallOfFameShowcase_MoveCursor
	.public HallOfFameShowcase_GetOverworldModelFileId

	thumb_func_start HallOfFameShowcase_InitBgs
HallOfFameShowcase_InitBgs: ; 0x021E5B10
	push {r3, r4, r5, lr}
	sub sp, #0x90
	add r5, r0, #0
	mov r0, #0x3b
	bl BgConfig_Alloc
	add r3, sp, #0x80
	ldr r4, _021E5C90 ; =sShowcaseGraphicsModes
	str r0, [r5, #4]
	add r2, r3, #0
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	add r0, r2, #0
	bl SetBothScreensModesAndDisable
	ldr r4, _021E5C94 ; =sShowcaseBgTemplate_Main0
	add r3, sp, #0x64
	ldmia r4!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r4]
	mov r1, #0
	str r0, [r3]
	ldr r0, [r5, #4]
	add r3, r1, #0
	bl InitBgFromTemplate
	ldr r0, [r5, #4]
	mov r1, #0
	bl BgClearTilemapBufferAndCommit
	mov r0, #0
	mov r1, #0x20
	add r2, r0, #0
	mov r3, #0x3b
	bl BG_ClearCharDataRange
	ldr r4, _021E5C98 ; =sShowcaseBgTemplate_Main1
	add r3, sp, #0x48
	ldmia r4!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r4]
	mov r1, #1
	str r0, [r3]
	ldr r0, [r5, #4]
	mov r3, #0
	bl InitBgFromTemplate
	ldr r4, _021E5C9C ; =sShowcaseBgTemplate_Sub0
	add r3, sp, #0x2c
	ldmia r4!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r4]
	mov r1, #4
	str r0, [r3]
	ldr r0, [r5, #4]
	mov r3, #0
	bl InitBgFromTemplate
	ldr r0, [r5, #4]
	mov r1, #4
	bl BgClearTilemapBufferAndCommit
	mov r0, #4
	mov r1, #0x20
	mov r2, #0
	mov r3, #0x3b
	bl BG_ClearCharDataRange
	ldr r4, _021E5CA0 ; =sShowcaseBgTemplate_Sub1
	add r3, sp, #0x10
	ldmia r4!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r4]
	mov r1, #5
	str r0, [r3]
	ldr r0, [r5, #4]
	mov r3, #0
	bl InitBgFromTemplate
	mov r0, #0x61
	mov r1, #0x3b
	bl NARC_New
	mov r1, #0
	str r1, [sp]
	str r1, [sp, #4]
	mov r3, #1
	str r3, [sp, #8]
	mov r2, #0x3b
	str r2, [sp, #0xc]
	ldr r2, [r5, #4]
	add r4, r0, #0
	bl GfGfxLoader_LoadScrnDataFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	mov r1, #1
	str r1, [sp, #8]
	mov r0, #0x3b
	str r0, [sp, #0xc]
	ldr r2, [r5, #4]
	add r0, r4, #0
	add r3, r1, #0
	bl GfGfxLoader_LoadCharDataFromOpenNarc
	mov r0, #0x20
	str r0, [sp]
	mov r0, #0x3b
	mov r2, #0
	str r0, [sp, #4]
	add r0, r4, #0
	mov r1, #2
	add r3, r2, #0
	bl GfGfxLoader_GXLoadPalFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	mov r0, #0x3b
	str r0, [sp, #0xc]
	ldr r2, [r5, #4]
	add r0, r4, #0
	mov r1, #3
	mov r3, #5
	bl GfGfxLoader_LoadScrnDataFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	mov r0, #0x3b
	str r0, [sp, #0xc]
	ldr r2, [r5, #4]
	add r0, r4, #0
	mov r1, #4
	mov r3, #5
	bl GfGfxLoader_LoadCharDataFromOpenNarc
	mov r0, #0x20
	str r0, [sp]
	mov r0, #0x3b
	str r0, [sp, #4]
	add r0, r4, #0
	mov r1, #5
	mov r2, #4
	mov r3, #0
	bl GfGfxLoader_GXLoadPalFromOpenNarc
	add r0, r4, #0
	bl NARC_Delete
	mov r1, #0x1e
	mov r0, #0
	lsl r1, r1, #4
	mov r2, #0x3b
	bl LoadFontPal0
	mov r1, #0x1e
	mov r0, #4
	lsl r1, r1, #4
	mov r2, #0x3b
	bl LoadFontPal0
	add sp, #0x90
	pop {r3, r4, r5, pc}
	nop
_021E5C90: .word sShowcaseGraphicsModes
_021E5C94: .word sShowcaseBgTemplate_Main0
_021E5C98: .word sShowcaseBgTemplate_Main1
_021E5C9C: .word sShowcaseBgTemplate_Sub0
_021E5CA0: .word sShowcaseBgTemplate_Sub1
	thumb_func_end HallOfFameShowcase_InitBgs
