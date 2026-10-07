	.include "asm/macros.inc"
	.include "global.inc"

	.extern OS_ReceiveMessage
	.extern OS_SendMessage
	.extern OS_Terminate
	.extern SND_BeginSleep
	.extern WMSP_GetBuffer4Callback2Wm9
	.extern WMSP_ReturnResult2Wm9

.text

	arm_func_start __VENEER_OS_SendMessage
__VENEER_OS_SendMessage: ; 0x06000284
	ldr pc, _06000288 ; =OS_SendMessage
	.align 2, 0
_06000288: .word OS_SendMessage
	arm_func_end __VENEER_OS_SendMessage

	arm_func_start __VENEER_OS_ReceiveMessage
__VENEER_OS_ReceiveMessage: ; 0x0600028C
	ldr pc, _06000290 ; =OS_ReceiveMessage
	.align 2, 0
_06000290: .word OS_ReceiveMessage
	arm_func_end __VENEER_OS_ReceiveMessage

	arm_func_start __VENEER_WMSP_GetBuffer4Callback2Wm9
__VENEER_WMSP_GetBuffer4Callback2Wm9: ; 0x06000294
	ldr pc, _06000298 ; =WMSP_GetBuffer4Callback2Wm9
	.align 2, 0
_06000298: .word WMSP_GetBuffer4Callback2Wm9
	arm_func_end __VENEER_WMSP_GetBuffer4Callback2Wm9

	arm_func_start __VENEER_WMSP_ReturnResult2Wm9
__VENEER_WMSP_ReturnResult2Wm9: ; 0x0600029C
	ldr pc, _060002A0 ; =WMSP_ReturnResult2Wm9
	.align 2, 0
_060002A0: .word WMSP_ReturnResult2Wm9
	arm_func_end __VENEER_WMSP_ReturnResult2Wm9

	arm_func_start __VENEER_SND_BeginSleep
__VENEER_SND_BeginSleep: ; 0x060002A4
	ldr pc, _060002A8 ; =SND_BeginSleep
	.align 2, 0
_060002A8: .word SND_BeginSleep
	arm_func_end __VENEER_SND_BeginSleep

	arm_func_start __VENEER_OS_Terminate
__VENEER_OS_Terminate: ; 0x060002AC
	ldr pc, _060002B0 ; =OS_Terminate
	.align 2, 0
_060002B0: .word OS_Terminate
	arm_func_end __VENEER_OS_Terminate
