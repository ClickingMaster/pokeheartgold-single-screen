	.include "asm/macros.inc"
	.include "global.inc"

	.extern OS_Sleep
	.extern __VENEER_MATH_CountPopulation
	.extern __VENEER_OS_DisableInterrupts
	.extern __VENEER_OS_LockMutex
	.extern __VENEER_OS_RestoreInterrupts
	.extern __VENEER_OS_UnlockMutex
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern wmspW

.text
