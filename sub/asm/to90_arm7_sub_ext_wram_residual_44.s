	.include "asm/macros.inc"
	.include "global.inc"

	.extern OS_SetIrqFunction

.text

	arm_func_start __VENEER_OS_SetIrqFunction
__VENEER_OS_SetIrqFunction: ; 0x0600D9EC
	ldr pc, _0600D9F0 ; =OS_SetIrqFunction
	.align 2, 0
_0600D9F0: .word OS_SetIrqFunction
	arm_func_end __VENEER_OS_SetIrqFunction
