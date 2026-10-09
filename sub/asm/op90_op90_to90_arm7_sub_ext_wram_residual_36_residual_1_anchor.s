	.extern OS_SetPeriodicAlarm
	.extern __VENEER_OS_CancelAlarm
	.extern __VENEER_OS_GetTick
	.include "asm/macros.inc"
	.include "global.inc"
	.public __VENEER_OS_SetPeriodicAlarm

.text
