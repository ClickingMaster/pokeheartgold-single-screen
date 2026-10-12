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

	thumb_func_start PlazaGameResults_IsFanfareDone
PlazaGameResults_IsFanfareDone: ; 0x0225B978
	push {r4, lr}
	add r4, r0, #0
	mov r0, #0x66
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	cmp r0, #1
	bne _0225B996
	bl IsFanfarePlaying
	cmp r0, #0
	bne _0225B996
	mov r0, #0x66
	mov r1, #0
	lsl r0, r0, #4
	str r1, [r4, r0]
_0225B996:
	mov r0, #0x66
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	cmp r0, #1
	beq _0225B9A4
	mov r0, #1
	pop {r4, pc}
_0225B9A4:
	mov r0, #0
	pop {r4, pc}
	thumb_func_end PlazaGameResults_IsFanfareDone


	thumb_func_start PlazaGameResults_RecordFriendPlays
PlazaGameResults_RecordFriendPlays: ; 0x0225B9A8
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	ldr r0, [r5, #0xc]
	bl Save_FriendRoster_Get
	add r4, r0, #0
	ldrb r0, [r5, #0x14]
	mov r6, #0
	cmp r0, #0
	ble _0225BA12
	add r7, sp, #0
_0225B9BE:
	add r0, r5, r6
	ldrb r0, [r0, #0x10]
	bl CommPlayerInfo_GetExchangeToken
	add r1, r0, #0
	ldr r0, [r5, #0xc]
	add r2, r7, #0
	bl Comm_FindFriendSlot
	cmp r0, #0
	beq _0225B9D8
	cmp r0, #1
	bne _0225BA0A
_0225B9D8:
	ldrb r0, [r5, #7]
	cmp r0, #0
	beq _0225B9E8
	cmp r0, #1
	beq _0225B9F4
	cmp r0, #2
	beq _0225BA00
	b _0225BA0A
_0225B9E8:
	ldr r1, [sp]
	add r0, r4, #0
	mov r2, #1
	bl sub_0202C554
	b _0225BA0A
_0225B9F4:
	ldr r1, [sp]
	add r0, r4, #0
	mov r2, #1
	bl sub_0202C584
	b _0225BA0A
_0225BA00:
	ldr r1, [sp]
	add r0, r4, #0
	mov r2, #1
	bl sub_0202C5B4
_0225BA0A:
	ldrb r0, [r5, #0x14]
	add r6, r6, #1
	cmp r6, r0
	blt _0225B9BE
_0225BA12:
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end PlazaGameResults_RecordFriendPlays


	thumb_func_start PlazaGameResults_GetScoreThreshold
PlazaGameResults_GetScoreThreshold: ; 0x0225BA14
	push {r3, lr}
	ldrb r1, [r0, #7]
	cmp r1, #0
	beq _0225BA26
	cmp r1, #1
	beq _0225BA2A
	cmp r1, #2
	beq _0225BA2E
	pop {r3, pc}
_0225BA26:
	mov r0, #0
	pop {r3, pc}
_0225BA2A:
	mov r0, #0
	pop {r3, pc}
_0225BA2E:
	bl GF_AssertFail
	mov r0, #0
	pop {r3, pc}
	.balign 4, 0
	thumb_func_end PlazaGameResults_GetScoreThreshold


	thumb_func_start PlazaGameResults_ReportWinners
PlazaGameResults_ReportWinners: ; 0x0225BA38
	push {r3, r4, r5, r6, lr}
	sub sp, #0x1c
	add r1, sp, #0xc
	mov r4, #0
	str r4, [r1]
	str r4, [r1, #4]
	str r4, [r1, #8]
	add r5, r0, #0
	str r4, [r1, #0xc]
	bl PlazaGameResults_GetScoreThreshold
	ldrb r2, [r5, #0x14]
	add r1, r4, #0
	cmp r2, #0
	ble _0225BA7E
	add r2, r5, #0
	add r3, sp, #0xc
_0225BA5A:
	add r6, r5, r1
	add r6, #0x2c
	ldrb r6, [r6]
	cmp r6, #0
	bne _0225BA74
	ldr r6, [r2, #0x1c]
	cmp r0, r6
	beq _0225BA74
	ldr r6, [r5, #0x18]
	add r4, r4, #1
	add r6, r6, r1
	ldrb r6, [r6, #4]
	stmia r3!, {r6}
_0225BA74:
	ldrb r6, [r5, #0x14]
	add r1, r1, #1
	add r2, r2, #4
	cmp r1, r6
	blt _0225BA5A
_0225BA7E:
	cmp r4, #0
	beq _0225BA9C
	ldr r0, [sp, #0x10]
	add r2, r4, #0
	str r0, [sp]
	ldr r0, [sp, #0x14]
	str r0, [sp, #4]
	ldr r0, [sp, #0x18]
	str r0, [sp, #8]
	ldr r0, [r5, #0x18]
	ldrb r1, [r5, #7]
	ldr r0, [r0, #0x30]
	ldr r3, [sp, #0xc]
	bl ov45_0222ACB8
_0225BA9C:
	add sp, #0x1c
	pop {r3, r4, r5, r6, pc}
	thumb_func_end PlazaGameResults_ReportWinners


	thumb_func_start PlazaGameResults_ReportTeamWin
PlazaGameResults_ReportTeamWin: ; 0x0225BAA0
	push {lr}
	sub sp, #0xc
	add r1, r0, #0
	ldrb r2, [r1, #0x14]
	cmp r2, #0
	beq _0225BACA
	ldr r0, [r1, #0x30]
	cmp r0, #0xa
	blo _0225BACA
	ldr r3, [r1, #0x18]
	ldrb r0, [r3, #5]
	str r0, [sp]
	ldrb r0, [r3, #6]
	str r0, [sp, #4]
	ldrb r0, [r3, #7]
	str r0, [sp, #8]
	ldr r0, [r3, #0x30]
	ldrb r1, [r1, #7]
	ldrb r3, [r3, #4]
	bl ov45_0222ACB8
_0225BACA:
	add sp, #0xc
	pop {pc}
	.balign 4, 0
	thumb_func_end PlazaGameResults_ReportTeamWin


	thumb_func_start PlazaGameSignWindow_Init
PlazaGameSignWindow_Init: ; 0x0225BAD0
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r7, r0, #0
	str r1, [sp, #8]
	str r2, [sp, #0xc]
	add r0, #0xc
	mov r1, #0
	mov r2, #0xc0
	str r3, [sp, #0x10]
	bl memset
	add r0, r7, #0
	add r0, #0xcc
	mov r1, #0
	mov r2, #0xc0
	bl memset
	mov r6, #0
	strh r6, [r7, #8]
	mov r0, #0x63
	strh r6, [r7, #0xa]
	lsl r0, r0, #2
	ldr r4, _0225BBC4 ; =ov90_0225C2B4
	str r6, [r7, r0]
	add r5, r7, #0
_0225BB02:
	mov r0, #0
	str r0, [sp]
	ldr r0, [sp, #0x10]
	ldr r1, [sp, #0xc]
	str r0, [sp, #4]
	ldr r2, [r4]
	ldr r3, [r4, #4]
	lsl r2, r2, #4
	lsl r3, r3, #4
	ldr r0, [sp, #8]
	asr r2, r2, #0x10
	asr r3, r3, #0x10
	bl PlazaGameGfx_CreateSprite
	str r0, [r5]
	mov r1, #5
	bl Sprite_SetAnimCtrlSeq
	ldr r0, [r5]
	mov r1, #0
	bl Sprite_SetPriority
	add r6, r6, #1
	add r4, #0xc
	add r5, r5, #4
	cmp r6, #2
	blt _0225BB02
	mov r3, #1
	lsl r3, r3, #0x1a
	ldr r1, [r3]
	mov r0, #0xe
	lsl r0, r0, #0xc
	and r1, r0
	ldr r4, [r3]
	ldr r2, _0225BBC8 ; =0xFFFF1FFF
	lsr r1, r1, #0xd
	and r4, r2
	mov r2, #1
	bic r1, r2
	lsl r1, r1, #0xd
	orr r1, r4
	add r2, r3, #0
	str r1, [r3]
	add r2, #0x48
	ldrh r4, [r2]
	mov r1, #0x3f
	bic r4, r1
	mov r1, #0x1f
	orr r4, r1
	mov r1, #0x20
	orr r4, r1
	strh r4, [r2]
	add r2, r3, #0
	mov r4, #0
	add r2, #0x40
	strh r4, [r2]
	add r2, r3, #0
	add r2, #0x44
	strh r4, [r2]
	add r2, r3, #0
	add r2, #0x4a
	ldrb r4, [r2]
	mov r2, #0x19
	lsl r2, r2, #4
	strb r4, [r7, r2]
	ldr r3, [r3]
	and r0, r3
	lsr r3, r0, #0xd
	mov r0, #2
	and r3, r0
	add r0, r2, #4
	str r3, [r7, r0]
	add r0, r2, #1
	ldrb r3, [r7, r0]
	mov r0, #0x1f
	bic r3, r0
	mov r0, #0xf
	orr r3, r0
	add r0, r2, #1
	strb r3, [r7, r0]
	ldrb r0, [r7, r0]
	orr r1, r0
	add r0, r2, #1
	strb r1, [r7, r0]
	ldr r0, _0225BBCC ; =PlazaGameSignWindow_HBlank
	add r1, r7, #0
	bl Main_SetHBlankIntrCB
	ldr r0, [r7, #4]
	mov r1, #2
	bl Sprite_SetFlipMode
	add r0, r7, #0
	bl PlazaGameSignWindow_UpdateLines
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0225BBC4: .word ov90_0225C2B4
_0225BBC8: .word 0xFFFF1FFF
_0225BBCC: .word PlazaGameSignWindow_HBlank
	thumb_func_end PlazaGameSignWindow_Init


	thumb_func_start PlazaGameSignWindow_Open
PlazaGameSignWindow_Open: ; 0x0225BBD0
	push {r4, lr}
	mov r1, #8
	ldrsh r2, [r0, r1]
	mov r4, #0
	add r2, r2, #1
	strh r2, [r0, #8]
	ldrsh r2, [r0, r1]
	cmp r2, #8
	blt _0225BBE6
	strh r1, [r0, #8]
	mov r4, #1
_0225BBE6:
	mov r1, #1
	bl PlazaGameSignWindow_Move
	add r0, r4, #0
	pop {r4, pc}
	thumb_func_end PlazaGameSignWindow_Open


	thumb_func_start PlazaGameSignWindow_Close
PlazaGameSignWindow_Close: ; 0x0225BBF0
	push {r3, r4, r5, lr}
	add r5, r0, #0
	mov r0, #8
	ldrsh r2, [r5, r0]
	mov r1, #0
	add r4, r1, #0
	sub r2, r2, #1
	strh r2, [r5, #8]
	ldrsh r0, [r5, r0]
	cmp r0, #0
	bgt _0225BC18
	mov r4, #1
	strh r4, [r5, #8]
	ldr r0, [r5]
	bl Sprite_SetDrawFlag
	ldr r0, [r5, #4]
	mov r1, #0
	bl Sprite_SetDrawFlag
_0225BC18:
	mov r1, #0
	add r0, r5, #0
	mvn r1, r1
	bl PlazaGameSignWindow_Move
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end PlazaGameSignWindow_Close


	thumb_func_start PlazaGameSignWindow_Move
PlazaGameSignWindow_Move: ; 0x0225BC28
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	str r0, [sp]
	ldr r2, [sp]
	mov r0, #8
	ldrsh r2, [r2, r0]
	mov r3, #0x17
	add r4, r2, #0
	mul r4, r3
	asr r2, r4, #2
	lsr r2, r2, #0x1d
	add r2, r4, r2
	asr r4, r2, #3
	ldr r2, [sp]
	strh r4, [r2, #0xa]
	ldrsh r0, [r2, r0]
	add r0, r0, r1
	cmp r0, #8
	bgt _0225BC62
	cmp r0, #0
	blt _0225BC62
	add r1, r0, #0
	mul r1, r3
	asr r0, r1, #2
	lsr r0, r0, #0x1d
	add r0, r1, r0
	asr r0, r0, #3
	str r0, [sp, #4]
	b _0225BC6A
_0225BC62:
	ldr r0, [sp]
	mov r1, #0xa
	ldrsh r0, [r0, r1]
	str r0, [sp, #4]
_0225BC6A:
	ldr r0, [sp, #4]
	ldr r7, _0225BD04 ; =ov90_0225C2B4
	ldr r4, [sp]
	mov r6, #0
	lsl r5, r0, #0xc
_0225BC74:
	add r3, r7, #0
	ldmia r3!, {r0, r1}
	add r2, sp, #8
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	cmp r6, #0
	str r0, [r2]
	bne _0225BCB6
	ldr r0, [sp, #4]
	cmp r0, #0
	ble _0225BC9C
	add r0, r5, #0
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _0225BCAA
_0225BC9C:
	add r0, r5, #0
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_0225BCAA:
	bl _ffix
	ldr r1, [sp, #0xc]
	sub r0, r1, r0
	str r0, [sp, #0xc]
	b _0225BCE6
_0225BCB6:
	ldr r0, [sp, #4]
	cmp r0, #0
	ble _0225BCCE
	add r0, r5, #0
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _0225BCDC
_0225BCCE:
	add r0, r5, #0
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_0225BCDC:
	bl _ffix
	ldr r1, [sp, #0xc]
	add r0, r1, r0
	str r0, [sp, #0xc]
_0225BCE6:
	ldr r0, [r4]
	add r1, sp, #8
	bl Sprite_SetMatrix
	add r6, r6, #1
	add r7, #0xc
	add r4, r4, #4
	cmp r6, #2
	blt _0225BC74
	ldr r0, [sp]
	bl PlazaGameSignWindow_UpdateLines
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	nop
_0225BD04: .word ov90_0225C2B4
	thumb_func_end PlazaGameSignWindow_Move


	thumb_func_start PlazaGameSignWindow_Free
PlazaGameSignWindow_Free: ; 0x0225BD08
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	mov r0, #0x63
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	cmp r0, #0
	beq _0225BD22
	bl SysTask_Destroy
	mov r0, #0x63
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r6, r0]
_0225BD22:
	mov r4, #0
	add r5, r6, #0
	add r7, r4, #0
_0225BD28:
	ldr r0, [r5]
	cmp r0, #0
	beq _0225BD34
	bl Sprite_Delete
	str r7, [r5]
_0225BD34:
	add r4, r4, #1
	add r5, r5, #4
	cmp r4, #2
	blt _0225BD28
	bl HBlankInterruptDisable
	mov r2, #1
	lsl r2, r2, #0x1a
	ldr r1, [r2]
	ldr r0, _0225BD7C ; =0xFFFF1FFF
	add r3, r1, #0
	and r3, r0
	mov r0, #0x65
	lsl r0, r0, #2
	ldr r1, [r6, r0]
	sub r0, r0, #4
	lsl r1, r1, #0xd
	orr r1, r3
	str r1, [r2]
	add r2, #0x4a
	ldrh r2, [r2]
	mov r1, #0x3f
	ldrb r0, [r6, r0]
	bic r2, r1
	lsl r1, r0, #0x1b
	lsr r1, r1, #0x1b
	lsl r0, r0, #0x1a
	orr r1, r2
	lsr r0, r0, #0x1f
	beq _0225BD74
	mov r0, #0x20
	orr r1, r0
_0225BD74:
	ldr r0, _0225BD80 ; =0x0400004A
	strh r1, [r0]
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0225BD7C: .word 0xFFFF1FFF
_0225BD80: .word 0x0400004A
	thumb_func_end PlazaGameSignWindow_Free


	thumb_func_start PlazaGameSignWindow_UpdateLines
PlazaGameSignWindow_UpdateLines: ; 0x0225BD84
	push {r3, r4, r5, r6, r7, lr}
	add r4, r0, #0
	add r0, #0xcc
	mov r1, #0
	mov r2, #0xc0
	bl memset
	mov r0, #0xa
	ldrsh r2, [r4, r0]
	mov r0, #0x17
	mov r1, #0
	sub r0, r0, r2
	cmp r0, #0
	ble _0225BDC0
	ldr r7, _0225BDDC ; =0x00000115
	mov r0, #1
	mov r2, #0x79
	mov r5, #0xa
	mov r6, #0x17
_0225BDAA:
	add r3, r4, r1
	strb r0, [r3, r7]
	sub r3, r2, r1
	add r3, r4, r3
	add r3, #0xcc
	strb r0, [r3]
	ldrsh r3, [r4, r5]
	add r1, r1, #1
	sub r3, r6, r3
	cmp r1, r3
	blt _0225BDAA
_0225BDC0:
	mov r0, #0x63
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	cmp r0, #0
	bne _0225BDDA
	ldr r0, _0225BDE0 ; =PlazaGameSignWindow_CommitLinesTask
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnVWaitQueue
	mov r1, #0x63
	lsl r1, r1, #2
	str r0, [r4, r1]
_0225BDDA:
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0225BDDC: .word 0x00000115
_0225BDE0: .word PlazaGameSignWindow_CommitLinesTask
	thumb_func_end PlazaGameSignWindow_UpdateLines


	thumb_func_start PlazaGameSignWindow_CommitLinesTask
PlazaGameSignWindow_CommitLinesTask: ; 0x0225BDE4
	push {r4, lr}
	add r4, r1, #0
	add r0, r4, #0
	add r0, #0xc
	add r1, #0xcc
	mov r2, #0xc0
	bl memcpy
	mov r0, #0x63
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl SysTask_Destroy
	mov r0, #0x63
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r4, r0]
	pop {r4, pc}
	thumb_func_end PlazaGameSignWindow_CommitLinesTask


	thumb_func_start PlazaGameSignWindow_HBlank
PlazaGameSignWindow_HBlank: ; 0x0225BE08
	push {r4, r5}
	ldr r1, _0225BED0 ; =0x04000006
	ldrh r2, [r1]
	add r2, r2, #1
	lsr r4, r2, #0x1f
	lsl r3, r2, #0x18
	sub r3, r3, r4
	mov r2, #0x18
	ror r3, r2
	add r2, r4, r3
	cmp r2, #0xc0
	bge _0225BECA
	sub r3, r1, #6
	ldr r4, [r3]
	mov r3, #0xe
	lsl r3, r3, #0xc
	and r3, r4
	sub r4, r1, #2
	ldrh r5, [r4]
	mov r4, #2
	lsr r3, r3, #0xd
	tst r4, r5
	beq _0225BECA
	cmp r2, #0x49
	blt _0225BE3E
	cmp r2, #0x79
	ble _0225BE6A
_0225BE3E:
	ldr r1, _0225BED4 ; =0x0400004A
	mov r4, #0x3f
	ldrh r1, [r1]
	bic r1, r4
	mov r4, #0x19
	lsl r4, r4, #4
	ldrb r4, [r0, r4]
	lsl r5, r4, #0x1b
	lsr r5, r5, #0x1b
	lsl r4, r4, #0x1a
	orr r1, r5
	lsr r4, r4, #0x1f
	beq _0225BE5C
	mov r4, #0x20
	orr r1, r4
_0225BE5C:
	ldr r4, _0225BED4 ; =0x0400004A
	strh r1, [r4]
	mov r1, #0x65
	lsl r1, r1, #2
	ldr r1, [r0, r1]
	orr r3, r1
	b _0225BE98
_0225BE6A:
	add r1, #0x44
	ldrh r1, [r1]
	mov r4, #0x3f
	bic r1, r4
	ldr r4, _0225BED8 ; =0x00000191
	ldrb r4, [r0, r4]
	lsl r5, r4, #0x1b
	lsr r5, r5, #0x1b
	lsl r4, r4, #0x1a
	orr r1, r5
	lsr r4, r4, #0x1f
	beq _0225BE86
	mov r4, #0x20
	orr r1, r4
_0225BE86:
	ldr r4, _0225BED4 ; =0x0400004A
	strh r1, [r4]
	mov r1, #0x65
	lsl r1, r1, #2
	ldr r4, [r0, r1]
	mov r1, #2
	orr r1, r4
	mvn r1, r1
	and r3, r1
_0225BE98:
	add r0, r0, r2
	ldrb r0, [r0, #0xc]
	cmp r0, #1
	ldr r0, _0225BEDC ; =0xFFFF1FFF
	bne _0225BEB8
	mov r2, #1
	lsl r2, r2, #0x1a
	ldr r1, [r2]
	and r1, r0
	mov r0, #1
	orr r0, r3
	lsl r0, r0, #0xd
	orr r0, r1
	str r0, [r2]
	pop {r4, r5}
	bx lr
_0225BEB8:
	mov r2, #1
	lsl r2, r2, #0x1a
	ldr r1, [r2]
	and r1, r0
	mov r0, #1
	bic r3, r0
	lsl r0, r3, #0xd
	orr r0, r1
	str r0, [r2]
_0225BECA:
	pop {r4, r5}
	bx lr
	nop
_0225BED0: .word 0x04000006
_0225BED4: .word 0x0400004A
_0225BED8: .word 0x00000191
_0225BEDC: .word 0xFFFF1FFF
	thumb_func_end PlazaGameSignWindow_HBlank


	thumb_func_start PlazaGameSign_CountdownTask
PlazaGameSign_CountdownTask: ; 0x0225BEE0
	push {r4, r5, r6, lr}
	sub sp, #8
	add r4, r1, #0
	ldrh r0, [r4, #4]
	cmp r0, #5
	bhi _0225BF44
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0225BEF8: ; jump table
	.short _0225C05E - _0225BEF8 - 2 ; case 0
	.short _0225BF04 - _0225BEF8 - 2 ; case 1
	.short _0225BF38 - _0225BEF8 - 2 ; case 2
	.short _0225BF54 - _0225BEF8 - 2 ; case 3
	.short _0225C018 - _0225BEF8 - 2 ; case 4
	.short _0225C03A - _0225BEF8 - 2 ; case 5
_0225BF04:
	add r0, r4, #0
	ldr r1, [r4, #0x28]
	ldr r2, [r4, #8]
	ldr r3, [r4]
	add r0, #0x34
	bl PlazaGameSignWindow_Init
	mov r0, #0x40
	str r0, [sp]
	ldr r0, [r4]
	mov r2, #0x80
	str r0, [sp, #4]
	ldr r0, [r4, #0x28]
	ldr r1, [r4, #8]
	mov r3, #0x61
	bl PlazaGameGfx_CreateSprite
	str r0, [r4, #0x30]
	mov r1, #0
	bl Sprite_SetPriority
	ldrh r0, [r4, #4]
	add sp, #8
	add r0, r0, #1
	strh r0, [r4, #4]
	pop {r4, r5, r6, pc}
_0225BF38:
	add r0, r4, #0
	add r0, #0x34
	bl PlazaGameSignWindow_Open
	cmp r0, #1
	beq _0225BF46
_0225BF44:
	b _0225C05E
_0225BF46:
	ldrh r0, [r4, #4]
	add sp, #8
	add r0, r0, #1
	strh r0, [r4, #4]
	mov r0, #0
	strh r0, [r4, #6]
	pop {r4, r5, r6, pc}
_0225BF54:
	ldr r0, [r4, #0x30]
	bl Sprite_GetAnimationFrame
	add r6, r0, #0
	mov r1, #2
	ldr r0, [r4, #0x30]
	lsl r1, r1, #0xc
	bl Sprite_UpdateAnim
	ldr r0, [r4, #0x30]
	bl Sprite_GetAnimationFrame
	add r5, r0, #0
	cmp r6, r5
	beq _0225BFC8
	cmp r5, #0xf
	bhi _0225BFC8
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0225BF82: ; jump table
	.short _0225BFC8 - _0225BF82 - 2 ; case 0
	.short _0225BFC8 - _0225BF82 - 2 ; case 1
	.short _0225BFC8 - _0225BF82 - 2 ; case 2
	.short _0225BFC8 - _0225BF82 - 2 ; case 3
	.short _0225BFC8 - _0225BF82 - 2 ; case 4
	.short _0225BFC8 - _0225BF82 - 2 ; case 5
	.short _0225BFC8 - _0225BF82 - 2 ; case 6
	.short _0225BFA2 - _0225BF82 - 2 ; case 7
	.short _0225BFC8 - _0225BF82 - 2 ; case 8
	.short _0225BFAA - _0225BF82 - 2 ; case 9
	.short _0225BFC8 - _0225BF82 - 2 ; case 10
	.short _0225BFB2 - _0225BF82 - 2 ; case 11
	.short _0225BFC8 - _0225BF82 - 2 ; case 12
	.short _0225BFBA - _0225BF82 - 2 ; case 13
	.short _0225BFC8 - _0225BF82 - 2 ; case 14
	.short _0225BFC2 - _0225BF82 - 2 ; case 15
_0225BFA2:
	ldr r0, _0225C064 ; =0x000005DD
	bl PlaySE
	b _0225BFC8
_0225BFAA:
	ldr r0, _0225C064 ; =0x000005DD
	bl PlaySE
	b _0225BFC8
_0225BFB2:
	ldr r0, _0225C064 ; =0x000005DD
	bl PlaySE
	b _0225BFC8
_0225BFBA:
	ldr r0, _0225C064 ; =0x000005DD
	bl PlaySE
	b _0225BFC8
_0225BFC2:
	ldr r0, _0225C068 ; =0x00000642
	bl PlaySE
_0225BFC8:
	cmp r5, #0xf
	bne _0225BFFC
	mov r0, #6
	ldrsh r0, [r4, r0]
	cmp r0, #0
	bne _0225BFDE
	ldr r0, [r4, #0x30]
	mov r1, #0
	bl Sprite_SetPalOffsetRespectVramOffset
	b _0225BFEA
_0225BFDE:
	cmp r0, #4
	bne _0225BFEA
	ldr r0, [r4, #0x30]
	mov r1, #1
	bl Sprite_SetPalOffsetRespectVramOffset
_0225BFEA:
	mov r0, #6
	ldrsh r1, [r4, r0]
	add r1, r1, #1
	strh r1, [r4, #6]
	ldrsh r0, [r4, r0]
	cmp r0, #8
	blt _0225BFFC
	mov r0, #0
	strh r0, [r4, #6]
_0225BFFC:
	ldr r0, [r4, #0x30]
	bl Sprite_IsAnimated
	cmp r0, #0
	bne _0225C05E
	ldrh r0, [r4, #4]
	mov r1, #0
	add r0, r0, #1
	strh r0, [r4, #4]
	ldr r0, [r4, #0x30]
	bl Sprite_SetPalOffsetRespectVramOffset
	add sp, #8
	pop {r4, r5, r6, pc}
_0225C018:
	add r0, r4, #0
	add r0, #0x34
	bl PlazaGameSignWindow_Close
	cmp r0, #1
	bne _0225C05E
	ldr r0, [r4, #0x30]
	bl Sprite_Delete
	mov r1, #0
	str r1, [r4, #0x30]
	ldrh r0, [r4, #4]
	add sp, #8
	add r0, r0, #1
	strh r0, [r4, #4]
	strh r1, [r4, #6]
	pop {r4, r5, r6, pc}
_0225C03A:
	mov r0, #6
	ldrsh r1, [r4, r0]
	add r1, r1, #1
	strh r1, [r4, #6]
	ldrsh r0, [r4, r0]
	cmp r0, #2
	blt _0225C05E
	mov r0, #0
	strh r0, [r4, #4]
	add r0, r4, #0
	add r0, #0x34
	bl PlazaGameSignWindow_Free
	ldr r0, [r4, #0x2c]
	bl SysTask_Destroy
	mov r0, #0
	str r0, [r4, #0x2c]
_0225C05E:
	add sp, #8
	pop {r4, r5, r6, pc}
	nop
_0225C064: .word 0x000005DD
_0225C068: .word 0x00000642
	thumb_func_end PlazaGameSign_CountdownTask


	thumb_func_start PlazaGameSign_FinishTask
PlazaGameSign_FinishTask: ; 0x0225C06C
	push {r4, lr}
	sub sp, #8
	add r4, r1, #0
	ldrh r0, [r4, #4]
	cmp r0, #5
	bhi _0225C14C
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0225C084: ; jump table
	.short _0225C14C - _0225C084 - 2 ; case 0
	.short _0225C090 - _0225C084 - 2 ; case 1
	.short _0225C0CC - _0225C084 - 2 ; case 2
	.short _0225C0EE - _0225C084 - 2 ; case 3
	.short _0225C106 - _0225C084 - 2 ; case 4
	.short _0225C128 - _0225C084 - 2 ; case 5
_0225C090:
	add r0, r4, #0
	ldr r1, [r4, #0x28]
	ldr r2, [r4, #8]
	ldr r3, [r4]
	add r0, #0x34
	bl PlazaGameSignWindow_Init
	mov r0, #0x40
	str r0, [sp]
	ldr r0, [r4]
	mov r2, #0x80
	str r0, [sp, #4]
	ldr r0, [r4, #0x28]
	ldr r1, [r4, #8]
	mov r3, #0x61
	bl PlazaGameGfx_CreateSprite
	str r0, [r4, #0x30]
	mov r1, #1
	bl Sprite_SetAnimCtrlSeq
	ldr r0, [r4, #0x30]
	mov r1, #0
	bl Sprite_SetPriority
	ldrh r0, [r4, #4]
	add sp, #8
	add r0, r0, #1
	strh r0, [r4, #4]
	pop {r4, pc}
_0225C0CC:
	add r0, r4, #0
	add r0, #0x34
	bl PlazaGameSignWindow_Open
	cmp r0, #1
	bne _0225C14C
	ldr r0, [r4, #0x30]
	mov r1, #1
	bl Sprite_SetAnimActiveFlag
	ldrh r0, [r4, #4]
	add sp, #8
	add r0, r0, #1
	strh r0, [r4, #4]
	mov r0, #0
	strh r0, [r4, #6]
	pop {r4, pc}
_0225C0EE:
	mov r0, #6
	ldrsh r1, [r4, r0]
	add r1, r1, #1
	strh r1, [r4, #6]
	ldrsh r0, [r4, r0]
	cmp r0, #0x32
	blt _0225C14C
	ldrh r0, [r4, #4]
	add sp, #8
	add r0, r0, #1
	strh r0, [r4, #4]
	pop {r4, pc}
_0225C106:
	add r0, r4, #0
	add r0, #0x34
	bl PlazaGameSignWindow_Close
	cmp r0, #1
	bne _0225C14C
	ldr r0, [r4, #0x30]
	bl Sprite_Delete
	mov r0, #0
	str r0, [r4, #0x30]
	strh r0, [r4, #6]
	ldrh r0, [r4, #4]
	add sp, #8
	add r0, r0, #1
	strh r0, [r4, #4]
	pop {r4, pc}
_0225C128:
	mov r0, #6
	ldrsh r1, [r4, r0]
	add r1, r1, #1
	strh r1, [r4, #6]
	ldrsh r0, [r4, r0]
	cmp r0, #2
	blt _0225C14C
	mov r0, #0
	strh r0, [r4, #4]
	add r0, r4, #0
	add r0, #0x34
	bl PlazaGameSignWindow_Free
	ldr r0, [r4, #0x2c]
	bl SysTask_Destroy
	mov r0, #0
	str r0, [r4, #0x2c]
_0225C14C:
	add sp, #8
	pop {r4, pc}
	thumb_func_end PlazaGameSign_FinishTask
