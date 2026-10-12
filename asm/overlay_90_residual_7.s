	.include "asm/macros.inc"
	.include "overlay_90.inc"
	.include "global.inc"

	.text
	.public _0225C1E0
	.public PlazaGameRoster_Init
	.public PlazaGameRoster_GetNetId
	.public ov90_022588A4
	.public PlazaGameRoster_GetProfile
	.public PlazaGameEntry_Free
	.public PlazaGameResults_Free
	.public PlazaGame_CalcRanks
	.public PlazaGameResults_ReceiveVote
	.public PlazaGameRoster_CheckConnectionLost
	.public PlazaGameRoster_ProcessConnectionLost
	.public PlazaGameSign_New
	.public PlazaGameSign_Free
	.public PlazaGameSign_StartCountdown
	.public PlazaGameSign_StartFinish
	.public PlazaGameMotion_Init
	.public PlazaGameMotion_Update
	.public PlazaGameGfx_Init
	.public PlazaGameGfx_Free
	.public PlazaGameGfx_Load
	.public PlazaGameGfx_CreateSprite
	.public PlazaGameGfx_LoadInto
	.public PlazaGameGfx_Unload
	.public PlazaGameBg_Init
	.public PlazaGameBg_Free
	.public PlazaGameSprites_Init
	.public PlazaGameText_Init
	.public PlazaGameText_Print
	.public PlazaGameText_PrintRightAligned
	.public PlazaGameText_PrintMessage
	.public PlazaGameText_PrintWithColor
	.public PlazaGameMessageBox_Init
	.public PlazaGameMessageBox_Free
	.public PlazaGameMessageBox_Show
	.public PlazaGameMessageBox_Hide
	.public PlazaGameMessageBox_HideWaitingIcon
	.public PlazaGameEntry_New
	.public PlazaGameEntry_LoadGraphics
	.public PlazaGameEntry_MainTask
	.public PlazaGameEntry_VBlankTask
	.public PlazaGame_PickWeighted
	.public PlazaGameEntry_UpdatePlayers
	.public PlazaGamePlayer_Init
	.public PlazaGamePlayer_PrintName
	.public PlazaGamePlayer_PrintLocation
	.public PlazaGamePlayer_ShowRank
	.public PlazaGamePlayer_PrintScore
	.public PlazaGamePlayer_Free
	.public PlazaGamePlayer_StartSlideIn
	.public PlazaGamePlayer_StartSlideOut
	.public PlazaGamePlayer_UpdateSlide
	.public PlazaGamePlayer_SetSpritePos
	.public PlazaGame_GetRelativeSeat
	.public PlazaGameLineScroll_Init
	.public PlazaGameLineScroll_Free
	.public PlazaGameLineScroll_SetPlayer
	.public PlazaGameLineScroll_HBlank
	.public PlazaGamePlayerInfo_Init
	.public PlazaGamePlayerInfo_HasDifferentCountries
	.public PlazaGame_GetPoweredUpTapToy
	.public PlazaGameTitle_Init
	.public PlazaGameTitle_StartSlideIn
	.public PlazaGameTitle_StartSlideOut
	.public PlazaGameTitle_UpdateSlideIn
	.public PlazaGameTitle_UpdateSlideOut
	.public PlazaGameTitle_SetScroll
	.public PlazaGameTitle_ScheduleScroll
	.public PlazaGame_LoadGamePalette
	.public PlazaGameResults_New
	.public PlazaGameResults_LoadGraphics
	.public PlazaGameResults_RankingTask
	.public PlazaGameResults_TeamTask
	.public PlazaGameResults_VBlankTask
	.public PlazaGameResults_LoadFlashPalette
	.public PlazaGameResults_StartFlash
	.public PlazaGameResults_UpdateFlash
	.public PlazaGameContinue_Init
	.public PlazaGameContinue_Run
	.public PlazaGameContinue_ReceiveVote
	.public PlazaGameBalloons_Init
	.public PlazaGameBalloons_Update
	.public PlazaGameResults_ShowPlayer
	.public PlazaGameResults_UpdatePlayers
	.public PlazaGameResults_IsFanfareDone
	.public PlazaGameResults_RecordFriendPlays
	.public PlazaGameResults_GetScoreThreshold
	.public PlazaGameResults_ReportWinners
	.public PlazaGameResults_ReportTeamWin
	.public PlazaGameSignWindow_Init
	.public PlazaGameSignWindow_Open
	.public PlazaGameSignWindow_Close
	.public PlazaGameSignWindow_Move
	.public PlazaGameSignWindow_Free
	.public PlazaGameSignWindow_UpdateLines
	.public PlazaGameSignWindow_CommitLinesTask
	.public PlazaGameSignWindow_HBlank
	.public PlazaGameSign_CountdownTask
	.public PlazaGameSign_FinishTask
	.public ov90_0225C1E4
	.public ov90_0225C1E8
	.public ov90_0225C1EC
	.public ov90_0225C1F0
	.public ov90_0225C1F8
	.public ov90_0225C204
	.public ov90_0225C210
	.public ov90_0225C21C
	.public ov90_0225C228
	.public ov90_0225C234
	.public ov90_0225C240
	.public ov90_0225C24C
	.public ov90_0225C258
	.public ov90_0225C264
	.public ov90_0225C274
	.public ov90_0225C276
	.public ov90_0225C284
	.public ov90_0225C294
	.public ov90_0225C2A4
	.public ov90_0225C2B4
	.public ov90_0225C2CC
	.public ov90_0225C2F4
	.public ov90_0225C310
	.public ov90_0225C314
	.public ov90_0225C318
	.public ov90_0225C31C
	.public ov90_0225C324
	.public ov90_0225C34C
	.public ov90_0225C39C
	.public ov90_0225C43C
	.public ov90_0225C4FC

	.public PlazaGameRoster_GetIndexOfNetId
	.public PlazaGameEntry_NewSwalotPlop
	.public PlazaGameEntry_NewMimeJrTop
	.public PlazaGameEntry_NewWobbuffetPop
	.public PlazaGameEntry_IsDone
	.public PlazaGameEntry_GetSelection
	.public PlazaGameResults_NewSwalotPlop
	.public PlazaGameResults_NewMimeJrTop
	.public PlazaGameResults_NewWobbuffetPop
	.public PlazaGameResults_IsDone
	.public PlazaGameResults_GetContinue
	.public PlazaGameEntry_SetSelection
	.public PlazaGameResults_ReceiveContinue
	.public PlazaGameSign_GetPlttOffset
	.public PlazaGameSign_IsDone
	.public PlazaGame_OffsetScreenTiles
	.public PlazaGameBg_Update
	.public PlazaGameSprites_Free
	.public PlazaGameSprites_Update
	.public PlazaGameSprites_ApplyOam
	.public PlazaGameText_Free
	.public PlazaGameText_ResetBuffers
	.public PlazaGameText_BufferPlayerName
	.public PlazaGameText_BufferCountry
	.public PlazaGameText_BufferCity
	.public PlazaGameText_BufferNumber
	.public PlazaGameText_BufferTwoDigitNumber
	.public PlazaGameText_BufferTapToyName
	.public PlazaGameText_BufferActivityName
	.public PlazaGameText_Format
	.public PlazaGameMessageBox_IsPrinterFinished
	.public PlazaGameMessageBox_ShowWaitingIcon
	.public PlazaGameEntry_FreeGraphics
	.public PlazaGamePlayer_ScheduleTransfer
	.public PlazaGamePlayer_ShowSprite
	.public PlazaGameLineScroll_Swap
	.public PlazaGameTitle_Free
	.public PlazaGameResults_FreeGraphics
	.public PlazaGameResults_FreeFlashPalette
	.public PlazaGameContinue_Free
	.public PlazaGameContinue_GetResult
	.public PlazaGameContinue_SetResult
	.public PlazaGameBalloons_Free
	.public PlazaGameBalloons_Start
	.public PlazaGameResults_PlayFanfare
	.public PlazaGame_GetCommandTable
	.public PlazaGame_GetCommandCount
	.public PlazaGameResults_InitCommands
	.public PlazaGameEntry_InitCommands
	.public PlazaGame_GetCommandSizeNone
	.public PlazaGame_GetCommandSizeU32
	.public PlazaGame_HandleSelectionCommand
	.public PlazaGame_HandleVoteYesCommand
	.public PlazaGame_HandleVoteNoCommand
	.public PlazaGame_HandleContinueCommand
	.public PlazaGame_HandleQuitCommand

	thumb_func_start PlazaGameRoster_CheckConnectionLost
PlazaGameRoster_CheckConnectionLost: ; 0x02258B54
	push {r3, r4, r5, lr}
	add r5, r0, #0
	ldr r0, [r5, #0xc]
	mov r4, #0
	cmp r0, #0
	beq _02258B94
	bl Comm_CountConnected
	ldrb r1, [r5, #8]
	cmp r1, r0
	beq _02258B6C
	mov r4, #1
_02258B6C:
	bl CommState_WasWifiDisconnected
	cmp r0, #1
	bne _02258B76
	mov r4, #1
_02258B76:
	bl CommState_GetWifiMatchStatus
	cmp r0, #2
	blt _02258B80
	mov r4, #1
_02258B80:
	cmp r4, #1
	bne _02258B8A
	ldr r0, [r5, #0xc]
	mov r1, #1
	str r1, [r0, #0x1c]
_02258B8A:
	ldr r0, [r5, #0xc]
	ldr r0, [r0, #0x1c]
	cmp r0, #1
	bne _02258B94
	mov r4, #1
_02258B94:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	thumb_func_end PlazaGameRoster_CheckConnectionLost


	thumb_func_start PlazaGameRoster_ProcessConnectionLost
PlazaGameRoster_ProcessConnectionLost: ; 0x02258B98
	push {r4, lr}
	add r4, r0, #0
	ldr r0, [r4, #0xc]
	cmp r0, #0
	bne _02258BA6
	bl GF_AssertFail
_02258BA6:
	ldr r0, [r4, #0xc]
	ldr r0, [r0, #0x1c]
	cmp r0, #1
	beq _02258BB2
	bl GF_AssertFail
_02258BB2:
	bl CommPlayerInfo_Exists
	cmp r0, #1
	bne _02258BC4
	bl CommPlayerInfo_Free
	bl CommState_EndPlazaMatch
	b _02258BD0
_02258BC4:
	bl CommState_IsWifiInLobby
	cmp r0, #1
	bne _02258BD0
	mov r0, #1
	pop {r4, pc}
_02258BD0:
	mov r0, #0
	pop {r4, pc}
	thumb_func_end PlazaGameRoster_ProcessConnectionLost


	thumb_func_start PlazaGameSign_New
PlazaGameSign_New: ; 0x02258BD4
	push {r3, r4, r5, r6, lr}
	sub sp, #0x14
	add r5, r1, #0
	mov r1, #0x73
	add r6, r0, #0
	add r0, r5, #0
	lsl r1, r1, #2
	bl Heap_Alloc
	mov r2, #0x73
	add r4, r0, #0
	mov r1, #0
	lsl r2, r2, #2
	bl memset
	str r6, [r4, #8]
	mov r0, #0xc8
	add r1, r5, #0
	bl NARC_New
	str r0, [r4, #0xc]
	add r0, r4, #0
	add r0, #0x10
	mov r1, #1
	add r2, r5, #0
	bl PlazaGameGfx_Init
	mov r0, #2
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	ldr r0, _02258C34 ; =0x00001388
	mov r2, #3
	str r0, [sp, #0xc]
	str r5, [sp, #0x10]
	add r0, r4, #0
	ldr r1, [r4, #0xc]
	add r0, #0x10
	add r3, r2, #0
	bl PlazaGameGfx_Load
	str r0, [r4, #0x28]
	add r0, r4, #0
	add sp, #0x14
	pop {r3, r4, r5, r6, pc}
	nop
_02258C34: .word 0x00001388
	thumb_func_end PlazaGameSign_New


	thumb_func_start PlazaGameSign_Free
PlazaGameSign_Free: ; 0x02258C38
	push {r4, lr}
	add r4, r0, #0
	ldr r0, [r4, #0x2c]
	cmp r0, #0
	beq _02258C4A
	bl SysTask_Destroy
	mov r0, #0
	str r0, [r4, #0x2c]
_02258C4A:
	ldr r0, [r4, #0x30]
	cmp r0, #0
	beq _02258C54
	bl Sprite_Delete
_02258C54:
	add r0, r4, #0
	add r0, #0x34
	bl PlazaGameSignWindow_Free
	add r0, r4, #0
	add r0, #0x10
	bl PlazaGameGfx_Free
	ldr r0, [r4, #0xc]
	bl NARC_Delete
	add r0, r4, #0
	bl Heap_Free
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end PlazaGameSign_Free
