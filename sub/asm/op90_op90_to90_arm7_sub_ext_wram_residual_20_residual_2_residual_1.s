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
	.extern __VENEER_OS_LockMutex
	.extern __VENEER_OS_UnlockMutex
	.public __VENEER_MATH_CountPopulation

.text
	arm_func_start __VENEER_MATH_CountPopulation
__VENEER_MATH_CountPopulation: ; 0x06007448
	ldr pc, _0600744C ; =MATH_CountPopulation
	.align 2, 0
_0600744C: .word MATH_CountPopulation
	arm_func_end __VENEER_MATH_CountPopulation
