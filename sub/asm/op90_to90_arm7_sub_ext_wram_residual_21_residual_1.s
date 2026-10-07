	.extern OS_Sleep
	.extern __VENEER_MATH_CountPopulation
	.extern __VENEER_OS_DisableInterrupts
	.extern __VENEER_OS_LockMutex
	.extern __VENEER_OS_RestoreInterrupts
	.extern __VENEER_OS_UnlockMutex
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern wmspW
	.include "asm/macros.inc"
	.include "global.inc"

.text
	arm_func_start __VENEER_OS_Sleep
__VENEER_OS_Sleep: ; 0x06007540
	ldr pc, _06007544 ; =OS_Sleep
	.align 2, 0
_06007544: .word OS_Sleep
	arm_func_end __VENEER_OS_Sleep
