	.extern SpinTrade_State_Start
	.extern SpinTrade_State_WaitFadeIn
	.extern SpinTrade_State_WaitMembers
	.extern SpinTrade_State_SendStart
	.extern SpinTrade_State_WaitConnections
	.extern SpinTrade_State_SendPlayerCount
	.extern SpinTrade_State_SendSlots
	.extern SpinTrade_State_SendReady
	.extern SpinTrade_State_SendJoin
	.extern SpinTrade_State_WaitReady
	.extern SpinTrade_State_StartSync
	.extern SpinTrade_State_WaitSync
	.extern SpinTrade_State_SendTradeSlot
	.extern SpinTrade_State_SendParty
	.extern SpinTrade_State_CheckEggs
	.extern SpinTrade_State_WaitConfirm
	.extern SpinTrade_State_SetupDrop
	.extern SpinTrade_State_DropEggs
	.extern SpinTrade_State_WaitEggsLanded
	.extern SpinTrade_State_WaitBeforeSpin
	.extern SpinTrade_State_SendSpinStart
	.extern SpinTrade_State_WaitSpinStart
	.extern SpinTrade_State_StartSpinSync
	.extern SpinTrade_State_WaitSpinSync
	.extern SpinTrade_State_StartCountdown
	.extern SpinTrade_State_ClearNames
	.extern SpinTrade_State_BeginSpin
	.extern SpinTrade_State_SpinHost
	.extern SpinTrade_State_SpinClient
	.extern SpinTrade_State_StopSpin
	.extern SpinTrade_State_RotateToOwnEgg
	.extern SpinTrade_State_AlignToSlot
	.extern SpinTrade_State_StartResultSync
	.extern SpinTrade_State_WaitResultSync
	.extern SpinTrade_State_SendResult
	.extern SpinTrade_State_ReceiveResult
	.extern SpinTrade_State_ShowNames
	.extern SpinTrade_State_ShowReceivedEgg
	.extern SpinTrade_State_WaitEggFanfare
	.extern SpinTrade_State_GiveBonusBerry
	.extern SpinTrade_State_WaitBerryFanfare
	.extern SpinTrade_State_ShowBagFull
	.extern SpinTrade_State_Wait
	.extern SpinTrade_State_StartSave
	.extern SpinTrade_State_WaitSaveSync
	.extern SpinTrade_State_Save
	.extern SpinTrade_State_Cancel
	.extern SpinTrade_State_WaitCancelMessage
	.extern SpinTrade_State_StartExitSync
	.extern SpinTrade_State_WaitExitSync
	.extern SpinTrade_State_WaitDisconnect
	.extern SpinTrade_State_FadeOut
	.extern SpinTrade_State_WaitFadeOut
	.extern SpinTrade_State_Exit
	.extern SpinTradeEggDrop_Init
	.extern SpinTradeEggDrop_Fall
	.extern SpinTradeEggDrop_Land
	.extern SpinTrade_AddHover
	.public gSpinTradeHoverOffsets
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
	.public SpinTradeEggDrop_Init
	.public SpinTradeEggDrop_Fall
	.public SpinTradeEggDrop_Land
	.public SpinTrade_AddHover
	.public SpinTradeGroup_State_ShowPrompt
	.public SpinTradeGroup_State_HandleInput
	.public SpinTradeGroup_State_WaitTextThenResetMenu
	.public SpinTradeGroup_State_WaitPrinter
	.public SpinTradeGroup_State_ResetLeaderMenuState
	.public SpinTradeGroup_State_ShowCancelYesNo
	.public SpinTradeGroup_State_HandleCancelYesNo
	.public SpinTradeGroup_State_ResetLeaveWait
	.public SpinTradeGroup_State_SendLeave
	.public SpinTradeGroup_State_AbortLeave
	.public SpinTradeGroup_State_ShowConfirmYesNo
	.public SpinTradeGroup_State_HandleConfirmYesNo
	.public SpinTradeGroup_State_SendStartSync
	.public SpinTradeGroup_State_WaitSyncCount
	.public SpinTradeGroup_State_BeforeSave
	.public SpinTradeGroup_State_StartSave
	.public SpinTradeGroup_State_Save
	.public SpinTradeGroup_State_WaitAfterSave
	.public SpinTradeGroup_State_ShowCanceled
	.public SpinTradeGroup_State_FadeOutCanceled
	.public SpinTradeGroup_State_ShowDisbandYesNo
	.public SpinTradeGroup_State_HandleDisbandYesNo
	.public SpinTradeGroup_State_ShowDisbanded
	.public SpinTradeGroup_State_WaitBeforeExit
	.public SpinTradeGroup_State_StartExitSync
	.public SpinTradeGroup_State_WaitExitSync
	.public SpinTradeGroup_State_ShowMemberDropped
	.public SpinTradeGroup_State_WaitMemberDropped
	.public SpinTradeGroup_State_ShowMemberLeft
	.public SpinTradeGroup_State_StartMemberLeftWait
	.public SpinTradeGroup_State_WaitMemberLeft
	.public SpinTradeGroup_State_Proceed
	.public gSpinTradeBonusRanges2P
	.public gSpinTradeBonusRanges3P
	.public gSpinTradeMsgWindowTemplate
	.public gSpinTradeBonusRanges5P
	.public gSpinTradeBonusRanges4P
	.public gSpinTradeEggDropStateFuncs
	.public gSpinTradeCountdownSounds
	.public gSpinTradeGraphicsModes
	.public gSpinTradeNameWindowTemplates2P
	.public gSpinTradeMarkerModelIds
	.public gSpinTradeOamCharTransferParam
	.public gSpinTradeMarkerAnimIds
	.public gSpinTradeResObjCounts
	.public gSpinTradeNameWindowTemplates
	.public gSpinTradeMachineModelIds
	.public gSpinTradeMachineAngleOffsets
	.public gSpinTradeNameWindowTemplates3P
	.public gSpinTradeBg1Template
	.public gSpinTradeBg2Template
	.public gSpinTradeBg3Template
	.public gSpinTradeSubBg2Template
	.public gSpinTradeSubBg3Template
	.public gSpinTradeNameWindowTemplates4P
	.public gSpinTradeOamManagerParam
	.public gSpinTradeNameWindowTemplates5P
	.public gSpinTradeGraphicsBanks
	.public gSpinTradeBonusBerryRanges
	.public ov85_021EA72C
	.public gSpinTradeBonusAreas
	.public gSpinTradeSlotAngles
	.public gSpinTradeDropTargetAngles
	.public gSpinTradeStateFuncs
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
	.include "asm/macros.inc"
	.include "overlay_85.inc"
	.include "global.inc"

.text
	.rodata

gSpinTradeBonusRanges2P:
	.byte 0x00, 0x00, 0xB4, 0x00

gSpinTradeBonusRanges3P:
	.byte 0x00, 0x00, 0xC6, 0x00, 0x56, 0x01, 0x67, 0x01

gSpinTradeMsgWindowTemplate: ; 0x021EA4FC
	.byte 0x01, 0x02, 0x13, 0x1A
	.byte 0x04, 0x0F, 0x28, 0x00

gSpinTradeBonusRanges5P:
	.byte 0x00, 0x00, 0xD7, 0x00, 0x43, 0x01, 0x67, 0x01, 0x00, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00

gSpinTradeBonusRanges4P:
	.byte 0x25, 0x00, 0x8E, 0x00, 0xD7, 0x00, 0x43, 0x01

gSpinTradeEggDropStateFuncs: ; 0x021EA51C
	.word SpinTradeEggDrop_Init, SpinTradeEggDrop_Fall, SpinTradeEggDrop_Land

gSpinTradeCountdownSounds: ; 0x021EA528
	.byte 0xDD, 0x05, 0x00, 0x00, 0xDD, 0x05, 0x00, 0x00
	.byte 0xDD, 0x05, 0x00, 0x00, 0x42, 0x06, 0x00, 0x00

gSpinTradeGraphicsModes: ; 0x021EA538
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00

gSpinTradeNameWindowTemplates2P:
	.byte 0x01, 0x0C, 0x12, 0x08, 0x02, 0x0F, 0x94, 0x00
	.byte 0x01, 0x0C, 0x06, 0x08, 0x02, 0x0F, 0xBC, 0x00

gSpinTradeMarkerModelIds: ; 0x021EA558
	.byte 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00

gSpinTradeOamCharTransferParam: ; 0x021EA56C
	.byte 0x60, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x01, 0x00, 0x00, 0x40, 0x00, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x00, 0x00

gSpinTradeMarkerAnimIds: ; 0x021EA580
	.byte 0x0B, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00
	.byte 0x0F, 0x00, 0x00, 0x00

gSpinTradeResObjCounts: ; 0x021EA594
	.byte 0x60, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00
	.byte 0x40, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00

gSpinTradeNameWindowTemplates: ; 0x021EA5AC
	.word 0
	.word 0
	.word gSpinTradeNameWindowTemplates2P
	.word gSpinTradeNameWindowTemplates3P
	.word gSpinTradeNameWindowTemplates4P
	.word gSpinTradeNameWindowTemplates5P

gSpinTradeMachineModelIds: ; 0x021EA5C4
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00

gSpinTradeMachineAngleOffsets: ; 0x021EA5DC
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFB, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB0, 0xF4, 0xFF
	.byte 0x00, 0x00, 0x00, 0x00

gSpinTradeNameWindowTemplates3P:
	.byte 0x01, 0x0C, 0x12, 0x08, 0x02, 0x0F, 0x94, 0x00, 0x01, 0x05, 0x0A, 0x08
	.byte 0x02, 0x0F, 0xBC, 0x00, 0x01, 0x13, 0x0A, 0x08, 0x02, 0x0F, 0xE4, 0x00

gSpinTradeBg1Template: ; 0x021EA60C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

gSpinTradeBg2Template: ; 0x021EA628
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x03, 0x00, 0x02, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

gSpinTradeBg3Template: ; 0x021EA644
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

gSpinTradeSubBg2Template: ; 0x021EA660
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x02, 0x05, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

gSpinTradeSubBg3Template: ; 0x021EA67C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x03, 0x07
	.byte 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

gSpinTradeNameWindowTemplates4P:
	.byte 0x01, 0x0C, 0x12, 0x08, 0x02, 0x0F, 0x94, 0x00
	.byte 0x01, 0x04, 0x0D, 0x08, 0x02, 0x0F, 0xBC, 0x00, 0x01, 0x0C, 0x06, 0x08, 0x02, 0x0F, 0xE4, 0x00
	.byte 0x01, 0x14, 0x0D, 0x08, 0x02, 0x0F, 0x0C, 0x01

gSpinTradeOamManagerParam: ; 0x021EA6B8
	.byte 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00

gSpinTradeNameWindowTemplates5P:
	.byte 0x01, 0x0C, 0x13, 0x08, 0x02, 0x0F, 0x94, 0x00
	.byte 0x01, 0x04, 0x0E, 0x08, 0x02, 0x0F, 0xBC, 0x00, 0x01, 0x06, 0x09, 0x08, 0x02, 0x0F, 0xE4, 0x00
	.byte 0x01, 0x12, 0x09, 0x08, 0x02, 0x0F, 0x0C, 0x01, 0x01, 0x14, 0x0E, 0x08, 0x02, 0x0F, 0x34, 0x01

gSpinTradeGraphicsBanks: ; 0x021EA700
	.byte 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00

gSpinTradeBonusBerryRanges: ; 0x021EA728
	.byte 0x00, 0x00, 0x00, 0x00

ov85_021EA72C: ; 0x021EA72C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9F, 0x00, 0x00, 0x00, 0xA3, 0x00, 0x00, 0x00
	.byte 0x9F, 0x00, 0x00, 0x00, 0xA3, 0x00, 0x00, 0x00, 0xA4, 0x00, 0x00, 0x00, 0xA8, 0x00, 0x00, 0x00
	.byte 0xAF, 0x00, 0x00, 0x00, 0xB7, 0x00, 0x00, 0x00

gSpinTradeBonusAreas: ; 0x021EA758
	.word 0, 0
	.word 0, 0
	.word 1, gSpinTradeBonusRanges2P
	.word 2, gSpinTradeBonusRanges3P
	.word 2, gSpinTradeBonusRanges4P
	.word 2, gSpinTradeBonusRanges5P

gSpinTradeSlotAngles: ; 0x021EA788
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB4, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x0E, 0x01, 0xB4, 0x00, 0x5A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x01, 0xD8, 0x00
	.byte 0x90, 0x00, 0x48, 0x00

gSpinTradeDropTargetAngles: ; 0x021EA7C4
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5A, 0x00, 0x0E, 0x01, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x5A, 0x00, 0xD2, 0x00, 0x4A, 0x01, 0x00, 0x00, 0x00, 0x00, 0x5A, 0x00, 0xB4, 0x00
	.byte 0x0E, 0x01, 0x00, 0x00, 0x00, 0x00, 0x5A, 0x00, 0xA2, 0x00, 0xEA, 0x00, 0x32, 0x01, 0x12, 0x00

gSpinTradeStateFuncs: ; 0x021EA800
	.word SpinTrade_State_Start
	.word SpinTrade_State_WaitFadeIn
	.word SpinTrade_State_WaitMembers
	.word SpinTrade_State_SendStart
	.word SpinTrade_State_WaitConnections
	.word SpinTrade_State_SendPlayerCount
	.word SpinTrade_State_SendSlots
	.word SpinTrade_State_SendReady
	.word SpinTrade_State_SendJoin
	.word SpinTrade_State_WaitReady
	.word SpinTrade_State_StartSync
	.word SpinTrade_State_WaitSync
	.word SpinTrade_State_SendTradeSlot
	.word SpinTrade_State_SendParty
	.word SpinTrade_State_CheckEggs
	.word SpinTrade_State_WaitConfirm
	.word SpinTrade_State_SetupDrop
	.word SpinTrade_State_DropEggs
	.word SpinTrade_State_WaitEggsLanded
	.word SpinTrade_State_WaitBeforeSpin
	.word SpinTrade_State_SendSpinStart
	.word SpinTrade_State_WaitSpinStart
	.word SpinTrade_State_StartSpinSync
	.word SpinTrade_State_WaitSpinSync
	.word SpinTrade_State_StartCountdown
	.word SpinTrade_State_ClearNames
	.word SpinTrade_State_BeginSpin
	.word SpinTrade_State_SpinHost
	.word SpinTrade_State_SpinClient
	.word SpinTrade_State_StopSpin
	.word SpinTrade_State_RotateToOwnEgg
	.word SpinTrade_State_AlignToSlot
	.word SpinTrade_State_StartResultSync
	.word SpinTrade_State_WaitResultSync
	.word SpinTrade_State_SendResult
	.word SpinTrade_State_ReceiveResult
	.word SpinTrade_State_ShowNames
	.word SpinTrade_State_ShowReceivedEgg
	.word SpinTrade_State_WaitEggFanfare
	.word SpinTrade_State_GiveBonusBerry
	.word SpinTrade_State_WaitBerryFanfare
	.word SpinTrade_State_ShowBagFull
	.word SpinTrade_State_Wait
	.word SpinTrade_State_StartSave
	.word SpinTrade_State_WaitSaveSync
	.word SpinTrade_State_Save
	.word SpinTrade_State_Cancel
	.word SpinTrade_State_WaitCancelMessage
	.word SpinTrade_State_StartExitSync
	.word SpinTrade_State_WaitExitSync
	.word SpinTrade_State_WaitDisconnect
	.word SpinTrade_State_FadeOut
	.word SpinTrade_State_WaitFadeOut
	.word SpinTrade_State_Exit

	.data

gSpinTradeHoverOffsets:
	.byte 0x04, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00
	.byte 0x0B, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00
	.byte 0x0B, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
	.byte 0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
