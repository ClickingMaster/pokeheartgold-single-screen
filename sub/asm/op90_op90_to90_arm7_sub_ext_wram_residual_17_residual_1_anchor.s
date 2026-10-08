	.include "asm/macros.inc"
	.include "global.inc"
	.extern WMSP_CleanSendQueue
	.extern WMSP_InitSendQueue
	.extern WMSP_PutSendQueue
	.extern WMSP_SetMPParameterCore
	.extern WMSP_SetThreadPriorityHigh
	.extern WMSP_SetVAlarm
	.extern WMSP_WL_ParamSetNullKeyResponseMode
	.extern __VENEER_OS_DisableInterrupts
	.extern __VENEER_OS_GetTick
	.extern __VENEER_OS_RestoreInterrupts
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern wmspW

.text
