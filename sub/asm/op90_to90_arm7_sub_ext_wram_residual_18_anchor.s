	.include "asm/macros.inc"
	.include "global.inc"

	.extern WMSP_SetAllParams
	.extern WMSP_WL_DevClass1
	.extern WMSP_WL_DevGetStationState
	.extern WMSP_WL_DevIdle
	.extern WMSP_WL_MlmeMeasureChannel
	.extern WMSP_WL_MlmePowerManagement
	.extern WmspError_06005BB8
	.extern __VENEER_MI_CpuFill8
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern wmspW

.text
