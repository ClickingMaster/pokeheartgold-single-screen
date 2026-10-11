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

	thumb_func_start PlazaGameEntry_Free
PlazaGameEntry_Free: ; 0x02258938
	push {r4, r5, r6, lr}
	add r5, r0, #0
	ldr r0, _022589B8 ; =0x000005EC
	ldr r0, [r5, r0]
	bl SysTask_Destroy
	mov r0, #0x5f
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	bl SysTask_Destroy
	mov r0, #0xb9
	lsl r0, r0, #2
	add r0, r5, r0
	bl PlazaGameLineScroll_Free
	mov r0, #0x73
	lsl r0, r0, #2
	add r0, r5, r0
	bl PlazaGameTitle_Free
	ldrb r0, [r5, #0x10]
	mov r6, #0
	cmp r0, #0
	ble _02258980
	mov r0, #0x81
	lsl r0, r0, #2
	add r4, r5, r0
_02258970:
	add r0, r4, #0
	bl PlazaGamePlayer_Free
	ldrb r0, [r5, #0x10]
	add r6, r6, #1
	add r4, #0x38
	cmp r6, r0
	blt _02258970
_02258980:
	add r0, r5, #0
	add r0, #0x4c
	bl PlazaGameMessageBox_Free
	add r0, r5, #0
	bl PlazaGameEntry_FreeGraphics
	add r0, r5, #0
	add r0, #0x3c
	bl PlazaGameText_Free
	mov r0, #0x1b
	lsl r0, r0, #4
	add r0, r5, r0
	bl PlazaGameGfx_Free
	add r0, r5, #0
	add r0, #0x84
	bl PlazaGameSprites_Free
	add r0, r5, #0
	add r0, #0x30
	bl PlazaGameBg_Free
	add r0, r5, #0
	bl Heap_Free
	pop {r4, r5, r6, pc}
	.balign 4, 0
_022589B8: .word 0x000005EC
	thumb_func_end PlazaGameEntry_Free
