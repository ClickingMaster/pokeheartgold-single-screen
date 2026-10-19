	.include "asm/macros.inc"
	.include "overlay_93_thumb_2.inc"
	.include "global.inc"

	.text
	.public WobbuffetPop_RecvPlayerValue
	.public gWobbuffetPopCommands
	.public gWobbuffetPopEventFuncs
	.public WobbuffetPop_GetEventCommandSize
	.public WobbuffetPop_GetPlayerValueCommandSize
	.public WobbuffetPop_RecvEvent
	.public WobbuffetPop_RecvFinished
	.public WobbuffetPop_HandlePumpEvent
	.public WobbuffetPop_HandlePopEvent
	.public WobbuffetPop_HandleNewBalloonEvent
	.public WobbuffetPop_HandleCountdownEvent
	.public WobbuffetPop_HandleFinishEvent
	.public WobbuffetPop_HandleSyncEvent
	.public WobbuffetPop_HandleDemoEvent
	.rodata

gWobbuffetPopBalloonAirRates:
	.byte 0x64, 0x00, 0x00, 0x00, 0x7D, 0x00, 0x00, 0x00
	.byte 0x58, 0x00, 0x00, 0x00

gWobbuffetPopCommands:
	.word 0
	.word CommCmd_GetSize0
	.word 0
	.word WobbuffetPop_RecvPlayerValue
	.word WobbuffetPop_GetPlayerValueCommandSize
	.word 0
	.word WobbuffetPop_RecvEvent
	.word WobbuffetPop_GetEventCommandSize
	.word 0
	.word WobbuffetPop_RecvFinished
	.word CommCmd_GetSize0
	.word 0

gWobbuffetPopEventFuncs:
	.word 0
	.word WobbuffetPop_HandlePumpEvent
	.word WobbuffetPop_HandlePopEvent
	.word WobbuffetPop_HandleNewBalloonEvent
	.word WobbuffetPop_HandleCountdownEvent
	.word WobbuffetPop_HandleFinishEvent
	.word WobbuffetPop_HandleSyncEvent
	.word WobbuffetPop_HandleDemoEvent
