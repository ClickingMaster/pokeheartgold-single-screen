	.include "asm/macros.inc"
	.include "global.inc"

	.extern MIi_CpuCopy32

.text

	arm_func_start __VENEER_MIi_CpuCopy32
__VENEER_MIi_CpuCopy32: ; 0x06011EC8
	ldr pc, _06011ECC ; =MIi_CpuCopy32
	.align 2, 0
_06011ECC: .word MIi_CpuCopy32
	arm_func_end __VENEER_MIi_CpuCopy32
