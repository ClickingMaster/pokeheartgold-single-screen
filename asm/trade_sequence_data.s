	.extern TradeSequenceSend_Init
	.extern TradeSequenceSend_Exit
	.extern TradeSequenceSend_Main
	.extern TradeSequenceDepart_Init
	.extern TradeSequenceDepart_Exit
	.extern TradeSequenceDepart_Main
	.extern TradeSequenceRecede_Init
	.extern TradeSequenceRecede_Exit
	.extern TradeSequenceRecede_Main
	.extern TradeSequenceExchange_Init
	.extern TradeSequenceExchange_Exit
	.extern TradeSequenceExchange_Main
	.extern TradeSequenceApproach_Init
	.extern TradeSequenceApproach_Exit
	.extern TradeSequenceApproach_Main
	.extern TradeSequenceArrive_Init
	.extern TradeSequenceArrive_Exit
	.extern TradeSequenceArrive_Main
	.extern TradeSequenceReceive_Init
	.extern TradeSequenceReceive_Exit
	.extern TradeSequenceReceive_Main
	.public sTradeSequenceBgZoomCenterY
	.public sTradeSequenceMonPicSettings
	.public sTradeSequenceSceneInit
	.public sTradeSequenceSceneMain
	.public sTradeSequenceSceneExit
	.public sTradeSequenceSceneModes
	.include "asm/macros.inc"
	.include "overlay_71.inc"
	.include "global.inc"

	.rodata

sTradeSequenceBgZoomCenterY:
	.byte 0x90, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00

sTradeSequenceMonPicSettings:
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00

sTradeSequenceSceneInit:
	.word TradeSequenceSend_Init

sTradeSequenceSceneMain:
	.word TradeSequenceSend_Main

sTradeSequenceSceneExit:
	.word TradeSequenceSend_Exit

sTradeSequenceSceneModes:
	.byte 0x03, 0x00, 0x00, 0x00
	.word TradeSequenceDepart_Init
	.word TradeSequenceDepart_Main
	.word TradeSequenceDepart_Exit
	.byte 0x03, 0x00, 0x00, 0x00
	.word TradeSequenceRecede_Init
	.word TradeSequenceRecede_Main
	.word TradeSequenceRecede_Exit
	.byte 0x03, 0x00, 0x00, 0x00
	.word TradeSequenceExchange_Init
	.word TradeSequenceExchange_Main
	.word TradeSequenceExchange_Exit
	.byte 0x01, 0x00, 0x00, 0x00
	.word TradeSequenceApproach_Init
	.word TradeSequenceApproach_Main
	.word TradeSequenceApproach_Exit
	.byte 0x05, 0x00, 0x00, 0x00
	.word TradeSequenceArrive_Init
	.word TradeSequenceArrive_Main
	.word TradeSequenceArrive_Exit
	.byte 0x05, 0x00, 0x00, 0x00
	.word TradeSequenceReceive_Init
	.word TradeSequenceReceive_Main
	.word TradeSequenceReceive_Exit
	.byte 0x05, 0x00, 0x00, 0x00
