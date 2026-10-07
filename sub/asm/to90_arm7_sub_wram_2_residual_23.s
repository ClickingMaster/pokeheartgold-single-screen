	.include "asm/macros.inc"
	.include "global.inc"

	.extern WL_Terminate
	.extern WMSP_CancelAllAlarms
	.extern WMSP_CancelVAlarm
	.extern WMSP_GetIndicateThread
	.extern WMSP_GetRequestThread

.text

	; veneers into wvram

	arm_func_start __VENEER_WMSP_CancelVAlarm
__VENEER_WMSP_CancelVAlarm: ; 0x03805FE8
	ldr pc, _03805FEC ; =WMSP_CancelVAlarm
	.align 2, 0
_03805FEC: .word WMSP_CancelVAlarm
	arm_func_end __VENEER_WMSP_CancelVAlarm

	arm_func_start __VENEER_WMSP_CancelAllAlarms
__VENEER_WMSP_CancelAllAlarms: ; 0x03805FF0
	ldr pc, _03805FF4 ; =WMSP_CancelAllAlarms
	.align 2, 0
_03805FF4: .word WMSP_CancelAllAlarms
	arm_func_end __VENEER_WMSP_CancelAllAlarms

	arm_func_start __VENEER_WMSP_GetRequestThread
__VENEER_WMSP_GetRequestThread: ; 0x03805FF8
	ldr pc, _03805FFC ; =WMSP_GetRequestThread
	.align 2, 0
_03805FFC: .word WMSP_GetRequestThread
	arm_func_end __VENEER_WMSP_GetRequestThread

	arm_func_start __VENEER_WL_Terminate
__VENEER_WL_Terminate: ; 0x03806000
	ldr pc, _03806004 ; =WL_Terminate
	.align 2, 0
_03806004: .word WL_Terminate
	arm_func_end __VENEER_WL_Terminate

	arm_func_start __VENEER_WMSP_GetIndicateThread
__VENEER_WMSP_GetIndicateThread: ; 0x03806008
	ldr pc, _0380600C ; =WMSP_GetIndicateThread
	.align 2, 0
_0380600C: .word WMSP_GetIndicateThread
	arm_func_end __VENEER_WMSP_GetIndicateThread

	; math
