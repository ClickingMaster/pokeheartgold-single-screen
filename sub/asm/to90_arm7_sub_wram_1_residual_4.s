	.include "asm/macros.inc"
	.include "global.inc"

	.extern SVC_GetVolumeTable

.text

	arm_func_start __VENEER_SVC_GetVolumeTable
__VENEER_SVC_GetVolumeTable: ; 0x037FBDE0
	ldr ip, _037FBDE8 ; =SVC_GetVolumeTable
	bx ip
	.align 2, 0
_037FBDE8: .word SVC_GetVolumeTable
	arm_func_end __VENEER_SVC_GetVolumeTable
