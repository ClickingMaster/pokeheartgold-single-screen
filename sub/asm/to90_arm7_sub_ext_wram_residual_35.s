	.include "asm/macros.inc"
	.include "global.inc"

	.extern OS_IsAlarmAvailable

.text

	arm_func_start __VENEER_OS_IsAlarmAvailable
__VENEER_OS_IsAlarmAvailable: ; 0x0600B64C
	ldr pc, _0600B650 ; =OS_IsAlarmAvailable
	.align 2, 0
_0600B650: .word OS_IsAlarmAvailable
	arm_func_end __VENEER_OS_IsAlarmAvailable
