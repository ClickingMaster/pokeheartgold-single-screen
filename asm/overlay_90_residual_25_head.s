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
	bl sub_0202C6F4
	add r4, r0, #0
	ldrb r0, [r5, #0x14]
	mov r6, #0
	cmp r0, #0
	ble _0225BA12
	add r7, sp, #0
_0225B9BE:
	add r0, r5, r6
	ldrb r0, [r0, #0x10]
	bl sub_0203484C
	add r1, r0, #0
	ldr r0, [r5, #0xc]
	add r2, r7, #0
	bl sub_0203A084
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
