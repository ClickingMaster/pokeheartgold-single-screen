#include "constants/easy_chat.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/species.h"
	.include "asm/macros.inc"
	.include "unk_02097024.inc"
	.include "global.inc"

	.rodata

	.public gOverlayTemplate_SpinTradeGroup
gOverlayTemplate_SpinTradeGroup:
	.word SpinTradeGroupApp_Init, SpinTradeGroupApp_Main, SpinTradeGroupApp_Exit, FS_OVERLAY_ID(OVY_85)
	.public gOverlayTemplate_SpinTrade
gOverlayTemplate_SpinTrade:
	.word SpinTradeApp_Init, SpinTradeApp_Main, SpinTradeApp_Exit, FS_OVERLAY_ID(OVY_85)
	.public gSpinTradeFlowStates
gSpinTradeFlowStates:
	.word SpinTradeFlow_LaunchGroupApp
	.word SpinTradeFlow_WaitGroupApp
	.word SpinTradeFlow_WaitPartyMenu
	.word SpinTradeFlow_WaitSummary
	.word SpinTradeFlow_WaitSpinTradeApp
	.word SpinTradeFlow_Finish
