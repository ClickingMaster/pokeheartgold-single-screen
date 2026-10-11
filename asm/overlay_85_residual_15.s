	.include "asm/macros.inc"
	.public _021EAA80
	.public ov85_021E5900
	.public ov85_021E5A34
	.public ov85_021E5AAC
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
	.public ov85_021E85C4
	.public ov85_021E85CC
	.public SpinTrade_ClearScratch
	.public SpinTrade_GetScratch
	.public SpinTrade_HasReceivedStatus
	.public SpinTrade_HaveAllMembersJoined
	.public SpinTrade_CountMembers
	.public ov85_021E8680
	.public ov85_021E86AC
	.public SpinTrade_IsMemberPresent
	.public SpinTrade_ReceiveTradedMon
	.public SpinTrade_CountReceivedParties
	.public ov85_021E8740
	.public ov85_021E8748
	.public SpinTrade_IsInBonusArea
	.public SpinTrade_PartyHasBadEgg
	.public SpinTrade_AnyPartyHasBadEgg
	.public SpinTrade_CountConfirmed
	.public SpinTrade_IsCancelled
	.public ov85_021E88AC
	.public ov85_021E8A08
	.public ov85_021E8B08
	.public ov85_021E8BB0
	.public ov85_021E8C14
	.public ov85_021E8C3C
	.public ov85_021E8C5C
	.public ov85_021E8D64
	.public ov85_021E8E00
	.public ov85_021E8E38
	.public ov85_021E8E6C
	.public ov85_021E8F58
	.public ov85_021E8F88
	.public ov85_021E9084
	.public ov85_021E9160
	.public ov85_021E9244
	.public ov85_021E9288
	.public ov85_021E92B8
	.public ov85_021E92F8
	.public ov85_021E9324
	.public ov85_021E943C
	.public ov85_021E9440
	.public ov85_021E9458
	.public ov85_021E9468
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
	.public ov85_021E9834
	.public ov85_021E9864
	.public ov85_021E98C4
	.public ov85_021E98E0
	.public ov85_021E9900
	.public ov85_021E993C
	.public ov85_021E996C
	.public ov85_021E9A1C
	.public ov85_021E9A6C
	.public ov85_021E9A90
	.public ov85_021E9AB0
	.public ov85_021E9AF0
	.public ov85_021E9B1C
	.public ov85_021E9B40
	.public ov85_021E9BA0
	.public ov85_021E9BDC
	.public ov85_021E9C50
	.public ov85_021E9C84
	.public ov85_021E9D9C
	.public ov85_021E9E58
	.public ov85_021E9EA4
	.public ov85_021E9EC0
	.public ov85_021E9FCC
	.public ov85_021E9FD0
	.public ov85_021E9FEC
	.public ov85_021EA010
	.public ov85_021EA0EC
	.public ov85_021EA17C
	.public ov85_021EA19C
	.public ov85_021EA1AC
	.public ov85_021EA2C0
	.public ov85_021EA324
	.public ov85_021EA368
	.public ov85_021EA398
	.public ov85_021EA39C
	.public ov85_021EA3F0
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

	thumb_func_start SpinTrade_InitPaletteData
SpinTrade_InitPaletteData: ; 0x021E6E14
	push {r4, lr}
	add r4, r0, #0
	mov r0, #0x66
	bl PaletteData_Init
	ldr r1, _021E6E68 ; =0x00000D9C
	str r0, [r4, r1]
	ldr r0, [r4, r1]
	mov r1, #1
	bl PaletteData_SetAutoTransparent
	ldr r0, _021E6E68 ; =0x00000D9C
	mov r2, #2
	ldr r0, [r4, r0]
	mov r1, #0
	lsl r2, r2, #8
	mov r3, #0x66
	bl PaletteData_AllocBuffers
	ldr r0, _021E6E68 ; =0x00000D9C
	mov r1, #2
	ldr r0, [r4, r0]
	lsl r2, r1, #8
	mov r3, #0x66
	bl PaletteData_AllocBuffers
	ldr r0, _021E6E68 ; =0x00000D9C
	mov r1, #1
	ldr r0, [r4, r0]
	lsl r2, r1, #9
	mov r3, #0x66
	bl PaletteData_AllocBuffers
	ldr r0, _021E6E68 ; =0x00000D9C
	mov r2, #2
	ldr r0, [r4, r0]
	mov r1, #3
	lsl r2, r2, #8
	mov r3, #0x66
	bl PaletteData_AllocBuffers
	pop {r4, pc}
	.balign 4, 0
_021E6E68: .word 0x00000D9C
	thumb_func_end SpinTrade_InitPaletteData


	thumb_func_start SpinTrade_FreePaletteData
SpinTrade_FreePaletteData: ; 0x021E6E6C
	push {r4, lr}
	add r4, r0, #0
	ldr r0, _021E6EA4 ; =0x00000D9C
	mov r1, #0
	ldr r0, [r4, r0]
	bl PaletteData_FreeBuffers
	ldr r0, _021E6EA4 ; =0x00000D9C
	mov r1, #2
	ldr r0, [r4, r0]
	bl PaletteData_FreeBuffers
	ldr r0, _021E6EA4 ; =0x00000D9C
	mov r1, #1
	ldr r0, [r4, r0]
	bl PaletteData_FreeBuffers
	ldr r0, _021E6EA4 ; =0x00000D9C
	mov r1, #3
	ldr r0, [r4, r0]
	bl PaletteData_FreeBuffers
	ldr r0, _021E6EA4 ; =0x00000D9C
	ldr r0, [r4, r0]
	bl PaletteData_Free
	pop {r4, pc}
	nop
_021E6EA4: .word 0x00000D9C
	thumb_func_end SpinTrade_FreePaletteData
