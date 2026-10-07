	.include "asm/macros.inc"
	.include "global.inc"

	.extern OS_FreeToHeap

.text

	arm_func_start __VENEER_OS_FreeToHeap
__VENEER_OS_FreeToHeap: ; 0x06001B54
	ldr pc, _06001B58 ; =OS_FreeToHeap
	.align 2, 0
_06001B58: .word OS_FreeToHeap
	arm_func_end __VENEER_OS_FreeToHeap
