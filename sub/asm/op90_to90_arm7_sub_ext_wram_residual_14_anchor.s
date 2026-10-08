	.include "asm/macros.inc"
	.include "global.inc"

	.extern WMSP_AddRssiToRandomPool
	.extern WMSP_GetLinkLevel
	.extern WMSP_GetRssi8_06003988
	.extern WMSP_SetAllParams
	.extern WMSP_WL_DevClass1
	.extern WMSP_WL_DevGetStationState
	.extern WMSP_WL_MlmePowerManagement
	.extern WMSP_WL_MlmeScan
	.extern WMSP_WL_ParamSetPreambleType
	.extern WMSP_WL_ParamSetSsidMask
	.extern WmspError_06003ED4
	.extern __VENEER_MI_CpuCopy8
	.extern __VENEER_MI_CpuFill8
	.extern __VENEER_MIi_CpuClear16
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern _u32_div_f
	.extern wmspW

.text
