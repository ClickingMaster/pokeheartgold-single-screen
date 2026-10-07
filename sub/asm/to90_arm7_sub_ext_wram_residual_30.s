	.include "asm/macros.inc"
	.include "global.inc"

	.extern MIi_CpuClear32

.text

	arm_func_start __VENEER_MIi_CpuClear32
__VENEER_MIi_CpuClear32: ; 0x0600A9F4
	ldr pc, _0600A9F8 ; =MIi_CpuClear32
	.align 2, 0
_0600A9F8: .word MIi_CpuClear32
	arm_func_end __VENEER_MIi_CpuClear32
