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
	.public __VENEER_OS_LockMutex
	.public __VENEER_OS_UnlockMutex

.text
	arm_func_start __VENEER_OS_LockMutex
__VENEER_OS_LockMutex: ; 0x06006598
	ldr pc, _0600659C ; =OS_LockMutex
	.align 2, 0
_0600659C: .word OS_LockMutex
	arm_func_end __VENEER_OS_LockMutex

	arm_func_start __VENEER_OS_UnlockMutex
__VENEER_OS_UnlockMutex: ; 0x060065A0
	ldr pc, _060065A4 ; =OS_UnlockMutex
	.align 2, 0
_060065A4: .word OS_UnlockMutex
	arm_func_end __VENEER_OS_UnlockMutex
