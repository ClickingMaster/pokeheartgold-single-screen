	.include "asm/macros.inc"
	.include "global.inc"

	.extern OS_AllocFromHeap

.text

	arm_func_start __VENEER_OS_AllocFromHeap
__VENEER_OS_AllocFromHeap: ; 0x06001D1C
	ldr pc, _06001D20 ; =OS_AllocFromHeap
	.align 2, 0
_06001D20: .word OS_AllocFromHeap
	arm_func_end __VENEER_OS_AllocFromHeap
