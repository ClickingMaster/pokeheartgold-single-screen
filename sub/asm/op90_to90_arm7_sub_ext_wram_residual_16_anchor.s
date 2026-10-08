	.include "asm/macros.inc"
	.include "global.inc"

	.extern WMSP_CancelVAlarm
	.extern WMSP_CleanSendQueue
	.extern WMSP_IndicateDisconnectionFromMyself
	.extern WMSP_ResetSizeVars
	.extern WMSP_SetThreadPriorityLow
	.extern WMSP_WL_DevIdle
	.extern WMSP_WL_MlmeDeAuthenticate
	.extern WMSP_WL_MlmeReset
	.extern WmspError_06004CB4
	.extern WmspIndError
	.extern __VENEER_MI_CpuCopy8
	.extern __VENEER_MI_CpuFill8
	.extern __VENEER_OS_DisableInterrupts
	.extern __VENEER_OS_RestoreInterrupts
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern wmspW

.text
