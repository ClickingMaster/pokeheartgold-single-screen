	.include "asm/macros.inc"
	.include "global.inc"

	.extern MIi_CpuClear16

.text

	arm_func_start __VENEER_MIi_CpuClear16
__VENEER_MIi_CpuClear16: ; 0x06000598
	ldr pc, _0600059C ; =MIi_CpuClear16
	.align 2, 0
_0600059C: .word MIi_CpuClear16
	arm_func_end __VENEER_MIi_CpuClear16
