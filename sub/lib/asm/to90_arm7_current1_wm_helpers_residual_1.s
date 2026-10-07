	.include "asm/macros.inc"
	.include "global.inc"

	.extern OS_DisableInterrupts
	.extern OS_DisableScheduler
	.extern OS_EnableScheduler
	.extern OS_RestoreInterrupts
	.extern OS_SetThreadPriority

.text

	arm_func_start __VENEER_OS_DisableInterrupts
__VENEER_OS_DisableInterrupts: ; 0x06000720
	ldr pc, _06000724 ; =OS_DisableInterrupts
	.align 2, 0
_06000724: .word OS_DisableInterrupts
	arm_func_end __VENEER_OS_DisableInterrupts

	arm_func_start __VENEER_OS_DisableScheduler
__VENEER_OS_DisableScheduler: ; 0x06000728
	ldr pc, _0600072C ; =OS_DisableScheduler
	.align 2, 0
_0600072C: .word OS_DisableScheduler
	arm_func_end __VENEER_OS_DisableScheduler

	arm_func_start __VENEER_OS_SetThreadPriority
__VENEER_OS_SetThreadPriority: ; 0x06000730
	ldr pc, _06000734 ; =OS_SetThreadPriority
	.align 2, 0
_06000734: .word OS_SetThreadPriority
	arm_func_end __VENEER_OS_SetThreadPriority

	arm_func_start __VENEER_OS_EnableScheduler
__VENEER_OS_EnableScheduler: ; 0x06000738
	ldr pc, _0600073C ; =OS_EnableScheduler
	.align 2, 0
_0600073C: .word OS_EnableScheduler
	arm_func_end __VENEER_OS_EnableScheduler

	arm_func_start __VENEER_OS_RestoreInterrupts
__VENEER_OS_RestoreInterrupts: ; 0x06000740
	ldr pc, _06000744 ; =OS_RestoreInterrupts
	.align 2, 0
_06000744: .word OS_RestoreInterrupts
	arm_func_end __VENEER_OS_RestoreInterrupts
