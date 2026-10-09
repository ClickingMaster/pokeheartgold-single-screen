	.include "asm/macros.inc"
	.include "global.inc"
	.extern WMSP_FillRssiIntoList
	.extern WMSP_GetLinkLevel
	.extern WMSP_SetAllParams
	.extern WMSP_SetChildMaxSize
	.extern WMSP_SetParentMaxSize
	.extern WMSP_WL_DevClass1
	.extern WMSP_WL_MlmeAssociate
	.extern WMSP_WL_MlmeAuthenticate
	.extern WMSP_WL_MlmeJoin
	.extern WMSP_WL_MlmePowerManagement
	.extern WMSP_WL_ParamSetBeaconLostThreshold
	.extern WMSP_WL_ParamSetNullKeyResponseMode
	.extern WmspError_060045D0
	.extern __VENEER_MI_CpuCopy8
	.extern __VENEER_MIi_CpuClear16
	.extern __VENEER_OS_DisableInterrupts
	.extern __VENEER_OS_GetTick
	.extern __VENEER_OS_RestoreInterrupts
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern _s32_div_f
	.extern wmspW
	.public __VENEER__s32_div_f

.text
