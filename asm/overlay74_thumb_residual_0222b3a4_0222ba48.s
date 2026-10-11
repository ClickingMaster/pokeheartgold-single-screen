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
	.extern MainMenuUtil_SetCryptoHeap
	.extern MainMenuUtil_GetCartridgeGiftSize
	.extern MainMenuUtil_ReadCartridgeGift
	.extern MainMenuUtil_InitWork
	.extern MainMenuUtil_CreateAppData
	.extern MainMenuUtil_InitBg
	.extern MainMenuUtil_SetFadeWhite
	.extern MainMenuUtil_StartFade
	.extern MainMenuUtil_WaitFade
	.extern MainMenuTextPrinter_Init
	.extern MainMenuTextPrinter_SetSize
	.extern MainMenuTextPrinter_SetFrameAndFont
	.extern MainMenuTextPrinter_SetTextPos
	.extern MainMenuTextPrinter_AddWindowAndPrint
	.extern MainMenuTextPrinter_GetTileCount
	.extern MainMenuUtil_InitObjTransfer
	.extern MainMenuUtil_SpritesActive
	.extern MainMenuUtil_InitSprites
	.extern MainMenuUtil_GetSpriteList
	.extern MainMenuUtil_LoadSpriteResources
	.extern MainMenuUtil_UpdateSprites
	.extern MainMenuUtil_UpdateGiftSprite
	.extern MainMenuUtil_CreateSprite
	.extern MainMenuUtil_FreeSprites
	.extern ov74_02235A68
	.extern MainMenuUtil_ShowCardSubScreen
	.extern MainMenuUtil_EncryptWonderCard
	.extern MainMenuUtil_DecryptWonderCard
	.extern MainMenuUtil_EnableCartridgePullOutCheck
	.extern MainMenuUtil_StartSave
	.extern MainMenuUtil_RunSave
	.extern MainMenuUtil_ResumeSave
	.extern MainMenuUtil_CancelSave
	.extern MainMenuUtil_GetSaveState
	.extern MysteryGiftBeacon_Stop
	.extern MysteryGiftBeacon_Init
	.extern MysteryGiftBeacon_DecryptGift
	.extern MysteryGiftBeacon_GetWorkSize
	.extern MysteryGiftBeacon_GetTitle
	.extern MysteryGiftWfc_Run
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



	thumb_func_start ov74_0222B3A4
ov74_0222B3A4: ; 0x0222B3A4
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0xb8
	add r5, r0, #0
	str r0, [sp]
	add r0, #0x88
	ldr r3, _0222B5E8 ; =sMysteryGiftSparkleDelays
	mov r4, #0
	add r7, sp, #0x64
	str r0, [sp]
	add r2, sp, #0x10
	mov r6, #0xa
_0222B3BA:
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	sub r6, r6, #1
	bne _0222B3BA
	ldr r0, [r3]
	str r0, [r2]
_0222B3C6:
	mov r2, #2
	add r0, r5, #0
	mov r1, #6
	lsl r2, r2, #0x12
	add r0, #0x98
	str r2, [r0]
	add r0, r5, #0
	lsl r1, r1, #0x10
	add r0, #0x9c
	str r1, [r0]
	add r0, r5, #0
	lsr r1, r2, #5
	add r0, #0xa4
	str r1, [r0]
	mov r1, #5
	add r0, r5, #0
	lsl r1, r1, #0xc
	add r0, #0xa8
	str r1, [r0]
	lsr r2, r4, #0x1f
	lsl r1, r4, #0x1e
	sub r1, r1, r2
	mov r0, #0x1e
	ror r1, r0
	add r0, r5, #0
	add r1, r2, r1
	add r0, #0xb4
	str r1, [r0]
	add r0, r5, #0
	mov r1, #0
	add r0, #0xbc
	str r1, [r0]
	add r0, r5, #0
	add r0, #0x8c
	str r1, [r0]
	add r0, r4, #0
	mov r1, #0xa
	bl _s32_div_f
	add r0, r0, #4
	cmp r0, #0
	ble _0222B42C
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _0222B43A
_0222B42C:
	lsl r0, r0, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_0222B43A:
	bl _ffix
	add r1, r5, #0
	add r1, #0xa4
	str r0, [r1]
	cmp r4, #0x32
	ble _0222B462
	bl LCRandom
	mov r1, #5
	bl _s32_div_f
	mov r0, #0x41
	lsl r0, r0, #2
	add r0, r4, r0
	add r1, r0, r1
	add r0, r5, #0
	add r0, #0xb8
	str r1, [r0]
	b _0222B4EC
_0222B462:
	cmp r4, #0x28
	ble _0222B47E
	bl LCRandom
	mov r1, #5
	bl _s32_div_f
	add r0, r4, #0
	add r0, #0xfa
	add r1, r0, r1
	add r0, r5, #0
	add r0, #0xb8
	str r1, [r0]
	b _0222B4EC
_0222B47E:
	cmp r4, #0x1e
	ble _0222B49A
	bl LCRandom
	mov r1, #5
	bl _s32_div_f
	add r0, r4, #0
	add r0, #0xf0
	add r1, r0, r1
	add r0, r5, #0
	add r0, #0xb8
	str r1, [r0]
	b _0222B4EC
_0222B49A:
	cmp r4, #0x19
	ble _0222B4B6
	bl LCRandom
	mov r1, #5
	bl _s32_div_f
	add r0, r4, #0
	add r0, #0xe6
	add r1, r0, r1
	add r0, r5, #0
	add r0, #0xb8
	str r1, [r0]
	b _0222B4EC
_0222B4B6:
	cmp r4, #0x14
	ble _0222B4D2
	bl LCRandom
	mov r1, #5
	bl _s32_div_f
	add r0, r4, #0
	add r0, #0xdc
	add r1, r0, r1
	add r0, r5, #0
	add r0, #0xb8
	str r1, [r0]
	b _0222B4EC
_0222B4D2:
	add r6, sp, #0x10
	add r3, sp, #0x64
	mov r2, #0xa
_0222B4D8:
	ldmia r6!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, r2, #1
	bne _0222B4D8
	ldr r0, [r6]
	str r0, [r3]
	add r0, r5, #0
	ldr r1, [r7]
	add r0, #0xb8
	str r1, [r0]
_0222B4EC:
	add r0, r5, #0
	add r0, #0xb8
	ldr r0, [r0]
	add r1, r0, #1
	lsr r0, r1, #0x1f
	add r0, r1, r0
	asr r0, r0, #1
	bl _dflt
	add r2, r0, #0
	add r3, r1, #0
	ldr r0, _0222B5EC ; =0x66666666
	ldr r1, _0222B5F0 ; =0x3FE66666
	bl _dmul
	bl _dfix
	add r1, r5, #0
	add r1, #0xb8
	str r0, [r1]
	add r0, r5, #0
	add r0, #0xb8
	ldr r0, [r0]
	add r1, r0, #1
	add r0, r5, #0
	add r0, #0xb8
	str r1, [r0]
	add r0, r5, #0
	mov r1, #0xf
	add r0, #0xc0
	str r1, [r0]
	add r0, r5, #0
	add r0, #0xc0
	ldr r1, [r0]
	add r0, r5, #0
	add r0, #0xc8
	str r1, [r0]
	add r0, r5, #0
	add r0, #0xc0
	ldr r1, [r0]
	add r0, r5, #0
	add r0, #0xcc
	str r1, [r0]
	add r0, r5, #0
	add r0, #0x94
	ldr r0, [r0]
	bl Sprite_GetMatrixPtr
	add r3, r0, #0
	ldmia r3!, {r0, r1}
	add r2, sp, #4
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	str r0, [r2]
	bl LCRandom
	mov r1, #0xe0
	bl _s32_div_f
	add r1, #0x10
	cmp r1, #0
	ble _0222B586
	bl LCRandom
	mov r1, #0xe0
	bl _s32_div_f
	add r1, #0x10
	lsl r0, r1, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _0222B5A0
_0222B586:
	bl LCRandom
	mov r1, #0xe0
	bl _s32_div_f
	add r1, #0x10
	lsl r0, r1, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_0222B5A0:
	bl _ffix
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	add r0, r5, #0
	add r0, #0x94
	ldr r0, [r0]
	add r1, sp, #4
	bl Sprite_SetMatrix
	add r0, r5, #0
	add r0, #0x94
	ldr r0, [r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
	ldr r0, _0222B5F4 ; =MysteryGiftAnim_UpdateParticle
	ldr r1, [sp]
	mov r2, #6
	bl SysTask_CreateOnMainQueue
	add r1, r5, #0
	add r1, #0xd0
	str r0, [r1]
	ldr r0, [sp]
	add r4, r4, #1
	add r0, #0x4c
	add r5, #0x4c
	add r7, r7, #4
	str r0, [sp]
	cmp r4, #0x50
	bge _0222B5E4
	b _0222B3C6
_0222B5E4:
	add sp, #0xb8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0222B5E8: .word sMysteryGiftSparkleDelays
_0222B5EC: .word 0x66666666
_0222B5F0: .word 0x3FE66666
_0222B5F4: .word MysteryGiftAnim_UpdateParticle
	thumb_func_end ov74_0222B3A4


	thumb_func_start ov74_0222B5F8
ov74_0222B5F8: ; 0x0222B5F8
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r5, r0, #0
	add r7, r5, #0
	mov r4, #0
	add r7, #0x88
_0222B604:
	add r0, r5, #0
	add r0, #0x94
	ldr r0, [r0]
	cmp r0, #0
	bne _0222B610
	b _0222B74C
_0222B610:
	add r1, r5, #0
	mov r0, #2
	add r1, #0x98
	lsl r0, r0, #0x12
	str r0, [r1]
	add r1, r5, #0
	mov r0, #6
	add r1, #0x9c
	lsl r0, r0, #0x10
	str r0, [r1]
	add r1, r5, #0
	add r1, #0xa4
	lsr r0, r0, #3
	str r0, [r1]
	add r1, r5, #0
	mov r0, #1
	add r1, #0xa8
	lsl r0, r0, #0xc
	str r0, [r1]
	add r1, r5, #0
	add r1, #0xb4
	mov r0, #0xff
	str r0, [r1]
	add r1, r5, #0
	add r1, #0xbc
	mov r0, #0
	str r0, [r1]
	add r1, r5, #0
	add r1, #0x8c
	str r0, [r1]
	add r1, r5, #0
	add r1, #0x90
	str r0, [r1]
	cmp r4, #0x1e
	ble _0222B66A
	bl LCRandom
	mov r1, #0xa
	bl _s32_div_f
	add r0, r5, #0
	add r1, #0xf
	add r0, #0xb8
	str r1, [r0]
	b _0222B6AC
_0222B66A:
	cmp r4, #0x14
	ble _0222B682
	bl LCRandom
	mov r1, #0xa
	bl _s32_div_f
	add r0, r5, #0
	add r1, #0xa
	add r0, #0xb8
	str r1, [r0]
	b _0222B6AC
_0222B682:
	cmp r4, #0xa
	ble _0222B69A
	bl LCRandom
	mov r1, #5
	bl _s32_div_f
	add r0, r5, #0
	add r1, #0xa
	add r0, #0xb8
	str r1, [r0]
	b _0222B6AC
_0222B69A:
	bl LCRandom
	mov r1, #5
	bl _s32_div_f
	add r0, r5, #0
	add r1, r1, #5
	add r0, #0xb8
	str r1, [r0]
_0222B6AC:
	add r1, r5, #0
	add r1, #0xb8
	mov r0, #0
	str r0, [r1]
	add r0, r5, #0
	add r0, #0xc0
	ldr r1, [r0]
	add r0, r5, #0
	add r0, #0xc8
	str r1, [r0]
	add r0, r5, #0
	add r0, #0xc0
	ldr r1, [r0]
	add r0, r5, #0
	add r0, #0xcc
	str r1, [r0]
	bl LCRandom
	mov r1, #0x5a
	lsl r1, r1, #2
	bl _s32_div_f
	str r1, [sp]
	bl LCRandom
	lsr r2, r0, #0x1f
	lsl r1, r0, #0x1b
	sub r1, r1, r2
	mov r0, #0x1b
	ror r1, r0
	ldr r0, [sp]
	add r6, r2, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	add r6, #0x40
	bl GF_SinDeg
	add r1, r5, #0
	add r1, #0x98
	ldr r1, [r1]
	mul r0, r6
	add r0, r1, r0
	str r0, [sp, #4]
	ldr r0, [sp]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl GF_CosDeg
	add r1, r5, #0
	add r1, #0x9c
	ldr r1, [r1]
	mul r0, r6
	add r0, r1, r0
	str r0, [sp, #8]
	add r0, r5, #0
	add r0, #0x94
	ldr r0, [r0]
	add r1, sp, #4
	bl Sprite_SetMatrix
	add r0, r5, #0
	add r0, #0x94
	ldr r0, [r0]
	mov r1, #1
	bl Sprite_TryChangeAnimSeq
	add r0, r5, #0
	add r0, #0x94
	ldr r0, [r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
	ldr r0, _0222B75C ; =MysteryGiftAnim_UpdateParticle
	add r1, r7, #0
	mov r2, #6
	bl SysTask_CreateOnMainQueue
	add r1, r5, #0
	add r1, #0xd0
	str r0, [r1]
_0222B74C:
	add r4, r4, #1
	add r5, #0x4c
	add r7, #0x4c
	cmp r4, #0x50
	bge _0222B758
	b _0222B604
_0222B758:
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0222B75C: .word MysteryGiftAnim_UpdateParticle
	thumb_func_end ov74_0222B5F8


	thumb_func_start ov74_0222B760
ov74_0222B760: ; 0x0222B760
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r5, r0, #0
	ldr r0, _0222B928 ; =0x00001848
	mov r4, #0
	add r6, r5, r0
_0222B76C:
	mov r1, #2
	ldr r0, _0222B92C ; =0x00001858
	lsl r1, r1, #0x12
	str r1, [r5, r0]
	mov r1, #6
	lsl r1, r1, #0x12
	add r0, r0, #4
	str r1, [r5, r0]
	mov r1, #1
	ldr r0, _0222B930 ; =0x00001864
	lsl r1, r1, #0xe
	str r1, [r5, r0]
	mov r1, #5
	lsl r1, r1, #0xc
	add r0, r0, #4
	str r1, [r5, r0]
	lsr r2, r4, #0x1f
	lsl r1, r4, #0x1f
	sub r1, r1, r2
	mov r0, #0x1f
	ror r1, r0
	add r0, r2, r1
	add r1, r0, #4
	ldr r0, _0222B934 ; =0x00001874
	str r1, [r5, r0]
	mov r1, #0
	add r0, #8
	str r1, [r5, r0]
	ldr r0, _0222B938 ; =0x0000184C
	str r1, [r5, r0]
	bl LCRandom
	lsr r2, r0, #0x1f
	lsl r1, r0, #0x1e
	sub r1, r1, r2
	mov r0, #0x1e
	ror r1, r0
	add r0, r2, r1
	add r1, r0, #2
	ldr r0, _0222B93C ; =0x00001884
	str r1, [r5, r0]
	add r0, r4, #0
	mov r1, #0x1e
	bl _s32_div_f
	add r0, r0, #4
	cmp r0, #0
	ble _0222B7DE
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _0222B7EC
_0222B7DE:
	lsl r0, r0, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_0222B7EC:
	bl _ffix
	ldr r1, _0222B930 ; =0x00001864
	cmp r4, #0x3c
	str r0, [r5, r1]
	ble _0222B812
	mov r0, #0x62
	mov r1, #5
	lsl r0, r0, #6
	str r1, [r5, r0]
	bl LCRandom
	mov r1, #0xa
	bl _s32_div_f
	ldr r0, _0222B940 ; =0x00001878
	add r1, #0x46
	str r1, [r5, r0]
	b _0222B8C0
_0222B812:
	cmp r4, #0x32
	ble _0222B830
	mov r0, #0x62
	mov r1, #5
	lsl r0, r0, #6
	str r1, [r5, r0]
	bl LCRandom
	mov r1, #0xa
	bl _s32_div_f
	ldr r0, _0222B940 ; =0x00001878
	add r1, #0x3c
	str r1, [r5, r0]
	b _0222B8C0
_0222B830:
	cmp r4, #0x28
	ble _0222B84E
	mov r0, #0x62
	mov r1, #5
	lsl r0, r0, #6
	str r1, [r5, r0]
	bl LCRandom
	mov r1, #5
	bl _s32_div_f
	ldr r0, _0222B940 ; =0x00001878
	add r1, #0x32
	str r1, [r5, r0]
	b _0222B8C0
_0222B84E:
	cmp r4, #0x1e
	ble _0222B86C
	mov r0, #0x62
	mov r1, #5
	lsl r0, r0, #6
	str r1, [r5, r0]
	bl LCRandom
	mov r1, #0xa
	bl _s32_div_f
	ldr r0, _0222B940 ; =0x00001878
	add r1, #0x28
	str r1, [r5, r0]
	b _0222B8C0
_0222B86C:
	cmp r4, #0x14
	ble _0222B88A
	mov r0, #0x62
	mov r1, #5
	lsl r0, r0, #6
	str r1, [r5, r0]
	bl LCRandom
	mov r1, #0xa
	bl _s32_div_f
	ldr r0, _0222B940 ; =0x00001878
	add r1, #0x1e
	str r1, [r5, r0]
	b _0222B8C0
_0222B88A:
	cmp r4, #0xa
	ble _0222B8A8
	mov r0, #0x62
	mov r1, #5
	lsl r0, r0, #6
	str r1, [r5, r0]
	bl LCRandom
	mov r1, #5
	bl _s32_div_f
	ldr r0, _0222B940 ; =0x00001878
	add r1, #0x14
	str r1, [r5, r0]
	b _0222B8C0
_0222B8A8:
	mov r0, #0x62
	mov r1, #5
	lsl r0, r0, #6
	str r1, [r5, r0]
	bl LCRandom
	mov r1, #5
	bl _s32_div_f
	ldr r0, _0222B940 ; =0x00001878
	add r1, #0xa
	str r1, [r5, r0]
_0222B8C0:
	mov r0, #0x62
	lsl r0, r0, #6
	ldr r1, [r5, r0]
	add r0, #8
	str r1, [r5, r0]
	mov r0, #0x62
	lsl r0, r0, #6
	ldr r1, [r5, r0]
	add r0, #0xc
	str r1, [r5, r0]
	ldr r0, _0222B944 ; =0x00001854
	ldr r0, [r5, r0]
	bl Sprite_GetMatrixPtr
	add r3, r0, #0
	add r2, sp, #0
	ldmia r3!, {r0, r1}
	add r7, r2, #0
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	add r1, r7, #0
	str r0, [r2]
	mov r0, #2
	lsl r0, r0, #0x12
	str r0, [sp]
	mov r0, #6
	lsl r0, r0, #0x12
	str r0, [sp, #4]
	ldr r0, _0222B944 ; =0x00001854
	ldr r0, [r5, r0]
	bl Sprite_SetMatrix
	ldr r0, _0222B944 ; =0x00001854
	mov r1, #1
	ldr r0, [r5, r0]
	bl Sprite_SetDrawFlag
	ldr r0, _0222B948 ; =MysteryGiftAnim_UpdateParticle
	add r1, r6, #0
	mov r2, #6
	bl SysTask_CreateOnMainQueue
	ldr r1, _0222B94C ; =0x00001890
	add r4, r4, #1
	str r0, [r5, r1]
	add r5, #0x4c
	add r6, #0x4c
	cmp r4, #0x50
	bge _0222B924
	b _0222B76C
_0222B924:
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0222B928: .word 0x00001848
_0222B92C: .word 0x00001858
_0222B930: .word 0x00001864
_0222B934: .word 0x00001874
_0222B938: .word 0x0000184C
_0222B93C: .word 0x00001884
_0222B940: .word 0x00001878
_0222B944: .word 0x00001854
_0222B948: .word MysteryGiftAnim_UpdateParticle
_0222B94C: .word 0x00001890
	thumb_func_end ov74_0222B760


	thumb_func_start MysteryGiftAnim_StepTowards
MysteryGiftAnim_StepTowards: ; 0x0222B950
	push {r4, r5, r6, lr}
	sub sp, #0x18
	add r5, r1, #0
	mov r1, #0
	add r4, r2, #0
	add r6, r3, #0
	str r1, [sp]
	str r1, [sp, #4]
	bl _fflt
	add r1, r4, #0
	bl _fsub
	str r0, [sp, #8]
	add r0, r5, #0
	bl _fflt
	add r1, r6, #0
	bl _fsub
	str r0, [sp, #0xc]
	mov r0, #0
	str r0, [sp, #0x10]
	str r0, [sp, #0x14]
	ldr r0, [sp, #8]
	add r1, r0, #0
	bl _fmul
	add r4, r0, #0
	ldr r0, [sp, #0xc]
	add r1, r0, #0
	bl _fmul
	add r1, r0, #0
	add r0, r4, #0
	bl _fadd
	mov r1, #0
	add r4, r0, #0
	bl _fgr
	ldr r0, _0222BA44 ; =0x45800000
	bls _0222B9B8
	add r1, r4, #0
	bl _fmul
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _0222B9C6
_0222B9B8:
	add r1, r4, #0
	bl _fmul
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_0222B9C6:
	bl _ffix
	bl FX_Sqrt
	bl _fflt
	ldr r1, _0222BA44 ; =0x45800000
	bl _fdiv
	ldr r1, [sp, #0x30]
	add r4, r0, #0
	bl _fls
	blo _0222B9FE
	add r1, sp, #0x18
	mov r0, #0x1c
	ldrsh r0, [r1, r0]
	bl _fflt
	add r1, r4, #0
	bl _fgr
	bhi _0222B9FE
	mov r0, #0
	add r1, r4, #0
	bl _feq
	bne _0222BA04
_0222B9FE:
	add sp, #0x18
	mov r0, #0
	pop {r4, r5, r6, pc}
_0222BA04:
	ldr r0, [sp, #8]
	ldr r1, [sp, #0x30]
	bl _fmul
	add r1, r4, #0
	bl _fdiv
	str r0, [sp, #0x10]
	ldr r0, [sp, #0xc]
	ldr r1, [sp, #0x30]
	bl _fmul
	add r1, r4, #0
	bl _fdiv
	str r0, [sp, #0x14]
	ldr r0, [sp, #0x10]
	ldr r1, [sp]
	bl _fadd
	ldr r1, [sp, #0x28]
	str r0, [r1]
	ldr r0, [sp, #0x14]
	ldr r1, [sp, #4]
	bl _fadd
	ldr r1, [sp, #0x2c]
	str r0, [r1]
	mov r0, #1
	add sp, #0x18
	pop {r4, r5, r6, pc}
	nop
_0222BA44: .word 0x45800000
	thumb_func_end MysteryGiftAnim_StepTowards


	thumb_func_start MysteryGiftAnim_UpdateParticle
MysteryGiftAnim_UpdateParticle: ; 0x0222BA48
	push {r4, r5, r6, r7, lr}
	sub sp, #0x24
	add r5, r1, #0
	ldr r0, [r5, #0x30]
	cmp r0, #0
	ldr r0, [r5, #0xc]
	beq _0222BA66
	mov r1, #0
	bl Sprite_SetAnimationFrame
	ldr r0, [r5, #0x30]
	add sp, #0x24
	sub r0, r0, #1
	str r0, [r5, #0x30]
	pop {r4, r5, r6, r7, pc}
_0222BA66:
	mov r1, #1
	bl Sprite_SetDrawFlag
	ldr r0, [r5, #0xc]
	bl Sprite_GetMatrixPtr
	add r3, r0, #0
	ldmia r3!, {r0, r1}
	add r2, sp, #0x18
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	str r0, [r2]
	ldr r0, [sp, #0x18]
	bl _fflt
	ldr r1, _0222BD90 ; =0x45800000
	bl _fdiv
	add r6, r0, #0
	ldr r0, [sp, #0x1c]
	bl _fflt
	ldr r1, _0222BD90 ; =0x45800000
	bl _fdiv
	add r7, r0, #0
	ldr r0, [r5, #0x1c]
	bl _fflt
	ldr r1, _0222BD90 ; =0x45800000
	bl _fdiv
	add r4, r0, #0
	ldr r0, [r5, #0x20]
	bl _fflt
	ldr r1, _0222BD90 ; =0x45800000
	bl _fdiv
	bl _ffix
	add r1, sp, #0x14
	str r1, [sp]
	add r1, sp, #0x10
	str r1, [sp, #4]
	str r4, [sp, #8]
	str r0, [sp, #0xc]
	ldr r0, [r5, #0x10]
	ldr r1, [r5, #0x14]
	lsl r0, r0, #4
	lsl r1, r1, #4
	asr r0, r0, #0x10
	asr r1, r1, #0x10
	add r2, r6, #0
	add r3, r7, #0
	bl MysteryGiftAnim_StepTowards
	cmp r0, #0
	beq _0222BB5A
	ldr r0, [r5, #0x24]
	cmp r0, #0
	bne _0222BB5A
	ldr r0, [sp, #0x14]
	mov r1, #0
	bl _fgr
	ldr r0, _0222BD90 ; =0x45800000
	bls _0222BB00
	ldr r1, [sp, #0x14]
	bl _fmul
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _0222BB0E
_0222BB00:
	ldr r1, [sp, #0x14]
	bl _fmul
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_0222BB0E:
	bl _ffix
	ldr r1, [sp, #0x18]
	add r0, r1, r0
	str r0, [sp, #0x18]
	ldr r0, [sp, #0x10]
	mov r1, #0
	bl _fgr
	ldr r0, _0222BD90 ; =0x45800000
	bls _0222BB36
	ldr r1, [sp, #0x10]
	bl _fmul
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _0222BB44
_0222BB36:
	ldr r1, [sp, #0x10]
	bl _fmul
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_0222BB44:
	bl _ffix
	ldr r1, [sp, #0x1c]
	add r0, r1, r0
	str r0, [sp, #0x1c]
	ldr r0, [r5, #0xc]
	add r1, sp, #0x18
	bl Sprite_SetMatrix
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
_0222BB5A:
	ldr r0, [r5, #0x24]
	cmp r0, #0
	beq _0222BB68
	cmp r0, #1
	beq _0222BBDE
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
_0222BB68:
	ldr r1, [r5, #0x10]
	ldr r0, [sp, #0x18]
	sub r0, r1, r0
	cmp r0, #0
	ble _0222BB84
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _0222BB92
_0222BB84:
	lsl r0, r0, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_0222BB92:
	add r4, r0, #0
	ldr r1, [r5, #0x14]
	ldr r0, [sp, #0x1c]
	sub r0, r1, r0
	cmp r0, #0
	ble _0222BBB0
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _0222BBBE
_0222BBB0:
	lsl r0, r0, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_0222BBBE:
	bl _ffix
	add r6, r0, #0
	add r0, r4, #0
	bl _ffix
	add r1, r0, #0
	add r0, r6, #0
	bl FX_Atan2Idx
	str r0, [r5, #0x28]
	ldr r0, [r5, #0x24]
	add sp, #0x24
	add r0, r0, #1
	str r0, [r5, #0x24]
	pop {r4, r5, r6, r7, pc}
_0222BBDE:
	ldr r0, [r5, #4]
	cmp r0, #0x14
	bge _0222BBEA
	add r0, r0, #1
	str r0, [r5, #4]
	b _0222BBEE
_0222BBEA:
	mov r0, #1
	str r0, [r5, #8]
_0222BBEE:
	ldr r0, [r5, #0x28]
	add r0, #8
	str r0, [r5, #0x28]
	ldr r0, [r5, #0x2c]
	cmp r0, #0
	bne _0222BC44
	ldr r0, [r5, #0x34]
	mov r1, #0x5a
	add r0, #8
	lsl r1, r1, #2
	str r0, [r5, #0x34]
	bl _s32_div_f
	lsl r0, r1, #0x10
	str r1, [r5, #0x34]
	lsr r0, r0, #0x10
	bl GF_CosDeg
	ldr r1, [r5, #0x38]
	mul r0, r1
	asr r0, r0, #0xc
	str r0, [r5, #0x40]
	ldr r0, [r5, #0x28]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl GF_SinDeg
	ldr r1, [r5, #0x40]
	ldr r2, [r5, #0x10]
	mul r0, r1
	add r0, r2, r0
	str r0, [sp, #0x18]
	ldr r0, [r5, #0x28]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl GF_CosDeg
	ldr r1, [r5, #0x44]
	ldr r2, [r5, #0x14]
	mul r0, r1
	add r0, r2, r0
	str r0, [sp, #0x1c]
	b _0222BEBC
_0222BC44:
	cmp r0, #1
	bne _0222BC92
	ldr r0, [r5, #0x34]
	mov r1, #0x5a
	add r0, #8
	lsl r1, r1, #2
	str r0, [r5, #0x34]
	bl _s32_div_f
	lsl r0, r1, #0x10
	str r1, [r5, #0x34]
	lsr r0, r0, #0x10
	bl GF_SinDeg
	ldr r1, [r5, #0x38]
	mul r0, r1
	asr r0, r0, #0xc
	str r0, [r5, #0x44]
	ldr r0, [r5, #0x28]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl GF_SinDeg
	ldr r1, [r5, #0x40]
	ldr r2, [r5, #0x10]
	mul r0, r1
	add r0, r2, r0
	str r0, [sp, #0x18]
	ldr r0, [r5, #0x28]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl GF_CosDeg
	ldr r1, [r5, #0x44]
	ldr r2, [r5, #0x14]
	mul r0, r1
	add r0, r2, r0
	str r0, [sp, #0x1c]
	b _0222BEBC
_0222BC92:
	cmp r0, #2
	bne _0222BCF0
	ldr r0, [r5, #0x34]
	mov r1, #0x5a
	add r0, #8
	lsl r1, r1, #2
	str r0, [r5, #0x34]
	bl _s32_div_f
	lsl r0, r1, #0x10
	str r1, [r5, #0x34]
	lsr r0, r0, #0x10
	bl GF_SinDeg
	ldr r1, [r5, #0x38]
	mul r0, r1
	asr r0, r0, #0xc
	str r0, [r5, #0x44]
	ldr r0, [r5, #0x28]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl GF_SinDeg
	ldr r1, [r5, #0x40]
	ldr r3, [r5, #0x10]
	add r2, r1, #0
	mul r2, r0
	mov r0, #0
	mvn r0, r0
	mul r0, r2
	add r0, r3, r0
	str r0, [sp, #0x18]
	ldr r0, [r5, #0x28]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl GF_CosDeg
	ldr r1, [r5, #0x44]
	ldr r3, [r5, #0x14]
	add r2, r1, #0
	mul r2, r0
	mov r0, #0
	mvn r0, r0
	mul r0, r2
	add r0, r3, r0
	str r0, [sp, #0x1c]
	b _0222BEBC
_0222BCF0:
	cmp r0, #3
	bne _0222BD4E
	ldr r0, [r5, #0x34]
	mov r1, #0x5a
	add r0, #8
	lsl r1, r1, #2
	str r0, [r5, #0x34]
	bl _s32_div_f
	lsl r0, r1, #0x10
	str r1, [r5, #0x34]
	lsr r0, r0, #0x10
	bl GF_CosDeg
	ldr r1, [r5, #0x38]
	mul r0, r1
	asr r0, r0, #0xc
	str r0, [r5, #0x40]
	ldr r0, [r5, #0x28]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl GF_SinDeg
	ldr r1, [r5, #0x40]
	ldr r3, [r5, #0x10]
	add r2, r1, #0
	mul r2, r0
	mov r0, #0
	mvn r0, r0
	mul r0, r2
	add r0, r3, r0
	str r0, [sp, #0x18]
	ldr r0, [r5, #0x28]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl GF_CosDeg
	ldr r1, [r5, #0x44]
	ldr r3, [r5, #0x14]
	add r2, r1, #0
	mul r2, r0
	mov r0, #0
	mvn r0, r0
	mul r0, r2
	add r0, r3, r0
	str r0, [sp, #0x1c]
	b _0222BEBC
_0222BD4E:
	cmp r0, #4
	bne _0222BD94
	ldr r1, [r5, #0x38]
	cmp r1, #0x60
	bge _0222BD5E
	ldr r0, [r5, #0x3c]
	add r0, r1, r0
	str r0, [r5, #0x38]
_0222BD5E:
	ldr r0, [r5, #0x38]
	str r0, [r5, #0x40]
	ldr r0, [r5, #0x38]
	str r0, [r5, #0x44]
	ldr r0, [r5, #0x28]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl GF_SinDeg
	ldr r1, [r5, #0x40]
	ldr r2, [r5, #0x10]
	mul r0, r1
	add r0, r2, r0
	str r0, [sp, #0x18]
	ldr r0, [r5, #0x28]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl GF_CosDeg
	ldr r1, [r5, #0x44]
	ldr r2, [r5, #0x14]
	mul r0, r1
	add r0, r2, r0
	str r0, [sp, #0x1c]
	b _0222BEBC
	.balign 4, 0
_0222BD90: .word 0x45800000
_0222BD94:
	cmp r0, #5
	bne _0222BDE6
	ldr r1, [r5, #0x38]
	cmp r1, #0x60
	bge _0222BDA4
	ldr r0, [r5, #0x3c]
	add r0, r1, r0
	str r0, [r5, #0x38]
_0222BDA4:
	ldr r0, [r5, #0x38]
	str r0, [r5, #0x40]
	ldr r0, [r5, #0x38]
	str r0, [r5, #0x44]
	ldr r0, [r5, #0x28]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl GF_SinDeg
	ldr r1, [r5, #0x40]
	ldr r3, [r5, #0x10]
	add r2, r1, #0
	mul r2, r0
	mov r0, #0
	mvn r0, r0
	mul r0, r2
	add r0, r3, r0
	str r0, [sp, #0x18]
	ldr r0, [r5, #0x28]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl GF_CosDeg
	ldr r1, [r5, #0x44]
	ldr r3, [r5, #0x14]
	add r2, r1, #0
	mul r2, r0
	mov r0, #0
	mvn r0, r0
	mul r0, r2
	add r0, r3, r0
	str r0, [sp, #0x1c]
	b _0222BEBC
_0222BDE6:
	mov r0, #0xa
	str r0, [r5, #4]
	mov r0, #2
	ldr r1, [r5, #0x1c]
	lsl r0, r0, #0xa
	add r0, r1, r0
	str r0, [r5, #0x1c]
	ldr r0, [sp, #0x18]
	bl _fflt
	ldr r1, _0222BED4 ; =0x45800000
	bl _fdiv
	add r4, r0, #0
	ldr r0, [sp, #0x1c]
	bl _fflt
	ldr r1, _0222BED4 ; =0x45800000
	bl _fdiv
	add r6, r0, #0
	ldr r0, [r5, #0x1c]
	bl _fflt
	ldr r1, _0222BED4 ; =0x45800000
	bl _fdiv
	add r1, sp, #0x14
	str r1, [sp]
	add r1, sp, #0x10
	str r1, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	ldr r0, [r5, #0x10]
	ldr r1, [r5, #0x14]
	lsl r0, r0, #4
	lsl r1, r1, #4
	asr r0, r0, #0x10
	asr r1, r1, #0x10
	add r2, r4, #0
	add r3, r6, #0
	bl MysteryGiftAnim_StepTowards
	cmp r0, #0
	beq _0222BEB8
	ldr r0, [sp, #0x14]
	mov r1, #0
	bl _fgr
	ldr r0, _0222BED4 ; =0x45800000
	bls _0222BE60
	ldr r1, [sp, #0x14]
	bl _fmul
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _0222BE6E
_0222BE60:
	ldr r1, [sp, #0x14]
	bl _fmul
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_0222BE6E:
	bl _ffix
	ldr r1, [sp, #0x18]
	add r0, r1, r0
	str r0, [sp, #0x18]
	ldr r0, [sp, #0x10]
	mov r1, #0
	bl _fgr
	ldr r0, _0222BED4 ; =0x45800000
	bls _0222BE96
	ldr r1, [sp, #0x10]
	bl _fmul
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _0222BEA4
_0222BE96:
	ldr r1, [sp, #0x10]
	bl _fmul
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_0222BEA4:
	bl _ffix
	ldr r1, [sp, #0x1c]
	add r0, r1, r0
	str r0, [sp, #0x1c]
	ldr r0, [r5, #0xc]
	add r1, sp, #0x18
	bl Sprite_SetMatrix
	b _0222BEBC
_0222BEB8:
	mov r0, #1
	str r0, [r5, #8]
_0222BEBC:
	mov r1, #0x5a
	ldr r0, [r5, #0x28]
	lsl r1, r1, #2
	bl _s32_div_f
	str r1, [r5, #0x28]
	ldr r0, [r5, #0xc]
	add r1, sp, #0x18
	bl Sprite_SetMatrix
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0222BED4: .word 0x45800000
	thumb_func_end MysteryGiftAnim_UpdateParticle
