	.include "asm/macros.inc"
	.include "global.inc"

	.extern SVC_CpuSet

.text

	arm_func_start __VENEER_SVC_CpuSet
__VENEER_SVC_CpuSet: ; 0x03802F74
	ldr ip, _03802F7C ; =SVC_CpuSet
	bx ip
	.align 2, 0
_03802F7C: .word SVC_CpuSet
	arm_func_end __VENEER_SVC_CpuSet
