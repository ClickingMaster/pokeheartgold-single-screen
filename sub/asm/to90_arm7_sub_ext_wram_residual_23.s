	.include "asm/macros.inc"
	.include "global.inc"

	.extern WMSP_GetAllowedChannel

.text

	arm_func_start __VENEER_WMSP_GetAllowedChannel
__VENEER_WMSP_GetAllowedChannel: ; 0x060084A4
	ldr pc, _060084A8 ; =WMSP_GetAllowedChannel
	.align 2, 0
_060084A8: .word WMSP_GetAllowedChannel
	arm_func_end __VENEER_WMSP_GetAllowedChannel
