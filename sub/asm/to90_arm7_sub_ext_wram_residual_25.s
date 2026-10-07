	.include "asm/macros.inc"
	.include "global.inc"

	.extern MIi_CpuClearFast
	.extern OS_GetLockID

.text

	arm_func_start __VENEER_MIi_CpuClearFast
__VENEER_MIi_CpuClearFast: ; 0x06008D30
	ldr pc, _06008D34 ; =MIi_CpuClearFast
	.align 2, 0
_06008D34: .word MIi_CpuClearFast
	arm_func_end __VENEER_MIi_CpuClearFast

	arm_func_start __VENEER_OS_GetLockID
__VENEER_OS_GetLockID: ; 0x06008D38
	ldr pc, _06008D3C ; =OS_GetLockID
	.align 2, 0
_06008D3C: .word OS_GetLockID
	arm_func_end __VENEER_OS_GetLockID
