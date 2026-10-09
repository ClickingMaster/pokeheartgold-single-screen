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
