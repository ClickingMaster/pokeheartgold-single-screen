	.include "asm/macros.inc"
	.include "global.inc"

	.extern SVC_GetPitchTable

.text

	arm_func_start __VENEER_SVC_GetPitchTable
__VENEER_SVC_GetPitchTable: ; 0x037FBD60
	ldr ip, _037FBD68 ; =SVC_GetPitchTable
	bx ip
	.align 2, 0
_037FBD68: .word SVC_GetPitchTable
	arm_func_end __VENEER_SVC_GetPitchTable
