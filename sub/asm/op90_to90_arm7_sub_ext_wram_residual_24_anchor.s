	.include "asm/macros.inc"
	.include "global.inc"

	.extern WMSP_WL_MaData
	.extern WMSP_WL_MlmePowerManagement
	.extern __VENEER_MI_CpuCopy8
	.extern __VENEER_MIi_CpuClear16
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern wmspW

.text
