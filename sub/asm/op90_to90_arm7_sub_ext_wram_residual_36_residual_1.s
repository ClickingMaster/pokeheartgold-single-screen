	.extern OS_SetPeriodicAlarm
	.extern __VENEER_OS_CancelAlarm
	.extern __VENEER_OS_GetTick
	.include "asm/macros.inc"
	.include "global.inc"
	.public __VENEER_OS_SetPeriodicAlarm

.text
	arm_func_start __VENEER_OS_SetPeriodicAlarm
__VENEER_OS_SetPeriodicAlarm: ; 0x0600B704
	ldr pc, _0600B708 ; =OS_SetPeriodicAlarm
	.align 2, 0
_0600B708: .word OS_SetPeriodicAlarm
	arm_func_end __VENEER_OS_SetPeriodicAlarm
