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

	thumb_func_start LinkBattleScreen_Start
LinkBattleScreen_Start: ; 0x0221BA00
	push {r3, r4, r5, lr}
	add r5, r0, #0
	ldr r0, _0221BA5C ; =LinkBattleScreen_RunTask
	ldr r1, _0221BA60 ; =0x00000BD4
	ldr r3, [r5, #0x24]
	mov r2, #0x64
	bl CreateSysTaskAndEnvironment
	bl SysTask_GetData
	ldr r2, _0221BA60 ; =0x00000BD4
	mov r1, #0
	add r4, r0, #0
	bl memset
	ldr r0, _0221BA64 ; =0x00000B7F
	str r5, [r4]
	mov r1, #0
	strb r1, [r4, r0]
	bl sub_020304B4
	ldr r1, _0221BA68 ; =0x00000BC8
	str r0, [r4, r1]
	ldr r0, [r4, r1]
	cmp r0, #1
	bne _0221BA58
	ldr r0, [r4]
	ldr r1, [r0]
	cmp r1, #0
	beq _0221BA58
	mov r0, #7
	lsl r0, r0, #6
	ldr r0, [r1, r0]
	cmp r0, #0
	beq _0221BA58
	bl Save_Bag_Get
	ldr r1, _0221BA6C ; =0x000001D1
	ldr r3, [r5, #0x24]
	mov r2, #1
	bl Bag_HasItem
	ldr r1, _0221BA68 ; =0x00000BC8
	str r0, [r4, r1]
_0221BA58:
	pop {r3, r4, r5, pc}
	nop
_0221BA5C: .word LinkBattleScreen_RunTask
_0221BA60: .word 0x00000BD4
_0221BA64: .word 0x00000B7F
_0221BA68: .word 0x00000BC8
_0221BA6C: .word 0x000001D1
	thumb_func_end LinkBattleScreen_Start


	thumb_func_start LinkBattleScreen_RunTask
LinkBattleScreen_RunTask: ; 0x0221BA70
	push {r4, r5, r6, lr}
	ldr r2, _0221BAF8 ; =0x00000B7F
	add r5, r1, #0
	ldrb r2, [r5, r2]
	cmp r2, #0
	beq _0221BA86
	cmp r2, #1
	beq _0221BA92
	cmp r2, #2
	beq _0221BAC4
	b _0221BACC
_0221BA86:
	add r0, r5, #0
	bl LinkBattleScreen_Init
	ldr r1, _0221BAF8 ; =0x00000B7F
	strb r0, [r5, r1]
	b _0221BACC
_0221BA92:
	mov r4, #0x2e
	lsl r4, r4, #6
	add r6, r4, #0
_0221BA98:
	ldrb r1, [r5, r6]
	ldr r2, [r5, #4]
	add r0, r5, #0
	lsl r1, r1, #2
	ldr r1, [r2, r1]
	blx r1
	cmp r0, #0
	beq _0221BAAE
	ldrb r1, [r5, r4]
	add r1, r1, #1
	strb r1, [r5, r4]
_0221BAAE:
	cmp r0, #2
	beq _0221BA98
	ldr r0, [r5]
	add r0, #0x28
	ldrb r0, [r0]
	cmp r0, #2
	bne _0221BACC
	add r0, r5, #0
	bl LinkBattleScreen_UpdateBattleVideoPrompt
	b _0221BACC
_0221BAC4:
	bl LinkBattleScreen_Finish
	cmp r0, #1
	beq _0221BAF6
_0221BACC:
	mov r0, #0x2e
	lsl r0, r0, #6
	ldrb r0, [r5, r0]
	cmp r0, #0
	beq _0221BAE6
	add r0, r5, #0
	bl LinkBattleScreen_UpdateSpriteAnims
	mov r0, #0x65
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl SpriteSystem_DrawSprites
_0221BAE6:
	ldr r0, [r5]
	add r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	beq _0221BAF6
	ldr r0, _0221BAFC ; =0x04000540
	mov r1, #1
	str r1, [r0]
_0221BAF6:
	pop {r4, r5, r6, pc}
	.balign 4, 0
_0221BAF8: .word 0x00000B7F
_0221BAFC: .word 0x04000540
	thumb_func_end LinkBattleScreen_RunTask
