	.include "asm/macros.inc"
	.public LinkBattleScreen_Start
	.public LinkBattleScreen_RunTask
	.public LinkBattleScreen_SetVramBanks
	.public LinkBattleScreen_UpdateBattleVideoPrompt
	.public LinkBattleScreen_Init
	.public LinkBattleScreen_InitPartyScreen
	.public LinkBattleScreen_InitVsScreen
	.public LinkBattleScreen_InitResultScreen
	.public LinkBattleScreen_WaitFadeAndDelay
	.public LinkBattleScreen_SlidePanelsIn
	.public LinkBattleScreen_ShakePanels
	.public LinkBattleScreen_SlidePanelsOut
	.public ov05_0221C558
	.public LinkBattleScreen_DetermineNudgeDirection
	.public LinkBattleScreen_NudgePanels
	.public LinkBattleScreen_NudgePanelsRight
	.public LinkBattleScreen_NudgePanelsLeft
	.public ov05_0221C7FC
	.public LinkBattleScreen_RevealCenterBand
	.public LinkBattleScreen_ShowWinLoseSprites
	.public LinkBattleScreen_ShowDrawSprite
	.public LinkBattleScreen_SaveBattleVideo
	.public LinkBattleScreen_WaitForPartner
	.public LinkBattleScreen_ApplyOutcomePalette
	.public LinkBattleScreen_SlidePartyPanelsIn
	.public LinkBattleScreen_StartExit
	.public LinkBattleScreen_Finish
	.public LinkBattleScreen_IsLocalPlayerOnLeft
	.public LinkBattleScreen_InitSpriteSystem
	.public LinkBattleScreen_CreateSprite
	.public LinkBattleScreen_FreeSprites
	.public LinkBattleScreen_UpdateSpriteAnims
	.public LinkBattleScreen_VBlankCallback
	.public LinkBattleScreen_InitVsBgLayers
	.public LinkBattleScreen_RemoveMessageWindows
	.public LinkBattleScreen_FreeMessageData
	.public LinkBattleScreen_FreeVsBgLayers
	.public LinkBattleScreen_LoadVsBgGraphics
	.public LinkBattleScreen_InitMessageWindows
	.public LinkBattleScreen_SetBg1AlphaBlend
	.public LinkBattleScreen_InitVsSprites
	.public LinkBattleScreen_LoadPartyBallGfx
	.public LinkBattleScreen_SetBallRowPosition
	.public LinkBattleScreen_SetPartyBallAnims
	.public LinkBattleScreen_SetAllPartyBallAnims
	.public LinkBattleScreen_CreateNameTextObj
	.public LinkBattleScreen_CreateNameTextObjs
	.public LinkBattleScreen_SetNamePosition
	.public LinkBattleScreen_FreeNameTextObjs
	.public LinkBattleScreen_InitLayout
	.public LinkBattleScreen_InitResultSprites
	.public LinkBattleScreen_LoadOutcomeGfx
	.public LinkBattleScreen_SetupOutcomeSprites
	.public LinkBattleScreen_InitParticles
	.public LinkBattleScreen_UpdateParticles
	.public LinkBattleScreen_FreeParticles
	.public LinkBattleScreen_AllocParticleTexVram
	.public LinkBattleScreen_AllocParticlePlttVram
	.public LinkBattleScreen_InitPartyBgLayers
	.public LinkBattleScreen_FreePartyBgLayers
	.public LinkBattleScreen_LoadPartyBgGraphics
	.public LinkBattleScreen_DrawPartyPanels
	.public LinkBattleScreen_StashPanelTilemap
	.public LinkBattleScreen_StashPanelTilemaps
	.public LinkBattleScreen_RevealPanelColumns
	.public LinkBattleScreen_ReadPartyMonInfo
	.public LinkBattleScreen_ReadMonInfo
	.public LinkBattleScreen_GetMonIconAnim
	.public LinkBattleScreen_CreateMonSprites
	.public LinkBattleScreen_InitPartySprites
	.public LinkBattleScreen_LoadMonIconGfx
	.public LinkBattleScreen_LoadItemIconGfx
	.public LinkBattleScreen_LoadStatusIconGfx
	.public ov05_0221E4C8
	.public LinkBattleScreen_SetPartySpriteOffset
	.public LinkBattleScreen_AddPartyWindows
	.public LinkBattleScreen_RemovePartyWindows
	.public LinkBattleScreen_PrintMonName
	.public LinkBattleScreen_PrintMonLevel
	.public LinkBattleScreen_PrintMonHp
	.public LinkBattleScreen_DrawMonHpBar
	.public LinkBattleScreen_DrawMonPanelText
	.public LinkBattleScreen_DrawAllMonPanelText
	.public LinkBattleScreen_DrawMessageBox
	.public LinkBattleScreen_IsBattleRecordActive
	.public LinkBattleScreen_ShowWaitingIcon
	.public LinkBattleScreen_HideWaitingIcon
	.public sBallRowFirstSprite
	.public sBattleVideoYesNoTemplate
	.public sPartyPanelPositions
	.public sPanelNudgeSteps
	.public ov05_0221EA6D
	.public sVsGraphicsModes
	.public sPartyGraphicsModes
	.public sVsSequence
	.public sLinkBattleScreenOamTransferParam
	.public sPartySequence
	.public sResultSpriteResourceCounts
	.public sPartySpriteResourceCounts
	.public sVsSpriteResourceCounts
	.public sVsBg3Template
	.public sPartyBg2Template
	.public sVsBg2Template
	.public sVsBg1Template
	.public sPartyBg1Template
	.public sVsBg0Template
	.public sPartyBg3Template
	.public sLinkBattleScreenOamParam
	.public sVramBanks
	.public sResultDrawSequence
	.public sMonSpritePositions
	.public sResultSequence
	.public sPartyWindowTemplates
	.public sVsSpriteTemplates
	.public sPartySpriteTemplates
	.include "overlay_05.inc"
	.include "global.inc"

	.text

	thumb_func_start LinkBattleScreen_AllocParticleTexVram
LinkBattleScreen_AllocParticleTexVram: ; 0x0221DB4C
	push {r4, lr}
	ldr r3, _0221DB6C ; =NNS_GfdDefaultFuncAllocTexVram
	mov r2, #0
	ldr r3, [r3]
	blx r3
	add r4, r0, #0
	bl ParticleSystem_RegisterTexKey
	cmp r4, #0
	bne _0221DB64
	bl GF_AssertFail
_0221DB64:
	lsl r0, r4, #0x10
	lsr r0, r0, #0xd
	pop {r4, pc}
	nop
_0221DB6C: .word NNS_GfdDefaultFuncAllocTexVram
	thumb_func_end LinkBattleScreen_AllocParticleTexVram


	thumb_func_start LinkBattleScreen_AllocParticlePlttVram
LinkBattleScreen_AllocParticlePlttVram: ; 0x0221DB70
	push {r4, lr}
	ldr r3, _0221DB90 ; =NNS_GfdDefaultFuncAllocPlttVram
	mov r2, #1
	ldr r3, [r3]
	blx r3
	add r4, r0, #0
	bl ParticleSystem_RegisterPlttKey
	cmp r4, #0
	bne _0221DB88
	bl GF_AssertFail
_0221DB88:
	lsl r0, r4, #0x10
	lsr r0, r0, #0xd
	pop {r4, pc}
	nop
_0221DB90: .word NNS_GfdDefaultFuncAllocPlttVram
	thumb_func_end LinkBattleScreen_AllocParticlePlttVram


	thumb_func_start LinkBattleScreen_InitPartyBgLayers
LinkBattleScreen_InitPartyBgLayers: ; 0x0221DB94
	push {r4, r5, lr}
	sub sp, #0x64
	ldr r5, _0221DC24 ; =sPartyGraphicsModes
	add r3, sp, #0x54
	add r4, r0, #0
	add r2, r3, #0
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	add r0, r2, #0
	bl SetBothScreensModesAndDisable
	ldr r5, _0221DC28 ; =sPartyBg1Template
	add r3, sp, #0x38
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #1
	str r0, [r3]
	ldr r0, [r4, #0xc]
	mov r3, #0
	bl InitBgFromTemplate
	ldr r0, [r4, #0xc]
	mov r1, #1
	bl BgClearTilemapBufferAndCommit
	ldr r5, _0221DC2C ; =sPartyBg2Template
	add r3, sp, #0x1c
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #2
	str r0, [r3]
	ldr r0, [r4, #0xc]
	mov r3, #0
	bl InitBgFromTemplate
	ldr r0, [r4, #0xc]
	mov r1, #2
	bl BgClearTilemapBufferAndCommit
	ldr r5, _0221DC30 ; =sPartyBg3Template
	add r3, sp, #0
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #3
	str r0, [r3]
	ldr r0, [r4, #0xc]
	mov r3, #0
	bl InitBgFromTemplate
	add sp, #0x64
	pop {r4, r5, pc}
	nop
_0221DC24: .word sPartyGraphicsModes
_0221DC28: .word sPartyBg1Template
_0221DC2C: .word sPartyBg2Template
_0221DC30: .word sPartyBg3Template
	thumb_func_end LinkBattleScreen_InitPartyBgLayers
