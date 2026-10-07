	.include "asm/macros.inc"
	.include "global.inc"

	.extern OS_DisableIrqMask
	.extern OS_EnableIrqMask

.text

	arm_func_start __VENEER_OS_DisableIrqMask
__VENEER_OS_DisableIrqMask: ; 0x06008EF4
	ldr pc, _06008EF8 ; =OS_DisableIrqMask
	.align 2, 0
_06008EF8: .word OS_DisableIrqMask
	arm_func_end __VENEER_OS_DisableIrqMask

	arm_func_start __VENEER_OS_EnableIrqMask
__VENEER_OS_EnableIrqMask: ; 0x06008EFC
	ldr pc, _06008F00 ; =OS_EnableIrqMask
	.align 2, 0
_06008F00: .word OS_EnableIrqMask
	arm_func_end __VENEER_OS_EnableIrqMask
