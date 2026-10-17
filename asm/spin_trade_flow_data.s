	.include "asm/macros.inc"
	.include "spin_trade_flow_data.inc"
	.include "global.inc"

	.public gOverlayTemplate_SpinTradeGroup
	.public gOverlayTemplate_SpinTrade
	.public gSpinTradeFlowStates

	.rodata

gOverlayTemplate_SpinTradeGroup:
	.word SpinTradeGroupApp_Init, SpinTradeGroupApp_Main, SpinTradeGroupApp_Exit, FS_OVERLAY_ID(OVY_85)
gOverlayTemplate_SpinTrade:
	.word SpinTradeApp_Init, SpinTradeApp_Main, SpinTradeApp_Exit, FS_OVERLAY_ID(OVY_85)
gSpinTradeFlowStates:
	.word SpinTradeFlow_LaunchGroupApp
	.word SpinTradeFlow_WaitGroupApp
	.word SpinTradeFlow_WaitPartyMenu
	.word SpinTradeFlow_WaitSummary
	.word SpinTradeFlow_WaitSpinTradeApp
	.word SpinTradeFlow_Finish
