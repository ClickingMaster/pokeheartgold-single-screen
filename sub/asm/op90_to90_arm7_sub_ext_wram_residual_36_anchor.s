	.include "asm/macros.inc"
	.include "global.inc"

	.extern OS_SetPeriodicAlarm
	.extern __VENEER_OS_CancelAlarm
	.extern __VENEER_OS_GetTick

.text
