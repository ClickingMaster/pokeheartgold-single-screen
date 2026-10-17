	.include "asm/macros.inc"
	.include "overlay_93_thumb_2.inc"
	.include "global.inc"

	.text
	.public WobbuffetPop_RecvPlayerValue
	.public sWobbuffetPopCommands
	.public sWobbuffetPopEventFuncs
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

sWobbuffetPopBalloonAirRates:
	.byte 0x64, 0x00, 0x00, 0x00, 0x7D, 0x00, 0x00, 0x00
	.byte 0x58, 0x00, 0x00, 0x00

sWobbuffetPopCommands: ; 0x022630E4
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

sWobbuffetPopEventFuncs: ; 0x02263114
	.word 0
	.word WobbuffetPop_HandlePumpEvent
	.word WobbuffetPop_HandlePopEvent
	.word WobbuffetPop_HandleNewBalloonEvent
	.word WobbuffetPop_HandleCountdownEvent
	.word WobbuffetPop_HandleFinishEvent
	.word WobbuffetPop_HandleSyncEvent
	.word WobbuffetPop_HandleDemoEvent
	; 0x02263134
