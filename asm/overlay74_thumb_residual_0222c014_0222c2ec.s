#include "config.h"
#include "msgdata/msg/msg_0247.h"
#include "msgdata/msg/msg_0442.h"
#include "constants/sndseq.h"
	.include "asm/macros.inc"
	.include "overlay_74_thumb.inc"
	.include "global.inc"
	.public gApplication_TitleScreen
	.public sub_020342C0

	.public MainMenuApp_Init
	.public MainMenuApp_Main
	.public MainMenuApp_Exit
	.public ov74_0223B2C0
	.public ov74_0223B2CC
	.public ov74_0223B2D8
	.public ov74_0223B2E8
	.public gApp_MainMenu_SelectOption_ConnectToRanger
	.public gApp_MainMenu_SelectOption_MigrateFromAgb
	.public gApp_MainMenu_SelectOption_MysteryGift

	.text
	.public ConnectToRanger_Init
	.public ConnectToRanger_Main
	.public ConnectToRanger_Exit
	.public MysteryGiftComm_HandleReceived
	.public MysteryGiftComm_GetRecvBufferSize
	.public MysteryGiftComm_GetRecvBuffer
	.public ov74_02229DE0
	.public ov74_02229E60
	.public MysteryGiftApp_StartSearch
	.public MysteryGiftApp_AcceptGift
	.public ov74_0222A2A4
	.public MysteryGiftApp_SelectGetFromFriend
	.public MysteryGiftApp_SelectGetViaWfc
	.public MysteryGiftApp_SelectGetViaWireless
	.public ov74_0222A494
	.public ov74_0222A4CC
	.public ov74_0222A504
	.public ov74_0222A538
	.public ov74_0222A560
	.public MysteryGiftApp_CancelReceiveMenu
	.public MysteryGiftApp_SelectReceiveGift
	.public MysteryGiftApp_SelectCheckCard
	.public MysteryGiftApp_SelectViewLockCapsuleCard
	.public MysteryGiftApp_SelectExit
	.public MysteryGiftApp_ShowWfcError
	.public MysteryGiftApp_Init
	.public MysteryGiftApp_Main
	.public MysteryGiftApp_Exit
	.public WonderCardAlbum_SelectShare
	.public WonderCardAlbum_SelectTrash
	.public WonderCardAlbum_ConfirmTrash
	.public WonderCardAlbum_ReturnToCardMenu
	.public WonderCardAlbum_PrintMessageOnly
	.public WonderCardAlbum_PrintCardTitle
	.public WonderCardAlbum_PrintGiftStatus
	.public WonderCardAlbum_BufferReceiveDate
	.public WonderCardAlbum_PrintCardText
	.public WonderCardAlbum_PrintSharesLeft
	.public WonderCardAlbum_ShowCardMenu
	.public WonderCardAlbum_ShowTrashYesNo
	.public WonderCardAlbum_ShowShareYesNo
	.public ov74_0222D7D0
	.public WonderCardAlbum_Init
	.public WonderCardAlbum_Main
	.public WonderCardAlbum_Exit
	.public LockCapsuleCard_PrintMessageOnly
	.public LockCapsuleCard_PrintCardTitle
	.public LockCapsuleCard_PrintStatus
	.public LockCapsuleCard_BufferReceiveDate
	.public LockCapsuleCard_PrintCardText
	.public LockCapsuleCard_PrintNothing
	.public LockCapsuleCard_ShowCardMenu
	.public LockCapsuleCard_Init
	.public LockCapsuleCard_Main
	.public LockCapsuleCard_Exit
	.public RangerLink_SetMPDataCallback
	.public RangerLink_EnableCallback
	.public RangerLink_DisableCallback
	.public RangerLink_MeasureChannelCallback
	.public RangerLink_SetParentParameterCallback
	.public RangerLink_StartParentCallback
	.public RangerLink_ScanCallback
	.public RangerLink_ScanSelectedCallback
	.public RangerLink_EndScanCallback
	.public RangerLink_StartConnectCallback
	.public RangerLink_StartMPCallback
	.public RangerLink_PortCallback
	.public RangerLink_PowerOnCallback
	.public RangerLink_PowerOffCallback
	.public RangerLink_ResetCallback
	.public RangerLink_EndCallback
	.extern _0223B330
	.extern _0223B340
	.extern sMysteryGiftCommCommands
	.extern _0223B390
	.extern _0223B3A0
	.extern sMysteryGiftSparkleDelays
	.extern _0223B410
	.extern _0223B420
	.extern _0223B430
	.extern _0223B440
	.extern _0223B45C
	.extern _0223B478
	.extern _0223B494
	.extern _0223B4B0
	.extern gApp_MainMenu_SelectOption_ConnectToRanger
	.extern gApp_MainMenu_SelectOption_MigrateFromAgb
	.extern gApp_MainMenu_SelectOption_MysteryGift
	.extern RangerLink_IsFinished
	.extern RangerLink_GetParentParam
	.extern RangerLink_GetWork
	.extern RangerLink_GetState
	.extern RangerLink_SetState
	.extern RangerLink_GetRequestedState
	.extern RangerLink_SetRequestedState
	.extern RangerLink_GetMode
	.extern RangerLink_SetMode
	.extern RangerLink_SetChannel
	.extern RangerLink_GetScanParam
	.extern RangerLink_GetBssDesc
	.extern RangerLink_SetAid
	.extern RangerLink_IsConnectedToParent
	.extern RangerLink_SetConnected
	.extern RangerLink_SetDisconnected
	.extern RangerLink_SetFinished
	.extern RangerLink_GetFoundParents
	.extern RangerLink_GetChildren
	.extern RangerLink_ClearFoundParents
	.extern RangerLink_GetTransfer
	.extern RangerLink_ResetTransfer
	.extern RangerLink_GetGgid
	.extern RangerLink_GetGameCode
	.extern RangerLink_GetVersion
	.extern RangerLink_GetReceiverId
	.extern RangerLink_GetGameInfo
	.extern RangerLink_ReadMacAddressTail
	.extern RangerLink_ReadMacAddressHead
	.extern RangerLink_SplitDecimal
	.extern RangerLink_GetPacketHeaderSize
	.extern RangerLink_BuildPacket
	.extern RangerLink_Unscramble
	.extern RangerLink_CheckCrc
	.extern RangerLink_DebugError
	.extern RangerLink_DebugState
	.extern RangerLink_DebugScan
	.extern RangerLink_DebugFoundParents
	.extern RangerLink_DebugChildren
	.extern RangerLink_DebugPacket
	.extern RangerLink_SetMPData
	.extern RangerLink_WmEnable
	.extern RangerLink_WmDisable
	.extern RangerLink_WmMeasureChannel
	.extern RangerLink_WmSetParentParameter
	.extern RangerLink_WmStartParent
	.extern RangerLink_WmStartScan
	.extern RangerLink_WmStartScanSelected
	.extern RangerLink_WmEndScan
	.extern RangerLink_WmStartConnect
	.extern RangerLink_WmStartMP
	.extern RangerLink_WmPowerOn
	.extern RangerLink_WmPowerOff
	.extern RangerLink_WmReset
	.extern RangerLink_WmEnd
	.extern ov74_0223512C
	.extern ov74_02235230
	.extern ov74_02235258
	.extern ov74_022352A0
	.extern ov74_022352D0
	.extern ov74_02235308
	.extern ov74_02235390
	.extern ov74_0223539C
	.extern ov74_022353FC
	.extern ov74_02235414
	.extern ov74_02235464
	.extern ov74_0223546C
	.extern ov74_02235474
	.extern ov74_02235568
	.extern ov74_02235634
	.extern ov74_0223563C
	.extern ov74_0223567C
	.extern ov74_02235690
	.extern ov74_02235708
	.extern ov74_02235728
	.extern ov74_022358BC
	.extern ov74_022358C8
	.extern ov74_02235930
	.extern ov74_022359BC
	.extern ov74_02235A68
	.extern ov74_02235DC4
	.extern ov74_02235ED0
	.extern ov74_02235F58
	.extern ov74_02236034
	.extern ov74_022360A0
	.extern ov74_022360B0
	.extern ov74_02236128
	.extern ov74_02236140
	.extern ov74_0223615C
	.extern ov74_022365FC
	.extern ov74_02236680
	.extern ov74_022368D4
	.extern ov74_02236980
	.extern ov74_02236988
	.extern ov74_02236AE0
	.extern ov74_0223BD5C
	.extern sRangerCharTable
	.extern ov74_0223C1F4
	.extern ov74_0223C220
	.extern ov74_0223C230
	.extern ov74_0223C240
	.extern ov74_0223C250
	.extern ov74_0223C260
	.extern ov74_0223C270
	.extern ov74_0223C2A0
	.extern ov74_0223C2B0
	.extern ov74_0223C2D0
	.extern ov74_0223C2E0
	.extern ov74_0223C2F0
	.extern ov74_0223C320
	.extern ov74_0223C340
	.extern ov74_0223C6D0
	.extern ov74_0223C6E0
	.extern ov74_0223C700
	.extern gConnectToRanger_PlayerName
	.extern gConnectToRanger_TrainerId
	.extern sMysteryGiftCommWork
	.extern ov74_0223D0A8
	.extern ov74_0223D0BC
	.extern ov74_0223D0C0

	.extern ConnectToRanger_ConvertCharToRanger
	.extern ConnectToRanger_FormatNumber5Digits
	.extern ConnectToRanger_EncodePlayerData
	.extern ConnectToRanger_SetupGraphics
	.extern ConnectToRanger_LoadSubScreenBg
	.extern ConnectToRanger_ShowPokedexRequiredMessage
	.extern ConnectToRanger_PrintScreen
	.extern ConnectToRanger_PrintPlayerInfo
	.extern ConnectToRanger_FreeGraphics
	.extern ConnectToRanger_Init
	.extern ConnectToRanger_Main
	.extern ConnectToRanger_Exit
	.extern ConnectToRanger_StoreManaphyEggCard
	.extern ConnectToRanger_SetTimeout
	.extern ConnectToRanger_CheckTimeoutOrCancel
	.extern ConnectToRanger_RunTransfer
	.extern MysteryGiftComm_Start
	.extern MysteryGiftComm_SetState
	.extern ov74_02229BD0
	.extern ov74_02229C0C
	.extern ov74_02229C44
	.extern ov74_02229C5C
	.extern ov74_02229C60
	.extern ov74_02229C80
	.extern ov74_02229CB4
	.extern MysteryGiftComm_Task
	.extern ov74_02229CE0
	.extern ov74_02229D0C
	.extern ov74_02229D1C
	.extern ov74_02229D50
	.extern ov74_02229D6C
	.extern MysteryGiftComm_HandleReceived
	.extern MysteryGiftComm_IsReceived
	.extern MysteryGiftComm_GetRecvBufferSize
	.extern MysteryGiftComm_GetRecvBuffer
	.extern ov74_02229DE0
	.extern MysteryGiftComm_Free
	.extern MysteryGiftComm_RegisterCommands
	.extern MysteryGiftApp_SetWaitingIconVisible
	.extern ov74_02229E60
	.extern MysteryGiftApp_FreeGraphics
	.extern MysteryGiftApp_InitSprites
	.extern MysteryGiftApp_SetSpriteAnim
	.extern MysteryGiftApp_EraseWindowIfInUse
	.extern MysteryGiftApp_ClearWindowIfInUse
	.extern MysteryGiftApp_StartSearch
	.extern MysteryGiftApp_AcceptGift
	.extern MysteryGiftApp_GetReceivedCard
	.extern ov74_0222A0E4
	.extern MysteryGiftApp_WaitForSave
	.extern MysteryGiftApp_SaveReceivedGift
	.extern MysteryGiftApp_ShowReceivePrompt
	.extern ov74_0222A2A4
	.extern MysteryGiftApp_SelectGetFromFriend
	.extern MysteryGiftApp_SelectGetViaWfc
	.extern MysteryGiftApp_SelectGetViaWireless
	.extern ov74_0222A43C
	.extern ov74_0222A494
	.extern ov74_0222A4CC
	.extern ov74_0222A504
	.extern ov74_0222A538
	.extern ov74_0222A560
	.extern MysteryGiftApp_CancelReceiveMenu
	.extern MysteryGiftApp_SelectReceiveGift
	.extern MysteryGiftApp_SelectCheckCard
	.extern MysteryGiftApp_SelectViewLockCapsuleCard
	.extern MysteryGiftApp_SelectExit
	.extern MysteryGiftApp_InitBgLayer
	.extern MysteryGiftApp_InitBgLayers
	.extern MysteryGiftApp_LoadSubScreenBg
	.extern MysteryGiftApp_LoadMainScreenBg
	.extern MysteryGiftApp_CreateListMenu
	.extern MysteryGiftApp_ShowMainMenu
	.extern MysteryGiftApp_PrintMessage
	.extern MysteryGiftApp_PrintGiftTitle
	.extern MysteryGiftApp_HandleListMenuInput
	.extern MysteryGiftApp_SetupScreen
	.extern MysteryGiftApp_RunWfcDownload
	.extern MysteryGiftApp_ShowWfcError
	.extern MysteryGiftApp_ShowCannotReceiveMessage
	.extern MysteryGiftApp_PrintMessageAndWait
	.extern MysteryGiftApp_StopReceiving
	.extern MysteryGiftApp_Init
	.extern MysteryGiftAnim_LoadResources
	.extern MysteryGiftAnim_CreateSprite
	.extern MysteryGiftAnim_CreateGiftMainSprite
	.extern MysteryGiftAnim_CreateGiftSubSprite
	.extern MysteryGiftAnim_DeleteGiftMainSprite
	.extern MysteryGiftAnim_DeleteGiftSubSprite
	.extern MysteryGiftAnim_UpdateSprites
	.extern MysteryGiftAnim_CreateSparkles
	.extern ov74_0222B2C4
	.extern MysteryGiftAnim_CreateRibbons
	.extern MysteryGiftAnim_DeleteSparkles
	.extern MysteryGiftAnim_DeleteRibbons
	.extern ov74_0222B3A4
	.extern ov74_0222B5F8
	.extern ov74_0222B760
	.extern MysteryGiftAnim_StepTowards
	.extern MysteryGiftAnim_UpdateParticle
	.extern MysteryGiftAnim_ApplyBrightnessMain
	.extern MysteryGiftAnim_ApplyBrightnessSub
	.extern MysteryGiftAnim_ApplyBrightness
	.extern ov74_0222BF18
	.extern MysteryGiftAnim_StepBrightness
	.extern MysteryGiftAnim_StepBrightnessMain
	.extern MysteryGiftAnim_Init
	.extern MysteryGiftAnim_Task
	.extern MysteryGiftApp_Main
	.extern MysteryGiftApp_GetOverlayManager
	.extern MysteryGiftApp_SetOverlayManager
	.extern MysteryGiftApp_Exit
	.extern ov74_0222CE10
	.extern ov74_0222CE6C
	.extern ov74_0222CEC0
	.extern ov74_0222CEE0
	.extern WonderCardAlbum_LoadShareSprites
	.extern WonderCardAlbum_DeleteSprites
	.extern WonderCardAlbum_CreateButtonSprites
	.extern WonderCardAlbum_EraseWindowIfInUse
	.extern WonderCardAlbum_ClearWindowIfInUse
	.extern WonderCardAlbum_CreateListMenu
	.extern WonderCardAlbum_PrintMessage
	.extern WonderCardAlbum_LoadShareScreenBg
	.extern WonderCardAlbum_SelectShare
	.extern WonderCardAlbum_SelectTrash
	.extern WonderCardAlbum_ConfirmTrash
	.extern WonderCardAlbum_ReturnToCardMenu
	.extern WonderCardAlbum_DestroyListMenu
	.extern WonderCardAlbum_PrintMessageOnly
	.extern WonderCardAlbum_PrintCardTitle
	.extern WonderCardAlbum_PrintGiftStatus
	.extern WonderCardAlbum_BufferReceiveDate
	.extern WonderCardAlbum_PrintCardText
	.extern WonderCardAlbum_PrintSharesLeft
	.extern WonderCardAlbum_ShowCardMenu
	.extern WonderCardAlbum_ShowTrashYesNo
	.extern WonderCardAlbum_ShowShareYesNo
	.extern WonderCardAlbum_ShowWirelessYesNo
	.extern ov74_0222D7D0
	.extern WonderCardAlbum_StartFade
	.extern WonderCardAlbum_DrawCard
	.extern WonderCardAlbum_PrintTextBox
	.extern WonderCardAlbum_FindNextCard
	.extern WonderCardAlbum_LoadScreenData
	.extern WonderCardAlbum_LoadCardBg
	.extern WonderCardAlbum_HandleListMenuInput
	.extern WonderCardAlbum_CreateMonIconSprites
	.extern WonderCardAlbum_Init
	.extern WonderCardAlbum_FlipEffectTask
	.extern WonderCardAlbum_StartFlipEffect
	.extern WonderCardAlbum_StopFlipEffect
	.extern WonderCardAlbum_UpdateFlipEffect
	.extern WonderCardAlbum_WaitForSave
	.extern WonderCardAlbum_CountShare
	.extern WonderCardAlbum_HandleShareInput
	.extern WonderCardAlbum_Main
	.extern WonderCardAlbum_StartSharing
	.extern WonderCardAlbum_TakeEarliest
	.extern WonderCardAlbum_CountReceivers
	.extern WonderCardAlbum_UpdateReceiverList
	.extern WonderCardAlbum_PrintReceiverCount
	.extern WonderCardAlbum_StopSharing
	.extern WonderCardAlbum_Exit
	.extern WonderCardAlbum_DrawCardOnBg
	.extern LockCapsuleCard_DeleteSprites
	.extern LockCapsuleCard_EraseWindowIfInUse
	.extern LockCapsuleCard_ClearWindowIfInUse
	.extern LockCapsuleCard_CreateListMenu
	.extern LockCapsuleCard_DestroyListMenu
	.extern LockCapsuleCard_PrintMessageOnly
	.extern LockCapsuleCard_PrintCardTitle
	.extern LockCapsuleCard_PrintStatus
	.extern LockCapsuleCard_BufferReceiveDate
	.extern LockCapsuleCard_PrintCardText
	.extern LockCapsuleCard_PrintNothing
	.extern LockCapsuleCard_ShowCardMenu
	.extern LockCapsuleCard_StartFade
	.extern LockCapsuleCard_DrawCard
	.extern LockCapsuleCard_PrintTextBox
	.extern LockCapsuleCard_LoadScreenData
	.extern LockCapsuleCard_LoadCardBg
	.extern LockCapsuleCard_HandleListMenuInput
	.extern LockCapsuleCard_CreateMonIconSprites
	.extern LockCapsuleCard_Init
	.extern LockCapsuleCard_FlipEffectTask
	.extern LockCapsuleCard_StartFlipEffect
	.extern LockCapsuleCard_StopFlipEffect
	.extern LockCapsuleCard_UpdateFlipEffect
	.extern LockCapsuleCard_Main
	.extern LockCapsuleCard_Exit
	.extern LockCapsuleCard_DrawCardOnBg
	.extern WonderCard_SetGraphicsBanks
	.extern WonderCard_InitBgLayers
	.extern MysteryGift_CheckCanReceive
	.extern RangerLink_Abort
	.extern RangerLink_GetMacAddressTail
	.extern RangerLink_GetMacAddressHead
	.extern RangerLink_HandleEndRequest
	.extern RangerLink_RemoveChild
	.extern RangerLink_NextChannelToMeasure
	.extern RangerLink_InitChannelMeasure
	.extern RangerLink_SetBeaconTiming
	.extern RangerLink_InitScanParam
	.extern RangerLink_NextScanChannel
	.extern RangerLink_IndicationCallback
	.extern RangerLink_SetMPDataCallback
	.extern RangerLink_EnableCallback
	.extern RangerLink_DisableCallback
	.extern RangerLink_MeasureChannelCallback
	.extern RangerLink_SetParentParameterCallback
	.extern RangerLink_StartParentCallback
	.extern RangerLink_AddFoundParent
	.extern RangerLink_ScanNextChannel
	.extern RangerLink_HandleBeacon
	.extern RangerLink_ScanCallback
	.extern RangerLink_HandleSelectedBeacon
	.extern RangerLink_ScanSelectedCallback
	.extern RangerLink_EndScanCallback
	.extern RangerLink_StartConnectCallback
	.extern RangerLink_StartAfterPowerOn
	.extern RangerLink_StartMPCallback
	.extern RangerLink_ReceiveAck
	.extern RangerLink_ReceiveListPacket
	.extern RangerLink_ReceiveCommand
	.extern RangerLink_PortCallback
	.extern RangerLink_PowerOnCallback
	.extern RangerLink_PowerOffCallback
	.extern RangerLink_ResetCallback
	.extern RangerLink_EndCallback
	.extern RangerLink_SetIndicationCallback
	.extern RangerLink_Begin
	.extern RangerLink_SendData
	.extern RangerLink_GetSendBufferSize
	.extern RangerLink_GetRecvBufferSize
	.extern RangerLink_Init
	.extern RangerLink_UpdateSend
	.extern RangerLink_UpdateCommand
	.extern RangerLink_UpdateTransfer
	.extern RangerLink_UpdateChild
	.extern RangerLink_UpdateParent
	.extern RangerLink_Update
	.extern RangerLink_StartChild
	.extern RangerLink_CountFoundParents
	.extern RangerLink_CanSelectParent
	.extern RangerLink_SelectParent
	.extern RangerLink_CountActiveChildren
	.extern RangerLink_GetLinkLevel
	.extern RangerLink_CountPeers
	.extern RangerLink_StartSendList
	.extern RangerLink_StartSendNames
	.extern RangerLink_StartCommand
	.extern RangerLink_IsSendDone
	.extern RangerLink_IsCommandDone
	.extern RangerLink_IsCommandAcknowledged
	.extern RangerLink_IsConnected
	.extern RangerLink_RequestEnd



	thumb_func_start MysteryGiftAnim_Init
MysteryGiftAnim_Init: ; 0x0222C014
	push {r4, lr}
	add r4, r0, #0
	mov r1, #0
	strb r1, [r4, #8]
	bl MysteryGiftAnim_ApplyBrightness
	add r0, r4, #0
	bl MysteryGiftAnim_LoadResources
	add r0, r4, #0
	bl MysteryGiftAnim_CreateSparkles
	add r0, r4, #0
	bl ov74_0222B3A4
	add r0, r4, #0
	bl MysteryGiftAnim_CreateGiftMainSprite
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineBTogglePlanes
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end MysteryGiftAnim_Init


	thumb_func_start MysteryGiftAnim_Task
MysteryGiftAnim_Task: ; 0x0222C04C
	push {r3, r4, r5, r6, r7, lr}
	add r4, r1, #0
	ldr r1, [r4]
	cmp r1, #9
	bls _0222C058
	b _0222C2BC
_0222C058:
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0222C064: ; jump table
	.short _0222C078 - _0222C064 - 2 ; case 0
	.short _0222C084 - _0222C064 - 2 ; case 1
	.short _0222C09A - _0222C064 - 2 ; case 2
	.short _0222C0FE - _0222C064 - 2 ; case 3
	.short _0222C126 - _0222C064 - 2 ; case 4
	.short _0222C17C - _0222C064 - 2 ; case 5
	.short _0222C19A - _0222C064 - 2 ; case 6
	.short _0222C222 - _0222C064 - 2 ; case 7
	.short _0222C294 - _0222C064 - 2 ; case 8
	.short _0222C2A8 - _0222C064 - 2 ; case 9
_0222C078:
	add r0, r4, #0
	bl MysteryGiftAnim_Init
	mov r0, #1
	str r0, [r4]
	b _0222C2D0
_0222C084:
	add r0, r4, #0
	mov r1, #1
	mov r2, #2
	bl MysteryGiftAnim_StepBrightness
	cmp r0, #0
	bne _0222C094
	b _0222C2D0
_0222C094:
	mov r0, #2
	str r0, [r4]
	b _0222C2D0
_0222C09A:
	mov r5, #0
	mov r6, #1
	add r0, r5, #0
	add r1, r4, #0
	add r3, r5, #0
_0222C0A4:
	add r2, r1, #0
	add r2, #0x90
	ldr r2, [r2]
	cmp r2, #0
	beq _0222C0B2
	add r5, r5, #1
	b _0222C0B4
_0222C0B2:
	add r6, r3, #0
_0222C0B4:
	add r0, r0, #1
	add r1, #0x4c
	cmp r0, #0x50
	blt _0222C0A4
	cmp r5, #0x32
	ble _0222C0CC
	ldr r0, _0222C2D8 ; =0x00003014
	mov r1, #4
	ldr r0, [r4, r0]
	bl Sprite_TryChangeAnimSeq
	b _0222C0EA
_0222C0CC:
	cmp r5, #0x1e
	ble _0222C0DC
	ldr r0, _0222C2D8 ; =0x00003014
	mov r1, #3
	ldr r0, [r4, r0]
	bl Sprite_TryChangeAnimSeq
	b _0222C0EA
_0222C0DC:
	cmp r5, #7
	ble _0222C0EA
	ldr r0, _0222C2D8 ; =0x00003014
	mov r1, #1
	ldr r0, [r4, r0]
	bl Sprite_SetDrawFlag
_0222C0EA:
	cmp r6, #0
	bne _0222C0F2
	cmp r5, #0x4f
	bne _0222C1A8
_0222C0F2:
	mov r0, #3
	str r0, [r4]
	add r0, r4, #0
	bl MysteryGiftAnim_DeleteSparkles
	b _0222C2D0
_0222C0FE:
	ldr r0, _0222C2DC ; =0x000030A0
	ldr r0, [r4, r0]
	ldr r0, [r0]
	cmp r0, #2
	bne _0222C1A8
	add r0, r4, #0
	bl ov74_0222B2C4
	add r0, r4, #0
	bl ov74_0222B5F8
	mov r0, #2
	mov r1, #0
	bl GfGfx_EngineATogglePlanes
	bl ov74_0222BF18
	mov r0, #4
	str r0, [r4]
	b _0222C2D0
_0222C126:
	mov r5, #0
	mov r6, #1
	add r1, r5, #0
	add r2, r4, #0
	add r0, r5, #0
_0222C130:
	add r3, r2, #0
	add r3, #0x94
	ldr r3, [r3]
	cmp r3, #0
	beq _0222C14A
	add r3, r2, #0
	add r3, #0x90
	ldr r3, [r3]
	cmp r3, #0
	beq _0222C148
	add r5, r5, #1
	b _0222C14A
_0222C148:
	add r6, r0, #0
_0222C14A:
	add r1, r1, #1
	add r2, #0x4c
	cmp r1, #0x50
	blt _0222C130
	mov r1, #2
	add r0, r4, #0
	add r2, r1, #0
	bl MysteryGiftAnim_StepBrightnessMain
	cmp r6, #0
	bne _0222C164
	cmp r5, #0x13
	bne _0222C1A8
_0222C164:
	ldr r0, _0222C2E0 ; =SEQ_SE_DP_SAVE
	bl PlaySE
	mov r0, #5
	str r0, [r4]
	add r0, r4, #0
	bl MysteryGiftAnim_DeleteSparkles
	add r0, r4, #0
	bl MysteryGiftAnim_CreateGiftSubSprite
	b _0222C2D0
_0222C17C:
	mov r1, #2
	add r0, r4, #0
	add r2, r1, #0
	bl MysteryGiftAnim_StepBrightnessMain
	cmp r0, #0
	beq _0222C1A8
	add r0, r4, #0
	mov r1, #1
	mov r2, #2
	bl MysteryGiftAnim_StepBrightnessMain
	mov r0, #6
	str r0, [r4]
	b _0222C2D0
_0222C19A:
	add r0, r4, #0
	mov r1, #1
	mov r2, #2
	bl MysteryGiftAnim_StepBrightnessMain
	cmp r0, #0
	bne _0222C1AA
_0222C1A8:
	b _0222C2D0
_0222C1AA:
	ldr r0, _0222C2D8 ; =0x00003014
	ldr r0, [r4, r0]
	bl Sprite_GetMatrixPtr
	add r5, r0, #0
	ldr r0, _0222C2E4 ; =0x00003060
	ldr r0, [r4, r0]
	bl Sprite_GetMatrixPtr
	mov r1, #0xe
	ldr r2, [r5, #4]
	lsl r1, r1, #0x10
	cmp r2, r1
	bge _0222C1CE
	mov r1, #2
	lsl r1, r1, #0xe
	add r1, r2, r1
	str r1, [r5, #4]
_0222C1CE:
	mov r1, #3
	ldr r2, [r5, #4]
	lsl r1, r1, #0x12
	sub r2, r2, r1
	cmp r2, #0
	ble _0222C2D0
	ldr r2, [r0, #4]
	lsl r1, r1, #1
	cmp r2, r1
	bge _0222C202
	mov r1, #2
	lsl r1, r1, #0xe
	add r1, r2, r1
	str r1, [r0, #4]
	ldr r0, _0222C2E4 ; =0x00003060
	ldr r0, [r4, r0]
	bl Sprite_GetDrawFlag
	cmp r0, #0
	bne _0222C2D0
	ldr r0, _0222C2E4 ; =0x00003060
	mov r1, #1
	ldr r0, [r4, r0]
	bl Sprite_SetDrawFlag
	b _0222C2D0
_0222C202:
	mov r0, #7
	str r0, [r4]
	add r0, r4, #0
	bl MysteryGiftAnim_DeleteGiftMainSprite
	add r0, r4, #0
	bl MysteryGiftAnim_CreateRibbons
	add r0, r4, #0
	bl ov74_0222B760
	mov r0, #2
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	b _0222C2D0
_0222C222:
	mov r5, #0
	ldr r0, _0222C2E8 ; =0x00001850
	mov r6, #1
	add r2, r5, #0
	add r3, r4, #0
	add r7, r5, #0
_0222C22E:
	ldr r1, [r3, r0]
	cmp r1, #0
	beq _0222C238
	add r5, r5, #1
	b _0222C23A
_0222C238:
	add r6, r7, #0
_0222C23A:
	add r2, r2, #1
	add r3, #0x4c
	cmp r2, #0x50
	blt _0222C22E
	cmp r5, #7
	ble _0222C250
	add r0, r4, #0
	mov r1, #2
	mov r2, #1
	bl MysteryGiftAnim_StepBrightness
_0222C250:
	cmp r5, #0x32
	ble _0222C260
	ldr r0, _0222C2E4 ; =0x00003060
	mov r1, #0
	ldr r0, [r4, r0]
	bl Sprite_SetDrawFlag
	b _0222C27E
_0222C260:
	cmp r5, #0x1e
	ble _0222C270
	ldr r0, _0222C2E4 ; =0x00003060
	mov r1, #2
	ldr r0, [r4, r0]
	bl Sprite_TryChangeAnimSeq
	b _0222C27E
_0222C270:
	cmp r5, #7
	ble _0222C27E
	ldr r0, _0222C2E4 ; =0x00003060
	mov r1, #3
	ldr r0, [r4, r0]
	bl Sprite_TryChangeAnimSeq
_0222C27E:
	cmp r6, #0
	beq _0222C2D0
	mov r0, #8
	str r0, [r4]
	add r0, r4, #0
	bl MysteryGiftAnim_DeleteRibbons
	add r0, r4, #0
	bl MysteryGiftAnim_DeleteGiftSubSprite
	b _0222C2D0
_0222C294:
	add r0, r4, #0
	mov r1, #2
	mov r2, #1
	bl MysteryGiftAnim_StepBrightness
	cmp r0, #0
	beq _0222C2D0
	mov r0, #9
	str r0, [r4]
	b _0222C2D0
_0222C2A8:
	add r0, r4, #0
	mov r1, #0
	mov r2, #2
	bl MysteryGiftAnim_StepBrightness
	cmp r0, #0
	beq _0222C2D0
	mov r0, #0xff
	str r0, [r4]
	b _0222C2D0
_0222C2BC:
	ldr r1, _0222C2DC ; =0x000030A0
	mov r2, #0
	ldr r1, [r4, r1]
	str r2, [r1]
	bl SysTask_Destroy
	add r0, r4, #0
	bl Heap_Free
	pop {r3, r4, r5, r6, r7, pc}
_0222C2D0:
	add r0, r4, #0
	bl MysteryGiftAnim_UpdateSprites
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0222C2D8: .word 0x00003014
_0222C2DC: .word 0x000030A0
_0222C2E0: .word SEQ_SE_DP_SAVE
_0222C2E4: .word 0x00003060
_0222C2E8: .word 0x00001850
	thumb_func_end MysteryGiftAnim_Task


	thumb_func_start MysteryGiftApp_Main
MysteryGiftApp_Main: ; 0x0222C2EC
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r6, r0, #0
	add r5, r1, #0
	bl OverlayManager_GetData
	add r4, r0, #0
	bl CTRDG_IsExisting
	ldr r0, _0222C640 ; =0x000005C8
	ldr r0, [r4, r0]
	cmp r0, #0x1d
	beq _0222C30C
	add r0, r4, #0
	bl ov74_0222CE10
_0222C30C:
	ldr r3, [r5]
	cmp r3, #0x3a
	bls _0222C316
	bl _0222CD3A
_0222C316:
	add r0, r3, r3
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0222C322: ; jump table
	.short _0222C398 - _0222C322 - 2 ; case 0
	.short _0222C3AA - _0222C322 - 2 ; case 1
	.short _0222C3C8 - _0222C322 - 2 ; case 2
	.short _0222C3D2 - _0222C322 - 2 ; case 3
	.short _0222C3E0 - _0222C322 - 2 ; case 4
	.short _0222C3EE - _0222C322 - 2 ; case 5
	.short _0222CD3A - _0222C322 - 2 ; case 6
	.short _0222C636 - _0222C322 - 2 ; case 7
	.short _0222C71C - _0222C322 - 2 ; case 8
	.short _0222C728 - _0222C322 - 2 ; case 9
	.short _0222C7E8 - _0222C322 - 2 ; case 10
	.short _0222C836 - _0222C322 - 2 ; case 11
	.short _0222C858 - _0222C322 - 2 ; case 12
	.short _0222C8E8 - _0222C322 - 2 ; case 13
	.short _0222C93E - _0222C322 - 2 ; case 14
	.short _0222C96A - _0222C322 - 2 ; case 15
	.short _0222C9AA - _0222C322 - 2 ; case 16
	.short _0222C9CC - _0222C322 - 2 ; case 17
	.short _0222CA04 - _0222C322 - 2 ; case 18
	.short _0222CA48 - _0222C322 - 2 ; case 19
	.short _0222CAEA - _0222C322 - 2 ; case 20
	.short _0222CAF6 - _0222C322 - 2 ; case 21
	.short _0222CAFC - _0222C322 - 2 ; case 22
	.short _0222CB32 - _0222C322 - 2 ; case 23
	.short _0222CB4C - _0222C322 - 2 ; case 24
	.short _0222CBC4 - _0222C322 - 2 ; case 25
	.short _0222CD3A - _0222C322 - 2 ; case 26
	.short _0222C3FC - _0222C322 - 2 ; case 27
	.short _0222CD3A - _0222C322 - 2 ; case 28
	.short _0222CD3A - _0222C322 - 2 ; case 29
	.short _0222CD3A - _0222C322 - 2 ; case 30
	.short _0222C4BA - _0222C322 - 2 ; case 31
	.short _0222C408 - _0222C322 - 2 ; case 32
	.short _0222C4CE - _0222C322 - 2 ; case 33
	.short _0222C570 - _0222C322 - 2 ; case 34
	.short _0222C54E - _0222C322 - 2 ; case 35
	.short _0222C612 - _0222C322 - 2 ; case 36
	.short _0222CD3A - _0222C322 - 2 ; case 37
	.short _0222CD3A - _0222C322 - 2 ; case 38
	.short _0222CD3A - _0222C322 - 2 ; case 39
	.short _0222CD3A - _0222C322 - 2 ; case 40
	.short _0222CD3A - _0222C322 - 2 ; case 41
	.short _0222CD3A - _0222C322 - 2 ; case 42
	.short _0222CD3A - _0222C322 - 2 ; case 43
	.short _0222CD3A - _0222C322 - 2 ; case 44
	.short _0222CD3A - _0222C322 - 2 ; case 45
	.short _0222CD3A - _0222C322 - 2 ; case 46
	.short _0222CD3A - _0222C322 - 2 ; case 47
	.short _0222CD3A - _0222C322 - 2 ; case 48
	.short _0222CBDE - _0222C322 - 2 ; case 49
	.short _0222CC02 - _0222C322 - 2 ; case 50
	.short _0222CC10 - _0222C322 - 2 ; case 51
	.short _0222CC2E - _0222C322 - 2 ; case 52
	.short _0222CC44 - _0222C322 - 2 ; case 53
	.short _0222CC58 - _0222C322 - 2 ; case 54
	.short _0222CC6A - _0222C322 - 2 ; case 55
	.short _0222CC7E - _0222C322 - 2 ; case 56
	.short _0222CCC0 - _0222C322 - 2 ; case 57
	.short _0222CD34 - _0222C322 - 2 ; case 58
_0222C398:
	mov r0, #0x54
	bl ov74_022352A0
	ldr r0, _0222C644 ; =0x000015D8
	str r5, [r4, r0]
	mov r0, #1
	str r0, [r5]
	bl _0222CD3A
_0222C3AA:
	add r0, r6, #0
	add r1, r4, #0
	bl MysteryGiftApp_SetupScreen
	add r0, r4, #0
	bl MysteryGiftApp_InitSprites
	mov r0, #1
	mov r1, #3
	add r2, r5, #0
	mov r3, #2
	bl ov74_0223539C
	bl _0222CD3A
_0222C3C8:
	add r0, r5, #0
	bl ov74_022353FC
	bl _0222CD3A
_0222C3D2:
	ldr r2, _0222C648 ; =MysteryGiftApp_SelectExit
	add r0, r6, #0
	add r1, r5, #0
	bl MysteryGiftApp_HandleListMenuInput
	bl _0222CD3A
_0222C3E0:
	ldr r2, _0222C64C ; =MysteryGiftApp_CancelReceiveMenu
	add r0, r6, #0
	add r1, r5, #0
	bl MysteryGiftApp_HandleListMenuInput
	bl _0222CD3A
_0222C3EE:
	ldr r2, _0222C650 ; =ov74_0222A2A4
	add r0, r6, #0
	add r1, r5, #0
	bl MysteryGiftApp_HandleListMenuInput
	bl _0222CD3A
_0222C3FC:
	add r0, r6, #0
	add r1, r5, #0
	bl MysteryGiftApp_RunWfcDownload
	bl _0222CD3A
_0222C408:
	add r0, r4, #0
	bl ov74_0222CEE0
	ldr r0, _0222C654 ; =ov74_0223D0A8
	ldr r0, [r0, #0x10]
	cmp r0, #0x2d
	bne _0222C4A0
	add r0, r4, #0
	mov r1, #0
	bl MysteryGiftApp_SetWaitingIconVisible
	add r0, r6, #0
	bl MysteryGiftApp_GetReceivedCard
	add r1, r4, #0
	ldr r0, [r4, #4]
	add r1, #0x8c
	bl MysteryGift_CheckCanReceive
	add r1, r4, #0
	add r1, #0x80
	str r0, [r1]
	add r0, r4, #0
	add r0, #0x80
	ldr r0, [r0]
	cmp r0, #1
	bne _0222C44C
	add r0, r4, #0
	mov r1, #0
	bl MysteryGiftApp_SetWaitingIconVisible
	mov r0, #0x31
	str r0, [r5]
	b _0222C4A0
_0222C44C:
	add r0, r4, #0
	add r0, #0x58
	bl WindowIsInUse
	cmp r0, #0
	bne _0222C47A
	mov r0, #2
	str r0, [sp]
	mov r0, #0x1a
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	mov r2, #0
	mov r0, #0x71
	add r1, r4, #0
	str r2, [sp, #0xc]
	lsl r0, r0, #2
	str r0, [sp, #0x10]
	ldr r0, [r4]
	add r1, #0x58
	mov r3, #3
	bl AddWindowParameterized
_0222C47A:
	bl ov74_02236988
	add r1, r4, #0
	add r2, r0, #0
	add r0, r6, #0
	add r1, #0x58
	bl MysteryGiftApp_PrintGiftTitle
	add r1, r4, #0
	add r0, r6, #0
	add r1, #0x18
	mov r2, #4
	bl MysteryGiftApp_PrintMessage
	add r0, r6, #0
	bl ov74_0222A43C
	mov r0, #0x1f
	str r0, [r5]
_0222C4A0:
	ldr r0, _0222C654 ; =ov74_0223D0A8
	ldr r0, [r0, #0x10]
	sub r0, #0x2e
	cmp r0, #1
	bls _0222C4AE
	bl _0222CD3A
_0222C4AE:
	bl ov74_0222CEC0
	mov r0, #0x11
	str r0, [r5]
	bl _0222CD3A
_0222C4BA:
	ldr r2, _0222C658 ; =ov74_0222A538
	add r0, r6, #0
	add r1, r5, #0
	bl MysteryGiftApp_HandleListMenuInput
	add r0, r4, #0
	bl ov74_0222CEE0
	bl _0222CD3A
_0222C4CE:
	add r0, r6, #0
	bl MysteryGiftApp_GetReceivedCard
	add r7, r0, #0
	add r0, r4, #0
	mov r1, #1
	bl MysteryGiftApp_SetWaitingIconVisible
	add r0, r4, #0
	mov r1, #1
	bl MysteryGiftApp_SetSpriteAnim
	add r0, r7, #0
	bl ov74_0222A0E4
	cmp r0, #1
	bne _0222C53A
	add r0, r6, #0
	bl MysteryGiftApp_SaveReceivedGift
	ldr r0, _0222C65C ; =0x000029F4
	mov r1, #1
	str r1, [r4, r0]
	add r0, r4, #0
	sub r1, r1, #2
	bl MysteryGiftApp_SetSpriteAnim
	add r0, r4, #0
	add r0, #0x58
	mov r1, #0
	bl MysteryGiftApp_EraseWindowIfInUse
	ldr r1, _0222C660 ; =0x000030A4
	mov r0, #0x54
	bl Heap_Alloc
	ldr r2, _0222C660 ; =0x000030A4
	mov r1, #0
	add r6, r0, #0
	bl memset
	ldr r0, _0222C65C ; =0x000029F4
	add r1, r4, r0
	ldr r0, _0222C664 ; =0x000030A0
	str r1, [r6, r0]
	bl ov74_02235708
	str r0, [r6, #0xc]
	ldr r0, _0222C668 ; =MysteryGiftAnim_Task
	add r1, r6, #0
	mov r2, #5
	bl SysTask_CreateOnMainQueue
	b _0222C548
_0222C53A:
	ldr r0, [r4]
	add r1, r7, #0
	bl ov74_02235DC4
	add r0, r6, #0
	bl MysteryGiftApp_SaveReceivedGift
_0222C548:
	mov r0, #0x22
	str r0, [r5]
	b _0222CD3A
_0222C54E:
	ldr r0, _0222C65C ; =0x000029F4
	ldr r0, [r4, r0]
	cmp r0, #0
	bne _0222C5DC
	add r1, r4, #0
	add r0, r6, #0
	add r1, #0x18
	mov r2, #8
	mov r3, #0x38
	bl MysteryGiftApp_PrintMessageAndWait
	str r0, [r5]
	mov r0, #1
	add r1, r0, #0
	bl GfGfx_EngineATogglePlanes
	b _0222CD3A
_0222C570:
	bl ov74_0223615C
	add r7, r0, #0
	cmp r7, #4
	bne _0222C57E
	bl ov74_02236128
_0222C57E:
	cmp r7, #2
	bne _0222C5D8
	add r0, r4, #0
	mov r1, #0
	bl MysteryGiftApp_SetWaitingIconVisible
	mov r1, #0
	add r0, r4, #0
	mvn r1, r1
	bl MysteryGiftApp_SetSpriteAnim
	add r0, r4, #0
	add r0, #0x18
	mov r1, #0
	bl MysteryGiftApp_ClearWindowIfInUse
	ldr r0, [r4]
	mov r1, #0
	bl BgClearTilemapBufferAndCommit
	mov r0, #1
	mov r1, #0
	bl GfGfx_EngineATogglePlanes
	add r6, r4, #0
	add r6, #0xdc
	add r0, r6, #0
	bl ov74_0222A0E4
	cmp r0, #1
	bne _0222C5CC
	ldr r0, [r4]
	add r1, r6, #0
	bl ov74_02235DC4
	ldr r0, _0222C65C ; =0x000029F4
	mov r1, #2
	str r1, [r4, r0]
	b _0222C5D2
_0222C5CC:
	ldr r0, _0222C66C ; =SEQ_SE_DP_UG_020
	bl PlaySE
_0222C5D2:
	mov r0, #0x23
	str r0, [r5]
	b _0222CD3A
_0222C5D8:
	cmp r7, #3
	beq _0222C5DE
_0222C5DC:
	b _0222CD3A
_0222C5DE:
	add r0, r4, #0
	mov r1, #0
	bl MysteryGiftApp_SetWaitingIconVisible
	mov r1, #0
	add r0, r4, #0
	mvn r1, r1
	bl MysteryGiftApp_SetSpriteAnim
	add r0, r4, #0
	add r0, #0xda
	ldrb r1, [r0]
	mov r0, #4
	mov r2, #0x1b
	bic r1, r0
	add r0, r4, #0
	add r0, #0xda
	strb r1, [r0]
	add r1, r4, #0
	add r0, r6, #0
	add r1, #0x18
	mov r3, #0x38
	bl MysteryGiftApp_PrintMessageAndWait
	str r0, [r5]
	b _0222CD3A
_0222C612:
	add r0, r4, #0
	bl ov74_0222CEE0
	ldr r0, _0222C654 ; =ov74_0223D0A8
	ldr r1, [r0, #0x10]
	add r0, r1, #0
	sub r0, #0x2d
	cmp r0, #2
	bhi _0222C704
	cmp r1, #0x2f
	bne _0222C62C
	bl ov74_0222CEC0
_0222C62C:
	add r0, r6, #0
	bl ov74_0222A494
	str r0, [r5]
	b _0222CD3A
_0222C636:
	add r0, r4, #0
	bl ov74_02229D6C
	ldr r1, _0222C670 ; =0x00000438
	b _0222C674
	.balign 4, 0
_0222C640: .word 0x000005C8
_0222C644: .word 0x000015D8
_0222C648: .word MysteryGiftApp_SelectExit
_0222C64C: .word MysteryGiftApp_CancelReceiveMenu
_0222C650: .word ov74_0222A2A4
_0222C654: .word ov74_0223D0A8
_0222C658: .word ov74_0222A538
_0222C65C: .word 0x000029F4
_0222C660: .word 0x000030A4
_0222C664: .word 0x000030A0
_0222C668: .word MysteryGiftAnim_Task
_0222C66C: .word SEQ_SE_DP_UG_020
_0222C670: .word 0x00000438
_0222C674:
	str r0, [r4, r1]
	mov r0, #0
	ldr r1, [r4, r1]
	mvn r0, r0
	cmp r1, r0
	beq _0222C6EC
	add r0, r4, #0
	mov r1, #0
	bl MysteryGiftApp_SetWaitingIconVisible
	add r1, r4, #0
	ldr r0, [r4, #4]
	add r1, #0x8c
	bl MysteryGift_CheckCanReceive
	add r1, r4, #0
	add r1, #0x80
	str r0, [r1]
	add r0, r4, #0
	add r0, #0x58
	bl WindowIsInUse
	cmp r0, #0
	bne _0222C6C6
	mov r0, #2
	str r0, [sp]
	mov r0, #0x1a
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	mov r2, #0
	mov r0, #0x71
	add r1, r4, #0
	str r2, [sp, #0xc]
	lsl r0, r0, #2
	str r0, [sp, #0x10]
	ldr r0, [r4]
	add r1, #0x58
	mov r3, #3
	bl AddWindowParameterized
_0222C6C6:
	add r1, r4, #0
	add r2, r4, #0
	add r0, r6, #0
	add r1, #0x58
	add r2, #0x8c
	bl MysteryGiftApp_PrintGiftTitle
	add r1, r4, #0
	add r0, r6, #0
	add r1, #0x18
	mov r2, #4
	bl MysteryGiftApp_PrintMessage
	add r0, r6, #0
	mov r1, #0
	bl MysteryGiftApp_ShowReceivePrompt
	mov r0, #8
	str r0, [r5]
_0222C6EC:
	ldr r0, _0222CA14 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #2
	tst r0, r1
	bne _0222C706
	ldr r0, _0222CA18 ; =0x00000434
	ldr r1, [r4, r0]
	sub r1, r1, #1
	str r1, [r4, r0]
	ldr r0, [r4, r0]
	cmp r0, #0
	beq _0222C706
_0222C704:
	b _0222CD3A
_0222C706:
	add r0, r4, #0
	mov r1, #0
	bl MysteryGiftApp_SetWaitingIconVisible
	bl MysteryGiftComm_Free
	bl WirelessStatusIcon_Destroy
	mov r0, #0x11
	str r0, [r5]
	b _0222CD3A
_0222C71C:
	ldr r2, _0222CA1C ; =ov74_0222A494
	add r0, r6, #0
	add r1, r5, #0
	bl MysteryGiftApp_HandleListMenuInput
	b _0222CD3A
_0222C728:
	bl Comm_GetLocalNetId
	add r7, r0, #0
	beq _0222C780
	lsl r0, r7, #0x10
	lsr r0, r0, #0x10
	bl Comm_IsNetIdConnected
	cmp r0, #0
	beq _0222C780
	mov r0, #0xab
	bl sub_02037B38
	cmp r0, #1
	bne _0222C7B0
	add r0, r4, #0
	mov r1, #0
	bl MysteryGiftApp_SetWaitingIconVisible
	mov r0, #1
	add r1, r0, #0
	bl sub_020398D4
	add r1, r4, #0
	add r0, r6, #0
	add r1, #0x18
	mov r2, #7
	bl MysteryGiftApp_PrintMessage
	add r0, r4, #0
	mov r1, #1
	bl MysteryGiftApp_SetSpriteAnim
	add r0, r4, #0
	mov r1, #1
	bl MysteryGiftApp_SetWaitingIconVisible
	mov r1, #0x96
	ldr r0, _0222CA18 ; =0x00000434
	lsl r1, r1, #2
	str r1, [r4, r0]
	mov r0, #0xa
	str r0, [r5]
	b _0222C7B0
_0222C780:
	cmp r7, #0
	beq _0222C7B0
	lsl r0, r7, #0x10
	lsr r0, r0, #0x10
	bl Comm_IsNetIdConnected
	cmp r0, #0
	bne _0222C7B0
	bl MysteryGiftComm_Free
	bl WirelessStatusIcon_Destroy
	mov r1, #0
	add r0, r4, #0
	mvn r1, r1
	bl MysteryGiftApp_SetSpriteAnim
	add r0, r4, #0
	mov r1, #0
	bl MysteryGiftApp_SetWaitingIconVisible
	mov r0, #0x10
	str r0, [r5]
	b _0222CD3A
_0222C7B0:
	ldr r0, _0222CA14 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #2
	tst r0, r1
	bne _0222C7C8
	ldr r0, _0222CA18 ; =0x00000434
	ldr r1, [r4, r0]
	sub r1, r1, #1
	str r1, [r4, r0]
	ldr r0, [r4, r0]
	cmp r0, #0
	bne _0222C8B8
_0222C7C8:
	bl MysteryGiftComm_Free
	bl WirelessStatusIcon_Destroy
	mov r1, #0
	add r0, r4, #0
	mvn r1, r1
	bl MysteryGiftApp_SetSpriteAnim
	add r0, r4, #0
	mov r1, #0
	bl MysteryGiftApp_SetWaitingIconVisible
	mov r0, #0x11
	str r0, [r5]
	b _0222CD3A
_0222C7E8:
	bl MysteryGiftComm_IsReceived
	cmp r0, #0
	beq _0222C7F4
	mov r0, #0xb
	str r0, [r5]
_0222C7F4:
	bl Comm_GetLocalNetId
	ldr r1, _0222CA14 ; =gSystem
	ldr r2, [r1, #0x48]
	mov r1, #2
	tst r1, r2
	bne _0222C820
	ldr r1, _0222CA18 ; =0x00000434
	ldr r2, [r4, r1]
	sub r2, r2, #1
	str r2, [r4, r1]
	ldr r1, [r4, r1]
	cmp r1, #0
	beq _0222C820
	cmp r0, #0
	beq _0222C8B8
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl Comm_IsNetIdConnected
	cmp r0, #0
	bne _0222C8B8
_0222C820:
	add r0, r4, #0
	mov r1, #0
	bl MysteryGiftApp_SetWaitingIconVisible
	bl MysteryGiftComm_Free
	bl WirelessStatusIcon_Destroy
	mov r0, #0x11
	str r0, [r5]
	b _0222CD3A
_0222C836:
	add r0, r6, #0
	bl MysteryGiftApp_GetReceivedCard
	add r1, r0, #0
	ldr r0, [r4]
	bl ov74_02235DC4
	mov r0, #0
	add r1, r0, #0
	bl sub_020398D4
	add r0, r6, #0
	bl MysteryGiftApp_SaveReceivedGift
	mov r0, #0xc
	str r0, [r5]
	b _0222CD3A
_0222C858:
	bl Comm_GetLocalNetId
	cmp r0, #0
	beq _0222C896
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl Comm_IsNetIdConnected
	cmp r0, #0
	bne _0222C896
	bl MysteryGiftComm_Free
	ldr r0, _0222CA20 ; =0x000029EC
	mov r1, #1
	str r1, [r4, r0]
	bl ov74_02236140
	add r0, r4, #0
	mov r1, #0
	bl MysteryGiftApp_SetWaitingIconVisible
	bl WirelessStatusIcon_Destroy
	mov r1, #0
	add r0, r4, #0
	mvn r1, r1
	bl MysteryGiftApp_SetSpriteAnim
	mov r0, #0xe
	str r0, [r5]
	b _0222CD3A
_0222C896:
	bl ov74_0223615C
	cmp r0, #4
	bne _0222C8B0
	mov r0, #0x93
	bl sub_02037AC0
	mov r0, #0xd
	str r0, [r5]
	ldr r0, _0222CA24 ; =0x0000043C
	mov r1, #0x78
	str r1, [r4, r0]
	b _0222CD3A
_0222C8B0:
	bl ov74_0223615C
	cmp r0, #3
	beq _0222C8BA
_0222C8B8:
	b _0222CD3A
_0222C8BA:
	bl ov74_02236140
	add r0, r4, #0
	add r0, #0xda
	ldrb r1, [r0]
	mov r0, #4
	bic r1, r0
	add r0, r4, #0
	add r0, #0xda
	strb r1, [r0]
	add r0, r4, #0
	mov r1, #0
	bl MysteryGiftApp_SetWaitingIconVisible
	add r1, r4, #0
	add r0, r6, #0
	add r1, #0x18
	mov r2, #0x1b
	mov r3, #0x38
	bl MysteryGiftApp_PrintMessageAndWait
	str r0, [r5]
	b _0222CD3A
_0222C8E8:
	mov r0, #0x93
	bl sub_02037B38
	cmp r0, #1
	bne _0222C902
	bl ov74_02236128
	ldr r0, _0222CA24 ; =0x0000043C
	mov r1, #0xa
	str r1, [r4, r0]
	mov r0, #0xf
	str r0, [r5]
	b _0222CD3A
_0222C902:
	ldr r0, _0222CA24 ; =0x0000043C
	ldr r1, [r4, r0]
	sub r1, r1, #1
	str r1, [r4, r0]
	ldr r0, [r4, r0]
	cmp r0, #0
	bne _0222CA0C
	bl ov74_02236140
	add r0, r4, #0
	add r0, #0xda
	ldrb r1, [r0]
	mov r0, #4
	bic r1, r0
	add r0, r4, #0
	add r0, #0xda
	strb r1, [r0]
	add r0, r4, #0
	mov r1, #0
	bl MysteryGiftApp_SetWaitingIconVisible
	add r1, r4, #0
	add r0, r6, #0
	add r1, #0x18
	mov r2, #0x1b
	mov r3, #0x38
	bl MysteryGiftApp_PrintMessageAndWait
	str r0, [r5]
	b _0222CD3A
_0222C93E:
	add r0, r4, #0
	add r0, #0x58
	mov r1, #0
	bl MysteryGiftApp_EraseWindowIfInUse
	add r0, r4, #0
	add r0, #0xda
	ldrb r1, [r0]
	mov r0, #4
	mov r2, #0x1d
	bic r1, r0
	add r0, r4, #0
	add r0, #0xda
	strb r1, [r0]
	add r1, r4, #0
	add r0, r6, #0
	add r1, #0x18
	mov r3, #0x38
	bl MysteryGiftApp_PrintMessageAndWait
	str r0, [r5]
	b _0222CD3A
_0222C96A:
	ldr r0, _0222CA24 ; =0x0000043C
	ldr r1, [r4, r0]
	sub r1, r1, #1
	str r1, [r4, r0]
	ldr r0, [r4, r0]
	cmp r0, #0
	bne _0222CA0C
	add r0, r4, #0
	mov r1, #0
	bl MysteryGiftApp_SetWaitingIconVisible
	bl MysteryGiftComm_Free
	bl WirelessStatusIcon_Destroy
	ldr r0, _0222CA28 ; =SEQ_SE_DP_UG_020
	bl PlaySE
	mov r1, #0
	add r0, r4, #0
	mvn r1, r1
	bl MysteryGiftApp_SetSpriteAnim
	add r1, r4, #0
	add r0, r6, #0
	add r1, #0x18
	mov r2, #8
	mov r3, #0x38
	bl MysteryGiftApp_PrintMessageAndWait
	str r0, [r5]
	b _0222CD3A
_0222C9AA:
	ldr r0, _0222CA2C ; =SEQ_SE_DP_SELECT
	bl PlaySE
	add r1, r4, #0
	add r0, r6, #0
	add r1, #0x18
	mov r2, #0x19
	bl MysteryGiftApp_PrintMessage
	add r0, r4, #0
	add r0, #0x58
	mov r1, #0
	bl MysteryGiftApp_EraseWindowIfInUse
	mov r0, #0x12
	str r0, [r5]
	b _0222CD3A
_0222C9CC:
	ldr r0, _0222CA30 ; =0x000015DC
	ldr r1, [r4, r0]
	cmp r1, #0
	beq _0222C9DA
	sub r1, r1, #1
	str r1, [r4, r0]
	b _0222CD3A
_0222C9DA:
	add r0, r4, #0
	mov r1, #0
	bl MysteryGiftApp_SetWaitingIconVisible
	ldr r0, _0222CA2C ; =SEQ_SE_DP_SELECT
	bl PlaySE
	add r1, r4, #0
	add r0, r6, #0
	add r1, #0x18
	mov r2, #0x18
	bl MysteryGiftApp_PrintMessage
	add r0, r4, #0
	add r0, #0x58
	mov r1, #0
	bl MysteryGiftApp_EraseWindowIfInUse
	mov r0, #0x12
	str r0, [r5]
	b _0222CD3A
_0222CA04:
	ldr r0, _0222CA14 ; =gSystem
	ldr r0, [r0, #0x48]
	cmp r0, #0
	bne _0222CA0E
_0222CA0C:
	b _0222CD3A
_0222CA0E:
	ldr r0, _0222CA2C ; =SEQ_SE_DP_SELECT
	b _0222CA34
	nop
_0222CA14: .word gSystem
_0222CA18: .word 0x00000434
_0222CA1C: .word ov74_0222A494
_0222CA20: .word 0x000029EC
_0222CA24: .word 0x0000043C
_0222CA28: .word SEQ_SE_DP_UG_020
_0222CA2C: .word SEQ_SE_DP_SELECT
_0222CA30: .word 0x000015DC
_0222CA34:
	bl PlaySE
	add r0, r6, #0
	mov r1, #0xc4
	mov r2, #0
	bl MysteryGiftApp_ShowMainMenu
	mov r0, #3
	str r0, [r5]
	b _0222CD3A
_0222CA48:
	ldr r0, _0222CD54 ; =0x000015DC
	ldr r1, [r4, r0]
	cmp r1, #0
	beq _0222CA56
	sub r1, r1, #1
	str r1, [r4, r0]
	b _0222CD3A
_0222CA56:
	add r0, r4, #0
	add r0, #0x84
	ldr r0, [r0]
	cmp r0, #0x1b
	beq _0222CA6C
	add r0, r4, #0
	mov r1, #0xea
	add r0, #0x8c
	lsl r1, r1, #2
	bl ov74_02235258
_0222CA6C:
	add r1, r4, #0
	ldr r0, [r4, #4]
	add r1, #0x8c
	bl MysteryGift_CheckCanReceive
	add r1, r4, #0
	add r1, #0x80
	str r0, [r1]
	add r0, r4, #0
	add r0, #0x80
	ldr r0, [r0]
	cmp r0, #1
	bne _0222CA94
	add r0, r4, #0
	mov r1, #0
	bl MysteryGiftApp_SetWaitingIconVisible
	mov r0, #0x31
	str r0, [r5]
	b _0222CD3A
_0222CA94:
	add r0, r4, #0
	add r0, #0x58
	bl WindowIsInUse
	cmp r0, #0
	bne _0222CAC2
	mov r0, #2
	str r0, [sp]
	mov r0, #0x1a
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	mov r2, #0
	mov r0, #0x71
	add r1, r4, #0
	str r2, [sp, #0xc]
	lsl r0, r0, #2
	str r0, [sp, #0x10]
	ldr r0, [r4]
	add r1, #0x58
	mov r3, #3
	bl AddWindowParameterized
_0222CAC2:
	add r1, r4, #0
	add r2, r4, #0
	add r0, r6, #0
	add r1, #0x58
	add r2, #0x8c
	bl MysteryGiftApp_PrintGiftTitle
	add r1, r4, #0
	add r0, r6, #0
	add r1, #0x18
	mov r2, #4
	bl MysteryGiftApp_PrintMessage
	add r0, r6, #0
	mov r1, #0
	bl MysteryGiftApp_ShowReceivePrompt
	mov r0, #0x14
	str r0, [r5]
	b _0222CD3A
_0222CAEA:
	ldr r2, _0222CD58 ; =ov74_0222A494
	add r0, r6, #0
	add r1, r5, #0
	bl MysteryGiftApp_HandleListMenuInput
	b _0222CD3A
_0222CAF6:
	mov r0, #0x16
	str r0, [r5]
	b _0222CD3A
_0222CAFC:
	add r0, r6, #0
	bl MysteryGiftApp_GetReceivedCard
	add r1, r0, #0
	ldr r0, [r4]
	bl ov74_02235DC4
	add r1, r4, #0
	add r0, r6, #0
	add r1, #0x18
	mov r2, #7
	bl MysteryGiftApp_PrintMessage
	add r0, r4, #0
	mov r1, #1
	bl MysteryGiftApp_SetSpriteAnim
	add r0, r4, #0
	mov r1, #1
	bl MysteryGiftApp_SetWaitingIconVisible
	ldr r0, _0222CD5C ; =0x0000043C
	mov r1, #0x3c
	str r1, [r4, r0]
	mov r0, #0x17
	str r0, [r5]
	b _0222CD3A
_0222CB32:
	ldr r0, _0222CD5C ; =0x0000043C
	ldr r1, [r4, r0]
	sub r1, r1, #1
	str r1, [r4, r0]
	ldr r0, [r4, r0]
	cmp r0, #0
	bne _0222CC18
	add r0, r6, #0
	bl MysteryGiftApp_SaveReceivedGift
	mov r0, #0x18
	str r0, [r5]
	b _0222CD3A
_0222CB4C:
	bl ov74_0223615C
	cmp r0, #4
	bne _0222CB58
	bl ov74_02236128
_0222CB58:
	bl ov74_0223615C
	cmp r0, #2
	bne _0222CB90
	ldr r0, _0222CD5C ; =0x0000043C
	mov r1, #1
	str r1, [r4, r0]
	add r0, r4, #0
	mov r1, #0
	bl MysteryGiftApp_SetWaitingIconVisible
	mov r1, #0
	add r0, r4, #0
	mvn r1, r1
	bl MysteryGiftApp_SetSpriteAnim
	ldr r0, _0222CD60 ; =SEQ_SE_DP_UG_020
	bl PlaySE
	add r1, r4, #0
	add r0, r6, #0
	add r1, #0x18
	mov r2, #8
	mov r3, #0x19
	bl MysteryGiftApp_PrintMessageAndWait
	str r0, [r5]
	b _0222CD3A
_0222CB90:
	bl ov74_0223615C
	cmp r0, #3
	bne _0222CC18
	mov r1, #0
	add r0, r4, #0
	mvn r1, r1
	bl MysteryGiftApp_SetSpriteAnim
	add r0, r4, #0
	add r0, #0xda
	ldrb r1, [r0]
	mov r0, #4
	mov r2, #0x1b
	bic r1, r0
	add r0, r4, #0
	add r0, #0xda
	strb r1, [r0]
	add r1, r4, #0
	add r0, r6, #0
	add r1, #0x18
	mov r3, #0x38
	bl MysteryGiftApp_PrintMessageAndWait
	str r0, [r5]
	b _0222CD3A
_0222CBC4:
	ldr r0, _0222CD5C ; =0x0000043C
	ldr r1, [r4, r0]
	sub r1, r1, #1
	str r1, [r4, r0]
	ldr r1, [r4, r0]
	cmp r1, #0
	bne _0222CC18
	mov r1, #1
	lsl r1, r1, #0x10
	str r1, [r4, r0]
	mov r0, #0x38
	str r0, [r5]
	b _0222CD3A
_0222CBDE:
	add r0, r4, #0
	add r0, #0x80
	ldr r0, [r0]
	cmp r0, #5
	beq _0222CBEE
	add r0, r4, #0
	bl MysteryGiftApp_StopReceiving
_0222CBEE:
	add r0, r6, #0
	bl MysteryGiftApp_ShowCannotReceiveMessage
	str r0, [r5]
	add r0, r4, #0
	add r0, #0x48
	mov r1, #0
	bl MysteryGiftApp_EraseWindowIfInUse
	b _0222CD3A
_0222CC02:
	mov r1, #0
	add r0, r6, #0
	add r2, r1, #0
	bl MysteryGiftApp_PrintMessageAndWait
	str r0, [r5]
	b _0222CD3A
_0222CC10:
	ldr r0, _0222CD64 ; =gSystem
	ldr r0, [r0, #0x48]
	cmp r0, #0
	bne _0222CC1A
_0222CC18:
	b _0222CD3A
_0222CC1A:
	add r0, r4, #0
	add r0, #0x58
	mov r1, #0
	bl MysteryGiftApp_EraseWindowIfInUse
	add r0, r6, #0
	bl MysteryGiftApp_CancelReceiveMenu
	str r0, [r5]
	b _0222CD3A
_0222CC2E:
	add r0, r6, #0
	mov r1, #1
	bl MysteryGiftApp_ShowReceivePrompt
	mov r0, #8
	str r0, [r5]
	add r0, r4, #0
	mov r1, #0
	add r0, #0x80
	str r1, [r0]
	b _0222CD3A
_0222CC44:
	add r0, r6, #0
	bl MysteryGiftApp_FreeGraphics
	mov r0, #0x11
	mov r1, #0
	lsl r0, r0, #6
	str r1, [r4, r0]
	add sp, #0x14
	mov r0, #1
	pop {r4, r5, r6, r7, pc}
_0222CC58:
	add r0, r6, #0
	bl MysteryGiftApp_FreeGraphics
	mov r1, #0x11
	mov r0, #1
	lsl r1, r1, #6
	add sp, #0x14
	str r0, [r4, r1]
	pop {r4, r5, r6, r7, pc}
_0222CC6A:
	add r0, r6, #0
	bl MysteryGiftApp_FreeGraphics
	mov r0, #0x11
	mov r1, #2
	lsl r0, r0, #6
	str r1, [r4, r0]
	add sp, #0x14
	mov r0, #1
	pop {r4, r5, r6, r7, pc}
_0222CC7E:
	ldr r0, _0222CD64 ; =gSystem
	ldr r0, [r0, #0x48]
	cmp r0, #0
	beq _0222CD3A
	ldr r0, _0222CD68 ; =SEQ_SE_DP_SELECT
	bl PlaySE
	add r0, r4, #0
	add r0, #0xda
	ldrb r0, [r0]
	lsl r0, r0, #0x1d
	lsr r0, r0, #0x1f
	cmp r0, #1
	bne _0222CCAA
	ldr r2, _0222CD6C ; =0x000015D8
	mov r0, #0
	ldr r2, [r4, r2]
	mov r1, #0x39
	mov r3, #2
	bl ov74_0223539C
	b _0222CD3A
_0222CCAA:
	mov r0, #1
	bl ov74_02235390
	ldr r2, _0222CD6C ; =0x000015D8
	mov r0, #0
	ldr r2, [r4, r2]
	mov r1, #0x3a
	mov r3, #2
	bl ov74_0223539C
	b _0222CD3A
_0222CCC0:
	ldr r0, [r4]
	bl MysteryGiftApp_LoadSubScreenBg
	mov r0, #1
	mov r1, #0
	bl GfGfx_EngineBTogglePlanes
	mov r0, #2
	mov r1, #1
	bl GfGfx_EngineBTogglePlanes
	add r0, r4, #0
	add r0, #0xda
	ldrb r0, [r0]
	lsl r0, r0, #0x1d
	lsr r0, r0, #0x1f
	cmp r0, #1
	bne _0222CD08
	add r0, r4, #0
	add r0, #0xdc
	ldrh r0, [r0]
	cmp r0, #3
	bne _0222CD08
	add r0, r4, #0
	add r0, #0xe0
	ldr r1, [r0]
	ldr r0, _0222CD70 ; =0x00000215
	cmp r1, r0
	bne _0222CD08
	add r1, r4, #0
	ldr r0, [r4]
	add r1, #0xdc
	mov r2, #0x54
	bl LockCapsuleCard_DrawCardOnBg
	b _0222CD14
_0222CD08:
	add r1, r4, #0
	ldr r0, [r4]
	add r1, #0xdc
	mov r2, #0x54
	bl WonderCardAlbum_DrawCardOnBg
_0222CD14:
	ldr r2, _0222CD6C ; =0x000015D8
	mov r0, #1
	ldr r2, [r4, r2]
	mov r1, #0x38
	mov r3, #2
	bl ov74_0223539C
	add r0, r4, #0
	add r0, #0xda
	ldrb r1, [r0]
	mov r0, #4
	bic r1, r0
	add r0, r4, #0
	add r0, #0xda
	strb r1, [r0]
	b _0222CD3A
_0222CD34:
	mov r0, #0
	bl OS_ResetSystem
_0222CD3A:
	ldr r0, _0222CD74 ; =0x000029E8
	ldr r1, [r4, r0]
	cmp r1, #0
	beq _0222CD46
	add r0, r4, #0
	blx r1
_0222CD46:
	ldr r0, _0222CD78 ; =0x000029F4
	ldr r0, [r4, r0]
	bl ov74_022358C8
	mov r0, #0
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0222CD54: .word 0x000015DC
_0222CD58: .word ov74_0222A494
_0222CD5C: .word 0x0000043C
_0222CD60: .word SEQ_SE_DP_UG_020
_0222CD64: .word gSystem
_0222CD68: .word SEQ_SE_DP_SELECT
_0222CD6C: .word 0x000015D8
_0222CD70: .word 0x00000215
_0222CD74: .word 0x000029E8
_0222CD78: .word 0x000029F4
	thumb_func_end MysteryGiftApp_Main
