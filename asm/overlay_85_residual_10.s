	.include "asm/macros.inc"
	.public _021EAA80
	.public SpinTradeApp_Init
	.public SpinTradeApp_Exit
	.public SpinTradeApp_Main
	.public SpinTrade_ClearLinkFlags
	.public SpinTrade_State_Start
	.public SpinTrade_State_WaitFadeIn
	.public SpinTrade_State_WaitMembers
	.public SpinTrade_State_SendStart
	.public SpinTrade_State_WaitConnections
	.public SpinTrade_State_SendPlayerCount
	.public SpinTrade_State_SendSlots
	.public SpinTrade_State_SendReady
	.public SpinTrade_State_SendJoin
	.public SpinTrade_State_WaitReady
	.public SpinTrade_State_StartSync
	.public SpinTrade_State_WaitSync
	.public SpinTrade_State_SendTradeSlot
	.public SpinTrade_State_SendParty
	.public SpinTrade_State_CheckEggs
	.public SpinTrade_State_WaitConfirm
	.public SpinTrade_State_SetupDrop
	.public SpinTrade_State_DropEggs
	.public SpinTrade_State_WaitEggsLanded
	.public SpinTrade_State_WaitBeforeSpin
	.public SpinTrade_State_SendSpinStart
	.public SpinTrade_State_WaitSpinStart
	.public SpinTrade_State_StartSpinSync
	.public SpinTrade_State_WaitSpinSync
	.public SpinTrade_State_StartCountdown
	.public SpinTrade_State_ClearNames
	.public SpinTrade_State_BeginSpin
	.public SpinTrade_State_SpinHost
	.public SpinTrade_State_SpinClient
	.public SpinTrade_State_StopSpin
	.public SpinTrade_State_RotateToOwnEgg
	.public SpinTrade_State_AlignToSlot
	.public SpinTrade_State_StartResultSync
	.public SpinTrade_State_WaitResultSync
	.public SpinTrade_State_SendResult
	.public SpinTrade_State_ReceiveResult
	.public SpinTrade_State_ShowNames
	.public SpinTrade_State_ShowReceivedEgg
	.public SpinTrade_State_WaitEggFanfare
	.public SpinTrade_State_GiveBonusBerry
	.public SpinTrade_State_WaitBerryFanfare
	.public SpinTrade_State_ShowBagFull
	.public SpinTrade_State_Wait
	.public SpinTrade_State_StartSave
	.public SpinTrade_State_WaitSaveSync
	.public SpinTrade_State_Save
	.public SpinTrade_State_Cancel
	.public SpinTrade_State_WaitCancelMessage
	.public SpinTrade_State_StartExitSync
	.public SpinTrade_State_WaitExitSync
	.public SpinTrade_State_WaitDisconnect
	.public SpinTrade_State_FadeOut
	.public SpinTrade_State_WaitFadeOut
	.public SpinTrade_State_Exit
	.public SpinTrade_VBlankCallback
	.public SpinTrade_InitGraphics
	.public SpinTrade_FreeGraphics
	.public SpinTrade_Draw
	.public SpinTrade_SetGraphicsBanks
	.public SpinTrade_SetupPlanes
	.public SpinTrade_DisableDisplay
	.public SpinTrade_Init3D
	.public SpinTrade_InitBgLayers
	.public SpinTrade_LoadBgGraphics
	.public SpinTrade_FreeBgLayers
	.public SpinTrade_LoadButtonTilemap
	.public SpinTrade_SetButtonPressed
	.public SpinTrade_SetButtonReleased
	.public SpinTrade_SetButtonHalfPressed
	.public SpinTrade_InitPaletteData
	.public SpinTrade_FreePaletteData
	.public SpinTrade_SetSubBgFadeLevel
	.public SpinTrade_InitSpriteSystem
	.public SpinTrade_LoadSpriteResources
	.public SpinTrade_FreeSpriteSystem
	.public SpinTrade_CreateSprite
	.public SpinTrade_InitText
	.public SpinTrade_FreeText
	.public SpinTrade_PrintMessage
	.public SpinTrade_PrintPlayerMessage
	.public SpinTrade_PrintItemMessage
	.public SpinTrade_ClearMessage
	.public SpinTrade_AddNameWindows
	.public SpinTrade_RemoveNameWindows
	.public SpinTrade_PrintNameWindow
	.public SpinTrade_PrintPlayerName
	.public SpinTrade_ClearNameWindow
	.public SpinTrade_InitCamera
	.public SpinTrade_FreeCamera
	.public SpinTrade_UpdateTouch
	.public SpinTrade_IsInButtonArea
	.public SpinTrade_IsButtonTouched
	.public SpinTrade_InitMachine
	.public SpinTrade_FreeMachine
	.public SpinTrade_UpdateMachinePosition
	.public SpinTrade_DrawMachine
	.public SpinTradeMachine_Rotate
	.public SpinTrade_LoadObjects
	.public SpinTrade_FreeObjects
	.public SpinTrade_LoadObjectModels
	.public SpinTrade_FreeObjectModels
	.public SpinTrade_ShowEgg
	.public SpinTrade_UpdateEggPositions
	.public SpinTrade_DrawEggs
	.public SpinTrade_RotateEggs
	.public SpinTradeEgg_UpdatePosition
	.public SpinTrade_FindEggNearestSlot
	.public SpinTrade_ShowShadow
	.public SpinTradeShadow_Update
	.public SpinTrade_ShowMarker
	.public SpinTradeMarker_Update
	.public SpinTrade_StartEggDrop
	.public SpinTrade_EggDropTask
	.public SpinTradeEggDrop_Init
	.public SpinTradeEggDrop_Fall
	.public SpinTradeEggDrop_Land
	.public SpinTradeBounce_Update
	.public SpinTrade_BounceTask
	.public SpinTrade_StartBounceTask
	.public SpinTrade_StopBounceTask
	.public SpinTrade_AddBounce
	.public SpinTrade_BounceAllEggs
	.public SpinTrade_IsAnyEggBouncing
	.public SpinTradeSpin_Update
	.public SpinTrade_SpinTask
	.public SpinTrade_StartSpinTask
	.public SpinTrade_StopSpinTask
	.public SpinTrade_AddSpin
	.public SpinTradeWobble_Update
	.public SpinTrade_WobbleTask
	.public SpinTrade_StartWobbleTask
	.public SpinTrade_StopWobbleTask
	.public SpinTrade_AddWobble
	.public SpinTradeJump_Update
	.public SpinTrade_JumpTask
	.public SpinTrade_StartJumpTask
	.public SpinTrade_StopJumpTask
	.public SpinTrade_StartJump
	.public SpinTrade_HasJumpLanded
	.public SpinTrade_IsJumping
	.public SpinTrade_CountdownTask
	.public SpinTrade_StartCountdown
	.public SpinTradeButtonFlash_Update
	.public SpinTrade_ButtonFlashTask
	.public SpinTrade_StartButtonFlashTask
	.public SpinTrade_StopButtonFlashTask
	.public SpinTrade_IsButtonFlashing
	.public SpinTrade_StartButtonFlash
	.public SpinTradeHover_Update
	.public SpinTrade_HoverTask
	.public SpinTrade_StartHoverTask
	.public SpinTrade_StopHoverTask
	.public SpinTrade_AddHover
	.public SpinTrade_HoverBonusEggs
	.public SpinTrade_SubBgFadeTask
	.public SpinTrade_StartSubBgFade
	.public SpinTrade_AddAngle
	.public SpinTrade_RotateAll
	.public SpinTrade_TickSpinTimer
	.public SpinTrade_LoadNarcMember
	.public SpinTrade_ReceiveJump
	.public SpinTrade_ReceiveSpinData
	.public SpinTrade_ClearScratch
	.public SpinTrade_GetScratch
	.public SpinTrade_HasReceivedStatus
	.public SpinTrade_HaveAllMembersJoined
	.public SpinTrade_CountMembers
	.public SpinTrade_ReceiveMemberSlot
	.public SpinTrade_ReceivePlayerCount
	.public SpinTrade_IsMemberPresent
	.public SpinTrade_ReceiveTradedMon
	.public SpinTrade_CountReceivedParties
	.public SpinTrade_ReceiveTradeSlot
	.public SpinTrade_ReceiveResult
	.public SpinTrade_IsInBonusArea
	.public SpinTrade_PartyHasBadEgg
	.public SpinTrade_AnyPartyHasBadEgg
	.public SpinTrade_CountConfirmed
	.public SpinTrade_IsCancelled
	.public SpinTradeGroupApp_Init
	.public SpinTradeGroupApp_Main
	.public SpinTradeGroupApp_Exit
	.public SpinTradeGroup_VBlankTask
	.public SpinTradeGroup_VBlankCallback
	.public SpinTradeGroup_SetGraphicsBanks
	.public SpinTradeGroup_InitBgLayers
	.public SpinTradeGroup_InitData
	.public SpinTradeGroup_FreeData
	.public SpinTradeGroup_FreeBgLayers
	.public SpinTradeGroup_LoadBgGraphics
	.public SpinTradeGroup_InitObjTransfer
	.public SpinTradeGroup_InitSpriteResources
	.public SpinTradeGroup_CreateAvatarSprites
	.public SpinTradeGroup_InitWindows
	.public SpinTradeGroup_PrintTitle
	.public SpinTradeGroup_RemoveWindows
	.public SpinTradeGroup_State_ShowPrompt
	.public SpinTradeGroup_State_HandleInput
	.public SpinTradeGroup_HandleButtons
	.public ov85_021E943C
	.public ov85_021E9440
	.public SpinTradeGroup_WaitPrinterThen
	.public SpinTradeGroup_State_WaitPrinter
	.public ov85_021E948C
	.public ov85_021E94BC
	.public ov85_021E94EC
	.public ov85_021E9614
	.public ov85_021E962C
	.public ov85_021E96B0
	.public ov85_021E96D0
	.public ov85_021E9700
	.public ov85_021E97B8
	.public ov85_021E97E8
	.public ov85_021E9814
	.public SpinTradeGroup_State_StartSave
	.public SpinTradeGroup_State_Save
	.public ov85_021E98C4
	.public SpinTradeGroup_State_ShowCanceled
	.public SpinTradeGroup_State_FadeOutCanceled
	.public ov85_021E993C
	.public ov85_021E996C
	.public ov85_021E9A1C
	.public ov85_021E9A6C
	.public SpinTradeGroup_State_StartExitSync
	.public SpinTradeGroup_State_WaitExitSync
	.public ov85_021E9AF0
	.public ov85_021E9B1C
	.public ov85_021E9B40
	.public ov85_021E9BA0
	.public ov85_021E9BDC
	.public SpinTradeGroup_State_Proceed
	.public ov85_021E9C84
	.public ov85_021E9D9C
	.public SpinTradeGroup_CycleHighlightColor
	.public SpinTradeGroup_CountProfiles
	.public SpinTradeGroup_PrintMemberList
	.public ov85_021E9FCC
	.public SpinTradeGroup_CountMembers
	.public SpinTradeGroup_GetMemberMask
	.public SpinTradeGroup_UpdateMembers
	.public SpinTradeGroup_PrintMessage
	.public SpinTradeGroup_IsPrinterDone
	.public SpinTradeGroup_ClearMessage
	.public SpinTradeGroup_UpdateAvatars
	.public SpinTradeGroup_LoadAvatarGraphics
	.public SpinTradeGroup_LoadAvatar
	.public SpinTradeGroup_FreeAvatarGraphics
	.public SpinTradeGroup_GetTextSpeed
	.public SpinTradeGroup_SetMaxConnections
	.public SpinTradeGroup_CheckMembers
	.public ov85_021EA4EC
	.public ov85_021EA4F0
	.public ov85_021EA4F4
	.public ov85_021EA4FC
	.public ov85_021EA504
	.public ov85_021EA514
	.public ov85_021EA51C
	.public ov85_021EA528
	.public ov85_021EA538
	.public ov85_021EA548
	.public ov85_021EA558
	.public ov85_021EA56C
	.public ov85_021EA580
	.public ov85_021EA594
	.public ov85_021EA5AC
	.public ov85_021EA5C4
	.public ov85_021EA5DC
	.public ov85_021EA5F4
	.public ov85_021EA60C
	.public ov85_021EA628
	.public ov85_021EA644
	.public ov85_021EA660
	.public ov85_021EA67C
	.public ov85_021EA698
	.public ov85_021EA6B8
	.public ov85_021EA6D8
	.public ov85_021EA700
	.public ov85_021EA728
	.public ov85_021EA72C
	.public ov85_021EA758
	.public ov85_021EA788
	.public ov85_021EA7C4
	.public ov85_021EA800
	.public ov85_021EA8D8
	.public ov85_021EA8E0
	.public ov85_021EA8EA
	.public ov85_021EA8EB
	.public ov85_021EA8F4
	.public ov85_021EA904
	.public ov85_021EA914
	.public ov85_021EA92C
	.public ov85_021EA948
	.public ov85_021EA964
	.public ov85_021EA980
	.public ov85_021EA99C
	.public ov85_021EA9B8
	.public ov85_021EA9E0
	.include "overlay_85.inc"
	.include "global.inc"

	.text
	.public SpinTrade_ClearLinkFlags
	.public SpinTrade_State_Start
	.public SpinTrade_State_WaitMembers
	.public SpinTrade_State_SendReady
	.public SpinTrade_State_SendJoin
	.public SpinTrade_State_WaitReady
	.public SpinTrade_State_StartSync
	.public SpinTrade_State_WaitSync
	.public SpinTrade_State_SendTradeSlot
	.public SpinTrade_State_SendParty
	.public SpinTrade_State_WaitConfirm
	.public SpinTrade_State_SendSpinStart
	.public SpinTrade_State_WaitSpinStart
	.public SpinTrade_State_StartSpinSync
	.public SpinTrade_State_WaitSpinSync
	.public SpinTrade_State_StartCountdown
	.public SpinTrade_State_BeginSpin
	.public SpinTrade_State_StartResultSync
	.public SpinTrade_State_WaitEggFanfare
	.public SpinTrade_State_WaitBerryFanfare
	.public SpinTrade_State_ShowBagFull
	.public SpinTrade_State_Wait
	.public SpinTrade_State_Cancel
	.public SpinTrade_State_WaitCancelMessage
	.public SpinTrade_State_StartExitSync
	.public SpinTrade_State_WaitDisconnect
	.public SpinTrade_State_WaitFadeOut
	.public SpinTrade_State_Exit
	.public SpinTrade_VBlankCallback
	.public SpinTrade_FreeGraphics
	.public SpinTrade_SetButtonPressed
	.public SpinTrade_SetButtonReleased
	.public SpinTrade_SetButtonHalfPressed
	.public SpinTrade_SetSubBgFadeLevel
	.public SpinTrade_FreeSpriteSystem
	.public SpinTrade_CreateSprite
	.public SpinTrade_ClearNameWindow
	.public SpinTrade_FreeCamera
	.public SpinTrade_IsInButtonArea
	.public SpinTrade_IsButtonTouched
	.public SpinTrade_FreeMachine
	.public SpinTrade_DrawMachine
	.public SpinTradeMachine_Rotate
	.public SpinTrade_LoadObjects
	.public SpinTrade_FreeObjects
	.public SpinTrade_BounceTask
	.public SpinTrade_StopBounceTask
	.public SpinTrade_AddBounce
	.public SpinTrade_IsAnyEggBouncing
	.public SpinTrade_SpinTask
	.public SpinTrade_StopSpinTask
	.public SpinTrade_WobbleTask
	.public SpinTrade_StopWobbleTask
	.public SpinTrade_JumpTask
	.public SpinTrade_StopJumpTask
	.public SpinTrade_StartJump
	.public SpinTrade_HasJumpLanded
	.public SpinTrade_IsJumping
	.public SpinTrade_ButtonFlashTask
	.public SpinTrade_StopButtonFlashTask
	.public SpinTrade_IsButtonFlashing
	.public SpinTrade_StartButtonFlash
	.public SpinTrade_HoverTask

	thumb_func_start SpinTrade_State_StartSave
SpinTrade_State_StartSave: ; 0x021E6594
	push {r4, lr}
	add r4, r0, #0
	add r0, #0xcc
	ldr r0, [r0]
	mov r1, #0x78 ; GAME_STAT_EGGS_SPUN
	ldr r0, [r0, #0x28]
	bl GameStats_Inc
	add r0, r4, #0
	add r0, #0xcc
	ldr r0, [r0]
	mov r1, #0x22 ; SCORE_EVENT_SPIN_TRADE
	ldr r0, [r0, #0x28]
	bl GameStats_AddScore
	add r0, r4, #0
	mov r1, #0xb
	bl SpinTrade_PrintMessage
	mov r0, #0xca
	bl LinkSync_StartSync
	ldr r0, _021E65D0 ; =gSystem + 0x60
	mov r1, #1
	strb r1, [r0, #0xc]
	mov r0, #0x2c
	str r0, [r4]
	mov r0, #0
	pop {r4, pc}
	nop
_021E65D0: .word gSystem + 0x60
	thumb_func_end SpinTrade_State_StartSave


	thumb_func_start SpinTrade_State_WaitSaveSync
SpinTrade_State_WaitSaveSync: ; 0x021E65D4
	push {r4, lr}
	add r4, r0, #0
	mov r0, #0xca
	bl LinkSync_IsSynced
	cmp r0, #0
	beq _021E6604
	ldr r1, [r4, #0x28]
	add r0, r4, #0
	ldr r1, [r1, #0xc]
	bl SpinTrade_ReceiveTradedMon
	add r0, r4, #4
	bl Comm_InitSyncedSave
	ldr r0, _021E6608 ; =0x00000CA8
	mov r1, #0xa
	add r0, r4, r0
	bl WaitingIcon_New
	ldr r1, _021E660C ; =0x00000DC8
	str r0, [r4, r1]
	mov r0, #0x2d
	str r0, [r4]
_021E6604:
	mov r0, #0
	pop {r4, pc}
	.balign 4, 0
_021E6608: .word 0x00000CA8
_021E660C: .word 0x00000DC8
	thumb_func_end SpinTrade_State_WaitSaveSync


	thumb_func_start SpinTrade_State_Save
SpinTrade_State_Save: ; 0x021E6610
	push {r4, lr}
	add r4, r0, #0
	add r0, #0xcc
	ldr r0, [r0]
	mov r1, #2
	ldr r0, [r0, #0x1c]
	add r2, r4, #4
	bl Comm_SyncedSave
	cmp r0, #0
	beq _021E6638
	ldr r0, _021E663C ; =gSystem + 0x60
	mov r1, #0
	strb r1, [r0, #0xc]
	ldr r0, _021E6640 ; =0x00000DC8
	ldr r0, [r4, r0]
	bl WaitingIcon_Delete
	mov r0, #0x30
	str r0, [r4]
_021E6638:
	mov r0, #0
	pop {r4, pc}
	.balign 4, 0
_021E663C: .word gSystem + 0x60
_021E6640: .word 0x00000DC8
	thumb_func_end SpinTrade_State_Save
