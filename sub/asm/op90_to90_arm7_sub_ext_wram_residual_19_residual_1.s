	.extern OS_CancelVAlarm
	.extern OS_SetVAlarm
	.extern WmspChildAdjustVSync2
	.extern WmspChildVAlarmMP
	.extern wmspVAlarm
	.extern wmspW
	.include "asm/macros.inc"
	.include "global.inc"
	.public __VENEER_OS_SetVAlarm

.text
	arm_func_start __VENEER_OS_CancelVAlarm
__VENEER_OS_CancelVAlarm: ; 0x06005DB0
	ldr pc, _06005DB4 ; =OS_CancelVAlarm
	.align 2, 0
_06005DB4: .word OS_CancelVAlarm
	arm_func_end __VENEER_OS_CancelVAlarm

	arm_func_start __VENEER_OS_SetVAlarm
__VENEER_OS_SetVAlarm: ; 0x06005DB8
	ldr pc, _06005DBC ; =OS_SetVAlarm
	.align 2, 0
_06005DBC: .word OS_SetVAlarm
	arm_func_end __VENEER_OS_SetVAlarm
