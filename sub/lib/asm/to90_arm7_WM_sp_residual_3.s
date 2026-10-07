	.include "asm/macros.inc"
	.include "global.inc"

	.extern MI_CpuCopy8

.text

	arm_func_start __VENEER_MI_CpuCopy8
__VENEER_MI_CpuCopy8: ; 0x06000440
	ldr pc, _06000444 ; =MI_CpuCopy8
	.align 2, 0
_06000444: .word MI_CpuCopy8
	arm_func_end __VENEER_MI_CpuCopy8
