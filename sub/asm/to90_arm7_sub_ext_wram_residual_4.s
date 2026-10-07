	.include "asm/macros.inc"
	.include "global.inc"

	.extern OS_CreateAlarm

.text

	arm_func_start __VENEER_OS_CreateAlarm
__VENEER_OS_CreateAlarm: ; 0x06001DE4
	ldr pc, _06001DE8 ; =OS_CreateAlarm
	.align 2, 0
_06001DE8: .word OS_CreateAlarm
	arm_func_end __VENEER_OS_CreateAlarm
