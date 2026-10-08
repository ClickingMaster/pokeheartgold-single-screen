	.include "asm/macros.inc"
	.include "global.inc"

	.extern MATH_CountPopulation
	.extern OS_LockMutex
	.extern OS_UnlockMutex
	.extern WMSP_WL_MaKeyData
	.extern WMSP_WL_MaMp
	.extern WmspGetTmptt
	.extern __VENEER_MIi_CpuCopy16
	.extern __VENEER_OS_DisableInterrupts
	.extern __VENEER_OS_RestoreInterrupts
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern wmspW

.text
