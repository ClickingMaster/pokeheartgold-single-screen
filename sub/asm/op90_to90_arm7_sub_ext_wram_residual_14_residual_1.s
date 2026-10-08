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
	.public __VENEER__u32_div_f

.text
	arm_func_start __VENEER__u32_div_f
__VENEER__u32_div_f: ; 0x06003ECC
	ldr pc, _06003ED0 ; =_u32_div_f
	.align 2, 0
_06003ED0: .word _u32_div_f
	arm_func_end __VENEER__u32_div_f
