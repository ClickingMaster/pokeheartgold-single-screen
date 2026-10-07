	.extern AddTask
	.extern OS_IsThreadTerminated
	.extern OS_JoinThread
	.extern pTaskFunc
	.include "asm/macros.inc"
	.include "global.inc"

.text
	arm_func_start __VENEER_OS_IsThreadTerminated
__VENEER_OS_IsThreadTerminated: ; 0x06008DA0
	ldr pc, _06008DA4 ; =OS_IsThreadTerminated
	.align 2, 0
_06008DA4: .word OS_IsThreadTerminated
	arm_func_end __VENEER_OS_IsThreadTerminated

	arm_func_start __VENEER_OS_JoinThread
__VENEER_OS_JoinThread: ; 0x06008DA8
	ldr pc, _06008DAC ; =OS_JoinThread
	.align 2, 0
_06008DAC: .word OS_JoinThread
	arm_func_end __VENEER_OS_JoinThread
