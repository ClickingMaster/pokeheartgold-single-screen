	.include "asm/macros.inc"
	.include "global.inc"

	.extern PM_SetLEDPattern
	.extern WMSP_CopyParentParam
	.extern WMSP_SetAllParams
	.extern WMSP_SetChildMaxSize
	.extern WMSP_SetParentMaxSize
	.extern WMSP_WL_DevClass1
	.extern WMSP_WL_MlmePowerManagement
	.extern WMSP_WL_MlmeStart
	.extern WMSP_WL_ParamSetMaxConnectableChild
	.extern WmspError_060033F4
	.extern __VENEER_MI_CpuCopy8
	.extern __VENEER_MIi_CpuClear16
	.extern __VENEER_OS_DisableInterrupts
	.extern __VENEER_OS_RestoreInterrupts
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern wmspW

.text
