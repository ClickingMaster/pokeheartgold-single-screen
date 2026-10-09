	.include "asm/macros.inc"
	.include "global.inc"
	.extern OS_LockMutex
	.extern OS_UnlockMutex
	.extern PXI_SendWordByFifo
	.extern VENEER_SVC_WaitByLoop
	.extern wmspW
	.public tpw
	.public last_touch_flg
	.public PMi_KeyPattern
	.public PMi_TriggerBL
	.public PMi_Initialized
	.public PMi_Work
	.public PMi_BlinkCounter
	.public PMi_BlinkPatternNo
	.bss
tpw: ; 0x038092D4
	.space 0xD4
	.public last_touch_flg

last_touch_flg: ; 0x038093A8
	.space 0x2

	.balign 4, 0
	.public PMi_KeyPattern
PMi_KeyPattern: ; 0x038093AC
	.space 0x2
	.public PMi_TriggerBL

PMi_TriggerBL: ; 0x038093AE
	.space 0x2

	.public PMi_Initialized
PMi_Initialized: ; 0x038093B0
	.space 0x4
	.public PMi_Work

PMi_Work: ; 0x038093B4
	.space 0x2C
	.public PMi_BlinkCounter

PMi_BlinkCounter: ; 0x038093E0
	.space 0x4

PMi_BlinkPatternNo: ; 0x038093E4
	.space 0x4
	.public micw

